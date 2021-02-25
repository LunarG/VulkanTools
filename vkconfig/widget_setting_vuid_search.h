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

#pragma once

#include <QWidget>
#include <QResizeEvent>
#include <QStringList>
#include <QCompleter>
#include <QLineEdit>
#include <QPushButton>

class WidgetSettingVUIDSearch : public QWidget {
    Q_OBJECT

   public:
    explicit WidgetSettingVUIDSearch(const std::vector<std::string> &layer_vuids, const std::vector<std::string> &selected_vuids);

   public Q_SLOTS:
    void addButtonPressed();
    void addCompleted(const QString &addedItem);
    void addToSearchList(const QString &newItem);

   Q_SIGNALS:
    void itemSelected(const QString &textSelected);
    void itemChanged();

   private:
    WidgetSettingVUIDSearch(const WidgetSettingVUIDSearch &) = delete;
    WidgetSettingVUIDSearch &operator=(const WidgetSettingVUIDSearch &) = delete;

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual bool eventFilter(QObject *target, QEvent *event) override;

    void ResetCompleter();

    std::vector<std::string> layer_vuids;
    QCompleter *_search_vuid;
    QLineEdit *_user_box;
    QPushButton *_add_button;
};
