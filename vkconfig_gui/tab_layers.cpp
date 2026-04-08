/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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

#include "widget_tab_layers_path.h"
#include "tab_layers.h"
#include "mainwindow.h"
#include "style.h"

#include "../vkconfig_core/configurator.h"
#include "../vkconfig_core/type_hide_message.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>
#include <QShortcut>

#include <chrono>
#include <thread>

TabLayers::TabLayers(MainWindow &window, std::shared_ptr<Ui::MainWindow> ui) : Tab(TAB_LAYERS_PATHS, window, ui) {
    Configurator &configurator = Configurator::Get();

    this->ui->layers_progress->setValue(0);
    this->ui->layers_progress->setVisible(false);
    this->ui->layers_path_lineedit->setVisible(true);
    this->ui->layers_search_clear->setEnabled(false);

    this->connect(this->ui->layers_browse_button, SIGNAL(clicked()), this, SLOT(on_layers_browse_pressed()));
    this->connect(this->ui->layers_reload_button, SIGNAL(clicked()), this, SLOT(on_layers_reload_pressed()));
    this->connect(this->ui->layers_path_lineedit, SIGNAL(editingFinished()), this, SLOT(on_layers_append_pressed()));
    this->connect(this->ui->layers_path_lineedit, SIGNAL(returnPressed()), this, SLOT(on_layers_append_pressed()));

    this->connect(this->ui->layers_search, SIGNAL(textEdited(QString)), this, SLOT(on_search_textEdited(QString)));
    this->connect(this->ui->layers_search_clear, SIGNAL(clicked()), this, SLOT(on_search_clear_pressed()));

    QShortcut *shortcut_search = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_F), this->ui->layers_list);
    this->connect(shortcut_search, SIGNAL(activated()), this, SLOT(on_focus_search()));
    QShortcut *shortcut_browse = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this->ui->layers_list);
    this->connect(shortcut_browse, SIGNAL(activated()), this, SLOT(on_layers_browse_pressed()));
    QShortcut *shortcut_reload = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_R), this->ui->layers_list);
    this->connect(shortcut_reload, SIGNAL(activated()), this, SLOT(on_layers_reload_pressed()));

    this->ui->layers_list->installEventFilter(&window);
}

TabLayers::~TabLayers() {}

void TabLayers::UpdateUI_LayersPaths(UpdateUIMode ui_update_mode) {
    (void)ui_update_mode;

    Configurator &configurator = Configurator::Get();

    this->ui->layers_list->blockSignals(true);

    switch (ui_update_mode) {
        case UPDATE_REBUILD_UI: {
            this->ui->layers_list->clear();

            const std::set<LayerDisplay> &layer_display_list = configurator.layers.BuildLayerDisplayList();

            for (auto it = layer_display_list.begin(), end = layer_display_list.end(); it != end; ++it) {
                const Layer *layer = configurator.layers.Find(it->id, false);
                if (layer == nullptr) {
                    continue;
                }

                if (!this->layer_filter.empty()) {
                    const std::string status = layer->status == STATUS_STABLE ? "" : format(" (%s)", ::GetToken(layer->status));
                    const std::string text = format("%s - %s%s - %s layer", layer->key.c_str(), layer->api_version.str().c_str(),
                                                    status.c_str(), ::GetToken(layer->type));

                    std::string lower_text = ::ToLowerCase(text);
                    std::string layer_filter_search = ::ToLowerCase(this->layer_filter);
                    if (lower_text.find(layer_filter_search.c_str()) == std::string::npos) {
                        continue;
                    }
                }

                QListWidgetItem *item = new QListWidgetItem;
                item->setFlags(item->flags() | Qt::ItemIsSelectable);
                item->setSizeHint(QSize(0, ITEM_HEIGHT));

                LayerWidget *layer_widget = new LayerWidget(*layer);
                this->connect(layer_widget, SIGNAL(itemChanged()), this, SLOT(on_paths_changed()));
                this->connect(layer_widget, SIGNAL(itemToggled()), this, SLOT(on_paths_toggled()));

                this->ui->layers_list->addItem(item);
                this->ui->layers_list->setItemWidget(item, layer_widget);
            }
        } break;
        default: {
        } break;
    }

    this->ui->layers_list->blockSignals(false);
}

void TabLayers::UpdateUI(UpdateUIMode ui_update_mode) {
    const Configurator &configurator = Configurator::Get();

    this->ui->layers_search_clear->setEnabled(!this->ui->layers_search->text().isEmpty());
    this->ui->layers_search->setFocus();
    this->ui->layers_progress->resetFormat();
    this->ui->layers_progress->setValue(0);

    this->UpdateUI_LayersPaths(ui_update_mode);
}

void TabLayers::CleanUI() {}

bool TabLayers::EventFilter(QObject *target, QEvent *event) {
    (void)target;
    (void)event;

    return false;
}

void TabLayers::on_paths_changed() {
    Configurator &configurator = Configurator::Get();

    configurator.UpdateConfigurations();
    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);
}

void TabLayers::on_paths_toggled() {
    Configurator &configurator = Configurator::Get();

    configurator.UpdateConfigurations();
    configurator.Override(OVERRIDE_AREA_ALL);

    this->UpdateUI_LayersPaths(UPDATE_REFRESH_UI);
}

void TabLayers::on_layers_append_pressed() { this->LoadLayersManifest(this->ui->layers_path_lineedit->text()); }

void TabLayers::on_layers_browse_pressed() {
    Configurator &configurator = Configurator::Get();

    const QString selected_path =
        QFileDialog::getExistingDirectory(this->ui->layers_browse_button, "Select Layer Manifests Folder...",
                                          configurator.layers.last_layers_dir.AbsolutePath().c_str());

    this->LoadLayersManifest(selected_path);
}

void TabLayers::on_focus_search() { this->ui->layers_search->setFocus(); }

void TabLayers::on_search_textEdited(const QString &text) {
    this->layer_filter = text.toStdString();

    this->ui->layers_search_clear->setEnabled(!text.isEmpty());

    this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);
}

void TabLayers::on_search_clear_pressed() {
    this->layer_filter.clear();

    this->ui->layers_search->clear();
    this->ui->layers_search_clear->setEnabled(false);

    this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);
}

void TabLayers::on_layers_reload_pressed() {
    this->ui->layers_search->setVisible(false);
    this->ui->layers_search_clear->setVisible(false);
    this->ui->layers_path_lineedit->setVisible(false);
    this->ui->layers_browse_button->setVisible(false);
    this->ui->layers_progress->setVisible(true);

    Path new_path = this->ui->layers_path_lineedit->text().toStdString();

    Configurator &configurator = Configurator::Get();

    std::vector<int> layers_count(LAYER_LOAD_COUNT, 0);

    const std::vector<Path> &layers_paths = ::CollectLayersPaths(new_path);

    this->ui->layers_progress->setMaximum(static_cast<int>(layers_paths.size()));
    this->ui->layers_progress->setValue(0);

    for (std::size_t i = 0, n = layers_paths.size(); i < n; ++i) {
        const char *label = configurator.layers.validate_manifests ? "Validating" : "Loading";

        this->ui->layers_progress->setFormat(
            format("%s %s... - %d/%d files", label, layers_paths[i].AbsolutePath().c_str(), i + 1, layers_paths.size()).c_str());
        this->ui->layers_progress->setValue(static_cast<int>(i + 1));
        this->ui->layers_progress->update();

        LayerLoadStatus status = configurator.layers.LoadLayers(layers_paths[i], LAYER_TYPE_EXPLICIT, configurator.mode);
        ++layers_count[status];

        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::milliseconds(10));
    }

    configurator.UpdateConfigurations();
    configurator.Override(OVERRIDE_AREA_ALL);

    std::string last_layers_path = configurator.layers.last_layers_dir.AbsolutePath();

    if (!configurator.Get(HIDE_MESSAGE_NOTIFICATION_LAYERS_LOADED)) {
        std::string text = "Loading and reloading all located layers manifests:\n";
        text += format(" - %d layer manifests added.\n", layers_count[LAYER_LOAD_ADDED]);
        text += format(" - %d layer manifests reloaded.\n", layers_count[LAYER_LOAD_RELOADED]);
        text += format(" - %d layer manifests unmodified.", layers_count[LAYER_LOAD_UNMODIFIED]);

        QMessageBox message;
        message.setIcon(QMessageBox::Information);
        message.setWindowTitle("Loading Layers Completed.");
        message.setText(text.c_str());
        if (!new_path.Empty()) {
            message.setInformativeText(format("From '%s' folder.", new_path.AbsolutePath().c_str()).c_str());
        }
        message.setCheckBox(new QCheckBox("Do not show again."));
        message.exec();
        if (message.checkBox()->isChecked()) {
            configurator.Set(HIDE_MESSAGE_NOTIFICATION_LAYERS_LOADED);
        }
    }

    this->ui->layers_search->setVisible(true);
    this->ui->layers_search_clear->setVisible(true);
    this->ui->layers_path_lineedit->setVisible(true);
    this->ui->layers_browse_button->setVisible(true);
    this->ui->layers_progress->setVisible(false);
}

void TabLayers::LoadLayersManifest(const QString &selected_path) {
    Configurator &configurator = Configurator::Get();

    if (!selected_path.isEmpty()) {
        configurator.layers.last_layers_dir = selected_path.toStdString();
        if (!configurator.layers.last_layers_dir.Empty()) {
            this->ui->layers_path_lineedit->setText(configurator.layers.last_layers_dir.AbsolutePath().c_str());
        }

        configurator.layers.gui_added_layers_paths.insert(configurator.layers.last_layers_dir);

        this->on_layers_reload_pressed();

        this->UpdateUI_LayersPaths(UPDATE_REBUILD_UI);
    }
}
