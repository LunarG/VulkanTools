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
    Configurator &configurator = Configurator::Get();

    if (configurator.vulkan_system_info.loaderVersion < Version(1, 4, 322) && false) {
        this->ui->tab_widget->setTabEnabled(TAB_DRIVERS, false);
        this->ui->tab_widget->setTabToolTip(TAB_DRIVERS, "Additional Vulkan Drivers require Vulkan Loader 1.4.322 or newer");
    }

    this->connect(this->ui->driver_browse_button, SIGNAL(clicked()), this, SLOT(on_driver_browse_pressed()));
    this->connect(this->ui->driver_path_lineedit, SIGNAL(returnPressed()), this, SLOT(on_driver_append_pressed()));
}

TabDrivers::~TabDrivers() {}

void TabDrivers::UpdateUI(UpdateUIMode ui_update_mode) {
    Configurator &configurator = Configurator::Get();

    this->ui->driver_path_lineedit->setText(configurator.last_driver_path.RelativePath().c_str());
    this->ui->driver_paths_list->blockSignals(true);

    switch (ui_update_mode) {
        case UPDATE_REFRESH_UI:
        case UPDATE_REBUILD_UI: {
            this->ui->driver_paths_list->clear();

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
        } break;
    }

    this->ui->driver_paths_list->blockSignals(false);
}

void TabDrivers::CleanUI() {}

bool TabDrivers::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}

void TabDrivers::on_paths_changed() {
    Configurator::Get().Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabDrivers::on_paths_toggled() { Configurator::Get().Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT); }

void TabDrivers::on_driver_append_pressed() {
    Configurator &configurator = Configurator::Get();
    const Path &selected_path = this->ui->driver_path_lineedit->text().toStdString();

    if (selected_path.Empty()) {
        return;
    }

    configurator.driver_paths.insert(std::pair(selected_path, true));
    configurator.last_driver_path = selected_path;
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
    configurator.UpdateVulkanSystemInfo();

    this->UpdateUI(UPDATE_REBUILD_UI);
}
