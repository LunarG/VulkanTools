/*
 * Copyright (c) 2018-2019 Valve Corporation
 * Copyright (c) 2018-2019 LunarG, Inc.
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

#include "vkconfig.h"

#include <QApplication>
#include <QBoxLayout>
#include <QFile>
#include <qmessagebox.h>

#if !defined(NO_HTML)
#include <QWebEngineView>
#endif

int main(int argc, char **argv) {
    QCoreApplication::setOrganizationName("LunarG");
    QCoreApplication::setOrganizationDomain("lunarg.com");
    QCoreApplication::setApplicationName("vkconfig");
#if defined(_WIN32)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);
    LayerManager manager;
    manager.show();
    return app.exec();
}

LayerManager::LayerManager() {
    setWindowIcon(QPixmap(":/layermgr/icons/logo_square.png"));
    setWindowTitle(tr("Vulkan Configuration Tool"));

    QWidget *center_widget = new QWidget();
    QVBoxLayout *center_layout = new QVBoxLayout();

    QTabWidget *tab_widget = new QTabWidget();

    // Run vulkaninfo and display its output on a tab
#if !defined(NO_HTML)
    QProcess *vulkan_info = new QProcess(this);
    vulkan_info->setProgram("vulkaninfo");
    vulkan_info->setArguments(QStringList() << "--html");
    vulkan_info->setWorkingDirectory(QDir::temp().path());
    tab_widget->addTab(showHtml(vulkan_info, "Vulkan Info", "vulkaninfo.html"), tr("Vulkan Info"));
#endif

    // Build the layer manager
    QWidget *layer_widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    outer_split = new QSplitter(Qt::Horizontal);
    inner_split = new QSplitter(Qt::Vertical);

    locations = new LayerLocationsWidget();
    inner_split->addWidget(locations);
    active_layers = new ActiveLayersWidget();
    inner_split->addWidget(active_layers);
    outer_split->addWidget(inner_split);

    QWidget *right_widget = new QWidget();
    QVBoxLayout *right_layout = new QVBoxLayout();
    QLabel *logo_widget = new QLabel();
    logo_widget->setPixmap(QPixmap(":/layermgr/icons/lunarg_logo.png"));
    right_layout->addWidget(logo_widget, 0, Qt::AlignRight);
    layer_settings = new LayerSettingsWidget();
    right_layout->addWidget(layer_settings, 1);
    right_widget->setLayout(right_layout);
    outer_split->addWidget(right_widget);

    connect(locations, &LayerLocationsWidget::pathsChanged, active_layers, &ActiveLayersWidget::updateAvailableLayers);
    connect(active_layers, &ActiveLayersWidget::enabledLayersUpdated, layer_settings, &LayerSettingsWidget::updateAvailableLayers);
    layout->addWidget(outer_split, 1);

    layer_widget->setLayout(layout);
    tab_widget->addTab(layer_widget, tr("Layer Manager"));

    // Run via and display its output on a tab
#if !defined(NO_HTML)
    QProcess *via = new QProcess(this);
    via->setProgram("vkvia");
    via->setArguments(QStringList() << "--disable_cube_tests"
                                    << "--output_path" << QDir::temp().path());
    tab_widget->addTab(showHtml(via, "VIA", "vkvia.html"), tr("Installation Analyzer"));
#endif

    connect(tab_widget, &QTabWidget::currentChanged, this, &LayerManager::tabChanged);
    center_layout->addWidget(tab_widget, 1);

    QHBoxLayout *button_layout = new QHBoxLayout();
    save_button = new QPushButton(tr("Save"));
    save_button->setToolTip(tr("Save layers and settings"));
    button_layout->addWidget(save_button, 0);
    restore_button = new QPushButton(tr("Restore"));
    restore_button->setToolTip(tr("Restore to last saved state"));
    button_layout->addWidget(restore_button, 0);
    clear_button = new QPushButton(tr("Clear"));
    clear_button->setToolTip(tr("Clear saved layers and settings"));
    button_layout->addWidget(clear_button, 0);

#if !defined(NO_HTML)
    save_button->setEnabled(false);
    restore_button->setEnabled(false);
    clear_button->setEnabled(false);
#endif

    button_layout->addSpacing(24);
    notification_label = new QLabel("");
    notification_base_color = notification_label->palette().color(QPalette::WindowText);
    connect(&notification_timer, &QTimer::timeout, this, &LayerManager::timerUpdate);
    button_layout->addWidget(notification_label);
    button_layout->addStretch(1);

    // Create about button, hook to message box below
    QPushButton *about_button = new QPushButton(tr("About VkConfig"));
    connect(about_button, &QPushButton::clicked, this, &LayerManager::about);
    button_layout->addWidget(about_button, 0);

    QPushButton *exit_button = new QPushButton(tr("Exit"));
    button_layout->addWidget(exit_button, 0);

    connect(save_button, &QPushButton::clicked, this, &LayerManager::saveAll);
    connect(restore_button, &QPushButton::clicked, this, &LayerManager::restore);
    connect(clear_button, &QPushButton::clicked, this, &LayerManager::clear);
    connect(exit_button, &QPushButton::clicked, this, &LayerManager::close);
    center_layout->addLayout(button_layout, 0);

    center_widget->setLayout(center_layout);
    setCentralWidget(center_widget);

    // Restore the state from the settings and the override settings
    QList<QPair<QString, LayerType>> custom_paths;
    int length = settings.beginReadArray("LayerPaths");
    for (int i = 0; i < length; ++i) {
        settings.setArrayIndex(i);
        QString location = settings.value("Path").toString();
        LayerType type = LayerEnum(settings.value("Type").toString());
        custom_paths.append(QPair<QString, LayerType>(location, type));
    }
    settings.endArray();
    locations->setCustomLayerPaths(custom_paths);
    locations->setUseCustomLayerPaths(settings.value("UseCustomPaths").toBool());

    if (settings.contains("ExpirationValue") && settings.contains("ExpirationUnit")) {
        active_layers->setExpiration(settings.value("ExpirationValue").toInt(),
                                     (DurationUnit)settings.value("ExpirationUnit").toInt());
    }
    active_layers->setEnabledLayers(override_settings.EnabledLayers());
    active_layers->setDisabledLayers(override_settings.DisabledLayers());

    layer_settings->setSettingsValues(override_settings.LayerSettings());
    // TODO: Restore the active layer

    inner_split->restoreState(settings.value("InnerSplitState").toByteArray());
    outer_split->restoreState(settings.value("OuterSplitState").toByteArray());
    restoreGeometry(settings.value("WindowGeometry").toByteArray());
}

void LayerManager::closeEvent(QCloseEvent *event) {
    if (active_layers->shouldClearOnClose()) {
        clear();
    }

    // The settings are saved here, but the override settings are only saved when the user clicks "Save"
    settings.setValue("UseCustomPaths", locations->useCustomLayerPaths());

    settings.remove("LayerPaths");
    settings.beginWriteArray("LayerPaths");
    int i = 0;
    for (const QPair<QString, LayerType> &pair : locations->customLayerPaths()) {
        settings.setArrayIndex(i++);
        settings.setValue("Type", LayerString(pair.second));
        settings.setValue("Path", pair.first);
    }
    settings.endArray();

    settings.setValue("ExpirationValue", active_layers->expiration());
    settings.setValue("ExpirationUnit", (int)active_layers->expirationUnit());

    settings.setValue("InnerSplitState", inner_split->saveState());
    settings.setValue("OuterSplitState", outer_split->saveState());
    settings.setValue("WindowGeometry", saveGeometry());
}

void LayerManager::clear() {
    override_settings.ClearLayers();
    override_settings.ClearSettings();
    active_layers->clearEnabledLayers();
    active_layers->clearDisabledLayers();
    notify("Cleared all layers and settings");
}

void LayerManager::notify(const QString &message) {
    notification_label->setText("(" + QTime::currentTime().toString() + ") " + message);
    QPalette palette = notification_label->palette();
    palette.setColor(QPalette::WindowText, QColor(255, 0, 0));
    notification_label->setPalette(palette);
    notification_timer.start(50);
}

void LayerManager::restore() {
    active_layers->setEnabledLayers(override_settings.EnabledLayers());
    active_layers->setDisabledLayers(override_settings.DisabledLayers());
    layer_settings->setSettingsValues(override_settings.LayerSettings());

    notify("Restored layers and settings to last saved state");
}

void LayerManager::saveAll() {
    // The override settings are saved here, but the regular settings are only saved when we close
    QList<QPair<QString, LayerType>> paths;
    if (locations->useCustomLayerPaths()) {
        paths = locations->customLayerPaths();
    }
    const QList<LayerManifest> &enabled_layers = active_layers->enabledLayers();
    const QList<LayerManifest> &disabled_layers = active_layers->disabledLayers();
    override_settings.SaveLayers(paths, enabled_layers, disabled_layers, active_layers->expiration());

    const QHash<QString, QHash<QString, LayerValue>> &settings = layer_settings->settings();
    override_settings.SaveSettings(settings);

    notify("Saved all layers and settings");
}

void LayerManager::about() {
    QMessageBox *msgbox = new QMessageBox();

    msgbox->setTextFormat(Qt::RichText);
    msgbox->setText(
        "Copyright 2019 LunarG, Inc.<br/>"
        "Licensed under Apache 2.0<br/>"
        "<br/>"
        "Qt licensed under  <a href='https://spdx.org/licenses/GPL-1.0-or-later.html'>LGPL 3.0</a>:<br/>"
        "&nbsp;&nbsp;&nbsp;&nbsp;- <a href='https://doc.qt.io/qt-5/qtcore-index.html'>Qt Core source</a><br/>"
        "&nbsp;&nbsp;&nbsp;&nbsp;- <a href='https://doc.qt.io/qt-5/qtgui-index.html'>Qt GUI source</a><br/>"
        "&nbsp;&nbsp;&nbsp;&nbsp;- <a href='https://doc.qt.io/qt-5/qtwidgets-index.html'>Qt Widgets source</a><br/>"
        "&nbsp;&nbsp;&nbsp;&nbsp;- <a href='https://doc.qt.io/qt-5/qtwebengine-licensing.html'>Qt WebEngine source</a><br/>");

    QPixmap pixmap = QPixmap(":/layermgr/icons/lunarg_logo.png");
    msgbox->setWindowIcon(QPixmap(":/layermgr/icons/logo_square.png"));
    msgbox->setIconPixmap(pixmap);
    msgbox->exec();
}

void LayerManager::tabChanged(int index) {
#if !defined(NO_HTML)
    bool enabled = index == 1;
#else
    bool enabled = true;
#endif
    save_button->setEnabled(enabled);
    restore_button->setEnabled(enabled);
    clear_button->setEnabled(enabled);
}

void LayerManager::timerUpdate() {
    QPalette palette = notification_label->palette();
    QColor color = palette.color(QPalette::WindowText);
    if (color == notification_base_color) {
        notification_timer.stop();
    }

    color.setRed(color.red() + qBound(-4, notification_base_color.red() - color.red(), 4));
    palette.setColor(QPalette::WindowText, color);
    notification_label->setPalette(palette);
}

#if !defined(NO_HTML)
QWidget *LayerManager::showHtml(QProcess *process, const QString &name, const QString &html_file) {
    process->start();
    if (process->waitForFinished() || process->error() != QProcess::FailedToStart) {
        if (QDir::temp().exists(html_file)) {
            QWebEngineView *display = new QWebEngineView();
            display->load(QUrl::fromLocalFile(QDir::temp().absoluteFilePath(html_file)));
            return display;
        } else {
            QString message =
                "<b>Could not find %1 output file (%2).</b><br>"
                "An unknown error occurred in %1. To diagnose this, try running %1 separately.";
            QLabel *error = new QLabel(message.arg(name, html_file));
            error->setAlignment(Qt::AlignCenter);
            return error;
        }
    } else {
        QString message =
            "<b>Could not start %1.</b><br>"
            "Most likely, the '%2' executable is not in the system PATH.<br>"
            "Add '%2' to the system path to see the results here.";
        QLabel *error = new QLabel(message.arg(name, process->program()));
        error->setAlignment(Qt::AlignCenter);
        return error;
    }
}
#endif
