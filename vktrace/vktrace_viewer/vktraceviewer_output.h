/**************************************************************************
 *
 * Copyright 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
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
#ifndef VKTRACEVIEWER_OUTPUT_H
#define VKTRACEVIEWER_OUTPUT_H

#include <QString>
#include <QTextBrowser>
extern "C" {
#include "vktrace_platform.h"
#include "vktrace_tracelog.h"
}

class QTextEdit;

class vktraceviewer_output {
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
inline void vktraceviewer_output_message(const QString& message) { gs_OUTPUT.message((uint64_t)-1, message); }

inline void vktraceviewer_output_warning(uint64_t packetIndex, const QString& warning) { gs_OUTPUT.warning(packetIndex, warning); }
inline void vktraceviewer_output_warning(const QString& warning) { gs_OUTPUT.warning((uint64_t)-1, warning); }

inline void vktraceviewer_output_error(uint64_t packetIndex, const QString& error) { gs_OUTPUT.error(packetIndex, error); }
inline void vktraceviewer_output_error(const QString& error) { gs_OUTPUT.error((uint64_t)-1, error); }
inline void vktraceviewer_output_deinit() { gs_OUTPUT.init(0); }

#endif  // VKTRACEVIEWER_OUTPUT_H
