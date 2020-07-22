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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include <QWidget>
#include <QCheckBox>
#include <QTreeWidgetItem>
#include <QString>

#include "layerfile.h"

class MultiEnumSetting : public QCheckBox {
    Q_OBJECT
   public:
    MultiEnumSetting(LayerSettings *layer_settings, QString thiss_setting);

   private:
    LayerSettings *_layer_settings;
    QString _my_setting;

   public Q_SLOTS:
    void itemChecked(bool bChecked);

   Q_SIGNALS:
    void itemChanged();
};
