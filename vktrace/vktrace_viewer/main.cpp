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

#include <QApplication>

#include "vktraceviewer_settings.h"
#include "vktraceviewer.h"

#if defined(WIN32)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{

#if defined(WIN32)
    int argc = 1;
    char *argv[1];
    argv[0] = "vktraceviewer";
#endif
    // Initialize QApplication before initializing settings
    QApplication a(argc, argv);

    // initialize settings
    if (vktraceviewer_initialize_settings(argc, argv) == false) {
        return -1;
    }

    vktraceviewer w;
    w.show();

    if (g_settings.trace_file_to_open != NULL && strlen(g_settings.trace_file_to_open) > 0) {
        w.open_trace_file_threaded(QString(g_settings.trace_file_to_open));
    }

    int result = a.exec();

    vktrace_SettingGroup_Delete_Loaded(&g_pAllSettings, &g_numAllSettings);

    return result;
}
