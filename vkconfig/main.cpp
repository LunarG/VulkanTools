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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include <QApplication>
#include <QCheckBox>
#include <QMessageBox>

#include "mainwindow.h"
#include "appsingleton.h"
#include "command_line.h"
#include "test.h"

int main(int argc, char* argv[]) {
    const CommandLine command_line(argc, argv);

    switch (command_line.mode) {
        case CommandLine::ModeExecute: {
            QCoreApplication::setOrganizationName("LunarG");
            QCoreApplication::setOrganizationDomain("lunarg.com");
            QCoreApplication::setApplicationName("Vulkan Configurator");

            // Older Qt versions do not need this, but Linux builds do benefit
            // if it is present.
#if QT_VERSION > QT_VERSION_CHECK(5, 6, 0)
           QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
            QApplication app(argc, argv);

            // macOS only, has to hold constant, or get's 'tiny' in Retina mode.
#ifdef __APPLE__
           app.setStyleSheet("QWidget{font-size:12px;}");
#endif
            // This has to go after the construction of QApplication in
            // order to use a QMessageBox and avoid some QThread warnings.
            AppSingleton singleApp("vkconifg_single_instance");
            if (!singleApp.IsFirstApp()) {
                QMessageBox alert(nullptr);
                alert.setWindowTitle("Cannot start another instance of vkconfig");
                alert.setIcon(QMessageBox::Critical);
                alert.setText("Another copy of vkconfig is currently running. Please close the other instance and try again.");
                alert.exec();
                return -1;
            }

            // We simply cannot run without any layers
            if (Configurator::Get().InitializeConfigurator() == false) return -1;

            // The main GUI is driven here
            MainWindow main_window;
            main_window.show();

            return app.exec();
        } break;
        case CommandLine::ModeShowUsage: {
            command_line.usage();
            return 0;
        }
        case CommandLine::ModeRunTest: {
            return test();
        }
    }
}
