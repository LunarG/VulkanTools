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

#include "vuidsearchwidget.h"
#include "vk_vuids.h"

VUIDSearchWidget::VUIDSearchWidget(QWidget *parent) : QWidget(parent) {
    int nNumElements = sizeof(vuids) / sizeof(vuids[0]);
    for (int i = 0; i < nNumElements; i++) vuid_list_ << vuids[i];

    // Completer does not need this sorted, but we do
    vuid_list_.sort();

    user_box_ = new QLineEdit(this);
    user_box_->setFocusPolicy(Qt::StrongFocus);

    add_button_ = new QPushButton(this);
    add_button_->setText("Add");

    user_box_->show();
    user_box_->setText("");
    user_box_->installEventFilter(this);

    search_vuid_ = nullptr;  // Safe to delete a pointer
    ResetCompleter();

    connect(add_button_, SIGNAL(pressed()), this, SLOT(addButtonPressed()));
    connect(user_box_, SIGNAL(returnPressed()), this, SLOT(addButtonPressed()));
}

void VUIDSearchWidget::resizeEvent(QResizeEvent *event) {
    const int button_size = 52;
    QSize parentSize = event->size();
    user_box_->setGeometry(0, 0, parentSize.width() - 2 - button_size, parentSize.height());
    add_button_->setGeometry(parentSize.width() - button_size, 0, button_size, parentSize.height());
}

/////////////////////////////////////////////////////////////////////
/// Reload the completer with a revised list of VUID's.
void VUIDSearchWidget::ResetCompleter(void) {
    if (search_vuid_ != nullptr) search_vuid_->deleteLater();

    search_vuid_ = new QCompleter(vuid_list_, this);
    search_vuid_->setCaseSensitivity(Qt::CaseSensitive);
    search_vuid_->setCompletionMode(QCompleter::PopupCompletion);
    search_vuid_->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    search_vuid_->setFilterMode(Qt::MatchContains);
    search_vuid_->setMaxVisibleItems(15);
    search_vuid_->setCaseSensitivity(Qt::CaseInsensitive);
    user_box_->setCompleter(search_vuid_);
    connect(search_vuid_, SIGNAL(activated(const QString &)), this, SLOT(addCompleted(const QString &)), Qt::QueuedConnection);
}

///////////////////////////////////////////////////////////////////////
// Add the text in the edit control to the list, and clear the control
void VUIDSearchWidget::addButtonPressed(void) {
    QString entry = user_box_->text();
    if (entry.isEmpty()) return;

    emit itemSelected(entry);  // Triggers update of GUI
    emit itemChanged();        // Triggers save of profile
    user_box_->setText("");
}

//////////////////////////////////////////////////////////////////////
// Clear the edit control after the completer is finished.
void VUIDSearchWidget::addCompleted(const QString &addedItem) {
    (void)addedItem;
    user_box_->setText("");
}

// Ignore mouse wheel events in combo box, otherwise, it fills the list box with ID's
bool VUIDSearchWidget::eventFilter(QObject *target, QEvent *event) {
    (void)target;
    if (event->type() == QEvent::Wheel) {
        event->ignore();
        return true;
    }

    return user_box_->eventFilter(target, event);
}
