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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include <QApplication>
#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setOrganizationName("LunarG");
    QCoreApplication::setOrganizationDomain("lunarg.com");
    QCoreApplication::setApplicationName("vkconfig");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //    QCoreApplication::setAttribute(Qt::AA_Use96Dpi);
    //    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    //    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    QApplication app(argc, argv);

    // Get and initialize the application model, which is essentially the Vulkan
    // configuration of the system.
    CVulkanConfiguration* pModel = CVulkanConfiguration::getVulkanConfig();

    // Double check, we simply cannot run without any layers
    if (pModel->allLayers.size() == 0) {
        delete pModel;
        return -1;
    }

    // The main GUI is driven here
    MainWindow mainWindow;
    mainWindow.show();
    int nRet = app.exec();

    delete pModel;  // Cleanup everything when app terminates
    return nRet;
}
