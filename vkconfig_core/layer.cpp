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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/util.h"

#include <QFile>
#include <QMessageBox>
#include <QJsonArray>

#include <cassert>

// delimited string is a comma delimited string. If value is found remove it
void RemoveString(QString& delimitedString, QString value) {
    // Well, it's not there now is it...
    if (!delimitedString.contains(value)) return;

    QStringList list = delimitedString.split(",");
    for (int i = 0; i < list.size(); i++)
        if (list[i] == value) {
            list.removeAt(i);
            break;
        }

    delimitedString = list.join(",");
}

// Pretty simple, add to list if it's not already in it
void AppendString(QString& delimitedString, QString value) {
    // Do I have anything to do?
    if (delimitedString.contains(value))  // Nope
        return;

    if (!delimitedString.isEmpty()) delimitedString += ",";

    delimitedString += value;
}

enum LayerID {
    LAYER_THIRD_PARTY = -1,
    LAYER_KHRONOS_VALIDATION = 0,
    LAYER_LUNARG_API_DUMP,
    LAYER_LUNARG_DEVICE_SIMULATION,
    LAYER_LUNARG_GFXRECONSTRUCT,
    LAYER_LUNARG_MONITOR,
    LAYER_LUNARG_SCREENSHOT,

    LAYER_FIRST = LAYER_KHRONOS_VALIDATION,
    LAYER_LAST = LAYER_LUNARG_SCREENSHOT
};

enum { LAYER_COUNT = LAYER_LAST - LAYER_FIRST + 1 };

struct DefaultLayer {
    LayerID id;
    const char* name;
    const char* file;
};

static const DefaultLayer default_layers[] = {
    {LAYER_KHRONOS_VALIDATION, "VK_LAYER_KHRONOS_validation", "VkLayer_khronos_validation.json"},
    {LAYER_LUNARG_API_DUMP, "VK_LAYER_LUNARG_api_dump", "VkLayer_api_dump.json"},
    {LAYER_LUNARG_DEVICE_SIMULATION, "VK_LAYER_LUNARG_device_simulation", "VkLayer_device_simulation.json"},
    {LAYER_LUNARG_GFXRECONSTRUCT, "VK_LAYER_LUNARG_gfxreconstruct", "VkLayer_gfxreconstruct.json"},
    {LAYER_LUNARG_MONITOR, "VK_LAYER_LUNARG_monitor", "VkLayer_monitor.json"},
    {LAYER_LUNARG_SCREENSHOT, "VK_LAYER_LUNARG_screenshot", "VkLayer_screenshot.json"}};
static_assert(countof(default_layers) == LAYER_COUNT, "The tranlation table size doesn't match the enum number of elements");

LayerID GetLayerID(const QString& name) {
    assert(!name.isEmpty());

    for (std::size_t i = 0, n = countof(default_layers); i < n; ++i) {
        if (default_layers[i].name == name) return default_layers[i].id;
    }

    return LAYER_THIRD_PARTY;
}

// TODO: add latest, add all layer versions
QString GetBuiltinFolder(const Version& version) {
    if (version <= Version(1, 2, 148))
        return "layers_1_2_148/";
    else
        return "layers_1_2_154/";
    /*
        if (version <= Version(1, 2, 148))
            return "layers_1_2_148/";
        else if (version == Version(1, 2, 154))
            return "layers_1_2_154/";
        else
            return "layers_latest/";
    */
}

Layer::Layer() {}

Layer::Layer(const QString& name, const LayerType layer_type) : name(name), _layer_type(layer_type) {}

Layer::Layer(const QString& name, const LayerType layer_type, const Version& file_format_version, const Version& api_version,
             const QString& implementation_version, const QString& library_path, const QString& type)
    : name(name),
      _layer_type(layer_type),
      _type(type),
      _file_format_version(file_format_version),
      _api_version(api_version),
      _implementation_version(implementation_version),
      _library_path(library_path),
      _default_preset_index(PRESET_INDEX_USER_DEFINED) {}

// Todo: Load the layer with Vulkan API
bool Layer::IsValid() const {
    return _file_format_version != Version::VERSION_NULL && !name.isEmpty() && !_type.isEmpty() && !_library_path.isEmpty() &&
           _api_version != Version::VERSION_NULL && !_implementation_version.isEmpty();
}

bool Layer::LoadLayerSettingsDefault(const QString& layer_name, const Version& api_version, std::vector<LayerSetting>& settings) {
    assert(!layer_name.isEmpty());
    assert(api_version != Version::VERSION_NULL);

    const LayerID layer_id = GetLayerID(layer_name);
    if (layer_id == LAYER_THIRD_PARTY) return false;

    Layer default_layer;
    const bool result = default_layer.Load(
        QString(":/resourcefiles/") + GetBuiltinFolder(api_version) + default_layers[layer_id].file, _layer_type);
    assert(result);

    _default_preset_index = default_layer._default_preset_index;
    settings = default_layer.settings;
    presets = default_layer.presets;

    return true;
}

bool Layer::Load(QString full_path_to_file, LayerType layer_type) {
    static const Version version_1_3_0(1, 3, 0);

    assert(!full_path_to_file.isEmpty());

    _layer_path = full_path_to_file;
    _layer_type = layer_type;  // Set layer type, no way to know this from the json file

    QFile file(full_path_to_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    const QString json_text = file.readAll();
    file.close();

    QJsonParseError parse_error;
    const QJsonDocument& json_doc = QJsonDocument::fromJson(json_text.toUtf8(), &parse_error);
    if (parse_error.error != QJsonParseError::NoError || json_doc.isNull() || json_doc.isEmpty()) {
        return false;
    }

    const QJsonObject json_object = json_doc.object();

    const QJsonValue json_value_layer = json_object.value("layer");
    if (json_value_layer == QJsonValue::Undefined)  // This is not a layer JSON file
        return false;

    const QJsonValue json_value_file_format_version = json_object.value("file_format_version");
    _file_format_version = Version(json_value_file_format_version.toString());

    const QJsonObject json_object_layer = json_value_layer.toObject();

    {
        const QJsonValue value = json_object_layer.value("name");
        assert(value != QJsonValue::Undefined);
        name = value.toString();
    }
    {
        const QJsonValue value = json_object_layer.value("type");
        assert(value != QJsonValue::Undefined);
        _type = value.toString();
    }
    {
        const QJsonValue value = json_object_layer.value("library_path");
        if (value != QJsonValue::Undefined) _library_path = value.toString();
    }
    {
        const QJsonValue value = json_object_layer.value("api_version");
        assert(value != QJsonValue::Undefined);
        _api_version = Version(value.toString());
    }
    {
        const QJsonValue value = json_object_layer.value("implementation_version");
        assert(value != QJsonValue::Undefined);
        _implementation_version = value.toString();
    }
    {
        const QJsonValue value = json_object_layer.value("description");
        assert(value != QJsonValue::Undefined);
        _description = value.toString();
    }
    {
        const QJsonValue value = json_object_layer.value("settings");
        assert((_file_format_version < version_1_3_0 && value == QJsonValue::Undefined) ||
               (_file_format_version >= version_1_3_0 && value != QJsonValue::Undefined));
        if (value != QJsonValue::Undefined) {
            LoadLayerSettings(value, settings);
        } else if (!full_path_to_file.startsWith(":/resourcefiles/")) {
            // When the path start with :/resourcefiles/ we already called LoadLayerSettingsDefault
            LoadLayerSettingsDefault(name, _api_version, settings);
        }
    }
    {
        const QJsonValue value = json_object_layer.value("default_preset_index");
        assert((_file_format_version < version_1_3_0 && value == QJsonValue::Undefined) ||
               (_file_format_version >= version_1_3_0 && value != QJsonValue::Undefined));
        if (value != QJsonValue::Undefined) _default_preset_index = value.toInt();
    }
    {
        const QJsonValue value = json_object_layer.value("presets");
        if (value != QJsonValue::Undefined) {
            LoadLayerPresets(value, presets);
        }
    }

    return IsValid();
}
