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
#include "vktraceviewer_settings.h"
#include "vktraceviewer_output.h"
#include <assert.h>

#include <QCoreApplication>
#include <QDir>

extern "C" {
#include "vktrace_settings.h"
}

static const unsigned int VKTRACEVIEWER_SETTINGS_FILE_FORMAT_VERSION_1 = 1;
static const unsigned int VKTRACEVIEWER_SETTINGS_FILE_FORMAT_VERSION = VKTRACEVIEWER_SETTINGS_FILE_FORMAT_VERSION_1;

static const char *s_SETTINGS_FILE = "vktraceviewer_settings.txt";

// declared as extern in header
vktraceviewer_settings g_settings;
static vktraceviewer_settings s_default_settings;
vktrace_SettingGroup* g_pAllSettings = NULL;
unsigned int g_numAllSettings = 0;

vktrace_SettingInfo g_settings_info[] =
{
    { "o", "OpenTraceFile", VKTRACE_SETTING_STRING, &g_settings.trace_file_to_open, &s_default_settings.trace_file_to_open, TRUE, "Load the specified trace file when vktraceviewer is opened."},
    { "wl", "WindowLeft", VKTRACE_SETTING_INT, &g_settings.window_position_left, &s_default_settings.window_position_left, TRUE, "Left coordinate of VkTraceViewer window on startup."},
    { "wt", "WindowTop", VKTRACE_SETTING_INT, &g_settings.window_position_top, &s_default_settings.window_position_top, TRUE, "Top coordinate of VkTraceViewer window on startup."},
    { "ww", "WindowWidth", VKTRACE_SETTING_INT, &g_settings.window_size_width, &s_default_settings.window_size_width, TRUE, "Width of VkTraceViewer window on startup."},
    { "wh", "WindowHeight", VKTRACE_SETTING_INT, &g_settings.window_size_height, &s_default_settings.window_size_height, TRUE, "Height of VkTraceViewer window on startup."},

    { "", "GenTraceApplication", VKTRACE_SETTING_STRING, &g_settings.gentrace_application, &s_default_settings.gentrace_application, FALSE, "The most recent application path in the 'Generate Trace' dialog."},
    { "", "GenTraceArguments", VKTRACE_SETTING_STRING, &g_settings.gentrace_arguments, &s_default_settings.gentrace_arguments, FALSE, "The most recent application arguments in the 'Generate Trace' dialog."},
    { "", "GenTraceWorkingDir", VKTRACE_SETTING_STRING, &g_settings.gentrace_working_dir, &s_default_settings.gentrace_working_dir, FALSE, "The most recent working directory in the 'Generate Trace' dialog."},
    { "", "GenTraceVkLayerPath", VKTRACE_SETTING_STRING, &g_settings.gentrace_vk_layer_path, &s_default_settings.gentrace_vk_layer_path, FALSE, "The most recent VK_LAYER_PATH used in the 'Generate Trace' dialog."},
    { "", "GenTraceOutputFile", VKTRACE_SETTING_STRING, &g_settings.gentrace_output_file, &s_default_settings.gentrace_output_file, FALSE, "The most recent output trace file in the 'Generate Trace' dialog."},

    { "", "SettingsDialogWidth", VKTRACE_SETTING_INT, &g_settings.settings_dialog_width, &s_default_settings.settings_dialog_width, TRUE, "Width of VkTraceViewer settings dialog when opened."},
    { "", "SettingsDialogHeight", VKTRACE_SETTING_INT, &g_settings.settings_dialog_height, &s_default_settings.settings_dialog_height, TRUE, "Height of VkTraceViewer settings dialog when opened."},

    //{ "tltps", "trim_large_trace_prompt_size", VKTRACE_SETTING_UINT, &g_settings.trim_large_trace_prompt_size, &s_default_settings.trim_large_trace_prompt_size, TRUE, "The number of frames in a trace file at which the user should be prompted to trim the trace before loading."},
    //{ "gsr", "group_state_render", VKTRACE_SETTING_BOOL, &g_settings.groups_state_render, &s_default_settings.groups_state_render, TRUE, "Path to the dynamic tracer library to be injected, may use [0-15]."},
    //{ "gppm", "groups_push_pop_markers", VKTRACE_SETTING_BOOL, &g_settings.groups_push_pop_markers, &s_default_settings.groups_push_pop_markers, TRUE, "Path to the dynamic tracer library to be injected, may use [0-15]."},
    //{ "gnc", "groups_nested_calls", VKTRACE_SETTING_BOOL, &g_settings.groups_nested_calls, &s_default_settings.groups_nested_calls, TRUE, "Path to the dynamic tracer library to be injected, may use [0-15]."},
};

vktrace_SettingGroup g_settingGroup =
{
    "vktraceviewer",
    sizeof(g_settings_info) / sizeof(g_settings_info[0]),
    &g_settings_info[0]
};

QString vktraceviewer_get_settings_file_path()
{
    QString result = vktraceviewer_get_settings_directory() + "/" + QString(s_SETTINGS_FILE);
    return result;
}

QString vktraceviewer_get_settings_directory()
{
    char * pSettingsPath = vktrace_platform_get_settings_path();
    QString result = QString(pSettingsPath);
    vktrace_free(pSettingsPath);
    return result;
}

QString vktraceviewer_get_sessions_directory()
{
    char * pDataPath = vktrace_platform_get_data_path();
    QString result = QString(pDataPath) + "/sessions/";
    vktrace_free(pDataPath);
    return result;
}

bool vktraceviewer_initialize_settings(int argc, char* argv[])
{
    bool bSuccess = true;

    // setup default values
    memset(&s_default_settings, 0, sizeof(vktraceviewer_settings));

    s_default_settings.trace_file_to_open = NULL;
    s_default_settings.window_position_left = 0;
    s_default_settings.window_position_top = 0;
    s_default_settings.window_size_width = 1024;
    s_default_settings.window_size_height = 768;

    s_default_settings.gentrace_application = NULL;
    s_default_settings.gentrace_arguments = NULL;
    s_default_settings.gentrace_working_dir = NULL;
    s_default_settings.gentrace_vk_layer_path = NULL;
    s_default_settings.gentrace_output_file = NULL;

    // This seems to be a reasonable default size for the dialog.
    s_default_settings.settings_dialog_width = 600;
    s_default_settings.settings_dialog_height = 600;

    // initialize settings as defaults
    g_settings = s_default_settings;

    QString settingsFilePath = vktraceviewer_get_settings_file_path();
    FILE* pFile = fopen(settingsFilePath.toStdString().c_str(), "r");
    if (pFile == NULL)
    {
        vktrace_LogWarning("Unable to open settings file: '%s'.", settingsFilePath.toStdString().c_str());
    }

    // Secondly set options based on settings file
    if (pFile != NULL)
    {
        g_pAllSettings = NULL;
        g_numAllSettings = 0;
        if (vktrace_SettingGroup_Load_from_file(pFile, &g_pAllSettings, &g_numAllSettings) == -1)
        {
            vktrace_SettingGroup_print(&g_settingGroup);
            return false;
        }

        if (g_pAllSettings != NULL && g_numAllSettings > 0)
        {
            vktrace_SettingGroup_Apply_Overrides(&g_settingGroup, g_pAllSettings, g_numAllSettings);
        }

        fclose(pFile);
        pFile = NULL;
    }

    // apply settings from settings file and from cmd-line args
    if (vktrace_SettingGroup_init_from_cmdline(&g_settingGroup, argc, argv, &g_settings.trace_file_to_open) != 0)
    {
        // invalid options specified
        bSuccess = false;
    }

    // Merge known vktraceviewer settings into the loaded settings.
    // This ensures that new known settings are added to the settings dialog
    // and will be re-written to the settings file upon saving.
    vktrace_SettingGroup_merge(&g_settingGroup, &g_pAllSettings, &g_numAllSettings);

    // This would be a good place to validate any "required" settings, but right now there aren't any!

    if (bSuccess == false)
    {
        vktrace_SettingGroup_print(&g_settingGroup);
        vktrace_SettingGroup_delete(&g_settingGroup);
    }

    return bSuccess;
}

void vktraceviewer_settings_updated()
{
    vktrace_SettingGroup_update(&g_settingGroup, g_pAllSettings, g_numAllSettings);
}

void vktraceviewer_save_settings()
{
    QDir settingsDir(vktraceviewer_get_settings_directory());
    if (settingsDir.mkpath(".") == false)
    {
        QString error = "Failed to create " + settingsDir.path();
        vktraceviewer_output_error(error);
    }

    QString filepath = vktraceviewer_get_settings_file_path();
    FILE* pSettingsFile = fopen(filepath.toStdString().c_str(), "w");
    if (pSettingsFile == NULL)
    {
        QString error = "Failed to open settings file for writing: " + filepath;
        vktraceviewer_output_error(error);
    }
    else
    {
        if (vktrace_SettingGroup_save(g_pAllSettings, g_numAllSettings, pSettingsFile) == FALSE)
        {
            QString error = "Failed to save settings file: " + filepath;
            vktraceviewer_output_error(error);
        }

        fclose(pSettingsFile);
    }
}
