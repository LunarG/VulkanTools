/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
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

#include <QTimer>

#include "vuidsearchwidget.h"
#include "vk_vuids.h"

VUIDSearchWidget::VUIDSearchWidget(QWidget *parent) : QWidget(parent) {
    int nNumElements = sizeof(vuids) / sizeof(vuids[0]);
    for (int i = 0; i < nNumElements; i++) _vuid_list << vuids[i];

    // Completer does not need this sorted, but we do
    _vuid_list.sort();

    _user_box = new QLineEdit(this);
    _user_box->setFocusPolicy(Qt::StrongFocus);

    _add_button = new QPushButton(this);
    _add_button->setText("Add");

    _user_box->show();
    _user_box->setText("");
    _user_box->installEventFilter(this);

    _search_vuid = nullptr;  // Safe to delete a pointer
    ResetCompleter();

    connect(_add_button, SIGNAL(pressed()), this, SLOT(addButtonPressed()));
}

void VUIDSearchWidget::resizeEvent(QResizeEvent *event) {
    const int button_size = 52;
    QSize parentSize = event->size();
    _user_box->setGeometry(0, 0, parentSize.width() - 2 - button_size, parentSize.height());
    _add_button->setGeometry(parentSize.width() - button_size, 0, button_size, parentSize.height());
}

/////////////////////////////////////////////////////////////////////
/// Reload the completer with a revised list of VUID's.
void VUIDSearchWidget::ResetCompleter(void) {
    if (_search_vuid != nullptr) _search_vuid->deleteLater();

    _search_vuid = new QCompleter(_vuid_list, this);
    _search_vuid->setCaseSensitivity(Qt::CaseSensitive);
    _search_vuid->setCompletionMode(QCompleter::PopupCompletion);
    _search_vuid->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    _search_vuid->setFilterMode(Qt::MatchContains);
    _search_vuid->setMaxVisibleItems(15);
    _search_vuid->setCaseSensitivity(Qt::CaseInsensitive);
    _user_box->setCompleter(_search_vuid);
    connect(_search_vuid, SIGNAL(activated(const QString &)), this, SLOT(addCompleted(const QString &)), Qt::QueuedConnection);
}

///////////////////////////////////////////////////////////////////////
// Add the text in the edit control to the list, and clear the control
// This is not really used much, only if they want to add something
// that is not in the completer list.
void VUIDSearchWidget::addButtonPressed(void) {
    QString entry = _user_box->text();
    if (entry.isEmpty()) return;

    emit itemSelected(entry);  // Triggers update of GUI
    emit itemChanged();        // Triggers save of profile
    _user_box->setText("");
}

//////////////////////////////////////////////////////////////////////
// Clear the edit control after the completer is finished.
void VUIDSearchWidget::addCompleted(const QString &addedItem) {
    (void)addedItem;
    // We can't do this right away, the completer emits it's signal
    // before it's really "complete". If we clear the control too soon
    // it clears the completers value too. This might be a Qt bug, but this
    // works really well as a work-a-round
    addButtonPressed();
}

// Ignore mouse wheel events in combo box, otherwise, it fills the list box with ID's
bool VUIDSearchWidget::eventFilter(QObject *target, QEvent *event) {
    (void)target;
    if (event->type() == QEvent::Wheel) {
        event->ignore();
        return true;
    }

    return _user_box->eventFilter(target, event);
}
