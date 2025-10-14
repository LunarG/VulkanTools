/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "generate_layers_settings_markdown.h"
#include "vulkan_util.h"
#include "setting_flags.h"
#include "layer.h"
#include "configuration.h"

#include <vector>

static std::string BuildArchor(const std::string& label) {
    std::string tmp = ToLowerCase(label);
    std::replace(tmp.begin(), tmp.end(), ' ', '-');
    return tmp;
}

static std::string GetTabs(std::size_t depth) {
    std::string result;
    for (std::size_t i = 0, n = depth; i < n; ++i) {
        result += "&nbsp;&nbsp;&nbsp;&nbsp;";
    }
    return result;
}

static std::string BuildPlatformsMarkdown(int platform_flags) {
    std::string text;

    const std::vector<std::string>& platforms = GetPlatformTokens(platform_flags);
    for (std::size_t i = 0, n = platforms.size(); i < n; ++i) {
        text += "`" + platforms[i] + "`";
        if (i < platforms.size() - 1) {
            text += ", ";
        }
    }

    return text;
}

static void WriteSettingsOverviewMarkdown(std::string& text, const Layer& layer, const SettingMetaSet& settings,
                                          std::size_t depth) {
    for (std::size_t i = 0, n = settings.size(); i < n; ++i) {
        const SettingMeta* setting = settings[i];

        if (setting->view == SETTING_VIEW_STANDARD) {
            text += "| ";
            text += ::GetTabs(depth) + "[" + setting->label + "](#" + ::BuildArchor(setting->label) + ") | ";
            text += "`" + setting->key + "` | ";
            text += std::string("`") + GetToken(setting->type) + std::string("` | ");
            text += std::string("`") + setting->Export(EXPORT_MODE_DOC) + "` | ";
            text += BuildPlatformsMarkdown(setting->platform_flags) + " |\n";
        }

        if (IsEnum(setting->type) || IsFlags(setting->type)) {
            const SettingMetaEnumeration& setting_enum = static_cast<const SettingMetaEnumeration&>(*setting);
            for (std::size_t j = 0, o = setting_enum.enum_values.size(); j < o; ++j) {
                WriteSettingsOverviewMarkdown(text, layer, setting_enum.enum_values[j].settings, depth + 1);
            }
        }

        WriteSettingsOverviewMarkdown(text, layer, setting->children, depth + 1);
    }
}

static void WriteSettingsDetailsMarkdown(std::string& text, const Layer& layer, const SettingMetaSet& settings) {
    for (std::size_t i = 0, n = settings.size(); i < n; ++i) {
        const SettingMeta* setting = settings[i];

        if (setting->view == SETTING_VIEW_STANDARD) {
            if (setting->status == STATUS_STABLE) {
                text += "### " + setting->label + "\n";
            } else {
                text += "### " + setting->label + "(" + GetToken(setting->status) + ")\n";
            }
            text += "\n";

            if (!setting->description.empty()) {
                text += setting->description;

                const std::string trim_desc = ::TrimSurroundingWhitespace(setting->description);
                if (trim_desc[trim_desc.size() - 1] != '.') {
                    text += ".";
                }

                text += "\n\n";
            }

            if (!setting->detailed.empty()) {
                text += setting->detailed;
                const std::string trim_detailed = ::TrimSurroundingWhitespace(setting->detailed);
                if (trim_detailed[trim_detailed.size() - 1] != '.') {
                    text += ".";
                }

                text += "\n\n";
            }

            if (!setting->url.Empty()) {
                const std::string url = ConvertStandardSeparators(setting->url.AbsolutePath());
                text += format("Learn more about the *[%s](%s)* setting.\n", setting->label.c_str(), url.c_str());
                text += "\n";
            }

            if (setting->status == STATUS_DEPRECATED && !setting->deprecated_by_key.empty()) {
                const SettingMeta* replaced_setting = FindSetting(layer.settings, setting->deprecated_by_key.c_str());

                text += "This setting was deprecated and replaced by [" + replaced_setting->label + "](#" +
                        ::BuildArchor(setting->label) + " setting).\n";

                text += "\n";
            }

            if (!setting->dependence.empty()) {
                if (setting->dependence.size() > 1) {
                    switch (setting->dependence_mode) {
                        case DEPENDENCE_NONE:
                            text += "\n";
                            break;
                        case DEPENDENCE_ALL:
                            text += "All the following condition must be fulfilled for the setting to be applied.\n";
                            break;
                        case DEPENDENCE_ANY:
                            text += "Any of the following condition must be fulfilled for the setting to be applied.\n";
                            break;
                    }
                    text += "\n";
                }

                text += "| Dependences | Variables Key | Type | Value |\n";
                text += "| --- | --- | --- | --- |\n";
                for (std::size_t i = 0, n = setting->dependence.size(); i < n; ++i) {
                    const SettingMeta* setting_dep = ::FindSetting(layer.settings, setting->dependence[i]->key.c_str());
                    if (setting_dep == nullptr) {
                        continue;
                    }

                    text += "| ";
                    text += "[" + setting_dep->label + "](#" + ::BuildArchor(setting_dep->label) + ") | ";
                    text += "`" + setting_dep->key + "` | ";
                    text += GetToken(setting_dep->type) + std::string(" | ");
                    text += setting->dependence[i]->Export(EXPORT_MODE_DOC) + " |\n";
                }
                text += "\n";
            }

            if (!setting->children.empty()) {
                if (setting->children.size() == 1) {
                    text += "Sub-setting: ";
                } else {
                    text += "Sub-settings: ";
                }

                for (std::size_t i = 0, n = setting->children.size(); i < n; ++i) {
                    SettingMeta* child = setting->children[i];
                    text += format("*[%s](#%s)*", child->label.c_str(), ::BuildArchor(child->label).c_str());
                    if (i < n - 2) {
                        text += ", ";
                    } else if (i < n - 1) {
                        text += ", and ";
                    } else {
                        text += ".\n";
                    }
                }
                text += "\n";
            }

            {
                text += format("- Key: `%s`\n", setting->key.c_str());
                text += format("- Type: `%s`\n", GetToken(setting->type));
                if (setting->type != SETTING_GROUP) {
                    std::string defeault_value = setting->Export(EXPORT_MODE_DOC);
                    if (defeault_value.empty()) {
                        text += "- Default Value:\n";
                    } else {
                        text += format("- Default Value: `%s`\n", setting->Export(EXPORT_MODE_DOC).c_str());
                    }
                }
                text += "- Platforms: " + BuildPlatformsMarkdown(setting->platform_flags) + "\n";
                text += "\n";
            }

            if (IsEnum(setting->type) || IsFlags(setting->type)) {
                const SettingMetaEnumeration& setting_enum = static_cast<const SettingMetaEnumeration&>(*setting);
                if (setting_enum.enum_values.size() > 0) {
                    if (IsEnum(setting->type)) {
                        text += "|Enum Value|Label|Description|Platforms|\n";
                    } else {
                        // Flags
                        text += "|Flags|Label|Description|Platforms|\n";
                    }
                    text += "|---|---|---|---|\n";
                    for (std::size_t j = 0, o = setting_enum.enum_values.size(); j < o; ++j) {
                        const SettingEnumValue& value = setting_enum.enum_values[j];

                        if (value.view != SETTING_VIEW_STANDARD) {
                            continue;
                        }

                        text += "| `" + value.key + "` |" + value.label + "|";
                        if (value.description.empty()) {
                            text += "N/A|";
                        } else if (value.detailed.empty()) {
                            text += value.description + "|";
                        } else {
                            text += value.description + value.detailed + "|";
                        }
                        text += BuildPlatformsMarkdown(value.platform_flags) + "|\n";
                    }
                    text += "\n";
                }
            }

            if (setting->type != SETTING_GROUP) {
                text += "| Methods: | |\n";
                text += "| --- | --- |\n";

                {
                    const char* string_char = IsString(setting->type) ? "\"" : "";
                    std::string default_value = setting->Export(EXPORT_MODE_DOC);
                    if (setting->type == SETTING_BOOL) {
                        if (default_value == "true") {
                            default_value = "VK_TRUE";
                        }
                        if (default_value == "false") {
                            default_value = "VK_FALSE";
                        }
                    }

                    text += "| `VK_EXT_layer_settings`: | ";
                    text += format("<pre>%s data[] = { %s%s%s };<br>", ::GetCodeTypeString(setting->type), string_char,
                                   default_value.c_str(), string_char);
                    text += "VkLayerSettingEXT setting {<br>";
                    text += format("&emsp;\"%s\", \"%s\", %s,<br>", layer.key.c_str(), setting->key.c_str(),
                                   ::GetLayerSettingTypeString(setting->type));
                    text += "&emsp;static_cast<uint32_t>(std::size(data)), data };</pre> |\n";
                }

                text += format("| `vk_layer_settings.txt`: | <pre>%s = %s</pre> |\n",
                               (GetLayerSettingPrefix(layer.key) + setting->key).c_str(), setting->Export(EXPORT_MODE_DOC).c_str());

                {
                    std::vector<std::string> envs = BuildEnvVariablesList(layer.key.c_str(), setting->key.c_str());
                    if (!setting->env.empty()) {
                        envs.push_back(setting->env);
                    }

                    text += "| Environment variables: | <pre>";
                    for (std::size_t i = 0, n = envs.size(); i < n; ++i) {
                        std::string value = setting->Export(EXPORT_MODE_DOC);
                        if (!value.empty()) {  // Wordaround for Vulkan Configurator variables
                            if (value[0] == '$') {
                                value = "\\" + value;
                            }
                        }
                        text += format("export %s=%s", envs[i].c_str(), value.c_str());
                        if (i < n - 1) {
                            text += "<br>";
                        }
                    }

                    text += "</pre> |\n";
                }

                if (setting->platform_flags & PLATFORM_ANDROID_BIT) {
                    std::vector<std::string> commands = BuildEnvVariablesList(layer.key.c_str(), setting->key.c_str(), true);

                    text += "| Android system properties: | <pre>";

                    for (std::size_t i = 0, n = commands.size(); i < n; ++i) {
                        text += format("adb setprop %s %s", commands[i].c_str(), setting->Export(EXPORT_MODE_DOC).c_str());
                        if (i < n - 1) {
                            text += "<br>";
                        }
                    }

                    if (!layer.prefix.empty()) {
                        text += format("<br>adb setprop debug.%s %s", (layer.prefix + "." + setting->key).c_str(),
                                       setting->Export(EXPORT_MODE_DOC).c_str());
                    }

                    text += "</pre> |\n";
                }
                text += "\n";
            }

            if (IsEnum(setting->type) || IsFlags(setting->type)) {
                const SettingMetaEnumeration& setting_enum = static_cast<const SettingMetaEnumeration&>(*setting);
                for (std::size_t j = 0, o = setting_enum.enum_values.size(); j < o; ++j) {
                    ::WriteSettingsDetailsMarkdown(text, layer, setting_enum.enum_values[j].settings);
                }
            }
        }

        ::WriteSettingsDetailsMarkdown(text, layer, setting->children);
    }
}

std::string GenerateSettingsMarkdown::operator()(const LayerManager& layers, const Configuration* configuration) const {
    std::string text;

    for (std::size_t parameter_index = 0, parameter_count = configuration->parameters.size(); parameter_index < parameter_count;
         ++parameter_index) {
        const Parameter& parameter = configuration->parameters[parameter_index];

        if (parameter.builtin == LAYER_BUILTIN_UNORDERED) {
            continue;
        }

        if (parameter.control == LAYER_CONTROL_OFF || parameter.control == LAYER_CONTROL_DISCARD) {
            continue;
        }

        const Layer* layer = layers.FindFromManifest(parameter.manifest);
        if (layer == nullptr) {
            continue;
        }

        // Title
        {
            if (layer->url.Empty()) {
                text += format("# %s", layer->key.c_str());
            } else {
                text += format("# [%s](%s)", layer->key.c_str(), layer->url.AbsolutePath(false).c_str());
            }

            if (layer->status != STATUS_STABLE) {
                text += format(" (%s)", GetToken(layer->status));
            }

            text += "\n\n";
        }

        if (!layer->description.empty()) {
            text += "## " + layer->description + "\n";
            text += "\n";
        }

        if (!layer->introduction.empty()) {
            text += layer->introduction + "\n";
            text += "\n";
        }

        text += "## Layer Properties\n";
        text += "\n";
        text += "- API Version: " + layer->api_version.str() + "\n";
        text += "- Implementation Version: " + layer->implementation_version + "\n";
        text += "- Layer Manifest: `" + QFileInfo(layer->manifest_path.RelativePath().c_str()).fileName().toStdString() + "`\n";
        text += "  - File Format: " + layer->file_format_version.str() + "\n";
        text += "  - Layer Binary: `";
        text += (::ConvertStandardSeparators(layer->binary_path.RelativePath()).rfind("./", 0) == 0
                     ? ::ConvertStandardSeparators(layer->binary_path.RelativePath()).substr(2)
                     : ::ConvertStandardSeparators(layer->binary_path.RelativePath())) +
                "`\n";
        if (layer->platforms != 0) {
            text += "- Platforms: " + BuildPlatformsMarkdown(layer->platforms) + "\n";
        }
        if (layer->status != STATUS_STABLE) {
            text += format(" - Status: %s\n", GetToken(layer->status));
        }

        /* TODO
                text += "- Variables:\n";
                text += format("  - `vk_layer_settings.txt` namespace: `%s`\n", ToLowerCase(TrimPrefix(layer->key)).c_str());
                text += format("  - Environment Variable prefix: `VK_%s_`\n", ToUpperCase(TrimPrefix(layer->key)).c_str());
                text += format("  - Android system property prefix: `adb setprop debug.vulkan.%s.`\n",
                               ToLowerCase(TrimPrefix(layer->key)).c_str());

                if (!layer->settings.empty()) {
                    text += format("- Number of Layer Settings: %d\n", ::CountSettings(layer->settings, true));
                }
                if (!layer->presets.empty()) {
                    text += format("- Number of Layer Presets: %d\n", layer->presets.size());
                }
                text += "\n";
        */

        if (!layer->settings.empty()) {
            text += "## Layer Settings Overview\n";
            text += "\n";
            text += "|Label|Variables Key|Type|Default Value|Platforms|\n";
            text += "|---|---|---|---|---|\n";
            WriteSettingsOverviewMarkdown(text, *layer, layer->settings, 0);
            text += "\n";

            text += "## Layer Settings Details\n";
            text += "\n";
            WriteSettingsDetailsMarkdown(text, *layer, layer->settings);
        }

        if (!layer->presets.empty()) {
            text += "## Layer Presets\n";
            text += "\n";
            for (std::size_t i = 0, n = layer->presets.size(); i < n; ++i) {
                const LayerPreset& preset = layer->presets[i];

                text += "### " + preset.label + "\n";
                text += "\n";
                text += preset.description + "\n";
                text += "\n";
                text += "#### Preset Setting Values:\n";
                for (std::size_t j = 0, o = preset.settings.size(); j < o; ++j) {
                    const SettingData* data = preset.settings[j];
                    const SettingMeta* meta = FindSetting(layer->settings, data->key.c_str());

                    text += "- " + meta->label + ": " + data->Export(EXPORT_MODE_DOC).c_str() + "\n";
                }
                text += "\n";
            }
        }
    }

    return text;
}

bool ExportSettingsMarkdown(const LayerManager& layers, const Configuration* configuration, const Path& output_path) {
    GenerateSettingsMarkdown generate_settings;

    std::string data = generate_settings(layers, configuration);

    QFile file(output_path.AbsolutePath().c_str());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(data.c_str());
        file.close();
        printf("vkconfig: Markdown file written to %s\n", output_path.AbsolutePath().c_str());
        return true;
    } else {
        printf("vkconfig: Could not write %s\n", output_path.AbsolutePath().c_str());
        return false;
    }
}
