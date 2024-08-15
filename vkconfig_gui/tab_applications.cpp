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

#include "tab_applications.h"
#include "mainwindow.h"

#include "../vkconfig_core/configurator.h"

/*
    case PATH_EXECUTABLE: {
        static const char* TABLE[] = {
            "Applications (*.exe)",     // PLATFORM_WINDOWS
            "Applications (*)",         // PLATFORM_LINUX
            "Applications (*.app, *)",  // PLATFORM_MACOS
            "N/A"                       // PLATFORM_ANDROID
        };
        static_assert(std::size(TABLE) == PLATFORM_COUNT,
                        "The tranlation table size doesn't match the enum number of elements");

        const std::string filter = TABLE[VKC_PLATFORM];
        const std::string selected_path = QFileDialog::getOpenFileName(parent, "Select a Vulkan Executable...",
                                                                        suggested_path.AbsolutePath().c_str(), filter.c_str())
                                                .toStdString();
        if (selected_path.empty())  // The user cancelled
            return Path("");

        SetPath(path, QFileInfo(selected_path.c_str()).absolutePath().toStdString());
        return GetFullPath(path, QFileInfo(selected_path.c_str()).fileName().toStdString());
    }
    case PATH_WORKING_DIR: {
        const std::string selected_path =
            QFileDialog::getExistingDirectory(parent, "Set Working Folder To...", suggested_path.AbsolutePath().c_str())
                .toStdString();
        if (selected_path.empty())  // The user cancelled
            return Path("");

        SetPath(path, selected_path);
        return Path(GetPath(path));
    }
*/

TabApplications::TabApplications(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_APPLICATIONS, window, ui) {}

TabApplications::~TabApplications() {}

void TabApplications::UpdateUI(UpdateUIMode mode) {
    const Configurator &configurator = Configurator::Get();
    const Environment &environment = Configurator::Get().environment;

    const std::vector<Application> &applications = configurator.environment.GetApplications();
    ui->application_list->clear();
    for (std::size_t i = 0, n = applications.size(); i < n; ++i) {
        QListWidgetItem *application_item = new QListWidgetItem();
        application_item->setText(applications[i].executable_path.RelativePath().c_str());
        application_item->setToolTip(applications[i].executable_path.AbsolutePath().c_str());
        ui->application_list->addItem(application_item);
    }

    const std::vector<Configuration> &configurations = configurator.configurations.available_configurations;
    ui->application_configuration_comboBox->clear();
    for (std::size_t i = 0, n = configurations.size(); i < n; ++i) {
        ui->application_configuration_comboBox->addItem(configurations[i].key.c_str());
    }

    /*
        // App Name
        QTreeWidgetItem *launcher_parent = new QTreeWidgetItem();
        launcher_parent->setText(0, "Application");
        ui->application_list->addTopLevelItem(launcher_parent);

        _launcher_apps_combo = new QComboBox();
        _launcher_apps_combo->setFocusPolicy(Qt::StrongFocus);
        _launcher_apps_combo->setMinimumHeight(LAUNCH_ROW_HEIGHT);
        _launcher_apps_combo->setMaximumHeight(LAUNCH_ROW_HEIGHT);
        ui->application_list->setItemWidget(launcher_parent, 1, _launcher_apps_combo);

        _launcher_apps_browse_button = new QPushButton();
        _launcher_apps_browse_button->setText("...");
        _launcher_apps_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        _launcher_apps_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
        _launcher_apps_browse_button->setMinimumHeight(LAUNCH_ROW_HEIGHT);
        _launcher_apps_browse_button->setMaximumHeight(LAUNCH_ROW_HEIGHT);
        ui->launcher_tree->setItemWidget(launcher_parent, 2, _launcher_apps_browse_button);
        connect(_launcher_apps_combo, SIGNAL(currentIndexChanged(int)), this, SLOT(launchItemChanged(int)));
        connect(_launcher_apps_browse_button, SIGNAL(clicked()), this, SLOT(on_push_button_applications_clicked()));

        // Executable
        QTreeWidgetItem *launcher_executable_item = new QTreeWidgetItem();
        launcher_executable_item->setText(0, "Executable");
        launcher_parent->addChild(launcher_executable_item);

        _launcher_executable = new QLineEdit();
        _launcher_executable->setMinimumHeight(LAUNCH_ROW_HEIGHT);
        _launcher_executable->setMaximumHeight(LAUNCH_ROW_HEIGHT);
        ui->application_list->setItemWidget(launcher_executable_item, 1, _launcher_executable);
        _launcher_executable->setReadOnly(false);

        _launcher_executable_browse_button = new QPushButton();
        _launcher_executable_browse_button->setText("...");
        _launcher_executable_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        _launcher_executable_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
        _launcher_executable_browse_button->setMinimumHeight(LAUNCH_ROW_HEIGHT);
        _launcher_executable_browse_button->setMaximumHeight(LAUNCH_ROW_HEIGHT);
        ui->launcher_tree->setItemWidget(launcher_executable_item, 2, _launcher_executable_browse_button);
        connect(_launcher_executable, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeExecutable(const QString &)));
        connect(_launcher_executable_browse_button, SIGNAL(clicked()), this, SLOT(launchSetExecutable()));

        // Working folder
        QTreeWidgetItem *launcher_folder_item = new QTreeWidgetItem();
        launcher_folder_item->setText(0, "Working Directory");
        launcher_parent->addChild(launcher_folder_item);

        _launcher_working = new QLineEdit();
        _launcher_working->setMinimumHeight(LAUNCH_ROW_HEIGHT);
        _launcher_working->setMaximumHeight(LAUNCH_ROW_HEIGHT);
        ui->launcher_tree->setItemWidget(launcher_folder_item, 1, _launcher_working);
        _launcher_working->setReadOnly(false);

        _launcher_working_browse_button = new QPushButton();
        _launcher_working_browse_button->setText("...");
        _launcher_working_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        _launcher_working_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
        _launcher_working_browse_button->setMinimumHeight(LAUNCH_ROW_HEIGHT);
        _launcher_working_browse_button->setMaximumHeight(LAUNCH_ROW_HEIGHT);
        ui->launcher_tree->setItemWidget(launcher_folder_item, 2, _launcher_working_browse_button);
        connect(_launcher_working, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeWorkingFolder(const QString &)));
        connect(_launcher_working_browse_button, SIGNAL(clicked()), this, SLOT(launchSetWorkingFolder()));

        // Command line arguments
        QTreeWidgetItem *launcher_arguments_item = new QTreeWidgetItem();
        launcher_arguments_item->setText(0, "Command-line Arguments");
        launcher_parent->addChild(launcher_arguments_item);

        _launcher_arguments = new QLineEdit();
        _launcher_arguments->setMinimumHeight(LAUNCH_ROW_HEIGHT);
        _launcher_arguments->setMaximumHeight(LAUNCH_ROW_HEIGHT);
        ui->launcher_tree->setItemWidget(launcher_arguments_item, 1, _launcher_arguments);
        connect(_launcher_arguments, SIGNAL(textEdited(const QString &)), this, SLOT(launchArgsEdited(const QString &)));

        // Log file
        QTreeWidgetItem *launcher_log_file_item = new QTreeWidgetItem();
        launcher_log_file_item->setText(0, "Output Log");
        launcher_parent->addChild(launcher_log_file_item);

        _launcher_log_file_edit = new QLineEdit();
        _launcher_log_file_edit->setMinimumHeight(LAUNCH_ROW_HEIGHT);
        _launcher_log_file_edit->setMaximumHeight(LAUNCH_ROW_HEIGHT);
        ui->launcher_tree->setItemWidget(launcher_log_file_item, 1, _launcher_log_file_edit);
        connect(_launcher_log_file_edit, SIGNAL(textEdited(const QString &)), this, SLOT(launchChangeLogFile(const QString &)));

        _launcher_log_file_browse_button = new QPushButton();
        _launcher_log_file_browse_button->setText("...");
        _launcher_log_file_browse_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        _launcher_log_file_browse_button->setMaximumWidth(LAUNCH_COLUMN2_SIZE);
        ui->launcher_tree->setItemWidget(launcher_log_file_item, 2, _launcher_log_file_browse_button);
        connect(_launcher_log_file_browse_button, SIGNAL(clicked()), this, SLOT(launchSetLogFile()));

        // Launcher tree
        ui->launcher_tree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 5 + 6);
        ui->launcher_tree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 5 + 6);

        ui->launcher_tree->setColumnWidth(0, LAUNCH_COLUMN0_SIZE);
        ui->launcher_tree->setColumnWidth(
            1, ui->launcher_tree->rect().width() - LAUNCH_COLUMN0_SIZE - LAUNCH_COLUMN2_SIZE - LAUNCH_SPACING_SIZE);
        ui->launcher_tree->setColumnWidth(2, LAUNCH_COLUMN2_SIZE);

        ui->launcher_tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->launcher_tree->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    */
}

void TabApplications::CleanUI() {}

bool TabApplications::EventFilter(QObject *target, QEvent *event) { return false; }

/*
// Expanding the tree also grows the tree to match
void MainWindow::launchItemExpanded(QTreeWidgetItem *item) {
    (void)item;
    ui->launcher_tree->setMinimumHeight(LAUNCH_ROW_HEIGHT * 5 + 6);
    ui->launcher_tree->setMaximumHeight(LAUNCH_ROW_HEIGHT * 5 + 6);
}

// Collapsing the tree also shrinks the tree to match and show only the first line
void MainWindow::launchItemCollapsed(QTreeWidgetItem *item) {
    (void)item;
    ui->launcher_tree->setMinimumHeight(LAUNCH_ROW_HEIGHT + 6);
    ui->launcher_tree->setMaximumHeight(LAUNCH_ROW_HEIGHT + 6);
}
*/

/*
void MainWindow::OnLauncherLoaderMessageChanged(int level) {
    Configurator &configurator = Configurator::Get();

    // configurator.environment.SetLoaderMessage(static_cast<LoaderMessageLevel>(level));
    // configurator.environment.SetLoaderMessageFlags(GetBit(static_cast<LogBit>(level)));

    if (ui->check_box_clear_on_launch->isChecked()) {
        this->UpdateUI();
    }
}

void MainWindow::launchSetExecutable() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);

    //const Path exe = configurator.path.SelectPath(this, PATH_EXECUTABLE, application.executable_path);

    //// The user has cancel the operation
    //if (exe.Empty()) {
    //    return;
    //}

    //application.executable_path = exe;

    _launcher_executable->setText(application.executable_path.RelativePath().c_str());
}
*/
/*
void MainWindow::launchSetLogFile() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    ApplicationOptions &options = application.GetActiveOptions();
    // TODO
    //const Path &path = configurator.path.SelectPath(this, PATH_LAUNCHER_LOG_FILE, options.log_file);

    //// The user has cancel the operation
    //if (path.Empty()) {
    //    return;
    //}

    //options.log_file = path;

    _launcher_log_file_edit->setText(options.log_file.RelativePath().c_str());
}
*/
/*
void MainWindow::launchSetWorkingFolder() {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Configurator &configurator = Configurator::Get();
    Application &application = configurator.environment.GetApplication(current_application_index);
    ApplicationOptions &options = application.GetActiveOptions();

    // TODO
    //const Path path = configurator.path.SelectPath(this, PATH_WORKING_DIR, options.working_folder);

    //// The user has cancel the operation
    //if (path.Empty()) {
    //    return;
    //}

    //options.working_folder = path;

    _launcher_working->setText(options.working_folder.RelativePath().c_str());
}
*/

/*
// Log file path edited manually.
void MainWindow::launchChangeLogFile(const QString &log_file) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    ApplicationOptions &options = application.GetActiveOptions();
    options.log_file = log_file.toStdString();
}

void MainWindow::launchChangeExecutable(const QString &exe) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    application.executable_path = exe.toStdString();
}

void MainWindow::launchChangeWorkingFolder(const QString &working_folder) {
    int current_application_index = _launcher_apps_combo->currentIndex();
    assert(current_application_index >= 0);

    Application &application = Configurator::Get().environment.GetApplication(current_application_index);
    ApplicationOptions &options = application.GetActiveOptions();
    options.working_folder = working_folder.toStdString();
}

// Launch app change
void MainWindow::launchItemChanged(int application_index) {
    if (application_index < 0) return;

    Environment &environment = Configurator::Get().environment;

    environment.SelectActiveApplication(application_index);

    Application &application = environment.GetApplication(application_index);
    ApplicationOptions &options = application.GetActiveOptions();

    _launcher_executable->setText(application.executable_path.RelativePath().c_str());
    _launcher_working->setText(options.working_folder.RelativePath().c_str());
    _launcher_arguments->setText(Merge(options.arguments, " ").c_str());
    _launcher_log_file_edit->setText(options.log_file.RelativePath().c_str());
}

/// New command line arguments. Update them.
void MainWindow::launchArgsEdited(const QString &arguments) {
    int application_index = _launcher_apps_combo->currentIndex();
    if (application_index < 0) return;

    Application &application = Configurator::Get().environment.GetApplication(application_index);
    ApplicationOptions &options = application.GetActiveOptions();
    options.arguments = SplitSpace(arguments.toStdString());
}
*/
