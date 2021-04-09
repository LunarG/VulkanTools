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
    : tree(tree),
      item(item),
      meta(meta),
      data(*data_set.Get<SettingDataList>(meta.key.c_str())),
      data_set(data_set),
      element(element),
      button(new QPushButton(this)) {
    assert(&meta);
    assert(&data);

    const std::string text = element.key.empty() ? format("%d", element.number) : element.key;

    this->setText(text.c_str());
    this->setFont(tree->font());
    this->setToolTip(text.c_str());
    this->setCheckable(true);
    this->setChecked(this->GetChecked());

    this->connect(this, SIGNAL(clicked(bool)), this, SLOT(OnItemChecked(bool)));

    this->button->setText("-");
    this->button->setFont(tree->font());
    this->button->show();
    this->connect(this->button, SIGNAL(clicked()), this, SLOT(OnButtonClicked()));

    tree->setItemWidget(item, 0, this);
}

void WidgetSettingListElement::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);

    const bool enabled = ::CheckDependence(this->meta, data_set);

    this->setEnabled(enabled);
    this->button->setEnabled(enabled);
}

void WidgetSettingListElement::resizeEvent(QResizeEvent* event) {
    const QSize parent_size = event->size();

    const QRect button_rect = QRect(parent_size.width() - MIN_BUTTON_SIZE, 0, MIN_BUTTON_SIZE, parent_size.height());
    this->button->setGeometry(button_rect);
}

void WidgetSettingListElement::OnItemChecked(bool checked) {
    this->element.enabled = checked;

    emit itemChanged();
}

void WidgetSettingListElement::OnButtonClicked() {
    auto it = std::find(this->data.value.begin(), this->data.value.end(), this->element);
    assert(it != this->data.value.end());

    this->data.value.erase(it);

    this->setEnabled(false);

    emit itemSelected(this->text());
}

bool WidgetSettingListElement::GetChecked() const {
    auto it = std::find(this->data.value.begin(), this->data.value.end(), this->element);
    assert(it != this->data.value.end());

    return it->enabled;
}
