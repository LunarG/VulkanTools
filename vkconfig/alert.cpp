/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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

#include "alert.h"

#include <QMessageBox>

void Alert::ConfigurationRenamingFailed() {
    QMessageBox alert;
    alert.setWindowTitle("Renaming of the layers configuration failed...");
    alert.setText("There is already a configuration with the same name.");
    alert.setInformativeText("Use a different name for the configuration.");
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::ConfigurationNameEmpty() {
    QMessageBox alert;
    alert.setWindowTitle("Renaming of the layers configuration failed...");
    alert.setText("The configuration name is empty.");
    alert.setInformativeText("The configuration name is required.");
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}

void Alert::ConfigurationNameInvalid() {
    QMessageBox alert;
    alert.setWindowTitle("Invalid name for a configuration...");
    alert.setText("The configuration name is used to build a filename.");
    alert.setInformativeText("The name can't contain any of the following characters: \\ / : * \" < > |.");
    alert.setStandardButtons(QMessageBox::Ok);
    alert.setDefaultButton(QMessageBox::Ok);
    alert.setIcon(QMessageBox::Warning);
    alert.exec();
}
