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

#include "tab_documentation.h"
#include "mainwindow.h"

TabDocumentation::TabDocumentation(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DOCUMENTATION, window, ui) {}

TabDocumentation::~TabDocumentation() {}

void TabDocumentation::UpdateUI(UpdateUIMode mode) {
    if (mode == UPDATE_REBUILD_UI) {
        // We need to rebuild in case the style theme changed
        QFile file_spec(":/resourcefiles/DOCUMENTATION.md");
        const bool result_spec = file_spec.open(QIODevice::ReadOnly | QIODevice::Text);
        assert(result_spec);
        this->ui->documentation_text->setMarkdown(file_spec.readAll());
        file_spec.close();
    }
}

void TabDocumentation::CleanUI() {}

bool TabDocumentation::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}
