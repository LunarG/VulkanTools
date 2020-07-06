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
 * Search with completer for VUID's
 *
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include "vuidsearchwidget.h"
#include "vk_vuids.h"

VUIDSearchWidget::VUIDSearchWidget(QWidget *parent) : QWidget(parent) {
    int nNumElements = sizeof(vuids) / sizeof(vuids[0]);
    for (int i = 0; i < nNumElements; i++) list << vuids[i];

    user_box_ = new QComboBox(this);
    user_box_->setEditable(true);
    user_box_->setFocusPolicy(Qt::StrongFocus);
    user_box_->addItems(list);

    search_vuid_ = new QCompleter(list, this);
    search_vuid_->setCaseSensitivity(Qt::CaseSensitive);

    search_vuid_->setCompletionMode(QCompleter::PopupCompletion);
    search_vuid_->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    search_vuid_->setFilterMode(Qt::MatchContains);
    search_vuid_->setMaxVisibleItems(15);
    search_vuid_->setCaseSensitivity(Qt::CaseInsensitive);

    user_box_->setCompleter(search_vuid_);
    user_box_->show();
    user_box_->setCurrentText("");

    user_box_->installEventFilter(this);

    connect(user_box_, SIGNAL(currentIndexChanged(int)), this, SLOT(itemSelected(int)));
}

void VUIDSearchWidget::resizeEvent(QResizeEvent *event) {
    QSize parentSize = event->size();
    user_box_->setGeometry(0, 0, parentSize.width() - 2, parentSize.height());
}

///////////////////////////////////////////////////////////////////////
// Emit a signal so we can send this to the list box
void VUIDSearchWidget::itemSelected(int nIndex) {
    emit itemSelected(list[nIndex]);
    user_box_->setCurrentText("");
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
