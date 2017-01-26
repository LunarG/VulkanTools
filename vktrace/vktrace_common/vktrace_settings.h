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
 * Author: Jon Ashburn <jon@lunarg.com>
 * Author: Peter Lohrmann <peterl@valvesoftware.com>
 **************************************************************************/
#pragma once

#include "vktrace_common.h"

typedef enum VKTRACE_SETTING_TYPE
{
    VKTRACE_SETTING_STRING,
    VKTRACE_SETTING_BOOL,
    VKTRACE_SETTING_UINT,
    VKTRACE_SETTING_INT
} VKTRACE_SETTING_TYPE;

// ------------------------------------------------------------------------------------------------
typedef struct vktrace_SettingInfo
{
    const char* pShortName;
    const char* pLongName;
    VKTRACE_SETTING_TYPE type;
    union Data
    {
        void* pVoid;
        char** ppChar;
        BOOL* pBool;
        unsigned int* pUint;
        int* pInt;
    } Data;
    union Default
    {
        void* pVoid;
        char** ppChar;
        BOOL* pBool;
        unsigned int* pUint;
        int* pInt;
    } Default;
    BOOL bPrintInHelp;
    const char* pDesc;
} vktrace_SettingInfo;

typedef struct vktrace_SettingGroup
{
    const char* pName;
    unsigned int numSettings;
    vktrace_SettingInfo* pSettings;
} vktrace_SettingGroup;

int vktrace_SettingGroup_init(vktrace_SettingGroup* pSettingGroup, FILE *pSettingsFile, int argc, char* argv[], const char** ppOut_remaining_args);
BOOL vktrace_SettingGroup_save(vktrace_SettingGroup* pSettingGroup, unsigned int numSettingGroups, FILE* pSettingsFile);
void vktrace_SettingGroup_delete(vktrace_SettingGroup* pSettingGroup);
void vktrace_SettingGroup_reset_defaults(vktrace_SettingGroup* pSettingGroup);

// Adds pSrc group to ppDestGroups if the named group is not already there,
// or adds missing settings from pSrc into the existing group in ppDestGroups.
// pNumDestGroups is updated if pSrc is added to ppDestGroups.
void vktrace_SettingGroup_merge(vktrace_SettingGroup* pSrc, vktrace_SettingGroup** ppDestGroups, unsigned int* pNumDestGroups);

// Updates DestGroups with values from Src
void vktrace_SettingGroup_update(vktrace_SettingGroup* pSrc, vktrace_SettingGroup* pDestGroups, unsigned int numDestGroups);

// Creates a new named group at the end of the ppSettingGroups array, and updates pNumSettingGroups.
vktrace_SettingGroup* vktrace_SettingGroup_Create(const char* pGroupName, vktrace_SettingGroup** ppSettingGroups, unsigned int* pNumSettingGroups);

// Adds a STRING settingInfo to pDestGroup which holds a copy of pSrcInfo, but with a stringified value.
// The conversion to string is necessary for memory management purposes.
void vktrace_SettingGroup_Add_Info(vktrace_SettingInfo* pSrcInfo, vktrace_SettingGroup* pDestGroup);

int vktrace_SettingGroup_Load_from_file(FILE* pFile, vktrace_SettingGroup** ppSettingGroups, unsigned int* pNumSettingGroups);
void vktrace_SettingGroup_Delete_Loaded(vktrace_SettingGroup** ppSettingGroups, unsigned int* pNumSettingGroups);

void vktrace_SettingGroup_Apply_Overrides(vktrace_SettingGroup* pSettingGroup, vktrace_SettingGroup* pOverrideGroups, unsigned int numOverrideGroups);

int vktrace_SettingGroup_init_from_cmdline(vktrace_SettingGroup* pSettingGroup, int argc, char* argv[], char** ppOut_remaining_args);

void vktrace_SettingGroup_print(const vktrace_SettingGroup* pSettingGroup);
void vktrace_SettingInfo_print(const vktrace_SettingInfo* pSetting);

char* vktrace_SettingInfo_stringify_value(vktrace_SettingInfo* pSetting);
BOOL vktrace_SettingInfo_parse_value(vktrace_SettingInfo* pSetting, const char* arg);
void vktrace_SettingInfo_reset_default(vktrace_SettingInfo* pSetting);
