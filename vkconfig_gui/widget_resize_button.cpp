/*
 * Copyright (c) 2020-2024 Valve Corporation
 * Copyright (c) 2020-2024 LunarG, Inc.
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

#include "widget_resize_button.h"

ResizeButton::ResizeButton(QWidget *parent) : QPushButton(parent), parent(parent) {}

bool ResizeButton::eventFilter(QObject *o, QEvent *e) {
    (void)o;

    if (e->type() == QEvent::Resize) {
        const QRect enabled_button_rect = QRect(this->parent->width() - 24 - 5, 0, 24, 24);
        this->setGeometry(enabled_button_rect);
    }
    return false;
}
