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

#include "../vkconfig_core/util.h"

#include <QMessageBox>
#include <QTimer>

#include <cassert>

WidgetSettingFrames::WidgetSettingFrames(QTreeWidgetItem* item, const SettingMetaFrames& setting_meta,
                                         SettingDataFrames& setting_data)
    : setting_meta(setting_meta), setting_data(setting_data) {
    assert(item);
    assert(&setting_meta);
    assert(&setting_data);

    item->setText(0, setting_meta.label.c_str());
    item->setToolTip(0, setting_meta.description.c_str());
    this->setText(setting_data.value.c_str());

    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(itemEdited(const QString&)));
}

void WidgetSettingFrames::FieldEditedCheck() {
    if (!IsUIntRanges(setting_data.value)) {
        const std::string text = format("'%s' is an invalid value. Use list of comma separated integer ranges. Example: '0-2,16'.",
                                        this->text().toStdString().c_str());
        const std::string into = format("Resetting to the setting default value: '%s'.", this->setting_meta.default_value.c_str());

        this->setting_data.value = this->setting_meta.default_value;
        this->setText(this->setting_data.value.c_str());

        QMessageBox alert;
        alert.setWindowTitle(format("Invalid '%s' setting value", setting_meta.label.c_str()).c_str());
        alert.setText(text.c_str());
        alert.setInformativeText(into.c_str());
        alert.setStandardButtons(QMessageBox::Ok);
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
    }
}

void WidgetSettingFrames::itemEdited(const QString& value) {
    this->setting_data.value = value.toStdString();
    QTimer::singleShot(1000, [this]() { FieldEditedCheck(); });

    emit itemChanged();
}
