
#pragma once

#include <QEvent>
#include <QLabel>
#include <QMainWindow>
#include <QSettings>
#include <QSplitter>
#include <QTimer>

#include "active_layers_widget.h"
#include "layer_locations_widget.h"
#include "layer_settings_widget.h"
#include "override_settings.h"

class LayerManager : public QMainWindow
{
    Q_OBJECT

public:
    LayerManager();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void clear();
    void notify(const QString& message);
    void restore();
    void saveAll();
    void timerUpdate();

private:
    QSettings settings;
    OverrideSettings override_settings;

    QSplitter *outer_split;
    QSplitter *inner_split;
    LayerLocationsWidget *locations;
    ActiveLayersWidget *active_layers;
    LayerSettingsWidget *layer_settings;

    QLabel *notification_label;
    QTimer notification_timer;
    QColor notification_base_color;
};
