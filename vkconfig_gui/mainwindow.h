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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include "settings_tree.h"
#include "configuration_layer_widget.h"

#include "ui_mainwindow.h"

#include <QDialog>
#include <QMainWindow>
#include <QLabel>
#include <QRadioButton>
#include <QShowEvent>
#include <QResizeEvent>
#include <QProcess>
#include <QSystemTrayIcon>

#include <memory>
#include <string>

/// This just allows me to associate a specific profile definition
/// with a list widget item.
class ConfigurationListItem : public QTreeWidgetItem {
   public:
    ConfigurationListItem(const std::string &configuration_name) : configuration_name(configuration_name) {}
    std::string configuration_name;
    QRadioButton *radio_button;
    QPushButton *push_button_reset;
    QPushButton *push_button_duplicate;
    QPushButton *push_button_remove;

   private:
    ConfigurationListItem(const ConfigurationListItem &) = delete;
    ConfigurationListItem &operator=(const ConfigurationListItem &) = delete;
};

class LayerPathWidget : public QLabel {
    Q_OBJECT

   public:
    LayerPathWidget(const std::string &layer_path, QListWidget *list, QListWidgetItem *item) : item(item) {
        this->button_edit = new QPushButton(this);
        this->button_edit->setText("...");
        this->button_edit->show();
        this->buttom_remove = new QPushButton(this);
        this->buttom_remove->setText("x");
        this->buttom_remove->show();

        this->setText(layer_path.c_str());

        item->setSizeHint(QSize(0, ITEM_HEIGHT));
    }

    QPushButton *button_edit;
    QPushButton *buttom_remove;

   protected:
    void resizeEvent(QResizeEvent *event) override {
        QSize size = event->size();

        const QFontMetrics fm = this->button_edit->fontMetrics();
        const int button_width_state = 30;

        const QRect edit_button_rect =
            QRect(size.width() - button_width_state - button_width_state, 0, button_width_state, size.height());
        this->button_edit->setGeometry(edit_button_rect);

        const QRect remove_button_rect = QRect(size.width() - button_width_state, 0, button_width_state, size.height());
        this->buttom_remove->setGeometry(remove_button_rect);
    }

   public:
    QListWidgetItem *item;
};

class TreeWidgetItemParameter : public QListWidgetItem {
   public:
    TreeWidgetItemParameter(const char *layer_name) : widget(nullptr), layer_name(layer_name) { assert(layer_name != nullptr); }

    QWidget *widget;
    std::string layer_name;
};

enum Tool { TOOL_VULKAN_INFO, TOOL_VULKAN_INSTALL };

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void UpdateUI();

   private:
    SettingsTreeManager _settings_tree_manager;

    std::unique_ptr<QProcess> _launch_application;  // Keeps track of the monitored app
    QFile _log_file;                                // Log file for layer output

    void LoadConfigurationList();
    void SetupLauncherTree();

    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *target, QEvent *event) override;

    std::unique_ptr<QDialog> vk_info_dialog;
    std::unique_ptr<QDialog> vk_installation_dialog;

    void Log(const std::string &log);

    ConfigurationListItem *GetCheckedItem();

    QComboBox *_launcher_apps_combo;
    QLineEdit *_launcher_executable;
    QLineEdit *_launcher_arguments;
    QLineEdit *_launcher_working;
    QLineEdit *_launcher_log_file_edit;
    QPushButton *_launcher_apps_browse_button;
    QPushButton *_launcher_executable_browse_button;
    QPushButton *_launcher_working_browse_button;
    QPushButton *_launcher_log_file_browse_button;
    QSystemTrayIcon *_tray_icon;
    QMenu *_tray_icon_menu;
    QAction *_tray_restore_action;
    QAction *_tray_layers_controlled_by_applications;
    QAction *_tray_layers_controlled_by_configurator;
    QAction *_tray_layers_disabled_by_configurator;
    QAction *_tray_quit_action;

    void RemoveClicked(ConfigurationListItem *item);
    void ResetClicked(ConfigurationListItem *item);
    void RenameClicked(ConfigurationListItem *item);
    void NewClicked();
    void DuplicateClicked(ConfigurationListItem *item);
    void ExportClicked(ConfigurationListItem *item);
    void ImportClicked(ConfigurationListItem *item);

    void AddLayerPathItem(const std::string &layer_path);
    void AddLayerItem(Parameter &parameter);

   private slots:
    void trayActionRestore();
    void trayActionControlledByApplications(bool checked);
    void trayActionControlledByConfigurator(bool checked);
    void trayActionDisabledByApplications(bool checked);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

   public Q_SLOTS:
    void commitDataRequest(QSessionManager &manager);

    void toolsVulkanInfo(bool checked);
    void toolsVulkanInstallation(bool checked);
    void toolsResetToDefault(bool checked);

    void OnHelpFindLayers(bool checked);
    void OnHelpReadme(bool checked);
    void OnHelpChangelog(bool checked);
    void OnHelpVulkanSpec(bool checked);
    void OnHelpLayerSpec(bool checked);
    void OnHelpGPUInfo(bool checked);

    void editorExpanded(QTreeWidgetItem *item);

    void launchItemExpanded(QTreeWidgetItem *item);
    void launchItemCollapsed(QTreeWidgetItem *item);
    void launchItemChanged(int index);
    void launchSetLogFile();
    void launchSetExecutable();
    void launchSetWorkingFolder();
    void launchChangeLogFile(const QString &new_text);
    void launchChangeExecutable(const QString &new_text);
    void launchChangeWorkingFolder(const QString &new_text);
    void launchArgsEdited(const QString &new_text);

    void on_tab_widget_currentChanged(int index);

    void on_push_button_launcher_clicked();
    void on_push_button_clear_log_clicked();
    void on_check_box_clear_on_launch_clicked();
    void on_push_button_applications_clicked();
    void on_push_button_new_clicked();
    void on_push_button_rename_clicked();
    void on_push_button_remove_clicked();
    void on_push_button_duplicate_clicked();

    void on_check_box_per_application_toggled(bool checked);
    void on_combo_box_mode_currentIndexChanged(int index);
    void on_combo_box_applications_currentIndexChanged(int index);

    void OnConfigurationItemExpanded(QTreeWidgetItem *item);
    void OnConfigurationItemClicked(bool checked);
    void OnConfigurationTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void OnConfigurationItemChanged(QTreeWidgetItem *item, int column);
    void OnConfigurationTreeClicked(QTreeWidgetItem *item, int column);
    void OnSettingsTreeClicked(QTreeWidgetItem *item, int column);
    void OnLauncherLoaderMessageChanged(int level);

    void standardOutputAvailable();                                 // stdout output is available
    void errorOutputAvailable();                                    // Layeroutput is available
    void processClosed(int exitCode, QProcess::ExitStatus status);  // app died

   private:
    MainWindow(const MainWindow &) = delete;
    MainWindow &operator=(const MainWindow &) = delete;

    void InitTray();
    void UpdateTray();

    void RemoveConfiguration(const std::string &configuration_name);
    void ResetLaunchApplication();
    void StartTool(Tool tool);
    QStringList BuildEnvVariables() const;

    std::unique_ptr<Ui::MainWindow> ui;
};
