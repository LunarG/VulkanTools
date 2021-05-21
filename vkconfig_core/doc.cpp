/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

static void WriteSettingsOverview(std::string& text, const std::string& layer_key, const SettingMetaSet& settings) {
    for (std::size_t i = 0, n = settings.Size(); i < n; ++i) {
        if (settings[i].type != SETTING_GROUP && settings[i].view != SETTING_VIEW_HIDDEN) {
            text += "<tr>\n";
            text += format("\t<td><a id=\"%s\"href=\"#%s-detailed\">%s</a></td>\n", settings[i].key.c_str(),
                           settings[i].key.c_str(), settings[i].label.c_str());
            if (settings[i].env.empty()) {
                text += format("\t<td>N/A</td>\n", settings[i].env.c_str());
            } else {
                text += format("\t<td><span class=\"code\">%s</span></td>\n", settings[i].env.c_str());
            }

            text +=
                format("\t<td><span class=\"code\">%s</span></td>\n", (GetLayerSettingPrefix(layer_key) + settings[i].key).c_str());
            text += format("\t<td><span class=\"code\">%s</span></td>\n", GetSettingTypeToken(settings[i].type));
            text += format("\t<td>%s</td>\n", "TODO");
            text += "\t<td>";
            const std::vector<std::string>& platforms = GetPlatformTokens(settings[i].platform_flags);
            for (std::size_t i = 0, n = platforms.size(); i < n; ++i) {
                text += "<span class=\"code\">" + platforms[i] + "</ span>";
                if (i < n - 1) {
                    text += ", ";
                }
            }
            text += "</td>\n";
            text += "</tr>\n";
        }

        WriteSettingsOverview(text, layer_key, settings[i].children);
    }
}

static void WriteSettingsDetails(std::string& text, const std::string& layer_key, const SettingMetaSet& settings) {
    for (std::size_t i = 0, n = settings.Size(); i < n; ++i) {
        if (settings[i].type != SETTING_GROUP && settings[i].view != SETTING_VIEW_HIDDEN) {
            if (settings[i].status == STATUS_STABLE) {
                text += format("<h3><a id=\"%s-detailed\" href=\"#%s\">%s</a></h3>\n", settings[i].key.c_str(),
                               settings[i].key.c_str(), settings[i].label.c_str());
            } else {
                text += format("<h3><a id=\"%s-detailed\" href=\"#%s\">%s</a> (%s)</h3>\n", settings[i].key.c_str(),
                               settings[i].key.c_str(), settings[i].label.c_str(), GetToken(settings[i].status));
            }

            text += format("\t<p>%s</p>\n", settings[i].description.c_str());

            text += "<h4>Setting Properties:</h4>\n";
            text += "<ul>\n";
            if (settings[i].env.empty()) {
                text += format("\t<li>Environment Variable: <span class=\"code\">%s</span></li>\n", "N/A");
            } else {
                text += format("\t<li>Environment Variable: <span class=\"code\">%s</span></li>\n", settings[i].env.c_str());
            }
            text += format("\t<li>vk_layer_settings.txt Variable: <span class=\"code\">%s</span></li>\n",
                           (GetLayerSettingPrefix(layer_key) + settings[i].key).c_str());

            text += "\t<li>Platforms Supported: ";
            const std::vector<std::string>& platforms = GetPlatformTokens(settings[i].platform_flags);
            for (std::size_t i = 0, n = platforms.size(); i < n; ++i) {
                text += "<span class=\"code\">" + platforms[i] + "</span>";
                if (i < n - 1) {
                    text += ", ";
                }
            }
            text += "</li>\n";
            if (settings[i].view != SETTING_VIEW_STANDARD) {
                text += format("\t<li>Setting Level: %s</li>\n", GetSettingViewToken(settings[i].view));
            }

            text += "</ul>\n";

            text += format(
                "\t<p>Setting Type: <span class=\"code\">%s</span> - Setting Default Value: <span class=\"code\">%s</span></p>\n",
                GetSettingTypeToken(settings[i].type), "TODO");

            if (IsEnum(settings[i].type)) {
                const SettingMetaEnumeration& setting_enum = static_cast<const SettingMetaEnumeration&>(settings[i]);

                text +=
                    "<table><thead><tr><th>Enum Value</th><th>Label</th><th class=\"desc\">Description</th><th>Platforms "
                    "Supported</th></tr></thead><tbody>\n";
                for (std::size_t j = 0, o = setting_enum.enum_values.size(); j < o; ++j) {
                    const SettingEnumValue& value = setting_enum.enum_values[j];

                    if (value.view == SETTING_VIEW_HIDDEN) continue;

                    text += "<tr>\n";
                    text += format("\t<td><a id=\"%s\"href=\"#%s-detailed\">%s</a></td>\n", value.key.c_str(), value.key.c_str(),
                                   value.key.c_str());
                    text += format("\t<td>%s</td>\n", value.label.c_str());
                    if (value.description.empty()) {
                        text += "\t<td>N/A</td>\n";
                    } else {
                        text += format("\t<td class=\"desc\">%s</td>\n", value.description.c_str());
                    }
                    text += "\t<td>";
                    const std::vector<std::string>& platforms = GetPlatformTokens(settings[i].platform_flags);
                    for (std::size_t i = 0, n = platforms.size(); i < n; ++i) {
                        text += "<span class=\"code\">" + platforms[i] + "</ span>";
                        if (i < n - 1) {
                            text += ", ";
                        }
                    }
                    text += "</td>\n";
                    text += "</tr>\n";
                }
            }
        }

        WriteSettingsDetails(text, layer_key, settings[i].children);
    }
}

void ExportHtmlDoc(const Layer& layer, const std::string& path) {
    std::string text;

    text += "<!DOCTYPE html>\n";
    text += "<html>\n";
    text += format("<head><title></title></head>\n", layer.key.c_str());
    text += "<body>\n";
    text += "<style>\n";
    text += "\ta {color: #A41E22;}\n";
    text += "\th1 {color: #A41E22;}\n";
    text += "\th2 {color: #A41E22;}\n";
    text += "\ttable {border: 1px solid; width: 100%; margin-left: auto; margin-right: auto;}\n";
    text += "\ttd {border: 1px dotted;}\n";
    text += "\t.code {color: #008000; font-family: consolas; }\n";
    text += "\t.desc {width:50%;}\n";
    text += "</style>\n";

    text += "<h1>";
    if (layer.url.empty()) {
        text += format("%s\n", layer.key.c_str());
    } else {
        text += format("<a href=\"%s\">%s</a>\n", layer.url.c_str(), layer.key.c_str());
    }
    if (layer.status != STATUS_STABLE) {
        text += format(" (%s)", GetToken(layer.status));
    }
    text += "</h1>";

    if (!layer.introduction.empty()) {
        text += format("<p>%s</p>\n", layer.introduction.c_str());
    }

    text += "<h2>Layer Properties</h2>\n";
    text += "<p>TODO</p>";

    if (!layer.settings.Empty()) {
        text += "<h2>Settings Overview</h2>\n";
        text +=
            "<table><thead><tr><th>Setting</th><th>Environment Variable</th><th>vk_layer_settings.txt Variable</th><th>Setting "
            "Type</th><th>Default Value</th><th>Platforms Supported</th></tr></thead><tbody>\n";
        WriteSettingsOverview(text, layer.key, layer.settings);
        text += "</tbody></table>\n";

        text += "<h2>Settings Details</h2>\n";
        WriteSettingsDetails(text, layer.key, layer.settings);
    }

    if (!layer.presets.empty()) {
        text += "<h2>Presets</h2>\n";
        for (std::size_t i = 0, n = layer.presets.size(); i < n; ++i) {
            text += format("<h3>%s</h3>\n", layer.presets[i].label.c_str());
            text += format("<p>%s</p>", layer.presets[i].description.c_str());
        }
    }

    text += "</body>\n";
    text += "</html>\n";

    QFile file(path.c_str());
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(text.c_str());
        file.close();
    }
}

void ExportSettingsDoc(const Layer& layer, const std::string& path) {}
