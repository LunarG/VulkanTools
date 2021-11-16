/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

#include "dialog_applications.h"
#include "configurator.h"

#include "../vkconfig_core/alert.h"

#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QCheckBox>

#include <cassert>

ApplicationsDialog::ApplicationsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::dialog_applications()), _last_selected_application_index(-1) {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    Configurator &configurator = Configurator::Get();

    // The header is hidden by default and stays hidden when no checkboxes are used.
    if (!configurator.environment.UseApplicationListOverrideMode())
        setWindowTitle("Vulkan Applications Launcher Shortcuts");
    else {
        ui->treeWidget->setHeaderHidden(false);
        ui->treeWidget->setHeaderLabel("Check to override Vulkan layers");
    }

    // Show the current list
    const std::vector<Application> &applications = configurator.environment.GetApplications();
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        CreateApplicationItem(applications[i]);
    }

    ui->treeWidget->installEventFilter(this);

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(selectedPathChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(itemChanged(QTreeWidgetItem *, int)));
    connect(ui->lineEditAppName, SIGNAL(textEdited(const QString &)), this, SLOT(editAppName(const QString &)));
    connect(ui->lineEditExecutable, SIGNAL(textEdited(const QString &)), this, SLOT(editExecutable(const QString &)));
    connect(ui->lineEditCmdArgs, SIGNAL(textEdited(const QString &)), this, SLOT(editCommandLine(const QString &)));
    connect(ui->lineEditWorkingFolder, SIGNAL(textEdited(const QString &)), this, SLOT(editWorkingFolder(const QString &)));
    connect(ui->lineEditLogFile, SIGNAL(textEdited(const QString &)), this, SLOT(editLogFile(const QString &)));

    // If there is an item in the tree (nullptr is okay here), make it the currently selected item.
    // This is a work around for macOS, where the currentItemChanged() signal is being emitted (by something)
    // after this constructor, without actually selecting the first row. The effect there is, the remove button is
    // enabled, and the first item is selected, but not visibly so. Repainting does not fix the issue either. This
    // is a macOS only fix, but is put in for all platforms so that the GUI behavior is consistent across all
    // platforms.
    QTreeWidgetItem *item = ui->treeWidget->topLevelItem(0);
    ui->treeWidget->setCurrentItem(item);
}

bool ApplicationsDialog::eventFilter(QObject *target, QEvent *event) {
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

/// Make sure any changes are saved
void ApplicationsDialog::closeEvent(QCloseEvent *event) {
    Environment &environment = Configurator::Get().environment;

    event->accept();

    // When we don't use overridden list only, no need to alert the user about empty list cases.
    if (!environment.UseApplicationListOverrideMode()) return;

    if (environment.GetApplications().empty() || !environment.HasOverriddenApplications()) {
        environment.SetMode(OVERRIDE_MODE_LIST, false);

        Alert::ApplicationListEmpty();
    }
}

/// Browse for and select an executable file to add to the list.
void ApplicationsDialog::on_pushButtonAdd_clicked()  // Pick the test application
{
    Configurator &configurator = Configurator::Get();

    const std::string suggested_path(configurator.path.GetPath(PATH_EXECUTABLE).c_str());
    std::string executable_full_path = configurator.path.SelectPath(this, PATH_EXECUTABLE, suggested_path).c_str();

    // If they have selected something!
    if (!executable_full_path.empty()) {
        // On macOS, they may have selected a binary, or they may have selected an app bundle.
        // If the later, we need to drill down to the actuall applicaiton
        if (executable_full_path.find(".app") != std::string::npos) {
            // Start by drilling down
            ExactExecutableFromAppBundle(executable_full_path);
        }

        std::string app_name;
        if (executable_full_path.find(GetNativeSeparator()) != std::string::npos) {
            app_name = executable_full_path.substr(executable_full_path.rfind(GetNativeSeparator()) + 1);
        } else {
            app_name = executable_full_path;
        }

        Application new_application(app_name, executable_full_path, "");
        configurator.environment.AppendApplication(new_application);

        QTreeWidgetItem *item = CreateApplicationItem(new_application);

        configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
        ui->treeWidget->setCurrentItem(item);
        configurator.environment.SelectActiveApplication(ui->treeWidget->indexOfTopLevelItem(item));
    }
}

QTreeWidgetItem *ApplicationsDialog::CreateApplicationItem(const Application &application) const {
    Configurator &configurator = Configurator::Get();

    QTreeWidgetItem *item = new QTreeWidgetItem();
    ui->treeWidget->addTopLevelItem(item);

    if (configurator.environment.UseApplicationListOverrideMode()) {
        QCheckBox *check_box = new QCheckBox(application.app_name.c_str());
        check_box->setChecked(application.override_layers);
        ui->treeWidget->setItemWidget(item, 0, check_box);
        connect(check_box, SIGNAL(clicked(bool)), this, SLOT(itemClicked(bool)));
    } else {
        item->setText(0, application.app_name.c_str());
    }

    return item;
}

/// Easy enough, just remove the selected program from the list
void ApplicationsDialog::on_pushButtonRemove_clicked() {
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
    ui->lineEditAppName->setText("");
    ui->lineEditExecutable->setText("");
    ui->lineEditCmdArgs->setText("");
    ui->lineEditWorkingFolder->setText("");
    ui->lineEditLogFile->setText("");

    configurator.configurations.RefreshConfiguration(configurator.layers.available_layers);
    ui->treeWidget->update();
}

// Dismiss the dialog, and preserve app information so it can be set to
// the launcher.
void ApplicationsDialog::on_pushButtonSelect_clicked() {
    Configurator &configurator = Configurator::Get();
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if (item != nullptr) {
        configurator.environment.SelectActiveApplication(ui->treeWidget->indexOfTopLevelItem(item));
    }

    close();
}

/// The remove button is disabled until/unless something is selected that can
/// be removed. Also the working folder and command line arguments are updated
void ApplicationsDialog::selectedPathChanged(QTreeWidgetItem *current_item, QTreeWidgetItem *previous_item) {
    (void)previous_item;
    int application_index = ui->treeWidget->indexOfTopLevelItem(current_item);

    ui->groupLaunchInfo->setEnabled(application_index >= 0);
    ui->pushButtonRemove->setEnabled(application_index >= 0);
    ui->pushButtonSelect->setEnabled(application_index >= 0);

    if (application_index < 0) {
        ui->lineEditAppName->setText("");
        ui->lineEditExecutable->setText("");
        ui->lineEditCmdArgs->setText("");
        ui->lineEditWorkingFolder->setText("");
        ui->lineEditLogFile->setText("");
        return;
    }

    const Application &application = Configurator::Get().environment.GetApplication(application_index);

    ui->lineEditAppName->setText(application.app_name.c_str());
    ui->lineEditExecutable->setText(application.executable_path.c_str());
    ui->lineEditWorkingFolder->setText(application.working_folder.c_str());
    ui->lineEditCmdArgs->setText(application.arguments.c_str());
    ui->lineEditLogFile->setText(ReplaceBuiltInVariable(application.log_file.c_str()).c_str());
}

void ApplicationsDialog::itemChanged(QTreeWidgetItem *item, int column) {
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(item);
    QCheckBox *check_box = dynamic_cast<QCheckBox *>(ui->treeWidget->itemWidget(item, column));
    if (check_box != nullptr) {
        Configurator::Get().environment.GetApplication(_last_selected_application_index).override_layers = check_box->isChecked();
    }
}

/// Something was clicked. We don't know what, and short of setting up a new
/// signal/slot for each button, this seemed a reasonable approach. Just poll
/// all of them. There aren't that many, so KISS (keep it simple stupid)
/// If one of them had their state flipped, that's the one that was checked, make
/// it the currently selected one.
void ApplicationsDialog::itemClicked(bool clicked) {
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

void ApplicationsDialog::editAppName(const QString &name) {
    QTreeWidgetItem *current = ui->treeWidget->currentItem();
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(current);
    if (_last_selected_application_index < 0) return;

    Configurator::Get().environment.GetApplication(_last_selected_application_index).app_name = name.toStdString();
}

void ApplicationsDialog::editExecutable(const QString &executable) {
    QTreeWidgetItem *current = ui->treeWidget->currentItem();
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(current);
    if (_last_selected_application_index < 0) return;

    Configurator::Get().environment.GetApplication(_last_selected_application_index).executable_path = executable.toStdString();
}

void ApplicationsDialog::editCommandLine(const QString &cmdLine) {
    QTreeWidgetItem *current = ui->treeWidget->currentItem();
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(current);
    if (_last_selected_application_index < 0) return;

    Configurator::Get().environment.GetApplication(_last_selected_application_index).arguments = cmdLine.toStdString();
}

void ApplicationsDialog::editWorkingFolder(const QString &workingFolder) {
    QTreeWidgetItem *current = ui->treeWidget->currentItem();
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(current);
    if (_last_selected_application_index < 0) return;

    Configurator::Get().environment.GetApplication(_last_selected_application_index).working_folder = workingFolder.toStdString();
}

void ApplicationsDialog::editLogFile(const QString &logFile) {
    QTreeWidgetItem *current = ui->treeWidget->currentItem();
    _last_selected_application_index = ui->treeWidget->indexOfTopLevelItem(current);
    if (_last_selected_application_index < 0) return;

    Configurator::Get().environment.GetApplication(_last_selected_application_index).log_file = logFile.toStdString();
}
