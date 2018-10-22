
#pragma once

#include <QHash>
#include <QList>
#include <QString>

#include "layer_manifest.h"

// Add a Paths() method?
class OverrideSettings
{
public:
    OverrideSettings();
    void ClearLayers();
    void ClearSettings();
    QList<QString> Layers() const;
    QHash<QString, QHash<QString, QString>> LayerSettings() const;
    bool SaveLayers(const QList<QPair<QString, LayerType>> &paths, const QList<LayerManifest> &layers, int expiration);
    bool SaveSettings(const QHash<QString, QHash<QString, LayerValue>> &settings);

private:
    QString LayerFile(bool create_path) const;
    QString LayerSettingsFile(bool create_path) const;

    QList<QString> layers;
    QHash<QString, QHash<QString, QString>> layer_settings;
};
