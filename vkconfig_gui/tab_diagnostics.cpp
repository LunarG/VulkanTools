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
#include "widget_resize_button.h"
#include "style.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QDesktopServices>
#include <QShortcut>

TabDiagnostics::TabDiagnostics(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DIAGNOSTIC, window, ui) {
    this->connect(this->ui->diagnostic_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mode_changed(int)));
    this->connect(this->ui->diagnostic_export, SIGNAL(clicked()), this, SLOT(on_export()));
    this->connect(this->ui->diagnostic_search_edit, SIGNAL(textEdited(QString)), this, SLOT(on_search_textEdited(QString)));
    this->connect(this->ui->diagnostic_search_edit, SIGNAL(returnPressed()), this, SLOT(on_search_next_pressed()));
    this->connect(this->ui->diagnostic_search_next, SIGNAL(clicked()), this, SLOT(on_search_next_pressed()));
    this->connect(this->ui->diagnostic_search_prev, SIGNAL(clicked()), this, SLOT(on_search_prev_pressed()));
    this->connect(this->ui->diagnostic_status_text, SIGNAL(customContextMenuRequested(QPoint)), this,
                  SLOT(on_customContextMenuRequested(const QPoint &)));
    // this->connect(this->ui->diagnostic_status_text, SIGNAL(returnPressed()), this, SLOT(on_search_next_pressed()));
    this->connect(this->ui->diagnostic_search_case, SIGNAL(toggled(bool)), this, SLOT(on_search_case_toggled(bool)));
    this->connect(this->ui->diagnostic_search_whole, SIGNAL(toggled(bool)), this, SLOT(on_search_whole_toggled(bool)));
    this->connect(this->ui->diagnostic_search_regex, SIGNAL(toggled(bool)), this, SLOT(on_search_regex_toggled(bool)));

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this->ui->diagnostic_status_text);
    this->connect(shortcut, SIGNAL(activated()), this, SLOT(on_focus_search()));

    Configurator &configurator = Configurator::Get();

    this->ui->diagnostic_export->setVisible(false);

    this->ui->diagnostic_search_edit->setText(configurator.diagnostic_search_text.c_str());
    this->ui->diagnostic_search_next->setEnabled(!this->ui->diagnostic_search_edit->text().isEmpty());
    this->ui->diagnostic_search_prev->setEnabled(!this->ui->diagnostic_search_edit->text().isEmpty());

    this->ui->diagnostic_status_text->installEventFilter(&window);
    this->ui->diagnostic_status_text->document()->setMaximumBlockCount(65536);
    this->ui->diagnostic_status_text->setContextMenuPolicy(Qt::CustomContextMenu);

    this->UpdateStatus();
}

TabDiagnostics::~TabDiagnostics() {}

void TabDiagnostics::UpdateStatus() {
    Configurator &configurator = Configurator::Get();

    // configurator.Surrender(OVERRIDE_AREA_ALL);

    switch (this->mode) {
        default: {
            assert(0);
        } break;
        case DIAGNOSTIC_VULKAN_STATUS: {
            this->status = configurator.GenerateVulkanStatus();
            this->ui->diagnostic_status_text->setText(this->status.c_str());
        } break;
        case DIAGNOSTIC_VULKAN_LOADER_LOG:
        case DIAGNOSTIC_VULKAN_INFO_SUMMARY:
        case DIAGNOSTIC_VULKAN_INFO_TEXT:
        case DIAGNOSTIC_VULKAN_PROFILE: {
            std::string saved_envs;

            if (this->mode == DIAGNOSTIC_VULKAN_LOADER_LOG) {
                bool is_set = qEnvironmentVariableIsSet("VK_LOADER_DEBUG");
                saved_envs = is_set ? qgetenv("VK_LOADER_DEBUG").toStdString() : "";
                qputenv("VK_LOADER_DEBUG", "all");
            }

            const DefaultExecutable defaults_executable{::GetExecutable(EXECUTABLE_VKINFO), "vulkaninfo"};
            const Executable executable(defaults_executable);
            const ExecutableOptions *options = executable.GetActiveOptions();

            if (this->launch_application == nullptr) {
                this->launch_application.reset(new QProcess(this));
                this->connect(this->launch_application.get(), SIGNAL(readyReadStandardOutput()), this,
                              SLOT(standardOutputAvailable()));
                this->connect(this->launch_application.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
                this->connect(this->launch_application.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
                              SLOT(processClosed(int, QProcess::ExitStatus)));
            }

            this->launch_application->setProgram(executable.path.AbsolutePath().c_str());
            this->launch_application->setWorkingDirectory(options->working_folder.AbsolutePath().c_str());
            this->launch_application->setEnvironment(QProcess::systemEnvironment());

            QStringList args;
            args += "--summary";
            if (this->mode == DIAGNOSTIC_VULKAN_LOADER_LOG) {
                args += "-o";
                args += "vulkaninfo_summary.txt";
            }

            this->launch_application->setArguments(args);

            this->launch_application->setProcessChannelMode(QProcess::MergedChannels);
            this->launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
            this->launch_application->closeWriteChannel();

            // if (!this->launch_application->waitForStarted(4000)) {}
            while (!this->launch_application->waitForFinished(2000)) {
            }

            this->launch_application->kill();

            if (this->mode == DIAGNOSTIC_VULKAN_LOADER_LOG) {
                qputenv("VK_LOADER_DEBUG", saved_envs.c_str());
            }
        } break;
        case DIAGNOSTIC_ACTIVE_LOADER_CONFIGURATION: {
            const Configuration *configuration = configurator.GetActiveConfiguration();

            if (configuration != nullptr) {
                const Path &path = AbsolutePath(Path::HOME) + "/active_configuration.json";
                configuration->Save(path.AbsolutePath(), true);

                QFile file(path.AbsolutePath().c_str());
                const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
                assert(result);
                this->status = file.readAll().toStdString();
                file.close();
            } else {
                this->status = "No active loader configuration selected.";
            }

            this->ui->diagnostic_status_text->setText(this->status.c_str());
        } break;
        case DIAGNOSTIC_VULKAN_LOADER_SETTINGS: {
            const Path &path = AbsolutePath(Path::LOADER_SETTINGS);

            QFile file(path.AbsolutePath().c_str());
            const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
            if (result) {
                this->status = file.readAll().toStdString();
            } else {
                this->status = "No active loader configuration applied.";
            }
            file.close();

            this->ui->diagnostic_status_text->setText(this->status.c_str());
        } break;
        case DIAGNOSTIC_VULKAN_LAYERS_SETTINGS: {
            const Path &path = AbsolutePath(Path::LAYERS_SETTINGS);

            QFile file(path.AbsolutePath().c_str());
            const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
            if (result) {
                this->status = file.readAll().toStdString();
            } else {
                this->status = "No active loader configuration applied.";
            }
            file.close();

            this->ui->diagnostic_status_text->setText(this->status.c_str());
        } break;
    }

    // configurator.Override(OVERRIDE_AREA_ALL);
}

void TabDiagnostics::UpdateUI(UpdateUIMode mode) {
    (void)mode;

    this->UpdateStatus();
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
    this->status.clear();
    this->UpdateStatus();
    this->ui->diagnostic_search_edit->setFocus();
}

void TabDiagnostics::on_export() {}

void TabDiagnostics::on_focus_search() { this->ui->diagnostic_search_edit->setFocus(); }

void TabDiagnostics::on_search_textEdited(const QString &text) {
    this->ui->diagnostic_search_next->setEnabled(!this->ui->diagnostic_search_edit->text().isEmpty());
    this->ui->diagnostic_search_prev->setEnabled(!this->ui->diagnostic_search_edit->text().isEmpty());

    Configurator &configurator = Configurator::Get();
    configurator.diagnostic_search_text = text.toStdString();
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

void TabDiagnostics::on_search_case_toggled(bool checked) {
    this->search_case = checked;
    this->ui->diagnostic_status_text->setFocus();
}

void TabDiagnostics::on_search_whole_toggled(bool checked) {
    this->search_whole = checked;
    this->ui->diagnostic_status_text->setFocus();
}

void TabDiagnostics::on_search_regex_toggled(bool checked) {
    this->search_regex = checked;
    this->ui->diagnostic_status_text->setFocus();
}

void TabDiagnostics::on_customContextMenuRequested(const QPoint &pos) {
    Configurator &configurator = Configurator::Get();

    QMenu *menu = this->ui->diagnostic_status_text->createStandardContextMenu();
    menu->addSeparator();

    QAction *action_refresh = new QAction(this->status.empty() ? "Refresh" : "Clear", nullptr);
    action_refresh->setEnabled(true);
    menu->addAction(action_refresh);

    QAction *action_save = new QAction("Save...", nullptr);
    action_save->setEnabled(!this->status.empty());
    menu->addAction(action_save);

    menu->addSeparator();

    QAction *action_search = new QAction("Search...", nullptr);
    action_search->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
    action_search->setEnabled(!configurator.GetShowDiagnosticSearch());
    menu->addAction(action_search);

    QAction *action = menu->exec(this->ui->diagnostic_status_text->mapToGlobal(pos));

    if (action == action_refresh) {
        if (this->status.empty()) {
            this->UpdateStatus();
        } else {
            this->status.clear();
            this->ui->diagnostic_status_text->clear();
        }
    } else if (action == action_search) {
        this->on_focus_search();
    } else if (action == action_save) {
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
    }

    menu->deleteLater();
}

void TabDiagnostics::processClosed(int exit_code, QProcess::ExitStatus status) {
    (void)exit_code;
    (void)status;

    assert(this->launch_application);

    this->ui->diagnostic_status_text->setText(this->status.c_str());

    if (this->launch_application->processId() > 0) {
        this->launch_application->kill();
        this->launch_application->waitForFinished();
    }
}

void TabDiagnostics::standardOutputAvailable() {
    if (this->launch_application != nullptr) {
        this->status += this->launch_application->readAllStandardOutput().toStdString();
    }
}

void TabDiagnostics::errorOutputAvailable() {
    if (this->launch_application != nullptr) {
        this->status += this->launch_application->readAllStandardError().toStdString();
    }
}
