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
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "tab.h"

class TabLayers : public Tab {
    Q_OBJECT

   public:
    TabLayers(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui);
    virtual ~TabLayers();

    virtual void UpdateUI(UpdateUIMode mode) override;
    virtual void CleanUI() override;
    virtual bool EventFilter(QObject *target, QEvent *event) override;

    void UpdateUI_LayersPaths(UpdateUIMode ui_update_mode);

   public Q_SLOTS:
    void on_paths_changed();
    void on_paths_toggled();
    void on_layers_validate_checkBox_toggled(bool checked);
    void on_layers_append_pressed();
    void on_layers_browse_pressed();
    void on_layers_reload_pressed();

   private:
    void LoadLayersManifest(const QString &selected_path);

    std::string new_path;
};
