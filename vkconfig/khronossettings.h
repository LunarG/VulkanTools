#ifndef KHRONOSSETTINGS_H
#define KHRONOSSETTINGS_H

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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 *
 * Khronos layer specific common settings dialog.
 *
 */

#include <QWidget>
#include <layerfile.h>


namespace Ui {
class KhronosSettings;
}

class KhronosSettings : public QWidget
{
    Q_OBJECT

public:
    explicit KhronosSettings(QWidget *parent,  QVector<TLayerSettings *>& layerSettings, QString qsText);
    ~KhronosSettings();

    bool CollectSettings(void);
private:
    Ui::KhronosSettings *ui;

    QVector<TLayerSettings *> linkedSetting;
    bool                      bGPU;


 public Q_SLOTS:

    void on_pushButtonBrowse_clicked(void);
    void on_radioButtonDebug_clicked(void);
    void on_radioButtonGPU_clicked(void);

};

#endif // KHRONOSSETTINGS_H
