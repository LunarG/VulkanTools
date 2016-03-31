/**************************************************************************
*
* Copyright 2015-2016 Valve Corporation
* Copyright (C) 2015-2016 LunarG, Inc.
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
#ifndef VKTRACEVIEWER_OUTPUT_H
#define VKTRACEVIEWER_OUTPUT_H

#include <QString>
#include <QTextBrowser>
extern "C"
{
#include "vktrace_platform.h"
#include "vktrace_tracelog.h"
}

class QTextEdit;

class vktraceviewer_output
{
public:
    vktraceviewer_output();
    ~vktraceviewer_output();

    void init(QTextBrowser* pTextEdit);

    void message(uint64_t packetIndex, const QString& message);
    void warning(uint64_t packetIndex, const QString& warning);
    void error(uint64_t packetIndex, const QString& error);

private:
    QString convertToHtml(QString message);
    void moveCursorToEnd();
    QTextBrowser* m_pTextEdit;
};

extern vktraceviewer_output gs_OUTPUT;

inline void vktraceviewer_output_init(QTextBrowser* pTextEdit) { gs_OUTPUT.init(pTextEdit); }

inline void vktraceviewer_output_message(uint64_t packetIndex, const QString& message) { gs_OUTPUT.message(packetIndex, message); }
inline void vktraceviewer_output_message(const QString& message) { gs_OUTPUT.message(-1, message); }

inline void vktraceviewer_output_warning(uint64_t packetIndex, const QString& warning) { gs_OUTPUT.warning(packetIndex, warning); }
inline void vktraceviewer_output_warning(const QString& warning) { gs_OUTPUT.warning(-1, warning); }

inline void vktraceviewer_output_error(uint64_t packetIndex, const QString& error) { gs_OUTPUT.error(packetIndex, error); }
inline void vktraceviewer_output_error(const QString& error) { gs_OUTPUT.error(-1, error); }
inline void vktraceviewer_output_deinit() { gs_OUTPUT.init(0); }

#endif // VKTRACEVIEWER_OUTPUT_H
