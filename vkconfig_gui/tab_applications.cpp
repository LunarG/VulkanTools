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
#include "../vkconfig_core/alert.h"

#include <QFileDialog>

TabApplications::TabApplications(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui)
    : Tab(TAB_APPLICATIONS, window, ui), _launch_application(nullptr) {
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
    this->connect(this->ui->applications_options_append_pushButton, SIGNAL(pressed()), this,
                  SLOT(on_applications_options_append_pushButton_pressed()));
    this->connect(this->ui->applications_options_comboBox, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_applications_options_comboBox_activated(int)));
    this->connect(this->ui->applications_options_comboBox, SIGNAL(editTextChanged(QString)), this,
                  SLOT(on_applications_options_comboBox_textEdited(QString)));

    this->connect(this->ui->applications_configuration_comboBox, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_applications_configuration_comboBox_activated(int)));

    this->connect(this->ui->applications_directory_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_applications_directory_edit_textEdited(QString)));
    this->connect(this->ui->applications_directory_edit_pushButton, SIGNAL(pressed()), this,
                  SLOT(on_applications_directory_edit_pushButton_pressed()));
    this->connect(this->ui->applications_args_list, SIGNAL(textEdited(QString)), this,
                  SLOT(on_applications_args_list_textEdited(QString)));
    this->connect(this->ui->applications_envs_list, SIGNAL(textEdited(QString)), this,
                  SLOT(on_applications_envs_list_textEdited(QString)));
    this->connect(this->ui->applications_output_log_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_applications_output_log_edit_textEdited(QString)));
    this->connect(this->ui->applications_output_log_pushButton, SIGNAL(pressed()), this,
                  SLOT(on_applications_output_log_pushButton_pressed()));

    this->connect(this->ui->applications_push_button_launcher, SIGNAL(pressed()), this,
                  SLOT(on_applications_launcher_pushButton_pressed()));
    this->connect(this->ui->applications_push_button_clear_log, SIGNAL(pressed()), this,
                  SLOT(on_applications_clear_log_pushButton_pressed()));
    this->connect(this->ui->applications_check_box_clear_on_launch, SIGNAL(pressed()), this,
                  SLOT(on_applications_check_box_clear_on_launch_pressed()));

    const Configurator &configurator = Configurator::Get();

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    this->ui->log_browser->document()->setMaximumBlockCount(2048);

    this->ui->applications_push_button_launcher->setEnabled(!configurator.executables.Empty());

    this->ui->applications_args_list->setToolTip("Eg: '--argA --argB'");
    this->ui->applications_envs_list->setToolTip(VKC_ENV == VKC_ENV_WIN32 ? "Eg: 'ENV_A=ValueA;ENV_B=ValueB'"
                                                                          : "Eg: 'ENV_A=ValueA:ENV_B=ValueB'");
}

TabApplications::~TabApplications() { this->ResetLaunchApplication(); }

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

    ui->applications_check_box_clear_on_launch->setChecked(configurator.executables.launcher_clear_on_launch);

    this->on_applications_list_comboBox_activated(configurator.executables.GetActiveExecutableIndex());
}

void TabApplications::CleanUI() { this->ResetLaunchApplication(); }

bool TabApplications::EventFilter(QObject *target, QEvent *event) { return false; }

void TabApplications::on_applications_remove_application_pushButton_pressed() {
    Configurator &configurator = Configurator::Get();

    configurator.executables.RemoveExecutable(ui->applications_list_comboBox->currentIndex());
    this->EnableOptions();

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_applications_append_application_pushButton_pressed() {
    Configurator &configurator = Configurator::Get();

    const Path &path = configurator.executables.last_path_executable;
    const Path &selected_path =
        QFileDialog::getOpenFileName(&this->window, "Executable Path", path.AbsolutePath().c_str(), ::GetExecutableFilter())
            .toStdString();

    if (selected_path.Empty()) {
        return;
    }

    configurator.executables.AppendExecutable(selected_path);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_applications_list_comboBox_activated(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.executables.SetActiveExecutable(index);

    const Executable *executable = configurator.executables.GetActiveExecutable();

    if (executable != nullptr) {
        ui->applications_list_comboBox->setToolTip(executable->path.AbsolutePath().c_str());
        ui->applications_options_remove_pushButton->setEnabled(executable->options.size() > 1);
        this->on_applications_options_comboBox_activated(executable->GetActiveOptionsIndex());
    }
    ui->applications_options_list_layout->setEnabled(executable != nullptr);
    ui->applications_options_layout->setEnabled(executable != nullptr);

    ui->applications_options_comboBox->blockSignals(true);
    ui->applications_options_comboBox->clear();
    if (executable != nullptr) {
        for (std::size_t i = 0, n = executable->options.size(); i < n; ++i) {
            ui->applications_options_comboBox->addItem(executable->options[i].label.c_str());
        }
    }
    ui->applications_options_comboBox->blockSignals(false);

    if (executable != nullptr) {
        ui->applications_options_comboBox->setCurrentIndex(executable->GetActiveOptionsIndex());
    }
}

void TabApplications::on_applications_list_comboBox_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    if (executable != nullptr) {
        executable->path = text.toStdString();
    }
}

void TabApplications::on_applications_options_remove_pushButton_pressed() {
    Configurator &configurator = Configurator::Get();

    this->EnableOptions();
}

void TabApplications::on_applications_options_append_pushButton_pressed() {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();

    ExecutableOptions options = *executable->GetActiveOptions();
    options.label = configurator.executables.MakeOptionsName(options.label);

    executable->active_option = options.label;
    executable->options.push_back(options);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_applications_options_comboBox_activated(int index) {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();

    executable->active_option = executable->options[index].label;
    const ExecutableOptions *options = executable->GetActiveOptions();

    // ui->applications_layers_mode_comboBox->setCurrentIndex(options->layers_mode);
    // ui->applications_configuration_comboBox->setEnabled(options->layers_mode == LAYERS_CONTROLLED_BY_CONFIGURATOR);
    ui->applications_configuration_comboBox->setCurrentIndex(
        configurator.configurations.GetConfigurationIndex(options->configuration));

    ui->applications_directory_edit->setText(options->working_folder.RelativePath().c_str());
    ui->applications_directory_edit->setToolTip(options->working_folder.AbsolutePath().c_str());
    ui->applications_args_list->setText(Merge(options->args, " ").c_str());
    ui->applications_envs_list->setText(Merge(options->envs, ",").c_str());
    ui->applications_output_log_edit->setText(options->log_file.RelativePath().c_str());
    ui->applications_output_log_edit->setToolTip(options->log_file.AbsolutePath().c_str());
}

void TabApplications::on_applications_options_comboBox_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();

    ExecutableOptions *options = executable->GetActiveOptions();
    options->label = text.toStdString();
}

void TabApplications::on_applications_configuration_comboBox_activated(int index) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->configuration = ui->applications_configuration_comboBox->itemText(index).toStdString();
}

void TabApplications::on_applications_directory_edit_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->working_folder = text.toStdString();
}

void TabApplications::on_applications_directory_edit_pushButton_pressed() {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    const QString selected_path =
        QFileDialog::getExistingDirectory(this->ui->applications_directory_edit_pushButton, "Select Working Directory...",
                                          this->ui->applications_directory_edit->text());

    if (!selected_path.isEmpty()) {
        options->working_folder = selected_path.toStdString();
        this->ui->applications_directory_edit->setText(options->working_folder.RelativePath().c_str());
    }
}

void TabApplications::on_applications_args_list_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->args = Split(text.toStdString(), " ");
}

void TabApplications::on_applications_envs_list_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->envs = Split(text.toStdString(), VKC_ENV == VKC_ENV_WIN32 ? ";" : ":");
}

void TabApplications::on_applications_output_log_edit_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->log_file = text.toStdString();
}

void TabApplications::on_applications_output_log_pushButton_pressed() {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    const QString selected_path = QFileDialog::getSaveFileName(this->ui->applications_output_log_pushButton, "Select Log file...",
                                                               this->ui->applications_output_log_edit->text(), "Log (*.txt)");

    if (!selected_path.isEmpty()) {
        options->log_file = selected_path.toStdString();
        this->ui->applications_output_log_edit->setText(options->log_file.RelativePath().c_str());
    }
}

void TabApplications::on_check_box_clear_on_launch_clicked() {
    Configurator::Get().executables.launcher_clear_on_launch = ui->applications_check_box_clear_on_launch->isChecked();
}

void TabApplications::on_applications_clear_log_pushButton_pressed() {
    ui->log_browser->clear();
    ui->log_browser->update();
    ui->applications_push_button_clear_log->setEnabled(false);
}

void TabApplications::on_applications_launcher_pushButton_pressed() {
    // Are we already monitoring a running app? If so, terminate it
    if (_launch_application != nullptr) {
        if (_launch_application->processId() > 0) {
            this->ResetLaunchApplication();
            return;
        }
    }

    // We are logging, let's add that we've launched a new application
    std::string launch_log = "Launching Vulkan Application:\n";

    Configurator &configurator = Configurator::Get();
    const Executable *active_executable = configurator.executables.GetActiveExecutable();

    assert(!active_executable->path.Empty());
    launch_log += format("- Executable: %s\n", active_executable->path.AbsolutePath().c_str());
    if (!active_executable->path.Exists()) {
        Alert::PathInvalid(
            active_executable->path,
            format("The '%s' application will fail to launch.", active_executable->path.AbsolutePath().c_str()).c_str());
    }

    const ExecutableOptions *options = active_executable->GetActiveOptions();

    launch_log += format("- Working Directory: %s\n", options->working_folder.AbsolutePath().c_str());
    if (!options->working_folder.Exists()) {
        Alert::PathInvalid(
            options->working_folder,
            format("The '%s' application will fail to launch.", active_executable->path.AbsolutePath().c_str()).c_str());
    }
    /*
    if (!_launcher_arguments->text().isEmpty()) {
        launch_log += format("- Command-line Arguments: %s\n", _launcher_arguments->text().toStdString().c_str());
    }
    */
    if (!options->log_file.Empty()) {
        launch_log += format("- Log file: %s\n", options->log_file.AbsolutePath().c_str());
    }

    if (!options->log_file.Empty()) {
        // Start logging
        // Make sure the log file is not already opened. This can occur if the
        // launched application is closed from the applicaiton.
        if (!this->_log_file.isOpen()) {
            this->_log_file.setFileName(options->log_file.AbsolutePath().c_str());

            // Open and append, or open and truncate?
            QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
            if (!ui->applications_check_box_clear_on_launch->isChecked()) {
                mode |= QIODevice::Append;
            }

            if (!this->_log_file.open(mode)) {
                Alert::LogFileFailed();
            }
        }
    }

    if (ui->applications_check_box_clear_on_launch->isChecked()) {
        ui->log_browser->clear();
    }
    this->Log(launch_log.c_str());

    // Launch the test application
    if (_launch_application == nullptr) {
        _launch_application.reset(new QProcess(this));
        this->connect(_launch_application.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));
        this->connect(_launch_application.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
        this->connect(_launch_application.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
                      SLOT(processClosed(int, QProcess::ExitStatus)));
    }

    this->_launch_application->setProgram(active_executable->path.AbsolutePath().c_str());
    this->_launch_application->setWorkingDirectory(options->working_folder.AbsolutePath().c_str());

    Configuration *configuration = configurator.configurations.FindConfiguration(options->configuration);

    QStringList env = QProcess::systemEnvironment();
    env << (QString("VK_LOADER_DEBUG=") + ::GetLogString(configuration->loader_log_messages_flags).c_str());
    if (!options->envs.empty()) {
        const QStringList envs = ConvertString(options->envs);
        env << envs;
    }
    this->_launch_application->setEnvironment(env);

    if (!options->args.empty()) {
        const QStringList args = ConvertString(options->args);
        this->_launch_application->setArguments(args);
    }

    this->ui->applications_push_button_launcher->setText("Terminate");
    this->_launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    this->_launch_application->setProcessChannelMode(QProcess::MergedChannels);
    this->_launch_application->closeWriteChannel();

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!this->_launch_application->waitForStarted(4000)) {
        this->ui->applications_push_button_launcher->setText("Launch");

        const std::string failed_log = std::string("Failed to launch ") + active_executable->path.AbsolutePath().c_str() + "!\n";
        this->Log(failed_log);
    }
}

void TabApplications::EnableOptions() {
    Configurator &configurator = Configurator::Get();
    const bool executable_enabled = !configurator.executables.Empty();

    const Executable *executable = configurator.executables.GetActiveExecutable();
    const bool options_enabled = executable_enabled && (executable == nullptr ? false : !executable->options.empty());

    this->ui->applications_push_button_launcher->setEnabled(executable_enabled);
    this->ui->applications_list_comboBox->setEnabled(executable_enabled);
    this->ui->applications_remove_application_pushButton->setEnabled(executable_enabled);

    this->ui->applications_options_comboBox->setEnabled(executable_enabled);
    this->ui->applications_options_remove_pushButton->setEnabled(executable_enabled ? (executable->options.size() > 1) : false);
    this->ui->applications_options_append_pushButton->setEnabled(executable_enabled);

    this->ui->applications_directory_edit->setEnabled(options_enabled);
    this->ui->applications_directory_edit_pushButton->setEnabled(options_enabled);
    this->ui->applications_args_list->setEnabled(options_enabled);
    this->ui->applications_envs_list->setEnabled(options_enabled);
    this->ui->applications_output_log_edit->setEnabled(options_enabled);
    this->ui->applications_output_log_pushButton->setEnabled(options_enabled);

    if (!options_enabled) {
        this->ui->applications_directory_edit->clear();
        this->ui->applications_args_list->clear();
        this->ui->applications_envs_list->clear();
        this->ui->applications_output_log_edit->clear();
    }
}

void TabApplications::ResetLaunchApplication() {
    if (_launch_application != nullptr) {
        if (_launch_application->processId() > 0) {
            this->_launch_application->kill();
            this->_launch_application->waitForFinished();
        }
    }

    ui->applications_push_button_launcher->setText("Launch");
}

/// The process we are following is closed. We don't actually care about the
/// exit status/code, we just need to know to destroy the QProcess object
/// and set it back to nullptr so that we know we can launch a new app.
/// Also, if we are logging, it's time to close the log file.
void TabApplications::processClosed(int exit_code, QProcess::ExitStatus status) {
    (void)exit_code;
    (void)status;

    assert(_launch_application);

    Log("Process terminated");

    if (_log_file.isOpen()) {
        _log_file.close();
    }

    ResetLaunchApplication();
}

/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void TabApplications::standardOutputAvailable() {
    if (_launch_application) {
        this->Log(_launch_application->readAllStandardOutput().toStdString());
    }
}

void TabApplications::errorOutputAvailable() {
    if (_launch_application) {
        this->Log(_launch_application->readAllStandardError().toStdString());
    }
}

void TabApplications::Log(const std::string &log) {
    ui->log_browser->setPlainText(ui->log_browser->toPlainText() + "\n" + log.c_str());
    ui->applications_push_button_clear_log->setEnabled(true);

    if (this->_log_file.isOpen()) {
        this->_log_file.write(log.c_str(), log.size());
        this->_log_file.flush();
    }
}
