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

#include "tab_diagnostics.h"
#include "mainwindow.h"

#include "../vkconfig_core/configurator.h"

TabDiagnostics::TabDiagnostics(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DIAGNOSTIC, window, ui) {
    this->connect(this->ui->diagnostic_executable_mode, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_diagnostics_executable_mode_currentIndexChanged(int)));

    Configurator &configurator = Configurator::Get();
    this->ui->diagnostic_executable_mode->setCurrentIndex(configurator.GetExecutableMode());
}

TabDiagnostics::~TabDiagnostics() {}

void TabDiagnostics::UpdateUI(UpdateUIMode mode) {}

void TabDiagnostics::CleanUI() {}

bool TabDiagnostics::EventFilter(QObject *target, QEvent *event) { return false; }

void TabDiagnostics::on_diagnostics_executable_mode_currentIndexChanged(int index) {
    Configurator &configurator = Configurator::Get();

    configurator.SetExecutableMode(static_cast<ExecutableMode>(index));
    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI(UPDATE_REFRESH_UI);
    this->window.UpdateUI_Status();
}
