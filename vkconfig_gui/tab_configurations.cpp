/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/ui.h"
#include "../vkconfig_core/doc.h"
#include "../vkconfig_core/type_hide_message.h"

#include <QSettings>
#include <QMenu>
#include <QFileDialog>
#include <QDesktopServices>

TabConfigurations::TabConfigurations(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui)
    : Tab(TAB_CONFIGURATIONS, window, ui), _settings_tree_manager(ui) {
    ui->configurations_list->installEventFilter(&window);
    ui->configurations_layers_list->installEventFilter(&window);
    ui->configurations_settings->installEventFilter(&window);

    this->connect(this->ui->configurations_executable_scope, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_configurations_executable_scope_currentIndexChanged(int)));
    this->connect(this->ui->configurations_executable_list, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_configurations_executable_list_currentIndexChanged(int)));
    this->connect(this->ui->configurations_executable_append, SIGNAL(pressed()), this,
                  SLOT(on_configurations_executable_append_pressed()));

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
    this->on_configurations_executable_scope_currentIndexChanged(current_scope);

    this->ui->configurations_executable_scope->blockSignals(false);

    this->ui_configurations_group_box_settings_tooltip();

    this->advanced_mode = new ResizeButton(this->ui->configurations_group_box_layers, 0);
    this->advanced_mode->setMinimumSize(24, 24);
    this->advanced_mode->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->advanced_mode->adjustSize();
    this->ui->configurations_group_box_layers->installEventFilter(this->advanced_mode);

    this->connect(this->advanced_mode, SIGNAL(pressed()), this, SLOT(on_configurations_advanced_toggle_pressed()));

    this->ui_configurations_advanced_toggle();
}

TabConfigurations::~TabConfigurations() {
    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    settings.setValue("vkconfig3/mainwindow/splitter_main_state", ui->splitter_main->saveState());
    settings.setValue("vkconfig3/mainwindow/splitter_configurations_state", ui->splitter_configurations->saveState());
    settings.setValue("vkconfig3/mainwindow/splitter_settings_state", ui->splitter_settings->saveState());
}

void TabConfigurations::UpdateUI_Configurations(UpdateUIMode ui_update_mode) {
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
        if (configurator.GetActionConfigurationName() == configuration.key) {
            item->setIcon(QIcon(":/resourcefiles/system-on.png"));
            item->setToolTip(format("Using the %s configuration with Vulkan executables", configuration.key.c_str()).c_str());
            ui->configurations_group_box_layers->setChecked(configuration.override_layers);
            ui->configurations_group_box_loader->setChecked(configuration.override_loader);
            current_row = static_cast<int>(i);
        } else if (has_missing_layer) {
            item->setIcon(QIcon(":/resourcefiles/system-invalid.png"));
            item->setToolTip(
                format("The %s configuration has missing layers. These layers are ignored.", configuration.key.c_str()).c_str());
        } else {
            item->setIcon(QIcon(":/resourcefiles/system-off.png"));
            item->setToolTip(
                format("Select the %s configuration to use it with Vulkan executables", configuration.key.c_str()).c_str());
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

    if (!executables.empty()) {
        this->ui->configurations_executable_list->blockSignals(true);

        if (ui_update_mode == UPDATE_REBUILD_UI) {
            this->ui->configurations_executable_list->clear();
            for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
                const Executable &executable = executables[i];

                this->ui->configurations_executable_list->addItem(executable.path.RelativePath().c_str());
            }
        }

        const Executable *executable = configurator.executables.GetActiveExecutable();
        this->ui->configurations_executable_list->setCurrentIndex(configurator.executables.GetActiveExecutableIndex());
        this->ui->configurations_executable_list->blockSignals(false);
    }
}

void TabConfigurations::UpdateUI_LoaderMessages() {
    const Configurator &configurator = Configurator::Get();

    const Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        ui->configuration_loader_errors->blockSignals(true);
        ui->configuration_loader_errors->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_ERROR));
        ui->configuration_loader_errors->blockSignals(false);
        ui->configuration_loader_warns->blockSignals(true);
        ui->configuration_loader_warns->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_WARN));
        ui->configuration_loader_warns->blockSignals(false);
        ui->configuration_loader_infos->blockSignals(true);
        ui->configuration_loader_infos->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_INFO));
        ui->configuration_loader_infos->blockSignals(false);
        ui->configuration_loader_debug->blockSignals(true);
        ui->configuration_loader_debug->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_DEBUG));
        ui->configuration_loader_debug->blockSignals(false);
        ui->configuration_loader_layers->blockSignals(true);
        ui->configuration_loader_layers->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_LAYER));
        ui->configuration_loader_layers->blockSignals(false);
        ui->configuration_loader_drivers->blockSignals(true);
        ui->configuration_loader_drivers->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_DRIVER));
        ui->configuration_loader_drivers->blockSignals(false);
    }
}

void TabConfigurations::UpdateUI_Layers(UpdateUIMode mode) {
    ui->configurations_layers_list->blockSignals(true);
    ui->configurations_layers_list->clear();

    Configurator &configurator = Configurator::Get();
    const std::string &selected_contiguration_name = configurator.GetActionConfigurationName();
    const bool has_selected_configuration = !selected_contiguration_name.empty();

    if (has_selected_configuration) {
        Configuration *configuration = configurator.configurations.FindConfiguration(selected_contiguration_name);
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

                ListItem *item = new ListItem(parameter.key.c_str());
                item->setFlags(item->flags() | Qt::ItemIsSelectable);
                if (has_multiple_parameter) {
                    item->setIcon(QIcon(":/resourcefiles/drag.png"));
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
    }

    ui->configurations_layers_list->blockSignals(false);
}

void TabConfigurations::UpdateUI_Settings(UpdateUIMode mode) {
    Configurator &configurator = Configurator::Get();

    if (configurator.GetActionConfigurationName().empty()) {
        this->_settings_tree_manager.CleanupGUI();
        this->ui->configurations_presets->setVisible(false);
    } else {
        this->_settings_tree_manager.CreateGUI();
    }
}

void TabConfigurations::UpdateUI(UpdateUIMode ui_update_mode) {
    this->UpdateUI_Configurations(ui_update_mode);
    this->UpdateUI_Applications(ui_update_mode);
    this->UpdateUI_LoaderMessages();
    this->UpdateUI_Layers(ui_update_mode);
    this->UpdateUI_Settings(ui_update_mode);
}

void TabConfigurations::CleanUI() { this->_settings_tree_manager.CleanupGUI(); }

bool TabConfigurations::EventFilter(QObject *target, QEvent *event) {
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
            const std::string &layer_name = static_cast<ConfigurationLayerWidget *>(widget)->layer_name;
            layer_names.push_back(layer_name);
        }

        Configuration *configuration = configurator.GetActiveConfiguration();
        if (configuration != nullptr) {
            configuration->Reorder(layer_names);
            configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
        }

        return true;
    }

    if (configurator.GetExecutableScope() == EXECUTABLE_NONE) {
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

            QAction *action_reset = new QAction("Reset the Configuration", nullptr);
            action_reset->setEnabled(item != nullptr);
            action_reset->setToolTip("Reset the Configuration using the built-in configuration");
            menu.addAction(action_reset);

            QAction *action_reload = new QAction("Reload the Configuration", nullptr);
            action_reload->setEnabled(item != nullptr);
            action_reset->setToolTip("Reload the Configuration, discarding all changes from this session");
            menu.addAction(action_reload);

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
            } else if (action == action_reset) {
                this->OnContextMenuResetClicked(item);
            } else if (action == action_reload) {
                this->OnContextMenuReloadClicked(item);
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

            const Layer *layer = configurator.layers.Find(item->key, Version::LATEST);

            QMenu menu(ui->configurations_layers_list);

            QAction *action_description = new QAction("Open the Layer Description...", nullptr);
            action_description->setEnabled(item != nullptr);
            menu.addAction(action_description);

            QAction *export_html_action = new QAction("Open the Layer HTML Documentation...", nullptr);
            export_html_action->setEnabled(item != nullptr);
            menu.addAction(export_html_action);

            QAction *export_markdown_action = new QAction("Open the Layer Markdown Documentation...", nullptr);
            export_markdown_action->setEnabled(item != nullptr);
            menu.addAction(export_markdown_action);

            QAction *visit_layer_website_action = new QAction("Visit the Layer Website...", nullptr);
            visit_layer_website_action->setEnabled(layer != nullptr ? !layer->url.empty() : false);
            menu.addAction(visit_layer_website_action);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            if (action == action_description) {
                Alert::LayerProperties(layer);
            } else if (action == visit_layer_website_action) {
                QDesktopServices::openUrl(QUrl(layer->url.c_str()));
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

    // Pass it on
    return false;
}

void TabConfigurations::OnSelectConfiguration(int currentRow) {
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
    if (configurator.GetActionConfigurationName() != item->key) {
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

                int ret_val = alert.exec();
                if (alert.checkBox()->isChecked()) {
                    configurator.Set(HIDE_MESSAGE_WARN_MISSING_LAYERS_IGNORE);
                }
            }
        }

        configurator.Override(OVERRIDE_AREA_ALL);

        this->UpdateUI_Configurations(UPDATE_REFRESH_UI);
        this->UpdateUI_LoaderMessages();
        this->UpdateUI_Layers(UPDATE_REBUILD_UI);
        this->UpdateUI_Settings(UPDATE_REBUILD_UI);
    }
}

void TabConfigurations::OnRenameConfiguration(QListWidgetItem *list_item) {
    ListItem *item = dynamic_cast<ListItem *>(list_item);
    if (item == nullptr) {
        return;
    }

    Configurator &configurator = Configurator::Get();

    // We are renaming the file. Things can go wrong here...
    // This is the name of the configuratin we are changing
    // const std::string full_path =
    //    ConvertNativeSeparators(GetPath(BUILTIN_PATH_CONFIG_LAST) + "/" + configuration_item->configuration_name + ".json");

    // This is the new name we want to use for the configuration
    const std::string &new_name = item->text().toStdString();
    bool valid_new_name = true;

    if (new_name.empty()) {
        valid_new_name = false;
        Alert::ConfigurationNameEmpty();
    } else if (!IsPortableFilename(new_name)) {
        valid_new_name = false;
        Alert::ConfigurationNameInvalid();
    }

    Configuration *duplicate_configuration = configurator.configurations.FindConfiguration(new_name);

    if (duplicate_configuration != nullptr) {
        valid_new_name = false;
        Alert::ConfigurationRenamingFailed();
    }

    // Find existing configuration using it's old name
    const std::string &old_name = item->key;
    Configuration *configuration = configurator.configurations.FindConfiguration(old_name);

    if (valid_new_name) {
        // Rename configuration ; Remove old configuration file ; change the name of the configuration
        configurator.configurations.RemoveConfigurationFile(old_name);
        configuration->key = item->key = new_name;
        configurator.SetActiveConfigurationName(new_name);

        this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
        this->UpdateUI_LoaderMessages();
        this->UpdateUI_Layers(UPDATE_REBUILD_UI);
    } else {
        // If the configurate name is empty or the configuration name is taken, keep old configuration name
        ui->configurations_list->blockSignals(true);
        item->setText(old_name.c_str());
        ui->configurations_list->blockSignals(false);

        configurator.SetActiveConfigurationName(old_name);
    }

    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
}

void TabConfigurations::OnSelectLayer(int currentRow) {
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
        if (layer_string == ::GetLabel(LAYER_BUILTIN_API)) {
            if (!configurator.Get(HIDE_MESSAGE_NOTIFICATION_CONTROL_APPLICATION_API)) {
                QMessageBox message;
                message.setIcon(QMessageBox::Information);
                message.setWindowTitle(::GetLabel(LAYER_BUILTIN_API));
                message.setText(
                    "This item refers to Vulkan Layers not visible by Vulkan Configurator but at Vulkan Application launched, but "
                    "enabled by the Vulkan Application at "
                    "Vulkan Layers that are located using 'VK_ADD_LAYER_PATH' and enabled using 'vkCreateInstance'.");
                message.setInformativeText(
                    format("If the Vulkan application is enabling a layer already controlled by Vulkan Configurator, the Vulkan "
                           "Application setup is ignored.\n\nTo force ignoring these Vulkan Layers, set '%s' to 'Off'.",
                           ::GetLabel(LAYER_BUILTIN_API))
                        .c_str());
                message.setCheckBox(new QCheckBox("Do not show again."));
                message.exec();
                if (message.checkBox()->isChecked()) {
                    configurator.Set(HIDE_MESSAGE_NOTIFICATION_CONTROL_APPLICATION_API);
                }
            }
        } else if (layer_string == ::GetLabel(LAYER_BUILTIN_ENV)) {
            if (!configurator.Get(HIDE_MESSAGE_NOTIFICATION_CONTROL_APPLICATION_ENV)) {
                QMessageBox message;
                message.setIcon(QMessageBox::Information);
                message.setWindowTitle(::GetLabel(LAYER_BUILTIN_ENV));
                message.setText(
                    "This item refers to Vulkan Layers not visible by Vulkan Configurator but at Vulkan Application launched, "
                    "Vulkan Layers that are located using 'VK_ADD_LAYER_PATH' and enabled using 'VK_LOADER_LAYERS_ENABLE' "
                    "Environment Variables.");
                message.setInformativeText(format("If the Vulkan application is enabling a layer already controlled by Vulkan "
                                                  "Configurator, the Vulkan Application "
                                                  "setup is ignored.\n\nTo force ignoring these Vulkan Layers, set '%s' to 'Off'.",
                                                  ::GetLabel(LAYER_BUILTIN_ENV))
                                               .c_str());
                message.setCheckBox(new QCheckBox("Do not show again."));
                message.exec();
                if (message.checkBox()->isChecked()) {
                    configurator.Set(HIDE_MESSAGE_NOTIFICATION_CONTROL_APPLICATION_ENV);
                }
            }
        }

        configuration->selected_layer_name = layer_string;

        this->_settings_tree_manager.CreateGUI();
    }
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

void TabConfigurations::OnContextMenuNewClicked(ListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    configurator.configurations.CreateConfiguration(configurator.layers, "New Configuration");
    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
    this->UpdateUI_LoaderMessages();
    this->UpdateUI_Layers(UPDATE_REBUILD_UI);
    this->UpdateUI_Settings(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuImportClicked(ListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    const Path &path_import = configurator.configurations.last_path_import;
    const std::string selected_path = QFileDialog::getOpenFileName(&this->window, "Import Layers Configuration File",
                                                                   path_import.AbsolutePath().c_str(), "JSON configuration(*.json)")
                                          .toStdString();

    if (selected_path.empty()) {
        return;
    }

    const bool result = configurator.configurations.ImportConfiguration(configurator.layers, selected_path);

    if (result) {
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

    this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
    this->UpdateUI_Applications(UPDATE_REFRESH_UI);
    this->UpdateUI_LoaderMessages();
    this->UpdateUI_Layers(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuResetClicked(ListItem *item) {
    assert(item);
    assert(!item->key.empty());

    Configurator &configurator = Configurator::Get();
    Configuration *configuration = configurator.configurations.FindConfiguration(item->key);
    assert(configuration != nullptr);

    QMessageBox alert;
    alert.setWindowTitle(format("Resetting *%s* configuration...", configuration->key.c_str()).c_str());
    alert.setText(format("Are you sure you want to reset the *%s* configuration?", configuration->key.c_str()).c_str());
    if (configuration->IsBuiltIn())
        alert.setInformativeText(
            format("The configuration layers and settings will be restored to default built-in *%s* configuration.",
                   configuration->key.c_str())
                .c_str());
    else if (configurator.configurations.HasFile(*configuration))
        alert.setInformativeText(
            format("The configuration layers and settings will be reloaded using the *%s* saved file from previous %s run.",
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

    this->UpdateUI_Configurations(UPDATE_REFRESH_UI);
}

void TabConfigurations::OnContextMenuReloadClicked(ListItem *item) {
    assert(item);
    assert(!item->key.empty());

    // TODO
    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle("TODO");
    msg.exec();
}

void TabConfigurations::OnContextMenuExportConfigsClicked(ListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    const Path path_export = configurator.configurations.last_path_export + "/" + item->key + ".json";
    const std::string &selected_path =
        QFileDialog::getSaveFileName(&this->window, "Export Layers Configuration File", path_export.AbsolutePath().c_str(),
                                     "JSON configuration(*.json)")
            .toStdString();

    if (selected_path.empty()) {
        return;
    }

    const bool result = configurator.configurations.ExportConfiguration(configurator.layers, selected_path, item->key);

    if (!result) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Critical);
        msg.setWindowTitle("Exporting of a layers Configuration file failed...");
        msg.setText(format("Couldn't be create %s layers configuration file.", selected_path.c_str()).c_str());
        msg.exec();
    } else if (!(configurator.Get(HIDE_MESSAGE_NOTIFICATION_EXPORT))) {
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setWindowTitle("Exporting of a layers Configuration file successful.");
        msg.setText(format("%s layers configuration file was created.", selected_path.c_str()).c_str());
        msg.setCheckBox(new QCheckBox("Do not show again."));
        msg.exec();

        if (msg.checkBox()->isChecked()) {
            configurator.Set(HIDE_MESSAGE_NOTIFICATION_EXPORT);
        }
    }
}

void TabConfigurations::OnContextMenuExportSettingsClicked(ListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    // TODO
    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setWindowTitle("TODO");
    msg.exec();
}

void TabConfigurations::ui_configurations_group_box_list_tooltip() {
    Configurator &configurator = Configurator::Get();
    const ExecutableScope scope = configurator.GetExecutableScope();

    if (scope == EXECUTABLE_PER || scope == EXECUTABLE_ALL) {
        const Executable *executable = configurator.GetActiveExecutable();
        const std::string path = executable->path.RelativePath();

        if (scope == EXECUTABLE_PER) {
            this->ui->configurations_group_box_list->setToolTip(
                format("Select a configuration applied to '%s' executable", path.c_str()).c_str());
        } else {
            const std::string state =
                configurator.executables.GetActiveExecutable()->enabled ? "Uncheck to disable" : "Check to enable";
            this->ui->configurations_group_box_list->setToolTip(
                format("%s the configuration for '%s' executable", state.c_str(), path.c_str()).c_str());
        }

    } else if (scope == EXECUTABLE_ANY) {
        this->ui->configurations_group_box_list->setToolTip("Select the active layers configuration");
    } else {
        this->ui->configurations_group_box_list->setToolTip("");
    }
}

void TabConfigurations::ui_configurations_group_box_settings_tooltip() {
    Configurator &configurator = Configurator::Get();

    const Parameter *parameter = configurator.GetActiveParameter();
    if (parameter != nullptr) {
        const std::string state = parameter->override_settings ? "Uncheck to disable" : "Check to enable";
        const std::string tooltip = format("%s '%s' layer settings", state.c_str(), parameter->key.c_str());

        this->ui->configurations_group_box_settings->setToolTip(tooltip.c_str());
    }
}

void TabConfigurations::ui_configurations_advanced_toggle() {
    Configurator &configurator = Configurator::Get();

    if (this->advanced_mode) {
        if (configurator.advanced) {
            this->advanced_mode->setIcon(QIcon(":/resourcefiles/settings_basic.png"));
            this->advanced_mode->setToolTip("Click to switch to basic Layers Configuration mode");
        } else {
            this->advanced_mode->setIcon(QIcon(":/resourcefiles/settings_advanced.png"));
            this->advanced_mode->setToolTip("Click to switch to advanced Layers Configuration mode");
        }
    }

    // this->ui->configurations_group_box_layers->setCheckable(configurator.advanced);
    // this->ui->configurations_group_box_loader->setCheckable(configurator.advanced);
    this->ui->configurations_group_box_settings->setCheckable(configurator.advanced);
}

void TabConfigurations::on_configurations_advanced_toggle_pressed() {
    Configurator &configurator = Configurator::Get();
    configurator.advanced = !configurator.advanced;

    this->ui_configurations_advanced_toggle();

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabConfigurations::on_configurations_executable_scope_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();

    const ExecutableScope scope = static_cast<ExecutableScope>(index);

    configurator.SetExecutableScope(scope);
    configurator.Override(OVERRIDE_AREA_ALL);

    this->ui->configurations_executable_scope->setToolTip(GetTooltip(scope));
    this->ui->configurations_executable_list->setEnabled(index == EXECUTABLE_ALL || index == EXECUTABLE_PER);
    this->ui->configurations_executable_append->setEnabled(index == EXECUTABLE_ALL || index == EXECUTABLE_PER);

    this->ui->configurations_group_box_list->setEnabled(index != EXECUTABLE_NONE);
    this->ui->configurations_group_box_list->setCheckable(index == EXECUTABLE_ALL);
    this->ui->configurations_group_box_list->setChecked(configurator.executables.GetActiveExecutable()->enabled);

    this->ui_configurations_group_box_list_tooltip();

    const Configuration *configuration = configurator.GetActiveConfiguration();
    const bool enabled_layers = index != EXECUTABLE_NONE && configuration->override_layers;

    this->ui->configurations_group_box_layers->setEnabled(enabled_layers);
    this->ui->configurations_group_box_loader->setEnabled(index != EXECUTABLE_NONE && configuration->override_loader);
    this->ui->configurations_group_box_settings->setEnabled(configuration->override_layers && configurator.HasActiveSettings());

    this->UpdateUI(UPDATE_REFRESH_UI);
    this->window.UpdateUI_Status();
}

void TabConfigurations::on_configurations_executable_list_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.executables.SetActiveExecutable(index);

    ExecutableScope scope = configurator.GetExecutableScope();
    assert(scope == EXECUTABLE_PER || scope == EXECUTABLE_ALL);

    const Executable *executable = configurator.GetActiveExecutable();
    const std::string path = executable->path.RelativePath();

    this->ui->configurations_executable_list->setToolTip(executable->path.AbsolutePath().c_str());
    this->ui->configurations_group_box_list->setChecked(executable->enabled);
    if (configurator.GetExecutableScope() == EXECUTABLE_PER) {
        const Configuration *configuration = configurator.GetActiveConfiguration();
        this->ui->configurations_group_box_layers->setEnabled(configuration->override_layers);
        this->ui->configurations_group_box_loader->setEnabled(configuration->override_loader);
        this->ui->configurations_group_box_settings->setEnabled(configuration->override_layers && configurator.HasActiveSettings());
    }

    this->ui_configurations_group_box_list_tooltip();

    this->UpdateUI(UPDATE_REFRESH_UI);
    this->window.UpdateUI_Status();
}

void TabConfigurations::on_configurations_executable_append_pressed() {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);

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

void TabConfigurations::on_configurations_list_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);

    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.GetActiveExecutable();
    if (executable != nullptr) {
        executable->enabled = checked;
    }

    const Configuration *configuration = configurator.GetActiveConfiguration();

    if (configuration == nullptr) {
        this->ui->configurations_group_box_layers->setEnabled(false);
        this->ui->configurations_group_box_loader->setEnabled(false);
        this->ui->configurations_group_box_settings->setEnabled(false);
    } else {
        this->ui->configurations_group_box_layers->setEnabled(checked);
        this->ui->configurations_group_box_loader->setEnabled(checked);
        this->ui->configurations_group_box_settings->setEnabled(checked && configuration->override_layers &&
                                                                configurator.HasActiveSettings());
    }

    this->ui_configurations_group_box_list_tooltip();
}

void TabConfigurations::on_configurations_layers_ordering_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);

    Configurator &configurator = Configurator::Get();
    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        configuration->override_layers = checked;

        configurator.Override(OVERRIDE_AREA_ALL);
    }

    this->ui->configurations_group_box_settings->setEnabled(configurator.HasActiveSettings());
}

void TabConfigurations::on_configurations_loader_messages_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        configuration->override_loader = checked;

        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
    }
}

void TabConfigurations::on_configurations_layers_settings_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);

    Configurator &configurator = Configurator::Get();

    Parameter *parameter = configurator.GetActiveParameter();
    if (parameter != nullptr) {
        parameter->override_settings = checked;

        configurator.Override(OVERRIDE_AREA_LAYERS_SETTINGS_BIT);
    }

    this->ui_configurations_group_box_settings_tooltip();
}

void TabConfigurations::on_configuration_loader_errors_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_warns_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_infos_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_debug_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_layers_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_drivers_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

/// An item has been changed. Check for edit of the items name (configuration name)
void TabConfigurations::on_configurations_list_itemChanged(QListWidgetItem *item) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnRenameConfiguration(item);
}

void TabConfigurations::on_configurations_list_currentRowChanged(int currentRow) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnSelectConfiguration(currentRow);
}

void TabConfigurations::on_configurations_layers_list_currentRowChanged(int currentRow) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnSelectLayer(currentRow);
}

void TabConfigurations::on_configurations_layerVersionChanged() { this->UpdateUI_Layers(UPDATE_REBUILD_UI); }
