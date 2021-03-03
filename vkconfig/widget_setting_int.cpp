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

#include "../vkconfig_core/util.h"

#include <QMessageBox>
#include <QTimer>
#include <QFontMetrics>

#include <cassert>

static const int MIN_FIELD_WIDTH = 48;

WidgetSettingInt::WidgetSettingInt(QTreeWidgetItem* item, const SettingMetaInt& setting_meta, SettingDataInt& setting_data)
    : setting_meta(setting_meta), setting_data(setting_data) {
    assert(item);
    assert(&setting_meta);
    assert(&setting_data);

    item->setText(0, setting_meta.label.c_str());
    item->setToolTip(0, setting_meta.description.c_str());

    this->field = new QLineEdit(this);
    this->field->setText(format("%d", setting_data.value).c_str());
    this->field->setAlignment(Qt::AlignRight);
    this->field->show();

    connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(itemEdited(const QString&)));
}

void WidgetSettingInt::FieldEditedCheck() {
    if (this->field == nullptr) return;

    if (setting_data.value < setting_meta.min_value || setting_data.value > setting_meta.max_value) {
        const std::string text =
            format("'%s' is out of range. Use a value in the [%d-%d].", this->field->text().toStdString().c_str(),
                   this->setting_meta.min_value, this->setting_meta.max_value);
        const std::string into = format("Resetting to the setting default value: '%d'.", this->setting_meta.default_value);

        this->setting_data.value = this->setting_meta.default_value;
        this->field->setText(format("%d", this->setting_data.value).c_str());

        QMessageBox alert;
        alert.setWindowTitle(format("Invalid '%s' setting value", setting_meta.label.c_str()).c_str());
        alert.setText(text.c_str());
        alert.setInformativeText(into.c_str());
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
    }
}

void WidgetSettingInt::Resize() {
    const QFontMetrics fm = this->field->fontMetrics();
    const int width = std::max(fm.horizontalAdvance(this->field->text()) + fm.horizontalAdvance("00"), MIN_FIELD_WIDTH);

    const QRect button_rect = QRect(this->resize.width() - width, 0, width, this->resize.height());
    this->field->setGeometry(button_rect);
}

void WidgetSettingInt::resizeEvent(QResizeEvent* event) {
    this->resize = event->size();

    if (this->field == nullptr) return;

    this->Resize();
}

void WidgetSettingInt::itemEdited(const QString& number) {
    if (number.isEmpty()) {
        this->setting_data.value = this->setting_meta.default_value;
        this->field->setText(format("%d", setting_data.value).c_str());
    } else if (!IsNumber(number.toStdString())) {
        this->setting_data.value = this->setting_meta.default_value;
        this->field->setText(format("%d", setting_data.value).c_str());

        QMessageBox alert;
        alert.setWindowTitle(format("Invalid '%s' setting value", setting_meta.label.c_str()).c_str());
        alert.setText("The setting input value is not a number. Please use digits [1-9] only.");
        alert.setInformativeText(format("Resetting to the setting default value: '%d'.", this->setting_meta.default_value).c_str());
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
    } else {
        this->setting_data.value = std::atoi(number.toStdString().c_str());
    }

    QTimer::singleShot(1000, [this]() { FieldEditedCheck(); });
    this->Resize();

    emit itemChanged();
}
