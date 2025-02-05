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

class TabSettings : public Tab {
    Q_OBJECT

   public:
    TabSettings(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui);
    virtual ~TabSettings();

    virtual void UpdateUI(UpdateUIMode mode) override;
    virtual void CleanUI() override;
    virtual bool EventFilter(QObject *target, QEvent *event) override;

   public Q_SLOTS:
    void on_keep_running_toggled(bool checked);
    void on_vk_home_text_pressed();
    void on_vk_home_browse_pressed();
    void on_reset_hard_pressed();
    void on_layer_dev_mode_toggled(bool checked);

   private:
};
