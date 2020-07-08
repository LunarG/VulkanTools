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

//////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::dlgCreateAssociation(QWidget *parent)
    : QDialog(parent), ui_(new Ui::dlgCreateAssociation), last_selected_application_index_(-1) {
    ui_->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Configurator &configurator = Configurator::Get();
    configurator.override_application_list_updated = false;

    if (!configurator.HasActiveOverrideOnApplicationListOnly()) setWindowTitle("Applications Launcher Shortcuts");

    // Show the current list
    for (int i = 0; i < configurator.overridden_application_list.size(); i++) {
        QTreeWidgetItem *item = CreateApplicationItem(*configurator.overridden_application_list[i]);
    }

    ui_->treeWidget->installEventFilter(this);

    connect(ui_->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(selectedPathChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui_->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(itemChanged(QTreeWidgetItem *, int)));
    connect(ui_->lineEditCmdArgs, SIGNAL(textEdited(const QString &)), this, SLOT(editCommandLine(const QString &)));
    connect(ui_->lineEditWorkingFolder, SIGNAL(textEdited(const QString &)), this, SLOT(editWorkingFolder(const QString &)));
    connect(ui_->lineEditLogFile, SIGNAL(textEdited(const QString &)), this, SLOT(editLogFile(const QString &)));
}

///////////////////////////////////////////////////////////////////////////////
dlgCreateAssociation::~dlgCreateAssociation() { delete ui_; }

//////////////////////////////////////////////////////////////////////////////
bool dlgCreateAssociation::eventFilter(QObject *target, QEvent *event) {
    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just relay on the resize method
    // of this window.
    if (target == ui_->treeWidget) {
        if (event->type() == QEvent::Resize) {
            ui_->treeWidget->resizeColumnToContents(1);
            int nLastColumnWidth = ui_->treeWidget->columnWidth(1);
            QRect rect = ui_->treeWidget->geometry();
            ui_->treeWidget->setColumnWidth(0, rect.width() - nLastColumnWidth);
        }
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
/// Make sure any changes are saved
void dlgCreateAssociation::closeEvent(QCloseEvent *event) {
    Configurator &configurator = Configurator::Get();

    configurator.SaveOverriddenApplicationList();
    event->accept();

    // When we don't use overridden list only, no need to alert the user about empty list cases.
    if (!configurator.overridden_application_list_only) return;

    if (configurator.overridden_application_list.empty() || !configurator.HasOverriddenApplications()) {
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

    QString filter = ("Applications (*)");  // Linux default

#ifdef __APPLE__
    filter = QString("Applications (*.app, *");
#endif

#ifdef _WIN32
    filter = QString("Applications (*.exe)");
#endif

    // Go get it.
    QString full_suggested_path = configurator.GetPath(Configurator::LastExecutablePath);
    QString executable_full_path = QFileDialog::getOpenFileName(this, "Select a Vulkan Executable", full_suggested_path, filter);

    // If they have selected something!
    if (!executable_full_path.isEmpty()) {
        configurator.SetPath(Configurator::LastExecutablePath, executable_full_path);

        // On macOS, they may have selected a binary, or they may have selected an app bundle.
        // If the later, we need to drill down to the actuall applicaiton
        if (executable_full_path.right(4) == QString(".app")) {
            // Start by drilling down
            GetExecutableFromAppBundle(executable_full_path);
        }

        Application *new_application = new Application(executable_full_path, "");
        configurator.overridden_application_list.push_back(new_application);

        QTreeWidgetItem *item = CreateApplicationItem(*new_application);

        configurator.SaveSettings();
        configurator.SaveOverriddenApplicationList();
        configurator.RefreshConfiguration();
        ui_->treeWidget->setCurrentItem(item);
        last_selected_application_index_ = ui_->treeWidget->indexOfTopLevelItem(item);
    }
}

QTreeWidgetItem *dlgCreateAssociation::CreateApplicationItem(const Application &application) const {
    Configurator &configurator = Configurator::Get();

    QTreeWidgetItem *item = new QTreeWidgetItem();
    ui_->treeWidget->addTopLevelItem(item);

    if (configurator.HasActiveOverrideOnApplicationListOnly()) {
        item->setText(0, tr("    ") + application.executable_path);
        QCheckBox *check_box = new QCheckBox("");
        check_box->setChecked(application.override_layers);
        ui_->treeWidget->setItemWidget(item, 0, check_box);
        connect(check_box, SIGNAL(clicked(bool)), this, SLOT(itemClicked(bool)));
    } else {
        item->setText(0, application.executable_path);
    }

    return item;
}

///////////////////////////////////////////////////////////////////////////////
/// Easy enough, just remove the selected program from the list
void dlgCreateAssociation::on_pushButtonRemove_clicked() {
    QTreeWidgetItem *pCurrent = ui_->treeWidget->currentItem();
    int iSel = ui_->treeWidget->indexOfTopLevelItem(pCurrent);
    if (iSel < 0) return;

    Configurator &configurator = Configurator::Get();

    ui_->treeWidget->takeTopLevelItem(iSel);
    ui_->treeWidget->setCurrentItem(nullptr);
    configurator.overridden_application_list.removeAt(iSel);

    ui_->groupLaunchInfo->setEnabled(false);
    ui_->pushButtonRemove->setEnabled(false);
    ui_->lineEditCmdArgs->setText("");
    ui_->lineEditWorkingFolder->setText("");
    ui_->lineEditLogFile->setText("");

    configurator.SaveOverriddenApplicationList();
    configurator.RefreshConfiguration();
    ui_->treeWidget->update();
    last_selected_application_index_ = -1;
}

///////////////////////////////////////////////////////////////////////////////
/// The remove button is disabled until/unless something is selected that can
/// be removed. Also the working folder and command line arguments are updated
void dlgCreateAssociation::selectedPathChanged(QTreeWidgetItem *current_item, QTreeWidgetItem *previous_item) {
    (void)previous_item;
    last_selected_application_index_ = ui_->treeWidget->indexOfTopLevelItem(current_item);
    if (last_selected_application_index_ < 0) {
        ui_->groupLaunchInfo->setEnabled(false);
        ui_->pushButtonRemove->setEnabled(false);
        ui_->lineEditCmdArgs->setText("");
        ui_->lineEditWorkingFolder->setText("");
        ui_->lineEditWorkingFolder->setText("");
        return;
    }

    ui_->groupLaunchInfo->setEnabled(true);
    ui_->pushButtonRemove->setEnabled(true);

    Configurator &configurator = Configurator::Get();

    ui_->lineEditWorkingFolder->setText(configurator.overridden_application_list[last_selected_application_index_]->working_folder);
    ui_->lineEditCmdArgs->setText(configurator.overridden_application_list[last_selected_application_index_]->arguments);
    ui_->lineEditLogFile->setText(configurator.overridden_application_list[last_selected_application_index_]->log_file);
}

///////////////////////////////////////////////////////////////////////////////
void dlgCreateAssociation::itemChanged(QTreeWidgetItem *item, int column) {
    last_selected_application_index_ = ui_->treeWidget->indexOfTopLevelItem(item);
    QCheckBox *check_box = dynamic_cast<QCheckBox *>(ui_->treeWidget->itemWidget(item, column));
    if (check_box != nullptr) {
        Configurator &configurator = Configurator::Get();
        configurator.overridden_application_list[last_selected_application_index_]->override_layers = check_box->isChecked();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// Something was clicked. We don't know what, and short of setting up a new
/// signal/slot for each button, this seemed a reasonable approach. Just poll
/// all of them. There aren't that many, so KISS (keep it simple stupid)
void dlgCreateAssociation::itemClicked(bool clicked) {
    (void)clicked;

    Configurator &configurator = Configurator::Get();
    bool need_checkbox = configurator.HasActiveOverrideOnApplicationListOnly();
    if (!need_checkbox) return;

    // Loop through the whole list and reset the checkboxes
    for (int i = 0; i < ui_->treeWidget->topLevelItemCount(); i++) {
        QTreeWidgetItem *item = ui_->treeWidget->topLevelItem(i);
        QCheckBox *check_box = dynamic_cast<QCheckBox *>(ui_->treeWidget->itemWidget(item, 0));
        Q_ASSERT(check_box != nullptr);
        configurator.overridden_application_list[i]->override_layers = check_box->isChecked();
    }
}

///////////////////////////////////////////////////////////////////////////////
void dlgCreateAssociation::editCommandLine(const QString &cmdLine) {
    QTreeWidgetItem *current = ui_->treeWidget->currentItem();
    last_selected_application_index_ = ui_->treeWidget->indexOfTopLevelItem(current);
    if (last_selected_application_index_ < 0) return;

    Configurator::Get().overridden_application_list[last_selected_application_index_]->arguments = cmdLine;
}

//////////////////////////////////////////////////////////////////////////////
void dlgCreateAssociation::editWorkingFolder(const QString &workingFolder) {
    QTreeWidgetItem *current = ui_->treeWidget->currentItem();
    last_selected_application_index_ = ui_->treeWidget->indexOfTopLevelItem(current);
    if (last_selected_application_index_ < 0) return;

    Configurator::Get().overridden_application_list[last_selected_application_index_]->working_folder = workingFolder;
}

void dlgCreateAssociation::editLogFile(const QString &logFile) {
    QTreeWidgetItem *current = ui_->treeWidget->currentItem();
    last_selected_application_index_ = ui_->treeWidget->indexOfTopLevelItem(current);
    if (last_selected_application_index_ < 0) return;

    Configurator::Get().overridden_application_list[last_selected_application_index_]->log_file = logFile;
}

///////////////////////////////////////////////////////////////////////////////
/// This is only used on macOS to extract the executable from the bundle.
/// You have to look at the plist.info file, you can't just assume whatever
/// you find in the /MacOS folder is the executable.
/// The initial path is the folder where info.plist resides, and the
/// path is completed to the executable upon completion.
void dlgCreateAssociation::GetExecutableFromAppBundle(QString &path) {
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
