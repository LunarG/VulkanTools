/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "tab_applications.h"
#include "mainwindow.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>

/*
    case PATH_EXECUTABLE: {
        static const char* TABLE[] = {
            "Applications (*.exe)",     // PLATFORM_WINDOWS
            "Applications (*)",         // PLATFORM_LINUX
            "Applications (*.app, *)",  // PLATFORM_MACOS
            "N/A"                       // PLATFORM_ANDROID
        };
        static_assert(std::size(TABLE) == PLATFORM_COUNT,
                        "The tranlation table size doesn't match the enum number of elements");

        const std::string filter = TABLE[VKC_PLATFORM];
        const std::string selected_path = QFileDialog::getOpenFileName(parent, "Select a Vulkan Executable...",
                                                                        suggested_path.AbsolutePath().c_str(), filter.c_str())
                                                .toStdString();
        if (selected_path.empty())  // The user cancelled
            return Path("");

        SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
        return GetFullPath(path, QFileInfo(selected_path.c_str()).fileName().toStdString());
    }
    case PATH_WORKING_DIR: {
        const std::string selected_path =
            QFileDialog::getExistingDirectory(parent, "Set Working Folder To...", suggested_path.AbsolutePath().c_str())
                .toStdString();
        if (selected_path.empty())  // The user cancelled
            return Path("");

        SetPath(path, selected_path);
        return Path(GetPath(path));
    }
*/

TabApplications::TabApplications(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_APPLICATIONS, window, ui) {
    this->connect(this->ui->applications_remove_application_pushButton, SIGNAL(pressed()), this,
                  SLOT(on_applications_remove_application_pushButton_pressed()));
    this->connect(this->ui->applications_append_application_pushButton, SIGNAL(pressed()), this,
                  SLOT(on_applications_append_application_pushButton_pressed()));
    this->connect(this->ui->applications_list_comboBox, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_applications_list_comboBox_activated(int)));
    this->connect(this->ui->applications_list_comboBox, SIGNAL(editTextChanged(QString)), this,
                  SLOT(on_applications_list_comboBox_textEdited(QString)));

    this->connect(this->ui->applications_options_remove_pushButton, SIGNAL(pressed()), this,
                  SLOT(on_applications_options_remove_pushButton_pressed()));
    this->connect(this->ui->applications_options_duplicate_pushButton, SIGNAL(pressed()), this,
                  SLOT(on_applications_options_duplicate_pushButton_pressed()));
    this->connect(this->ui->applications_options_comboBox, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_applications_options_comboBox_activated(int)));
    this->connect(this->ui->applications_options_comboBox, SIGNAL(editTextChanged(QString)), this,
                  SLOT(on_applications_options_comboBox_textEdited(QString)));

    this->connect(this->ui->applications_layers_mode_comboBox, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_applications_layers_mode_comboBox_activated(int)));
    this->connect(this->ui->applications_configuration_comboBox, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_applications_configuration_comboBox_activated(int)));
}

TabApplications::~TabApplications() {}

void TabApplications::UpdateUI(UpdateUIMode mode) {
    const Configurator &configurator = Configurator::Get();

    const std::vector<Executable> &executables = configurator.executables.GetExecutables();

    if (mode == UPDATE_REBUILD_UI) {
        // Rebuild list of layers configurations
        const std::vector<Configuration> &configurations = configurator.configurations.available_configurations;
        ui->applications_configuration_comboBox->blockSignals(true);
        ui->applications_configuration_comboBox->clear();
        for (std::size_t i = 0, n = configurations.size(); i < n; ++i) {
            ui->applications_configuration_comboBox->addItem(configurations[i].key.c_str());
        }
        ui->applications_configuration_comboBox->blockSignals(false);

        // Rebuild list of applications
        ui->applications_list_comboBox->blockSignals(true);
        ui->applications_list_comboBox->clear();
        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            ui->applications_list_comboBox->addItem(executables[i].path.RelativePath().c_str());
        }
        ui->applications_list_comboBox->blockSignals(false);
        ui->applications_list_comboBox->setCurrentIndex(configurator.executables.GetActiveExecutableIndex());
    }

    this->on_applications_list_comboBox_activated(configurator.executables.GetActiveExecutableIndex());
}

void TabApplications::CleanUI() {}

bool TabApplications::EventFilter(QObject *target, QEvent *event) { return false; }

void TabApplications::on_applications_remove_application_pushButton_pressed() {
    Configurator &configurator = Configurator::Get();

    configurator.executables.RemoveExecutable(ui->applications_list_comboBox->currentIndex());
    ui->applications_list_comboBox->setEnabled(!configurator.executables.Empty());

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_applications_append_application_pushButton_pressed() {
    static const char *TABLE[] = {
        "Applications (*.exe)",     // PLATFORM_WINDOWS_X86
        "Applications (*.exe)",     // PLATFORM_WINDOWS_ARM
        "Applications (*)",         // PLATFORM_LINUX
        "Applications (*.app, *)",  // PLATFORM_MACOS
        "N/A",                      // PLATFORM_ANDROID
        "N/A"                       // PLATFORM_IOS
    };
    static_assert(std::size(TABLE) == PLATFORM_COUNT, "The tranlation table size doesn't match the enum number of elements");

    const std::string filter = TABLE[VKC_PLATFORM];

    Configurator &configurator = Configurator::Get();

    const Path &path = configurator.executables.last_path_executable;
    const Path &selected_path =
        QFileDialog::getOpenFileName(&this->window, "Executable Path", path.AbsolutePath().c_str(), filter.c_str()).toStdString();

    if (selected_path.Empty()) {
        return;
    }

    configurator.executables.AppendExecutable(selected_path);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_applications_list_comboBox_activated(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.executables.SelectActiveExecutable(index);

    const Executable *executable = configurator.executables.GetActiveExecutable();
    ui->applications_list_comboBox->setToolTip(executable->path.AbsolutePath().c_str());

    ui->applications_options_remove_pushButton->setEnabled(executable->options.size() > 1);

    ui->applications_options_comboBox->blockSignals(true);
    ui->applications_options_comboBox->clear();
    for (std::size_t i = 0, n = executable->options.size(); i < n; ++i) {
        ui->applications_options_comboBox->addItem(executable->options[i].label.c_str());
    }
    ui->applications_options_comboBox->blockSignals(false);
    ui->applications_options_comboBox->setCurrentIndex(executable->active_option_index);

    this->on_applications_options_comboBox_activated(executable->active_option_index);
}

void TabApplications::on_applications_list_comboBox_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    executable->path = text.toStdString();
}

void TabApplications::on_applications_options_remove_pushButton_pressed() {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();

    this->ui->applications_options_remove_pushButton->setEnabled(executable->options.size() > 1);
}

void TabApplications::on_applications_options_duplicate_pushButton_pressed() {}

void TabApplications::on_applications_options_comboBox_activated(int index) {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();

    executable->active_option_index = index;
    const ExecutableOptions *options = executable->GetActiveOptions();

    ui->applications_layers_mode_comboBox->setCurrentIndex(options->layers_mode);
    ui->applications_configuration_comboBox->setEnabled(options->layers_mode == LAYERS_CONTROLLED_BY_CONFIGURATOR);
    ui->applications_configuration_comboBox->setCurrentIndex(
        configurator.configurations.GetConfigurationIndex(options->configuration));

    ui->applications_directory_edit->setText(options->working_folder.RelativePath().c_str());
    ui->applications_directory_edit->setToolTip(options->working_folder.AbsolutePath().c_str());

    ui->application_args_list->blockSignals(true);
    ui->application_args_list->clear();
    for (std::size_t i = 0, n = options->args.size(); i < n; ++i) {
        ui->application_args_list->addItem(options->args[i].c_str());
    }
    ui->application_args_list->blockSignals(false);

    ui->application_envs_list->blockSignals(true);
    ui->application_envs_list->clear();
    for (std::size_t i = 0, n = options->envs.size(); i < n; ++i) {
        ui->application_envs_list->addItem(options->envs[i].c_str());
    }
    ui->application_envs_list->blockSignals(false);

    ui->application_output_log_edit->setText(options->log_file.RelativePath().c_str());
    ui->application_output_log_edit->setToolTip(options->log_file.AbsolutePath().c_str());
}

void TabApplications::on_applications_options_comboBox_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();

    ExecutableOptions *options = executable->GetActiveOptions();
    options->label = text.toStdString();
}

void TabApplications::on_applications_layers_mode_comboBox_activated(int index) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    ui->applications_configuration_comboBox->setEnabled(index == LAYERS_CONTROLLED_BY_CONFIGURATOR);
    options->layers_mode = static_cast<LayersMode>(index);
}

void TabApplications::on_applications_configuration_comboBox_activated(int index) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->configuration = ui->applications_configuration_comboBox->itemText(index).toStdString();
}
