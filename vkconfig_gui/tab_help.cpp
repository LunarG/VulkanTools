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

#include "tab_help.h"
#include "mainwindow.h"

TabHelp::TabHelp(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_HELP, window, ui) {}

TabHelp::~TabHelp() {}

void TabHelp::UpdateUI(UpdateUIMode mode) {}

void TabHelp::CleanUI() {}

bool TabHelp::EventFilter(QObject *target, QEvent *event) { return false; }
