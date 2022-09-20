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
#include <QFontMetrics>
#include <QCheckBox>
#include <QSettings>

#include <cassert>

static const int MIN_FIELD_WIDTH = 80;

WidgetSettingFrames::WidgetSettingFrames(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaFrames& meta,
                                         SettingDataSet& data_set)
    : WidgetSettingBase(tree, item),
      meta(meta),
      data_set(data_set),
      field(new QLineEdit(this)),
      timer_error(new QTimer(this)),
      timer_valid(new QTimer(this)) {
    assert(&meta);

    this->field->setFont(tree->font());
    this->field->setToolTip("Use list of comma separated integer ranges. Example: '0-2,16'.");
    this->field->setAlignment(Qt::AlignRight);
    this->field->show();
    this->default_palette = this->field->palette();

    this->connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(OnTextEdited(const QString&)));
    this->connect(this->timer_error, &QTimer::timeout, this, &WidgetSettingFrames::OnErrorValue);
    this->connect(this->timer_valid, &QTimer::timeout, this, &WidgetSettingFrames::OnValidValue);

    this->item->setText(0, GetLabel(this->meta).c_str());
    this->item->setFont(0, this->tree->font());
    this->item->setToolTip(0, meta.description.c_str());
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->item->setExpanded(this->meta.expanded);
    this->tree->setItemWidget(this->item, 0, this);

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

WidgetSettingFrames::~WidgetSettingFrames() {
    this->timer_error->stop();
    this->timer_valid->stop();
}

void WidgetSettingFrames::Refresh(RefreshAreas refresh_areas) {
    const SettingDependenceMode enabled = ::CheckDependence(this->meta, data_set);

    this->item->setHidden(enabled == SETTING_DEPENDENCE_HIDE);
    this->item->setDisabled(enabled != SETTING_DEPENDENCE_ENABLE);
    this->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);
    this->field->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);

    if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        this->field->blockSignals(true);
        this->field->setText(this->data().value.c_str());
        this->field->blockSignals(false);
    }
}

void WidgetSettingFrames::resizeEvent(QResizeEvent* event) {
    this->resize = event->size();
    this->Resize();
}

void WidgetSettingFrames::OnErrorValue() {
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(255, 192, 192));
    this->field->setPalette(palette);

    QSettings settings;
    if (settings.value("VKCONFIG_WIDGET_SETTING_FRAMES").toBool() == false) {
        const std::string text =
            format("The setting input '%s' is invalid. Use list of comma separated integer ranges. Example: '0-2,16'.",
                   this->data().value.c_str());
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
        if (alert.exec() == QMessageBox::Yes) {
            this->data().value = this->meta.default_value;
            this->field->setText(this->meta.default_value.c_str());
            this->field->setPalette(default_palette);
            this->Resize();
        }
        if (alert.checkBox()->isChecked()) {
            settings.setValue("VKCONFIG_WIDGET_SETTING_FRAMES", true);
        }
    }

    this->timer_error->stop();
}

void WidgetSettingFrames::OnValidValue() {
    emit itemChanged();

    this->timer_valid->stop();
}

void WidgetSettingFrames::Resize() {
    const QFontMetrics fm = this->field->fontMetrics();
    const int width = std::max(HorizontalAdvance(fm, this->field->text() + "00"), MIN_FIELD_WIDTH);

    const QRect button_rect = QRect(this->resize.width() - width, 0, width, this->resize.height());
    this->field->setGeometry(button_rect);
}

SettingInputError WidgetSettingFrames::ProcessInputValue() { return this->data().ProcessInput(this->value_buffer); }

void WidgetSettingFrames::OnTextEdited(const QString& new_value) {
    this->timer_error->stop();
    this->timer_valid->stop();

    this->value_buffer = new_value.toStdString();
    this->Resize();

    if (this->ProcessInputValue() == SETTING_INPUT_NO_ERROR) {
        this->field->setPalette(default_palette);
        this->timer_valid->start(500);
    } else {
        this->timer_error->start(3000);
    }

    emit itemChanged();
}

SettingDataFrames& WidgetSettingFrames::data() {
    SettingDataFrames* data = FindSetting<SettingDataFrames>(this->data_set, this->meta.key.c_str());
    assert(data != nullptr);
    return *data;
}
