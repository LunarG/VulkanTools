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

#include "widget_setting_list_element.h"
#include "widget_setting.h"

#include "../vkconfig_core/layer.h"
#include "../vkconfig_core/util.h"

#include <cassert>

WidgetSettingListElement::WidgetSettingListElement(QTreeWidget* tree, const SettingMetaList& meta, SettingDataList& data,
                                                   const std::string& element)
    : meta(meta), data(data), element(element), button(new QPushButton(this)) {
    assert(&meta);
    assert(&data);
    assert(!element.empty());

    this->setText(element.c_str());
    this->setFont(tree->font());
    this->setToolTip(element.c_str());
    this->setCheckable(true);
    this->setChecked(this->GetChecked());

    this->connect(this, SIGNAL(clicked(bool)), this, SLOT(OnItemChecked(bool)));

    this->button->setText("-");
    this->button->setFont(tree->font());
    this->button->show();
    this->connect(this->button, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));
}

void WidgetSettingListElement::resizeEvent(QResizeEvent* event) {
    const QSize parent_size = event->size();

    const QRect button_rect = QRect(parent_size.width() - MIN_BUTTON_SIZE, 0, MIN_BUTTON_SIZE, parent_size.height());
    this->button->setGeometry(button_rect);
}

void WidgetSettingListElement::OnItemChecked(bool checked) {
    EnabledString* enabled_string = FindByKey(data.values, this->element.c_str());
    enabled_string->enabled = checked;

    emit itemChanged();
}

void WidgetSettingListElement::OnButtonClicked() {
    EnabledString value;
    value.key = this->element;
    value.enabled = true;

    auto it = std::find(this->data.values.begin(), this->data.values.end(), value);
    assert(it != this->data.values.end());

    if (it != this->data.values.end()) {
        this->data.values.erase(it);
    }

    this->setEnabled(false);

    emit itemSelected(this->element.c_str());
}

bool WidgetSettingListElement::GetChecked() const {
    EnabledString value;
    value.key = this->element;
    value.enabled = true;

    auto it = std::find(this->data.values.begin(), this->data.values.end(), value);
    assert(it != this->data.values.end());

    return it->enabled;
}
