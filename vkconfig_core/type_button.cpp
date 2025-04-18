/*
 * Copyright (c) 2020-2025 Valve Corporation
 * Copyright (c) 2020-2025 LunarG, Inc.
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

#include "type_button.h"

#include <array>
#include <cstring>

const char* GetToken(ButtonType value) {
    static const char* TABLE[] = {
        "OK",       // BUTTON_OK
        "YES",      // BUTTON_YES
        "NO",       // BUTTON_NO
        "ABORT",    // BUTTON_ABORT
        "IGNORE",   // BUTTON_IGNORE
        "CANCEL",   // BUTTON_CANCEL
        "DISCARD",  // BUTTON_DISCARD
    };
    static_assert(std::size(TABLE) == BUTTON_COUNT, "The tranlation table size doesn't match the enum number of elements");

    if (value >= BUTTON_FIRST && value <= BUTTON_LAST) {
        return TABLE[value - BUTTON_FIRST];
    } else {
        return nullptr;
    }
}

ButtonType GetButtonType(const char* token) {
    for (std::size_t i = 0, n = BUTTON_COUNT; i < n; ++i) {
        const ButtonType value = static_cast<ButtonType>(i);
        if (std::strcmp(::GetToken(value), token) == 0) {
            return value;
        }
    }

    return BUTTON_OK;
}

QMessageBox::StandardButton GetStandardButton(ButtonType type) {
    static QMessageBox::StandardButton TABLE[] = {
        QMessageBox::Ok,       // BUTTON_OK
        QMessageBox::Yes,      // BUTTON_YES
        QMessageBox::No,       // BUTTON_NO
        QMessageBox::Abort,    // BUTTON_ABORT
        QMessageBox::Ignore,   // BUTTON_IGNORE
        QMessageBox::Cancel,   // BUTTON_CANCEL
        QMessageBox::Discard,  // BUTTON_DISCARD
    };
    static_assert(std::size(TABLE) == BUTTON_COUNT, "The tranlation table size doesn't match the enum number of elements");

    return TABLE[type - BUTTON_FIRST];
}
