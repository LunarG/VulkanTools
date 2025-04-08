/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "tab_diagnostics.h"
#include "mainwindow.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QDesktopServices>
#include <QShortcut>

TabDiagnostics::TabDiagnostics(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DIAGNOSTIC, window, ui) {
    this->connect(this->ui->diagnostic_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mode_changed(int)));
    this->connect(this->ui->diagnostic_mode_options, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mode_options_changed(int)));
    this->connect(this->ui->diagnostic_export_folder, SIGNAL(clicked()), this, SLOT(on_export_folder()));
    this->connect(this->ui->diagnostic_export_file, SIGNAL(clicked()), this, SLOT(on_export_file()));
    this->connect(this->ui->diagnostic_search_edit, SIGNAL(textEdited(QString)), this, SLOT(on_search_textEdited(QString)));
    this->connect(this->ui->diagnostic_search_edit, SIGNAL(returnPressed()), this, SLOT(on_search_next_pressed()));
    this->connect(this->ui->diagnostic_search_clear, SIGNAL(clicked()), this, SLOT(on_search_clear_pressed()));
    this->connect(this->ui->diagnostic_search_next, SIGNAL(clicked()), this, SLOT(on_search_next_pressed()));
    this->connect(this->ui->diagnostic_search_prev, SIGNAL(clicked()), this, SLOT(on_search_prev_pressed()));
    this->connect(this->ui->diagnostic_status_text, SIGNAL(customContextMenuRequested(QPoint)), this,
                  SLOT(on_context_menu(const QPoint &)));
    this->connect(this->ui->diagnostic_search_case, SIGNAL(toggled(bool)), this, SLOT(on_search_case_toggled(bool)));
    this->connect(this->ui->diagnostic_search_whole, SIGNAL(toggled(bool)), this, SLOT(on_search_whole_toggled(bool)));
    this->connect(this->ui->diagnostic_search_regex, SIGNAL(toggled(bool)), this, SLOT(on_search_regex_toggled(bool)));

    QShortcut *shortcut_search = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this->ui->diagnostic_status_text);
    this->connect(shortcut_search, SIGNAL(activated()), this, SLOT(on_focus_search()));
    QShortcut *shortcut_next = new QShortcut(QKeySequence(Qt::Key_F3), this->ui->diagnostic_status_text);
    this->connect(shortcut_next, SIGNAL(activated()), this, SLOT(on_search_next_pressed()));
    QShortcut *shortcut_prev = new QShortcut(QKeySequence(Qt::SHIFT | Qt::Key_F3), this->ui->diagnostic_status_text);
    this->connect(shortcut_prev, SIGNAL(activated()), this, SLOT(on_search_prev_pressed()));

    QShortcut *shortcut_case = new QShortcut(QKeySequence(Qt::ALT | Qt::Key_C), this->ui->diagnostic_status_text);
    this->connect(shortcut_case, SIGNAL(activated()), this, SLOT(on_search_case_activated()));
    QShortcut *shortcut_whole = new QShortcut(QKeySequence(Qt::ALT | Qt::Key_W), this->ui->diagnostic_status_text);
    this->connect(shortcut_whole, SIGNAL(activated()), this, SLOT(on_search_whole_activated()));
    QShortcut *shortcut_regex = new QShortcut(QKeySequence(Qt::ALT | Qt::Key_R), this->ui->diagnostic_status_text);
    this->connect(shortcut_regex, SIGNAL(activated()), this, SLOT(on_search_regex_activated()));

    this->ui->diagnostic_search_next->setEnabled(false);
    this->ui->diagnostic_search_prev->setEnabled(false);

    this->ui->diagnostic_status_text->installEventFilter(&window);
    this->ui->diagnostic_status_text->document()->setMaximumBlockCount(65536);
    this->ui->diagnostic_status_text->setContextMenuPolicy(Qt::CustomContextMenu);

    this->UpdateStatus();
}

TabDiagnostics::~TabDiagnostics() {}

std::string TabDiagnostics::BuildStatus(DiagnosticMode selected_mode, std::size_t mode_index) {
    this->log_path.Clear();

    std::string log_status;

    Path working_directory = AbsolutePath(Path::APPDATA) + "/vkconfig/diagnostic";
    if (!working_directory.Exists()) {
        working_directory.Create();
    }

    Configurator &configurator = Configurator::Get();

    // configurator.Surrender(OVERRIDE_AREA_ALL);

    switch (selected_mode) {
        default: {
            assert(0);
        } break;
        case DIAGNOSTIC_VULKAN_STATUS: {
            log_status = configurator.GenerateVulkanStatus();
        } break;
        case DIAGNOSTIC_VULKAN_INFO_SUMMARY:
        case DIAGNOSTIC_VULKAN_INFO_TEXT:
        case DIAGNOSTIC_VULKAN_PROFILE:
        case DIAGNOSTIC_VULKAN_LOADER_LOG: {
            const DefaultExecutable defaults_executable{::GetExecutable(EXECUTABLE_VKINFO), "vulkaninfo"};
            const Executable executable(defaults_executable);

            if (this->process == nullptr) {
                this->process.reset(new QProcess(this));
                this->connect(this->process.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));
                this->connect(this->process.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
                this->connect(this->process.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
                              SLOT(processClosed(int, QProcess::ExitStatus)));
            }

            this->process->setProgram(executable.path.AbsolutePath().c_str());
            this->process->setWorkingDirectory(working_directory.AbsolutePath().c_str());
            this->process->setEnvironment(QProcess::systemEnvironment());

            QStringList args;

            std::string filename;

            if (selected_mode == DIAGNOSTIC_VULKAN_LOADER_LOG) {
                this->status.clear();

                args += "--summary";
                args += "-o";
                args += "vulkaninfo_tmp.txt";
            }
            if (selected_mode == DIAGNOSTIC_VULKAN_INFO_SUMMARY) {
                this->status.clear();

                filename = "vulkaninfo_summary.txt";

                args += "--summary";
                args += "-o";
                args += filename.c_str();
            }

            if (selected_mode == DIAGNOSTIC_VULKAN_INFO_TEXT) {
                filename = "vulkaninfo.txt";

                args += "--text";
                args += "-o";
                args += filename.c_str();
            }
            if (selected_mode == DIAGNOSTIC_VULKAN_PROFILE) {
                filename = format("%s.json", configurator.vulkan_system_info.physicalDevices[mode_index].deviceName.c_str());

                args += format("--json=%d", mode_index).c_str();
                args += "-o";
                args += filename.c_str();
            }

            if (!filename.empty()) {
                this->log_path = working_directory.AbsolutePath() + "/" + filename;
            }

            this->process->setArguments(args);

            this->process->setProcessChannelMode(QProcess::MergedChannels);
            this->process->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
            this->process->closeWriteChannel();

            while (this->process->waitForFinished(1000)) {
            }

            log_status = this->status;
        } break;
        case DIAGNOSTIC_VULKAN_LOADER_CONFIGURATION: {
            const Configuration *configuration = nullptr;

            Configurator &configurator = Configurator::Get();
            if (configurator.GetExecutableScope() == EXECUTABLE_PER) {
                const std::vector<Executable> &executables = configurator.executables.GetExecutables();
                configuration = configurator.configurations.FindConfiguration(executables[mode_index].configuration);
            } else {
                configuration = configurator.GetActiveConfiguration();
            }

            if (configuration != nullptr) {
                const Path &path = working_directory.AbsolutePath() + "/loader_configuration.json";
                configuration->Save(path.AbsolutePath(), true);

                QFile file(path.AbsolutePath().c_str());
                const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
                assert(result);
                log_status = file.readAll().toStdString();
                file.close();
            } else {
                log_status = "No active loader configuration selected.";
            }
        } break;
        case DIAGNOSTIC_VULKAN_LAYERS_SETTINGS: {
            Path path = AbsolutePath(Path::LAYERS_SETTINGS);
            if (configurator.GetExecutableScope() == EXECUTABLE_PER) {
                const std::vector<Executable> &executables = configurator.executables.GetExecutables();
                path = executables[mode_index].GetLocalLayersSettingsPath();
            }

            QFile file(path.AbsolutePath().c_str());
            const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
            if (result) {
                log_status = file.readAll().toStdString();
            } else {
                log_status = "No active loader configuration applied.";
            }
            file.close();
        } break;
        case DIAGNOSTIC_VULKAN_LOADER_SETTINGS: {
            const Path &path = AbsolutePath(Path::LOADER_SETTINGS);

            QFile file(path.AbsolutePath().c_str());
            const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
            if (result) {
                log_status = file.readAll().toStdString();
            } else {
                log_status = "No active loader configuration applied.";
            }
            file.close();
        } break;
    }

    /*
        if (this->log_path.Empty()) {
            this->ui->diagnostic_status_text->setText(log_status.c_str());
        }
    */

    // this->ui->diagnostic_search_clear->setVisible(!this->ui->diagnostic_search_edit->text().isEmpty());
    // this->ui->diagnostic_search_edit->setFocus();

    // configurator.Override(OVERRIDE_AREA_ALL);

    return log_status;
}

void TabDiagnostics::UpdateStatus() {
    this->status = this->BuildStatus(this->mode, this->ui->diagnostic_mode_options->currentIndex());

    this->ui->diagnostic_status_text->setText(this->status.c_str());

    this->ui->diagnostic_search_clear->setVisible(!this->ui->diagnostic_search_edit->text().isEmpty());
    this->ui->diagnostic_search_edit->setFocus();
}

void TabDiagnostics::UpdateUI(UpdateUIMode mode) {
    (void)mode;

    this->on_mode_changed(this->mode);
    // this->UpdateStatus();
}

void TabDiagnostics::CleanUI() {}

bool TabDiagnostics::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

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

    return false;
}

void TabDiagnostics::on_mode_changed(int index) {
    this->mode = static_cast<DiagnosticMode>(index);

    Configurator &configurator = Configurator::Get();
    const bool per_executable =
        (this->mode == DIAGNOSTIC_VULKAN_LOADER_CONFIGURATION || this->mode == DIAGNOSTIC_VULKAN_LAYERS_SETTINGS) &&
        configurator.GetExecutableScope() == EXECUTABLE_PER;
    const bool multiple_gpu = this->mode == DIAGNOSTIC_VULKAN_PROFILE && configurator.vulkan_system_info.physicalDevices.size() > 1;

    this->ui->diagnostic_mode_options->setVisible(multiple_gpu || per_executable);

    if (this->ui->diagnostic_mode_options->isVisible()) {
        this->ui->diagnostic_mode_options->blockSignals(true);
        this->ui->diagnostic_mode_options->clear();
        if (multiple_gpu) {
            for (std::size_t i = 0, n = configurator.vulkan_system_info.physicalDevices.size(); i < n; ++i) {
                this->ui->diagnostic_mode_options->addItem(configurator.vulkan_system_info.physicalDevices[i].deviceName.c_str());
            }
        } else {
            const std::vector<Executable> &executables = configurator.executables.GetExecutables();
            for (std::size_t i = 0, n = executables.size(); i < n; ++i) {
                if (i == 0) {
                    this->ui->diagnostic_mode_options->setToolTip(executables[i].path.AbsolutePath().c_str());
                }
                this->ui->diagnostic_mode_options->addItem(executables[i].path.Basename().c_str());
                this->ui->diagnostic_mode_options->setItemData(i, executables[i].path.AbsolutePath().c_str(), Qt::ToolTipRole);
            }
        }
        this->ui->diagnostic_mode_options->blockSignals(false);
    }

    this->UpdateStatus();
}

void TabDiagnostics::on_mode_options_changed(int index) { this->status = this->BuildStatus(this->mode, index); }

void TabDiagnostics::on_export_folder() {
    Configurator &configurator = Configurator::Get();
    const Configuration *configuration = nullptr;

    std::string saved_status = this->status;

    Path export_dir = configurator.last_path_status;
    if (!export_dir.Exists()) {
        export_dir.Create(false);
    }

    const QString selected_dir = QFileDialog::getExistingDirectory(
        this->ui->diagnostic_export_folder, "Select Diagnostic Logs Directory...", export_dir.AbsolutePath().c_str());
    if (selected_dir.isEmpty()) {
        return;
    }

    configurator.last_path_status = export_dir = selected_dir.toStdString();
    QDesktopServices::openUrl(QUrl::fromLocalFile(export_dir.AbsolutePath().c_str()));

    for (int diag_index = 0, diag_count = DIAGNOSTIC_MODE_COUNT; diag_index < diag_count; ++diag_index) {
        DiagnosticMode current_mode = static_cast<DiagnosticMode>(diag_index);
        std::size_t options_count = 1;

        switch (diag_index) {
            case DIAGNOSTIC_VULKAN_PROFILE:
                options_count = configurator.vulkan_system_info.physicalDevices.size();
                break;
            case DIAGNOSTIC_VULKAN_LOADER_CONFIGURATION:
            case DIAGNOSTIC_VULKAN_LAYERS_SETTINGS:
                if (configurator.GetExecutableScope() == EXECUTABLE_PER) {
                    options_count = configurator.executables.GetExecutables().size();
                }
                break;
            default:
                break;
        }

        for (std::size_t options_index = 0; options_index < options_count; ++options_index) {
            std::string log_status = this->BuildStatus(current_mode, options_index);

            Path export_path = export_dir;

            switch (current_mode) {
                default:
                case DIAGNOSTIC_VULKAN_STATUS:
                case DIAGNOSTIC_VULKAN_INFO_SUMMARY:
                case DIAGNOSTIC_VULKAN_INFO_TEXT:
                case DIAGNOSTIC_VULKAN_LOADER_LOG:
                case DIAGNOSTIC_VULKAN_LOADER_SETTINGS:
                    export_path = export_dir.RelativePath() + ::GetFilename(current_mode);
                    break;
                case DIAGNOSTIC_VULKAN_PROFILE: {
                    export_path =
                        export_dir.RelativePath() +
                        format("/%s.json", configurator.vulkan_system_info.physicalDevices[options_index].deviceName.c_str());
                } break;
                case DIAGNOSTIC_VULKAN_LOADER_CONFIGURATION:
                case DIAGNOSTIC_VULKAN_LAYERS_SETTINGS: {
                    if (configurator.GetExecutableScope() == EXECUTABLE_PER) {
                        const std::vector<Executable> &executables = configurator.executables.GetExecutables();
                        configuration = configurator.configurations.FindConfiguration(executables[options_index].configuration);
                    } else {
                        configuration = configurator.GetActiveConfiguration();
                    }

                    if (current_mode == DIAGNOSTIC_VULKAN_LOADER_CONFIGURATION) {
                        export_path = export_dir.RelativePath() + format("/%s.json", configuration->key.c_str());
                    } else {
                        export_path = export_dir.RelativePath() + format("/%s.txt", configuration->key.c_str());
                    }
                } break;
            }

            QFile json_file(export_path.AbsolutePath().c_str());
            const bool result = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
            if (result) {
                json_file.write(log_status.c_str());
                json_file.close();
            }
        }
    }

    this->status = saved_status;
}

void TabDiagnostics::on_export_file() {
    Configurator &configurator = Configurator::Get();

    const Configuration *configuration = nullptr;

    Path export_path = configurator.last_path_status;

    switch (this->mode) {
        default:
        case DIAGNOSTIC_VULKAN_STATUS:
        case DIAGNOSTIC_VULKAN_INFO_SUMMARY:
        case DIAGNOSTIC_VULKAN_INFO_TEXT:
        case DIAGNOSTIC_VULKAN_LOADER_LOG:
        case DIAGNOSTIC_VULKAN_LOADER_SETTINGS:
            export_path = export_path.RelativePath() + ::GetFilename(this->mode);
            break;
        case DIAGNOSTIC_VULKAN_PROFILE: {
            std::size_t index = this->ui->diagnostic_mode_options->currentIndex();

            export_path = export_path.RelativePath() +
                          format("/%s.json", configurator.vulkan_system_info.physicalDevices[index].deviceName.c_str());
        } break;
        case DIAGNOSTIC_VULKAN_LOADER_CONFIGURATION:
        case DIAGNOSTIC_VULKAN_LAYERS_SETTINGS: {
            if (configurator.GetExecutableScope() == EXECUTABLE_PER) {
                int index = this->ui->diagnostic_mode_options->currentIndex();
                const std::vector<Executable> &executables = configurator.executables.GetExecutables();
                configuration = configurator.configurations.FindConfiguration(executables[index].configuration);
            } else {
                configuration = configurator.GetActiveConfiguration();
            }

            if (this->mode == DIAGNOSTIC_VULKAN_LOADER_CONFIGURATION) {
                export_path = export_path.RelativePath() + format("/%s.json", configuration->key.c_str());
            } else {
                export_path = export_path.RelativePath() + format("/%s.txt", configuration->key.c_str());
            }
        } break;
    }

    export_path = QFileDialog::getSaveFileName(this->ui->diagnostic_export_file, "Save file...", export_path.AbsolutePath().c_str(),
                                               "Log (*.*)")
                      .toStdString();

    if (!export_path.Empty()) {
        configurator.last_path_status = export_path.AbsoluteDir();

        QFile json_file(export_path.AbsolutePath().c_str());
        const bool result = json_file.open(QIODevice::WriteOnly | QIODevice::Text);
        if (result) {
            json_file.write(this->status.c_str());
            json_file.close();

            QDesktopServices::openUrl(QUrl::fromLocalFile(export_path.AbsolutePath().c_str()));
        } else {
            QMessageBox message;
            message.setIcon(QMessageBox::Critical);
            message.setWindowTitle("Failed to save the diagnostic log!");
            message.setText(format("Couldn't write to '%s'.", export_path.AbsolutePath().c_str()).c_str());
            message.setInformativeText("Select a file path with 'write' rights.");
        }
    }
}

void TabDiagnostics::on_focus_search() { this->ui->diagnostic_search_edit->setFocus(); }

void TabDiagnostics::on_search_textEdited(const QString &text) {
    this->ui->diagnostic_search_next->setEnabled(!this->ui->diagnostic_search_edit->text().isEmpty());
    this->ui->diagnostic_search_prev->setEnabled(!this->ui->diagnostic_search_edit->text().isEmpty());

    this->diagnostic_search_text = text.toStdString();
    this->ui->diagnostic_search_clear->setVisible(!text.isEmpty());
}

void TabDiagnostics::on_search_clear_pressed() {
    this->diagnostic_search_text.clear();
    this->ui->diagnostic_search_edit->clear();
    this->ui->diagnostic_search_clear->setVisible(false);
}

void TabDiagnostics::on_search_next_pressed() { this->SearchFind(false); }

void TabDiagnostics::on_search_prev_pressed() { this->SearchFind(true); }

void TabDiagnostics::SearchFind(bool prev) {
    QTextDocument::FindFlags flags = prev ? QTextDocument::FindBackward : QTextDocument::FindFlags(0);

    if (this->search_case) {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (this->search_whole) {
        flags |= QTextDocument::FindWholeWords;
    }

    this->ui->diagnostic_status_text->setFocus();

    if (this->search_regex) {
        this->ui->diagnostic_status_text->find(QRegularExpression(this->ui->diagnostic_search_edit->text()), flags);
    } else {
        this->ui->diagnostic_status_text->find(this->ui->diagnostic_search_edit->text(), flags);
    }
}

void TabDiagnostics::on_search_case_activated() { this->ui->diagnostic_search_case->setChecked(!this->search_case); }

void TabDiagnostics::on_search_case_toggled(bool checked) {
    this->search_case = checked;
    this->ui->diagnostic_status_text->setFocus();
}

void TabDiagnostics::on_search_whole_activated() { this->ui->diagnostic_search_whole->setChecked(!this->search_whole); }

void TabDiagnostics::on_search_whole_toggled(bool checked) {
    this->search_whole = checked;
    this->ui->diagnostic_status_text->setFocus();
}

void TabDiagnostics::on_search_regex_activated() { this->ui->diagnostic_search_regex->setChecked(!this->search_regex); }

void TabDiagnostics::on_search_regex_toggled(bool checked) {
    this->search_regex = checked;
    this->ui->diagnostic_status_text->setFocus();
}

void TabDiagnostics::on_context_menu(const QPoint &pos) {
    Configurator &configurator = Configurator::Get();

    QMenu *menu = this->ui->diagnostic_status_text->createStandardContextMenu();
    menu->addSeparator();

    QAction *action_refresh = new QAction(this->status.empty() ? "Refresh" : "Clear", nullptr);
    action_refresh->setEnabled(true);
    menu->addAction(action_refresh);
    /*
    QAction *action_save = new QAction("Save...", nullptr);
    action_save->setEnabled(!this->status.empty());
    menu->addAction(action_save);
    */
    menu->addSeparator();

    QAction *action_search = new QAction("Search...", nullptr);
    action_search->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
    action_search->setEnabled(!configurator.GetShowDiagnosticSearch());
    menu->addAction(action_search);

    QAction *action = menu->exec(this->ui->diagnostic_status_text->mapToGlobal(pos));

    if (action == action_refresh) {
        if (this->status.empty()) {
            this->status = this->BuildStatus(static_cast<DiagnosticMode>(this->ui->diagnostic_mode->currentIndex()),
                                             this->ui->diagnostic_mode_options->currentIndex());
        } else {
            this->status.clear();
            this->ui->diagnostic_status_text->clear();
        }
    } else if (action == action_search) {
        this->on_focus_search();
    } /* else if (action == action_save) {
        const QString selected_path =
            QFileDialog::getSaveFileName(this->ui->diagnostic_status_text, "Select Log file...",
                                         configurator.last_path_status.AbsolutePath().c_str(), "Log (*.txt)");

        if (!selected_path.isEmpty()) {
            QFile file(selected_path.toStdString().c_str());
            const bool result = file.open(QFile::WriteOnly);
            if (!result) {
                QMessageBox message;
                message.setIcon(QMessageBox::Critical);
                message.setWindowTitle("Failed to save 'Vulkan Development Status'!");
                message.setText(format("Couldn't write to '%s'.", selected_path.toStdString().c_str()).c_str());
                message.setInformativeText("Select a file path with 'write' rights.");
                return;
            }

            file.write(this->status.c_str());
            file.close();

            configurator.last_path_status = selected_path.toStdString();

            QDesktopServices::openUrl(QUrl::fromLocalFile(selected_path.toStdString().c_str()));
        }
    } */

    menu->deleteLater();
}

void TabDiagnostics::processClosed(int exit_code, QProcess::ExitStatus status) {
    (void)exit_code;
    (void)status;

    assert(this->process);

    if (!this->log_path.Empty()) {
        QFile file(this->log_path.AbsolutePath().c_str());
        const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
        assert(result);
        this->status = file.readAll().toStdString();
        file.close();
    }

    this->ui->diagnostic_status_text->setText(this->status.c_str());

    if (this->process->processId() > 0) {
        this->process->kill();
        this->process->waitForFinished();
    }
}

void TabDiagnostics::standardOutputAvailable() {
    if (this->process != nullptr && this->log_path.Empty()) {
        this->status += this->process->readAllStandardOutput().toStdString();
    }
}

void TabDiagnostics::errorOutputAvailable() {
    if (this->process != nullptr && this->log_path.Empty()) {
        this->status += this->process->readAllStandardError().toStdString();
    }
}
