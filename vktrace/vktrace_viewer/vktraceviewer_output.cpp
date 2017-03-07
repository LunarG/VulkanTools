/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
 **************************************************************************/
#include "vktraceviewer_output.h"
#include <QTextEdit>

vktraceviewer_output gs_OUTPUT;

vktraceviewer_output::vktraceviewer_output() {}

vktraceviewer_output::~vktraceviewer_output() {}

void vktraceviewer_output::init(QTextBrowser* pTextEdit) { m_pTextEdit = pTextEdit; }

QString vktraceviewer_output::convertToHtml(QString message) {
    QString result;
    if (message.endsWith("\n")) {
        message.chop(1);
    }
    result = message.replace("\n", "<br>");
    return result;
}

void vktraceviewer_output::moveCursorToEnd() {
    QTextCursor cursor = m_pTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    m_pTextEdit->setTextCursor(cursor);
}

void vktraceviewer_output::message(uint64_t packetIndex, const QString& message) {
    if (m_pTextEdit != NULL) {
        QString msg;
        if (packetIndex == (uint64_t)-1) {
            msg = message;
        } else {
            msg = QString("(<a href='packet#%1'>%1</a>): %2 ").arg(packetIndex).arg(message);
        }
        moveCursorToEnd();
        m_pTextEdit->append(msg);
    }
}

void vktraceviewer_output::warning(uint64_t packetIndex, const QString& warning) {
    if (m_pTextEdit != NULL) {
        QString msg;
        if (packetIndex == (uint64_t)-1) {
            msg = QString("<font color='red'>Warning: %1</font> ").arg(warning);
        } else {
            msg = QString("<font color='red'>(<a href='packet#%1'>%1</a>) Warning: %2</font> ").arg(packetIndex).arg(warning);
        }
        moveCursorToEnd();
        m_pTextEdit->append(msg);
    }
}

void vktraceviewer_output::error(uint64_t packetIndex, const QString& error) {
    if (m_pTextEdit != NULL) {
        QString msg;
        if (packetIndex == (uint64_t)-1) {
            msg = QString("<font color='red'><b>Error: %1</b></font> ").arg(convertToHtml(error));
        } else {
            msg = QString("<font color='red'><b>(<a href='packet#%1'>%1</a>) Error: %2</b></font> ")
                      .arg(packetIndex)
                      .arg(convertToHtml(error));
        }
        moveCursorToEnd();
        m_pTextEdit->append(msg);
    }
}
