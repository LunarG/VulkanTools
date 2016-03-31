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
#ifndef VKTRACEVIEWER_SETTINGS_H
#define VKTRACEVIEWER_SETTINGS_H

extern "C" {
#include "vktrace_settings.h"
}

#include <QString>

extern vktrace_SettingGroup* g_pAllSettings;
extern unsigned int g_numAllSettings;

typedef struct vktraceviewer_settings
{
    char * trace_file_to_open;
    int window_position_left;
    int window_position_top;
    int window_size_width;
    int window_size_height;
    char * gentrace_application;
    char * gentrace_arguments;
    char * gentrace_working_dir;
    char * gentrace_vk_layer_path;
    char * gentrace_output_file;
    int settings_dialog_width;
    int settings_dialog_height;
    //unsigned int trim_large_trace_prompt_size;

    //bool groups_state_render;
    //bool groups_push_pop_markers;
    //bool groups_nested_calls;
} vktraceviewer_settings;

extern vktraceviewer_settings g_settings;
extern vktrace_SettingGroup g_settingGroup;

bool vktraceviewer_initialize_settings(int argc, char* argv[]);

void vktraceviewer_settings_updated();

void vktraceviewer_save_settings();

QString vktraceviewer_get_settings_file_path();
QString vktraceviewer_get_settings_directory();
QString vktraceviewer_get_sessions_directory();

#endif // VKTRACEVIEWER_SETTINGS_H
