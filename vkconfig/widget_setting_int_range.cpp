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

#include "widget_setting_int_range.h"

#include "../vkconfig_core/util.h"

#include <QMessageBox>
#include <QTimer>
#include <QFontMetrics>

#include <cassert>

static const int MIN_FIELD_WIDTH = 48;

WidgetSettingIntRange::WidgetSettingIntRange(QTreeWidget* tree, QTreeWidgetItem* parent, const SettingMetaIntRanges& setting_meta,
                                             SettingDataIntRanges& setting_data)
    : WidgetSetting(tree, parent, setting_meta),
      setting_meta(setting_meta),
      setting_data(setting_data),
      field(new QLineEdit(this)) {
    assert(&setting_data);

    this->field->setText(setting_data.value.c_str());
    this->field->setAlignment(Qt::AlignRight);
    this->field->show();

    connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(itemEdited(const QString&)));
}

void WidgetSettingIntRange::FieldEditedCheck() {
    if (this->field == nullptr) return;

    if (!IsUIntRanges(setting_data.value)) {
        const std::string text = format("'%s' is an invalid value. Use list of comma separated integer ranges. Example: '0-2,16'.",
                                        this->field->text().toStdString().c_str());
        const std::string into = format("Resetting to the setting default value: '%s'.", this->setting_meta.default_value.c_str());

        this->setting_data.value = this->setting_meta.default_value;
        this->field->setText(this->setting_data.value.c_str());

        QMessageBox alert;
        alert.setWindowTitle(format("Invalid '%s' setting value", setting_meta.label.c_str()).c_str());
        alert.setText(text.c_str());
        alert.setInformativeText(into.c_str());
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
    }
}

void WidgetSettingIntRange::Resize() {
    const QFontMetrics fm = this->field->fontMetrics();
    const int width = std::max(fm.horizontalAdvance(this->field->text()) + fm.horizontalAdvance("00"), MIN_FIELD_WIDTH);

    const QRect button_rect = QRect(this->resize.width() - width, 0, width, this->resize.height());
    this->field->setGeometry(button_rect);
}

void WidgetSettingIntRange::resizeEvent(QResizeEvent* event) {
    this->resize = event->size();
    this->Resize();
}

void WidgetSettingIntRange::itemEdited(const QString& numbers) {
    this->setting_data.value = numbers.toStdString();
    QTimer::singleShot(1000, [this]() { FieldEditedCheck(); });

    this->Resize();

    emit itemChanged();
}
