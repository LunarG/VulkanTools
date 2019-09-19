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

#include "layer_settings_widget.h"

#include <QApplication>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>

LayerSettingsWidget::LayerSettingsWidget(QWidget *parent)
    : QGroupBox(tr("Layer Settings"), parent)
{
    QVBoxLayout *layout = new QVBoxLayout();

    enabled_only_box = new QCheckBox(tr("Show enabled layers only"));
    enabled_only_box->setCheckState(Qt::Checked);
    connect(enabled_only_box, &QCheckBox::stateChanged, this, &LayerSettingsWidget::updateLayers);
    layout->addWidget(enabled_only_box);

    tool_box = new QToolBox();
    layout->addWidget(tool_box);

    setLayout(layout);
    setFixedWidth(500);
}

void LayerSettingsWidget::setSettingsValues(const QHash<QString, QHash<QString, QString>> &values)
{
    for (const QString &layer : values.keys()) {
        for (const QString &setting : values[layer].keys()) {
            QString value = values[layer][setting];
            if (settings_widgets.contains(layer) && settings_widgets[layer].contains(setting)) {
                settings_widgets[layer][setting]->setValue(value);
            }
        }
    }
}

void LayerSettingsWidget::updateAvailableLayers(const QList<LayerManifest>& enabled, const QList<LayerManifest>& disabled)
{
    enabled_layers = enabled;
    disabled_layers = disabled;
    updateLayers();
}

void LayerSettingsWidget::changeValue(const LayerValue &value)
{
    layer_settings[value.layer_name][value.name] = value;
    emit settingsChanged(layer_settings);
}

void LayerSettingsWidget::updateLayers()
{
    QList<LayerManifest> layers;
    if (enabled_only_box->isChecked()) {
        layers = enabled_layers;
    } else {
        layers = enabled_layers + disabled_layers;
        qSort(layers);
    }

    // Remove any items that are no longer present
    for (const QString &layer : layer_settings.keys()) {
        bool layer_found = false;
        for (const LayerManifest &manifest :  layers) {
            if (manifest.name == layer) {
                layer_found = true;
                break;
            }
        }
        if (!layer_found) {
            for (int i = 0; i < tool_box->count(); ++i) {
                if (tool_box->itemToolTip(i) == layer) {
                    tool_box->removeItem(i);
                    layer_settings.remove(layer);
                }
            }
        }
    }

    // Add any new layers
    for (const LayerManifest &manifest : layers) {

        // Skip if the layer is already present
        bool layer_found = false;
        for (const QString &layer : layer_settings.keys()) {
            if (manifest.name == layer) {
                layer_found = true;
                break;
            }
        }
        if (layer_found) {
            continue;
        }

        // Figure out where to add the item
        int index = 0;
        for (const QString &layer : layer_settings.keys()) {
            if (!(manifest.name < layer)) {
                ++index;
            }
        }

        // Add the layer
        QList<LayerOption> options = LayerOption::LoadOptions(manifest);
        if (options.empty()) {
            continue;
        }
        qSort(options);
        layer_settings.insert(manifest.name, QHash<QString, LayerValue>());
        QByteArray bytes = manifest.name.toLocal8Bit();
        const char* str = bytes.data();

        QWidget *widget = new QWidget();
        QGridLayout *grid = new QGridLayout();

        for (int i = 0; i < options.count(); ++i) {
            QLabel *label = new QLabel(options[i].pretty_name + ":");
            if (options[i].description != "") {
                label->setToolTip(options[i].description);
            }
            grid->addWidget(label, i, 0);

            switch (options[i].type) {

            case LayerOptionType::Bool: {
                BoolSelectWidget *selector = new BoolSelectWidget(options[i]);
                layer_settings[manifest.name].insert(options[i].name, selector->value());
                connect(selector, &BoolSelectWidget::valueChanged, this, &LayerSettingsWidget::changeValue);
                grid->addWidget(selector, i, 2);
                settings_widgets[manifest.name][options[i].name] = selector;
                break;
            }
            case LayerOptionType::Enum: {
                EnumSelectWidget *selector = new EnumSelectWidget(options[i]);
                layer_settings[manifest.name].insert(options[i].name, selector->value());
                connect(selector, &EnumSelectWidget::valueChanged, this, &LayerSettingsWidget::changeValue);
                grid->addWidget(selector, i, 2);
                break;
            }
            case LayerOptionType::MultiEnum: {
                MultiEnumSelectWidget *selector = new MultiEnumSelectWidget(options[i]);
                layer_settings[manifest.name].insert(options[i].name, selector->value());
                connect(selector, &MultiEnumSelectWidget::valueChanged, this, &LayerSettingsWidget::changeValue);
                grid->addWidget(selector, i, 2);
                break;
            }
            case LayerOptionType::OpenFile:
            case LayerOptionType::SaveFile: {
                FileSelectWidget *selector = new FileSelectWidget(options[i]);
                layer_settings[manifest.name].insert(options[i].name, selector->value());
                connect(selector, &FileSelectWidget::valueChanged, this, &LayerSettingsWidget::changeValue);
                grid->addWidget(selector, i, 2);
                break;
            }
            case LayerOptionType::String: {
                StringSelectWidget *selector = new StringSelectWidget(options[i]);
                layer_settings[manifest.name].insert(options[i].name, selector->value());
                connect(selector, &StringSelectWidget::valueChanged, this, &LayerSettingsWidget::changeValue);
                grid->addWidget(selector, i, 2);
                break;
            }
            }
        }
        grid->setColumnStretch(1, 1);
        grid->setRowStretch(options.count(), 1);
        widget->setLayout(grid);

        tool_box->insertItem(index, widget, manifest.PrettyName());
        tool_box->setItemToolTip(index, manifest.name);
    }
}

BoolSelectWidget::BoolSelectWidget(const LayerOption &option, QWidget *parent)
    : QWidget(parent), option(option), value_info{option.layer_name, option.name, LayerOptionType::Bool, QSet<QString>()}
{
    if (!option.default_values.empty()) {
        value_info.values.insert(*option.default_values.begin());
    }

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    true_button = new QRadioButton(tr("True"));
    layout->addWidget(true_button);
    false_button = new QRadioButton(tr("False"));
    layout->addWidget(false_button);
    (option.default_values.contains("True") ? true_button : false_button)->setChecked(true);
    setLayout(layout);

    connect(true_button, &QRadioButton::clicked, this, &BoolSelectWidget::setTrue);
    connect(false_button, &QRadioButton::clicked, this, &BoolSelectWidget::setFalse);
}

void BoolSelectWidget::setValue(const QString &value)
{
    if (value == "TRUE") {
        true_button->setChecked(true);
    } else {
        false_button->setChecked(true);
    }
}

void BoolSelectWidget::setFalse(bool checked)
{
    value_info.values.clear();
    value_info.values.insert(checked ? "FALSE" : "TRUE");

    emit valueChanged(value_info);
}

void BoolSelectWidget::setTrue(bool checked)
{
    value_info.values.clear();
    value_info.values.insert(checked ? "TRUE" : "FALSE");

    emit valueChanged(value_info);
}

EnumSelectWidget::EnumSelectWidget(const LayerOption &option, QWidget *parent)
    : QComboBox(parent), option(option), value_info{option.layer_name, option.name, LayerOptionType::Enum, QSet<QString>()}
{
    if (!option.default_values.empty()) {
        value_info.values.insert(*option.default_values.begin());
    }
    keys = option.enum_options.keys();
    qSort(keys);

    for (const QString &key : keys) {
        addItem(option.enum_options[key]);
        if (option.default_values.contains(key)) {
            setCurrentIndex(count() - 1);
        }
    }

    connect(this, static_cast<void (EnumSelectWidget::*)(int)>(&EnumSelectWidget::activated), this, &EnumSelectWidget::selectIndex);
}

void EnumSelectWidget::selectIndex(int index)
{
    value_info.values.clear();
    value_info.values.insert(keys[index]);

    emit valueChanged(value_info);
}

MultiEnumSelectWidget::MultiEnumSelectWidget(const LayerOption &option, QWidget *parent)
    : QComboBox(parent), option(option), value_info{option.layer_name, option.name, LayerOptionType::Enum, QSet<QString>()}
{
    for (const QString &item : option.default_values) {
        value_info.values.insert(item);
    }

    QComboBox::addItem(tr("<None>"));
    QComboBox::setMinimumContentsLength(50);
    popup_menu = new QMenu(this);
    popup_menu->addAction(tr("<None>"));
    popup_menu->addSeparator();
    connect(popup_menu, &QMenu::triggered, this, &MultiEnumSelectWidget::changeItem);

    QList<QString> keys = option.enum_options.keys();
    qSort(keys);
    for (const QString &key : keys) {
        QAction *action = popup_menu->addAction(option.enum_options[key]);
        action->setData(key);
        action->setCheckable(true);
        action->setChecked(option.default_values.contains(key));
    }
    updateText();
}

void MultiEnumSelectWidget::showPopup()
{
    QComboBox::hidePopup();

    popup_menu->setFixedWidth(width());
    popup_menu->popup(mapToGlobal(QPoint(0, 0)));
}

void MultiEnumSelectWidget::changeItem(QAction *action)
{
    QList<QAction*> actions = popup_menu->actions();
    if (action == actions.first()) {
        return;
    }

    QString value = action->data().toString();
    if (action->isChecked()) {
        value_info.values.insert(value);
    } else {
        value_info.values.remove(value);
    }

    updateText();
    showPopup();
}

void MultiEnumSelectWidget::updateText()
{
    QStringList selected_items;
    QList<QAction*> actions = popup_menu->actions();
    for (QAction *action : actions) {
        if (action->isChecked()) {
            selected_items.append(action->text());
        }
    }

    QString text = selected_items.empty() ? tr("<None>") : selected_items.join(", ");
    setItemText(0, text);
    actions.first()->setText(text);

    emit valueChanged(value_info);
}

FileSelectWidget::FileSelectWidget(const LayerOption &option, QWidget *parent)
    : QWidget(parent), option(option), value_info{option.layer_name, option.name, option.type, QSet<QString>()}
{
    if (!option.default_values.empty()) {
        value_info.values.insert(*option.default_values.begin());
    }

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    line_edit = new QLineEdit();
    line_edit->setText(*option.default_values.begin());
    line_edit->setToolTip(line_edit->text());
    line_edit->setSizePolicy(QSizePolicy::Maximum, line_edit->sizePolicy().verticalPolicy());
    line_edit->setMinimumWidth(300);
    layout->addWidget(line_edit, 1);
    connect(line_edit, &QLineEdit::textChanged, this, &FileSelectWidget::setText);

    QPushButton *button = new QPushButton();
    button->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon));
    button->setSizePolicy(QSizePolicy::Minimum, button->sizePolicy().verticalPolicy());
    button->setMaximumWidth(20);
    layout->addWidget(button, 0);
    connect(button, &QPushButton::clicked, this, &FileSelectWidget::selectFile);

    setLayout(layout);
}

void FileSelectWidget::selectFile()
{
    QString path;
    switch (option.type) {

    case LayerOptionType::OpenFile:
        path = QFileDialog::getOpenFileName(this, tr("File Path"), QDir::homePath());
        break;
    case LayerOptionType::SaveFile:
        path = QFileDialog::getSaveFileName(this, tr("File Path"), QDir::homePath());
        break;
    default:
        return;
    }
    if (path == "") {
        return;
    }

    line_edit->setText(path);
    line_edit->setToolTip(path);
}

void FileSelectWidget::setText(const QString &text)
{
    value_info.values.clear();
    value_info.values.insert(text);

    emit valueChanged(value_info);
}

StringSelectWidget::StringSelectWidget(const LayerOption &option, QWidget *parent)
    : QLineEdit(parent), option(option), value_info{option.layer_name, option.name, option.type, QSet<QString>()}
{
    if (!option.default_values.empty()) {
        value_info.values.insert(*option.default_values.begin());
        setText(*option.default_values.begin());
    }
    connect(this, &StringSelectWidget::textChanged, this, &StringSelectWidget::setValue);
}

void StringSelectWidget::setValue(const QString &text)
{
    value_info.values.clear();
    value_info.values.insert(text);

    emit valueChanged(value_info);
}
