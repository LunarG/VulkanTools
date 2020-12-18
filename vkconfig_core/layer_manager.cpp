/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authors:
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "layer_manager.h"
#include "util.h"
#include "platform.h"
#include "registry.h"

#include <QSettings>
#include <QDir>
#include <QStringList>

/// Going back and forth between the Windows registry and looking for files
/// in specific folders is just a mess. This class consolidates all that into
/// one single abstraction that knows whether to look in the registry or in
/// a folder with QDir.
/// This is a little weird because generally QSettings is for going back
/// and forth between the Registry or .ini files. Here, I'm going from
/// the registry to directory entries.
class PathFinder {
   private:
    QStringList files;

   public:
    PathFinder() {}

    // Constructor does all the work. Abstracts away instances where we might
    // be searching a disk path, or a registry path.
    // TBD, does this really need it's own file/module?
    PathFinder(const QString &path, bool force_file_system = (VKC_PLATFORM != VKC_PLATFORM_WINDOWS)) {
        if (!force_file_system) {
            QSettings settings(path, QSettings::NativeFormat);
            files = settings.allKeys();
        } else {
            QDir dir(path);
            QFileInfoList file_info_list = dir.entryInfoList(QStringList() << "*.json", QDir::Files);

            for (int file_index = 0; file_index < file_info_list.size(); ++file_index) {
                files << file_info_list[file_index].filePath();
            }
        }
    }

    int FileCount() const { return files.size(); }
    QString GetFileName(int i) const { return files[i]; }
};

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
static const QString SEARCH_PATHS[] = {"HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ExplicitLayers",
                                       "HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ImplicitLayers",
                                       "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ExplicitLayers",
                                       "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers",
                                       "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers",
                                       "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers"};
#else
static const QString SEARCH_PATHS[] = {"/usr/local/etc/vulkan/explicit_layer.d",  // Not used on macOS, okay to just ignore
                                       "/usr/local/etc/vulkan/implicit_layer.d",  // Not used on macOS, okay to just ignore
                                       "/usr/local/share/vulkan/explicit_layer.d",
                                       "/usr/local/share/vulkan/implicit_layer.d",
                                       "/etc/vulkan/explicit_layer.d",
                                       "/etc/vulkan/implicit_layer.d",
                                       "/usr/share/vulkan/explicit_layer.d",
                                       "/usr/share/vulkan/implicit_layer.d",
                                       ".local/share/vulkan/explicit_layer.d",
                                       ".local/share/vulkan/implicit_layer.d"};
#endif

LayerManager::LayerManager(const Environment &environment) : environment(environment) {
    available_layers.reserve(10);

    // See if the VK_LAYER_PATH environment variable is set. If so, parse it and
    // assemble a list of paths that take precidence for layer discovery.
    QString layer_path = qgetenv("VK_LAYER_PATH");
    if (!layer_path.isEmpty()) {
        VK_LAYER_PATH = layer_path.split(GetPlatformString(PLATFORM_STRING_SEPARATOR));
    }
}

void LayerManager::Clear() { available_layers.clear(); }

bool LayerManager::Empty() const { return available_layers.empty(); }

// Find all installed layers on the system.
void LayerManager::LoadAllInstalledLayers() {
    available_layers.clear();

    // FIRST: If VK_LAYER_PATH is set it has precedence over other layers.
    for (int i = 0, n = VK_LAYER_PATH.count(); i < n; ++i) {
        LoadLayersFromPath(VK_LAYER_PATH[i], available_layers);
    }

    // SECOND: Any custom paths? Search for those too
    const QStringList &custom_layers_paths = environment.GetCustomLayerPaths();
    for (int i = 0, n = custom_layers_paths.size(); i < n; ++i) {
        LoadLayersFromPath(custom_layers_paths[i], available_layers);
    }

    // THIRD: Standard layer paths, in standard locations. The above has always taken precedence.
    for (std::size_t i = 0, n = countof(SEARCH_PATHS); i < n; i++) {
        LoadLayersFromPath(SEARCH_PATHS[i], available_layers);
    }

    // FOURTH: Finally, see if thee is anyting in the VULKAN_SDK path that wasn't already found elsewhere
    const QString vulkan_sdk(qgetenv("VULKAN_SDK"));
    if (!vulkan_sdk.isEmpty()) {
        LoadLayersFromPath(vulkan_sdk + GetPlatformString(PLATFORM_STRING_EXPLICIT_LAYERS), available_layers);
    }
}

/// Search a folder and load up all the layers found there. This does NOT
/// load the default settings for each layer. This is just a master list of
/// layers found. Do NOT load duplicate layer names. The type of layer (explicit or implicit) is
/// determined from the path name.
void LayerManager::LoadLayersFromPath(const QString &path, std::vector<Layer> &layers) {
    // On Windows custom files are in the file system. On non Windows all layers are
    // searched this way
    LayerType type = LAYER_TYPE_CUSTOM;
    if (path.contains("explicit", Qt::CaseInsensitive)) type = LAYER_TYPE_EXPLICIT;
    if (path.contains("implicit", Qt::CaseInsensitive)) type = LAYER_TYPE_IMPLICIT;

    PathFinder file_list;

    if (VKC_PLATFORM == VKC_PLATFORM_WINDOWS) {
        if (path.contains("...")) {
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
            LoadRegistryLayers(path, layers, type);
#endif
            return;
        }

        file_list = PathFinder(path, (type == LAYER_TYPE_CUSTOM));
    } else if (VKC_PLATFORM == VKC_PLATFORM_LINUX || VKC_PLATFORM == VKC_PLATFORM_MACOS) {
        // On Linux/Mac, we also need the home folder
        QString search_path = path;
        if (path[0] == '.') {
            search_path = QDir().homePath() + "/" + path;
        }

        file_list = PathFinder(search_path, true);
    } else {
        assert(0);  // Platform
    }

    for (int i = 0, n = file_list.FileCount(); i < n; ++i) {
        Layer layer;
        if (layer.Load(file_list.GetFileName(i), type)) {
            if (layer.key == "VK_LAYER_LUNARG_override") continue;

            // Make sure this layer name has not already been added
            if (FindByKey(available_layers, layer.key.c_str()) != nullptr) continue;

            // Good to go, add the layer
            layers.push_back(layer);
        }
    }
}
