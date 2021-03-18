/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "widget_setting_search.h"
#include "widget_setting.h"

#include "../vkconfig_core/util.h"

#include <QMessageBox>

#include <cassert>

WidgetSettingSearch::WidgetSettingSearch(QTreeWidget *tree, QTreeWidgetItem *item, const SettingMetaList &meta,
                                         SettingDataList &data)
    : meta(meta),
      data(data),
      item(item),
      search(nullptr),
      field(new QLineEdit(this)),
      add_button(new QPushButton(this)),
      list(meta.list) {
    assert(tree != nullptr);
    assert(item != nullptr);
    assert(&meta);
    assert(&data);

    std::sort(this->list.begin(), this->list.end());
    for (std::size_t i = 0, n = data.values.size(); i < n; ++i) {
        RemoveString(this->list, data.values[i].key.c_str());
    }

    this->setFont(tree->font());

    item->setText(0, meta.label.c_str());
    item->setFont(0, tree->font());
    item->setToolTip(0, meta.description.c_str());
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    const std::string tooltip = meta.list_only && this->list.empty() ? "All the accepted values are already listed"
                                                                     : "Start tapping to search for available values";

    this->field->show();
    this->field->setText("");
    this->field->setToolTip(tooltip.c_str());
    this->field->setFont(tree->font());
    this->field->setFocusPolicy(Qt::StrongFocus);
    this->field->installEventFilter(this);
    this->field->setEnabled(!this->list.empty());

    this->connect(this->field, SIGNAL(itemSelected(const QString &)), this, SLOT(OnItemSelected(const QString &)));
    this->connect(this->field, SIGNAL(textEdited(const QString &)), this, SLOT(OnTextEdited(const QString &)),
                  Qt::QueuedConnection);

    this->add_button->setText("+");
    this->add_button->setFont(tree->font());
    this->add_button->setEnabled(!this->field->text().isEmpty());

    this->connect(this->add_button, SIGNAL(pressed()), this, SLOT(OnButtonPressed()));

    ResetCompleter();
}

void WidgetSettingSearch::resizeEvent(QResizeEvent *event) {
    const int button_size = MIN_BUTTON_SIZE;
    const QSize parent_size = event->size();

    const QFontMetrics fm = this->fontMetrics();
    const int text_width = HorizontalAdvance(fm, QString(meta.label.c_str()) + "00");

    this->field->setGeometry(text_width, 0, parent_size.width() - button_size - text_width, parent_size.height());
    this->add_button->setGeometry(parent_size.width() - button_size, 0, button_size, parent_size.height());
}

/// Reload the completer with a revised list of VUID's.
/// I'm quite impressed with how fast this brute force implementation
/// runs in release mode.
void WidgetSettingSearch::ResetCompleter() {
    if (this->search != nullptr) this->search->deleteLater();

    // ConvertString(this->list);

    this->search = new QCompleter(QStringList(ConvertString(list)), this);
    this->search->setCaseSensitivity(Qt::CaseSensitive);
    this->search->setCompletionMode(QCompleter::PopupCompletion);
    this->search->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    this->search->setFilterMode(Qt::MatchContains);
    this->search->setMaxVisibleItems(15);
    this->search->setCaseSensitivity(Qt::CaseInsensitive);

    this->field->setCompleter(search);

    this->connect(this->search, SIGNAL(activated(const QString &)), this, SLOT(OnAddCompleted(const QString &)),
                  Qt::QueuedConnection);
}

// Clear the edit control after the completer is finished.
void WidgetSettingSearch::OnAddCompleted(const QString &added_value) {
    (void)added_value;
    // We can't do this right away, the completer emits it's signal
    // before it's really "complete". If we clear the control too soon
    // it clears the completers value too. This might be a Qt bug, but this
    // works really well as a work-a-round
    OnButtonPressed();
}

void WidgetSettingSearch::OnButtonPressed() {
    QString entry = this->field->text();
    if (entry.isEmpty()) return;

    emit itemSelected(entry);  // Triggers update of GUI
    emit itemChanged();        // Triggers save of configuration

    this->field->setText("");

    if (meta.list_only && IsStringFound(this->meta.list, entry.toStdString())) {
        QMessageBox alert;
        alert.setWindowTitle("Invalid value");
        alert.setText(
            format("'%s' setting doesn't accept '%s' as a value", this->meta.label.c_str(), entry.toStdString().c_str()).c_str());
        alert.setInformativeText("Please select a value from the list.");
        alert.setIcon(QMessageBox::Warning);
        alert.exec();
        return;
    }

    EnabledString enabled_string;
    enabled_string.key = entry.toStdString();
    enabled_string.enabled = true;
    this->data.values.push_back(enabled_string);

    RemoveString(this->list, entry.toStdString());
    ResetCompleter();
}

void WidgetSettingSearch::OnItemSelected(const QString &value) {
    QTreeWidgetItem *child = new QTreeWidgetItem();
    child->setText(0, value);
    child->setFont(0, this->item->font(0));
    child->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    child->setToolTip(0, value);
    child->setCheckState(0, Qt::Checked);
    this->item->addChild(child);
}

void WidgetSettingSearch::OnTextEdited(const QString &value) {
    assert(this->add_button);
    assert(this->field);

    this->add_button->setEnabled(!value.isEmpty());
}

// Ignore mouse wheel events in combo box, otherwise, it fills the list box with ID's
bool WidgetSettingSearch::eventFilter(QObject *target, QEvent *event) {
    (void)target;
    if (event->type() == QEvent::Wheel) {
        event->ignore();
        return true;
    }

    return field->eventFilter(target, event);
}
