/*
 * Copyright (c) 2020-2022 Valve Corporation
 * Copyright (c) 2020-2022 LunarG, Inc.
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
    PathFinder(const std::string &path, bool force_file_system = (VKC_PLATFORM != VKC_PLATFORM_WINDOWS)) {
        if (!force_file_system) {
            QSettings settings(path.c_str(), QSettings::NativeFormat);
            files = settings.allKeys();
        } else {
            QDir dir(path.c_str());
            QFileInfoList file_info_list = dir.entryInfoList(QStringList() << "*.json", QDir::Files);

            for (int file_index = 0; file_index < file_info_list.size(); ++file_index) {
                files << file_info_list[file_index].filePath();
            }
        }
    }

    int FileCount() const { return files.size(); }
    std::string GetFileName(int i) const { return files[i].toStdString(); }
};

#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
static const char *SEARCH_PATHS[] = {"HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ExplicitLayers",
                                     "HKEY_LOCAL_MACHINE\\Software\\Khronos\\Vulkan\\ImplicitLayers",
                                     "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ExplicitLayers",
                                     "HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers",
                                     "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers",
                                     "HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers"};
#else
static const char *SEARCH_PATHS[] = {"/usr/local/etc/vulkan/explicit_layer.d",  // Not used on macOS, okay to just ignore
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

LayerManager::LayerManager(const Environment &environment) : environment(environment) { available_layers.reserve(10); }

void LayerManager::Clear() { available_layers.clear(); }

bool LayerManager::Empty() const { return available_layers.empty(); }

// Find all installed layers on the system.
void LayerManager::LoadAllInstalledLayers() {
    available_layers.clear();

    // FIRST: If VK_LAYER_PATH is set it has precedence over other layers.
    const std::vector<std::string> &env_user_defined_layers_paths_set =
        environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_ENV_SET);
    for (std::size_t i = 0, n = env_user_defined_layers_paths_set.size(); i < n; ++i) {
        LoadLayersFromPath(env_user_defined_layers_paths_set[i]);
    }

    // SECOND: Any per layers configuration user-defined path from Vulkan Configurator? Search for those too
    const std::vector<std::string> &gui_config_user_defined_layers_paths =
        environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI);
    for (std::size_t i = 0, n = gui_config_user_defined_layers_paths.size(); i < n; ++i) {
        LoadLayersFromPath(gui_config_user_defined_layers_paths[i]);
    }

    // THIRD: Add VK_ADD_LAYER_PATH layers
    const std::vector<std::string> &env_user_defined_layers_paths_add =
        environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_ENV_ADD);
    for (std::size_t i = 0, n = env_user_defined_layers_paths_add.size(); i < n; ++i) {
        LoadLayersFromPath(env_user_defined_layers_paths_add[i]);
    }

    // FOURTH: Standard layer paths, in standard locations. The above has always taken precedence
    for (std::size_t i = 0, n = countof(SEARCH_PATHS); i < n; i++) {
        LoadLayersFromPath(SEARCH_PATHS[i]);
    }

    // FIFTH: See if thee is anyting in the VULKAN_SDK path that wasn't already found elsewhere
    if (!qgetenv("VULKAN_SDK").isEmpty()) {
        LoadLayersFromPath(GetPath(BUILTIN_PATH_EXPLICIT_LAYERS));
    }
}

// Load a single layer
void LayerManager::LoadLayer(const std::string &layer_name) {
    available_layers.clear();

    // FIRST: If VK_LAYER_PATH is set it has precedence over other layers.
    const std::vector<std::string> &env_user_defined_layers_paths_set =
        environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_ENV_SET);
    for (std::size_t i = 0, n = env_user_defined_layers_paths_set.size(); i < n; ++i) {
        if (LoadLayerFromPath(layer_name, env_user_defined_layers_paths_set[i])) return;
    }

    // SECOND: Any per layers configuration user-defined path from Vulkan Configurator? Search for those too
    const std::vector<std::string> &gui_config_user_defined_layers_paths =
        environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_GUI);
    for (std::size_t i = 0, n = gui_config_user_defined_layers_paths.size(); i < n; ++i) {
        if (LoadLayerFromPath(layer_name, gui_config_user_defined_layers_paths[i])) return;
    }

    // THIRD: Add VK_ADD_LAYER_PATH layers
    const std::vector<std::string> &env_user_defined_layers_paths_add =
        environment.GetUserDefinedLayersPaths(USER_DEFINED_LAYERS_PATHS_ENV_ADD);
    for (std::size_t i = 0, n = env_user_defined_layers_paths_add.size(); i < n; ++i) {
        if (LoadLayerFromPath(layer_name, env_user_defined_layers_paths_add[i])) return;
    }

    // FOURTH: Standard layer paths, in standard locations. The above has always taken precedence
    for (std::size_t i = 0, n = countof(SEARCH_PATHS); i < n; i++) {
        if (LoadLayerFromPath(layer_name, SEARCH_PATHS[i])) return;
    }

    // FIFTH: See if thee is anyting in the VULKAN_SDK path that wasn't already found elsewhere
    if (!qgetenv("VULKAN_SDK").isEmpty()) {
        if (LoadLayerFromPath(layer_name, GetPath(BUILTIN_PATH_EXPLICIT_LAYERS))) return;
    }
}

/// Search a folder and load up all the layers found there. This does NOT
/// load the default settings for each layer. This is just a master list of
/// layers found. Do NOT load duplicate layer names. The type of layer (explicit or implicit) is
/// determined from the path name.
void LayerManager::LoadLayersFromPath(const std::string &path) {
    // On Windows custom files are in the file system. On non Windows all layers are
    // searched this way
    LayerType type = LAYER_TYPE_USER_DEFINED;
    if (QString(path.c_str()).contains("explicit", Qt::CaseInsensitive)) type = LAYER_TYPE_EXPLICIT;
    if (QString(path.c_str()).contains("implicit", Qt::CaseInsensitive)) type = LAYER_TYPE_IMPLICIT;

    PathFinder file_list;

    if (VKC_PLATFORM == VKC_PLATFORM_WINDOWS) {
        if (QString(path.c_str()).contains("...")) {
#if VKC_PLATFORM == VKC_PLATFORM_WINDOWS
            LoadRegistryLayers(path.c_str(), available_layers, type);
#endif
            return;
        }

        file_list = PathFinder(path, (type == LAYER_TYPE_USER_DEFINED));
    } else if (VKC_PLATFORM == VKC_PLATFORM_LINUX || VKC_PLATFORM == VKC_PLATFORM_MACOS) {
        // On Linux/Mac, we also need the home folder
        std::string search_path = path;
        if (path[0] == '.') {
            search_path = QDir().homePath().toStdString() + "/" + path;
        }

        file_list = PathFinder(search_path, true);
    } else {
        assert(0);  // Platform unknown
    }

    for (int i = 0, n = file_list.FileCount(); i < n; ++i) {
        Layer layer;
        if (layer.Load(available_layers, file_list.GetFileName(i).c_str(), type)) {
            // Make sure this layer name has not already been added
            if (FindByKey(available_layers, layer.key.c_str()) != nullptr) continue;

            // Good to go, add the layer
            available_layers.push_back(layer);
        }
    }
}

// Attempt to load the named layer from the given path
bool LayerManager::LoadLayerFromPath(const std::string &layer_name, const std::string &path) {
    LayerType type = LAYER_TYPE_USER_DEFINED;
    if (QString(path.c_str()).contains("explicit", Qt::CaseInsensitive)) type = LAYER_TYPE_EXPLICIT;
    if (QString(path.c_str()).contains("implicit", Qt::CaseInsensitive)) type = LAYER_TYPE_IMPLICIT;

    PathFinder file_list;

    if (VKC_PLATFORM == VKC_PLATFORM_LINUX || VKC_PLATFORM == VKC_PLATFORM_MACOS) {
        // On Linux/Mac, we also need the home folder
        std::string search_path = path;
        if (path[0] == '.') {
            search_path = QDir().homePath().toStdString() + "/" + path;
        }
        file_list = PathFinder(search_path, true);
    } else {
        file_list = PathFinder(path, true);
    }

    for (int i = 0, n = file_list.FileCount(); i < n; ++i) {
        Layer layer;
        if (layer.Load(available_layers, file_list.GetFileName(i).c_str(), type)) {
            // Add this layer if the layer name matches, then return
            if (layer_name == layer.key) {
                available_layers.push_back(layer);
                return true;
            }
        }
    }
    return false;
}
