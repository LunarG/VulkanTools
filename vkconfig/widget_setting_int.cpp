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

#include "widget_setting_int.h"
#include "widget_setting.h"

#include <QMessageBox>
#include <QFontMetrics>
#include <QCheckBox>
#include <QSettings>

#include <cassert>

static const int MIN_FIELD_WIDTH = 80;

WidgetSettingInt::WidgetSettingInt(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaInt& meta, SettingDataInt& data)
    : meta(meta), data(data), field(new QLineEdit(this)), timer(new QTimer(this)) {
    assert(tree != nullptr);
    assert(item != nullptr);
    assert(&meta);
    assert(&data);

    const std::string unit = meta.unit.empty() ? "" : format(" (%s)", meta.unit.c_str());

    item->setText(0, (meta.label + unit).c_str());
    item->setFont(0, tree->font());
    item->setToolTip(0, meta.description.c_str());
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    this->field->setText(format("%d", data.value).c_str());
    this->field->setFont(tree->font());
    this->field->setToolTip(format("[%d, %d]", meta.min_value, meta.max_value).c_str());
    this->field->setAlignment(Qt::AlignRight);
    this->field->show();
    this->default_palette = this->field->palette();

    this->connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));
    this->connect(this->timer, &QTimer::timeout, this, &WidgetSettingInt::OnInvalidValue);

    tree->setItemWidget(item, 0, this);
}

WidgetSettingInt::~WidgetSettingInt() {
    if (this->meta.IsValid(this->data)) {
        this->data.value = this->meta.default_value;
    }
}

void WidgetSettingInt::Enable(bool enable) { this->field->setEnabled(enable); }

void WidgetSettingInt::OnInvalidValue() {
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(255, 192, 192));
    this->field->setPalette(palette);

    QSettings settings;
    if (settings.value("VKCONFIG_WIDGET_SETTING_INT").toBool() == false) {
        const std::string info = format("Do you want to reset to the setting default value? '%d'", this->meta.default_value);
        const std::string range = format("Enter a number in the range [%d, %d].", this->meta.min_value, this->meta.max_value);

        std::string text;
        if (this->value.empty()) {
            text = format("'%s' value is empty. %s", meta.label.c_str(), range.c_str());
        } else if (!IsNumber(this->value)) {
            text = format("'%s' value has invalid characters. %s", meta.label.c_str(), range.c_str());
        } else if (!this->meta.IsValid(this->data)) {
            text = format("'%s' value is out of range. %s", meta.label.c_str(), range.c_str());
        }

        QMessageBox alert;
        alert.setWindowTitle(format("Invalid '%s' setting value", meta.label.c_str()).c_str());
        alert.setText(text.c_str());
        alert.setInformativeText(info.c_str());
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setDefaultButton(QMessageBox::Yes);
        alert.setIcon(QMessageBox::Critical);
        alert.setCheckBox(new QCheckBox("Do not show again."));
        if (alert.exec() == QMessageBox::Yes) {
            this->data.value = this->meta.default_value;
            this->field->setText(format("%d", this->data.value).c_str());
            this->field->setPalette(default_palette);
            this->Resize();
        }
        if (alert.checkBox()->isChecked()) {
            settings.setValue("VKCONFIG_WIDGET_SETTING_INT", true);
        }
    }

    this->timer->stop();
}

void WidgetSettingInt::Resize() {
    const QFontMetrics fm = this->field->fontMetrics();
    const int width = std::max(HorizontalAdvance(fm, this->field->text() + "00"), MIN_FIELD_WIDTH);

    const QRect button_rect = QRect(this->resize.width() - width, 0, width, this->resize.height());
    this->field->setGeometry(button_rect);
}

void WidgetSettingInt::resizeEvent(QResizeEvent* event) {
    this->resize = event->size();
    this->Resize();
}

bool WidgetSettingInt::ValidateInputValue() {
    if (this->value.empty())
        return false;
    else if (IsNumber(this->value)) {
        this->data.value = std::atoi(this->value.c_str());
        if (!this->meta.IsValid(this->data)) return false;
    } else {
        return false;
    }

    return true;
}

void WidgetSettingInt::OnTextEdited(const QString& new_value) {
    this->value = new_value.toStdString();
    this->Resize();

    this->timer->stop();

    // Process the input value, notify an error is invalid, give time for the user to correct it
    if (!this->ValidateInputValue()) {
        this->timer->start(1000);
    } else {
        this->field->setPalette(default_palette);
    }

    emit itemChanged();
}
