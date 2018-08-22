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
#ifndef VKTRACEVIEWER_SETTINGS_H
#define VKTRACEVIEWER_SETTINGS_H

extern "C" {
#include "vktrace_settings.h"
}

#include <QString>

extern vktrace_SettingGroup* g_pAllSettings;
extern unsigned int g_numAllSettings;

typedef struct vktraceviewer_settings {
    char* trace_file_to_open;
    char* adb_location;
    int window_position_left;
    int window_position_top;
    int window_size_width;
    int window_size_height;
    char* gentrace_application;
    char* gentrace_arguments;
    char* gentrace_working_dir;
    char* gentrace_vk_layer_path;
    char* gentrace_output_file;
    int settings_dialog_width;
    int settings_dialog_height;
    // unsigned int trim_large_trace_prompt_size;

    // bool groups_state_render;
    // bool groups_push_pop_markers;
    // bool groups_nested_calls;
} vktraceviewer_settings;

extern vktraceviewer_settings g_settings;
extern vktrace_SettingGroup g_settingGroup;

bool vktraceviewer_initialize_settings(int argc, char* argv[]);

void vktraceviewer_settings_updated();

void vktraceviewer_save_settings();

QString vktraceviewer_get_settings_file_path();
QString vktraceviewer_get_settings_directory();
QString vktraceviewer_get_sessions_directory();

#endif  // VKTRACEVIEWER_SETTINGS_H
