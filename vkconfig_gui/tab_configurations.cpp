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

#include "widget_tab_configurations_layer.h"
#include "tab_configurations.h"
#include "mainwindow.h"
#include "widget_resize_button.h"
#include "style.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/doc.h"
#include "../vkconfig_core/type_hide_message.h"

#include <QSettings>
#include <QMenu>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>

static std::string BuildPropertiesLog(const Layer &layer) {
    std::string description;
    if (!layer.description.empty()) {
        description += layer.description + "\n";
    }
    description += "API Version: " + layer.api_version.str() + " - Implementation Version: " + layer.implementation_version + "\n";
    if (layer.platforms != 0) {
        description += "Supported Platforms: ";

        const std::vector<std::string> &platforms = GetPlatformTokens(layer.platforms);
        for (std::size_t i = 0, n = platforms.size(); i < n; ++i) {
            description += platforms[i];
            if (i < n - 1) {
                description += ", ";
            }
        }

        description += "\n";
    }

    description += "\n";
    description += layer.manifest_path.AbsolutePath() + "\n";
    description += format("- %s Layers Path \n", GetToken(layer.type));
    description += "- File Format: " + layer.file_format_version.str() + "\n";
    description += "- Layer Binary Path:\n    " + layer.binary_path.AbsolutePath() + "\n";
    description += "\n";
    description +=
        format("Total Settings Count: %d - Total Presets Count: %d", CountSettings(layer.settings), layer.presets.size());
    return description;
}

TabConfigurations::TabConfigurations(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui)
    : Tab(TAB_CONFIGURATIONS, window, ui), _settings_tree_manager(ui) {
    this->ui->configurations_list->installEventFilter(&window);
    this->ui->configurations_layers_list->installEventFilter(&window);
    this->ui->configurations_settings->installEventFilter(&window);

    this->connect(this->ui->configurations_executable_scope, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_configurations_executable_scope_currentIndexChanged(int)));
    this->connect(this->ui->configurations_executable_list, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_configurations_executable_list_currentIndexChanged(int)));
    this->connect(this->ui->configurations_executable_append, SIGNAL(clicked()), this,
                  SLOT(on_configurations_executable_append_pressed()));
    this->connect(this->ui->configurations_executable_remove, SIGNAL(clicked()), this,
                  SLOT(on_configurations_executable_remove_pressed()));

    this->connect(this->ui->configurations_group_box_list, SIGNAL(toggled(bool)), this, SLOT(on_configurations_list_toggled(bool)));
    this->connect(this->ui->configurations_group_box_layers, SIGNAL(toggled(bool)), this,
                  SLOT(on_configurations_layers_ordering_toggled(bool)));
    this->connect(this->ui->configurations_group_box_loader, SIGNAL(toggled(bool)), this,
                  SLOT(on_configurations_loader_messages_toggled(bool)));
    this->connect(this->ui->configurations_group_box_settings, SIGNAL(toggled(bool)), this,
                  SLOT(on_configurations_layers_settings_toggled(bool)));

    this->connect(this->ui->configuration_loader_errors, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_errors_toggled(bool)));
    this->connect(this->ui->configuration_loader_warns, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_warns_toggled(bool)));
    this->connect(this->ui->configuration_loader_infos, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_infos_toggled(bool)));
    this->connect(this->ui->configuration_loader_debug, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_debug_toggled(bool)));
    this->connect(this->ui->configuration_loader_layers, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_layers_toggled(bool)));
    this->connect(this->ui->configuration_loader_drivers, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_drivers_toggled(bool)));

    this->connect(this->ui->configurations_list, SIGNAL(itemChanged(QListWidgetItem *)), this,
                  SLOT(on_configurations_list_itemChanged(QListWidgetItem *)));
    this->connect(this->ui->configurations_list, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
                  SLOT(on_configurations_list_itemDoubleClicked(QListWidgetItem *)));
    this->connect(this->ui->configurations_list, SIGNAL(currentRowChanged(int)), this,
                  SLOT(on_configurations_list_currentRowChanged(int)));

    this->connect(this->ui->configurations_layers_list, SIGNAL(currentRowChanged(int)), this,
                  SLOT(on_configurations_layers_list_currentRowChanged(int)));

    this->connect(&this->_settings_tree_manager, SIGNAL(signalLayerVersionChanged()), this,
                  SLOT(on_configurations_layerVersionChanged()));

    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    this->ui->splitter_main->restoreState(settings.value("vkconfig3/mainwindow/splitter_main_state").toByteArray());
    this->ui->splitter_configurations->restoreState(
        settings.value("vkconfig3/mainwindow/splitter_configurations_state").toByteArray());
    this->ui->splitter_settings->restoreState(settings.value("vkconfig3/mainwindow/splitter_settings_state").toByteArray());

    Configurator &configurator = Configurator::Get();

    this->ui->configurations_executable_scope->blockSignals(true);
    this->ui->configurations_executable_scope->clear();
    for (int i = 0, n = EXECUTABLE_SCOPE_COUNT; i < n; ++i) {
        ExecutableScope scope = static_cast<ExecutableScope>(i);
        this->ui->configurations_executable_scope->addItem(GetLabel(scope));
        this->ui->configurations_executable_scope->setItemData(i, GetTooltip(scope), Qt::ToolTipRole);
    }

    ExecutableScope current_scope = configurator.GetExecutableScope();
    this->ui->configurations_executable_scope->setCurrentIndex(current_scope);
    this->ui->configurations_executable_scope->blockSignals(false);

    this->advanced_mode = new ResizeButton(this->ui->configurations_group_box_layers, 0);
    this->advanced_mode->setMinimumSize(24, 24);
    this->advanced_mode->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->advanced_mode->adjustSize();
    this->ui->configurations_group_box_layers->installEventFilter(this->advanced_mode);

    this->connect(this->advanced_mode, SIGNAL(pressed()), this, SLOT(on_configurations_advanced_toggle_pressed()));

    this->UpdateUI(UPDATE_REBUILD_UI);
}

TabConfigurations::~TabConfigurations() {
    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    settings.setValue("vkconfig3/mainwindow/splitter_main_state", ui->splitter_main->saveState());
    settings.setValue("vkconfig3/mainwindow/splitter_configurations_state", ui->splitter_configurations->saveState());
    settings.setValue("vkconfig3/mainwindow/splitter_settings_state", ui->splitter_settings->saveState());
}

void TabConfigurations::UpdateUI_Configurations(UpdateUIMode mode) {
    (void)mode;

    Configurator &configurator = Configurator::Get();

    ui->configurations_executable_scope->blockSignals(true);
    ui->configurations_executable_scope->setCurrentIndex(configurator.GetExecutableScope());
    ui->configurations_executable_scope->blockSignals(false);

    ui->configurations_list->blockSignals(true);

    ui->configurations_list->clear();

    int current_row = -1;

    for (std::size_t i = 0, n = configurator.configurations.available_configurations.size(); i < n; ++i) {
        const Configuration &configuration = configurator.configurations.available_configurations[i];

        std::vector<std::string> missing_layers;
        const bool has_missing_layer = configuration.HasMissingLayer(configurator.layers, missing_layers);

        // Hide built-in configuration when the layer is missing. The Vulkan user may have not installed the necessary layer
        // if (configuration.IsBuiltIn() && has_missing_layer) {
        //    continue;
        //}

        ListItem *item = new ListItem(configuration.key.c_str());
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setText(configuration.key.c_str());
        if (configurator.GetActiveConfiguration() == &configuration) {
            item->setIcon(::Get(::ICON_SYSTEM_ON));
            item->setToolTip(format("Using the '%s' configuration with Vulkan executables", configuration.key.c_str()).c_str());
            ui->configurations_group_box_layers->blockSignals(true);
            ui->configurations_group_box_layers->setChecked(configuration.override_layers);
            ui->configurations_group_box_layers->blockSignals(false);
            ui->configurations_group_box_loader->blockSignals(true);
            ui->configurations_group_box_loader->setChecked(configuration.override_loader);
            if (configurator.vulkan_system_info.loaderVersion <= Version(1, 4, 304)) {
                ui->configurations_group_box_loader->setCheckable(configuration.override_layers);
                ui->configurations_group_box_loader->setEnabled(configuration.override_layers);
                if (configuration.override_layers) {
                    ui->configurations_group_box_loader->setToolTip("Configure Loader Messages");
                } else {
                    ui->configurations_group_box_loader->setToolTip(
                        "Due to a Vulkan Loader 304 bug, layers must be overridden for loader messages to be enabled.");
                }
            }
            ui->configurations_group_box_loader->blockSignals(false);
            current_row = static_cast<int>(i);
        } else if (has_missing_layer) {
            item->setIcon(::Get(::ICON_SYSTEM_INVALID));
            item->setToolTip(
                format("The '%s' configuration has missing layers. These layers are ignored.", configuration.key.c_str()).c_str());
        } else {
            item->setIcon(::Get(::ICON_SYSTEM_OFF));
            item->setToolTip(
                format("Select the '%s' configuration to use it with Vulkan executables", configuration.key.c_str()).c_str());
        }
        ui->configurations_list->addItem(item);
    }

    if (current_row != -1) {
        ui->configurations_list->setCurrentRow(current_row);
    }

    ui->configurations_list->blockSignals(false);
}

void TabConfigurations::UpdateUI_Applications(UpdateUIMode ui_update_mode) {
    Configurator &configurator = Configurator::Get();

    const std::vector<Executable> &executables = configurator.executables.GetExecutables();

    this->ui->configurations_executable_list->blockSignals(true);

    if (ui_update_mode == UPDATE_REBUILD_UI) {
        this->ui->configurations_executable_list->clear();
        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            const Executable &executable = executables[i];

            this->ui->configurations_executable_list->addItem(executable.path.RelativePath().c_str());
        }
    }

    if (!executables.empty()) {
        this->ui->configurations_executable_list->setCurrentIndex(configurator.executables.GetActiveExecutableIndex());
    }

    this->ui->configurations_executable_list->blockSignals(false);
}

void TabConfigurations::UpdateUI_LoaderMessages() {
    const Configurator &configurator = Configurator::Get();

    const Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        const bool enabled =
            configurator.vulkan_system_info.loaderVersion <= Version(1, 4, 304) ? configuration->override_layers : true;

        ui->configuration_loader_errors->blockSignals(true);
        ui->configuration_loader_errors->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_ERROR));
        ui->configuration_loader_errors->setEnabled(enabled);
        ui->configuration_loader_errors->blockSignals(false);
        ui->configuration_loader_warns->blockSignals(true);
        ui->configuration_loader_warns->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_WARN));
        ui->configuration_loader_warns->setEnabled(enabled);
        ui->configuration_loader_warns->blockSignals(false);
        ui->configuration_loader_infos->blockSignals(true);
        ui->configuration_loader_infos->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_INFO));
        ui->configuration_loader_infos->setEnabled(enabled);
        ui->configuration_loader_infos->blockSignals(false);
        ui->configuration_loader_debug->blockSignals(true);
        ui->configuration_loader_debug->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_DEBUG));
        ui->configuration_loader_debug->setEnabled(enabled);
        ui->configuration_loader_debug->blockSignals(false);
        ui->configuration_loader_layers->blockSignals(true);
        ui->configuration_loader_layers->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_LAYER));
        ui->configuration_loader_layers->setEnabled(enabled);
        ui->configuration_loader_layers->blockSignals(false);
        ui->configuration_loader_drivers->blockSignals(true);
        ui->configuration_loader_drivers->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_DRIVER));
        ui->configuration_loader_drivers->setEnabled(enabled);
        ui->configuration_loader_drivers->blockSignals(false);
    }

    this->ui->configurations_group_box_loader->setEnabled(configurator.HasEnabledUI(ENABLE_UI_LOADER));
}

void TabConfigurations::UpdateUI_Layers(UpdateUIMode mode) {
    (void)mode;

    ui->configurations_layers_list->blockSignals(true);
    ui->configurations_layers_list->clear();

    Configurator &configurator = Configurator::Get();
    this->ui->configurations_group_box_layers->setEnabled(configurator.HasEnabledUI(ENABLE_UI_LAYERS));

    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        const bool has_multiple_parameter = configuration->HasMultipleActiveParameter() || configurator.advanced;

        ui->execute_closer_application_label->setVisible(has_multiple_parameter);
        ui->execute_closer_driver_label->setVisible(has_multiple_parameter);
        ui->configurations_layers_list->setDragEnabled(has_multiple_parameter);

        bool selected_layer = configuration->selected_layer_name.empty();
        for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
            Parameter &parameter = configuration->parameters[i];

            if (!configurator.advanced) {
                if (parameter.control != LAYER_CONTROL_ON && parameter.control != LAYER_CONTROL_OFF) {
                    continue;
                }
            }

            QListWidgetItem *item = new ListItem(parameter.key.c_str());
            item->setFlags(item->flags() | Qt::ItemIsSelectable);
            item->setSizeHint(QSize(0, ITEM_HEIGHT));
            if (has_multiple_parameter) {
                item->setIcon(::Get(ICON_DRAG));
            }
            ui->configurations_layers_list->addItem(item);

            ConfigurationLayerWidget *layer_widget = new ConfigurationLayerWidget(this, parameter);

            ui->configurations_layers_list->setItemWidget(item, layer_widget);
            if (configuration->selected_layer_name == parameter.key) {
                ui->configurations_layers_list->setCurrentItem(item);
                selected_layer = true;
            }
        }

        if (!selected_layer) {
            configuration->selected_layer_name.clear();
        }
        // resizeEvent(nullptr);

        ui->configurations_layers_list->update();
    }

    ui->configurations_layers_list->blockSignals(false);
}

void TabConfigurations::UpdateUI_Settings(UpdateUIMode mode) {
    (void)mode;

    Configurator &configurator = Configurator::Get();

    if (configurator.GetActiveConfiguration() == nullptr) {
        this->_settings_tree_manager.CleanupGUI();
        this->ui->configurations_presets->setVisible(false);
    } else {
        this->_settings_tree_manager.CreateGUI();
    }

    const Parameter *parameter = configurator.GetActiveParameter();
    if (parameter != nullptr) {
        const std::string state = parameter->override_settings ? "Uncheck to disable" : "Check to enable";
        const std::string tooltip = format("%s '%s' layer settings", state.c_str(), parameter->key.c_str());

        this->ui->configurations_group_box_settings->setToolTip(tooltip.c_str());
    }
}

void TabConfigurations::UpdateUI(UpdateUIMode ui_update_mode) {
    this->UpdateUI_Configurations(ui_update_mode);
    this->UpdateUI_Applications(ui_update_mode);
    this->UpdateUI_LoaderMessages();
    this->UpdateUI_Layers(ui_update_mode);
    this->UpdateUI_Settings(ui_update_mode);

    const Configurator &configurator = Configurator::Get();
    const ExecutableScope scope = configurator.GetExecutableScope();

    const bool enabled_executable = ::EnabledExecutables(scope);

    if (enabled_executable) {
        const Executable *executable = configurator.GetActiveExecutable();

        if (executable != nullptr) {
            const std::string path = executable->path.RelativePath();

            const std::string state = executable->enabled ? "Uncheck to disable" : "Check to enable";

            if (scope == EXECUTABLE_PER) {
                this->ui->configurations_group_box_list->setToolTip(
                    format("%s to select a loader configuration for '%s' executable", state.c_str(), path.c_str()).c_str());
            } else {
                this->ui->configurations_group_box_list->setToolTip(
                    format("%s to enable the loader configuration for '%s' executable", state.c_str(), path.c_str()).c_str());
            }
        }
    } else if (scope == EXECUTABLE_ANY) {
        this->ui->configurations_group_box_list->setToolTip("Select the active loader configuration for any executable");
    } else {
        this->ui->configurations_group_box_list->setToolTip(
            "Change the 'Vulkan Loader Configuration scope' to apply a configuration.");
    }

    assert(this->advanced_mode != nullptr);
    if (configurator.advanced) {
        this->advanced_mode->setIcon(::Get(::ICON_BASIC));
        this->advanced_mode->setToolTip("Click to switch to basic Layers Configuration mode");
    } else {
        this->advanced_mode->setIcon(::Get(::ICON_ADVANCED));
        this->advanced_mode->setToolTip("Click to switch to advanced Layers Configuration mode");
    }

    this->ui->configurations_executable_scope->setToolTip(::GetTooltip(scope));
    this->ui->configurations_executable_list->setEnabled(enabled_executable);
    this->ui->configurations_executable_append->setEnabled(enabled_executable);
    this->ui->configurations_executable_remove->setEnabled(enabled_executable);

    this->ui->configurations_group_box_list->blockSignals(true);
    this->ui->configurations_group_box_list->setEnabled(configurator.HasEnabledUI(ENABLE_UI_CONFIG));
    this->ui->configurations_group_box_list->setCheckable(enabled_executable);
    const Executable *executable = configurator.executables.GetActiveExecutable();
    if (executable != nullptr) {
        this->ui->configurations_group_box_list->setChecked(executable->enabled);
    }
    this->ui->configurations_group_box_list->blockSignals(false);
}

void TabConfigurations::CleanUI() { this->_settings_tree_manager.CleanupGUI(); }

bool TabConfigurations::EventFilter(QObject *target, QEvent *event) {
    if (target == nullptr || event == nullptr) {
        return true;
    }

    QEvent::Type event_type = event->type();

    if (event_type == QEvent::Wheel) {
        return true;
    }

    Configurator &configurator = Configurator::Get();

    if (event_type == QEvent::ChildRemoved) {
        // Layers were reordered, we need to update the configuration

        std::vector<std::string> layer_names;
        for (int i = 0, n = ui->configurations_layers_list->count(); i < n; ++i) {
            QWidget *widget = ui->configurations_layers_list->itemWidget(ui->configurations_layers_list->item(i));
            if (widget != nullptr) {
                ConfigurationLayerWidget *layer_widget = dynamic_cast<ConfigurationLayerWidget *>(widget);
                if (layer_widget != nullptr) {
                    layer_names.push_back(layer_widget->layer_name);
                }
            }
        }

        Configuration *configuration = configurator.GetActiveConfiguration();
        if (configuration != nullptr) {
            configuration->Reorder(layer_names);
            configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

            this->UpdateUI_Settings(UPDATE_REBUILD_UI);
        }

        return true;
    }

    if (configurator.GetExecutableScope() == EXECUTABLE_NONE || !ui->configurations_list->isEnabled()) {
        return true;
    } else if (target == ui->configurations_list) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {
            ListItem *item = static_cast<ListItem *>(ui->configurations_list->itemAt(right_click->pos()));

            std::string name;

            if (item != nullptr) {
                name = item->key;
            }

            QMenu menu(ui->configurations_list);
            QFont subtitle_font = menu.font();
            subtitle_font.setBold(true);

            QAction *action_new = new QAction("Create a new Configuration", nullptr);
            action_new->setEnabled(true);
            menu.addAction(action_new);

            QAction *action_import = new QAction("Import a new Configuration...", nullptr);
            action_import->setEnabled(true);
            menu.addAction(action_import);

            menu.addSeparator();

            QAction *action_rename = new QAction("Rename the Configuration", nullptr);
            action_rename->setEnabled(item != nullptr);
            menu.addAction(action_rename);

            QAction *action_duplicate = new QAction("Duplicate the Configuration", nullptr);
            action_duplicate->setEnabled(item != nullptr);
            menu.addAction(action_duplicate);

            QAction *action_delete = new QAction("Delete the Configuration", nullptr);
            action_delete->setEnabled(item != nullptr);
            menu.addAction(action_delete);

            menu.addSeparator();

            QAction *action_reset_one = new QAction("Reset the Default Configuration", nullptr);
            action_reset_one->setEnabled(configurator.configurations.IsDefaultConfiguration(name));
            action_reset_one->setToolTip("Reset the configuration, discarding all changes of this configuration.");
            menu.addAction(action_reset_one);

            QAction *action_reset_all = new QAction("Reset All Default Configurations", nullptr);
            action_reset_all->setEnabled(true);
            action_reset_all->setToolTip("Reset all the default configurations, discarding all changes of these configurations.");
            menu.addAction(action_reset_all);

            menu.addSeparator();

            QAction *action_export_config = new QAction("Export the Configuration...", nullptr);
            action_export_config->setEnabled(item != nullptr);
            menu.addAction(action_export_config);

            QAction *action_export_settings = new QAction("Export the Layers Settings...", nullptr);
            action_export_settings->setEnabled(item != nullptr);
            menu.addAction(action_export_settings);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            if (action == action_new) {
                this->OnContextMenuNewClicked(item);
            } else if (action == action_import) {
                this->OnContextMenuImportClicked(item);
            } else if (action == action_rename) {
                this->OnContextMenuRenameClicked(item);
            } else if (action == action_duplicate) {
                this->OnContextMenuDuplicateClicked(item);
            } else if (action == action_delete) {
                this->OnContextMenuDeleteClicked(item);
            } else if (action == action_reset_one) {
                this->OnContextMenuResetOneClicked(item);
            } else if (action == action_reset_all) {
                this->OnContextMenuResetAllClicked(item);
            } else if (action == action_export_config) {
                this->OnContextMenuExportConfigsClicked(item);
            } else if (action == action_export_settings) {
                this->OnContextMenuExportSettingsClicked(item);
            }
        }
    } else if (target == ui->configurations_layers_list) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {
            ListItem *item = static_cast<ListItem *>(ui->configurations_layers_list->itemAt(right_click->pos()));

            if (item != nullptr) {
                const Layer *layer = configurator.layers.Find(item->key, Version::LATEST);

                QMenu menu(ui->configurations_layers_list);

                QAction *action_description = new QAction("Open the Layer Description...", nullptr);
                action_description->setEnabled(layer != nullptr);
                menu.addAction(action_description);

                QAction *export_html_action = new QAction("Open the Layer HTML Documentation...", nullptr);
                export_html_action->setEnabled(layer != nullptr);
                menu.addAction(export_html_action);

                QAction *export_markdown_action = new QAction("Open the Layer Markdown Documentation...", nullptr);
                export_markdown_action->setEnabled(layer != nullptr);
                menu.addAction(export_markdown_action);

                QAction *visit_layer_website_action = new QAction("Visit the Layer Website...", nullptr);
                visit_layer_website_action->setEnabled(layer != nullptr ? !layer->url.Empty() : false);
                menu.addAction(visit_layer_website_action);

                QPoint point(right_click->globalX(), right_click->globalY());
                QAction *action = menu.exec(point);

                if (action == action_description) {
                    assert(layer != nullptr);

                    std::string title = layer->key;
                    if (layer->status != STATUS_STABLE) {
                        title += format(" (%s)", GetToken(layer->status));
                    }

                    std::string text;
                    if (!layer->introduction.empty()) {
                        text += layer->introduction + "\n\n";
                    }
                    text += BuildPropertiesLog(*layer);

                    QMessageBox alert;
                    alert.setWindowTitle(title.c_str());
                    alert.setText(text.c_str());
                    alert.setStandardButtons(QMessageBox::Ok);
                    alert.setDefaultButton(QMessageBox::Ok);
                    alert.setIcon(QMessageBox::Information);
                    alert.exec();
                } else if (action == visit_layer_website_action) {
                    QDesktopServices::openUrl(QUrl(layer->url.AbsolutePath(false).c_str()));
                } else if (action == export_html_action) {
                    const std::string path = format("%s/%s.html", AbsolutePath(Path::APPDATA).c_str(), layer->key.c_str());
                    ExportHtmlDoc(*layer, path);
                    QDesktopServices::openUrl(QUrl(("file:///" + path).c_str()));
                } else if (action == export_markdown_action) {
                    const std::string path = format("%s/%s.md", AbsolutePath(Path::APPDATA).c_str(), layer->key.c_str());
                    ExportMarkdownDoc(*layer, path);
                    QDesktopServices::openUrl(QUrl(("file:///" + path).c_str()));
                }
            }
        }
    }

    // Pass it on
    return false;
}

void TabConfigurations::OnRenameConfiguration(QListWidgetItem *list_item) {
    ListItem *item = dynamic_cast<ListItem *>(list_item);
    if (item == nullptr) {
        return;
    }

    // This is the new name we want to use for the configuration
    const std::string &new_name = item->text().toStdString();
    bool valid_new_name = true;

    if (new_name.empty()) {
        valid_new_name = false;
        QMessageBox alert;
        alert.setWindowTitle("Renaming of the layers configuration failed...");
        alert.setText("The configuration name is empty.");
        alert.setInformativeText("The configuration name is required.");
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setDefaultButton(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
    } else if (!IsPortableFilename(new_name)) {
        valid_new_name = false;
        QMessageBox alert;
        alert.setWindowTitle("Invalid name for a configuration...");
        alert.setText("The configuration name is used to build a filename.");
        alert.setInformativeText("The name can't contain any of the following characters: \\ / : * \" < > |.");
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setDefaultButton(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
    } else if (new_name.size() > 255) {
        valid_new_name = false;
        QMessageBox alert;
        alert.setWindowTitle("Configuration name is too long...");
        alert.setText("The configuration name is used to build a filename.");
        alert.setInformativeText("The name must be a maximum of 255 characters.");
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setDefaultButton(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
    }

    Configurator &configurator = Configurator::Get();

    if (configurator.configurations.FindConfiguration(new_name) != nullptr) {
        valid_new_name = false;

        QMessageBox alert;
        alert.setWindowTitle("Renaming of the layers configuration failed...");
        alert.setText("There is already a configuration with the same name.");
        alert.setInformativeText("Use a different name for the configuration.");
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setDefaultButton(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
    }

    if (valid_new_name) {
        configurator.configurations.RenameConfiguration(item->key, new_name);
        item->key = new_name;
        configurator.SetActiveConfigurationName(new_name);

        this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
    } else {
        // If the configurate name is empty or the configuration name is taken, keep old configuration name
        ui->configurations_list->blockSignals(true);
        item->setText(item->key.c_str());  // "item->key" stored configuration old name
        ui->configurations_list->blockSignals(false);

        configurator.SetActiveConfigurationName(item->key);
    }

    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
}

void TabConfigurations::OnContextMenuNewClicked(ListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    Configuration &configuration = configurator.configurations.CreateConfiguration(configurator.layers, "New Configuration");
    configurator.SetActiveConfigurationName(configuration.key);
    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
    this->UpdateUI_LoaderMessages();
    this->UpdateUI_Layers(UPDATE_REBUILD_UI);
    this->UpdateUI_Settings(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuImportClicked(ListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    const Path &path_import = configurator.configurations.last_path_import_config;
    const std::string selected_path = QFileDialog::getOpenFileName(&this->window, "Import Layers Configuration File",
                                                                   path_import.AbsolutePath().c_str(), "JSON configuration(*.json)")
                                          .toStdString();

    if (selected_path.empty()) {
        return;
    }

    std::string configuration_name;
    const bool result = configurator.configurations.ImportConfiguration(configurator.layers, selected_path, configuration_name);

    if (result) {
        configurator.SetActiveConfigurationName(configuration_name);
        configurator.Override(OVERRIDE_AREA_ALL);

        this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
        this->UpdateUI_LoaderMessages();
        this->UpdateUI_Layers(UPDATE_REBUILD_UI);
        this->UpdateUI_Settings(UPDATE_REBUILD_UI);
    } else {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Importing of a layers Configuration file failed...");
        msg.setText(format("%s is not a valid layers configuration file or it could not be read.", selected_path.c_str()).c_str());
        msg.exec();
    }
}

void TabConfigurations::OnContextMenuRenameClicked(ListItem *item) {
    assert(item);

    ui->configurations_list->editItem(item);
}

void TabConfigurations::OnContextMenuDuplicateClicked(ListItem *item) {
    assert(item);
    assert(!item->key.empty());

    Configurator &configurator = Configurator::Get();
    const Configuration &duplicated_configuration =
        configurator.configurations.DuplicateConfiguration(configurator.layers, item->key);

    item->key = duplicated_configuration.key;
    configurator.SetActiveConfigurationName(item->key);

    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuDeleteClicked(ListItem *item) {
    assert(item);
    assert(!item->key.empty());

    // Let make sure...
    QMessageBox alert;
    alert.setWindowTitle(format("Removing *%s* configuration...", item->key.c_str()).c_str());
    alert.setText(format("Are you sure you want to remove the *%s* configuration?", item->key.c_str()).c_str());
    alert.setInformativeText("All the data from this configuration will be lost.");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Warning);
    if (alert.exec() == QMessageBox::No) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    configurator.configurations.RemoveConfiguration(item->key);
    configurator.SetActiveConfigurationName("");

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuResetOneClicked(ListItem *item) {
    assert(item);
    assert(!item->key.empty());

    Configurator &configurator = Configurator::Get();
    Configuration *configuration = configurator.configurations.FindConfiguration(item->key);
    assert(configuration != nullptr);

    QMessageBox alert;
    alert.setWindowTitle(format("Resetting *%s* loader configuration...", configuration->key.c_str()).c_str());
    alert.setText(format("Are you sure you want to reset the *%s* loader configuration?", configuration->key.c_str()).c_str());
    if (configuration->IsDefault())
        alert.setInformativeText(
            format("The loader configuration, including layers settings, will be restored to default built-in *%s* configuration.",
                   configuration->key.c_str())
                .c_str());
    else if (configurator.configurations.HasFile(*configuration))
        alert.setInformativeText(format("The loader configuration, including layers settings, will be reloaded using the *%s* "
                                        "saved file from previous %s run.",
                                        configuration->key.c_str(), VKCONFIG_NAME)
                                     .c_str());
    else
        alert.setInformativeText("The configuration layers and settings will be reset to zero.");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Warning);
    if (alert.exec() == QMessageBox::No) {
        return;
    }

    configuration->Reset(configurator.layers);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuResetAllClicked(ListItem *item) {
    (void)item;

    Configurator &configurator = Configurator::Get();

    QMessageBox alert;
    alert.setWindowTitle("Resetting all default loader configurations...");
    alert.setText("Are you sure you want to reset the default loader configurations?");
    alert.setInformativeText(
        "The loader configurations, including layers settings, will be restored to default built-in configurations.");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Warning);
    if (alert.exec() == QMessageBox::No) {
        return;
    }

    configurator.configurations.ResetDefaultConfigurations(configurator.layers);
    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuExportConfigsClicked(ListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    const Path path_export = configurator.configurations.last_path_export_config.RelativePath() + "/" + item->key + ".json";
    const std::string &selected_path =
        QFileDialog::getSaveFileName(&this->window, "Export Loader Configuration File", path_export.AbsolutePath().c_str(),
                                     "JSON configuration(*.json)")
            .toStdString();

    if (selected_path.empty()) {
        return;
    }

    const bool result = configurator.configurations.ExportConfiguration(configurator.layers, selected_path, item->key);

    if (!result) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Exporting of a Loader Configuration file failed...");
        msg.setText(format("Couldn't be create '%s' Loader configuration file.", selected_path.c_str()).c_str());
        msg.exec();
    } else {
        QDesktopServices::openUrl(QUrl::fromLocalFile(selected_path.c_str()));
    }
}

void TabConfigurations::OnContextMenuExportSettingsClicked(ListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    const Path path_export = configurator.configurations.last_path_export_settings;
    const std::string &selected_path = QFileDialog::getSaveFileName(&this->window, "Export Layers Settings File",
                                                                    path_export.AbsolutePath().c_str(), "Layers settings(*.txt)")
                                           .toStdString();

    if (selected_path.empty()) {
        return;
    }

    configurator.configurations.last_path_export_settings = selected_path;
    const bool result = configurator.WriteLayersSettings(OVERRIDE_AREA_LAYERS_SETTINGS_BIT, selected_path.c_str());

    if (!result) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Exporting of a layers settings file failed...");
        msg.setText(format("Couldn't be create '%s' layers settings file.", selected_path.c_str()).c_str());
        msg.exec();
    } else {
        QDesktopServices::openUrl(QUrl::fromLocalFile(selected_path.c_str()));
    }
}

void TabConfigurations::on_configurations_advanced_toggle_pressed() {
    Configurator &configurator = Configurator::Get();
    configurator.advanced = !configurator.advanced;

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabConfigurations::on_configurations_executable_scope_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();

    const ExecutableScope scope = static_cast<ExecutableScope>(index);
    switch (scope) {
        default: {
            configurator.SetExecutableScope(scope);
            configurator.Override(OVERRIDE_AREA_ALL);
            break;
        }
        case EXECUTABLE_PER: {
            if (!(configurator.Get(HIDE_MESSAGE_NOTIFICATION_EXECUTABLE_PER))) {
                QMessageBox message;
                message.setWindowTitle(format("Selected scope: '%s'", ::GetLabel(scope)).c_str());
                message.setText(::GetTooltip(scope));
                message.setInformativeText(
                    "As the vk_layer_settings.txt file is written in the executable working directory, all the executables "
                    "with the same working directory will share the same loader configuration.\n\nDo you want to continue?");
                message.setIcon(QMessageBox::Information);
                message.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                message.setDefaultButton(QMessageBox::Ok);
                message.setCheckBox(new QCheckBox("Do not show again."));
                int retval = message.exec();

                if (message.checkBox()->isChecked()) {
                    configurator.Set(HIDE_MESSAGE_NOTIFICATION_EXECUTABLE_PER);
                }

                if (retval == QMessageBox::Cancel) {
                    break;
                }
            }

            configurator.SetExecutableScope(scope);
            configurator.Override(OVERRIDE_AREA_ALL);
            break;
        }
    }

    this->UpdateUI(UPDATE_REFRESH_UI);
    this->window.UpdateUI_Status();
}

void TabConfigurations::on_configurations_executable_list_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.executables.SetActiveExecutable(index);

    ExecutableScope scope = configurator.GetExecutableScope();
    assert(::EnabledExecutables(scope));

    const Executable *executable = configurator.GetActiveExecutable();
    const std::string path = executable->path.RelativePath();

    this->ui->configurations_executable_list->setToolTip(executable->path.AbsolutePath().c_str());
    this->ui->configurations_group_box_list->setChecked(executable->enabled);

    this->UpdateUI(UPDATE_REFRESH_UI);
    this->window.UpdateUI_Status();
}

void TabConfigurations::on_configurations_executable_append_pressed() {
    Configurator &configurator = Configurator::Get();

    const Path &last_path = configurator.executables.last_path_executable;
    const Path selected_path =
        QFileDialog::getOpenFileName(&this->window, "Executable Path", last_path.AbsolutePath().c_str(), ::GetExecutableFilter())
            .toStdString();

    if (selected_path.Empty()) {
        return;
    }

    configurator.executables.AppendExecutable(selected_path);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabConfigurations::on_configurations_executable_remove_pressed() {
    Configurator &configurator = Configurator::Get();
    const Executable *executable = configurator.executables.GetActiveExecutable();
    assert(executable != nullptr);

    if (!(configurator.Get(HIDE_MESSAGE_WARN_REMOVE_EXECUTABLE))) {
        QMessageBox message;
        message.setWindowTitle(format("Removing an executable from %s", VKCONFIG_NAME).c_str());
        message.setText(format("The '%s' executable is being removed from %s with it's options that will be definitly lost.",
                               VKCONFIG_NAME, executable->path.AbsolutePath().c_str())
                            .c_str());
        message.setInformativeText("Do you want to continue?");
        message.setIcon(QMessageBox::Warning);
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message.setDefaultButton(QMessageBox::Yes);
        message.setCheckBox(new QCheckBox("Do not show again."));
        int ret_val = message.exec();

        if (message.checkBox()->isChecked()) {
            configurator.Set(HIDE_MESSAGE_WARN_REMOVE_EXECUTABLE);
        }

        if (ret_val == QMessageBox::No) {
            return;
        }
    }

    configurator.Surrender(OVERRIDE_AREA_ALL);
    configurator.executables.RemoveExecutable();
    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabConfigurations::on_configurations_list_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.GetActiveExecutable();
    if (executable != nullptr) {
        executable->enabled = checked;

        this->UpdatePerExecutableConfigurations();

        configurator.Override(OVERRIDE_AREA_ALL);
    }

    this->UpdateUI(UPDATE_REFRESH_UI);
}

void TabConfigurations::on_configurations_layers_ordering_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        configuration->override_layers = checked;

        configurator.Override(OVERRIDE_AREA_ALL);
    }

    this->UpdateUI(UPDATE_REFRESH_UI);
}

void TabConfigurations::on_configurations_loader_messages_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        configuration->override_loader = checked;

        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
    }
}

void TabConfigurations::on_configurations_layers_settings_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();

    Parameter *parameter = configurator.GetActiveParameter();
    if (parameter != nullptr) {
        parameter->override_settings = checked;

        configurator.Override(OVERRIDE_AREA_LAYERS_SETTINGS_BIT);
    }

    // The layer version combobox is on even when the layer settings group is unchecked
    this->UpdateUI_Settings(UPDATE_REFRESH_UI);
}

void TabConfigurations::OnCheckedLoaderMessageTypes(bool checked) {
    (void)checked;

    Configurator &configurator = Configurator::Get();

    Configuration *active_configuration = configurator.GetActiveConfiguration();
    if (active_configuration != nullptr) {
        int loader_log_messages_bits = 0;
        loader_log_messages_bits |= this->ui->configuration_loader_errors->isChecked() ? GetBit(LOG_ERROR) : 0;
        loader_log_messages_bits |= this->ui->configuration_loader_warns->isChecked() ? GetBit(LOG_WARN) : 0;
        loader_log_messages_bits |= this->ui->configuration_loader_infos->isChecked() ? GetBit(LOG_INFO) : 0;
        loader_log_messages_bits |= this->ui->configuration_loader_debug->isChecked() ? GetBit(LOG_DEBUG) : 0;
        loader_log_messages_bits |= this->ui->configuration_loader_layers->isChecked() ? GetBit(LOG_LAYER) : 0;
        loader_log_messages_bits |= this->ui->configuration_loader_drivers->isChecked() ? GetBit(LOG_DRIVER) : 0;
        active_configuration->loader_log_messages_flags = loader_log_messages_bits;

        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
    }
}

void TabConfigurations::on_configuration_loader_errors_toggled(bool checked) { this->OnCheckedLoaderMessageTypes(checked); }

void TabConfigurations::on_configuration_loader_warns_toggled(bool checked) { this->OnCheckedLoaderMessageTypes(checked); }

void TabConfigurations::on_configuration_loader_infos_toggled(bool checked) { this->OnCheckedLoaderMessageTypes(checked); }

void TabConfigurations::on_configuration_loader_debug_toggled(bool checked) { this->OnCheckedLoaderMessageTypes(checked); }

void TabConfigurations::on_configuration_loader_layers_toggled(bool checked) { this->OnCheckedLoaderMessageTypes(checked); }

void TabConfigurations::on_configuration_loader_drivers_toggled(bool checked) { this->OnCheckedLoaderMessageTypes(checked); }

void TabConfigurations::on_configurations_list_itemDoubleClicked(QListWidgetItem *item) { ui->configurations_list->editItem(item); }

/// An item has been changed. Check for edit of the items name (configuration name)
void TabConfigurations::on_configurations_list_itemChanged(QListWidgetItem *item) { this->OnRenameConfiguration(item); }

void TabConfigurations::UpdatePerExecutableConfigurations() {
    Configurator &configurator = Configurator::Get();
    if (configurator.GetExecutableScope() != EXECUTABLE_PER) {
        return;
    }

    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration == nullptr) {
        return;
    }

    std::vector<Path> updated_executable_paths;
    if (configurator.executables.UpdateConfigurations(updated_executable_paths)) {
        if (!configurator.Get(HIDE_MESSAGE_NOTIFICATION_PER_CONFIG_UPDATE)) {
            QMessageBox message;
            message.setWindowTitle("Vulkan Executable Loader Configuration Updated");
            message.setText(
                format("Per Executable Loader Configuration Scope requires that all executables with the same working "
                       "directory use the same loader configuration. The following executables were switch to '%s' configuration:",
                       configuration->key.c_str())
                    .c_str());
            std::string informative;
            for (std::size_t i = 0, n = updated_executable_paths.size(); i < n; ++i) {
                informative += format("- %s\n", updated_executable_paths[i].RelativePath().c_str());
            }

            message.setInformativeText(informative.c_str());
            message.setIcon(QMessageBox::Warning);
            message.setCheckBox(new QCheckBox("Do not show again."));
            message.exec();

            if (message.checkBox()->isChecked()) {
                configurator.Set(HIDE_MESSAGE_NOTIFICATION_PER_CONFIG_UPDATE);
            }
        }
    }
}

void TabConfigurations::on_configurations_list_currentRowChanged(int currentRow) {
    this->_settings_tree_manager.CleanupGUI();

    if (currentRow == -1) {
        return;  // No row selected
    }

    QListWidgetItem *list_item = ui->configurations_list->item(currentRow);
    ListItem *item = dynamic_cast<ListItem *>(list_item);

    if (item == nullptr) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    const Configuration *selected_configuration = configurator.configurations.FindConfiguration(item->key);

    if (configurator.GetActiveConfiguration() != selected_configuration) {
        configurator.SetActiveConfigurationName(item->key);

        if (!configurator.Get(HIDE_MESSAGE_WARN_MISSING_LAYERS_IGNORE)) {
            std::vector<std::string> missing_layers;

            const Configuration *configuration = configurator.GetActiveConfiguration();
            if (configuration->HasMissingLayer(configurator.layers, missing_layers)) {
                std::string text = format("The selected '%s' configuration is missing the layers:\n", configuration->key.c_str());
                for (std::size_t i = 0, n = missing_layers.size(); i < n; ++i) {
                    text += format(" - %s\n", missing_layers[i].c_str());
                }
                text += "These layers will be ignored.";

                QMessageBox alert;
                alert.setWindowTitle("Missing Vulkan layers...");
                alert.setText(text.c_str());
                alert.setIcon(QMessageBox::Warning);
                alert.setCheckBox(new QCheckBox("Do not show again."));
                alert.setInformativeText(format("Use the '%s' tab to add the missing layers.", GetLabel(TAB_LAYERS)).c_str());

                alert.exec();
                if (alert.checkBox()->isChecked()) {
                    configurator.Set(HIDE_MESSAGE_WARN_MISSING_LAYERS_IGNORE);
                }
            }
        }

        this->UpdatePerExecutableConfigurations();

        configurator.Override(OVERRIDE_AREA_ALL);

        this->UpdateUI(UPDATE_REBUILD_UI);
    }
}

void TabConfigurations::on_configurations_layers_list_currentRowChanged(int currentRow) {
    this->_settings_tree_manager.CleanupGUI();

    if (currentRow == -1) {
        return;  // No row selected
    }

    QWidget *widget = this->ui->configurations_layers_list->itemWidget(this->ui->configurations_layers_list->item(currentRow));
    if (widget == nullptr) {
        return;
    }

    const std::string &layer_string = static_cast<ConfigurationLayerWidget *>(widget)->layer_name;

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    assert(configuration != nullptr);

    if (configuration->selected_layer_name != layer_string) {
        configuration->selected_layer_name = layer_string;

        this->UpdateUI_Settings(UPDATE_REBUILD_UI);
    }
}

void TabConfigurations::on_configurations_layerVersionChanged() { this->UpdateUI_Layers(UPDATE_REBUILD_UI); }
