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

#include "tab_documentation.h"
#include "mainwindow.h"

static void UpdateColor1(QLabel *label, const QColor &color) {
    std::string ref = label->text().toStdString();
    const std::string hex = color.name().toStdString();
    std::string text = format(ref.c_str(), hex.c_str());
    label->setText(text.c_str());
}

static void UpdateColor2(QLabel *label, const QColor &color) {
    std::string ref = label->text().toStdString();
    const std::string hex = color.name().toStdString();
    std::string text = format(ref.c_str(), hex.c_str(), hex.c_str());
    label->setText(text.c_str());
}

static void UpdateColor3(QLabel *label, const QColor &color) {
    std::string ref = label->text().toStdString();
    const std::string hex = color.name().toStdString();
    std::string text = format(ref.c_str(), hex.c_str(), hex.c_str(), hex.c_str());
    label->setText(text.c_str());
}

static void UpdateColor4(QLabel *label, const QColor &color) {
    std::string ref = label->text().toStdString();
    const std::string hex = color.name().toStdString();
    std::string text = format(ref.c_str(), hex.c_str(), hex.c_str(), hex.c_str(), hex.c_str());
    label->setText(text.c_str());
}

TabDocumentation::TabDocumentation(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DOCUMENTATION, window, ui) {
    QPalette palette = this->ui->documentation_spec->palette();
    QColor color = palette.color(QPalette::Text);

    ::UpdateColor1(this->ui->documentation_spec_html, color);
    ::UpdateColor1(this->ui->documentation_spec_pdf, color);

    ::UpdateColor1(this->ui->documentation_doc0, color);
    ::UpdateColor1(this->ui->documentation_doc1, color);
    ::UpdateColor1(this->ui->documentation_doc2, color);

    ::UpdateColor2(this->ui->documentation_sdk0, color);
    ::UpdateColor2(this->ui->documentation_sdk1, color);
    ::UpdateColor2(this->ui->documentation_sdk2, color);
    ::UpdateColor2(this->ui->documentation_sdk3, color);
    ::UpdateColor1(this->ui->documentation_sdk4, color);
    ::UpdateColor3(this->ui->documentation_sdk5, color);

    ::UpdateColor4(this->ui->documentation_com0, color);
}

TabDocumentation::~TabDocumentation() {}

void TabDocumentation::UpdateUI(UpdateUIMode mode) { (void)mode; }

void TabDocumentation::CleanUI() {}

bool TabDocumentation::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}
