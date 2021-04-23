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

WidgetSettingListElement::WidgetSettingListElement(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaList& meta,
                                                   SettingDataSet& data_set, EnabledNumberOrString& element)
    : WidgetSettingBase(tree, item),
      meta(meta),
      data(*data_set.Get<SettingDataList>(meta.key.c_str())),
      data_set(data_set),
      element(element),
      button(new QPushButton(this)),
      field(new QCheckBox(this)) {
    assert(&meta);
    assert(&data);

    const std::string text = element.key.empty() ? format("%d", element.number) : element.key;

    this->field->setText(text.c_str());
    this->field->setFont(this->tree->font());
    this->field->setToolTip(text.c_str());
    this->field->setCheckable(true);
    this->connect(this->field, SIGNAL(clicked(bool)), this, SLOT(OnElementChecked(bool)));

    this->button->setText("-");
    this->button->setFont(tree->font());
    this->button->show();
    this->connect(this->button, SIGNAL(clicked()), this, SLOT(OnElementRemoved()));

    this->tree->setItemWidget(this->item, 0, this);

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void WidgetSettingListElement::Refresh(RefreshAreas refresh_areas) {
    const bool enabled = ::CheckDependence(this->meta, data_set);

    this->item->setDisabled(!enabled);
    this->setEnabled(enabled);
    this->button->setEnabled(enabled);

    if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
        this->field->blockSignals(true);
        this->field->setChecked(std::find(this->data.value.begin(), this->data.value.end(), this->element)->enabled);
        this->field->blockSignals(false);
    }
}

void WidgetSettingListElement::resizeEvent(QResizeEvent* event) {
    const QSize parent_size = event->size();

    const QRect button_rect = QRect(parent_size.width() - MIN_BUTTON_SIZE, 0, MIN_BUTTON_SIZE, parent_size.height());
    this->button->setGeometry(button_rect);
}

void WidgetSettingListElement::OnElementChecked(bool checked) {
    this->element.enabled = checked;

    emit itemChanged();
}

void WidgetSettingListElement::OnElementRemoved() {
    auto it = std::find(this->data.value.begin(), this->data.value.end(), this->element);
    assert(it != this->data.value.end());

    this->data.value.erase(it);

    emit itemSelected(this->field->text());  // Remove the element from the parent WidgetSettingList
    emit itemChanged();                      // Notify the setting tree that settings have changed
}
