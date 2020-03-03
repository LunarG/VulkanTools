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


#include <QSettings>
#include <QTimer>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

#include "dlglayeroutput.h"
#include "ui_dlglayeroutput.h"


///////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::dlgLayerOutput
/// \param parent
dlgLayerOutput::dlgLayerOutput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgLayerOutput)
    {
    ui->setupUi(this);
    vulkan_app = nullptr;
    pLogFile = nullptr;
    bAutoLaunch = false;

    // Get the singleton/model
    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();
    ui->checkBoxStdout->setChecked(pVulkanConfig->bLogStdout);

    // We are never running the test app already when this dialog is created
    ui->labelRunStatus->setText(tr("Not Running"));

    // Load up the GUI
    if(pVulkanConfig->qsLaunchApplicationWPath.isEmpty())
        ui->labelLaunchApp->setText(tr("Luanch app not set"));
    else
        ui->labelLaunchApp->setText(pVulkanConfig->qsLaunchApplicationWPath);

    // It's okay if these are just empty
    ui->lineEditCmdLine->setText(pVulkanConfig->qsLaunchApplicatinArgs);
    ui->lineEditWorkingDirectory->setText(pVulkanConfig->qsLaunchApplicationWorkingDir);


    if(pVulkanConfig->qsLogFileWPath.isEmpty())
        ui->labelLogFile->setText(tr("Log file not set."));
    else
        ui->labelLogFile->setText(pVulkanConfig->qsLogFileWPath);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::~dlgLayerOutput
/// The only real cleanup here is to make sure we are disconnected from
/// the app if we are monitoring it.
dlgLayerOutput::~dlgLayerOutput()
    {
    delete ui;
    if(vulkan_app) {
       disconnect(vulkan_app, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(processClosed(int, QProcess::ExitStatus)));

       disconnect(vulkan_app, SIGNAL(readyReadStandardError()), this,
                   SLOT(errorOutputAvailable()));

       disconnect(vulkan_app, SIGNAL(readyReadStandardOutput()), this,
                SLOT(standardOutputAvailable()));

        vulkan_app->close();
        delete vulkan_app;
        }
    }


//////////////////////////////////////////////////////////////////////////////
// If the bAutoLaunch flag is on, it means we want to open this dialog and
// launch the app right away.
void dlgLayerOutput::showEvent(QShowEvent *event)
    {
    (void)event;
    if(bAutoLaunch)
        on_pushButtonLaunchApp_clicked();

    event->accept();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::on_pushButtonSetApp_clicked
/// Use system file dialog to select an application to be monitored. Save that
/// in a persistant settings file for vkConfig. If the file name comes back
/// empty (user selected cancel), then the current setting is cleared.
void dlgLayerOutput::on_pushButtonSetApp_clicked()
    {
    QString fileWildcard = ("Applications (*)");    // Linux default

#ifdef __APPLE__
    fileWildcard = QString("Applications (*.app, *");
#endif

#ifdef _WIN32
    fileWildcard = QString("Applications (*.exe)");
#endif

    pVulkanConfig->qsLaunchApplicationWPath = QFileDialog::getOpenFileName(this,
        tr("Select executable to monitor"), "/", fileWildcard);

    if(pVulkanConfig->qsLaunchApplicationWPath.isEmpty())
        ui->labelLaunchApp->setText(tr("Luanch app not set"));
    else {
        ui->labelLaunchApp->setText(pVulkanConfig->qsLaunchApplicationWPath);

        // Break name seperate from app and set the path as the default
        // working directory. This does mean whenever the monitored app is changed,
        // the working directory is automatically set to that folder
        for(int i = pVulkanConfig->qsLaunchApplicationWPath.length()-1; i > 0; i--) {
            if(pVulkanConfig->qsLaunchApplicationWPath[i] == '/') {
                pVulkanConfig->qsLaunchApplicationWorkingDir = pVulkanConfig->qsLaunchApplicationWPath.left(i+1);
                break;
                }
            }

        ui->lineEditWorkingDirectory->setText(pVulkanConfig->qsLaunchApplicationWorkingDir);
        }

    // Save, just in case
    pVulkanConfig->SaveAppSettings();
    }



///////////////////////////////////////////////////////////////////////////////
// Select the working directory for the application. It is by default the same
// folder as the application is located in.
void dlgLayerOutput::on_pushButtonWorkingDir_clicked()
    {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    pVulkanConfig->qsLaunchApplicationWorkingDir =
        dialog.getExistingDirectory(this, tr("Select Working Directory"), pVulkanConfig->qsLaunchApplicationWPath);

    ui->lineEditWorkingDirectory->setText(pVulkanConfig->qsLaunchApplicationWorkingDir);

    pVulkanConfig->SaveAppSettings();
    }

///////////////////////////////////////////////////////////////////////////////
/// The command line arguments edit has been modified. Grab the text and save
/// it.
void dlgLayerOutput::on_lineEditCmdLine_editingFinished()
    {
    pVulkanConfig->qsLaunchApplicatinArgs = ui->lineEditCmdLine->text();
    pVulkanConfig->SaveAppSettings();
    }

void dlgLayerOutput::on_lineEditWorkingDirectory_editingFinished(void)
    {
    pVulkanConfig->qsLaunchApplicationWorkingDir = ui->lineEditWorkingDirectory->text();
    pVulkanConfig->SaveAppSettings();
    }

///////////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::on_pushButtonLaunchApp_clicked
/// Launch the app and monitor it's stdout to get layer output.
/// stdout is buffered by default, so in order to see realtime output it must
/// be flushed. Either of the following in the other app will do.
/// > fflush(stdout);    // Flush now
/// setlinebuf(stdout);  // always flush at the end of a line
///
/// The layers are automtically flushed, so they should show up as they
/// generated. One note... any other stdout generated by the monitored
/// application will also be captured.
///
/// If logging is enbabled (by setting a logging file), then the log file
/// is also opened.
void dlgLayerOutput::on_pushButtonLaunchApp_clicked()
    {
    // Are we already monitoring a running app? If so, terminate it
    if(vulkan_app != nullptr) {
        vulkan_app->terminate();
        return;
        }

    // Launch the test application
    vulkan_app = new QProcess(this);
    connect(vulkan_app, SIGNAL(readyReadStandardOutput()), this,
                 SLOT(standardOutputAvailable()));

    connect(vulkan_app, SIGNAL(readyReadStandardError()), this,
                SLOT(errorOutputAvailable()));

    connect(vulkan_app, SIGNAL(finished(int, QProcess::ExitStatus)),
                 this, SLOT(processClosed(int, QProcess::ExitStatus)));

     vulkan_app->setProgram(pVulkanConfig->qsLaunchApplicationWPath);
     vulkan_app->setWorkingDirectory(pVulkanConfig->qsLaunchApplicationWorkingDir);
     vulkan_app->setArguments(QStringList() << pVulkanConfig->qsLaunchApplicatinArgs);
     vulkan_app->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    // vulkan_app->setReadChannel(QProcess::StandardOutput);
   //  vulkan_app->setReadChannel(QProcess::StandardError);
     vulkan_app->setProcessChannelMode(QProcess::MergedChannels);
     vulkan_app->closeWriteChannel();
     ui->labelRunStatus->setText(tr("Running..."));
     ui->pushButtonLaunchApp->setText(tr("Terminate App"));

     // No log file is set, just bail
     if(pVulkanConfig->qsLogFileWPath.isEmpty())
        return;

     // This should never happen... but things that should never happen do in
     // fact happen... so just a sanity check.
     if(pLogFile != nullptr) {
         pLogFile->close();
         pLogFile = nullptr;
         }

     // Start logging
     pLogFile = new QFile(pVulkanConfig->qsLogFileWPath);
     if(!pVulkanConfig->qsLogFileWPath.isEmpty())
         if (!pLogFile->open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Append)) {
            QMessageBox err;
            err.setText(tr("Warning: Cannot open log file"));
            err.exec();
            delete pLogFile;
            pLogFile = nullptr;
            return;
            }


    // We are logging, let's add that we've launched a new application
    QString out;
    out.sprintf("Starting Vulkan Application: %s\n", pVulkanConfig->qsLaunchApplicationWPath.toUtf8().constData());
    pLogFile->write(out.toUtf8().constData(), out.length());
    out.sprintf("Working folder: %s\n", pVulkanConfig->qsLaunchApplicationWorkingDir.toUtf8().constData());
    pLogFile->write(out.toUtf8().constData(), out.length());
    out.sprintf("Command line arguments: %s\n", pVulkanConfig->qsLaunchApplicatinArgs.toUtf8().constData());
    pLogFile->write(out.toUtf8().constData(), out.length());
    }

/////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::processClosed
/// \param exitCode
/// \param status
/// The process we are following is closed. We don't actually care about the
/// exit status/code, we just need to know to destroy the QProcess object
/// and set it back to nullptr so that we know we can launch a new app.
/// Also, if we are logging, it's time to close the log file.
void dlgLayerOutput::processClosed(int exitCode, QProcess::ExitStatus status)
    {
    (void)exitCode;
    (void)status;

    // Not likely, but better to be sure...
    if(vulkan_app == nullptr)
        return;

    disconnect(vulkan_app, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(processClosed(int, QProcess::ExitStatus)));

    disconnect(vulkan_app, SIGNAL(readyReadStandardError()), this,
                SLOT(errorOutputAvailable()));

    disconnect(vulkan_app, SIGNAL(readyReadStandardOutput()), this,
                SLOT(standardOutputAvailable()));

    ui->labelRunStatus->setText(tr("Terminated"));
    ui->pushButtonLaunchApp->setText(tr("Launch App"));

    if(pLogFile) {
        pLogFile->close();
        delete pLogFile;
        pLogFile = nullptr;
        }

    delete vulkan_app;
    vulkan_app = nullptr;
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::on_pushButtonSetLogFile_clicked
/// Set the persistent log file. Canceling out the dialog clears the log file.
void dlgLayerOutput::on_pushButtonSetLogFile_clicked()
    {
    pVulkanConfig->qsLogFileWPath = QFileDialog::getSaveFileName(this,
        tr("Auto Save Output To..."),
        ".", tr("Log text(*.txt)"));

    if(pVulkanConfig->qsLogFileWPath.isEmpty())
        ui->labelLogFile->setText(tr("Not set"));
    else
        ui->labelLogFile->setText(pVulkanConfig->qsLogFileWPath);

    pVulkanConfig->SaveAppSettings();
    }

///////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::on_pushButtonClear_clicked
/// Just clear the currently displayed/saved layer output.
void dlgLayerOutput::on_pushButtonClear_clicked()
    {
    ui->textBrowser->clear();
    }

///////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::on_pushButtonSave_clicked
/// Save the current output to a text file. This is independent of the
/// main log file if may or may not be active, we don't care here, we just
/// want to save the current output to a seperate text file.
void dlgLayerOutput::on_pushButtonSave_clicked()
    {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Output"),
                               ".",
                               tr("Log text(*.txt)"));
    if(fileName.isEmpty())
        return;

    QFile logFile(fileName);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
       QMessageBox msgBox;
       msgBox.setText(tr("Could not create the file for writing."));
       msgBox.exec();

       return;
       }

    // Just grab the text right from the control and stream it out
    // in one go.
    QString appOutput = ui->textBrowser->toPlainText();
    QTextStream out(&logFile);
    out << appOutput;

    logFile.close();
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::on_checkBoxStdout_clicked
// Just toggle the flag. Logging of stdout can be turned on and off on the fly.
void dlgLayerOutput::on_checkBoxStdout_clicked(void)
    {
    pVulkanConfig->bLogStdout = ui->checkBoxStdout->isChecked();
    pVulkanConfig->SaveAppSettings();
    }

////////////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::standardOutputAvailable
/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void dlgLayerOutput::standardOutputAvailable(void)
    {
    if(vulkan_app == nullptr)
        return;

    // Nope, not interested, but thanks for playing
    if(!pVulkanConfig->bLogStdout)
        return;

    QString inFromApp = vulkan_app->readAllStandardOutput();
    ui->textBrowser->append(inFromApp);

    // Are we logging?
    if(pLogFile)
        pLogFile->write(inFromApp.toUtf8().constData(), inFromApp.length());
    }


void dlgLayerOutput::errorOutputAvailable(void)
    {
    if(vulkan_app == nullptr)
        return;

    QString inFromApp = vulkan_app->readAllStandardError();
    ui->textBrowser->append(inFromApp);

    // Are we logging?
    if(pLogFile)
        pLogFile->write(inFromApp.toUtf8().constData(), inFromApp.length());
    }


///////////////////////////////////////////////////////////////////////////
/// \brief dlgLayerOutput::on_pushButtonClearLogFile_clicked
/// Clear the log file. Confirm first, clear it whether it's open or
/// closed.
void dlgLayerOutput::on_pushButtonClearLogFile_clicked()
{
    // Is there a log file to begin with?
    if(pVulkanConfig->qsLogFileWPath.isEmpty()) {
        QMessageBox msg;
        msg.setText(tr("No log file specified!"));
        msg.exec();
        return;
        }

    // Let make sure...
    QMessageBox msg;
    msg.setText(tr("Are you sure you want to clear all of the contents from the log file?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if(msg.exec() == QMessageBox::No)
        return;


    // If the file is currently open, just close it,
    // as we'll just reopen it in truncate mode anyway.
    if(pLogFile) {
        pLogFile->close();
        pLogFile->open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate);
        return;
        }

    // It wasn't open, but we want to clear it. We can just delete it, or truncate it,
    // I'm going with truncate. It seems cleaner to leave an empty log file than to remove
    // it completely. Coming along after the fact, you can see there IS a log file, it's just
    // empty.
    pLogFile = new QFile(pVulkanConfig->qsLogFileWPath);
    pLogFile->open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate);
    pLogFile->close();
    delete pLogFile;
    pLogFile = nullptr;
}

