/*
 * Copyright (c) 2018 Valve Corporation
 * Copyright (c) 2018 LunarG, Inc.
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

#pragma once

#include <QDir>
#include <QJsonObject>
#include <QList>
#include <QSet>
#include <QString>

#if defined(_WIN32)
#include <windows.h>
#include <cfgmgr32.h>
#endif

// Don't change the number values; doing so will break compatibility with saved settings
enum class LayerType {
    Explicit = 0,
    Implicit = 1,
};

inline unsigned qHash(LayerType type) { return qHash((unsigned)type); }
inline LayerType LayerEnum(const QString &type) { return type == "Explicit" ? LayerType::Explicit : LayerType::Implicit; }
inline QString LayerString(LayerType type) { return type == LayerType::Explicit ? "Explicit" : "Implicit"; };

class LayerManifest {
  public:
    QString PrettyName() const;
    inline bool operator<(const LayerManifest &that) const { return name < that.name; }

    static QList<LayerManifest> LoadDirectory(const QDir &directory, LayerType type, bool recursive = false);
#if defined(_WIN32)
    static QList<LayerManifest> LoadRegistry(const QString &path, LayerType type);
#endif

    QString file_path;
    QString name;
    QString description;
    LayerType type;

  private:
    LayerManifest() = default;

    static QHash<QString, QString> LoadLayerAuthors(const QString &file_path);
    static void LoadLayerFile(const QString &file_path, LayerType type, QList<LayerManifest> *manifest_list);
    static void LoadLayerObject(const QJsonObject &layer_object, LayerType type, const QFileInfo &file,
                                QList<LayerManifest> *manifest_list);
#if defined(_WIN32)
    static QList<LayerManifest> LoadDeviceRegistry(DEVINST id, const QString &entry, LayerType type);
#endif
};

enum class LayerOptionType {
    Bool,
    Enum,
    MultiEnum,
    OpenFile,
    SaveFile,
    String,
};

class LayerOption {
  public:
    inline bool operator<(const LayerOption &that) const { return pretty_name < that.pretty_name; }

    static QList<LayerOption> LoadOptions(const LayerManifest &manifest);

    QString layer_name;
    QString name;
    QString pretty_name;
    QString description;
    LayerOptionType type;
    QSet<QString> default_values;
    QHash<QString, QString> enum_options;

  private:
    LayerOption() = default;

    static QJsonObject LoadOptionJson(const QString &file_path);
};

class LayerValue {
  public:
    QString layer_name;
    QString name;
    LayerOptionType type;
    QSet<QString> values;
};
