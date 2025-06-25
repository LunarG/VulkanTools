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

#include "doc.h"
#include "setting_flags.h"
#include "configurator.h"
#include "vulkan_util.h"

#include <QFileInfo>

static std::string BuildPlatformsHtml(int platform_flags) {
    std::string text;

    const std::vector<std::string>& platforms = GetPlatformTokens(platform_flags);
    for (std::size_t i = 0, n = platforms.size(); i < n; ++i) {
        text += "<span class=\"code\">" + platforms[i] + "</ span>";
        if (i < n - 1) {
            text += ", ";
        }
    }

    return text;
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

static std::string GetTabs(std::size_t depth) {
    std::string result;
    for (std::size_t i = 0, n = depth; i < n; ++i) {
        result += "&nbsp;&nbsp;&nbsp;&nbsp;";
    }
    return result;
}

static void WriteSettingsOverviewHtml(std::string& text, const Layer& layer, const SettingMetaSet& settings, std::size_t depth) {
    for (std::size_t i = 0, n = settings.size(); i < n; ++i) {
        const SettingMeta* setting = settings[i];

        if (setting->view == SETTING_VIEW_STANDARD) {
            text += "<tr>\n";
            text += format("\t<td>%s<a id=\"%s\" href=\"#%s-detailed\">%s</a></td>\n", ::GetTabs(depth).c_str(),
                           setting->key.c_str(), setting->key.c_str(), setting->label.c_str());
            text += format("\t<td><span class=\"code\">%s</span></td>\n", setting->key.c_str());
            text += format("\t<td><span class=\"code\">%s</span></td>\n", GetToken(setting->type));
            text += format("\t<td><span class=\"code\">%s</span></td>\n", setting->Export(EXPORT_MODE_DOC).c_str());

            text += format("\t<td>%s</td>\n", BuildPlatformsHtml(setting->platform_flags).c_str());

            text += "</tr>\n";
        }

        if (IsEnum(setting->type) || IsFlags(setting->type)) {
            const SettingMetaEnumeration& setting_enum = static_cast<const SettingMetaEnumeration&>(*setting);
            for (std::size_t j = 0, o = setting_enum.enum_values.size(); j < o; ++j) {
                WriteSettingsOverviewHtml(text, layer, setting_enum.enum_values[j].settings, depth + 1);
            }
        }

        WriteSettingsOverviewHtml(text, layer, setting->children, depth + 1);
    }
}

std::string BuildArchor(const std::string& label) {
    std::string tmp = ToLowerCase(label);
    std::replace(tmp.begin(), tmp.end(), ' ', '-');
    return tmp;
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

static const std::string GetLayerSettingsDocURL(const Layer& layer) {
    if (layer.api_version >= Version(1, 3, 268)) {
        return format("https://vulkan.lunarg.com/doc/sdk/%s.0/windows/layer_configuration.html", layer.api_version.str().c_str());
    } else {
        return "https://vulkan.lunarg.com/doc/view/latest/windows/layer_configuration.html";
    }
}

static void WriteSettingsDetailsHtml(std::string& text, const Layer& layer, const SettingMetaSet& settings) {
    for (std::size_t i = 0, n = settings.size(); i < n; ++i) {
        const SettingMeta* setting = settings[i];

        if (setting->view == SETTING_VIEW_STANDARD) {
            if (setting->status == STATUS_STABLE) {
                text += format("<h3><a id=\"%s-detailed\" href=\"#%s\">%s</a></h3>\n", setting->key.c_str(), setting->key.c_str(),
                               setting->label.c_str());
            } else {
                text += format("<h3><a id=\"%s-detailed\" href=\"#%s\">%s</a> (%s)</h3>\n", setting->key.c_str(),
                               setting->key.c_str(), setting->label.c_str(), GetToken(setting->status));
            }

            if (!setting->description.empty()) {
                std::string trim_desc = ::TrimSurroundingWhitespace(setting->description);
                if (trim_desc[trim_desc.size() - 1] != '.') {
                    trim_desc += ".";
                }

                text += format("\t<p>%s</p>\n", trim_desc.c_str());
            }

            if (!setting->detailed.empty()) {
                std::string trim_detailed = ::TrimSurroundingWhitespace(setting->detailed);
                if (trim_detailed[trim_detailed.size() - 1] != '.') {
                    trim_detailed += ".";
                }

                text += format("\t<p>%s</p>\n", trim_detailed.c_str());
            }

            if (!setting->url.Empty()) {
                const std::string url = ConvertStandardSeparators(setting->url.AbsolutePath());
                text +=
                    format("\t<p>Learn more about the <a href=\"%s\">%s</a> setting.</p>\n", url.c_str(), setting->label.c_str());
            }

            if (setting->status == STATUS_DEPRECATED && !setting->deprecated_by_key.empty()) {
                const SettingMeta* replaced_setting = FindSetting(layer.settings, setting->deprecated_by_key.c_str());

                text += format("\t<p>This setting was deprecated and replaced by <a href=\"#%s-detailed\">%s</a> setting.</p>\n",
                               replaced_setting->key.c_str(), replaced_setting->label.c_str());
            }

            if (!setting->dependence.empty()) {
                text += "<h4>Setting Dependences</h4>\n";
                if (setting->dependence.size() > 1) {
                    switch (setting->dependence_mode) {
                        case DEPENDENCE_NONE:
                            // text += "\n";
                            break;
                        case DEPENDENCE_ALL:
                            text += "<p>All the following condition must be fulfilled for the setting to be applied.</p>\n";
                            break;
                        case DEPENDENCE_ANY:
                            text += "<p>Any of the following condition must be fulfilled for the setting to be applied.</p>\n";
                            break;
                    }
                }

                text += "<table><thead><tr>";
                text += format("<th>Label</th><th><a href=\"%s\">Variables Key</a></th><th>Type</th><th>Value</th>",
                               GetLayerSettingsDocURL(layer).c_str());
                text += "</tr></thead><tbody>\n";

                for (std::size_t i = 0, n = setting->dependence.size(); i < n; ++i) {
                    const SettingMeta* setting_dep = ::FindSetting(layer.settings, setting->dependence[i]->key.c_str());
                    if (setting_dep == nullptr) {
                        continue;
                    }

                    text += "<tr>\n";
                    text +=
                        format("<td><a href=\"#%s-detailed\">%s</a></td>", setting_dep->key.c_str(), setting_dep->label.c_str());
                    text += format("<td>%s</td>", setting_dep->key.c_str());
                    text += format("<td>%s</td>", GetToken(setting_dep->type));
                    text += format("<td>%s</td>", setting->dependence[i]->Export(EXPORT_MODE_DOC).c_str());
                    text += "</tr>\n";
                }

                text += "</tbody></table>\n";
            }

            if (!setting->children.empty()) {
                text += "\t<p>";
                if (setting->children.size() == 1) {
                    text += "Sub-setting: ";
                } else {
                    text += "Sub-settings: ";
                }

                for (std::size_t i = 0, n = setting->children.size(); i < n; ++i) {
                    SettingMeta* child = setting->children[i];
                    text += format("<a href=\"#%s-detailed\">%s</a>", child->key.c_str(), ::BuildArchor(child->label).c_str());
                    if (i < n - 2) {
                        text += ", ";
                    } else if (i < n - 1) {
                        text += ", and ";
                    } else {
                        text += ".\n";
                    }
                }
                text += "</p>\n";
            }

            {
                text += "<ul>\n";

                text += format("\t<li>Key: <span class=\"code\">%s</span></li>\n", setting->key.c_str());
                text += format("\t<li>Type: <span class=\"code\">%s</span></li>\n", GetToken(setting->type));
                if (setting->type != SETTING_GROUP) {
                    text += format("\t<li>Default Value: <span class=\"code\">%s</span></li>\n",
                                   setting->Export(EXPORT_MODE_DOC).c_str());
                }
                text += format("\t<li>Platforms: %s</li>\n", BuildPlatformsHtml(setting->platform_flags).c_str());
                text += "</ul>\n";
            }

            if (IsEnum(setting->type) || IsFlags(setting->type)) {
                const SettingMetaEnumeration& setting_enum = static_cast<const SettingMetaEnumeration&>(*setting);
                if (setting_enum.enum_values.size() > 0) {
                    text += "<table>\n";
                    if (IsEnum(setting->type)) {
                        text += "<thead><tr><th>Enum Value</th><th>Label</th><th class=\"desc\">Description</th><th>Platforms ";
                    } else {
                        // Flags
                        text += "<thead><tr><th>Flags</th><th>Label</th><th class=\"desc\">Description</th><th>Platforms ";
                    }
                    text += "</th></tr></thead>\n";
                    text += "<tbody>\n";
                    for (std::size_t j = 0, o = setting_enum.enum_values.size(); j < o; ++j) {
                        const SettingEnumValue& value = setting_enum.enum_values[j];

                        if (value.view != SETTING_VIEW_STANDARD) {
                            continue;
                        }

                        text += "<tr>\n";
                        text += format("\t<td>%s</td>\n", value.key.c_str());
                        text += format("\t<td>%s</td>\n", value.label.c_str());
                        if (value.description.empty()) {
                            text += "\t<td>N/A</td>\n";
                        } else if (value.detailed.empty()) {
                            text += format("\t<td class=\"desc\">%s</td>\n", value.description.c_str());
                        } else {
                            text += format("\t<td class=\"desc\"><p>%s</p><p>%s</p></td>\n", value.description.c_str(),
                                           value.detailed.c_str());
                        }
                        text += format("\t<td>%s</td>\n", BuildPlatformsHtml(value.platform_flags).c_str());
                        text += "</tr>\n";
                    }
                    text += "</tbody></table>\n";
                    text += "<p></p>\n";
                }
            }

            if (setting->type != SETTING_GROUP) {
                text += "<table>\n";
                text += format("<thead><tr><th><a href=\"%s\">Methods:</a></th><th></th></tr></thead>\n",
                               GetLayerSettingsDocURL(layer).c_str());
                text += "<tbody>\n";

                {
                    const char* string_char = IsSettingTypeString(setting->type) ? "\"" : "";
                    std::string default_value = setting->Export(EXPORT_MODE_DOC);
                    if (setting->type == SETTING_BOOL) {
                        if (default_value == "true") {
                            default_value = "VK_TRUE";
                        }
                        if (default_value == "false") {
                            default_value = "VK_FALSE";
                        }
                    }

                    text += "\t<tr><td><span class=\"code\">VK_EXT_layer_settings</span>:</td><td><span class=\"code\">";

                    text += format("%s data[] = { %s%s%s };<br>", ::GetCodeTypeString(setting->type), string_char,
                                   default_value.c_str(), string_char);
                    text += "VkLayerSettingEXT setting {<br>";
                    text += format("&emsp;\"%s\", \"%s\", %s,<br>", layer.key.c_str(), setting->key.c_str(),
                                   ::GetLayerSettingTypeString(setting->type));
                    text += "&emsp;static_cast<uint32_t>(std::size(data)), data };\n";

                    text += "</span></td></tr>\n";
                }

                text += format(
                    "\t<tr><td><span class=\"code\">vk_layer_settings.txt</span>:</td><td><span "
                    "class=\"code\">%s = %s</span></td></tr>\n",
                    (GetLayerSettingPrefix(layer.key) + setting->key).c_str(), setting->Export(EXPORT_MODE_DOC).c_str());

                {
                    std::vector<std::string> envs = BuildEnvVariablesList(layer.key.c_str(), setting->key.c_str());
                    if (!setting->env.empty()) {
                        envs.push_back(setting->env);
                    }

                    text += "\t<tr><td> Environment variables : </ td><td><span class =\"code\">\n";

                    for (std::size_t i = 0, n = envs.size(); i < n; ++i) {
                        std::string value = setting->Export(EXPORT_MODE_DOC);
                        text += format("export %s=%s", envs[i].c_str(), value.c_str());
                        if (i < n - 1) {
                            text += "<br>";
                        }
                    }

                    text += "</span></td></tr>\n";
                }

                if (setting->platform_flags & PLATFORM_ANDROID_BIT) {
                    std::string value = setting->Export(EXPORT_MODE_DOC);

                    std::vector<std::string> commands = BuildEnvVariablesList(layer.key.c_str(), setting->key.c_str(), true);
                    std::vector<std::string> list;

                    for (std::size_t i = 0, n = commands.size(); i < n; ++i) {
                        list.push_back(format("adb setprop %s %s", commands[i].c_str(), value.c_str()));
                    }

                    if (!layer.prefix.empty()) {
                        list.push_back(
                            format("adb setprop debug.%s %s", (layer.prefix + "." + setting->key).c_str(), value.c_str()));
                    }

                    text += format("\t<tr><td>Android system properties:</td><td><span class=\"code\">%s</span></td></tr>\n",
                                   Merge(list, "<BR/>").c_str());
                }

                text += "</tbody></table>\n";
            }

            text += "<hr>\n";

            if (IsEnum(setting->type) || IsFlags(setting->type)) {
                const SettingMetaEnumeration& setting_enum = static_cast<const SettingMetaEnumeration&>(*setting);

                for (std::size_t j = 0, o = setting_enum.enum_values.size(); j < o; ++j) {
                    ::WriteSettingsDetailsHtml(text, layer, setting_enum.enum_values[j].settings);
                }
            }
        }

        ::WriteSettingsDetailsHtml(text, layer, setting->children);
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
                    const char* string_char = IsSettingTypeString(setting->type) ? "\"" : "";
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

bool ExportHtmlDoc(const Layer& layer, const std::string& path) {
    std::string text;

    text += "<!DOCTYPE html>\n";
    text += "<html>\n";
    text += format("<head><title></title></head>\n", layer.key.c_str());
    text += "<body>\n";
    text += "<style>\n";
    text += "\ta {color: #A41E22;}\n";
    text += "\th1 {color: #A41E22;}\n";
    text += "\th2 {color: #A41E22;}\n";
    text += "\ttable {border: 0px solid; width: 100%; margin-left: auto; margin-right: auto;}\n";
    text += "\ttd {border: 1px dotted;}\n";
    text += "\t.code {color: #008000; font-family: consolas; }\n";
    text += "\t.desc {width:50%;}\n";
    text += "</style>\n";

    text += "<h1 id=\"top\">";
    if (layer.url.Empty()) {
        text += format("%s\n", layer.key.c_str());
    } else {
        text += format("<a href=\"%s\">%s</a>\n", layer.url.AbsolutePath(false).c_str(), layer.key.c_str());
    }

    if (layer.status != STATUS_STABLE) {
        text += format(" (%s)", GetToken(layer.status));
    }
    text += "</h1>\n";

    if (!layer.description.empty()) {
        text += "<h3>" + layer.description + "</h3>\n";
    }

    if (!layer.introduction.empty()) {
        text += format("<p>%s</p>\n", layer.introduction.c_str());
    }

    text += "<h2>Layer Properties</h2>\n";
    text += "<ul>\n";
    text += format("\t<li>API Version: %s</li>\n", layer.api_version.str().c_str());
    text += format("\t<li>Implementation Version: %s</li>\n", layer.implementation_version.c_str());
    text += format("\t<li>Layer Manifest: <span class=\"code\">%s</span><ul>\n",
                   QFileInfo(layer.manifest_path.RelativePath().c_str()).fileName().toStdString().c_str());
    text += format("\t\t<li>File Format: %s</li>\n", layer.file_format_version.str().c_str());
    text += format("\t\t<li>Layer Binary Path: <span class=\"code\">%s</span></li>\n", layer.binary_path.RelativePath().c_str());
    text += "\t</ul></li>\n";
    if (layer.platforms != 0) {
        text += format("\t<li>Platforms: %s</li>\n", BuildPlatformsHtml(layer.platforms).c_str());
    }
    if (layer.status != STATUS_STABLE) {
        text += format("\t<li>Status: %s</li>\n", GetToken(layer.status));
    }
    if (!layer.settings.empty()) {
        text +=
            format("\t<li><a href=\"#settings\">Number of Layer Settings: %d</a></li>\n", ::CountSettings(layer.settings, true));
    }
    if (!layer.presets.empty()) {
        text += format("\t<li><a href=\"#presets\">Number of Layer Presets: %d</a></li>\n", layer.presets.size());
    }
    text += "</ul>\n";

    if (!layer.settings.empty()) {
        text += "<h2><a id=\"settings\">Layer Settings Overview</a></h2>\n";
        text += "<table><thead><tr>";
        text +=
            format("<th>Label</th><th><a href=\"%s\">Variables Key</a></th><th>Type</th><th>Default Value</th><th>Platforms</th>",
                   GetLayerSettingsDocURL(layer).c_str());
        text += "</tr></thead><tbody>\n";
        WriteSettingsOverviewHtml(text, layer, layer.settings, 0);
        text += "</tbody></table>\n";

        text += "<h2>Layer Settings Details</h2>\n";
        WriteSettingsDetailsHtml(text, layer, layer.settings);
    }

    if (!layer.presets.empty()) {
        text += "<h2><a id=\"presets\">Layer Presets</a></h2>\n";
        for (std::size_t i = 0, n = layer.presets.size(); i < n; ++i) {
            const LayerPreset& preset = layer.presets[i];

            text += format("<h3>%s</h3>\n", preset.label.c_str());
            text += format("<p>%s</p>", preset.description.c_str());

            text += "<h4>Preset Setting Values:</h4>\n";
            text += "<ul>\n";

            for (std::size_t j = 0, o = preset.settings.size(); j < o; ++j) {
                const SettingData* data = preset.settings[j];
                const SettingMeta* meta = FindSetting(layer.settings, data->key.c_str());

                text += format("\t<li><a href=\"#%s-detailed\">%s</a>: <span class=\"code\">%s</span></li>\n", meta->key.c_str(),
                               meta->label.c_str(), data->Export(EXPORT_MODE_DOC).c_str());
            }

            text += "</ul>\n";
        }
    }

    text += "</body>\n";
    text += "</html>\n";

    QFile file(path.c_str());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(text.c_str());
        file.close();
        printf("vkconfig: HTML file written to %s\n", path.c_str());
        return true;
    } else {
        printf("vkconfig: could not write %s\n", path.c_str());
        return false;
    }
}

bool ExportMarkdownDoc(const Layer& layer, const std::string& path) {
    std::string text;

    text += format("# %s\n", layer.key.c_str());
    text += "\n";

    if (!layer.description.empty()) {
        text += "## " + layer.description + "\n";
        text += "\n";
    }

    if (!layer.introduction.empty()) {
        text += layer.introduction + "\n";
        text += "\n";
    }

    text += "## Layer Properties\n";
    text += "\n";
    text += "- API Version: " + layer.api_version.str() + "\n";
    text += "- Implementation Version: " + layer.implementation_version + "\n";
    text += "- Layer Manifest: `" + QFileInfo(layer.manifest_path.RelativePath().c_str()).fileName().toStdString() + "`\n";
    text += "  - File Format: " + layer.file_format_version.str() + "\n";
    text += "  - Layer Binary: `";
    text += (layer.binary_path.RelativePath().rfind("./", 0) == 0 ? layer.binary_path.RelativePath().substr(2)
                                                                  : layer.binary_path.RelativePath()) +
            "`\n";
    text += "- Variables:\n";
    text += format("  - `vk_layer_settings.txt` namespace: `%s`\n", ToLowerCase(TrimPrefix(layer.key)).c_str());
    text += format("  - Environment Variable prefix: `VK_%s_`\n", ToUpperCase(TrimPrefix(layer.key)).c_str());
    text +=
        format("  - Android system property prefix: `adb setprop debug.vulkan.%s.`\n", ToLowerCase(TrimPrefix(layer.key)).c_str());

    if (layer.platforms != 0) {
        text += "- Platforms: " + BuildPlatformsMarkdown(layer.platforms) + "\n";
    }
    if (layer.status != STATUS_STABLE) {
        text += format(" - Status: %s\n", GetToken(layer.status));
    }
    if (!layer.settings.empty()) {
        text += format("- Number of Layer Settings: %d\n", ::CountSettings(layer.settings, true));
    }
    if (!layer.presets.empty()) {
        text += format("- Number of Layer Presets: %d\n", layer.presets.size());
    }
    text += "\n";

    if (!layer.settings.empty()) {
        text += "## Layer Settings Overview\n";
        text += "\n";
        text += "|Label|Variables Key|Type|Default Value|Platforms|\n";
        text += "|---|---|---|---|---|\n";
        WriteSettingsOverviewMarkdown(text, layer, layer.settings, 0);
        text += "\n";

        text += "## Layer Settings Details\n";
        text += "\n";
        WriteSettingsDetailsMarkdown(text, layer, layer.settings);
    }

    if (!layer.presets.empty()) {
        text += "## Layer Presets\n";
        text += "\n";
        for (std::size_t i = 0, n = layer.presets.size(); i < n; ++i) {
            const LayerPreset& preset = layer.presets[i];

            text += "### " + preset.label + "\n";
            text += "\n";
            text += preset.description + "\n";
            text += "\n";
            text += "#### Preset Setting Values:\n";
            for (std::size_t j = 0, o = preset.settings.size(); j < o; ++j) {
                const SettingData* data = preset.settings[j];
                const SettingMeta* meta = FindSetting(layer.settings, data->key.c_str());

                text += "- " + meta->label + ": " + data->Export(EXPORT_MODE_DOC).c_str() + "\n";
            }
            text += "\n";
        }
    }

    QFile file(path.c_str());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(text.c_str());
        file.close();
        printf("vkconfig: markdown file written to %s\n", path.c_str());
        return true;
    } else {
        printf("vkconfig: could not write %s\n", path.c_str());
        return false;
    }
}

bool ExportSettingsDoc(Configurator& configurator, const std::string& path) {
    const bool result = configurator.WriteLayersSettings(OVERRIDE_AREA_LAYERS_SETTINGS_BIT, path);

    if (result)
        printf("vkconfig: settings written to %s\n", path.c_str());
    else {
        printf("vkconfig: could not write %s\n", path.c_str());
    }

    return result;
}
