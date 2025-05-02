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

#include "tab_preferences.h"
#include "style.h"
#include "mainwindow.h"

#include "../vkconfig_core/configurator.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QStyleHints>

TabPreferences::TabPreferences(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_DIAGNOSTIC, window, ui) {
    Configurator &configurator = Configurator::Get();

#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
    this->ui->preferences_theme_mode->setToolTip(
        "Vulkan Configurator must be build with Qt 6.8 or newer to support UI appearance control.");
#endif

    this->connect(this->ui->preferences_keep_running, SIGNAL(toggled(bool)), this, SLOT(on_keep_running_toggled(bool)));
    this->connect(this->ui->preferences_vk_home_text, SIGNAL(returnPressed()), this, SLOT(on_vk_home_text_pressed()));
    this->connect(this->ui->preferences_vk_home_browse, SIGNAL(clicked()), this, SLOT(on_vk_home_browse_pressed()));
    this->connect(this->ui->preferences_vk_download_browse, SIGNAL(clicked()), this, SLOT(on_vk_download_browse_pressed()));
    this->connect(this->ui->preferences_reset, SIGNAL(clicked()), this, SLOT(on_reset_hard_pressed()));
    this->connect(this->ui->preferences_layer_dev_mode, SIGNAL(toggled(bool)), this, SLOT(on_layer_dev_mode_toggled(bool)));
    this->connect(this->ui->preferences_open_page, SIGNAL(clicked()), this, SLOT(on_open_page_pressed()));
    this->connect(this->ui->preferences_notify_releases, SIGNAL(toggled(bool)), this, SLOT(on_notify_releases_toggled(bool)));
    this->connect(this->ui->preferences_download, SIGNAL(clicked()), this, SLOT(on_download_pressed()));
    this->connect(this->ui->preferences_theme_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(on_theme_mode_changed(int)));

    this->ui->preferences_progress->setVisible(false);
    this->ui->preferences_notify_releases->setChecked(configurator.GetUseNotifyReleases());
    this->ui->preferences_theme_mode->blockSignals(true);
    this->ui->preferences_theme_mode->setCurrentIndex(this->current_theme_mode);
    this->ui->preferences_theme_mode->blockSignals(false);
    this->ui->preferences_theme_mode->setEnabled(QT_VERSION >= QT_VERSION_CHECK(6, 8, 0));

    this->ui->preferences_appearance->setVisible(false);

    this->on_theme_mode_changed();

    QUrl url(GetLatestReleaseSDK(VKC_PLATFORM));
    QNetworkRequest request(url);
    this->network_manager.get(request);
    this->connect(&this->network_manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(on_release_downloaded(QNetworkReply *)));

    this->ui->preferences_download->setText("Searching Latest Vulkan SDK...");
}

TabPreferences::~TabPreferences() {}

void TabPreferences::UpdateUI(UpdateUIMode mode) {
    (void)mode;

    Configurator &configurator = Configurator::Get();

    this->ui->preferences_keep_running->blockSignals(true);
    this->ui->preferences_keep_running->setChecked(configurator.GetUseSystemTray());
    this->ui->preferences_keep_running->blockSignals(false);

    this->ui->preferences_vk_home_text->blockSignals(true);
    this->ui->preferences_vk_home_text->setText(::Path(Path::HOME).RelativePath().c_str());
    this->ui->preferences_vk_home_text->setToolTip(::Path(Path::HOME).AbsolutePath().c_str());
    this->ui->preferences_vk_home_text->blockSignals(false);

    this->ui->preferences_layer_dev_mode->blockSignals(true);
    this->ui->preferences_layer_dev_mode->setChecked(configurator.GetUseLayerDevMode());
    this->ui->preferences_layer_dev_mode->blockSignals(false);

    this->ui->preferences_vk_download_text->blockSignals(true);
    this->ui->preferences_vk_download_text->setText(::Path(Path::DOWNLOAD).RelativePath().c_str());
    this->ui->preferences_vk_download_text->setToolTip(::Path(Path::DOWNLOAD).AbsolutePath().c_str());
    this->ui->preferences_vk_download_text->blockSignals(false);
}

void TabPreferences::CleanUI() {}

bool TabPreferences::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}

void TabPreferences::on_theme_mode_changed() {
    const ThemeMode mew_theme_mode = ::IsDarkMode() ? THEME_MODE_FORCE_DARK : THEME_MODE_FORCE_LIGHT;
    if (this->current_theme_mode == mew_theme_mode) {
        return;
    }

    // Configurations
    this->ui->configurations_executable_append->setIcon(::Get(::ICON_FILE_SEARCH));
    this->ui->configurations_executable_remove->setIcon(::Get(::ICON_FILE_REMOVE));

    // Layers
    this->ui->layers_browse_button->setIcon(::Get(::ICON_FOLDER_SEARCH));
    this->ui->layers_reload_button->setIcon(::Get(::ICON_FOLDER_RELOAD));

    // Applications
    this->ui->launch_executable_search->setIcon(::Get(::ICON_FILE_SEARCH));
    this->ui->launch_executable_append->setIcon(::Get(::ICON_FILE_APPEND));
    this->ui->launch_executable_remove->setIcon(::Get(::ICON_FILE_REMOVE));
    this->ui->launch_options_append->setIcon(::Get(::ICON_OPTIONS_COPY));
    this->ui->launch_options_remove->setIcon(::Get(::ICON_OPTIONS_REMOVE));
    this->ui->launch_options_dir_button->setIcon(::Get(::ICON_FOLDER_SEARCH));
    this->ui->launch_options_log_button->setIcon(::Get(::ICON_FILE_SEARCH));
    this->ui->launch_options_log_open->setIcon(::Get(::ICON_FILE_EXPORT));

    // Diagnostics
    this->ui->diagnostic_export_folder->setIcon(::Get(::ICON_FOLDER_EXPORT));
    this->ui->diagnostic_export_file->setIcon(::Get(::ICON_FILE_EXPORT));
    this->ui->diagnostic_search_clear->setIcon(::Get(::ICON_EXIT));
    this->ui->diagnostic_search_next->setIcon(::Get(::ICON_NEXT));
    this->ui->diagnostic_search_prev->setIcon(::Get(::ICON_PREV));
    this->ui->diagnostic_search_case->setIcon(::Get(::ICON_SEARCH_CASE));
    this->ui->diagnostic_search_whole->setIcon(::Get(::ICON_SEARCH_WHOLE));
    this->ui->diagnostic_search_regex->setIcon(::Get(::ICON_SEARCH_REGEX));

    // Preferences
    this->ui->preferences_reset->setIcon(::Get(::ICON_RESET));
    this->ui->preferences_vk_home_browse->setIcon(::Get(::ICON_FOLDER_SEARCH));
    this->ui->preferences_vk_download_browse->setIcon(::Get(::ICON_FOLDER_SEARCH));

    this->current_theme_mode = mew_theme_mode;

    this->window.update();
/*
    #if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
        switch (mew_theme_mode) {
            default:
            case THEME_MODE_FORCE_LIGHT:
                //if (this->window.app.styleHints()->colorScheme() != Qt::ColorScheme::Light) {
                    this->window.app.styleHints()->setColorScheme(Qt::ColorScheme::Light);
                //}
                break;
            case THEME_MODE_FORCE_DARK:
                //if (this->window.app.styleHints()->colorScheme() != Qt::ColorScheme::Dark) {
                    this->window.app.styleHints()->setColorScheme(Qt::ColorScheme::Dark);
                //}
                break;
        }
    #endif
*/
}

void TabPreferences::on_keep_running_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    configurator.SetUseSystemTray(checked);
}

void TabPreferences::on_vk_home_text_pressed() {
    Path path(this->ui->preferences_vk_home_text->text().toStdString());
    if (path.Exists()) {
        ::SetHomePath(this->ui->preferences_vk_home_text->text().toStdString());
    } else {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setWindowTitle("Invalid ${VULKAN_HOME} path...");
        message.setText(
            format("'%s' is not a valid, it doesn't exist.", this->ui->preferences_vk_home_text->text().toStdString().c_str())
                .c_str());
        message.setInformativeText(format("Restoring the previous path '%s'.", ::Path(Path::HOME).AbsolutePath().c_str()).c_str());
        message.exec();

        this->ui->preferences_vk_home_text->setText(::Path(Path::HOME).AbsolutePath().c_str());
    }
}

void TabPreferences::on_vk_home_browse_pressed() {
    const QString selected_path = QFileDialog::getExistingDirectory(
        this->ui->preferences_vk_home_browse, "Select the Vulkan Home Default Working Folder (Set ${VULKAN_HOME} value)...",
        ::Path(Path::HOME).AbsolutePath().c_str());

    if (!selected_path.isEmpty()) {
        this->ui->preferences_vk_home_text->setText(selected_path);

        ::SetHomePath(selected_path.toStdString());
    }
}

void TabPreferences::on_vk_download_browse_pressed() {
    const QString selected_path = QFileDialog::getExistingDirectory(
        this->ui->preferences_vk_download_browse,
        "Select the Vulkan Configurator Download Default Folder (Set ${VULKAN_DOWNLOAD} value)...",
        ::Path(Path::DOWNLOAD).AbsolutePath().c_str());

    if (!selected_path.isEmpty()) {
        this->ui->preferences_vk_download_text->setText(selected_path);

        ::SetDownloadPath(selected_path.toStdString());
    }
}

void TabPreferences::on_reset_hard_pressed() {
    QMessageBox message;
    message.setIcon(QMessageBox::Critical);
    message.setWindowTitle(format("Restoring and Resetting %s to default...", VKCONFIG_NAME).c_str());
    message.setText(
        "You are about to delete all the user-defined configurations and resetting all default configurations to their default "
        "state.");
    message.setInformativeText("Do you want to continue?");
    message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    message.setDefaultButton(QMessageBox::No);
    if (message.exec() == QMessageBox::Yes) {
        this->disconnect(qApp, &QGuiApplication::commitDataRequest, nullptr, nullptr);

        Configurator::Get().Reset(true);
        qApp->quit();
    }
}

void TabPreferences::on_notify_releases_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    configurator.SetUseNotifyReleases(checked);
}

void TabPreferences::on_layer_dev_mode_toggled(bool checked) {
    Configurator &configurator = Configurator::Get();
    configurator.SetUseLayerDevMode(checked);
}

void TabPreferences::on_open_page_pressed() { QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/sdk/home")); }

void TabPreferences::on_download_pressed() {
    Configurator &configurator = Configurator::Get();
    if (configurator.latest_sdk_version != configurator.online_sdk_version && configurator.online_sdk_version != Version::NONE) {
        this->ui->preferences_download->setEnabled(false);
        this->ui->preferences_progress->setVisible(true);
        this->ui->preferences_vk_download_text->setVisible(false);
        this->ui->preferences_vk_download_browse->setVisible(false);

        this->connect(&this->network_manager, SIGNAL(finished(QNetworkReply *)), this,
                      SLOT(on_package_downloaded(QNetworkReply *)));

        QUrl url(GetLatestPackageSDK(VKC_PLATFORM));
        QNetworkRequest request(url);
        QNetworkReply *reply = this->network_manager.get(request);
        this->connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(on_download_progress(qint64, qint64)));

        this->ui->preferences_progress->setFormat((std::string(GetLatestPackageSDK(VKC_PLATFORM)) + " - %p%").c_str());
    } else {
        const Path &path_latest = ::AbsolutePath(Path::DOWNLOAD) + Path::Separator() + GetInstallerFilename(VKC_PLATFORM);
        QDesktopServices::openUrl(QUrl::fromLocalFile(path_latest.AbsoluteDir().c_str()));
    }
}

void TabPreferences::on_release_downloaded(QNetworkReply *pReply) {
    this->disconnect(&this->network_manager, SIGNAL(finished(QNetworkReply *)), 0, 0);

    Configurator &configurator = Configurator::Get();

    if (pReply->error() == QNetworkReply::NoError) {
        configurator.online_sdk_version = Version(pReply->readAll().toStdString());
    } else {
        configurator.online_sdk_version = Version::NONE;
    }

    pReply->deleteLater();

    if (configurator.ShouldNotify()) {
        this->ui->preferences_download->setText(
            format("Download Latest Vulkan SDK %s", configurator.online_sdk_version.str().c_str()).c_str());

        if (configurator.GetUseNotifyReleases()) {
            QMessageBox alert;
            alert.setWindowTitle("A new version of the Vulkan SDK is available");
            alert.setText(format("Do you want to download Vulkan SDK %s?", configurator.online_sdk_version.str().c_str()).c_str());
            alert.setIcon(QMessageBox::Question);
            alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            alert.setDefaultButton(QMessageBox::Yes);
            alert.setCheckBox(new QCheckBox("Do not show again for this version."));

            int ret_val = alert.exec();
            if (alert.checkBox()->isChecked()) {
                configurator.latest_sdk_version = configurator.online_sdk_version;
            }

            if (ret_val == QMessageBox::Yes) {
                this->ui->tab_widget->setCurrentIndex(TAB_PREFERENCES);
                this->on_download_pressed();
            }
        }
    } else {
        this->ui->preferences_download->setText("Open Vulkan SDK download directory...");
    }

    // Vulkan Configurator just got updated, the latest online sdk version is considered updated
    if (configurator.last_vkconfig_version < Version::VKCONFIG) {
        configurator.latest_sdk_version = configurator.online_sdk_version;
    }
}

void TabPreferences::on_package_downloaded(QNetworkReply *pReply) {
    this->ui->preferences_download->setEnabled(true);
    this->ui->preferences_progress->setVisible(false);
    this->ui->preferences_vk_download_text->setVisible(true);
    this->ui->preferences_vk_download_browse->setVisible(true);

    Configurator &configurator = Configurator::Get();
    configurator.latest_sdk_version = configurator.online_sdk_version;

    if (pReply->error() == QNetworkReply::NoError) {
        this->downloaded_data = pReply->readAll();
    } else {
        QMessageBox alert;
        alert.setWindowTitle("Error while downloading the latest Vulkan SDK...");
        alert.setText(
            format("Vulkan Configurator couldn't download Vulkan SDK %s package...", configurator.online_sdk_version.str().c_str())
                .c_str());
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
        return;
    }

    pReply->deleteLater();

    const Path &path_latest = ::AbsolutePath(Path::DOWNLOAD) + Path::Separator() + GetInstallerFilename(VKC_PLATFORM);
    QFile file_latest(path_latest.AbsolutePath().c_str());

    const bool result_latest = file_latest.open(QFile::WriteOnly);
    if (result_latest) {
        file_latest.write(this->downloaded_data);
        file_latest.close();
    }

    const Path &path_version = ::AbsolutePath(Path::DOWNLOAD) + Path::Separator() +
                               format(GetVersionedFilename(VKC_PLATFORM), configurator.latest_sdk_version.str().c_str());
    QFile file_version(path_version.AbsolutePath().c_str());

    const bool result_version = file_version.open(QFile::WriteOnly);
    if (result_latest) {
        file_version.write(this->downloaded_data);
        file_version.close();
    }

    if (!result_latest && !result_version) {
        QMessageBox alert;
        alert.setWindowTitle("Couldn't save downloaded Vulkan SDK installer...");
        alert.setIcon(QMessageBox::Critical);
        alert.exec();
    }

    this->ui->preferences_download->setText("Open Vulkan SDK download directory...");

    if (VKC_ENV == VKC_ENV_WIN32) {
        QMessageBox alert;
        alert.setWindowTitle("The new version of the Vulkan SDK is downloaded");
        alert.setText(
            format("Do you want to Launch Vulkan SDK %s installer?", configurator.online_sdk_version.str().c_str()).c_str());
        alert.setIcon(QMessageBox::Question);
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setDefaultButton(QMessageBox::Yes);

        int ret_val = alert.exec();
        if (ret_val == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(path_latest.AbsolutePath().c_str()));

            qApp->quit();
            return;
        }
    } else if (VKC_PLATFORM == PLATFORM_LINUX) {
        QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/sdk/latest/linux/getting_started.html"));
    } else if (VKC_PLATFORM == PLATFORM_MACOS) {
        QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/sdk/latest/mac/getting_started.html"));
    }

    QDesktopServices::openUrl(QUrl::fromLocalFile(path_latest.AbsoluteDir().c_str()));
}

void TabPreferences::on_download_progress(qint64 bytesReceived, qint64 bytesTotal) {
    this->ui->preferences_progress->setMaximum(bytesTotal);
    this->ui->preferences_progress->setValue(bytesReceived);
}
