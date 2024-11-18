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
    this->connect(this->ui->launch_executable_list, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_launch_executable_list_activated(int)));
    this->connect(this->ui->launch_executable_list->lineEdit(), SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_executable_list_textEdited(QString)));
    this->connect(this->ui->launch_executable_search, SIGNAL(pressed()), this, SLOT(on_launch_executable_search_pressed()));
    this->connect(this->ui->launch_executable_append, SIGNAL(pressed()), this, SLOT(on_launch_executable_append_pressed()));
    this->connect(this->ui->launch_executable_remove, SIGNAL(pressed()), this, SLOT(on_launch_executable_remove_pressed()));

    this->connect(this->ui->launch_options_list, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_launch_options_list_activated(int)));
    this->connect(this->ui->launch_options_list->lineEdit(), SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_options_list_textEdited(QString)));
    this->connect(this->ui->launch_options_append, SIGNAL(pressed()), this, SLOT(on_launch_options_append_pressed()));
    this->connect(this->ui->launch_options_remove, SIGNAL(pressed()), this, SLOT(on_launch_options_remove_pressed()));

    this->connect(this->ui->launch_options_dir_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_options_dir_textEdited(QString)));
    this->connect(this->ui->launch_options_dir_button, SIGNAL(pressed()), this, SLOT(on_launch_options_dir_pressed()));
    this->connect(this->ui->launch_options_args_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_options_args_textEdited(QString)));
    this->connect(this->ui->launch_options_envs_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_options_envs_textEdited(QString)));
    this->connect(this->ui->launch_options_log_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_options_log_textEdited(QString)));
    this->connect(this->ui->launch_options_log_button, SIGNAL(pressed()), this, SLOT(on_launch_options_log_pressed()));

    this->connect(this->ui->launch_clear_at_launch, SIGNAL(pressed()), this, SLOT(on_launch_clear_at_launch_pressed()));
    this->connect(this->ui->launch_clear_log, SIGNAL(pressed()), this, SLOT(on_launch_clear_log_pressed()));
    this->connect(this->ui->launch_button, SIGNAL(pressed()), this, SLOT(on_launch_button_pressed()));

    const Configurator &configurator = Configurator::Get();

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    this->ui->launch_log_text->document()->setMaximumBlockCount(2048);

    this->ui->launch_button->setEnabled(!configurator.executables.Empty());

    this->ui->launch_options_args_edit->setToolTip("Eg: '--argA --argB'");
    this->ui->launch_options_envs_edit->setToolTip(VKC_ENV == VKC_ENV_WIN32 ? "Eg: 'ENV_A=ValueA;ENV_B=ValueB'"
                                                                            : "Eg: 'ENV_A=ValueA:ENV_B=ValueB'");
}

TabApplications::~TabApplications() { this->ResetLaunchApplication(); }

void TabApplications::UpdateUI(UpdateUIMode mode) {
    const Configurator &configurator = Configurator::Get();

    const std::vector<Executable> &executables = configurator.executables.GetExecutables();

    if (mode == UPDATE_REBUILD_UI) {
        // Rebuild list of applications
        ui->launch_executable_list->blockSignals(true);
        ui->launch_executable_list->clear();
        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            ui->launch_executable_list->addItem(executables[i].path.RelativePath().c_str());
        }
        ui->launch_executable_list->setCurrentIndex(configurator.executables.GetActiveExecutableIndex());
        ui->launch_executable_list->blockSignals(false);
    }

    ui->launch_clear_at_launch->setChecked(configurator.executables.launcher_clear_on_launch);

    this->on_launch_executable_list_activated(configurator.executables.GetActiveExecutableIndex());
}

void TabApplications::CleanUI() { this->ResetLaunchApplication(); }

bool TabApplications::EventFilter(QObject *target, QEvent *event) { return false; }

void TabApplications::on_launch_executable_search_pressed() {
    Configurator &configurator = Configurator::Get();

    const std::string &path = configurator.executables.GetActiveExecutable()->path.AbsolutePath();

    const QString &selected_path =
        QFileDialog::getOpenFileName(&this->window, "Executable Path", path.c_str(), ::GetExecutableFilter());

    if (selected_path.isEmpty()) {
        return;
    }

    configurator.executables.GetActiveExecutable()->path = selected_path.toStdString();

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_launch_executable_append_pressed() {
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

void TabApplications::on_launch_executable_remove_pressed() {
    Configurator &configurator = Configurator::Get();
    const Executable *executable = configurator.executables.GetActiveExecutable();
    assert(executable != nullptr);

    if (!(configurator.Get(HIDE_MESSAGE_WARN_REMOVE_EXECUTABLE))) {
        QMessageBox message;
        message.setWindowTitle(format("Removing an executable from %s", VKCONFIG_NAME).c_str());
        message.setText(format("The '%s' executable is being removed from %s with it's options that will be definitly lost.",
                               VKCONFIG_NAME, executable->path.AbsolutePath().c_str())
                            .c_str());
        message.setInformativeText("Do you want to continue?");
        message.setIcon(QMessageBox::Warning);
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message.setDefaultButton(QMessageBox::Yes);
        message.setCheckBox(new QCheckBox("Do not show again."));
        int ret_val = message.exec();

        if (message.checkBox()->isChecked()) {
            configurator.Set(HIDE_MESSAGE_WARN_REMOVE_EXECUTABLE);
        }

        if (ret_val == QMessageBox::No) {
            return;
        }
    }

    configurator.Surrender(OVERRIDE_AREA_ALL);
    configurator.executables.RemoveExecutable();
    configurator.Override(OVERRIDE_AREA_ALL);

    this->EnableOptions();
    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_launch_executable_list_activated(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.executables.SetActiveExecutable(index);

    const Executable *executable = configurator.executables.GetActiveExecutable();

    if (executable != nullptr) {
        this->ui->launch_executable_list->setToolTip(executable->path.AbsolutePath().c_str());
        this->ui->launch_options_remove->setEnabled(executable->GetOptions().size() > 1);
        this->on_launch_options_list_activated(executable->GetActiveOptionsIndex());
    }
    this->ui->launcher_options_list_layout->setEnabled(executable != nullptr);
    this->ui->launcher_applications_options->setEnabled(executable != nullptr);

    this->RebuildOptions();
}

void TabApplications::on_launch_executable_list_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    if (executable != nullptr) {
        executable->path = text.toStdString();
    }

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_launch_options_list_activated(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.Surrender(OVERRIDE_AREA_ALL);

    Executable *executable = configurator.executables.GetActiveExecutable();
    const std::vector<ExecutableOptions> &options_list = executable->GetOptions();

    executable->SetActiveOptions(options_list[index].label);

    configurator.Override(OVERRIDE_AREA_ALL);

    const ExecutableOptions *options = executable->GetActiveOptions();

    ui->launch_options_dir_edit->setText(options->working_folder.RelativePath().c_str());
    ui->launch_options_dir_edit->setToolTip(options->working_folder.AbsolutePath().c_str());
    ui->launch_options_args_edit->setText(Merge(options->args, " ").c_str());
    ui->launch_options_envs_edit->setText(Merge(options->envs, GetToken(PARSE_ENV_VAR)).c_str());
    ui->launch_options_log_edit->setText(options->log_file.RelativePath().c_str());
    ui->launch_options_log_edit->setToolTip(options->log_file.AbsolutePath().c_str());
}

void TabApplications::on_launch_options_list_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();
    executable->RenameActiveOptions(text.toStdString());

    this->RebuildOptions();
    this->ui->launch_options_list->setCurrentIndex(executable->GetActiveOptionsIndex());
    // this->ui->launch_options_list->setItemText(executable->GetActiveOptionsIndex(), text);
}

void TabApplications::on_launch_options_append_pressed() {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();

    executable->DuplicateActiveOptions();

    this->EnableOptions();
    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_launch_options_remove_pressed() {
    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();
    assert(executable != nullptr);

    if (!(configurator.Get(HIDE_MESSAGE_WARN_REMOVE_EXECUTABLE_OPTIONS))) {
        QMessageBox message;
        message.setWindowTitle("Removing executable launch options...");
        message.setText(format("This action is removing '%s' launch options for '%s'.", executable->GetActiveOptionsName().c_str(),
                               executable->path.RelativePath().c_str())
                            .c_str());
        message.setInformativeText("Do you want to continue?");
        message.setIcon(QMessageBox::Warning);
        message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        message.setDefaultButton(QMessageBox::No);
        message.setCheckBox(new QCheckBox("Do not show again."));

        int ret_val = message.exec();

        if (message.checkBox()->isChecked()) {
            configurator.Set(HIDE_MESSAGE_WARN_REMOVE_EXECUTABLE_OPTIONS);
        }

        if (ret_val == QMessageBox::No) {
            return;
        }
    }

    configurator.Surrender(OVERRIDE_AREA_ALL);
    executable->RemoveActiveOptions();
    configurator.Override(OVERRIDE_AREA_ALL);

    this->EnableOptions();
    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_launch_options_dir_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->working_folder = text.toStdString();
}

void TabApplications::on_launch_options_dir_pressed() {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    const QString selected_path = QFileDialog::getExistingDirectory(
        this->ui->launch_options_dir_button, "Select Working Directory...", this->ui->launch_options_dir_edit->text());

    if (!selected_path.isEmpty()) {
        options->working_folder = selected_path.toStdString();
        this->ui->launch_options_dir_edit->setText(options->working_folder.RelativePath().c_str());
    }
}

void TabApplications::on_launch_options_args_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->args = Split(text.toStdString(), " ");
}

void TabApplications::on_launch_options_envs_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->envs = Split(text.toStdString(), ::GetToken(PARSE_ENV_VAR));
}

void TabApplications::on_launch_options_log_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->log_file = text.toStdString();
}

void TabApplications::on_launch_options_log_pressed() {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();
    const std::string &log = options->log_file.AbsolutePath();

    const QString selected_path =
        QFileDialog::getSaveFileName(this->ui->launch_options_log_button, "Select Log file...", log.c_str(), "Log (*.txt)");

    if (!selected_path.isEmpty()) {
        options->log_file = selected_path.toStdString();
        this->ui->launch_options_log_edit->setText(options->log_file.RelativePath().c_str());
    }
}

void TabApplications::on_launch_clear_at_launch_pressed() {
    Configurator::Get().executables.launcher_clear_on_launch = ui->launch_clear_at_launch->isChecked();
}

void TabApplications::on_launch_clear_log_pressed() {
    ui->launch_log_text->clear();
    ui->launch_log_text->update();
    ui->launch_clear_log->setEnabled(false);
}

void TabApplications::on_launch_button_pressed() {
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

    if (!options->args.empty()) {
        launch_log += "- Command-line Arguments:\n";
        for (std::size_t i = 0, n = options->args.size(); i < n; ++i) {
            launch_log += format("  - %s\n", options->args[i].c_str());
        }
    }

    if (!options->envs.empty()) {
        launch_log += "- Environment Variables:\n";
        for (std::size_t i = 0, n = options->envs.size(); i < n; ++i) {
            launch_log += format("  - %s\n", options->envs[i].c_str());
        }
    }

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
            if (!ui->launch_clear_at_launch->isChecked()) {
                mode |= QIODevice::Append;
            }

            if (!this->_log_file.open(mode)) {
                Alert::LogFileFailed();
            }
        }
    }

    if (ui->launch_clear_at_launch->isChecked()) {
        ui->launch_log_text->clear();
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

    Configuration *configuration = configurator.configurations.FindConfiguration(active_executable->configuration);

    QStringList env = QProcess::systemEnvironment();

    if (!options->envs.empty()) {
        const QStringList envs = ConvertString(options->envs);
        env << envs;
    }
    this->_launch_application->setEnvironment(env);

    if (!options->args.empty()) {
        const QStringList args = ConvertString(options->args);
        this->_launch_application->setArguments(args);
    }

    this->ui->launch_button->setText("Terminate");
    this->_launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    this->_launch_application->setProcessChannelMode(QProcess::MergedChannels);
    this->_launch_application->closeWriteChannel();

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!this->_launch_application->waitForStarted(4000)) {
        this->ui->launch_button->setText("Launch");

        const std::string failed_log = std::string("Failed to launch ") + active_executable->path.AbsolutePath().c_str() + "!\n";
        this->Log(failed_log);
    }
}

void TabApplications::EnableOptions() {
    Configurator &configurator = Configurator::Get();
    const bool executable_enabled = !configurator.executables.Empty();

    const Executable *executable = configurator.executables.GetActiveExecutable();
    const bool options_enabled = executable_enabled && (executable == nullptr ? false : !executable->GetOptions().empty());

    this->ui->launch_button->setEnabled(executable_enabled);
    this->ui->launch_executable_list->setEnabled(executable_enabled);
    this->ui->launch_executable_remove->setEnabled(executable_enabled);

    this->ui->launch_options_list->setEnabled(executable_enabled);
    this->ui->launch_options_remove->setEnabled(executable_enabled ? (executable->GetOptions().size() > 1) : false);
    this->ui->launch_options_append->setEnabled(executable_enabled);

    this->ui->launch_options_dir_edit->setEnabled(options_enabled);
    this->ui->launch_options_dir_button->setEnabled(options_enabled);
    this->ui->launch_options_args_edit->setEnabled(options_enabled);
    this->ui->launch_options_envs_edit->setEnabled(options_enabled);
    this->ui->launch_options_log_edit->setEnabled(options_enabled);
    this->ui->launch_options_log_button->setEnabled(options_enabled);

    if (!options_enabled) {
        this->ui->launch_options_dir_edit->clear();
        this->ui->launch_options_args_edit->clear();
        this->ui->launch_options_envs_edit->clear();
        this->ui->launch_options_log_edit->clear();
    }
}

void TabApplications::RebuildOptions() {
    Configurator &configurator = Configurator::Get();
    const Executable *executable = configurator.executables.GetActiveExecutable();

    this->ui->launch_options_list->blockSignals(true);
    this->ui->launch_options_list->clear();
    if (executable != nullptr) {
        for (std::size_t i = 0, n = executable->GetOptions().size(); i < n; ++i) {
            this->ui->launch_options_list->addItem(executable->GetOptions()[i].label.c_str());
        }
    }

    if (executable != nullptr) {
        this->ui->launch_options_list->setCurrentIndex(executable->GetActiveOptionsIndex());
    }
    this->ui->launch_options_list->blockSignals(false);
}

void TabApplications::ResetLaunchApplication() {
    if (_launch_application != nullptr) {
        if (_launch_application->processId() > 0) {
            this->_launch_application->kill();
            this->_launch_application->waitForFinished();
        }
    }

    ui->launch_button->setText("Launch");
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
    ui->launch_log_text->setPlainText(ui->launch_log_text->toPlainText() + "\n" + log.c_str());
    ui->launch_clear_log->setEnabled(true);

    if (this->_log_file.isOpen()) {
        this->_log_file.write(log.c_str(), log.size());
        this->_log_file.flush();
    }
}
