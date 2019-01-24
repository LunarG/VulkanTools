/*
 * Copyright (c) 2018 Valve Corporation
 * Copyright (c) 2018 LunarG, Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 */

#pragma once

#include <QComboBox>
#include <QGroupBox>
#include <QListWidget>
#include <QSpinBox>
#include <QTime>

#include "layer_manifest.h"

enum class DurationUnit
{
    Minute,
    Hour,
    Day,
    None,
};

class ActiveLayersWidget : public QGroupBox
{
    Q_OBJECT

public:
    ActiveLayersWidget(QWidget *parent = NULL);
    inline const QList<LayerManifest>& enabledLayers() const { return enabled_layers; }
    int expiration() const;
    DurationUnit expirationUnit() const;
    void setEnabledLayers(const QList<QString> &layers);
    void setExpiration(int seconds, DurationUnit unit);

signals:
    void enabledLayersUpdated(const QList<LayerManifest> &enabled_layers, const QList<LayerManifest> &disabled_layers);

public slots:
    void clearLayers();
    void setExpirationEnabled(const QString &text);
    void updateAvailableLayers(const QList<QPair<QString, LayerType>> &path_list, bool is_custom);

private slots:
    void enableSelectedExplicitLayer();
    void enableSelectedImplicitLayer();
    void moveSelectedLayerDown();
    void moveSelectedLayerUp();
    void refreshAvailableLayers();
    void removeSelectedLayer();

private:
    QHash<LayerType, QIcon> layer_icons;
    QList<QPair<QString, LayerType>> layer_paths;
    bool custom_paths;

    QSpinBox *expiration_spin;
    QComboBox *expiration_units;

    QListWidget *enabled_layer_list;
    QHash<LayerType, QListWidget*> layer_lists;

    QList<LayerManifest> enabled_layers;
    QList<LayerManifest> disabled_layers;
};
