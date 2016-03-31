/**************************************************************************
*
* Copyright 2014-2016 Valve Corporation
* Copyright (C) 2014-2016 LunarG, Inc.
* All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
**************************************************************************/
#include "vktraceviewer_output.h"
#include <QTextEdit>

vktraceviewer_output gs_OUTPUT;

vktraceviewer_output::vktraceviewer_output()
{
}

vktraceviewer_output::~vktraceviewer_output()
{
}

void vktraceviewer_output::init(QTextBrowser *pTextEdit)
{
    m_pTextEdit = pTextEdit;
}

QString vktraceviewer_output::convertToHtml(QString message)
{
    QString result;
    if (message.endsWith("\n"))
    {
        message.chop(1);
    }
    result = message.replace("\n", "<br>");
    return result;
}

void vktraceviewer_output::moveCursorToEnd()
{
    QTextCursor cursor = m_pTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    m_pTextEdit->setTextCursor(cursor);
}

void vktraceviewer_output::message(uint64_t packetIndex, const QString& message)
{
    if (m_pTextEdit != NULL)
    {
        QString msg;
        if (packetIndex == (uint64_t)-1)
        {
            msg = message;
        }
        else
        {
            msg = QString("(<a href='packet#%1'>%1</a>): %2 ").arg(packetIndex).arg(message);
        }
        moveCursorToEnd();
        m_pTextEdit->append(msg);
    }
}

void vktraceviewer_output::warning(uint64_t packetIndex, const QString& warning)
{
    if (m_pTextEdit != NULL)
    {
        QString msg;
        if (packetIndex == (uint64_t)-1)
        {
            msg = QString("<font color='red'>Warning: %1</font> ").arg(warning);
        }
        else
        {
            msg = QString("<font color='red'>(<a href='packet#%1'>%1</a>) Warning: %2</font> ").arg(packetIndex).arg(warning);
        }
        moveCursorToEnd();
        m_pTextEdit->append(msg);
    }
}

void vktraceviewer_output::error(uint64_t packetIndex, const QString& error)
{
    if (m_pTextEdit != NULL)
    {
        QString msg;
        if (packetIndex == (uint64_t)-1)
        {
            msg = QString("<font color='red'><b>Error: %1</b></font> ").arg(convertToHtml(error));
        }
        else
        {
            msg = QString("<font color='red'><b>(<a href='packet#%1'>%1</a>) Error: %2</b></font> ").arg(packetIndex).arg(convertToHtml(error));
        }
        moveCursorToEnd();
        m_pTextEdit->append(msg);
    }
}
