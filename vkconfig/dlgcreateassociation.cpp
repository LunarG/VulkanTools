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

#include "dlgcreateassociation.h"
#include "ui_dlgcreateassociation.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QCheckBox>

#include <cassert>

//////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::dlgCreateAssociation(QWidget *parent)
    : QDialog(parent), ui(new Ui::dlgCreateAssociation), _last_selected_application_index(-1) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Configurator &configurator = Configurator::Get();

    // The header is hidden by default and stays hidden when no checkboxes are used.
    if (!configurator.environment.UseApplicationListOverrideMode())
        setWindowTitle("Applications Launcher Shortcuts");
    else {
        ui->treeWidget->setHeaderHidden(false);
        ui->treeWidget->setHeaderLabel("Check to override Vulkan layers");
    }

    // Show the current list
    const std::vector<Application> &applications = configurator.environment.GetApplications();
    for (std::size_t i = 0, n = applications.size(); i < n; i++) CreateApplicationItem(applications[i]);

    ui->treeWidget->installEventFilter(this);

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(selectedPathChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(itemChanged(QTreeWidgetItem *, int)));
    connect(ui->lineEditCmdArgs, SIGNAL(textEdited(const QString &)), this, SLOT(editCommandLine(const QString &)));
    connect(ui->lineEditWorkingFolder, SIGNAL(textEdited(const QString &)), this, SLOT(editWorkingFolder(const QString &)));
    connect(ui->lineEditLogFile, SIGNAL(textEdited(const QString &)), this, SLOT(editLogFile(const QString &)));
}

///////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::~dlgCreateAssociation() { delete ui; }

//////////////////////////////////////////////////////////////////////////////
bool dlgCreateAssociation::eventFilter(QObject *target, QEvent *event) {
    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just rely on the resize method
    // of this window.
    if (target == ui->treeWidget) {
        if (event->type() == QEvent::Resize) {
            ui->treeWidget->resizeColumnToContents(1);
            int nLastColumnWidth = ui->treeWidget->columnWidth(1);
            QRect rect = ui->treeWidget->geometry();
            ui->treeWidget->setColumnWidth(0, rect.width() - nLastColumnWidth);
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
/// Make sure any changes are saved
void dlgCreateAssociation::closeEvent(QCloseEvent *event) {
    Environment &environment = Configurator::Get().environment;

    event->accept();

    // When we don't use overridden list only, no need to alert the user about empty list cases.
    if (!environment.UseApplicationListOverrideMode()) return;

    if (environment.GetApplications().empty() || !environment.HasOverriddenApplications()) {
        environment.SetMode(OVERRIDE_MODE_LIST, false);

        QMessageBox alert;
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle("Vulkan Layers overriding will apply globally.");
        alert.setText("The application list to override is empty. Restricting layers overriding to the selected list is disabled.");
        alert.setInformativeText("As a result, Vulkan Layers overriding will apply globally, to all Vulkan applications.");
        alert.exec();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Browse for and select an executable file to add to the list.
void dlgCreateAssociation::on_pushButtonAdd_clicked()  // Pick the test application
{
    Configurator &configurator = Configurator::Get();

    const QString suggested_path = configurator.path.GetPath(PATH_EXECUTABLE);
    QString executable_full_path = configurator.path.SelectPath(this, PATH_EXECUTABLE, suggested_path);

    // If they have selected something!
    if (!executable_full_path.isEmpty()) {
        // On macOS, they may have selected a binary, or they may have selected an app bundle.
        // If the later, we need to drill down to the actuall applicaiton
        if (executable_full_path.right(4) == QString(".app")) {
            // Start by drilling down
            GetExecutableFromAppBundle(executable_full_path);
        }

        Application new_application(executable_full_path, "");
        configurator.environment.AppendApplication(new_application);

        QTreeWidgetItem *item = CreateApplicationItem(new_application);

        configurator.RefreshConfiguration();
        ui->treeWidget->setCurrentItem(item);
        configurator.environment.SelectActiveApplication(ui->treeWidget->indexOfTopLevelItem(item));
    }
}

QTreeWidgetItem *dlgCreateAssociation::CreateApplicationItem(const Application &application) const {
    Configurator &configurator = Configurator::Get();

    QTreeWidgetItem *item = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(item);

    if (configurator.environment.UseApplicationListOverrideMode()) {
        QCheckBox *check_box = new QCheckBox(application.executable_path);
        check_box->setChecked(application.override_layers);
        ui->treeWidget->setItemWidget(item, 0, check_box);
        connect(check_box, SIGNAL(clicked(bool)), this, SLOT(itemClicked(bool)));
    } else {
        item->setText(0, application.executable_path);
    }

    return item;
}

///////////////////////////////////////////////////////////////////////////////
/// Easy enough, just remove the selected program from the list
void dlgCreateAssociation::on_pushButtonRemove_clicked() {
    QTreeWidgetItem *current = ui->treeWidget->currentItem();
    int selection = ui->treeWidget->indexOfTopLevelItem(current);
    assert(selection >= 0 && selection < ui->treeWidget->topLevelItemCount());

    Configurator &configurator = Configurator::Get();

    ui->treeWidget->takeTopLevelItem(selection);
    ui->treeWidget->setCurrentItem(nullptr);
    configurator.environment.RemoveApplication(selection);

    ui->groupLaunchInfo->setEnabled(false);
    ui->pushButtonRemove->setEnabled(false);
    ui->pushButtonSelect->setEnabled(false);
    ui->lineEditCmdArgs->setText("");
    ui->lineEditWorkingFolder->setText("");
    ui->lineEditLogFile->setText("");

    configurator.RefreshConfiguration();
    ui->treeWidget->update();
}

//////////////////////////////////////////////////////////////////////////////
// Dismiss the dialog, and preserve app information so it can be set to
// the launcher.
void dlgCreateAssociation::on_pushButtonSelect_clicked() {
    Configurator &configurator = Configurator::Get();
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if (item != nullptr) {
        configurator.environment.SelectActiveApplication(ui->treeWidget->indexOfTopLevelItem(item));
    }

    close();
}

///////////////////////////////////////////////////////////////////////////////
/// The remove button is disabled until/unless something is selected that can
/// be removed. Also the working folder and command line arguments are updated
void dlgCreateAssociation::selectedPathChanged(QTreeWidgetItem *current_item, QTreeWidgetItem *previous_item) {
    (void)previous_item;
    int application_index = ui->treeWidget->indexOfTopLevelItem(current_item);

    ui->groupLaunchInfo->setEnabled(application_index >= 0);
    ui->pushButtonRemove->setEnabled(application_index >= 0);
    ui->pushButtonSelect->setEnabled(application_index >= 0);

    if (application_index < 0) {
        ui->lineEditCmdArgs->setText("");
        ui->lineEditWorkingFolder->setText("");
        ui->lineEditWorkingFolder->setText("");
        return;
    }

    const Application &application = Configurator::Get().environment.GetApplication(application_index);

    ui->lineEditWorkingFolder->setText(application.working_folder);
    ui->lineEditCmdArgs->setText(application.arguments);
    ui->lineEditLogFile->setText(application.log_file);
}

///////////////////////////////////////////////////////////////////////////////
void dlgCreateAssociation::itemChanged(QTreeWidgetItem *item, int column) {
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(item);
    QCheckBox *check_box = dynamic_cast<QCheckBox *>(ui->treeWidget->itemWidget(item, column));
    if (check_box != nullptr) {
        Configurator::Get().environment.GetApplication(_last_selected_application_index).override_layers = check_box->isChecked();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Something was clicked. We don't know what, and short of setting up a new
/// signal/slot for each button, this seemed a reasonable approach. Just poll
/// all of them. There aren't that many, so KISS (keep it simple stupid)
/// If one of them had their state flipped, that's the one that was checked, make
/// it the currently selected one.
void dlgCreateAssociation::itemClicked(bool clicked) {
    (void)clicked;

    Environment &environment = Configurator::Get().environment;
    const bool need_checkbox = environment.UseApplicationListOverrideMode();
    if (!need_checkbox) return;

    // Loop through the whole list and reset the checkboxes
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
        QCheckBox *check_box = dynamic_cast<QCheckBox *>(ui->treeWidget->itemWidget(item, 0));
        assert(check_box != nullptr);
        environment.GetApplication(i).override_layers = check_box->isChecked();
    }
}

///////////////////////////////////////////////////////////////////////////////
void dlgCreateAssociation::editCommandLine(const QString &cmdLine) {
    QTreeWidgetItem *current = ui->treeWidget->currentItem();
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(current);
    if (_last_selected_application_index < 0) return;

    Configurator::Get().environment.GetApplication(_last_selected_application_index).arguments = cmdLine;
}

//////////////////////////////////////////////////////////////////////////////
void dlgCreateAssociation::editWorkingFolder(const QString &workingFolder) {
    QTreeWidgetItem *current = ui->treeWidget->currentItem();
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(current);
    if (_last_selected_application_index < 0) return;

    Configurator::Get().environment.GetApplication(_last_selected_application_index).working_folder = workingFolder;
}

void dlgCreateAssociation::editLogFile(const QString &logFile) {
    QTreeWidgetItem *current = ui->treeWidget->currentItem();
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(current);
    if (_last_selected_application_index < 0) return;

    Configurator::Get().environment.GetApplication(_last_selected_application_index).log_file = logFile;
}

///////////////////////////////////////////////////////////////////////////////
/// This is only used on macOS to extract the executable from the bundle.
/// You have to look at the plist.info file, you can't just assume whatever
/// you find in the /MacOS folder is the executable.
/// The initial path is the folder where info.plist resides, and the
/// path is completed to the executable upon completion.
void dlgCreateAssociation::GetExecutableFromAppBundle(QString &app_path) {
    QString path = app_path;
    path += "/Contents/";
    QString list_file = path + "Info.plist";
    QFile file(list_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream stream(&file);

    // Read a line at a time looking for the executable tag
    QString line_buffer;
    while (!stream.atEnd()) {
        line_buffer = stream.readLine();
        if (line_buffer.contains("<key>CFBundleExecutable</key>")) {  // Exe follows this
            line_buffer = stream.readLine();                          // <string>Qt Creator</string>
            char *cExeName = new char[line_buffer.length()];          // Prevent buffer overrun

            const char *pStart = strstr(line_buffer.toUtf8().constData(), "<string>");
            if (pStart == nullptr) return;

            // We found it, now extract it out
            pStart += 8;
            int iIndex = 0;
            while (*pStart != '<') {
                cExeName[iIndex++] = *pStart++;
            }
            cExeName[iIndex] = '\0';

            // Complete the partial path
            path += QString("MacOS/");
            path += QString(cExeName);
            delete[] cExeName;
            break;
        }
    }

    file.close();
}
