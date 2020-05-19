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
#include <QLibrary>
#include <vulkan/vulkan.h>

#include "mainwindow.h"
#include "dlgbetamessage.h"
#include "dlgcustomalert.h"

int main(int argc, char *argv[])
    {
    QCoreApplication::setOrganizationName("LunarG");
    QCoreApplication::setOrganizationDomain("lunarg.com");
    QCoreApplication::setApplicationName("VkConfig");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);

    QApplication app(argc, argv);

    // Get and initialize the application model, which is essentially the Vulkan
    // configuration of the system.
    CVulkanConfiguration* pModel = CVulkanConfiguration::getVulkanConfig();

    // The main GUI is driven here
    MainWindow w;

    QSettings settings;
    bool bShowMe = settings.value("VK_CONFIG_BETA_MESSAGE").toBool();
    if(!bShowMe) {
        dlgBetaMessage beta(nullptr);
        beta.exec();

        if(beta.bDontShow)
            settings.setValue("VK_CONFIG_BETA_MESSAGE", true);
        }


    // Check loader version
#ifdef WIN32
    QLibrary library("vulkan-1.dll");
#else
    QLibrary library("libvulkan");
#endif
    if(!(library.load()))
        {
        dlgCustomAlert dlg(NULL);
        dlg.SetMessage("Could not load vulkan loader!");
        dlg.exec();
    } else {
        PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;

        vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)library.resolve("vkEnumerateInstanceVersion");
        uint32_t version=0;
        if(VK_SUCCESS == vkEnumerateInstanceVersion(&version)) {
            if(version < 4202633) {
                QString message;
                message.sprintf("Warning, you have an older Vulkan Loader. Layer overrides applied with this tool "
                                "will affect all Vulkan applications on your system.\n\nYou need at least version 1.2, patch 137 for this "
                                "feature to work.\n\nYour current detected loader version is %d.%d Patch(%d)\n\n", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
                dlgCustomAlert alert(nullptr);
                alert.SetMessage(message);
                alert.exec();
                }
            }
        }

    w.show();
    int nRet =  app.exec();

    delete pModel; // Cleanup everything when app terminates
    return nRet;
    }
