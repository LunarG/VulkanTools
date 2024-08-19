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

#include "mainwindow.h"

#include "dialog_vulkan_analysis.h"
#include "dialog_vulkan_info.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/alert.h"
#include "../vkconfig_core/util.h"
#include "../vkconfig_core/ui.h"
#include "../vkconfig_core/version.h"
#include "../vkconfig_core/help.h"
#include "../vkconfig_core/date.h"
#include "../vkconfig_core/type_platform.h"
#include "../vkconfig_core/type_log.h"
#include "../vkconfig_core/type_hide_message.h"

#include <QMenu>
#include <QProcess>
#include <QMessageBox>
#include <QFrame>
#include <QComboBox>
#include <QVariant>
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QFileDialog>
#include <QDesktopServices>
#include <QSettings>

#include <cassert>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      _launch_application(nullptr),
      _tray_icon(nullptr),
      _tray_icon_menu(nullptr),
      _tray_restore_action(nullptr),
      _tray_layers_controlled_by_applications(nullptr),
      _tray_layers_controlled_by_configurator(nullptr),
      _tray_layers_disabled_by_configurator(nullptr),
      _tray_quit_action(nullptr),
      ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->application_list->installEventFilter(this);

    this->InitTray();

    this->tabs[TAB_DIAGNOSTIC].reset(new TabDiagnostics(*this, ui));
    this->tabs[TAB_APPLICATIONS].reset(new TabApplications(*this, ui));
    this->tabs[TAB_LAYERS].reset(new TabLayers(*this, ui));
    this->tabs[TAB_CONFIGURATIONS].reset(new TabConfigurations(*this, ui));
    this->tabs[TAB_PREFERENCES].reset(new TabPreferences(*this, ui));
    this->tabs[TAB_HELP].reset(new TabHelp(*this, ui));

    connect(ui->action_find_more_layers, SIGNAL(triggered(bool)), this, SLOT(OnHelpFindLayers(bool)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(OnHelpAbout(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui->action_readme, SIGNAL(triggered(bool)), this, SLOT(OnHelpReadme(bool)));
    connect(ui->action_changelog, SIGNAL(triggered(bool)), this, SLOT(OnHelpChangelog(bool)));
    connect(ui->actionVulkan_specification, SIGNAL(triggered(bool)), this, SLOT(OnHelpVulkanSpec(bool)));
    connect(ui->actionVulkan_Layer_Specification, SIGNAL(triggered(bool)), this, SLOT(OnHelpLayerSpec(bool)));
    connect(ui->actionGPU_Info_Reports, SIGNAL(triggered(bool)), this, SLOT(OnHelpGPUInfo(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionRestore_Default_Configurations, SIGNAL(triggered(bool)), this, SLOT(toolsResetToDefault(bool)));

    // connect(ui->launcher_loader_debug, SIGNAL(currentIndexChanged(int)), this, SLOT(OnLauncherLoaderMessageChanged(int)));

    // connect(_launcher_apps_browse_button, SIGNAL(clicked()), this, SLOT(on_push_button_applications_clicked()));

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    ui->log_browser->document()->setMaximumBlockCount(2048);

    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    this->restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    this->restoreState(settings.value("mainwindow/state").toByteArray());

    this->tabs[this->ui->tab_widget->currentIndex()]->UpdateUI(UPDATE_REBUILD_UI);

    this->UpdateUI_Status();
    this->UpdateUI();
}

MainWindow::~MainWindow() { this->ResetLaunchApplication(); }

void MainWindow::InitTray() {
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        this->_tray_quit_action = new QAction("&Quit", this);
        connect(this->_tray_quit_action, &QAction::triggered, qApp, &QCoreApplication::quit);
        this->_tray_restore_action = new QAction("Open &Vulkan Configurator", this);
        connect(this->_tray_restore_action, &QAction::triggered, this, &MainWindow::trayActionRestore);
        this->_tray_layers_controlled_by_applications = new QAction("Layers Controlled by the Vulkan Applications", this);
        this->_tray_layers_controlled_by_applications->setCheckable(true);
        connect(this->_tray_layers_controlled_by_applications, &QAction::toggled, this,
                &MainWindow::trayActionControlledByApplications);
        this->_tray_layers_controlled_by_configurator = new QAction("Layers Controlled by the Vulkan Configurator", this);
        this->_tray_layers_controlled_by_configurator->setCheckable(true);
        connect(this->_tray_layers_controlled_by_configurator, &QAction::toggled, this,
                &MainWindow::trayActionControlledByConfigurator);
        this->_tray_layers_disabled_by_configurator = new QAction("Layers Disabled by the Vulkan Configurator", this);
        this->_tray_layers_disabled_by_configurator->setCheckable(true);
        connect(this->_tray_layers_disabled_by_configurator, &QAction::toggled, this,
                &MainWindow::trayActionDisabledByApplications);
        this->_tray_icon_menu = new QMenu(this);
        this->_tray_icon_menu->addAction(this->_tray_restore_action);
        this->_tray_icon_menu->addSeparator();
        this->_tray_icon_menu->addAction(this->_tray_layers_controlled_by_applications);
        this->_tray_icon_menu->addAction(this->_tray_layers_controlled_by_configurator);
        this->_tray_icon_menu->addAction(this->_tray_layers_disabled_by_configurator);
        this->_tray_icon_menu->addSeparator();
        this->_tray_icon_menu->addAction(this->_tray_quit_action);
        this->_tray_icon = new QSystemTrayIcon(this);
        this->_tray_icon->setContextMenu(this->_tray_icon_menu);
        this->UpdateUI_Status();
        this->_tray_icon->show();
        this->connect(this->_tray_icon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
    }
}

void MainWindow::UpdateUI_Status() {
    const Configurator &configurator = Configurator::Get();
    const Environment &environment = configurator.environment;
    const std::string &selected_contiguration_name = environment.GetActiveConfigurationInfo().GetName();
    const bool has_selected_configuration = !selected_contiguration_name.empty();
    const bool has_active_configuration = configurator.HasActiveConfiguration() && has_selected_configuration;

    // Update title bar
#ifdef VKCONFIG_DATE
    bool display_date = true;
#else
    bool display_date = false;
#endif

    this->setWindowTitle(GetMainWindowTitle(has_active_configuration, display_date).c_str());
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        switch (environment.GetActiveConfigurationInfo().GetMode()) {
            default:
            case LAYERS_CONTROLLED_BY_APPLICATIONS:
                this->_tray_layers_controlled_by_applications->setChecked(true);
                this->_tray_layers_controlled_by_configurator->setChecked(false);
                this->_tray_layers_disabled_by_configurator->setChecked(false);
                break;
            case LAYERS_CONTROLLED_BY_CONFIGURATOR:
                this->_tray_layers_controlled_by_applications->setChecked(false);
                this->_tray_layers_controlled_by_configurator->setChecked(true);
                this->_tray_layers_disabled_by_configurator->setChecked(false);
                break;
            case LAYERS_DISABLED_BY_CONFIGURATOR:
                this->_tray_layers_controlled_by_applications->setChecked(false);
                this->_tray_layers_controlled_by_configurator->setChecked(false);
                this->_tray_layers_disabled_by_configurator->setChecked(true);
                break;
        }
    }

    if (has_active_configuration && environment.GetActiveConfigurationInfo().GetMode() != LAYERS_CONTROLLED_BY_APPLICATIONS) {
        const QIcon icon(":/resourcefiles/vkconfig-on.png");
        this->setWindowIcon(icon);
        if (QSystemTrayIcon::isSystemTrayAvailable()) {
            this->_tray_icon->setIcon(icon);
            this->_tray_icon->setToolTip("Layers controlled by the Vulkan Configurator");
        }
    } else {
        const QIcon icon(":/resourcefiles/vkconfig-off.png");
        this->setWindowIcon(icon);
        if (QSystemTrayIcon::isSystemTrayAvailable()) {
            this->_tray_icon->setIcon(icon);
            this->_tray_icon->setToolTip("Layers controlled by the Vulkan Applications");
        }
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        default:
            break;
        case QSystemTrayIcon::Context:
            break;
        case QSystemTrayIcon::DoubleClick:
            Qt::WindowStates window_states = this->windowState();
            const bool is_minimized = this->isMinimized();
            const bool is_visible = this->isVisible();
            const bool is_hidden = this->isHidden();
            if (this->isMinimized() || this->isHidden()) {
                this->setVisible(true);
                this->showNormal();
                this->setWindowState(Qt::WindowState::WindowActive);
            } else {
                this->hide();
            }

            this->tabs[this->ui->tab_widget->currentIndex()]->UpdateUI(UPDATE_REBUILD_UI);
            this->UpdateUI_Status();
            break;
    }
}

void MainWindow::trayActionRestore() {
    this->hide();
    this->showNormal();
    this->UpdateUI();
    this->tabs[this->ui->tab_widget->currentIndex()]->UpdateUI(UPDATE_REBUILD_UI);
    this->UpdateUI_Status();
}

void MainWindow::trayActionControlledByApplications(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.environment.GetActiveConfigurationInfo().SetMode(LAYERS_CONTROLLED_BY_APPLICATIONS);
        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

        this->UpdateUI();
        if (this->tabs[this->ui->tab_widget->currentIndex()] != nullptr) {
            this->tabs[this->ui->tab_widget->currentIndex()]->UpdateUI(UPDATE_REBUILD_UI);
        }
        this->UpdateUI_Status();
    }
}

void MainWindow::trayActionControlledByConfigurator(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.environment.GetActiveConfigurationInfo().SetMode(LAYERS_CONTROLLED_BY_CONFIGURATOR);
        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

        this->UpdateUI();
        if (this->tabs[this->ui->tab_widget->currentIndex()] != nullptr) {
            this->tabs[this->ui->tab_widget->currentIndex()]->UpdateUI(UPDATE_REBUILD_UI);
        }
        this->UpdateUI_Status();
    }
}

void MainWindow::trayActionDisabledByApplications(bool checked) {
    if (checked) {
        Configurator &configurator = Configurator::Get();
        configurator.environment.GetActiveConfigurationInfo().SetMode(LAYERS_DISABLED_BY_CONFIGURATOR);
        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);

        this->UpdateUI();
        if (this->tabs[this->ui->tab_widget->currentIndex()] != nullptr) {
            this->tabs[this->ui->tab_widget->currentIndex()]->UpdateUI(UPDATE_REBUILD_UI);
        }
        this->UpdateUI_Status();
    }
}

void MainWindow::AddLayerPathItem(const std::string &layer_path) {
    TreeWidgetItemParameter *item_state = new TreeWidgetItemParameter(layer_path.c_str());

    item_state->setFlags(item_state->flags() | Qt::ItemIsSelectable);
    LayerPathWidget *layer_path_widget = new LayerPathWidget(layer_path, ui->layers_tree, item_state);

    item_state->widget = layer_path_widget;

    // Add the top level item
    ui->layers_tree->addTopLevelItem(item_state);
    ui->layers_tree->setItemWidget(item_state, 0, layer_path_widget);
}

void MainWindow::UpdateUI() {
    static int check_recurse = 0;
    ++check_recurse;
    assert(check_recurse <= 1);

    Configurator &configurator = Configurator::Get();
    const Environment &environment = Configurator::Get().environment;
    const std::string &selected_contiguration_name = environment.GetActiveConfigurationInfo().GetName();
    const bool has_selected_configuration = !selected_contiguration_name.empty();

    this->blockSignals(true);
    ui->configurations_list->blockSignals(true);

    const bool has_active_configuration = configurator.HasActiveConfiguration() && has_selected_configuration;

    const bool has_application_list = !environment.GetApplications().empty();
    ui->push_button_launcher->setText(_launch_application ? "Terminate" : "Launch");
    ui->check_box_clear_on_launch->setChecked(environment.launcher_clear_on_launch);

    // Mode states
    // this->UpdateUI_Status();

    ui->configurations_list->blockSignals(false);
    this->blockSignals(false);

    --check_recurse;
}

void MainWindow::on_check_box_clear_on_launch_clicked() {
    Configurator::Get().environment.launcher_clear_on_launch = ui->check_box_clear_on_launch->isChecked();
}

void MainWindow::toolsResetToDefault(bool checked) {
    (void)checked;

    if (Alert::ConfiguratorResetAll() == QMessageBox::No) {
        return;
    }

    Configurator &configurator = Configurator::Get();
    configurator.ResetToDefault(true);

    this->tabs[this->ui->tab_widget->currentIndex()]->UpdateUI(UPDATE_REFRESH_UI);
    this->UpdateUI();
}

void MainWindow::StartTool(Tool tool) {
    Configurator &configurator = Configurator::Get();

    if (!configurator.environment.GetPerApplicationConfig()) {
        configurator.Surrender(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
    }

    switch (tool) {
        case TOOL_VULKAN_INFO:
            vk_info_dialog.reset(new VulkanInfoDialog(this));
            break;
        case TOOL_VULKAN_INSTALL:
            vk_installation_dialog.reset(new VulkanAnalysisDialog(this));
            break;
    }

    if (!configurator.environment.GetPerApplicationConfig()) {
        configurator.Override(OVERRIDE_AREA_LOADER_SETTINGS_BIT);
    }
}

/// Create the VulkanInfo dialog if it doesn't already exits & show it.
void MainWindow::toolsVulkanInfo(bool checked) {
    (void)checked;
    this->StartTool(TOOL_VULKAN_INFO);
}

/// Create the VulkanTools dialog if it doesn't already exist & show it.
void MainWindow::toolsVulkanInstallation(bool checked) {
    (void)checked;
    this->StartTool(TOOL_VULKAN_INSTALL);
}

void MainWindow::OnHelpFindLayers(bool checked) {
    (void)checked;
    ShowDoc(DOC_FIND_LAYERS);
}

void MainWindow::OnHelpReadme(bool checked) {
    (void)checked;
    ShowDoc(DOC_VKCONFIG_README);
}

void MainWindow::OnHelpChangelog(bool checked) {
    (void)checked;
    ShowDoc(DOC_VKCONFIG_CHANGELOG);
}

void MainWindow::OnHelpVulkanSpec(bool checked) {
    (void)checked;
    ShowDoc(DOC_VULKAN_SPEC);
}

void MainWindow::OnHelpLayerSpec(bool checked) {
    (void)checked;
    ShowDoc(DOC_VULKAN_LAYERS);
}

void MainWindow::OnHelpGPUInfo(bool checked) {
    (void)checked;
    ShowDoc(DOC_GPU_INFO);
}

/// The only thing we need to do here is clear the configuration if
/// the user does not want it active.
void MainWindow::closeEvent(QCloseEvent *event) {
    Environment &environment = Configurator::Get().environment;

    // Alert the user to the current state of the vulkan configurator and
    // give them the option to not shutdown.
    if (environment.GetUseSystemTray()) {
        if (environment.hide_message_boxes_flags & GetBit(HIDE_MESSAGE_USE_SYSTEM_TRAY)) {
            std::string shut_down_state;

            if (environment.GetActiveConfigurationInfo().GetMode() == LAYERS_CONTROLLED_BY_CONFIGURATOR) {
                shut_down_state =
                    "Vulkan Layers override will remain in effect while Vulkan Configurator remain active in the system tray.";
            } else {
                shut_down_state =
                    "No Vulkan layers override will be active when Vulkan Configurator remain active in the system tray.";
            }

            QMessageBox alert(this);
            alert.setWindowTitle("Vulkan Configurator will remain in the system tray");
            alert.setText(shut_down_state.c_str());
            alert.setIcon(QMessageBox::Question);
            alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            alert.setCheckBox(new QCheckBox("Do not show again."));
            alert.setInformativeText("Are you still ready to move Vulkan Configurator in the system tray?");

            int ret_val = alert.exec();
            if (alert.checkBox()->isChecked()) {
                environment.hide_message_boxes_flags |= GetBit(HIDE_MESSAGE_USE_SYSTEM_TRAY);
            }

            if (ret_val == QMessageBox::No) {
                event->ignore();
                return;
            }
        }
    }

    // If a child process is still running, destroy it
    if (_launch_application) {
        ResetLaunchApplication();
    }

    this->tabs[this->ui->tab_widget->currentIndex()]->CleanUI();

    QSettings settings("LunarG", VKCONFIG_SHORT_NAME);
    settings.setValue("mainwindow/geometry", this->saveGeometry());
    settings.setValue("mainwindow/state", this->saveState());

    QMainWindow::closeEvent(event);
}
/*
/// Resizing needs a little help. Yes please, there has to be
/// a better way of doing this.
void MainWindow::resizeEvent(QResizeEvent *event) {
    if (event != nullptr) {
        event->accept();
    }

    const QFontMetrics fm = ui->layers_list->fontMetrics();
    const int combo_width = (fm.size(Qt::TextSingleLine, "Auto").width() * 1.6);
    const int width = ui->layers_list->width() - combo_width;

    // ui->tree_layers_list->setColumnWidth(0, width);
}
*/
void MainWindow::showEvent(QShowEvent *event) {
    (void)event;

    this->UpdateUI();

    event->accept();
}

// Clear the browser window
void MainWindow::on_push_button_clear_log_clicked() {
    ui->log_browser->clear();
    ui->log_browser->update();
    ui->push_button_clear_log->setEnabled(false);
}

bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    if (this->tabs[this->ui->tab_widget->currentIndex()] == nullptr) {
        return false;
    }

    return this->tabs[this->ui->tab_widget->currentIndex()]->EventFilter(target, event);
}

void MainWindow::ResetLaunchApplication() {
    if (_launch_application) {
        _launch_application->kill();
        _launch_application->waitForFinished();
        _launch_application.reset();

        UpdateUI();
    }
}

QStringList MainWindow::BuildEnvVariables() const {
    Configurator &configurator = Configurator::Get();

    QStringList env = QProcess::systemEnvironment();
    env << (QString("VK_LOADER_DEBUG=") + ::GetLogString(configurator.environment.GetLoaderMessageFlags()).c_str());
    return env;
}

void MainWindow::on_tab_widget_currentChanged(int index) { this->tabs[index]->UpdateUI(UPDATE_REBUILD_UI); }

void MainWindow::on_push_button_launcher_clicked() {
    // Are we already monitoring a running app? If so, terminate it
    if (_launch_application != nullptr) {
        ResetLaunchApplication();
        return;
    }

    // We are logging, let's add that we've launched a new application
    std::string launch_log = "Launching Vulkan Application:\n";

    Configurator &configurator = Configurator::Get();
    const Application &active_application = configurator.environment.GetActiveApplication();

    assert(!active_application.executable_path.Empty());
    launch_log += format("- Executable: %s\n", active_application.executable_path.AbsolutePath().c_str());
    if (!active_application.executable_path.Exists()) {
        Alert::PathInvalid(
            active_application.executable_path,
            format("The '%s' application will fail to launch.", active_application.executable_path.AbsolutePath().c_str()).c_str());
    }

    const ApplicationOptions &options = active_application.GetActiveOptions();

    launch_log += format("- Working Directory: %s\n", options.working_folder.AbsolutePath().c_str());
    if (!options.working_folder.Exists()) {
        Alert::PathInvalid(
            options.working_folder,
            format("The '%s' application will fail to launch.", active_application.executable_path.AbsolutePath().c_str()).c_str());
    }
    /*
    if (!_launcher_arguments->text().isEmpty()) {
        launch_log += format("- Command-line Arguments: %s\n", _launcher_arguments->text().toStdString().c_str());
    }
    */
    if (!options.log_file.Empty()) {
        launch_log += format("- Log file: %s\n", options.log_file.AbsolutePath().c_str());
    }

    if (!options.log_file.Empty()) {
        // Start logging
        // Make sure the log file is not already opened. This can occur if the
        // launched application is closed from the applicaiton.
        if (!_log_file.isOpen()) {
            _log_file.setFileName(options.log_file.AbsolutePath().c_str());

            // Open and append, or open and truncate?
            QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
            if (!ui->check_box_clear_on_launch->isChecked()) mode |= QIODevice::Append;

            if (!_log_file.open(mode)) {
                Alert::LogFileFailed();
            }
        }
    }

    if (ui->check_box_clear_on_launch->isChecked()) ui->log_browser->clear();
    Log(launch_log.c_str());

    // Launch the test application
    _launch_application.reset(new QProcess(this));
    connect(_launch_application.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));
    connect(_launch_application.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
    connect(_launch_application.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(processClosed(int, QProcess::ExitStatus)));

    //_launch_application->setProgram(ui->edit_executable->text());
    //_launch_application->setWorkingDirectory(ui->edit_dir->text());
    //_launch_application->setEnvironment(BuildEnvVariables() + ui->edit_env->text().split(","));

    if (!options.arguments.empty()) {
        const QStringList args = ConvertString(options.arguments);
        _launch_application->setArguments(args);
    }

    _launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    _launch_application->setProcessChannelMode(QProcess::MergedChannels);
    _launch_application->closeWriteChannel();

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!_launch_application->waitForStarted(4000)) {
        _launch_application->deleteLater();
        _launch_application = nullptr;

        const std::string failed_log =
            std::string("Failed to launch ") + active_application.executable_path.AbsolutePath().c_str() + "!\n";
        Log(failed_log);
    }

    UpdateUI();
}

/// The process we are following is closed. We don't actually care about the
/// exit status/code, we just need to know to destroy the QProcess object
/// and set it back to nullptr so that we know we can launch a new app.
/// Also, if we are logging, it's time to close the log file.
void MainWindow::processClosed(int exit_code, QProcess::ExitStatus status) {
    (void)exit_code;
    (void)status;

    assert(_launch_application);

    disconnect(_launch_application.get(), SIGNAL(finished(int, QProcess::ExitStatus)), this,
               SLOT(processClosed(int, QProcess::ExitStatus)));
    disconnect(_launch_application.get(), SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));
    disconnect(_launch_application.get(), SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));

    Log("Process terminated");

    if (_log_file.isOpen()) {
        _log_file.close();
    }

    ResetLaunchApplication();
}

/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void MainWindow::standardOutputAvailable() {
    if (_launch_application) {
        Log(_launch_application->readAllStandardOutput().toStdString());
    }
}

void MainWindow::errorOutputAvailable() {
    if (_launch_application) {
        Log(_launch_application->readAllStandardError().toStdString());
    }
}

void MainWindow::Log(const std::string &log) {
    ui->log_browser->setPlainText(ui->log_browser->toPlainText() + "\n" + log.c_str());
    ui->push_button_clear_log->setEnabled(true);

    if (_log_file.isOpen()) {
        _log_file.write(log.c_str(), log.size());
        _log_file.flush();
    }
}
