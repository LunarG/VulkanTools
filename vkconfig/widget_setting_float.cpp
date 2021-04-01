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

#include "widget_setting_float.h"
#include "widget_setting.h"

#include <QMessageBox>
#include <QFontMetrics>
#include <QCheckBox>
#include <QSettings>

#include <cassert>

static const int MIN_FIELD_WIDTH = 80;

WidgetSettingFloat::WidgetSettingFloat(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaFloat& meta,
                                       SettingDataFloat& data)
    : meta(meta), data(data), field(new QLineEdit(this)), timer(new QTimer(this)) {
    assert(tree != nullptr);
    assert(item != nullptr);
    assert(&meta);
    assert(&data);

    const std::string unit = meta.unit.empty() ? "" : format(" (%s)", meta.unit.c_str());
    const std::string float_format = meta.GetFloatFormat();

    item->setText(0, (meta.label + unit).c_str());
    item->setFont(0, tree->font());
    item->setToolTip(0, meta.description.c_str());
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    this->field->setText(format(float_format.c_str(), data.value).c_str());
    this->field->setFont(tree->font());
    this->field->setToolTip(format("[%f, %f]", meta.min_value, meta.max_value).c_str());
    this->field->setAlignment(Qt::AlignRight);
    this->field->show();
    this->default_palette = this->field->palette();

    this->connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));
    this->connect(this->timer, &QTimer::timeout, this, &WidgetSettingFloat::OnInvalidValue);

    tree->setItemWidget(item, 0, this);
}

WidgetSettingFloat::~WidgetSettingFloat() {
    if (this->meta.IsValid(this->data)) {
        this->data.value = this->meta.default_value;
    }
}

void WidgetSettingFloat::Enable(bool enable) { this->field->setEnabled(enable); }

void WidgetSettingFloat::OnInvalidValue() {
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(255, 192, 192));
    this->field->setPalette(palette);

    QSettings settings;
    if (settings.value("VKCONFIG_WIDGET_SETTING_FLOAT").toBool() == false) {
        const std::string float_format = this->meta.GetFloatFormat();
        const std::string info = format("Do you want to reset to the setting default value? '%s'", float_format.c_str());
        const std::string range = this->meta.HasRange()
                                      ? format("Enter a number in the range [%s, %s].", float_format.c_str(), float_format.c_str())
                                      : std::string("Enter a floating point number.");

        std::string text;
        if (this->value.empty()) {
            text = format(("'%s' value is empty. " + range).c_str(), this->meta.label.c_str(), this->meta.min_value,
                          this->meta.max_value);
        } else if (!IsFloat(this->value)) {
            text = format(("'%s' value has invalid characters. " + range).c_str(), this->meta.label.c_str(), this->meta.min_value,
                          this->meta.max_value);
        } else if (!this->meta.IsValid(this->data)) {
            text = format(("'%s' value is out of range. " + range).c_str(), this->meta.label.c_str(), this->meta.min_value,
                          this->meta.max_value);
        }

        QMessageBox alert;
        alert.setWindowTitle(format("Invalid '%s' setting value", meta.label.c_str()).c_str());
        alert.setText(text.c_str());
        alert.setInformativeText(format(info.c_str(), this->meta.default_value).c_str());
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setDefaultButton(QMessageBox::Yes);
        alert.setIcon(QMessageBox::Critical);
        alert.setCheckBox(new QCheckBox("Do not show again."));
        if (alert.exec() == QMessageBox::Yes) {
            const std::string field_value = format(this->meta.GetFloatFormat().c_str(), this->meta.default_value);

            this->data.value = this->meta.default_value;
            this->field->setText(field_value.c_str());
            this->field->setPalette(default_palette);
            this->Resize();
        }
        if (alert.checkBox()->isChecked()) {
            settings.setValue("VKCONFIG_WIDGET_SETTING_FLOAT", true);
        }
    }

    this->timer->stop();
}

void WidgetSettingFloat::Resize() {
    const QFontMetrics fm = this->field->fontMetrics();
    const int width = std::max(HorizontalAdvance(fm, this->field->text() + "00"), MIN_FIELD_WIDTH);

    const QRect button_rect = QRect(this->resize.width() - width, 0, width, this->resize.height());
    this->field->setGeometry(button_rect);
}

void WidgetSettingFloat::resizeEvent(QResizeEvent* event) {
    this->resize = event->size();
    this->Resize();
}

bool WidgetSettingFloat::ValidateInputValue() {
    if (this->value.empty())
        return false;
    else if (IsFloat(this->value)) {
        this->data.value = std::atof(this->value.c_str());
        if (!this->meta.IsValid(this->data)) return false;
    } else {
        return false;
    }

    return true;
}

void WidgetSettingFloat::OnTextEdited(const QString& new_value) {
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
