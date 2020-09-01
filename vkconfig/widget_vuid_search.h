/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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
 * - Richard S. Wright Jr.
 * - Christophe Riccio
 */

#pragma once

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QResizeEvent>
#include <QCompleter>
#include <QStringList>
#include <QLineEdit>
#include <QPushButton>

class VUIDSearchWidget : public QWidget {
    Q_OBJECT
   public:
    explicit VUIDSearchWidget(const QString &valuesAlreadyPresent);

   private:
    QStringList _vuid_list;
    QCompleter *_search_vuid;
    QLineEdit *_user_box;
    QPushButton *_add_button;

    void ResetCompleter();

    virtual void resizeEvent(QResizeEvent *event) override;
    virtual bool eventFilter(QObject *target, QEvent *event) override;

   public Q_SLOTS:
    void addButtonPressed();
    void addCompleted(const QString &addedItem);
    void addToSearchList(const QString &newItem);

   Q_SIGNALS:
    void itemSelected(const QString &textSelected);
    void itemChanged();

   private:
    VUIDSearchWidget(const VUIDSearchWidget &) = delete;
    VUIDSearchWidget &operator=(const VUIDSearchWidget &) = delete;
};
