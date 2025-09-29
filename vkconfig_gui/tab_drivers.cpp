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

#include "widget_tab_layers_path.h"
#include "widget_tab_driver_path.h"
#include "tab_drivers.h"
#include "mainwindow.h"
#include "style.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/type_hide_message.h"

#include <QFileDialog>
#include <QMessageBox>

#include <chrono>
#include <thread>

TabDrivers::TabDrivers(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DRIVERS, window, ui) {
    this->ui->drivers_device_list->installEventFilter(&window);

    Configurator &configurator = Configurator::Get();

    if (configurator.vulkan_system_info.loaderVersion < Version(1, 4, 322) && false) {
        this->ui->tab_widget->setTabEnabled(TAB_DRIVERS, false);
        this->ui->tab_widget->setTabToolTip(TAB_DRIVERS, "Additional Vulkan Drivers require Vulkan Loader 1.4.322 or newer");
    }

    this->connect(this->ui->driver_group_box_override, SIGNAL(toggled(bool)), this, SLOT(on_driver_override_toggled(bool)));
    this->connect(this->ui->driver_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(on_driver_mode_changed(int)));
    this->connect(this->ui->driver_forced_name, SIGNAL(currentIndexChanged(int)), this, SLOT(on_driver_name_changed(int)));

    this->connect(this->ui->driver_group_box_paths, SIGNAL(toggled(bool)), this, SLOT(on_driver_paths_toggled(bool)));
    this->connect(this->ui->driver_browse_button, SIGNAL(clicked()), this, SLOT(on_driver_browse_pressed()));
    this->connect(this->ui->driver_path_lineedit, SIGNAL(returnPressed()), this, SLOT(on_driver_append_pressed()));

    this->UpdateUI(UPDATE_REBUILD_UI);
}

TabDrivers::~TabDrivers() {}

void TabDrivers::UpdateUI(UpdateUIMode ui_update_mode) {
    Configurator &configurator = Configurator::Get();

    if (configurator.driver_override_list.empty()) {
        for (std::size_t i = 0, n = configurator.vulkan_system_info.physicalDevices.size(); i < n; ++i) {
            configurator.driver_override_list.push_back(configurator.vulkan_system_info.physicalDevices[i].deviceName.c_str());
        }
    }

    switch (ui_update_mode) {
        case UPDATE_REFRESH_UI:
        case UPDATE_REBUILD_UI: {
            this->ui->driver_group_box_override->blockSignals(true);
            this->ui->driver_group_box_override->setChecked(configurator.driver_override_enabled);
            this->ui->driver_group_box_override->blockSignals(false);
            this->ui->driver_mode->blockSignals(true);
            this->ui->driver_mode->setCurrentIndex(configurator.driver_override_mode);
            this->ui->driver_mode->blockSignals(false);

            switch (configurator.driver_override_mode) {
                default:
                case DRIVER_MODE_SINGLE: {
                    this->ui->driver_forced_name->blockSignals(true);
                    this->ui->driver_forced_name->clear();
                    for (std::size_t i = 0, n = configurator.vulkan_system_info.physicalDevices.size(); i < n; ++i) {
                        const int current_index = this->ui->driver_forced_name->count();
                        this->ui->driver_forced_name->addItem(
                            configurator.vulkan_system_info.physicalDevices[i].deviceName.c_str());

                        std::string version;
                        const VulkanPhysicalDeviceInfo &info = configurator.vulkan_system_info.physicalDevices[i];

                        if (info.vendorID == 0x10DE) {
                            version = format("%s driver: %s - Vulkan API version: %s", GetLabel(info.vendorID).c_str(),
                                             FormatNvidia(info.driverVersion).c_str(), info.apiVersion.str().c_str());
                        } else if ((info.vendorID == 0x8086) && (VKC_PLATFORM & PLATFORM_WINDOWS_BIT)) {
                            version = format("%s driver: %s - Vulkan API version: %s", GetLabel(info.vendorID).c_str(),
                                             FormatIntelWindows(info.driverVersion).c_str(), info.apiVersion.str().c_str());
                        } else {
                            version = format("%s driver: %s - Vulkan API version: %s", GetLabel(info.vendorID).c_str(),
                                             Version(info.driverVersion).str().c_str(), info.apiVersion.str().c_str());
                        }

                        this->ui->driver_forced_name->setItemData(current_index, version.c_str(), Qt::ToolTipRole);
                    }
                    this->ui->driver_forced_name->setCurrentIndex(configurator.GetActiveDeviceIndex());
                    this->ui->driver_forced_name->setToolTip(
                        this->ui->driver_forced_name->itemData(configurator.GetActiveDeviceIndex(), Qt::ToolTipRole).toString());
                    this->ui->driver_forced_name->blockSignals(false);

                    this->ui->driver_name_label->setVisible(true);
                    this->ui->driver_forced_name->setVisible(true);
                    this->ui->drivers_label_first->setVisible(false);
                    this->ui->drivers_device_list->setVisible(false);
                    this->ui->drivers_label_last->setVisible(false);
                } break;
                case DRIVER_MODE_SORTED: {
                    this->ui->drivers_device_list->blockSignals(true);
                    this->ui->drivers_device_list->clear();
                    for (std::size_t i = 0, n = configurator.driver_override_list.size(); i < n; ++i) {
                        const int current_index = this->ui->driver_forced_name->count();
                        QListWidgetItem *item = new QListWidgetItem(configurator.driver_override_list[i].c_str());
                        item->setIcon(::Get(configurator.current_theme_mode, ICON_DRAG));

                        for (std::size_t j = 0, o = configurator.vulkan_system_info.physicalDevices.size(); j < o; ++j) {
                            const VulkanPhysicalDeviceInfo &info = configurator.vulkan_system_info.physicalDevices[j];

                            if (info.deviceName != configurator.driver_override_list[i]) {
                                continue;
                            }

                            std::string version;
                            if (info.vendorID == 0x10DE) {
                                version = format("%s driver: %s - Vulkan API version: %s", GetLabel(info.vendorID).c_str(),
                                                 FormatNvidia(info.driverVersion).c_str(), info.apiVersion.str().c_str());
                            } else if ((info.vendorID == 0x8086) && (VKC_PLATFORM & PLATFORM_WINDOWS_BIT)) {
                                version = format("%s driver: %s - Vulkan API version: %s", GetLabel(info.vendorID).c_str(),
                                                 FormatIntelWindows(info.driverVersion).c_str(), info.apiVersion.str().c_str());
                            } else {
                                version = format("%s driver: %s - Vulkan API version: %s", GetLabel(info.vendorID).c_str(),
                                                 Version(info.driverVersion).str().c_str(), info.apiVersion.str().c_str());
                            }

                            item->setToolTip(version.c_str());
                        }

                        this->ui->drivers_device_list->addItem(item);
                    }

                    /*
                                        for (int i = 0, n = this->ui->drivers_device_list->count(); i < n; ++i) {
                                            this->ui->drivers_device_list->item(i)->setIcon(::Get(configurator.current_theme_mode,
                       ICON_DRAG));
                                            ;
                                            // QSize size = this->ui->drivers_device_list->item(i)->sizeHint();
                                            // size.setHeight(32);
                                            // this->ui->drivers_device_list->item(i)->setSizeHint(size);
                                        }
                    */
                    this->ui->drivers_device_list->blockSignals(false);

                    // this->ui->drivers_device_list->setMaximumHeight(this->ui->drivers_device_list->count() * 32);

                    this->ui->driver_name_label->setVisible(false);
                    this->ui->driver_forced_name->setVisible(false);
                    this->ui->drivers_label_first->setVisible(true);
                    this->ui->drivers_device_list->setVisible(true);
                    this->ui->drivers_label_last->setVisible(true);
                } break;
            }

            this->ui->driver_group_box_paths->blockSignals(true);
            this->ui->driver_group_box_paths->setChecked(configurator.driver_paths_enabled);
            this->ui->driver_group_box_paths->blockSignals(false);

            this->ui->driver_paths_list->clear();
            this->ui->driver_path_lineedit->setText(configurator.last_driver_path.RelativePath().c_str());
            this->ui->driver_paths_list->blockSignals(true);

            for (auto it = configurator.driver_paths.begin(); it != configurator.driver_paths.end(); ++it) {
                QListWidgetItem *item_state = new QListWidgetItem;
                item_state->setFlags(item_state->flags() | Qt::ItemIsSelectable);
                item_state->setSizeHint(QSize(0, ITEM_HEIGHT));
                DriverPathWidget *drivers_path_widget = new DriverPathWidget(it->first, it->second);
                this->connect(drivers_path_widget, SIGNAL(itemChanged()), this, SLOT(on_paths_changed()));
                this->connect(drivers_path_widget, SIGNAL(itemToggled()), this, SLOT(on_paths_toggled()));

                ui->driver_paths_list->addItem(item_state);
                ui->driver_paths_list->setItemWidget(item_state, drivers_path_widget);
            }

            this->ui->driver_paths_list->blockSignals(false);
        } break;
    }
}

void TabDrivers::CleanUI() {}

bool TabDrivers::EventFilter(QObject *target, QEvent *event) {
    if (target == nullptr || event == nullptr) {
        return true;
    }

    QEvent::Type event_type = event->type();

    if (event_type == QEvent::Wheel) {
        return true;
    }

    if (target == this->ui->drivers_device_list && event_type == QEvent::ChildRemoved) {
        // Drivers were reordered, we need to update the driver list

        Configurator &configurator = Configurator::Get();

        configurator.driver_override_list.clear();
        for (int i = 0, n = ui->drivers_device_list->count(); i < n; ++i) {
            configurator.driver_override_list.push_back(ui->drivers_device_list->item(i)->text().toStdString());
        }

        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

        return true;
    }

    return false;
}

void TabDrivers::on_driver_override_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();

    configurator.driver_override_enabled = checked;
    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
}

void TabDrivers::on_driver_mode_changed(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.driver_override_mode = static_cast<DriverMode>(index);
    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabDrivers::on_driver_name_changed(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.driver_override_name = this->ui->driver_forced_name->itemText(index).toStdString();
    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabDrivers::on_driver_paths_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();

    configurator.driver_paths_enabled = checked;
    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
}

void TabDrivers::on_paths_changed() {
    Configurator &configurator = Configurator::Get();

    configurator.driver_override_list.clear();
    configurator.UpdateVulkanSystemInfo();

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabDrivers::on_paths_toggled() {
    Configurator &configurator = Configurator::Get();

    configurator.driver_override_list.clear();
    configurator.UpdateVulkanSystemInfo();

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabDrivers::on_driver_append_pressed() {
    Configurator &configurator = Configurator::Get();
    const Path &selected_path = this->ui->driver_path_lineedit->text().toStdString();

    if (selected_path.Empty()) {
        return;
    }

    configurator.driver_paths.insert(std::pair(selected_path, true));
    configurator.last_driver_path = selected_path;
    configurator.driver_override_list.clear();

    configurator.UpdateVulkanSystemInfo();

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabDrivers::on_driver_browse_pressed() {
    Configurator &configurator = Configurator::Get();

    const Path &selected_path = QFileDialog::getOpenFileName(this->ui->driver_browse_button, "Adding a Driver Manifests File...",
                                                             configurator.last_driver_path.AbsolutePath().c_str(), "*.json")
                                    .toStdString();

    if (selected_path.Empty()) {
        return;
    }

    configurator.driver_paths.insert(std::pair(selected_path, true));
    configurator.last_driver_path = selected_path;
    configurator.driver_override_list.clear();

    configurator.UpdateVulkanSystemInfo();

    this->UpdateUI(UPDATE_REBUILD_UI);
}
