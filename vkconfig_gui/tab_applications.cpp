/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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
#include "style.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QDesktopServices>
#include <QShortcut>

static void PathInvalid(const Path &path, const char *message) {
    const std::string text = format("'%s' is not a valid path.", path.AbsolutePath().c_str());

    QMessageBox alert;
    alert.QDialog::setWindowTitle("The select path doesn't exist...");
    alert.setText(text.c_str());
    alert.setInformativeText(message);
    alert.setIcon(QMessageBox::Critical);
    alert.exec();
}

TabApplications::TabApplications(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui)
    : Tab(TAB_APPLICATIONS, window, ui),
      _launch_application(new QProcess(this)),
      timer_search(new QTimer(this)),
      timer_stream(new QTimer(this)) {
    this->connect(this->ui->launch_executable_list, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_launch_executable_list_activated(int)));
    this->connect(this->ui->launch_executable_list->lineEdit(), SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_executable_list_textEdited(QString)));
    this->connect(this->ui->launch_executable_search, SIGNAL(clicked()), this, SLOT(on_launch_executable_search_pressed()));
    this->connect(this->ui->launch_executable_append, SIGNAL(clicked()), this, SLOT(on_launch_executable_append_pressed()));
    this->connect(this->ui->launch_executable_remove, SIGNAL(clicked()), this, SLOT(on_launch_executable_remove_pressed()));

    this->connect(this->ui->launch_options_list, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_launch_options_list_activated(int)));
    this->connect(this->ui->launch_options_list->lineEdit(), &QLineEdit::editingFinished, this,
                  &TabApplications::on_launch_options_list_finished);
    this->connect(this->ui->launch_options_append, SIGNAL(clicked()), this, SLOT(on_launch_options_append_pressed()));
    this->connect(this->ui->launch_options_remove, SIGNAL(clicked()), this, SLOT(on_launch_options_remove_pressed()));

    this->connect(this->ui->launch_options_dir_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_options_dir_textEdited(QString)));
    this->connect(this->ui->launch_options_dir_button, SIGNAL(clicked()), this, SLOT(on_launch_options_dir_pressed()));
    this->connect(this->ui->launch_options_args_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_options_args_textEdited(QString)));
    this->connect(this->ui->launch_options_envs_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_options_envs_textEdited(QString)));
    this->connect(this->ui->launch_options_log_edit, SIGNAL(textEdited(QString)), this,
                  SLOT(on_launch_options_log_textEdited(QString)));
    this->connect(this->ui->launch_options_log_button, SIGNAL(clicked()), this, SLOT(on_launch_options_log_pressed()));
    this->connect(this->ui->launch_options_log_open, SIGNAL(clicked()), this, SLOT(on_launch_options_log_open_pressed()));

    this->connect(this->ui->launch_clear_at_launch, SIGNAL(toggled(bool)), this, SLOT(on_launch_clear_at_launch_toggled(bool)));
    this->connect(this->ui->launch_clear_log, SIGNAL(clicked()), this, SLOT(on_launch_clear_log_pressed()));
    this->connect(this->ui->launch_stdout_display, SIGNAL(currentIndexChanged(int)), this,
                  SLOT(on_launch_stdout_display_changed(int)));
    this->connect(this->ui->launch_button, SIGNAL(clicked()), this, SLOT(on_launch_button_pressed()));

    this->connect(this->ui->launch_search_edit, SIGNAL(textEdited(QString)), this, SLOT(on_search_textEdited(QString)));
    this->connect(this->ui->launch_search_edit, SIGNAL(returnPressed()), this, SLOT(on_search_next_pressed()));
    this->connect(this->ui->launch_search_clear, SIGNAL(clicked()), this, SLOT(on_search_clear_pressed()));
    this->connect(this->ui->launch_search_next, SIGNAL(clicked()), this, SLOT(on_search_next_pressed()));
    this->connect(this->ui->launch_search_prev, SIGNAL(clicked()), this, SLOT(on_search_prev_pressed()));
    this->connect(this->ui->launch_log_text, SIGNAL(customContextMenuRequested(QPoint)), this,
                  SLOT(on_context_menu(const QPoint &)));
    this->connect(this->ui->launch_search_case, SIGNAL(toggled(bool)), this, SLOT(on_search_case_toggled(bool)));
    this->connect(this->ui->launch_search_whole, SIGNAL(toggled(bool)), this, SLOT(on_search_whole_toggled(bool)));
    this->connect(this->ui->launch_search_regex, SIGNAL(toggled(bool)), this, SLOT(on_search_regex_toggled(bool)));
    this->connect(this->timer_search, &QTimer::timeout, this, &TabApplications::on_timer_search);
    this->connect(this->timer_stream, &QTimer::timeout, this, &TabApplications::on_timer_stream);

    QShortcut *shortcut_search = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this->ui->launch_log_text);
    this->connect(shortcut_search, SIGNAL(activated()), this, SLOT(on_focus_search()));
    QShortcut *shortcut_next = new QShortcut(QKeySequence(Qt::Key_F3), this->ui->launch_log_text);
    this->connect(shortcut_next, SIGNAL(activated()), this, SLOT(on_search_next_pressed()));
    QShortcut *shortcut_prev = new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F3), this->ui->launch_log_text);
    this->connect(shortcut_prev, SIGNAL(activated()), this, SLOT(on_search_prev_pressed()));
    /*
        QShortcut *shortcut_case = new QShortcut(QKeySequence(Qt::ALT | Qt::Key_C), this->ui->launch_search_case);
        this->connect(shortcut_case, SIGNAL(activated()), this, SLOT(on_search_case_activated()));
        QShortcut *shortcut_whole = new QShortcut(QKeySequence(Qt::ALT | Qt::Key_W), this->ui->launch_search_whole);
        this->connect(shortcut_whole, SIGNAL(activated()), this, SLOT(on_search_whole_activated()));
        QShortcut *shortcut_regex = new QShortcut(QKeySequence(Qt::ALT | Qt::Key_R), this->ui->launch_search_regex);
        this->connect(shortcut_regex, SIGNAL(activated()), this, SLOT(on_search_regex_activated()));
    */
    this->highlighter = new Highlighter(this->ui->launch_log_text->document());

    this->on_search_clear_pressed();

    this->ui->launch_search_case->setVisible(false);
    this->ui->launch_search_regex->setVisible(false);
    this->ui->launch_search_whole->setVisible(false);

    this->ui->launch_search_next->setEnabled(false);
    this->ui->launch_search_prev->setEnabled(false);

    // this->ui->launch_log_text->installEventFilter(&window);
    this->ui->launch_log_text->setContextMenuPolicy(Qt::CustomContextMenu);

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    // this->ui->launch_log_text->document()->setMaximumBlockCount(1024);
    this->ui->launch_log_text->moveCursor(QTextCursor::End);

    this->ui->launch_options_args_edit->setToolTip("Eg: '--argA --argB=valueB \"--argC=value C\" --argD=\"value D\"'");
    this->ui->launch_options_envs_edit->setToolTip("Eg: 'ENV_A= ENV_B=ValueB \"ENV_C=Value C\" ENV_D=\"Value D\"'");

    this->connect(this->_launch_application.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));
    this->connect(this->_launch_application.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
    this->connect(this->_launch_application.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
                  SLOT(processClosed(int, QProcess::ExitStatus)));

    // Dummy first launch so that following Vulkan Loader logging work...
    Configurator &configurator = Configurator::Get();
    int active_index = configurator.executables.GetActiveExecutableIndex();

    int vulkan_info = configurator.executables.GetVulkanInfoIndex();
    configurator.executables.SetActiveExecutable(vulkan_info);

    this->on_launch_button_pressed();

    configurator.executables.SetActiveExecutable(active_index);

    this->on_launch_button_pressed();
    this->on_launch_clear_log_pressed();
}

TabApplications::~TabApplications() { this->ResetLaunchApplication(); }

void TabApplications::UpdateUI(UpdateUIMode mode) {
    const Configurator &configurator = Configurator::Get();

    const std::vector<Executable> &executables = configurator.executables.GetExecutables();

    this->ui->launch_executable_search->setEnabled(!configurator.executables.Empty());
    this->ui->launch_executable_remove->setEnabled(!configurator.executables.Empty());
    this->ui->launch_button->setEnabled(!configurator.executables.Empty());

    if (mode == UPDATE_REBUILD_UI) {
        // Rebuild list of applications
        ui->launch_executable_list->blockSignals(true);
        ui->launch_executable_list->clear();
        for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
            ui->launch_executable_list->addItem(executables[i].path.AbsolutePath().c_str());
        }
        ui->launch_executable_list->setCurrentIndex(configurator.executables.GetActiveExecutableIndex());
        ui->launch_executable_list->blockSignals(false);
    }

    ui->launch_clear_at_launch->setChecked(configurator.executables.launcher_clear_on_launch);

    this->on_launch_executable_list_activated(configurator.executables.GetActiveExecutableIndex());

    this->ResetTextCursor();

    QPalette palette = this->ui->configurations_list->palette();
    QColor highlight = palette.color(QPalette::Highlight);
    this->highlighter->setColor(
        ::GetActualThemeMode(configurator.current_theme_mode) == THEME_MODE_FORCE_LIGHT ? highlight.lighter() : highlight.darker());
}

void TabApplications::CleanUI() { this->ResetLaunchApplication(); }

bool TabApplications::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    /*
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                this->SearchFind(false);
                return false;
        }
        return false;
    }
    */
    return false;
}

void TabApplications::on_launch_executable_search_pressed() {
    Configurator &configurator = Configurator::Get();

    const std::string &path = configurator.executables.GetActiveExecutable()->path.AbsolutePath();

    const QString &selected_path =
        QFileDialog::getOpenFileName(&this->window, "Executable Path", path.c_str(), ::GetExecutableFilter());

    if (selected_path.isEmpty()) {
        return;
    }

    configurator.executables.RenameActiveExecutable(selected_path.toStdString());
    configurator.executables.GetActiveExecutable()->path = selected_path.toStdString();

    configurator.Override(OVERRIDE_AREA_ALL);

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

    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI(UPDATE_REBUILD_UI);
}

void TabApplications::on_launch_executable_remove_pressed() {
    Configurator &configurator = Configurator::Get();
    const Executable *executable = configurator.executables.GetActiveExecutable();
    assert(executable != nullptr);

    if (!(configurator.Get(HIDE_MESSAGE_WARN_REMOVE_EXECUTABLE))) {
        QMessageBox message;
        message.setWindowTitle(format("Removing an executable from %s", VKCONFIG_NAME).c_str());
        message.setText(format("The '%s' executable '%s' is being removed, and its options will be lost.", VKCONFIG_NAME,
                               executable->path.AbsolutePath().c_str())
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

        this->ui->launch_executable_search->setEnabled(executable->removable);
        this->ui->launch_executable_remove->setEnabled(executable->removable);

        if (executable->removable) {
            this->ui->launch_executable_search->setToolTip("Update the current executable path");
            this->ui->launch_executable_remove->setToolTip("Delete the current executable path");
        } else {
            this->ui->launch_executable_search->setToolTip(
                "The current executable path can't be updated because the executable is necessary for Vulkan Configurator "
                "execution");
            this->ui->launch_executable_remove->setToolTip(
                "The current executable path can't be deleted because the executable is necessary for Vulkan Configurator "
                "execution");
        }
    }
    this->ui->launcher_options_list_layout->setEnabled(executable != nullptr);
    this->ui->launcher_applications_options->setEnabled(executable != nullptr);

    this->RebuildOptions();
}

void TabApplications::on_launch_executable_list_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    int index = configurator.executables.GetActiveExecutableIndex();

    Executable *executable = configurator.executables.GetActiveExecutable();
    if (executable != nullptr) {
        executable->path = text.toStdString();
    }

    configurator.executables.SetActiveExecutable(index);

    configurator.Override(OVERRIDE_AREA_ALL);

    this->ui->launch_executable_list->setCurrentIndex(configurator.executables.GetActiveExecutableIndex());

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

    ui->launch_options_dir_edit->setText(options->working_folder.AbsolutePath().c_str());
    ui->launch_options_dir_edit->setToolTip(options->working_folder.AbsolutePath().c_str());
    ui->launch_options_args_edit->setText(Merge(options->args, " ").c_str());
    ui->launch_options_envs_edit->setText(Merge(options->envs, " ").c_str());
    ui->launch_options_log_edit->setText(options->log_file.AbsolutePath().c_str());
    ui->launch_options_log_edit->setToolTip(options->log_file.AbsolutePath().c_str());
}

void TabApplications::on_launch_options_list_finished() {
    std::string new_text = this->ui->launch_options_list->lineEdit()->text().toStdString();

    Configurator &configurator = Configurator::Get();
    Executable *executable = configurator.executables.GetActiveExecutable();
    executable->RenameActiveOptions(new_text);

    this->RebuildOptions();
    this->ui->launch_options_list->setCurrentIndex(executable->GetActiveOptionsIndex());
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

    configurator.Override(OVERRIDE_AREA_ALL);
}

void TabApplications::on_launch_options_dir_pressed() {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    const QString selected_path = QFileDialog::getExistingDirectory(
        this->ui->launch_options_dir_button, "Select Working Directory...", options->working_folder.AbsolutePath().c_str());

    if (!selected_path.isEmpty()) {
        options->working_folder = selected_path.toStdString();
        this->ui->launch_options_dir_edit->setText(options->working_folder.RelativePath().c_str());
    }

    configurator.Override(OVERRIDE_AREA_ALL);
}

void TabApplications::on_launch_options_args_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->args = SplitArgs(text.toStdString());
}

void TabApplications::on_launch_options_envs_textEdited(const QString &text) {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    options->envs = SplitSpace(text.toStdString());
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

void TabApplications::on_launch_options_log_open_pressed() {
    Configurator &configurator = Configurator::Get();

    Executable *executable = configurator.executables.GetActiveExecutable();
    ExecutableOptions *options = executable->GetActiveOptions();

    if (!options->log_file.Exists()) {
        options->log_file.Create(true);
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(options->log_file.AbsolutePath().c_str()));
}

void TabApplications::on_launch_clear_at_launch_toggled(bool checked) {
    Configurator::Get().executables.launcher_clear_on_launch = checked;
}

void TabApplications::on_launch_clear_log_pressed() {
    this->ui->launch_search_edit->setEnabled(false);
    this->ui->launch_clear_log->setEnabled(false);
    this->ui->launch_log_text->clear();
    this->ui->launch_log_text->update();
}

void TabApplications::on_launch_stdout_display_changed(int index) {
    Configurator &configurator = Configurator::Get();
    configurator.stdout_display = static_cast<StdoutDisplay>(index);
}

void TabApplications::on_launch_button_pressed() {
    // Are we already monitoring a running app? If so, terminate it
    if (this->_launch_application != nullptr) {
        if (this->_launch_application->processId() > 0) {
            this->ResetLaunchApplication();
            return;
        }
    }

    Configurator &configurator = Configurator::Get();

    if (configurator.stdout_display == STDOUT_DISPLAY_ON_EXIT) {
        this->ui->launch_log_text->document()->setMaximumBlockCount(32768);
    } else {
        this->ui->launch_log_text->document()->setMaximumBlockCount(configurator.app_log_max_blocks);
    }

    this->highlighter->setDocument(nullptr);

    // We are logging, let's add that we've launched a new application
    std::string launch_log = "Launching Vulkan Application:\n";

    const Executable *active_executable = configurator.executables.GetActiveExecutable();

    assert(!active_executable->path.Empty());
    launch_log += format("- Executable: %s\n", active_executable->path.AbsolutePath().c_str());
    if (false && !active_executable->path.Exists()) {
        ::PathInvalid(active_executable->path,
                      format("The '%s' application will fail to launch.", active_executable->path.AbsolutePath().c_str()).c_str());
    }

    const ExecutableOptions *options = active_executable->GetActiveOptions();

    launch_log += format("- Working Directory: %s\n", options->working_folder.AbsolutePath().c_str());
    if (!options->working_folder.Empty() && !options->working_folder.Exists()) {
        ::PathInvalid(options->working_folder,
                      format("The '%s' application will fail to launch.", active_executable->path.AbsolutePath().c_str()).c_str());
    }

    if (!options->args.empty()) {
        launch_log += "- Command-line Arguments:\n";
        for (std::size_t i = 0, n = options->args.size(); i < n; ++i) {
            launch_log += format("  - %s\n", TrimSurroundingWhitespace(options->args[i]).c_str());
        }
    }

    if (!options->envs.empty()) {
        launch_log += "- Environment Variables:\n";
        for (std::size_t i = 0, n = options->envs.size(); i < n; ++i) {
            launch_log += format("  - %s\n", TrimSurroundingWhitespace(options->envs[i]).c_str());
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
                QMessageBox alert;
                alert.setWindowTitle("Cannot open log file");
                alert.setText(format("Cannot open %s...", options->log_file.AbsolutePath().c_str()).c_str());
                alert.setIcon(QMessageBox::Warning);
                alert.exec();
            }
        }
    }

    if (this->ui->launch_clear_at_launch->isChecked()) {
        this->ui->launch_log_text->clear();
    }
    this->Log(launch_log.c_str(), true);

    // Launch the test application
    this->_launch_application->setProgram(active_executable->path.AbsolutePath().c_str());
    this->_launch_application->setWorkingDirectory(options->working_folder.AbsolutePath().c_str());

    QStringList envs = QProcess::systemEnvironment();
    for (std::size_t i = 0, n = options->envs.size(); i < n; ++i) {
        std::string env = TrimSurroundingWhitespace(options->envs[i]);
        envs.append(env.c_str());
    }
    this->_launch_application->setEnvironment(envs);

    QStringList args;
    for (std::size_t i = 0, n = options->args.size(); i < n; ++i) {
        std::string arg = TrimSurroundingWhitespace(options->args[i]);
        args.append(arg.c_str());
    }
    this->_launch_application->setArguments(args);

    this->ui->launch_button->setText("Terminate");
    this->_launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    this->_launch_application->setProcessChannelMode(QProcess::MergedChannels);
    this->_launch_application->closeWriteChannel();

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!this->_launch_application->waitForStarted(4000)) {
        this->ui->launch_button->setText("Launch");

        const std::string failed_log = std::string("Failed to launch ") + active_executable->path.AbsolutePath().c_str() + "!\n";
        this->Log(failed_log.c_str(), true);
    }
}

void TabApplications::EnableOptions() {
    Configurator &configurator = Configurator::Get();
    const bool executable_enabled = !configurator.executables.Empty();

    const Executable *executable = configurator.executables.GetActiveExecutable();
    const bool options_enabled = executable_enabled && (executable == nullptr ? false : !executable->GetOptions().empty());

    this->ui->launch_button->setEnabled(executable_enabled);
    this->ui->launch_executable_search->setEnabled(executable_enabled);
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
    if (this->_launch_application != nullptr) {
        if (this->_launch_application->processId() > 0) {
            this->_launch_application->kill();
            this->_launch_application->waitForFinished();
        }
    }

    this->ui->launch_button->setText("Launch");
}

/// The process we are following is closed. We don't actually care about the
/// exit status/code, we just need to know to destroy the QProcess object
/// and set it back to nullptr so that we know we can launch a new app.
/// Also, if we are logging, it's time to close the log file.
void TabApplications::processClosed(int exit_code, QProcess::ExitStatus status) {
    (void)exit_code;
    (void)status;

    assert(this->_launch_application);

    this->on_timer_stream();  // flush log buffer

    this->Log("Process terminated", true);

    if (this->_log_file.isOpen()) {
        this->_log_file.close();

        Configurator &configurator = Configurator::Get();
        if (configurator.stdout_display == STDOUT_DISPLAY_ON_EXIT) {
            this->_log_file.open(QIODevice::ReadOnly | QIODevice::Text);
            this->ui->launch_log_text->setPlainText(this->_log_file.readAll());
            this->_log_file.close();
        }
    }

    this->ResetLaunchApplication();
}

/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void TabApplications::standardOutputAvailable() {
    if (this->_launch_application) {
        this->stream_text += this->_launch_application->readAllStandardOutput();
        if (!this->timer_stream->isActive()) {
            this->timer_stream->start(200);
        }
    }
}

void TabApplications::errorOutputAvailable() {
    if (this->_launch_application) {
        this->stream_text += this->_launch_application->readAllStandardError();
        if (!this->timer_stream->isActive()) {
            this->timer_stream->start(200);
        }
    }
}

void TabApplications::on_timer_stream() {
    this->timer_stream->stop();
    this->Log(this->stream_text, false);
    this->stream_text.clear();
}

void TabApplications::Log(const QString &log, bool flush) {
    this->ui->launch_search_edit->setEnabled(true);
    this->ui->launch_clear_log->setEnabled(true);

    Configurator &configurator = Configurator::Get();
    if (configurator.stdout_display == STDOUT_DISPLAY_STREAM || flush) {
        this->ui->launch_log_text->insertPlainText(log);
        this->ui->launch_log_text->moveCursor(QTextCursor::End);
    }

    if (this->_log_file.isOpen()) {
        this->_log_file.write(log.toStdString().c_str(), log.size());
        if (flush) {
            this->_log_file.flush();
        }
    }
}

void TabApplications::on_focus_search() { this->ui->launch_search_edit->setFocus(); }

void TabApplications::on_timer_search() {
    this->timer_search->stop();

    this->ResetTextCursor();

    if (!this->search_text.empty()) {
        this->highlighter->setSearch(this->search_text.c_str());

        this->ui->launch_log_text->moveCursor(QTextCursor::StartOfWord);
        this->SearchFind(false);
        this->text_is_reset = false;
    }
}

void TabApplications::on_search_textEdited(const QString &text) {
    this->timer_search->start(200);

    this->search_text = text.toStdString();

    this->ui->launch_search_next->setEnabled(!this->ui->launch_search_edit->text().isEmpty());
    this->ui->launch_search_prev->setEnabled(!this->ui->launch_search_edit->text().isEmpty());
    this->ui->launch_search_clear->setEnabled(!text.isEmpty());
}

void TabApplications::ResetTextCursor() {
    if (this->ui->launch_log_text->document()->isEmpty()) {
        return;
    }

    if (this->text_is_reset) {
        return;
    }

    QTextCursor cursor = this->ui->launch_log_text->textCursor();
    int saved_anchor = cursor.anchor();
    int saved_Position = cursor.position();

    cursor.setPosition(0, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    QTextCharFormat format;
    cursor.setCharFormat(format);

    cursor.setPosition(saved_Position, QTextCursor::MoveAnchor);
    cursor.setPosition(saved_anchor, QTextCursor::KeepAnchor);

    this->text_is_reset = true;
}

void TabApplications::on_search_clear_pressed() {
    this->highlighter->setDocument(nullptr);
    this->highlighter->setSearch("");

    this->search_text.clear();
    this->ui->launch_search_edit->clear();
    this->ui->launch_search_next->setEnabled(false);
    this->ui->launch_search_prev->setEnabled(false);
    this->ui->launch_search_clear->setEnabled(false);

    this->ResetTextCursor();
}

void TabApplications::on_search_next_pressed() {
    QTextCursor cursor = this->ui->launch_log_text->textCursor();
    this->ui->launch_log_text->moveCursor(QTextCursor::EndOfWord);
    this->ui->launch_log_text->setTextCursor(cursor);
    this->SearchFind(false);
}

void TabApplications::on_search_prev_pressed() {
    QTextCursor cursor = this->ui->launch_log_text->textCursor();
    this->ui->launch_log_text->moveCursor(QTextCursor::StartOfWord);
    this->ui->launch_log_text->setTextCursor(cursor);
    this->SearchFind(true);
}

void TabApplications::SearchFind(bool prev) {
    if (this->highlighter->document() == nullptr) {
        this->highlighter->setDocument(this->ui->launch_log_text->document());
    }

    QTextDocument::FindFlags flags = prev ? QTextDocument::FindBackward : QTextDocument::FindFlags(0);

    if (this->search_case) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (this->search_whole) {
        flags |= QTextDocument::FindWholeWords;
    }

    bool found = false;
    if (this->search_regex) {
        found = this->ui->launch_log_text->find(QRegularExpression(this->ui->launch_search_edit->text()), flags);
    } else {
        found = this->ui->launch_log_text->find(this->ui->launch_search_edit->text(), flags);
    }

    QTextCursor cursor = this->ui->launch_log_text->textCursor();

    if (found) {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        // format.setForeground(Qt::red);
        // format.setFontStyleHint();
        //         format.setBackground(Qt::yellow);
        cursor.mergeCharFormat(format);

        this->ui->launch_log_text->ensureCursorVisible();
    } else {
        if (!prev && !cursor.atStart()) {
            this->ui->launch_log_text->moveCursor(QTextCursor::Start);
            this->SearchFind(prev);
            return;
        }

        if (prev && !cursor.atEnd()) {
            this->ui->launch_log_text->moveCursor(QTextCursor::End);
            this->SearchFind(prev);
            return;
        }
    }
}

void TabApplications::on_search_case_activated() { this->ui->launch_search_case->setChecked(!this->search_case); }

void TabApplications::on_search_case_toggled(bool checked) {
    this->search_case = checked;
    this->ui->launch_log_text->setFocus();
}

void TabApplications::on_search_whole_activated() { this->ui->launch_search_whole->setChecked(!this->search_whole); }

void TabApplications::on_search_whole_toggled(bool checked) {
    this->search_whole = checked;
    this->ui->launch_log_text->setFocus();
}

void TabApplications::on_search_regex_activated() { this->ui->launch_search_regex->setChecked(!this->search_regex); }

void TabApplications::on_search_regex_toggled(bool checked) {
    this->search_regex = checked;
    this->ui->launch_log_text->setFocus();
}

void TabApplications::on_context_menu(const QPoint &pos) {
    QMenu *menu = this->ui->launch_log_text->createStandardContextMenu();
    menu->addSeparator();

    QAction *action_clear = new QAction("Clear", nullptr);
    action_clear->setEnabled(true);
    menu->addAction(action_clear);

    menu->addSeparator();

    QAction *action_search = new QAction("Search...", nullptr);
    action_search->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
    action_search->setEnabled(true);
    menu->addAction(action_search);

    QAction *action = menu->exec(this->ui->launch_log_text->mapToGlobal(pos));

    if (action == action_clear) {
        this->on_launch_clear_log_pressed();
    } else if (action == action_search) {
        QTextCursor cursor = this->ui->launch_log_text->textCursor();
        if (cursor.hasSelection()) {
            QString text = cursor.selectedText();
            this->ui->launch_log_text->setPlainText(text);
            this->on_search_textEdited(text);
        }
        this->on_focus_search();
    }

    menu->deleteLater();
}
