#ifndef CMUTEMESSAGEWIDGET_H
#define CMUTEMESSAGEWIDGET_H
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
 * This class creates a list box that displays all the filtered
 * message ids.
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>

#include <layerfile.h>

class CMuteMessageWidget : public QWidget {
    Q_OBJECT
   public:
    explicit CMuteMessageWidget(TLayerSettings *pLayerSetting);

   private:
    TLayerSettings *pSetting;
    QListWidget *pListWidget;
    QPushButton *pAddButton;
    QPushButton *pRemovebutton;

    void resizeEvent(QResizeEvent *event) override;

   public Q_SLOTS:
    void addItem(QString &item);  // Added from combo box
    void removePushed(void);      // Remove button

   Q_SIGNALS:
     void itemChanged(void);
};

#endif  // CMUTEMESSAGEWIDGET_H
