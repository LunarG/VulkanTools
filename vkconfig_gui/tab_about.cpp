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

#include "tab_about.h"
#include "mainwindow.h"

#define VKCONFIG_DATE 1
#include "../vkconfig_core/date.h"

#include <QDesktopServices>

TabAbout::TabAbout(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_ABOUT, window, ui) {
    this->connect(this->ui->about_lunarg_pushButton, SIGNAL(pressed()), this, SLOT(on_about_lunarg_pushButton_pressed()));
    this->connect(this->ui->about_qt_pushButton, SIGNAL(pressed()), this, SLOT(on_about_qt_pushButton_pressed()));

    this->ui->about_version->setText(format("Vulkan %s", Version::VKHEADER.str().c_str()).c_str());
    QPalette palette = this->ui->about_version->palette();
    palette.setColor(QPalette::WindowText, QColor(164, 30, 34));
    this->ui->about_version->setPalette(palette);
}

TabAbout::~TabAbout() {}

void TabAbout::UpdateUI(UpdateUIMode mode) {
    if (mode == UPDATE_REBUILD_UI) {
        // We need to rebuild in case the style theme changed
        QFile file(":/CHANGELOG.md");
        const bool result = file.open(QIODevice::ReadOnly | QIODevice::Text);
        assert(result);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        this->ui->about_changelog_textBrowser->setMarkdown(file.readAll());
#else
        // Workaround, only for building, display will be broken
        this->ui->about_changelog_textBrowser->setText(file.readAll());
#endif
        file.close();
    }
}

void TabAbout::CleanUI() {}

bool TabAbout::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}

void TabAbout::on_about_lunarg_pushButton_pressed() {
    QDesktopServices::openUrl(QUrl("https://www.lunarg.com/", QUrl::TolerantMode));
}

void TabAbout::on_about_qt_pushButton_pressed() { QDesktopServices::openUrl(QUrl("https://www.qt.io/", QUrl::TolerantMode)); }
