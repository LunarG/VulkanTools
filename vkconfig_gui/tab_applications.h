/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

struct TabApplications : public Tab {
    Q_OBJECT

   public:
    TabApplications(MainWindow& window, std::shared_ptr<Ui::MainWindow> ui);
    virtual ~TabApplications();

    virtual void UpdateUI(UpdateUIMode mode) override;
    virtual void CleanUI() override;
    virtual bool EventFilter(QObject* target, QEvent* event) override;

   public Q_SLOTS:
    void on_applications_remove_application_pushButton_pressed();
    void on_applications_append_application_pushButton_pressed();
    void on_applications_list_comboBox_activated(int index);
    void on_applications_list_comboBox_textEdited(const QString& text);

    void on_applications_options_remove_pushButton_pressed();
    void on_applications_options_duplicate_pushButton_pressed();
    void on_applications_options_comboBox_activated(int index);
    void on_applications_options_comboBox_textEdited(const QString& text);

    void on_applications_layers_mode_comboBox_activated(int index);
    void on_applications_configuration_comboBox_activated(int index);

   private:
};
