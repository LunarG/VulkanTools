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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_setting_filesystem.h"
#include "widget_setting.h"

#include "../vkconfig_core/path.h"
#include "../vkconfig_core/alert.h"
#include "../vkconfig/configurator.h"

#include <QFileDialog>

#include <cassert>

WidgetSettingFilesystem::WidgetSettingFilesystem(QTreeWidget* tree, QTreeWidgetItem* item, const SettingMetaFilesystem& meta,
                                                 SettingDataSet& data_set)
    : WidgetSettingBase(tree, item),
      meta(meta),
      data_set(data_set),
      item_child(new QTreeWidgetItem()),
      field(new QLineEdit(this)),
      button(new QPushButton(this)) {
    assert(&meta);

    this->field->show();
    this->connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(textFieldChanged(const QString&)));

    this->button->setText("...");
    this->button->show();
    this->connect(this->button, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));

    this->item->setText(0, GetLabel(this->meta).c_str());
    this->item->setToolTip(0, meta.description.c_str());
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->item->setExpanded(this->meta.expanded);
    this->item->addChild(item_child);
    tree->setItemWidget(this->item, 0, this);

    this->item_child->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->item_child->setExpanded(true);
    tree->setItemWidget(this->item_child, 0, this->field);

    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void WidgetSettingFilesystem::Refresh(RefreshAreas refresh_areas) {
    const SettingDependenceMode enabled = ::CheckDependence(this->meta, data_set);

    this->item->setHidden(enabled == SETTING_DEPENDENCE_HIDE);
    this->item->setDisabled(enabled != SETTING_DEPENDENCE_ENABLE);
    this->item_child->setDisabled(enabled != SETTING_DEPENDENCE_ENABLE);
    this->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);
    this->field->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);
    this->button->setEnabled(enabled == SETTING_DEPENDENCE_ENABLE);

    if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
        LoadFile(this->data().value);

        this->field->blockSignals(true);
        this->field->setText(ReplaceBuiltInVariable(this->data().value).c_str());
        this->field->blockSignals(false);

        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        } else {
            this->field->setToolTip(this->field->text());
        }
    }
}

void WidgetSettingFilesystem::resizeEvent(QResizeEvent* event) {
    const QSize parent_size = event->size();

    const QRect button_rect = QRect(parent_size.width() - MIN_BUTTON_SIZE, 0, MIN_BUTTON_SIZE, parent_size.height());
    this->button->setGeometry(button_rect);
}

void WidgetSettingFilesystem::LoadFile(const std::string& path) {
    if (this->meta.type == SETTING_LOAD_FILE) {
        const SettingMetaFileLoad& setting_file = static_cast<const SettingMetaFileLoad&>(this->meta);
        if (setting_file.format == "PROFILE") {
            if (path.empty()) return;

            SettingDataFileLoad& file_setting_data = static_cast<SettingDataFileLoad&>(this->data());
            file_setting_data.profile_names = GetProfileNames(path);

            SettingDataString* enum_setting_data = FindSetting<SettingDataString>(this->data_set, "profile_name");
            if (!file_setting_data.profile_names.empty() && enum_setting_data != nullptr) {
                enum_setting_data->value = file_setting_data.profile_names[0];
            }
        }
    }
}

void WidgetSettingFilesystem::browseButtonClicked() {
    std::string file;

    const char* filter = this->meta.filter.c_str();
    const std::string value = this->data().value;

    const std::string path = ReplaceBuiltInVariable(value.empty() ? "${VK_LOCAL}" : value.c_str());

    switch (this->meta.type) {
        case SETTING_LOAD_FILE:
            file = QFileDialog::getOpenFileName(this->button, "Select file", path.c_str(), filter).toStdString();
            break;
        case SETTING_SAVE_FILE:
            file = QFileDialog::getSaveFileName(this->button, "Select File", path.c_str(), filter).toStdString();
            break;
        case SETTING_SAVE_FOLDER:
            file = QFileDialog::getExistingDirectory(this->button, "Select Folder", path.c_str()).toStdString();
            break;
        default:
            assert(0);
            break;
    }

    if (!file.empty()) {
        file = ConvertNativeSeparators(file);
        LoadFile(file);

        this->data().value = file;

        field->setText(this->data().value.c_str());

        emit itemChanged();
    }
}

void WidgetSettingFilesystem::textFieldChanged(const QString& value) {
    this->data().value = value.toStdString();
    this->field->setToolTip(this->field->text());

    emit itemChanged();
}

SettingDataString& WidgetSettingFilesystem::data() {
    SettingDataString* data = FindSetting<SettingDataString>(this->data_set, this->meta.key.c_str());
    assert(data != nullptr);
    return *data;
}
