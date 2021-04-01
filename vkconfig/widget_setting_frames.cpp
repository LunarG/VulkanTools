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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_setting_frames.h"
#include "widget_setting.h"

#include "../vkconfig_core/util.h"

#include <QMessageBox>
#include <QTimer>
#include <QFontMetrics>
#include <QCheckBox>
#include <QSettings>

#include <cassert>

static const int MIN_FIELD_WIDTH = 48;

WidgetSettingFrames::WidgetSettingFrames(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaFrames& meta,
                                         SettingDataFrames& data)
    : meta(meta), data(data), field(new QLineEdit(this)), timer(new QTimer(this)) {
    assert(tree);
    assert(item);
    assert(&meta);
    assert(&data);

    item->setText(0, meta.label.c_str());
    item->setFont(0, tree->font());
    item->setToolTip(0, meta.description.c_str());
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    this->field->setText(data.value.c_str());
    this->field->setFont(tree->font());
    this->field->setToolTip("Use list of comma separated integer ranges. Example: '0-2,16'.");
    this->field->setAlignment(Qt::AlignRight);
    this->field->show();
    this->default_palette = this->field->palette();

    this->connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));
    this->connect(timer, &QTimer::timeout, this, &WidgetSettingFrames::OnInvalidValue);

    tree->setItemWidget(item, 0, this);
}

WidgetSettingFrames::~WidgetSettingFrames() {
    if (!IsFrames(this->data.value)) {
        this->data.value = this->meta.default_value;
    }
}

void WidgetSettingFrames::Enable(bool enable) { this->field->setEnabled(enable); }

void WidgetSettingFrames::OnInvalidValue() {
    if (!IsFrames(this->data.value)) {
        QSettings settings;
        if (settings.value("VKCONFIG_WIDGET_SETTING_FRAMES").toBool() == false) {
            const std::string text =
                format("The setting input '%s' is invalid. Use list of comma separated integer ranges. Example: '0-2,16'.",
                       this->data.value.c_str());
            const std::string info =
                format("Do you want to reset to the setting default value? '%s'", this->meta.default_value.c_str());

            QMessageBox alert;
            alert.setWindowTitle(format("Invalid '%s' setting value", meta.label.c_str()).c_str());
            alert.setText(text.c_str());
            alert.setInformativeText(info.c_str());
            alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            alert.setDefaultButton(QMessageBox::Yes);
            alert.setIcon(QMessageBox::Critical);
            alert.setCheckBox(new QCheckBox("Do not show again."));
            if (alert.exec() == QMessageBox::No) {
                QPalette palette;
                palette.setColor(QPalette::Base, Qt::red);
                this->field->setPalette(palette);
            } else {
                this->data.value = this->meta.default_value;
                this->field->setText(data.value.c_str());
                this->Resize();
            }
            if (alert.checkBox()->isChecked()) {
                settings.setValue("VKCONFIG_WIDGET_SETTING_FRAMES", true);
            }
        } else {
            QPalette palette;
            palette.setColor(QPalette::Base, Qt::red);
            this->field->setPalette(palette);
        }
    } else {
        this->field->setPalette(default_palette);
    }

    this->timer->stop();
}

void WidgetSettingFrames::Resize() {
    const QFontMetrics fm = this->field->fontMetrics();
    const int width = std::max(HorizontalAdvance(fm, this->field->text() + "00"), MIN_FIELD_WIDTH);

    const QRect button_rect = QRect(this->resize.width() - width, 0, width, this->resize.height());
    this->field->setGeometry(button_rect);
}

void WidgetSettingFrames::resizeEvent(QResizeEvent* event) {
    this->resize = event->size();
    this->Resize();
}

void WidgetSettingFrames::OnTextEdited(const QString& value) {
    this->data.value = value.toStdString().c_str();
    this->Resize();

    if (!IsFrames(this->data.value)) {
        if (this->timer->isActive()) {
            this->timer->stop();
        }
        this->timer->start(3000);
    } else {
        this->field->setPalette(default_palette);
    }

    emit itemChanged();
}
