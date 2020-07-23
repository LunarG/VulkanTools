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

#include "mainwindow.h"
#include "dlgabout.h"
#include "dlgvulkananalysis.h"
#include "dlgvulkaninfo.h"
#include "dlgprofileeditor.h"
#include "dlgcreateassociation.h"
#include "dlgcustompaths.h"
#include "configurator.h"
#include "preferences.h"

#include "ui_mainwindow.h"

#include <QProcess>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QFrame>
#include <QComboBox>
#include <QVariant>
#include <QContextMenuEvent>
#include <QFileDialog>
#include <QLineEdit>
#include <QDesktopServices>

#ifndef _WIN32
#include <unistd.h>
#endif

// This is what happens when programmers can touch type....
bool been_warned_about_old_loader = false;

#define EDITOR_CAPTION_EMPTY "Configuration Layer Settings"

static const int LAUNCH_COLUMN0_SIZE = 220;
static const int LAUNCH_COLUMN2_SIZE = 32;
static const int LAUNCH_SPACING_SIZE = 2;
#ifdef __APPLE__
static const int LAUNCH_ROW_HEIGHT = 26;
#else
static const int LAUNCH_ROW_HEIGHT = 28;
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->launchTree->installEventFilter(this);
    ui->profileTree->installEventFilter(this);

    _selected_configuration_item = nullptr;
    _vk_via = nullptr;
    _vk_info = nullptr;
    _help = nullptr;
    _launch_application = nullptr;
    _log_file = nullptr;
    _launcher_apps_combo = nullptr;
    _launch_arguments = nullptr;

    ///////////////////////////////////////////////
    Configurator &configurator = Configurator::Get();

    if (!configurator.VK_LAYER_PATH.isEmpty())
        ui->groupBoxProfiles->setTitle("Vulkan Layers Configurations With VK_LAYER_PATH Precedence");

    // We need to resetup the new profile for consistency sake.
    QString last_configuration = _settings.value(VKCONFIG_KEY_ACTIVEPROFILE, QString("Validation - Standard")).toString();
    Configuration *current_configuration = configurator.FindConfiguration(last_configuration);
    if (configurator._override_active) ChangeActiveConfiguration(current_configuration);

    LoadConfigurationList();
    SetupLaunchTree();

    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(aboutVkConfig(bool)));
    connect(ui->actionVulkan_Info, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInfo(bool)));
    connect(ui->actionHelp, SIGNAL(triggered(bool)), this, SLOT(helpShowHelp(bool)));
    connect(ui->actionVulkan_specification, SIGNAL(triggered(bool)), this, SLOT(helpShowVulkanSpec(bool)));
    connect(ui->actionVulkan_Layer_Specification, SIGNAL(triggered(bool)), this, SLOT(helpShowLayerSpec(bool)));

    connect(ui->actionCustom_Layer_Paths, SIGNAL(triggered(bool)), this, SLOT(toolsSetCustomPaths(bool)));

    connect(ui->actionVulkan_Installation, SIGNAL(triggered(bool)), this, SLOT(toolsVulkanInstallation(bool)));
    connect(ui->actionRestore_Default_Configurations, SIGNAL(triggered(bool)), this, SLOT(toolsResetToDefault(bool)));

    connect(ui->profileTree, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(profileItemChanged(QTreeWidgetItem *, int)));
    connect(ui->profileTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this,
            SLOT(profileTreeChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(ui->profileTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationTreeClicked(QTreeWidgetItem *, int)));

    connect(ui->layerSettingsTree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(editorExpanded(QTreeWidgetItem *)));
    connect(ui->layerSettingsTree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
            SLOT(OnConfigurationSettingsTreeClicked(QTreeWidgetItem *, int)));

    connect(ui->launchTree, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(launchItemCollapsed(QTreeWidgetItem *)));
    connect(ui->launchTree, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(launchItemExpanded(QTreeWidgetItem *)));

    if (!configurator.HasOverriddenApplications() || configurator._overridden_application_list.empty())
        configurator._overridden_application_list_only = false;

    ui->pushButtonAppList->setEnabled(configurator._overridden_application_list_only);
    ui->checkBoxApplyList->setChecked(configurator._overridden_application_list_only);
    ui->checkBoxPersistent->setChecked(configurator._override_permanent);

    if (configurator._override_active) {
        ui->radioOverride->setChecked(true);
        ui->groupBoxProfiles->setEnabled(true);
        ui->groupBoxEditor->setEnabled(true);
    } else {
        ui->radioFully->setChecked(true);
        ui->checkBoxApplyList->setEnabled(false);
        ui->checkBoxPersistent->setEnabled(false);
        ui->pushButtonAppList->setEnabled(false);
        ui->groupBoxProfiles->setEnabled(false);
        ui->groupBoxEditor->setEnabled(false);
    }

    QSettings settings;
    if (settings.value(VKCONFIG_KEY_RESTORE_GEOMETRY).toBool()) {
        restoreGeometry(_settings.value("geometry").toByteArray());
        restoreState(_settings.value("windowState").toByteArray());
        ui->splitter->restoreState(_settings.value("splitter1State").toByteArray());
        ui->splitter_2->restoreState(_settings.value("splitter2State").toByteArray());
        ui->splitter_3->restoreState(_settings.value("splitter3State").toByteArray());
    }
    settings.setValue(VKCONFIG_KEY_RESTORE_GEOMETRY, true);

    // All else is done, highlight and activeate the current profile on startup
    Configuration *configuration = configurator.GetActiveConfiguration();
    if (configuration != nullptr) {
        for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
            ConfigurationListItem *pItem = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));
            if (pItem != nullptr)
                if (pItem->configuration == configuration) {  // Ding ding ding... we have a winner
                    ui->profileTree->setCurrentItem(pItem);
                }
        }
    }
    ui->groupBoxEditor->setEnabled(configuration != nullptr);

    ui->logBrowser->append("Vulkan Development Status:");
    ui->logBrowser->append(configurator.CheckVulkanSetup());
    ui->profileTree->scrollToItem(ui->profileTree->topLevelItem(0), QAbstractItemView::PositionAtTop);

    // Resetting this from the default prevents the log window (a QTextEdit) from overflowing.
    // Whenever the control surpasses this block count, old blocks are discarded.
    // Note: We could make this a user configurable setting down the road should this be
    // insufficinet.
    ui->logBrowser->document()->setMaximumBlockCount(2048);
}

MainWindow::~MainWindow() { delete ui; }

///////////////////////////////////////////////////////////////////////////////
// Load or refresh the list of profiles. Any profile that uses a layer that
// is not detected on the system is disabled.
void MainWindow::LoadConfigurationList() {
    // There are lots of ways into this, and in none of them
    // can we have an active editor running.
    _settings_tree_manager.CleanupGUI();
    ui->profileTree->blockSignals(true);  // No signals firing off while we do this
    ui->profileTree->clear();

    // Who is the currently active profile?
    QString active_configuration_name = _settings.value(VKCONFIG_KEY_ACTIVEPROFILE).toString();

    Configurator &configurator = Configurator::Get();

    for (int i = 0, n = configurator._available_configurations.size(); i < n; i++) {
        // Add to list
        ConfigurationListItem *item = new ConfigurationListItem();
        item->configuration = configurator._available_configurations[i];
        ui->profileTree->addTopLevelItem(item);
        item->setText(1, configurator._available_configurations[i]->_name);
        item->setToolTip(1, configurator._available_configurations[i]->_description);
        item->radio_button = new QRadioButton();
        item->radio_button->setText("");

        if (!configurator._available_configurations[i]->IsValid()) {
            item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
            item->radio_button->setEnabled(false);
            item->radio_button->setChecked(false);
            item->setToolTip(1, "Missing Vulkan Layer to use this configuration, try to add Custom Path to locate the layers");
        }

        // Check if this is the current config... but... depending on how we came into this
        // function, this configuration may no longer be active. So double check that. Simply,
        // if you make a current config invalid and come back in... it can't be active any
        // longer
        if (active_configuration_name == configurator._available_configurations[i]->_name) {
            if (configurator._available_configurations[i]->IsValid())
                item->radio_button->setChecked(true);
            else
                configurator.SetActiveConfiguration(nullptr);
        }

        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->profileTree->setItemWidget(item, 0, item->radio_button);
        connect(item->radio_button, SIGNAL(clicked(bool)), this, SLOT(profileItemClicked(bool)));
    }

    ui->profileTree->blockSignals(false);
    ChangeActiveConfiguration(configurator.GetActiveConfiguration());
    ui->profileTree->setColumnWidth(0, 24);
    ui->profileTree->resizeColumnToContents(1);
    ui->groupBoxEditor->setEnabled(configurator.GetActiveConfiguration() != nullptr);
}

//////////////////////////////////////////////////////////
// No override at all, fully controlled by the application
void MainWindow::on_radioFully_clicked() {
    ui->checkBoxApplyList->setEnabled(false);
    ui->checkBoxPersistent->setEnabled(false);

    Configurator &configurator = Configurator::Get();

    configurator._override_active = false;
    ui->groupBoxProfiles->setEnabled(false);
    ui->groupBoxEditor->setEnabled(false);

    ui->pushButtonAppList->setEnabled(false);

    configurator.SaveSettings();
    ChangeActiveConfiguration(nullptr);
}

//////////////////////////////////////////////////////////
/// Okay, because we are using custom controls, some of
/// the signaling is not happening as expected. So, we cannot
/// always get an accurate answer to the currently selected
/// item, but we do often need to know what has been checked
/// when an event occurs. This unambigously answers that question.
ConfigurationListItem *MainWindow::GetCheckedItem() {
    // Just go through all the top level items
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));

        if (item != nullptr)
            if (item->radio_button->isChecked()) return item;
    }

    return nullptr;
}

//////////////////////////////////////////////////////////
/// Use the active profile as the override
void MainWindow::on_radioOverride_clicked() {
    Configurator &configurator = Configurator::Get();

    bool use = (!configurator._has_old_loader || !been_warned_about_old_loader);
    ui->checkBoxApplyList->setEnabled(use);
    ui->pushButtonAppList->setEnabled(use && configurator._overridden_application_list_only);

    ui->checkBoxPersistent->setEnabled(true);
    configurator._override_active = true;
    ui->groupBoxProfiles->setEnabled(true);
    ui->groupBoxEditor->setEnabled(true);
    configurator.SaveSettings();

    // This just doesn't work. Make a function to look for the radio button checked.
    ConfigurationListItem *configuration_item = GetCheckedItem();
    if (configuration_item == nullptr)
        ChangeActiveConfiguration(nullptr);
    else
        ChangeActiveConfiguration(configuration_item->configuration);

    ui->groupBoxEditor->setEnabled(configurator.GetActiveConfiguration() != nullptr);
}

///////////////////////////////////////////////////////////////////////
// We want to apply to just the app list... hang on there. Doe we have
// the new loader?
void MainWindow::on_checkBoxApplyList_clicked() {
    Configurator &configurator = Configurator::Get();

    if (configurator._has_old_loader && !been_warned_about_old_loader) {
        uint32_t version = configurator._vulkan_instance_version;
        QString message;
        message = QString().asprintf(
            "The detected Vulkan Loader version is %d.%d.%d but version 1.2.141 or newer is required in order to apply layers "
            "override to only a selected list of Vulkan applications.\n\n<br><br>"
            "Get the latest Vulkan Runtime from <a href='https://vulkan.lunarg.com/sdk/home'>HERE.</a> to use this feature.",
            VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
        QMessageBox alert(this);
        alert.setTextFormat(Qt::RichText);
        alert.setText(message);
        alert.setIcon(QMessageBox::Warning);
        alert.setWindowTitle(tr("Layers override of a selected list of Vulkan Applications is not available"));
        alert.exec();

        ui->pushButtonAppList->setEnabled(false);
        ui->checkBoxApplyList->setEnabled(false);
        ui->checkBoxApplyList->setChecked(false);
        QString messageToolTip;
        messageToolTip =
            QString().asprintf("The detected Vulkan loader version is %d.%d.%d but version 1.2.141 or newer is required",
                               VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
        ui->checkBoxApplyList->setToolTip(messageToolTip);
        ui->pushButtonAppList->setToolTip(messageToolTip);
        been_warned_about_old_loader = true;
    }

    configurator._overridden_application_list_only = ui->checkBoxApplyList->isChecked();
    configurator.SaveSettings();
    ui->pushButtonAppList->setEnabled(configurator._overridden_application_list_only);

    if (configurator._overridden_application_list_only &&
        (configurator._overridden_application_list.empty() || !configurator.HasOverriddenApplications())) {
        on_pushButtonAppList_clicked();
    } else {
        // Checking the list, the configuration need to be updated to the system
        if (configurator.GetActiveConfiguration()) ChangeActiveConfiguration(configurator.GetActiveConfiguration());
    }

    ui->groupBoxEditor->setEnabled(configurator.GetActiveConfiguration() != nullptr);
}

//////////////////////////////////////////////////////////
void MainWindow::on_checkBoxPersistent_clicked() {
    Configurator &configurator = Configurator::Get();

    configurator._override_permanent = ui->checkBoxPersistent->isChecked();
    configurator.SaveSettings();
}

//////////////////////////////////////////////////////////
void MainWindow::toolsResetToDefault(bool checked) {
    (void)checked;

    // Let make sure...
    QMessageBox msg;
    msg.setIcon(QMessageBox::Warning);
    msg.setWindowTitle(tr("Restoring and Resetting all Layers Configurations to default"));
    msg.setText(
        tr("You are about to delete all the user-defined configurations and resetting all default configurations to their default "
           "state.\n\n"
           "Are you sure you want to continue?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if (msg.exec() == QMessageBox::No) return;

    Configurator &configurator = Configurator::Get();

    // Clear the current profile as we may be about to remove it.
    configurator.SetActiveConfiguration(nullptr);

    // Delete all the *.json files in the storage folder
    QDir dir(configurator.GetPath(Configurator::ConfigurationPath));
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList configuration_files = dir.entryInfoList();

    // Loop through all the profiles found and remove them
    for (int i = 0; i < configuration_files.size(); i++) {
        QFileInfo info = configuration_files.at(i);
        if (info.absoluteFilePath().contains("applist.json")) continue;
        remove(info.filePath().toUtf8().constData());
    }

    // Reset to recopy from resource file
    _settings.setValue(VKCONFIG_KEY_INITIALIZE_FILES, true);

    // Now we need to kind of restart everything
    _settings_tree_manager.CleanupGUI();
    configurator.LoadAllConfigurations();

    // Find the Standard Validation and make it current if we are active
    Configuration *active_configuration = configurator.FindConfiguration(QString("Validation - Standard"));
    if (configurator._override_active) ChangeActiveConfiguration(active_configuration);

    LoadConfigurationList();

    // Active or not, set it in the tree so we can see the settings.
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));
        if (item != nullptr)
            if (item->configuration == active_configuration) ui->profileTree->setCurrentItem(item);
    }

    configurator.FindVkCube();
    ResetLaunchOptions();

    ui->logBrowser->clear();
    ui->logBrowser->append("Vulkan Development Status:");
    ui->logBrowser->append(configurator.CheckVulkanSetup());

    if (configurator._override_active) {
        ui->radioOverride->setChecked(true);
        ui->checkBoxApplyList->setEnabled(true);
        ui->pushButtonAppList->setEnabled(false);
        ui->checkBoxPersistent->setEnabled(true);
        ui->groupBoxProfiles->setEnabled(true);
        ui->groupBoxEditor->setEnabled(true);
    } else {
        ui->radioFully->setChecked(true);
        ui->checkBoxApplyList->setEnabled(false);
        ui->pushButtonAppList->setEnabled(false);
        ui->checkBoxPersistent->setEnabled(false);
        ui->pushButtonAppList->setEnabled(false);
        ui->groupBoxProfiles->setEnabled(false);
        ui->groupBoxEditor->setEnabled(false);
    }
}

////////////////////////////////////////////////////////////////////////////
/// Thist signal actually comes from the radio button
void MainWindow::profileItemClicked(bool checked) {
    (void)checked;
    // Someone just got checked, they are now the current profile
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = GetCheckedItem();
    if (configuration_item == nullptr) return;

    Configurator &configurator = Configurator::Get();

    // Do we go ahead and activate it?
    if (configurator._override_active) {
        ChangeActiveConfiguration(configuration_item->configuration);
    }
}

/////////////////////////////////////////////////////////////////////////////
/// An item has been changed. Check for edit of the items name (configuration name)
void MainWindow::profileItemChanged(QTreeWidgetItem *item, int column) {
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(item);
    if (configuration_item == nullptr) return;

    if (column == 1) {  // Profile name
        _settings_tree_manager.CleanupGUI();
        Configurator &configurator = Configurator::Get();

        // We are renaming the file. Just delete the old one and save this
        const QString full_path =
            configurator.GetPath(Configurator::ConfigurationPath) + "/" + configuration_item->configuration->_file;
        remove(full_path.toUtf8().constData());

        configuration_item->configuration->_name = configuration_item->text(1);
        configuration_item->configuration->_file = configuration_item->text(1) + QString(".json");
        configurator.SaveConfiguration(configuration_item->configuration);
        RestoreLastItem(configuration_item->configuration->_name.toUtf8().constData());
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
/// This gets called with keyboard selections and clicks that do not necessarily
/// result in a radio button change (but it may). So we need to do two checks here, one
/// for the radio button, and one to change the editor/information at lower right.
void MainWindow::profileTreeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous) {
    (void)previous;
    _settings_tree_manager.CleanupGUI();
    // This pointer will only be valid if it's one of the elements with
    // the radio button
    ConfigurationListItem *configuration_item = dynamic_cast<ConfigurationListItem *>(current);
    if (configuration_item == nullptr) return;

    if (!Preferences::Get()._use_separated_select_and_activate) {
        configuration_item->radio_button->setChecked(true);
        ChangeActiveConfiguration(configuration_item->configuration);
    }

    _settings_tree_manager.CreateGUI(ui->layerSettingsTree, configuration_item->configuration);
    QString title = configuration_item->configuration->_name;
    title += " Settings";
    ui->groupBoxEditor->setTitle(title);
    ui->layerSettingsTree->resizeColumnToContents(0);
}

////////////////////////////////////////////////////
// Unused flag, just display the about Qt dialog
void MainWindow::aboutVkConfig(bool checked) {
    (void)checked;
    dlgAbout dlg(this);
    dlg.exec();
}

//////////////////////////////////////////////////////////
/// Create the VulkanInfo dialog if it doesn't already
/// exits & show it.
void MainWindow::toolsVulkanInfo(bool checked) {
    (void)checked;

    if (_vk_info == nullptr) _vk_info = new dlgVulkanInfo(this);

    _vk_info->RunTool();
}

//////////////////////////////////////////////////////////
/// Create the VulkanTools dialog if it doesn't already
/// exist & show it.
void MainWindow::toolsVulkanInstallation(bool checked) {
    (void)checked;
    if (_vk_via == nullptr) _vk_via = new dlgVulkanAnalysis(this);

    _vk_via->RunTool();
}

////////////////////////////////////////////////////////////////
/// Show help, which is just a rich text file
void MainWindow::helpShowHelp(bool checked) {
    (void)checked;
    if (_help == nullptr) _help = new dlgHelp(nullptr);

    _help->show();
}

////////////////////////////////////////////////////////////////
// Open the web browser to the Vulkan specification
void MainWindow::helpShowVulkanSpec(bool checked) {
    (void)checked;
#ifdef _WIN32
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/windows/1.2-extensions/vkspec.html"));
#elif defined(__APPLE__)
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/mac/1.2-extensions/vkspec.html"));
#else
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/linux/1.2-extensions/vkspec.html"));
#endif
}

////////////////////////////////////////////////////////////////
// Open the web browser to the Vulkan Layers specification
void MainWindow::helpShowLayerSpec(bool checked) {
    (void)checked;
#ifdef _WIN32
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/windows/layer_configuration.html"));
#elif defined(__APPLE__)
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/mac/layer_configuration.html"));
#else
    QDesktopServices::openUrl(QUrl("https://vulkan.lunarg.com/doc/view/latest/linux/layer_configuration.html"));
#endif
}

////////////////////////////////////////////////////////////////
/// The only thing we need to do here is clear the profile if
/// the user does not want it active.
void MainWindow::closeEvent(QCloseEvent *event) {
    Configurator &configurator = Configurator::Get();

    // Alert the user to the current state of the vulkan configurator and
    // give them the option to not shutdown.
    if (_settings.value(VKCONFIG_WARN_SHUTDOWNSTATE).toBool() == false) {
        QMessageBox alert(nullptr);
        QString shut_down_state;

        if (!configurator._override_permanent || !configurator._override_active)
            shut_down_state = "No Vulkan layers override will be active when Vulkan Configurator closes.";

        else {
            shut_down_state = "Vulkan Layers override will remain in effect when Vulkan Configurator closes.";

            if (configurator._overridden_application_list_only)
                shut_down_state += " Overrides will be applied only to the application list.";
            else
                shut_down_state += " Overrides will be applied to ALL Vulkan applications.";
        }

        shut_down_state += "\n\nAre you still ready to close Vulkan Configurator?";

        alert.setText(shut_down_state);
        alert.setWindowTitle("Vulkan Layers configuration state on exit");
        alert.setIcon(QMessageBox::Question);
        alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        alert.setCheckBox(new QCheckBox("Do not show again."));

        int ret_val = alert.exec();
        if (alert.checkBox()->isChecked()) _settings.setValue(VKCONFIG_WARN_SHUTDOWNSTATE, true);
        if (ret_val == QMessageBox::No) {
            event->ignore();
            return;
        }
    }

    _settings_tree_manager.CleanupGUI();
    if (!configurator._override_permanent) configurator.SetActiveConfiguration(nullptr);

    configurator.SaveOverriddenApplicationList();
    configurator.SaveSettings();

    _settings.setValue("geometry", saveGeometry());
    _settings.setValue("windowState", saveState());
    _settings.setValue("splitter1State", ui->splitter->saveState());
    _settings.setValue("splitter2State", ui->splitter_2->saveState());
    _settings.setValue("splitter3State", ui->splitter_3->saveState());

    QMainWindow::closeEvent(event);
}

////////////////////////////////////////////////////////////////
/// Resizing needs a little help. Yes please, there has to be
/// a better way of doing this.
void MainWindow::resizeEvent(QResizeEvent *event) {
    if (event != nullptr) event->accept();
}

/////////////////////////////////////////////////////////////
void MainWindow::showEvent(QShowEvent *event) {
    //  resizeEvent(nullptr); // Fake to get controls to do the right thing
    event->accept();
}

///////////////////////////////////////////////////////////////////////////////
/// Edit the list of apps that can be filtered.
void MainWindow::on_pushButtonAppList_clicked() {
    dlgCreateAssociation dlg(this);
    dlg.exec();

    Configurator &configurator = Configurator::Get();

    if (Preferences::Get()._use_last_selected_application_in_launcher) {
        configurator.SelectLaunchApplication(dlg.GetSelectedLaunchApplicationIndex());
    }

    configurator.SaveOverriddenApplicationList();
    ResetLaunchOptions();

    // Also, we may have changed exclusion flags, so reset override
    Configuration *active_configuration = configurator.GetActiveConfiguration();
    if (active_configuration != nullptr) configurator.SetActiveConfiguration(active_configuration);
    ui->groupBoxEditor->setEnabled(active_configuration != nullptr);
}

///////////////////////////////////////////////////////////////////////////////
/// Just resave the list anytime we go into the editor
void MainWindow::on_pushButtonEditProfile_clicked() {
    ConfigurationListItem *item = SaveLastItem();

    // Save current state before we go in
    _settings_tree_manager.CleanupGUI();

    dlgProfileEditor dlg(this, item->configuration);
    dlg.exec();

    // 'item' will be invalid after LoadAllConfigurations, but I still -  // need the pointer to the configuration
    const QString edited_configuration_name = item->configuration->_name;

    Configurator::Get().LoadAllConfigurations();
    LoadConfigurationList();

    RestoreLastItem();
}

///////////////////////////////////////////////////////////////
// When changes are made to the layer list, it forces a reload
// of the configuration list. This wipes everything out, so we
// need a way to restore the currently selected item whenever
// certain kinds of edits occur. These push/pop functions
// accomplish that. If nothing can be found it should simply
// leave nothing selected.
ConfigurationListItem *MainWindow::SaveLastItem(void) {
    // Who is selected?
    ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->currentItem());
    if (item == nullptr) return nullptr;

    _lastItem = item->configuration->_name;
    return item;
}

////////////////////////////////////////////////////////////////
// Partner for above function. Returns false if the last config
// could not be found.
bool MainWindow::RestoreLastItem(const char *szOverride) {
    if (szOverride != nullptr) _lastItem = szOverride;

    // Reset the current item
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));
        if (item != nullptr)
            if (item->configuration->_name == _lastItem) {
                ui->profileTree->setCurrentItem(item);
                return true;
            }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// Edit the layers for the given profile.
void MainWindow::EditClicked(ConfigurationListItem *item) {
    SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    dlgProfileEditor dlg(this, item->configuration);
    dlg.exec();

    Configurator::Get().LoadAllConfigurations();
    LoadConfigurationList();
    RestoreLastItem();
}

////////////////////////////////////////////////////////////////////////////////
// Create a new blank profile
void MainWindow::NewClicked() {
    SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    Configurator &configurator = Configurator::Get();

    Configuration *configuration = configurator.CreateEmptyConfiguration();
    dlgProfileEditor dlg(this, configuration);
    if (QDialog::Accepted == dlg.exec()) {
        configurator.LoadAllConfigurations();
        LoadConfigurationList();
    }
    RestoreLastItem();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::addCustomPaths
/// Allow addition or removal of custom layer paths. Afterwards reset the list
/// of loaded layers, but only if something was changed.
void MainWindow::addCustomPaths() {
    // Get the tree state and clear it.
    _settings_tree_manager.CleanupGUI();

    dlgCustomPaths dlg(this);
    dlg.exec();
    LoadConfigurationList();  // Force a reload
}

//////////////////////////////////////////////////////////////////////////////
/// Remove the currently selected user defined profile.
// This option does not automatically select another profile when you
// delete the current one. Since it's not possible to select it without
// making it current, this is the only reasonable option I see.
void MainWindow::RemoveClicked(ConfigurationListItem *item) {
    // Let make sure...
    QMessageBox msg;
    msg.setInformativeText(item->configuration->_name);
    msg.setText(tr("Are you sure you want to remove this configuration?"));
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Yes);
    if (msg.exec() == QMessageBox::No) return;

    SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    // What if this is the active profile? We will go boom boom soon...
    Configurator &configurator = Configurator::Get();
    if (configurator.GetActiveConfiguration() == item->configuration) configurator.SetActiveConfiguration(nullptr);

    // Delete the file
    const QString full_path = configurator.GetPath(Configurator::ConfigurationPath) + "/" + item->configuration->_file;
    remove(full_path.toUtf8().constData());

    // Reload profiles
    configurator.LoadAllConfigurations();
    LoadConfigurationList();
    if (!RestoreLastItem()) ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
}

/////////////////////////////////////////////////////////////////////////////
void MainWindow::RenameClicked(ConfigurationListItem *item) {
    SaveLastItem();
    ui->profileTree->editItem(item, 1);
}

/////////////////////////////////////////////////////////////////////////////
// Copy the current configuration
void MainWindow::DuplicateClicked(ConfigurationListItem *item) {
    SaveLastItem();
    QString new_name = item->configuration->_name;
    new_name += "2";
    _settings_tree_manager.CleanupGUI();
    item->configuration->_name = new_name;

    Configurator &configurator = Configurator::Get();
    configurator.SaveConfiguration(item->configuration);
    configurator.LoadAllConfigurations();
    LoadConfigurationList();

    // Good enough? Nope, I want to select it and edit the name.
    // Find it.
    for (int i = 0; i < ui->profileTree->topLevelItemCount(); i++) {
        ConfigurationListItem *pItem = dynamic_cast<ConfigurationListItem *>(ui->profileTree->topLevelItem(i));
        if (pItem->configuration->_name == new_name) {
            ui->profileTree->editItem(pItem, 1);
            return;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Import a configuration file. File copy followed by a reload.
void MainWindow::ImportClicked(ConfigurationListItem *item) {
    (void)item;  // We don't need this
    Configurator &configurator = Configurator::Get();

    QString full_suggested_path = configurator.GetPath(Configurator::LastImportPath);
    QString full_import_path =
        QFileDialog::getOpenFileName(this, "Import Layers Configuration File", full_suggested_path, "*.json");

    if (full_import_path.isEmpty()) return;

    SaveLastItem();
    _settings_tree_manager.CleanupGUI();
    Configurator::Get().ImportConfiguration(full_import_path);
    LoadConfigurationList();
    configurator.SaveSettings();
    RestoreLastItem();
}

/////////////////////////////////////////////////////////////////////////////
// Export a configuration file. Basically just a file copy
void MainWindow::ExportClicked(ConfigurationListItem *item) {
    Configurator &configurator = Configurator::Get();

    // Where to put it and what to call it
    QString full_suggested_path = configurator.GetPath(Configurator::LastExportPath) + "/" + item->configuration->_file;
    QString full_export_path =
        QFileDialog::getSaveFileName(this, "Export Layers Configuration File", full_suggested_path, "*.json");
    if (full_export_path.isEmpty()) return;

    configurator.ExportConfiguration(item->configuration->_file, full_export_path);
    configurator.SaveSettings();
}

/////////////////////////////////////////////////////////////////////////////
// Export a configuration file. Basically just a file copy
void MainWindow::EditCustomPathsClicked(ConfigurationListItem *item) {
    (void)item;
    addCustomPaths();
    LoadConfigurationList();  // Force a reload
}

void MainWindow::toolsSetCustomPaths(bool checked) {
    (void)checked;
    addCustomPaths();
    LoadConfigurationList();  // Force a reload
}

/////////////////////////////////////////////////////////////////////////////
/// Update "decorations": window caption, (Active) status in list
void MainWindow::ChangeActiveConfiguration(Configuration *configuration) {
    Configurator &configurator = Configurator::Get();

    if (configuration == nullptr || !configurator._override_active) {
        configurator.SetActiveConfiguration(nullptr);

        setWindowTitle("Vulkan Configurator <VULKAN APPLICATION CONTROLLED>");
    } else {
        QString newCaption = configuration->_name;
        if (!configuration->IsValid()) newCaption += " (DISABLED)";
        newCaption += " - Vulkan Configurator ";
        configurator.SetActiveConfiguration(configuration);
        newCaption += "<VULKAN APPLICATIONS OVERRIDDEN>";

        setWindowTitle(newCaption);
    }

    ui->groupBoxEditor->setEnabled(configurator.GetActiveConfiguration() != nullptr);
}

void MainWindow::editorExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui->layerSettingsTree->resizeColumnToContents(0);
}

void MainWindow::profileItemExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui->layerSettingsTree->resizeColumnToContents(0);
    ui->layerSettingsTree->resizeColumnToContents(1);
}

void MainWindow::OnConfigurationTreeClicked(QTreeWidgetItem *item, int column) {
    (void)item;
    (void)column;
    Configurator::Get().CheckApplicationRestart();
}

void MainWindow::OnConfigurationSettingsTreeClicked(QTreeWidgetItem *item, int column) {
    (void)item;
    (void)column;

    Configurator::Get().CheckApplicationRestart();
}

///////////////////////////////////////////////////////////////////
/// Reload controls for launch control
void MainWindow::ResetLaunchOptions() {
    Configurator &configurator = Configurator::Get();
    ui->pushButtonLaunch->setEnabled(!configurator._overridden_application_list.empty());

    // Reload launch apps selections
    _launcher_apps_combo->blockSignals(true);
    _launcher_apps_combo->clear();

    for (int i = 0; i < configurator._overridden_application_list.size(); i++) {
        _launcher_apps_combo->addItem(configurator._overridden_application_list[i]->executable_path);
    }

    if (configurator._overridden_application_list.isEmpty()) {
        _launch_arguments->setText("");
        _launcher_working->setText("");
        _launcher_log_file_edit->setText("");
        return;
    }

    int launch_application_index = configurator.GetLaunchApplicationIndex();
    Q_ASSERT(launch_application_index >= 0);

    configurator.SelectLaunchApplication(launch_application_index);
    _launcher_apps_combo->setCurrentIndex(launch_application_index);

    // Reset working folder and command line choices
    _launch_arguments->setText(configurator._overridden_application_list[launch_application_index]->arguments);
    _launcher_working->setText(configurator._overridden_application_list[launch_application_index]->working_folder);
    _launcher_log_file_edit->setText(configurator._overridden_application_list[launch_application_index]->log_file);
    _launcher_apps_combo->blockSignals(false);
}

///////////////////////////////////////////////////////////////////
/// Launch and log area
void MainWindow::SetupLaunchTree() {
    /////////////////////////////////////////////////////////////////
    // Executable
    QTreeWidgetItem *launcher_parent = new QTreeWidgetItem();
    launcher_parent->setText(0, "Executable Path");
    ui->launchTree->addTopLevelItem(launcher_parent);

    _launcher_apps_combo = new QComboBox();
    _launcher_apps_combo->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_apps_combo->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_parent, 1, _launcher_apps_combo);

    _launcher_apps_browse_button = new QPushButton();
    _launcher_apps_browse_button->setText("...");
    _launcher_apps_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_apps_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    _launcher_apps_browse_button->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_apps_browse_button->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_parent, 2, _launcher_apps_browse_button);
    connect(_launcher_apps_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(launchItemChanged(int)));
    connect(_launcher_apps_browse_button, SIGNAL(clicked()), this, SLOT(on_pushButtonAppList_clicked()));

    //////////////////////////////////////////////////////////////////
    // Working folder
    QTreeWidgetItem *launcher_folder_item = new QTreeWidgetItem();
    launcher_folder_item->setText(0, "Working Directory");
    launcher_parent->addChild(launcher_folder_item);

    _launcher_working = new QLineEdit();
    _launcher_working->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_working->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_folder_item, 1, _launcher_working);
    _launcher_working->setReadOnly(false);

    // Comming soon
    //    pLaunchWorkingFolderButton = new QPushButton();
    //    pLaunchWorkingFolderButton->setText("...");
    //    pLaunchWorkingFolderButton->setMinimumWidth(32);
    //    ui->launchTree->setItemWidget(pLauncherFolder, 2, pLaunchWorkingFolderButton);

    //////////////////////////////////////////////////////////////////
    // Command line arguments
    QTreeWidgetItem *launcher_arguments_item = new QTreeWidgetItem();
    launcher_arguments_item->setText(0, "Command-line Arguments");
    launcher_parent->addChild(launcher_arguments_item);

    _launch_arguments = new QLineEdit();
    _launch_arguments->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launch_arguments->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_arguments_item, 1, _launch_arguments);
    connect(_launch_arguments, SIGNAL(textEdited(const QString &)), this, SLOT(launchArgsEdited(const QString &)));

    // Comming soon
    //    pButton = new QPushButton();
    //    pButton->setText("...");
    //    ui->launchTree->setItemWidget(pLauncherCMD, 2, pButton);

    //////////////////////////////////////////////////////////////////
    // LOG FILE
    QTreeWidgetItem *launcher_log_file_item = new QTreeWidgetItem();
    launcher_log_file_item->setText(0, "Output Log");
    launcher_parent->addChild(launcher_log_file_item);

    _launcher_log_file_edit = new QLineEdit();
    _launcher_log_file_edit->setMinimumHeight(LAUNCH_ROW_HEIGHT);
    _launcher_log_file_edit->setMaximumHeight(LAUNCH_ROW_HEIGHT);
    ui->launchTree->setItemWidget(launcher_log_file_item, 1, _launcher_log_file_edit);
    connect(_launcher_log_file_edit, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeLogFile(const QString &)));

    _launcher_log_file_button = new QPushButton();
    _launcher_log_file_button->setText("...");
    _launcher_log_file_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    _launcher_log_file_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
    ui->launchTree->setItemWidget(launcher_log_file_item, 2, _launcher_log_file_button);
    connect(_launcher_log_file_button, SIGNAL(clicked()), this, SLOT(launchSetLogFile()));

    //////////////////////////////////////////////////////////////////
    ui->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);

    ui->launchTree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
    ui->launchTree->setColumnWidth(
        1, ui->launchTree->rect().width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
    ui->launchTree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);

    if (_settings.value("launcherCollapsed").toBool())
        launchItemCollapsed(nullptr);
    else
        ui->launchTree->expandItem(launcher_parent);

    ui->launchTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->launchTree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ResetLaunchOptions();
}

////////////////////////////////////////////////////////////////////
// Expanding the tree also grows the tree to match
void MainWindow::launchItemExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    ui->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 4 + 6);
    _settings.setValue("launcherCollapsed", false);
}

////////////////////////////////////////////////////////////////////
// Collapsing the tree also shrinks the tree to match and show only
// the first line
void MainWindow::launchItemCollapsed(QTreeWidgetItem *item) {
    (void)item;
    ui->launchTree->setMinimumHeight(LAUNCH_ROW_HEIGHT + 6);
    ui->launchTree->setMaximumHeight(LAUNCH_ROW_HEIGHT + 6);
    _settings.setValue("launcherCollapsed", true);
}

////////////////////////////////////////////////////////////////////
void MainWindow::launchSetLogFile() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    Q_ASSERT(current_application_index >= 0);

    const QString log_file =
        QDir::toNativeSeparators(QFileDialog::getSaveFileName(this, tr("Set Log File To..."), ".", tr("Log text(*.txt)")));

    Configurator &configurator = Configurator::Get();
    configurator._overridden_application_list[current_application_index]->log_file = log_file;

    if (log_file.isEmpty())
        _launcher_log_file_edit->setText("");
    else
        _launcher_log_file_edit->setText(log_file);

    configurator.SaveOverriddenApplicationList();
}

///////////////////////////////////////////////////////////////////
// Log file path edited manually.
void MainWindow::launchChangeLogFile(const QString &new_text) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    Q_ASSERT(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    configurator._overridden_application_list[current_application_index]->log_file = new_text;
    configurator.SaveOverriddenApplicationList();
}

////////////////////////////////////////////////////////////////////
/// Launch app change
void MainWindow::launchItemChanged(int application_index) {
    Configurator &configurator = Configurator::Get();

    if (application_index < 0 || application_index >= configurator._overridden_application_list.size()) return;

    _launch_arguments->setText(configurator._overridden_application_list[application_index]->arguments);
    _launcher_working->setText(configurator._overridden_application_list[application_index]->working_folder);
    _launcher_log_file_edit->setText(configurator._overridden_application_list[application_index]->log_file);

    configurator.SelectLaunchApplication(application_index);
    configurator.SaveSettings();
}

/////////////////////////////////////////////////////////////////////
/// New command line arguments. Update them.
void MainWindow::launchArgsEdited(const QString &arguments) {
    int application_index = _launcher_apps_combo->currentIndex();
    if (application_index < 0) return;

    Configurator &configurator = Configurator::Get();
    configurator._overridden_application_list[application_index]->arguments = arguments;
    configurator.SaveOverriddenApplicationList();
}

//////////////////////////////////////////////////////////////////////
// Clear the browser window
void MainWindow::on_pushButtonClearLog_clicked() {
    ui->logBrowser->clear();
    ui->logBrowser->update();
    ui->pushButtonClearLog->setEnabled(false);
}

//////////////////////////////////////////////////////////////////////
bool MainWindow::eventFilter(QObject *target, QEvent *event) {
    Configurator &configurator = Configurator::Get();

    // When no application are overridden in the application list,
    // we disable override_application_list_only to reflect to the
    // user that overrides will apply to all applications.
    if (configurator._override_application_list_updated && configurator._overridden_application_list_only) {
        configurator._override_application_list_updated = false;

        if (!configurator.HasOverriddenApplications()) {
            configurator._overridden_application_list_only = false;
            ui->checkBoxApplyList->setChecked(false);
            ui->pushButtonAppList->setEnabled(false);
        }
    }

    // Launch tree does some fancy resizing and since it's down in
    // layouts and splitters, we can't just relay on the resize method
    // of this window.
    if (target == ui->launchTree) {
        if (event->type() == QEvent::Resize) {
            QRect rect = ui->launchTree->rect();
            ui->launchTree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
            ui->launchTree->setColumnWidth(1, rect.width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
            ui->launchTree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);
            return false;
        }
    }

    // Context menus for layer configuration files
    if (target == ui->profileTree) {
        QContextMenuEvent *right_click = dynamic_cast<QContextMenuEvent *>(event);
        if (right_click && event->type() == QEvent::ContextMenu) {
            // Which item were we over?
            QTreeWidgetItem *configuration_item = ui->profileTree->itemAt(right_click->pos());
            ConfigurationListItem *item = dynamic_cast<ConfigurationListItem *>(configuration_item);

            // Create context menu here
            QMenu menu(ui->profileTree);

            QAction *edit_action = new QAction("Edit this Layers Configuration...", nullptr);
            edit_action->setEnabled(item != nullptr);
            menu.addAction(edit_action);

            QAction *new_action = new QAction("New Layers Configuration...", nullptr);
            new_action->setEnabled(true);
            menu.addAction(new_action);

            QAction *duplicate_action = new QAction("Duplicate the Layers Configuration", nullptr);
            duplicate_action->setEnabled(item != nullptr);
            menu.addAction(duplicate_action);

            QAction *remove_action = new QAction("Remove the Layers Configuration", nullptr);
            remove_action->setEnabled(item != nullptr);
            menu.addAction(remove_action);

            QAction *rename_action = new QAction("Rename the Layers Configuration", nullptr);
            rename_action->setEnabled(item != nullptr);
            menu.addAction(rename_action);

            menu.addSeparator();

            QAction *import_action = new QAction("Import a Layers Configuration...", nullptr);
            import_action->setEnabled(true);
            menu.addAction(import_action);

            QAction *export_action = new QAction("Export the Layers Configuration...", nullptr);
            export_action->setEnabled(item != nullptr);
            menu.addAction(export_action);

            menu.addSeparator();

            QAction *custom_path_action = new QAction("Edit Layers Custom Path...", nullptr);
            custom_path_action->setEnabled(true);
            menu.addAction(custom_path_action);

            QPoint point(right_click->globalX(), right_click->globalY());
            QAction *action = menu.exec(point);

            // Pointer compares made me throw up in my mouth at least a little
            // less than doing a full string compare. Setting up signal/slot for
            // all of these just seemed ridiculous. Every problem is not a nail,
            // put the hammer away....

            // Edit the selected profile
            if (action == edit_action) {
                EditClicked(item);
                return true;
            }

            // New Profile...
            if (action == new_action) {
                NewClicked();
                return true;
            }

            // Duplicate
            if (action == duplicate_action) {
                DuplicateClicked(item);
                return true;
            }

            // Remove this profile....
            if (action == remove_action) {
                RemoveClicked(item);
                return true;
            }

            // Rename this profile...
            if (action == rename_action) {
                RenameClicked(item);
                return true;
            }

            // Export this profile (copy the .json)
            if (action == export_action) {
                ExportClicked(item);
                ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Import a profile (copy a json)
            if (action == import_action) {
                ImportClicked(item);
                return true;
            }

            // Edit Layer custom paths
            if (action == custom_path_action) {
                _settings_tree_manager.CleanupGUI();
                EditCustomPathsClicked(item);
                ui->groupBoxEditor->setTitle(tr(EDITOR_CAPTION_EMPTY));
                return true;
            }

            // Do not pass on
            return true;
        }
    }

    // Pass it on
    return false;
}

///////////////////////////////////////////////////////////////////////////////////
/// Launch the app and monitor it's stdout to get layer output.
/// stdout is buffered by default, so in order to see realtime output it must
/// be flushed. Either of the following in the other app will do.
/// > fflush(stdout);    // Flush now
/// setlinebuf(stdout);  // always flush at the end of a line
///
/// The layers are automtically flushed, so they should show up as they
/// generated. One note... any other stdout generated by the monitored
/// application will also be captured.
///
/// If logging is enbabled (by setting a logging file), then the log file
/// is also opened.
void MainWindow::on_pushButtonLaunch_clicked() {
    // Are we already monitoring a running app? If so, terminate it
    if (_launch_application != nullptr) {
        _launch_application->terminate();
        _launch_application->deleteLater();
        _launch_application = nullptr;
        ui->pushButtonLaunch->setText(tr("Launch"));
        return;
    }

    // Is there an app selected?
    int current_application_index = _launcher_apps_combo->currentIndex();

    // Launch the test application
    _launch_application = new QProcess(this);
    connect(_launch_application, SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));

    connect(_launch_application, SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));

    connect(_launch_application, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processClosed(int, QProcess::ExitStatus)));

    Configurator &configurator = Configurator::Get();
    _launch_application->setProgram(configurator._overridden_application_list[current_application_index]->executable_path);
    _launch_application->setWorkingDirectory(configurator._overridden_application_list[current_application_index]->working_folder);

    if (!configurator._overridden_application_list[current_application_index]->arguments.isEmpty()) {
        const QStringList args = configurator._overridden_application_list[current_application_index]->arguments.split(" ");
        _launch_application->setArguments(args);
    }

    // Some of these may have changed
    configurator.SaveSettings();

    _launch_application->start(QIODevice::ReadOnly | QIODevice::Unbuffered);
    _launch_application->setProcessChannelMode(QProcess::MergedChannels);
    _launch_application->closeWriteChannel();

    // We are logging, let's add that we've launched a new application
    QString launch_log = "Launching Vulkan Application:\n";

    const Application &current_application = *configurator._overridden_application_list[current_application_index];

    if (configurator.GetActiveConfiguration() == nullptr) {
        launch_log += QString().asprintf("- Layers fully controlled by the application.\n");
    } else if (!configurator.GetActiveConfiguration()->IsValid()) {
        launch_log += QString().asprintf("- No layers override. The active \"%s\" configuration is missing a layer.\n",
                                         configurator.GetActiveConfiguration()->_name.toUtf8().constData());
    } else if (configurator._override_active) {
        if (configurator._overridden_application_list_only && configurator.HasOverriddenApplications() &&
            !current_application.override_layers) {
            launch_log +=
                QString().asprintf("- Layers fully controlled by the application. Application excluded from layers override.\n");
        } else {
            launch_log += QString().asprintf("- Layers overridden by \"%s\" configuration.\n",
                                             configurator.GetActiveConfiguration()->_name.toUtf8().constData());
        }
    }

    launch_log += QString().asprintf("- Executable Path: %s\n", current_application.executable_path.toUtf8().constData());
    launch_log += QString().asprintf("- Working Directory: %s\n", current_application.working_folder.toUtf8().constData());
    launch_log += QString().asprintf("- Command-line Arguments: %s\n", current_application.arguments.toUtf8().constData());
    launch_log += QString().asprintf("- Log file: %s\n", current_application.log_file.toUtf8().constData());

    if (!current_application.log_file.isEmpty()) {
        // This should never happen... but things that should never happen do in
        // fact happen... so just a sanity check.
        if (_log_file != nullptr) {
            _log_file->close();
            _log_file = nullptr;
        }

        // Start logging
        _log_file = new QFile(current_application.log_file);

        // Open and append, or open and truncate?
        QIODevice::OpenMode mode = QIODevice::WriteOnly | QIODevice::Text;
        if (!ui->checkBoxClearOnLaunch->isChecked()) mode |= QIODevice::Append;

        if (!current_application.log_file.isEmpty()) {
            if (!_log_file->open(mode)) {
                QMessageBox err;
                err.setText(tr("Cannot open log file"));
                err.setIcon(QMessageBox::Warning);
                err.exec();
                delete _log_file;
                _log_file = nullptr;
            }
        }

        if (_log_file) {
            _log_file->write((launch_log + "\n").toUtf8().constData(), launch_log.length());
        }
    }

    if (ui->checkBoxClearOnLaunch->isChecked()) ui->logBrowser->clear();
    ui->logBrowser->append(launch_log);
    ui->pushButtonClearLog->setEnabled(true);

    // Wait... did we start? Give it 4 seconds, more than enough time
    if (!_launch_application->waitForStarted(4000)) {
        _launch_application->waitForStarted();
        _launch_application->deleteLater();
        _launch_application = nullptr;

        QString failed_log = QString().asprintf("Failed to launch %s!\n", current_application.executable_path.toUtf8().constData());

        ui->logBrowser->append(failed_log);
        if (_log_file) _log_file->write(failed_log.toUtf8().constData(), failed_log.length());

        return;
    }

    // We are off to the races....
    ui->pushButtonLaunch->setText(tr("Terminate"));
}

/////////////////////////////////////////////////////////////////////////////
/// The process we are following is closed. We don't actually care about the
/// exit status/code, we just need to know to destroy the QProcess object
/// and set it back to nullptr so that we know we can launch a new app.
/// Also, if we are logging, it's time to close the log file.
void MainWindow::processClosed(int exit_code, QProcess::ExitStatus status) {
    (void)exit_code;
    (void)status;

    // Not likely, but better to be sure...
    if (_launch_application == nullptr) return;

    disconnect(_launch_application, SIGNAL(finished(int, QProcess::ExitStatus)), this,
               SLOT(processClosed(int, QProcess::ExitStatus)));

    disconnect(_launch_application, SIGNAL(readyReadStandardError()), this, SLOT(errorOutputAvailable()));

    disconnect(_launch_application, SIGNAL(readyReadStandardOutput()), this, SLOT(standardOutputAvailable()));

    ui->pushButtonLaunch->setText(tr("Launch"));

    if (_log_file) {
        _log_file->close();
        delete _log_file;
        _log_file = nullptr;
    }

    delete _launch_application;
    _launch_application = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// This signal get's raised whenever the spawned Vulkan appliction writes
/// to stdout and there is data to be read. The layers flush after all stdout
/// writes, so we should see layer output here in realtime, as we just read
/// the string and append it to the text browser.
/// If a log file is open, we also write the output to the log.
void MainWindow::standardOutputAvailable() {
    if (_launch_application == nullptr) return;

    QString log = _launch_application->readAllStandardOutput();
    ui->logBrowser->append(log);
    ui->pushButtonClearLog->setEnabled(true);

    // Are we logging?
    if (_log_file) _log_file->write(log.toUtf8().constData(), log.length());
}

///////////////////////////////////////////////////////////////////////////////
void MainWindow::errorOutputAvailable() {
    if (_launch_application == nullptr) return;

    QString log = _launch_application->readAllStandardError();
    ui->logBrowser->append(log);
    ui->pushButtonClearLog->setEnabled(true);

    // Are we logging?
    if (_log_file) _log_file->write(log.toUtf8().constData(), log.length());
}
