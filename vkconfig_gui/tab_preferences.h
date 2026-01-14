/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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

#include <QNetworkAccessManager>
#include <QNetworkReply>

class TabPreferences : public Tab {
    Q_OBJECT

   public:
    TabPreferences(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui);
    virtual ~TabPreferences();

    virtual void UpdateUI(UpdateUIMode mode) override;
    virtual void CleanUI() override;
    virtual bool EventFilter(QObject *target, QEvent *event) override;

   public Q_SLOTS:
    void on_keep_running_toggled(bool checked);
    void on_vk_home_text_pressed();
    void on_vk_home_browse_pressed();
    void on_all_enabled_executables_changed(int index);
    void on_vk_download_browse_pressed();
    void on_vk_download_open_pressed();
    void on_reset_hard_pressed();
    void on_layer_debug_mode_toggled(bool checked);
    void on_open_page_pressed();
    void on_download_pressed();
    void on_release_downloaded(QNetworkReply *pReply);
    void on_package_downloaded(QNetworkReply *pReply);
    void on_download_progress(qint64 bytesReceived, qint64 bytesTotal);
    void on_notify_releases_toggled(bool checked);
    void on_theme_mode_changed(int index);
    void on_theme_light_alternate_enabled(bool checked);
    void on_theme_dark_alternate_enabled(bool checked);
    void on_theme_light_alternate_pressed();
    void on_theme_dark_alternate_pressed();

   private:
    QNetworkAccessManager network_manager;
    QByteArray downloaded_data;
    bool initialized = false;
};
