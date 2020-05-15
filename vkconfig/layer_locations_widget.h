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

#include <QCheckBox>
#include <QGroupBox>
#include <QListWidget>
#include <QPushButton>

#include "layer_manifest.h"

class LayerLocationsWidget : public QGroupBox {
    Q_OBJECT

  public:
    LayerLocationsWidget(QWidget *parent = NULL);

    inline const QList<QPair<QString, LayerType>> &customLayerPaths() const { return custom_layer_locations; }

    inline void setCustomLayerPaths(const QList<QPair<QString, LayerType>> &paths) {
        custom_layer_locations = paths;
        emit pathsChanged(currentLayerPaths(), custom_path_box->isChecked());
    }

    inline void setUseCustomLayerPaths(bool enabled) { custom_path_box->setCheckState(enabled ? Qt::Checked : Qt::Unchecked); }

    inline bool useCustomLayerPaths() { return custom_path_box->isChecked(); }

  signals:
    void pathsChanged(const QList<QPair<QString, LayerType>> &path_list, bool is_custom);

  private slots:
    void addLayerPath();
    void clearLayerPaths();
    void removeSelectedLayerPath();
    void searchPath();
    void selectCustomLayerPaths(int state);

  private:
    void loadLayerPaths();

    inline QList<QPair<QString, LayerType>> &currentLayerPaths() {
        return custom_path_box->isChecked() ? custom_layer_locations : default_layer_locations;
    }

    QHash<LayerType, QIcon> layer_icons;

    QCheckBox *custom_path_box;
    QListWidget *path_list;
    QHash<QString, LayerType> path_types;
    QPushButton *search_path_button;
    QPushButton *add_path_button;
    QPushButton *remove_path_button;
    QPushButton *clear_paths_button;

    QList<QPair<QString, LayerType>> default_layer_locations;
    QList<QPair<QString, LayerType>> custom_layer_locations;
};
