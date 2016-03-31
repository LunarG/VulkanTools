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

#include <QApplication>

#include "vktraceviewer.h"
#include "vktraceviewer_settings.h"

int main(int argc, char *argv[])
{
    // Initialize QApplication before initializing settings
    QApplication a(argc, argv);

    // initialize settings
    if (vktraceviewer_initialize_settings(argc, argv) == false)
    {
        return -1;
    }

    vktraceviewer w;
    w.show();

    if (g_settings.trace_file_to_open != NULL && strlen(g_settings.trace_file_to_open) > 0)
    {
        w.open_trace_file_threaded(QString(g_settings.trace_file_to_open));
    }

    int result = a.exec();

    vktrace_SettingGroup_Delete_Loaded(&g_pAllSettings, &g_numAllSettings);

    return result;
}
