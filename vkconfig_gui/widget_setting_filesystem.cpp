/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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
#include "../vkconfig_core/configurator.h"

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
    this->field->show();
    this->connect(this->field, SIGNAL(textEdited(const QString&)), this, SLOT(textFieldChanged(const QString&)));

    this->button->setText("...");
    this->button->show();
    this->connect(this->button, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));

    this->item->setText(0, GetLabel(this->meta).c_str());
    this->item->setToolTip(0, meta.description.c_str());
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
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
        LoadPath(this->data().GetValue());

        this->field->blockSignals(true);
        this->field->setText(this->data().GetValue().RelativePath().c_str());
        this->field->setToolTip(this->data().GetValue().AbsolutePath().c_str());

        if (::CheckSettingOverridden(this->meta)) {
            this->DisplayOverride(this->field, this->meta);
        }

        this->field->blockSignals(false);
    }
}

void WidgetSettingFilesystem::resizeEvent(QResizeEvent* event) {
    const QSize parent_size = event->size();

    const QRect button_rect = QRect(parent_size.width() - MIN_BUTTON_SIZE, 0, MIN_BUTTON_SIZE, parent_size.height());
    this->button->setGeometry(button_rect);
}

void WidgetSettingFilesystem::LoadPath(const Path& path) {
    switch (this->meta.type) {
        case SETTING_LOAD_FILE: {
            const SettingMetaFileLoad& setting_meta = static_cast<const SettingMetaFileLoad&>(this->meta);
            if (setting_meta.format == "PROFILE") {
                if (path.Empty()) {
                    return;
                }

                SettingDataFileLoad& file_setting_data = static_cast<SettingDataFileLoad&>(this->data());
                file_setting_data.profile_names = CollectProfileNamesFromFile(path);

                SettingDataString* enum_setting_data = FindSetting<SettingDataString>(this->data_set, "profile_name");
                if (!file_setting_data.profile_names.empty() && enum_setting_data != nullptr) {
                    enum_setting_data->SetValue(file_setting_data.profile_names[0].c_str());
                } else {
                    enum_setting_data->SetValue("");
                }
            }
            break;
        }
        case SETTING_LOAD_FOLDER: {
            const SettingMetaFolderLoad& setting_meta = static_cast<const SettingMetaFolderLoad&>(this->meta);
            if (setting_meta.format == "PROFILE") {
                if (path.Empty()) {
                    return;
                }

                SettingDataFolderLoad& setting_data = static_cast<SettingDataFolderLoad&>(this->data());
                setting_data.profile_names = CollectProfileNamesFromDir(path);

                SettingDataString* enum_setting_data = FindSetting<SettingDataString>(this->data_set, "profile_name");
                if (enum_setting_data != nullptr) {
                    if (std::find(setting_data.profile_names.begin(), setting_data.profile_names.end(),
                                  enum_setting_data->GetValue()) == setting_data.profile_names.end()) {
                        enum_setting_data->SetValue("${VP_DEFAULT}");
                    }
                }
            }
            break;
        }
        default:
            break;
    }
}

void WidgetSettingFilesystem::browseButtonClicked() {
    Path path(this->field->text().toStdString());
    if (path.Empty()) {
        path = Path("${VK_HOME}");
        this->data().SetValue(path);
    }

    switch (this->meta.type) {
        case SETTING_LOAD_FILE:
        case SETTING_SAVE_FILE:
            if (!path.Exists()) {
                path = this->data().GetValue();
            }
            if (!path.Exists()) {
                path.Clear();
            }
            break;
        case SETTING_LOAD_FOLDER:
        case SETTING_SAVE_FOLDER:
            if (!path.Exists()) {
                path = this->data().GetValue();
            }
            if (!path.Exists()) {
                path.Clear();
            }
            break;
        default:
            assert(0);
            break;
    }

    const std::string replaced_path = path.AbsolutePath();
    const std::string dir = path.AbsoluteDir();

    const char* filter = this->meta.filter.c_str();
    Path new_path;

    switch (this->meta.type) {
        case SETTING_LOAD_FILE:
            new_path = Path(QFileDialog::getOpenFileName(this->button, "Select file", dir.c_str(), filter).toStdString());
            break;
        case SETTING_SAVE_FILE:
            new_path = Path(QFileDialog::getSaveFileName(this->button, "Select File", dir.c_str(), filter).toStdString());
            break;
        case SETTING_LOAD_FOLDER:
        case SETTING_SAVE_FOLDER:
            new_path = Path(QFileDialog::getExistingDirectory(this->button, "Select Folder", replaced_path.c_str()).toStdString());
            break;
        default:
            assert(0);
            break;
    }

    // The user has cancel the operation
    if (new_path.Empty()) {
        return;
    }

    // The path didn't change, preserve the built-in variables
    if (replaced_path != new_path.AbsolutePath().c_str()) {
        this->data().SetValue(new_path);
    }

    this->field->setText(this->data().GetValue().RelativePath().c_str());
    this->field->setToolTip(new_path.AbsolutePath().c_str());

    this->LoadPath(new_path);

    emit itemChanged();
}

void WidgetSettingFilesystem::textFieldChanged(const QString& value) {
    Path path(value.toStdString());

    this->data().SetValue(path.RelativePath().c_str());
    this->field->setToolTip(path.AbsolutePath().c_str());

    if (path.Exists()) {
        this->LoadPath(path);
    }

    emit itemChanged();
}

SettingDataFilesystem& WidgetSettingFilesystem::data() {
    SettingDataFilesystem* data = FindSetting<SettingDataFilesystem>(this->data_set, this->meta.key.c_str());
    assert(data != nullptr);
    return *data;
}
