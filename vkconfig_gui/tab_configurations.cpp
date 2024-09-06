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

#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/ui.h"
#include "../vkconfig_core/doc.h"

#include <QSettings>
#include <QMenu>
#include <QFileDialog>
#include <QDesktopServices>

TabConfigurations::TabConfigurations(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_CONFIGURATIONS, window, ui) {
    ui->configurations_list->installEventFilter(&window);
    ui->configurations_layers_list->installEventFilter(&window);
    ui->configurations_settings_tree->installEventFilter(&window);

    this->connect(this->ui->check_box_per_application, SIGNAL(toggled(bool)), this,
                  SLOT(on_check_box_per_application_toggled(bool)));
    this->connect(this->ui->combo_box_mode, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_combo_box_mode_currentIndexChanged(int)));
    this->connect(this->ui->combo_box_applications, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_combo_box_applications_currentIndexChanged(int)));

    this->connect(this->ui->combo_box_layers_view, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_combo_box_layers_view_currentIndexChanged(int)));

    this->connect(this->ui->configuration_loader_errors_checkBox, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_errors_checkBox_toggled(bool)));
    this->connect(this->ui->configuration_loader_warns_checkBox, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_warns_checkBox_toggled(bool)));
    this->connect(this->ui->configuration_loader_infos_checkBox, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_infos_checkBox_toggled(bool)));
    this->connect(this->ui->configuration_loader_debug_checkBox, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_debug_checkBox_toggled(bool)));
    this->connect(this->ui->configuration_loader_layers_checkBox, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_layers_checkBox_toggled(bool)));
    this->connect(this->ui->configuration_loader_drivers_checkBox, SIGNAL(toggled(bool)), this,
                  SLOT(on_configuration_loader_drivers_checkBox_toggled(bool)));

    this->connect(this->ui->configurations_list, SIGNAL(itemChanged(QListWidgetItem *)), this,
                  SLOT(on_configurations_list_itemChanged(QListWidgetItem *)));
    this->connect(this->ui->configurations_list, SIGNAL(currentRowChanged(int)), this,
                  SLOT(on_configurations_list_currentRowChanged(int)));
    this->connect(this->ui->configurations_layers_list, SIGNAL(currentRowChanged(int)), this,
                  SLOT(on_configurations_layers_list_currentRowChanged(int)));

    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    this->ui->splitter_main->restoreState(settings.value("mainwindow/splitter_main_state").toByteArray());
    this->ui->splitter_configurations->restoreState(settings.value("mainwindow/splitter_configurations_state").toByteArray());
    this->ui->splitter_settings->restoreState(settings.value("mainwindow/splitter_settings_state").toByteArray());
}

TabConfigurations::~TabConfigurations() {
    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    settings.setValue("mainwindow/splitter_main_state", ui->splitter_main->saveState());
    settings.setValue("mainwindow/splitter_configurations_state", ui->splitter_configurations->saveState());
    settings.setValue("mainwindow/splitter_settings_state", ui->splitter_settings->saveState());
}

void TabConfigurations::UpdateUI_Configurations(UpdateUIMode ui_update_mode) {
    Configurator &configurator = Configurator::Get();
    Environment &environment = configurator.environment;

    const ConfigurationInfo *configuration_info = configurator.configurations.GetActiveConfigurationInfo();
    assert(configuration_info != nullptr);

    const bool enabled_ui = configuration_info->mode == LAYERS_CONTROLLED_BY_CONFIGURATOR;

    ui->combo_box_mode->blockSignals(true);
    ui->combo_box_mode->setCurrentIndex(configuration_info->mode);
    ui->combo_box_mode->blockSignals(false);

    ui->configurations_list->blockSignals(true);

    ui->configurations_list->setEnabled(enabled_ui);
    ui->group_box_settings->setEnabled(enabled_ui);
    ui->group_box_layers->setEnabled(enabled_ui);

    if (ui_update_mode == UPDATE_REBUILD_UI) {
        ui->configurations_list->clear();

        for (std::size_t i = 0, n = configurator.configurations.available_configurations.size(); i < n; ++i) {
            const Configuration &configuration = configurator.configurations.available_configurations[i];

            // Hide built-in configuration when the layer is missing. The Vulkan user may have not installed the necessary layer
            // if (configuration.IsBuiltIn() && HasMissingLayer(configuration.parameters, configurator.layers.available_layers))
            // continue;

            ConfigurationListItem *item = new ConfigurationListItem(configuration.key.c_str());
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setText(configuration.key.c_str());
            item->setToolTip(configuration.description.c_str());
            ui->configurations_list->addItem(item);
        }
    }

    for (int i = 0, n = ui->configurations_list->count(); i < n; ++i) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->configurations_list->item(i));
        assert(item);
        assert(!item->configuration_name.empty());

        Configuration *configuration = configurator.configurations.FindConfiguration(item->configuration_name);
        assert(configuration != nullptr);

        if (item->configuration_name == configuration_info->name) {
            // TODO: add PartiallyChecked when the configuration is not working
            // HasMissingLayer(configuration.parameters, configurator.layers.available_layers)
            // item->setIcon(QIcon(":/resourcefiles/config-invalid.png"));
            ui->configurations_list->setCurrentItem(item);
            // ui->configurations_list->setCurrentRow(i);
            item->setIcon(QIcon(":/resourcefiles/config-on.png"));
        } else {
            item->setIcon(QIcon(":/resourcefiles/config-off.png"));
        }
    }

    ui->configurations_list->blockSignals(false);
}

void TabConfigurations::UpdateUI_Applications(UpdateUIMode ui_update_mode) {
    Configurator &configurator = Configurator::Get();
    Environment &environment = configurator.environment;

    if (environment.GetApplications().empty()) {
        ui->check_box_per_application->setEnabled(false);
        ui->check_box_per_application->setVisible(false);
        ui->combo_box_applications->setVisible(false);
    } else {
        ui->check_box_per_application->setEnabled(true);
        ui->check_box_per_application->setChecked(configurator.configurations.GetPerApplicationConfig());

        ui->combo_box_applications->setEnabled(configurator.configurations.GetPerApplicationConfig());

        ui->combo_box_applications->blockSignals(true);

        if (ui_update_mode == UPDATE_REBUILD_UI) {
            ui->combo_box_applications->clear();
            for (std::size_t i = 0, n = environment.GetApplications().size(); i < n; ++i) {
                const Application &application = environment.GetApplications()[i];

                ui->combo_box_applications->addItem(application.executable_path.RelativePath().c_str());
            }
        }

        Application &application = configurator.environment.GetActiveApplication();
        ui->combo_box_applications->setCurrentIndex(environment.GetActiveApplicationIndex());
        ui->combo_box_applications->setToolTip(application.executable_path.AbsolutePath().c_str());
        ui->combo_box_applications->blockSignals(false);
    }
}

void TabConfigurations::UpdateUI_LoaderMessages() {
    const Configurator &configurator = Configurator::Get();

    const Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        ui->configuration_loader_errors_checkBox->blockSignals(true);
        ui->configuration_loader_errors_checkBox->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_ERROR));
        ui->configuration_loader_errors_checkBox->blockSignals(false);
        ui->configuration_loader_warns_checkBox->blockSignals(true);
        ui->configuration_loader_warns_checkBox->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_WARN));
        ui->configuration_loader_warns_checkBox->blockSignals(false);
        ui->configuration_loader_infos_checkBox->blockSignals(true);
        ui->configuration_loader_infos_checkBox->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_INFO));
        ui->configuration_loader_infos_checkBox->blockSignals(false);
        ui->configuration_loader_debug_checkBox->blockSignals(true);
        ui->configuration_loader_debug_checkBox->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_DEBUG));
        ui->configuration_loader_debug_checkBox->blockSignals(false);
        ui->configuration_loader_layers_checkBox->blockSignals(true);
        ui->configuration_loader_layers_checkBox->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_LAYER));
        ui->configuration_loader_layers_checkBox->blockSignals(false);
        ui->configuration_loader_drivers_checkBox->blockSignals(true);
        ui->configuration_loader_drivers_checkBox->setChecked(configuration->loader_log_messages_flags & GetBit(LOG_IMPLEM));
        ui->configuration_loader_drivers_checkBox->blockSignals(false);
    }
}

void TabConfigurations::UpdateUI_Layers(UpdateUIMode mode) {
    ui->configurations_layers_list->blockSignals(true);
    ui->configurations_layers_list->clear();

    Configurator &configurator = Configurator::Get();
    const std::string &selected_contiguration_name = configurator.configurations.GetActiveConfigurationInfo()->name;
    const bool has_selected_configuration = !selected_contiguration_name.empty();

    if (has_selected_configuration) {
        Configuration *configuration = configurator.configurations.FindConfiguration(selected_contiguration_name);
        if (configuration != nullptr) {
            for (std::size_t i = 0, n = configuration->parameters.size(); i < n; ++i) {
                Parameter &parameter = configuration->parameters[i];

                if (!configuration->view_advanced_layers) {
                    if (parameter.control == LAYER_CONTROL_AUTO) {
                        continue;
                    }
                }

                ListWidgetItemParameter *item_state = new ListWidgetItemParameter(parameter.key.c_str());
                item_state->setFlags(item_state->flags() | Qt::ItemIsSelectable);
                ui->configurations_layers_list->addItem(item_state);

                const std::vector<Version> &layer_versions = configurator.layers.GatherVersions(parameter.key);

                ConfigurationLayerWidget *layer_widget =
                    new ConfigurationLayerWidget(this, parameter, layer_versions, configuration->view_advanced_layers);
                item_state->widget = layer_widget;

                if (parameter.control == LAYER_CONTROL_APPLICATIONS_API) {
                    item_state->widget->setToolTip("Located and Enabled Layers using the Vulkan API by the Vulkan Application");
                } else if (parameter.control == LAYER_CONTROL_APPLICATIONS_ENV) {
                    item_state->widget->setToolTip(
                        "Located and Enabled Layers using Environment Variables by the Vulkan Application");
                }

                ui->configurations_layers_list->setItemWidget(item_state, layer_widget);
                if (configuration->selected_layer_name == parameter.key) {
                    ui->configurations_layers_list->setCurrentItem(item_state);
                }
            }
            // resizeEvent(nullptr);

            ui->combo_box_layers_view->setCurrentIndex(configuration->view_advanced_layers);

            ui->configurations_layers_list->update();
        }
    }

    ui->execute_closer_application_label->setVisible(true);
    ui->execute_closer_driver_label->setVisible(true);

    ui->configurations_layers_list->blockSignals(false);
}

void TabConfigurations::UpdateUI_Settings(UpdateUIMode mode) {
    Configurator &configurator = Configurator::Get();

    if (configurator.configurations.GetActiveConfigurationInfo() == nullptr) {
        this->_settings_tree_manager.CleanupGUI();
        ui->configurations_presets_comboBox->setVisible(false);
    } else {
        this->_settings_tree_manager.CreateGUI(ui->configurations_presets_comboBox, ui->configurations_settings_tree);
    }
}

void TabConfigurations::UpdateUI(UpdateUIMode ui_update_mode) {
    this->UpdateUI_Configurations(ui_update_mode);
    this->UpdateUI_Applications(ui_update_mode);
    this->UpdateUI_LoaderMessages();
    this->UpdateUI_Layers(ui_update_mode);
    this->UpdateUI_Settings(ui_update_mode);

    Configurator &configurator = Configurator::Get();

    // Update the layers configuration area
    LayersMode mode = configurator.configurations.GetActiveConfigurationInfo()->mode;
    const bool enabled_ui = mode == LAYERS_CONTROLLED_BY_CONFIGURATOR;

    ui->combo_box_mode->setCurrentIndex(mode);
    ui->configurations_list->setEnabled(enabled_ui);
    ui->group_box_settings->setEnabled(enabled_ui);
    ui->group_box_layers->setEnabled(enabled_ui);
}

void TabConfigurations::CleanUI() { this->_settings_tree_manager.CleanupGUI(); }

static const Layer *GetLayer(QListWidget *tree, QListWidgetItem *item) {
    if (item == nullptr) {
        return nullptr;
    }

    ListWidgetItemParameter *item_parameter = static_cast<ListWidgetItemParameter *>(item);

    const std::string &layer_string = item_parameter->layer_name;
    if (!layer_string.empty()) {
        Configurator &configurator = Configurator::Get();

        const std::string &layer_key = ExtractLayerName(configurator.layers, layer_string);
        return configurator.layers.Find(layer_key, Version::LATEST);
    }

    return nullptr;
}

bool TabConfigurations::EventFilter(QObject *target, QEvent *event) {
    QEvent::Type event_type = event->type();

    if (event_type == QEvent::Wheel) {
        return true;
    }

    if (event_type == QEvent::ChildRemoved) {
        // Layers were reordered, we need to update the configuration
        Configurator &configurator = Configurator::Get();
        Configuration *configuration = configurator.GetActiveConfiguration();

        std::vector<std::string> layer_names;
        for (int i = 0, n = ui->configurations_layers_list->count(); i < n; ++i) {
            QWidget *widget = ui->configurations_layers_list->itemWidget(ui->configurations_layers_list->item(i));
            const std::string &layer_name = static_cast<ConfigurationLayerWidget *>(widget)->layer_name;
            layer_names.push_back(layer_name);
        }
        configuration->Reorder(layer_names);

        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
        return true;
    }

    if (target == ui->configurations_list) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click) {
            ConfigurationListItem *item = static_cast<ConfigurationListItem *>(ui->configurations_list->itemAt(right_click->pos()));

            std::string name;

            if (item != nullptr) {
                name = item->configuration_name;
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
            QListWidgetItem *item = ui->configurations_layers_list->itemAt(right_click->pos());

            const Layer *layer = GetLayer(ui->configurations_layers_list, item);

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

    QListWidgetItem *item = ui->configurations_list->item(currentRow);
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);

    if (configuration_item == nullptr) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    if (configurator.configurations.GetActiveConfigurationInfo()->name != configuration_item->configuration_name) {
        configurator.configurations.GetActiveConfigurationInfo()->name = configuration_item->configuration_name;

        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

        this->UpdateUI_Configurations(UPDATE_REFRESH_UI);
        this->UpdateUI_LoaderMessages();
        this->UpdateUI_Layers(UPDATE_REBUILD_UI);
        this->UpdateUI_Settings(UPDATE_REBUILD_UI);
    }
}

void TabConfigurations::OnRenameConfiguration(QListWidgetItem *item) {
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    if (configuration_item == nullptr) {
        return;
    }

    Configurator &configurator = Configurator::Get();

    // We are renaming the file. Things can go wrong here...
    // This is the name of the configuratin we are changing
    // const std::string full_path =
    //    ConvertNativeSeparators(GetPath(BUILTIN_PATH_CONFIG_LAST) + "/" + configuration_item->configuration_name + ".json");

    // This is the new name we want to use for the configuration
    const std::string &new_name = configuration_item->text().toStdString();
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
    const std::string &old_name = configuration_item->configuration_name;
    Configuration *configuration = configurator.configurations.FindConfiguration(old_name);

    if (valid_new_name) {
        // Rename configuration ; Remove old configuration file ; change the name of the configuration
        configurator.configurations.RemoveConfigurationFile(old_name);
        configuration->key = configuration_item->configuration_name = new_name;
        configurator.configurations.GetActiveConfigurationInfo()->name = new_name;

        this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
        this->UpdateUI_LoaderMessages();
        this->UpdateUI_Layers(UPDATE_REBUILD_UI);
    } else {
        // If the configurate name is empty or the configuration name is taken, keep old configuration name
        ui->configurations_list->blockSignals(true);
        item->setText(old_name.c_str());
        ui->configurations_list->blockSignals(false);

        configurator.configurations.GetActiveConfigurationInfo()->name = old_name;
    }

    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
}

void TabConfigurations::OnSelectLayer(int currentRow) {
    this->_settings_tree_manager.CleanupGUI();

    if (currentRow == -1) {
        return;  // No row selected
    }

    QWidget *widget = ui->configurations_layers_list->itemWidget(ui->configurations_layers_list->item(currentRow));
    const std::string &layer_string = static_cast<ConfigurationLayerWidget *>(widget)->text().toStdString();

    Configurator &configurator = Configurator::Get();
    configurator.GetActiveConfiguration()->selected_layer_name = ExtractLayerName(configurator.layers, layer_string);

    this->_settings_tree_manager.CreateGUI(ui->configurations_presets_comboBox, ui->configurations_settings_tree);
}

void TabConfigurations::OnCheckedLoaderMessageTypes(bool checked) {
    (void)checked;

    Configurator &configurator = Configurator::Get();

    Configuration *active_configuration = configurator.GetActiveConfiguration();
    if (active_configuration != nullptr) {
        int loader_log_messages_bits = 0;

        loader_log_messages_bits |= ui->configuration_loader_errors_checkBox->isChecked() ? GetBit(LOG_ERROR) : 0;
        loader_log_messages_bits |= ui->configuration_loader_warns_checkBox->isChecked() ? GetBit(LOG_WARN) : 0;
        loader_log_messages_bits |= ui->configuration_loader_infos_checkBox->isChecked() ? GetBit(LOG_INFO) : 0;
        loader_log_messages_bits |= ui->configuration_loader_debug_checkBox->isChecked() ? GetBit(LOG_DEBUG) : 0;
        loader_log_messages_bits |= ui->configuration_loader_layers_checkBox->isChecked() ? GetBit(LOG_LAYER) : 0;
        loader_log_messages_bits |= ui->configuration_loader_drivers_checkBox->isChecked() ? GetBit(LOG_IMPLEM) : 0;

        active_configuration->loader_log_messages_flags = loader_log_messages_bits;
    }
}

void TabConfigurations::OnContextMenuNewClicked(ConfigurationListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    configurator.configurations.CreateConfiguration(configurator.layers, "New Configuration");
    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuImportClicked(ConfigurationListItem *item) {
    (void)item;  // We don't need this

    Configurator &configurator = Configurator::Get();

    const Path &path_import = configurator.environment.path_import;
    const std::string selected_path = QFileDialog::getOpenFileName(&this->window, "Import Layers Configuration File",
                                                                   path_import.AbsolutePath().c_str(), "JSON configuration(*.json)")
                                          .toStdString();

    if (selected_path.empty()) {
        return;
    }

    configurator.environment.path_import = selected_path;
    configurator.configurations.ImportConfiguration(configurator.layers, selected_path);

    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuRenameClicked(ConfigurationListItem *item) {
    assert(item);

    //    ui->configurations_list->blockSignals(true);

    //    item->widget->setVisible(false);
    //    item->setText(item->widget->text());

    //    ui->configurations_list->blockSignals(false);

    ui->configurations_list->editItem(item);
}

void TabConfigurations::OnContextMenuDuplicateClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.empty());

    Configurator &configurator = Configurator::Get();
    const Configuration &duplicated_configuration =
        configurator.configurations.CreateConfiguration(configurator.layers, item->configuration_name, true);

    item->configuration_name = duplicated_configuration.key;

    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuDeleteClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.empty());

    // Let make sure...
    QMessageBox alert;
    alert.setWindowTitle(format("Removing *%s* configuration...", item->configuration_name.c_str()).c_str());
    alert.setText(format("Are you sure you want to remove the *%s* configuration?", item->configuration_name.c_str()).c_str());
    alert.setInformativeText("All the data from this configuration will be lost.");
    alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    alert.setDefaultButton(QMessageBox::Yes);
    alert.setIcon(QMessageBox::Warning);
    if (alert.exec() == QMessageBox::No) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    configurator.configurations.RemoveConfiguration(item->configuration_name);

    this->UpdateUI_Configurations(UPDATE_REBUILD_UI);
    this->UpdateUI_Applications(UPDATE_REFRESH_UI);
    this->UpdateUI_LoaderMessages();
    this->UpdateUI_Layers(UPDATE_REBUILD_UI);
}

void TabConfigurations::OnContextMenuResetClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.empty());

    Configurator &configurator = Configurator::Get();
    Configuration *configuration = configurator.configurations.FindConfiguration(item->configuration_name);
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

void TabConfigurations::OnContextMenuReloadClicked(ConfigurationListItem *item) {
    assert(item);
    assert(!item->configuration_name.empty());

    // TODO
}

void TabConfigurations::OnContextMenuExportConfigsClicked(ConfigurationListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();

    const Path &path_export = configurator.environment.path_export;
    const std::string &selected_path =
        QFileDialog::getSaveFileName(&this->window, "Export Layers Configuration File", path_export.AbsolutePath().c_str(),
                                     "JSON configuration(*.json)")
            .toStdString();

    if (selected_path.empty()) {
        return;
    }

    configurator.configurations.ExportConfiguration(configurator.layers, selected_path, item->configuration_name);
}

void TabConfigurations::OnContextMenuExportSettingsClicked(ConfigurationListItem *item) {
    assert(item);

    Configurator &configurator = Configurator::Get();
    // TODO
}

void TabConfigurations::on_combo_box_mode_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();

    configurator.configurations.GetActiveConfigurationInfo()->mode = static_cast<LayersMode>(index);
    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->UpdateUI(UPDATE_REFRESH_UI);
    this->window.UpdateUI_Status();
}

void TabConfigurations::on_combo_box_applications_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.environment.SelectActiveApplication(index);

    this->UpdateUI(UPDATE_REFRESH_UI);
    this->window.UpdateUI_Status();
}

void TabConfigurations::on_check_box_per_application_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();

    configurator.configurations.SetPerApplicationConfig(checked);
    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI(UPDATE_REFRESH_UI);
    this->window.UpdateUI_Status();
}

void TabConfigurations::on_configuration_loader_errors_checkBox_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_warns_checkBox_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_infos_checkBox_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_debug_checkBox_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_layers_checkBox_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_configuration_loader_drivers_checkBox_toggled(bool checked) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);
    this->OnCheckedLoaderMessageTypes(checked);
}

void TabConfigurations::on_combo_box_layers_view_currentIndexChanged(int index) {
    assert(this->ui->tab_widget->currentIndex() == TAB_CONFIGURATIONS);

    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.GetActiveConfiguration();
    configuration->view_advanced_layers = index != 0;

    this->UpdateUI_Layers(UPDATE_REBUILD_UI);
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
