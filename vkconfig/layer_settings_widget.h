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

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QMenu>
#include <QRadioButton>
#include <QToolBox>

#include "layer_manifest.h"

class SettingsWidget
{
public:
    virtual void setValue(const QString &value) = 0;
};

class BoolSelectWidget : public QWidget, public SettingsWidget
{
    Q_OBJECT

public:
    BoolSelectWidget(const LayerOption &option, QWidget *parent = NULL);
    virtual void setValue(const QString &value) override;
    inline LayerValue value() const { return value_info; }

signals:
    void valueChanged(const LayerValue &value);

private slots:
    void setFalse(bool checked);
    void setTrue(bool checked);

private:
    LayerOption option;
    LayerValue value_info;

    QRadioButton *true_button;
    QRadioButton *false_button;
};

class EnumSelectWidget : public QComboBox
{
    Q_OBJECT

public:
    EnumSelectWidget(const LayerOption &option, QWidget *parent = NULL);
    inline LayerValue value() const { return value_info; }

signals:
    void valueChanged(const LayerValue &value);

private slots:
    void selectIndex(int index);

private:
    LayerOption option;
    LayerValue value_info;
    QList<QString> keys;
};

class MultiEnumSelectWidget : public QComboBox
{
    Q_OBJECT

public:
    MultiEnumSelectWidget(const LayerOption &option, QWidget *parent = NULL);
    virtual void showPopup() override;
    inline LayerValue value() const { return value_info; }

signals:
    void valueChanged(const LayerValue &value);

private slots:
    void changeItem(QAction *action);

private:
    void updateText();

    LayerOption option;
    LayerValue value_info;
    QMenu *popup_menu;
};

class FileSelectWidget : public QWidget
{
    Q_OBJECT

public:
    FileSelectWidget(const LayerOption &option, QWidget *parent = NULL);
    inline LayerValue value() const { return value_info; }

signals:
    void valueChanged(const LayerValue &value);

private slots:
    void selectFile();
    void setText(const QString &text);

private:
    LayerOption option;
    LayerValue value_info;
    QLineEdit *line_edit;
};

class StringSelectWidget : public QLineEdit
{
    Q_OBJECT

public:
    StringSelectWidget(const LayerOption &option, QWidget *parent = NULL);
    inline LayerValue value() const { return value_info; }

signals:
    void valueChanged(const LayerValue &value);

private slots:
    void setValue(const QString &text);

private:
    LayerOption option;
    LayerValue value_info;
};

class LayerSettingsWidget : public QGroupBox
{
    Q_OBJECT

public:
    LayerSettingsWidget(QWidget *parent = NULL);
    void setSettingsValues(const QHash<QString, QHash<QString, QString>> &values);
    inline QHash<QString, QHash<QString, LayerValue>> settings() { return layer_settings; }

signals:
    void settingsChanged(const QHash<QString, QHash<QString, LayerValue>> &settings);

public slots:
    void updateAvailableLayers(const QList<LayerManifest>& enabled_layers, const QList<LayerManifest>& disabled_layers);

private slots:
    void changeValue(const LayerValue &value);
    void updateLayers();

private:
    QHash<QString, QHash<QString, LayerValue>> layer_settings;
    QList<LayerManifest> enabled_layers;
    QList<LayerManifest> disabled_layers;

    QHash<QString, QHash<QString, SettingsWidget*>> settings_widgets;
    QCheckBox *enabled_only_box;
    QToolBox *tool_box;
};
