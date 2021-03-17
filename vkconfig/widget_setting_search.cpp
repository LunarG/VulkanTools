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

    this->setFont(tree->font());

    item->setText(0, meta.label.c_str());
    item->setFont(0, tree->font());
    item->setToolTip(0, meta.description.c_str());
    item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    // std::sort(this->list.begin(), this->list.end());
    for (std::size_t i = 0, n = data.value.size(); i < n; ++i) {
        // RemoveString(this->list, data.value[i].key.c_str());
    }

    field->setFocusPolicy(Qt::StrongFocus);

    add_button->setText("+");
    add_button->setFont(tree->font());

    field->show();
    field->setText("");
    field->setFont(tree->font());
    field->installEventFilter(this);

    ResetCompleter();

    connect(add_button, SIGNAL(pressed()), this, SLOT(addButtonPressed()));
}

void WidgetSettingSearch::resizeEvent(QResizeEvent *event) {
    const int button_size = MIN_BUTTON_SIZE;
    const QSize parent_size = event->size();

    const QFontMetrics fm = this->fontMetrics();
    const int text_width = HorizontalAdvance(fm, QString(meta.label.c_str()) + "00");

    field->setGeometry(text_width, 0, parent_size.width() - button_size - text_width, parent_size.height());
    add_button->setGeometry(parent_size.width() - button_size, 0, button_size, parent_size.height());
}

/// Reload the completer with a revised list of VUID's.
/// I'm quite impressed with how fast this brute force implementation
/// runs in release mode.
void WidgetSettingSearch::ResetCompleter() {
    if (search != nullptr) search->deleteLater();

    // ConvertString(this->list);

    search = new QCompleter(QStringList(), this);
    search->setCaseSensitivity(Qt::CaseSensitive);
    search->setCompletionMode(QCompleter::PopupCompletion);
    search->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    search->setFilterMode(Qt::MatchContains);
    search->setMaxVisibleItems(15);
    search->setCaseSensitivity(Qt::CaseInsensitive);
    field->setCompleter(search);

    connect(search, SIGNAL(activated(const QString &)), this, SLOT(addCompleted(const QString &)), Qt::QueuedConnection);
}

void WidgetSettingSearch::AddChildItem(const char *label, bool checked) {
    QTreeWidgetItem *child = new QTreeWidgetItem();
    child->setText(0, label);
    child->setFont(0, item->font(0));
    child->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    child->setToolTip(0, label);
    child->setCheckState(0, checked ? Qt::Checked : Qt::Unchecked);
    item->addChild(child);
}

// Add the text in the edit control to the list, and clear the control
// This is not really used much, only if they want to add something
// that is not in the completer list.
void WidgetSettingSearch::addButtonPressed() {
    QString entry = field->text();
    if (entry.isEmpty()) return;

    emit itemSelected(entry);  // Triggers update of GUI
    emit itemChanged();        // Triggers save of configuration
    field->setText("");

    // Remove the just added item from the search list
    std::size_t ref_size = list.size();
    RemoveString(list, entry.toStdString());
    if (ref_size > list.size()) {
        ResetCompleter();
    }
}

// Clear the edit control after the completer is finished.
void WidgetSettingSearch::addCompleted(const QString &addedItem) {
    (void)addedItem;
    // We can't do this right away, the completer emits it's signal
    // before it's really "complete". If we clear the control too soon
    // it clears the completers value too. This might be a Qt bug, but this
    // works really well as a work-a-round
    addButtonPressed();
}

// Item was removed from master list, so add it back to the search
// list.
void WidgetSettingSearch::addToSearchList(const QString &value) {
    this->list.push_back(value.toStdString());
    std::sort(this->list.begin(), this->list.end());
    ResetCompleter();
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
