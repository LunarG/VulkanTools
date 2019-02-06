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

#include "active_layers_widget.h"

#include <QApplication>
#include <QBoxLayout>
#include <QPushButton>

ActiveLayersWidget::ActiveLayersWidget(QWidget *parent)
    : QGroupBox(tr("Active Layers"), parent)
{
    custom_paths = true;

    layer_icons[LayerType::Explicit] = QIcon(":/layermgr/icons/explicit.png");
    layer_icons[LayerType::Implicit] = QIcon(":/layermgr/icons/implicit.png");

    QVBoxLayout *layer_layout = new QVBoxLayout();
    QHBoxLayout *top_layout = new QHBoxLayout();
    cleanup_box = new QCheckBox(tr("Undo changes on exit"));
    cleanup_box->setCheckState(Qt::Checked);
    connect(cleanup_box, &QCheckBox::stateChanged, this, &ActiveLayersWidget::toggleExpiration);
    top_layout->addWidget(cleanup_box);
    top_layout->addStretch(1);
    expiration_label = new QLabel(tr("Expiration:"));
    top_layout->addWidget(expiration_label, 0);
    expiration_spin = new QSpinBox();
    expiration_spin->setMaximum(999);
    expiration_spin->setValue(12);
    top_layout->addWidget(expiration_spin, 0);
    expiration_units = new QComboBox();
    expiration_units->addItem(tr("Minutes"));
    expiration_units->addItem(tr("Hours"));
    expiration_units->addItem(tr("Days"));
    expiration_units->addItem(tr("Never"));
    expiration_units->setCurrentIndex(3);
    connect(expiration_units, static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::activated), this, &ActiveLayersWidget::setExpirationEnabled);
    top_layout->addWidget(expiration_units, 0);
    top_layout->addStretch(1);
    QPushButton *refresh_button = new QPushButton();
    refresh_button->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
    refresh_button->setToolTip(tr("Refresh layers"));
    connect(refresh_button, &QPushButton::clicked, this, &ActiveLayersWidget::refreshAvailableLayers);
    top_layout->addWidget(refresh_button, 0);
    layer_layout->addLayout(top_layout);

    QHBoxLayout *column_layout = new QHBoxLayout();

    // Build the buttons on the left
    QGridLayout *left_column_layout = new QGridLayout();
    QPushButton *up_button = new QPushButton();
    up_button->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowUp));
    up_button->setObjectName("UpButton");
    up_button->setToolTip(tr("Move up"));
    connect(up_button, &QPushButton::clicked, this, &ActiveLayersWidget::moveSelectedLayerUp);
    left_column_layout->addWidget(up_button, 2, 0);
    QPushButton *down_button = new QPushButton();
    down_button->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowDown));
    down_button->setObjectName("DownButton");
    down_button->setToolTip(tr("Move down"));
    connect(down_button, &QPushButton::clicked, this, &ActiveLayersWidget::moveSelectedLayerDown);
    left_column_layout->addWidget(down_button, 3, 0);
    QPushButton *remove_button = new QPushButton();
    remove_button->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton));
    remove_button->setObjectName("RemoveButton");
    remove_button->setToolTip(tr("Remove selected"));
    connect(remove_button, &QPushButton::clicked, this, &ActiveLayersWidget::removeSelectedLayer);
    left_column_layout->addWidget(remove_button, 4, 0);
    QPushButton *clear_button = new QPushButton();
    clear_button->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton));
    clear_button->setToolTip(tr("Clear all"));
    connect(clear_button, &QPushButton::clicked, this, &ActiveLayersWidget::clearLayers);
    left_column_layout->addWidget(clear_button, 5, 0);

    // Build the table containing enabled layers
    left_column_layout->addWidget(new QLabel(tr("Enabled Layers")), 0, 1);
    QLabel *application_label = new QLabel(tr("Application Side"));
    QFont application_font = application_label->font();
    application_font.setBold(true);
    application_label->setFont(application_font);
    left_column_layout->addWidget(application_label, 1, 1, Qt::AlignCenter);
    enabled_layer_list = new QListWidget();
    enabled_layer_list->setSelectionMode(QListWidget::ExtendedSelection);
    left_column_layout->addWidget(enabled_layer_list, 2, 1, 5, 1);
    left_column_layout->setRowStretch(6, 1);
    QLabel *driver_label = new QLabel(tr("Driver Side"));
    QFont driver_font = driver_label->font();
    driver_font.setBold(true);
    driver_label->setFont(driver_font);
    left_column_layout->addWidget(driver_label, 7, 1, Qt::AlignCenter);
    column_layout->addLayout(left_column_layout);

    // Build the disabled layers
    QGridLayout *right_column_layout = new QGridLayout();
    right_column_layout->addWidget(new QLabel(tr("Disabled Explicit Layers")), 0, 1);
    QPushButton *add_explicit_button = new QPushButton();
    add_explicit_button->setObjectName("LeftButton");
    add_explicit_button->setToolTip(tr("Enable selected explcit layers"));
    add_explicit_button->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowLeft));
    connect(add_explicit_button, &QPushButton::clicked, this, &ActiveLayersWidget::enableSelectedExplicitLayer);
    right_column_layout->addWidget(add_explicit_button, 1, 0);
    QListWidget *explicit_list = new QListWidget();
    explicit_list->setSelectionMode(QListWidget::ExtendedSelection);
    right_column_layout->addWidget(explicit_list, 1, 1);
    layer_lists[LayerType::Explicit] = explicit_list;
    right_column_layout->addWidget(new QLabel(tr("Disabled Implicit Layers")), 2, 1);
    QPushButton *add_implicit_button = new QPushButton();
    add_implicit_button->setObjectName("LeftButton");
    add_implicit_button->setToolTip(tr("Enable selected implicit layers"));
    add_implicit_button->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowLeft));
    connect(add_implicit_button, &QPushButton::clicked, this, &ActiveLayersWidget::enableSelectedImplicitLayer);
    right_column_layout->addWidget(add_implicit_button, 3, 0);
    QListWidget *implicit_list = new QListWidget();
    implicit_list->setSelectionMode(QListWidget::ExtendedSelection);
    right_column_layout->addWidget(implicit_list, 3, 1);
    layer_lists[LayerType::Implicit] = implicit_list;
    column_layout->addLayout(right_column_layout);

    layer_layout->addLayout(column_layout);

    toggleExpiration(cleanup_box->checkState());

    setLayout(layer_layout);
}

int ActiveLayersWidget::expiration() const
{
    QTime duration;
    switch(expirationUnit()) {

    case DurationUnit::Minute:
        return 60 * expiration_spin->value();
    case DurationUnit::Hour:
        return 3600 * expiration_spin->value();
    case DurationUnit::Day:
        return 86400 * expiration_spin->value();
    default:
        return -1;
    }
}

DurationUnit ActiveLayersWidget::expirationUnit() const
{
    switch(expiration_units->currentIndex()) {

    case 0:
        return DurationUnit::Minute;
    case 1:
        return DurationUnit::Hour;
    case 2:
        return DurationUnit::Day;
    default:
        return DurationUnit::None;
    }
}

void ActiveLayersWidget::setEnabledLayers(const QList<QString> &layers)
{
    bool signal_state = blockSignals(true);
    clearLayers();
    blockSignals(signal_state);

    for (const QString &layer : layers) {
        for (int i = 0; i < disabled_layers.length(); ++i) {
            LayerManifest manifest = disabled_layers[i];
            if (layer == manifest.name) {

                // Remove the layer from diabled lists
                disabled_layers.removeAt(i);
                if (i < layer_lists[LayerType::Explicit]->count()) {
                    delete layer_lists[LayerType::Explicit]->takeItem(i);
                } else {
                    delete layer_lists[LayerType::Implicit]->takeItem(i - layer_lists[LayerType::Implicit]->count());
                }

                // Add the layer to enabled lists
                enabled_layers.append(manifest);
                QListWidgetItem *item = new QListWidgetItem(manifest.PrettyName());
                item->setIcon(layer_icons[manifest.type]);
                enabled_layer_list->addItem(item);
            }
        }
    }
}

void ActiveLayersWidget::setExpiration(int seconds, DurationUnit unit)
{
    switch(unit) {

    case DurationUnit::Minute:
        expiration_spin->setValue(seconds / 60);
        expiration_units->setCurrentIndex(0);
        break;
    case DurationUnit::Hour:
        expiration_spin->setValue(seconds / 3600);
        expiration_units->setCurrentIndex(1);
        break;
    case DurationUnit::Day:
        expiration_spin->setValue(seconds / 86400);
        expiration_units->setCurrentIndex(2);
        break;
    default:
        expiration_spin->setValue(0);
        expiration_units->setCurrentIndex(3);
        break;
    }
    setExpirationEnabled(expiration_units->currentText());
}

bool ActiveLayersWidget::shouldClearOnClose()
{
    return cleanup_box->isChecked();
}

void ActiveLayersWidget::clearLayers()
{
    while (!enabled_layers.isEmpty()) {
        LayerManifest manifest = enabled_layers.first();
        QListWidgetItem *item = enabled_layer_list->item(0);
        QListWidget *explicit_widget = layer_lists[LayerType::Explicit];
        QListWidget *implicit_widget = layer_lists[LayerType::Implicit];
        bool found = false;
        switch (manifest.type) {

        case LayerType::Explicit:
            for (int i = 0; i < explicit_widget->count(); ++i) {
                if (manifest < disabled_layers[i]) {
                    explicit_widget->insertItem(i, item->text());
                    disabled_layers.insert(i, manifest);
                    found = true;
                    break;
                }
            }
            if (!found) {
                explicit_widget->addItem(item->text());
                disabled_layers.insert(explicit_widget->count(), manifest);
            }
            break;
        case LayerType::Implicit:
            for (int i = explicit_widget->count(); i <  disabled_layers.count(); ++i) {
                if (manifest < disabled_layers[i]) {
                    implicit_widget->insertItem(i - explicit_widget->count(), item->text());
                    disabled_layers.insert(i, manifest);
                    found = true;
                    break;
                }
            }
            if (!found) {
                implicit_widget->addItem(item->text());
                disabled_layers.append(manifest);
            }
            break;
        }

        delete enabled_layer_list->takeItem(0);
        enabled_layers.removeFirst();
    }

    emit enabledLayersUpdated(enabled_layers, disabled_layers);
}

void ActiveLayersWidget::setExpirationEnabled(const QString &text)
{
    expiration_spin->setEnabled(!cleanup_box->isChecked() && text != tr("Never"));
}

void ActiveLayersWidget::updateAvailableLayers(const QList<QPair<QString, LayerType>> &path_list, bool is_custom)
{
    layer_paths = path_list;
    custom_paths = is_custom;
    refreshAvailableLayers();
}

void ActiveLayersWidget::enableSelectedExplicitLayer()
{
    QListWidget *explicit_list = layer_lists[LayerType::Explicit];
    for (QListWidgetItem *item : explicit_list->selectedItems()) {
        int row = explicit_list->row(item);

        QListWidgetItem *enabled_item = new QListWidgetItem(item->text());
        enabled_item->setIcon(layer_icons[LayerType::Explicit]);
        enabled_layer_list->addItem(enabled_item);
        enabled_layers.append(disabled_layers[row]);

        delete explicit_list->takeItem(row);
        disabled_layers.removeAt(row);
    }

    emit enabledLayersUpdated(enabled_layers, disabled_layers);
}

void ActiveLayersWidget::enableSelectedImplicitLayer()
{
    int disabled_offset = layer_lists[LayerType::Explicit]->count();
    QListWidget *implicit_list = layer_lists[LayerType::Implicit];
    for (QListWidgetItem *item : implicit_list->selectedItems()) {
        int row = implicit_list->row(item);

        QListWidgetItem *enabled_item = new QListWidgetItem(item->text());
        enabled_item->setIcon(layer_icons[LayerType::Implicit]);
        enabled_layer_list->addItem(enabled_item);
        enabled_layers.append(disabled_layers[row + disabled_offset]);

        delete implicit_list->takeItem(row);
        disabled_layers.removeAt(row + disabled_offset);
    }

    emit enabledLayersUpdated(enabled_layers, disabled_layers);
}

void ActiveLayersWidget::moveSelectedLayerDown()
{
    for (int row = enabled_layer_list->count() - 1; row >= 0; --row) {
        QListWidgetItem *item = enabled_layer_list->item(row);
        if (item->isSelected() && row < enabled_layer_list->count() - 1 && !enabled_layer_list->item(row + 1)->isSelected()) {
            enabled_layer_list->insertItem(row + 1, enabled_layer_list->takeItem(row));
            enabled_layers.insert(row + 1, enabled_layers.takeAt(row));
            item->setSelected(true);
        }
    }

    emit enabledLayersUpdated(enabled_layers, disabled_layers);
}

void ActiveLayersWidget::moveSelectedLayerUp()
{
    for (int row = 0; row < enabled_layer_list->count(); ++row) {
        QListWidgetItem *item = enabled_layer_list->item(row);
        if (item->isSelected() && row > 0 && !enabled_layer_list->item(row - 1)->isSelected()) {
            enabled_layer_list->insertItem(row - 1, enabled_layer_list->takeItem(row));
            enabled_layers.insert(row - 1, enabled_layers.takeAt(row));
            item->setSelected(true);
        }
    }

    emit enabledLayersUpdated(enabled_layers, disabled_layers);
}

void ActiveLayersWidget::refreshAvailableLayers()
{
    for(QListWidget *list_widget : layer_lists) {
        list_widget->clear();
    }

    QList<LayerManifest> explicit_layers;
    QList<LayerManifest> implicit_layers;
    for (auto pair : layer_paths)
    {
        QList<LayerManifest> *layers = pair.second == LayerType::Explicit ? &explicit_layers : &implicit_layers;
#if defined(_WIN32)
        if (custom_paths) {
            layers->append(LayerManifest::LoadDirectory(QDir(pair.first), pair.second));
        } else {
            layers->append(LayerManifest::LoadRegistry(pair.first, pair.second));
        }
#else
        layers->append(LayerManifest::LoadDirectory(QDir(pair.first), pair.second));
 #endif
    }
    qSort(explicit_layers);
    qSort(implicit_layers);

    enabled_layer_list->clear();
    enabled_layers.clear();
    disabled_layers = explicit_layers + implicit_layers;

    for (LayerManifest &manifest : disabled_layers) {
        QListWidgetItem *item = new QListWidgetItem(manifest.PrettyName());
        item->setToolTip(manifest.description);
        layer_lists[manifest.type]->addItem(item);
    }

    emit enabledLayersUpdated(enabled_layers, disabled_layers);
}

void ActiveLayersWidget::removeSelectedLayer()
{
    for (QListWidgetItem *item : enabled_layer_list->selectedItems()) {
        bool found = false; 
        int row = enabled_layer_list->row(item);
        QListWidget *explicit_widget = layer_lists[LayerType::Explicit];
        QListWidget *implicit_widget = layer_lists[LayerType::Implicit];
        switch (enabled_layers[row].type) {

        case LayerType::Explicit:
            for (int i = 0; i < explicit_widget->count(); ++i) {
                if (enabled_layers[row] < disabled_layers[i]) {
                    explicit_widget->insertItem(i, item->text());
                    disabled_layers.insert(i, enabled_layers[row]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                explicit_widget->addItem(item->text());
                disabled_layers.insert(explicit_widget->count(), enabled_layers[row]);
            }
            break;
        case LayerType::Implicit:
            for (int i = explicit_widget->count(); i <  disabled_layers.count(); ++i) {
                if (enabled_layers[row] < disabled_layers[i]) {
                    implicit_widget->insertItem(i - explicit_widget->count(), item->text());
                    disabled_layers.insert(i, enabled_layers[row]);
                    found = true;
                    break;
                }
            }
            if (!found) {
                implicit_widget->addItem(item->text());
                disabled_layers.append(enabled_layers[row]);
            }
            break;
        }

        delete enabled_layer_list->takeItem(row);
        enabled_layers.removeAt(row);
    }

    emit enabledLayersUpdated(enabled_layers, disabled_layers);
}

void ActiveLayersWidget::toggleExpiration(int state)
{
    bool enabled = state != Qt::Checked;
    bool box_enabled = expiration_units->currentText() != tr("Never");
    expiration_label->setEnabled(enabled);
    expiration_spin->setEnabled(enabled && box_enabled);
    expiration_units->setEnabled(enabled);
}
