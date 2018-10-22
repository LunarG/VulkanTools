
#include "vkconfig.h"

#include <QApplication>
#include <QBoxLayout>
#include <QFile>

int main(int argc, char **argv)
{
    QCoreApplication::setOrganizationName("LunarG");
    QCoreApplication::setOrganizationDomain("lunarg.com");
    QCoreApplication::setApplicationName("vkconfig");

    QApplication app(argc, argv);
    LayerManager manager;
    manager.show();
    return app.exec();
}

LayerManager::LayerManager()
{
    // Build the GUI
    setWindowIcon(QPixmap(":/layermgr/icons/logo_square.png"));
    setWindowTitle(tr("Vulkan Configuration Tool"));

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

    QHBoxLayout *button_layout = new QHBoxLayout();
    QPushButton *save_button = new QPushButton(tr("Save"));
    button_layout->addWidget(save_button, 0);
    QPushButton *restore_button = new QPushButton(tr("Restore"));
    button_layout->addWidget(restore_button, 0);
    QPushButton *clear_button = new QPushButton(tr("Clear"));
    button_layout->addWidget(clear_button, 0);

    button_layout->addSpacing(24);
    notification_label = new QLabel("");
    notification_base_color = notification_label->palette().color(QPalette::WindowText);
    connect(&notification_timer, &QTimer::timeout, this, &LayerManager::timerUpdate);
    button_layout->addWidget(notification_label);
    button_layout->addStretch(1);

    QPushButton *exit_button = new QPushButton(tr("Exit"));
    button_layout->addWidget(exit_button, 0);

    connect(save_button, &QPushButton::clicked, this, &LayerManager::saveAll);
    connect(restore_button, &QPushButton::clicked, this, &LayerManager::restore);
    connect(clear_button, &QPushButton::clicked, this, &LayerManager::clear);
    connect(exit_button, &QPushButton::clicked, this, &LayerManager::close);
    layout->addLayout(button_layout, 0);

    layer_widget->setLayout(layout);
    setCentralWidget(layer_widget);

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
        active_layers->setExpiration(settings.value("ExpirationValue").toInt(), (DurationUnit) settings.value("ExpirationUnit").toInt());
    }
    active_layers->setEnabledLayers(override_settings.Layers());

    layer_settings->setSettingsValues(override_settings.LayerSettings());
    // TODO: Restore the active layer

    inner_split->restoreState(settings.value("InnerSplitState").toByteArray());
    outer_split->restoreState(settings.value("OuterSplitState").toByteArray());
    restoreGeometry(settings.value("WindowGeometry").toByteArray());
}

void LayerManager::closeEvent(QCloseEvent *event)
{
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
    settings.setValue("ExpirationUnit", (int) active_layers->expirationUnit());

    settings.setValue("InnerSplitState", inner_split->saveState());
    settings.setValue("OuterSplitState", outer_split->saveState());
    settings.setValue("WindowGeometry", saveGeometry());
}

void LayerManager::clear()
{
    override_settings.ClearLayers();
    override_settings.ClearSettings();
    active_layers->clearLayers();
    notify("Cleared all layers and settings");
}

void LayerManager::notify(const QString& message)
{
    notification_label->setText("(" + QTime::currentTime().toString() + ") " + message);
    QPalette palette = notification_label->palette();
    palette.setColor(QPalette::WindowText, QColor(255, 0, 0));
    notification_label->setPalette(palette);
    notification_timer.start(50);
}

void LayerManager::restore()
{
    active_layers->setEnabledLayers(override_settings.Layers());
    layer_settings->setSettingsValues(override_settings.LayerSettings());

    notify("Restored layers and settings to last saved state");
}

void LayerManager::saveAll()
{
    // The override settings are saved here, but the regular settings are only saved when we close
    QList<QPair<QString, LayerType>> paths;
    if (locations->useCustomLayerPaths()) {
        paths = locations->customLayerPaths();
    }
    const QList<LayerManifest> &layers = active_layers->enabledLayers();
    override_settings.SaveLayers(paths, layers, active_layers->expiration());

    const QHash<QString, QHash<QString, LayerValue>> &settings = layer_settings->settings();
    override_settings.SaveSettings(settings);

    notify("Saved all layers and settings");
}

void LayerManager::timerUpdate()
{
    QPalette palette = notification_label->palette();
    QColor color = palette.color(QPalette::WindowText);
    if (color == notification_base_color) {
        notification_timer.stop();
    }

    color.setRed(color.red() + qBound(-4, notification_base_color.red() - color.red(), 4));
    palette.setColor(QPalette::WindowText, color);
    notification_label->setPalette(palette);
}
