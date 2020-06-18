#ifndef DLGLAYEROUTPUT_H
#define DLGLAYEROUTPUT_H

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

#include <QDialog>
#include <QString>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QShowEvent>

#include <vulkanconfiguration.h>

namespace Ui {
class dlgLayerOutput;
}

class dlgLayerOutput : public QDialog {
    Q_OBJECT

   public:
    explicit dlgLayerOutput(QWidget *parent, bool bTempEnv = false);
    ~dlgLayerOutput();

    bool bTempEnvironment;
    bool bAPIDump;

   private:
    Ui::dlgLayerOutput *ui;

    CVulkanConfiguration *pVulkanConfig;  // Singleton

    QProcess *vulkan_app;  // Keeps track of the monitored app

    QFile *pLogFile;  // Log file for layer output

    virtual void showEvent(QShowEvent *event);

   public Q_SLOTS:
    void on_pushButtonSetApp_clicked();                  // Pick the test application
    void on_pushButtonLaunchApp_clicked();               // Launch the test application
    void on_pushButtonWorkingDir_clicked();              // Set working directory
    void on_pushButtonSetLogFile_clicked();              // Set the log file name/location
    void on_pushButtonClear_clicked();                   // Clear the log
    void on_pushButtonSave_clicked();                    // Save log to file
    void on_pushButtonClearLogFile_clicked();            // Clear the log file
    void on_lineEditCmdLine_editingFinished();           // User has changed command line
    void on_lineEditWorkingDirectory_editingFinished();  // User has manually updated the working directory (we need this for mac
                                                         // app bundles)

    void standardOutputAvailable(void);  // stdout output is available
    void errorOutputAvailable(void);     // Layeroutput is available

    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died
};

#endif  // DLGLAYEROUTPUT_H
