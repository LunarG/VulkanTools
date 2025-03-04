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
    this->connect(this->ui->diagnostic_search_edit, SIGNAL(textEdited(QString)), this, SLOT(on_search_textEdited(QString)));
    this->connect(this->ui->diagnostic_search_edit, SIGNAL(returnPressed()), this, SLOT(on_search_find_pressed()));
    this->connect(this->ui->diagnostic_search_next, SIGNAL(clicked()), this, SLOT(on_search_next_pressed()));
    this->connect(this->ui->diagnostic_search_prev, SIGNAL(clicked()), this, SLOT(on_search_prev_pressed()));
    this->connect(this->ui->diagnostic_status_text, SIGNAL(customContextMenuRequested(QPoint)), this,
                  SLOT(on_customContextMenuRequested(const QPoint &)));
    this->connect(this->ui->diagnostic_status_text, SIGNAL(returnPressed()), this, SLOT(on_search_find_pressed()));
    this->connect(this->ui->diagnostic_search_hide, SIGNAL(clicked()), this, SLOT(on_hide_search()));
    this->connect(this->ui->diagnostic_search_case, SIGNAL(toggled(bool)), this, SLOT(on_search_case_toggled(bool)));
    this->connect(this->ui->diagnostic_search_whole, SIGNAL(toggled(bool)), this, SLOT(on_search_whole_toggled(bool)));
    this->connect(this->ui->diagnostic_search_regex, SIGNAL(toggled(bool)), this, SLOT(on_search_regex_toggled(bool)));

    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this->ui->diagnostic_status_text);
    this->connect(shortcut, SIGNAL(activated()), this, SLOT(on_show_search()));

    Configurator &configurator = Configurator::Get();

    this->ui->diagnostic_search_edit->setText(configurator.diagnostic_search_text.c_str());
    this->ui->diagnostic_search_next->setIcon(::Get(::ICON_NEXT));
    this->ui->diagnostic_search_prev->setIcon(::Get(::ICON_PREV));
    this->ui->diagnostic_search_hide->setIcon(::Get(::ICON_EXIT));
    this->ui->diagnostic_search_case->setIcon(::Get(::ICON_SEARCH_CASE));
    this->ui->diagnostic_search_whole->setIcon(::Get(::ICON_SEARCH_WHOLE));
    this->ui->diagnostic_search_regex->setIcon(::Get(::ICON_SEARCH_REGEX));

    this->ui->diagnostic_status_text->installEventFilter(&window);
    this->ui->diagnostic_status_text->document()->setMaximumBlockCount(65536);
    this->ui->diagnostic_status_text->setContextMenuPolicy(Qt::CustomContextMenu);

    this->ShowSearch(false);
    this->UpdateStatus();
}

TabDiagnostics::~TabDiagnostics() {}

void TabDiagnostics::UpdateStatus() {
    Configurator &configurator = Configurator::Get();

    configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

    this->status = configurator.GenerateVulkanStatus();
    this->ui->diagnostic_status_text->setText(this->status.c_str());

    configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
}

void TabDiagnostics::ShowSearch(bool visible) {
    this->ui->diagnostic_search_edit->setVisible(visible);
    this->ui->diagnostic_search_next->setVisible(visible);
    this->ui->diagnostic_search_next->setEnabled(!this->ui->diagnostic_search_edit->text().isEmpty());
    this->ui->diagnostic_search_prev->setVisible(visible);
    this->ui->diagnostic_search_prev->setEnabled(!this->ui->diagnostic_search_edit->text().isEmpty());
    this->ui->diagnostic_search_hide->setVisible(visible);
    this->ui->diagnostic_search_case->setVisible(visible);
    this->ui->diagnostic_search_whole->setVisible(visible);
    this->ui->diagnostic_search_regex->setVisible(visible);

    if (visible) {
        this->ui->diagnostic_search_edit->setFocus();
    }
}

void TabDiagnostics::UpdateUI(UpdateUIMode mode) {
    (void)mode;

    this->UpdateStatus();
}

void TabDiagnostics::CleanUI() {}

bool TabDiagnostics::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}

void TabDiagnostics::on_show_search() { this->ShowSearch(true); }

void TabDiagnostics::on_hide_search() { this->ShowSearch(false); }

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
            configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

            this->status = configurator.GenerateVulkanStatus();
            this->ui->diagnostic_status_text->setText(this->status.c_str());

            configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
        } else {
            this->status.clear();
            this->ui->diagnostic_status_text->clear();
        }
    } else if (action == action_search) {
        this->ShowSearch(true);
    } else if (action == action_save) {
        const QString selected_path =
            QFileDialog::getSaveFileName(this->ui->diagnostic_group_box_refresh, "Select Log file...",
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
