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

#include "vktrace_settings.h"

// ------------------------------------------------------------------------------------------------
void vktrace_SettingInfo_print(const vktrace_SettingInfo* pSetting)
{
    if (pSetting->bPrintInHelp)
    {
        char * pStrParams;
        char tmpStr[100];
        if (pSetting->type == VKTRACE_SETTING_STRING)
        {
            pStrParams = "<string>";
        } else if (pSetting->type == VKTRACE_SETTING_BOOL) {
            pStrParams = "<BOOL>";
        } else if (pSetting->type == VKTRACE_SETTING_UINT) {
            pStrParams = "<uint>";
        } else if (pSetting->type == VKTRACE_SETTING_INT) {
            pStrParams = "<int>";
        } else {
            pStrParams = "< ??? >";
        }
#if defined(WIN32)
        _snprintf_s(tmpStr, sizeof(tmpStr), _TRUNCATE, "-%s,--%s %s",
                    pSetting->pShortName, pSetting->pLongName, pStrParams);
# else
        snprintf(tmpStr, sizeof(tmpStr), "-%s, --%s %s",
                 pSetting->pShortName, pSetting->pLongName, pStrParams);
#endif
        printf("    %-33s  %s\n", tmpStr, pSetting->pDesc);
    }
}

// ------------------------------------------------------------------------------------------------
void vktrace_SettingGroup_print(const vktrace_SettingGroup* pSettingGroup)
{
    unsigned int i;
    printf("%s available options:\n", pSettingGroup->pName);

    for (i = 0; i < pSettingGroup->numSettings; i++)
    {
        vktrace_SettingInfo_print(&(pSettingGroup->pSettings[i]));
    }
}

// ------------------------------------------------------------------------------------------------
BOOL vktrace_SettingInfo_parse_value(vktrace_SettingInfo* pSetting, const char* arg)
{
    switch(pSetting->type)
    {
    case VKTRACE_SETTING_STRING:
        {
            vktrace_free(*pSetting->Data.ppChar);
            *pSetting->Data.ppChar = vktrace_allocate_and_copy(arg);
        }
        break;
    case VKTRACE_SETTING_BOOL:
        {
            BOOL bTrue = FALSE;
#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)
            bTrue = (strncasecmp(arg, "true", 4) == 0);
#elif defined(PLATFORM_WINDOWS)
            bTrue = (_strnicmp(arg, "true", 4) == 0);
#endif
            *pSetting->Data.pBool = bTrue;
        }
        break;
    case VKTRACE_SETTING_UINT:
        {
            if (sscanf(arg, "%u", pSetting->Data.pUint) != 1)
            {
                vktrace_LogWarning("Invalid unsigned int setting: '%s'. Resetting to default value instead.", arg);
                *(pSetting->Data.pUint) = *(pSetting->Default.pUint);
            }
        }
        break;
    case VKTRACE_SETTING_INT:
        {
            if (sscanf(arg, "%d", pSetting->Data.pInt) != 1)
            {
                vktrace_LogWarning("Invalid int setting: '%s'. Resetting to default value instead.", arg);
                *(pSetting->Data.pInt) = *(pSetting->Default.pInt);
            }
        }
        break;
    default:
        vktrace_LogError("Unhandled setting type (%d).", pSetting->type);
        return FALSE;
    }

    return TRUE;
}

// ------------------------------------------------------------------------------------------------
char* vktrace_SettingInfo_stringify_value(vktrace_SettingInfo* pSetting)
{
    switch(pSetting->type)
    {
    case VKTRACE_SETTING_STRING:
        {
            return vktrace_allocate_and_copy(*pSetting->Data.ppChar);
        }
        break;
    case VKTRACE_SETTING_BOOL:
        {
            return (*pSetting->Data.pBool ? vktrace_allocate_and_copy("TRUE") : vktrace_allocate_and_copy("FALSE"));
        }
        break;
    case VKTRACE_SETTING_UINT:
        {
            char value[100] = {0};
            sprintf(value, "%u", *pSetting->Data.pUint);
            return vktrace_allocate_and_copy(value);
        }
        break;
    case VKTRACE_SETTING_INT:
        {
            char value[100] = {0};
            sprintf(value, "%d", *pSetting->Data.pInt);
            return vktrace_allocate_and_copy(value);
        }
        break;
    default:
        assert(!"Unhandled setting type");
        break;
    }
    return vktrace_allocate_and_copy("<unhandled setting type>");
}

// ------------------------------------------------------------------------------------------------
void vktrace_SettingGroup_reset_defaults(vktrace_SettingGroup* pSettingGroup)
{
    if (pSettingGroup != NULL)
    {
        unsigned int u;
        for (u = 0; u < pSettingGroup->numSettings; u++)
        {
            vktrace_SettingInfo_reset_default(&pSettingGroup->pSettings[u]);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void vktrace_SettingInfo_reset_default(vktrace_SettingInfo* pSetting)
{
    assert(pSetting != NULL);
    switch(pSetting->type)
    {
    case VKTRACE_SETTING_STRING:
        if (*pSetting->Data.ppChar != NULL)
        {
            vktrace_free(*pSetting->Data.ppChar);
        }

        if (pSetting->Default.ppChar == NULL)
        {
            *pSetting->Data.ppChar = NULL;
        }
        else
        {
            *pSetting->Data.ppChar = vktrace_allocate_and_copy(*pSetting->Default.ppChar);
        }
        break;
    case VKTRACE_SETTING_BOOL:
        *pSetting->Data.pBool = *pSetting->Default.pBool;
        break;
    case VKTRACE_SETTING_UINT:
        *pSetting->Data.pUint = *pSetting->Default.pUint;
        break;
    case VKTRACE_SETTING_INT:
        *pSetting->Data.pInt = *pSetting->Default.pInt;
        break;
    default:
        assert(!"Unhandled VKTRACE_SETTING_TYPE");
        break;
    }
}

// ------------------------------------------------------------------------------------------------
void vktrace_SettingGroup_merge(vktrace_SettingGroup* pSrc, vktrace_SettingGroup** ppDestGroups, unsigned int* pNumDestGroups)
{
    unsigned int g;
    vktrace_SettingGroup* pDestGroup = NULL;
    assert(pSrc != NULL);
    assert(ppDestGroups != NULL);
    assert(pNumDestGroups != NULL);

    for (g = 0; g < *pNumDestGroups; g++)
    {
        if (strcmp(pSrc->pName, (*ppDestGroups)[g].pName) == 0)
        {
            // group exists, store the pointer
            pDestGroup = &(*ppDestGroups)[g];
            break;
        }
    }

    if (pDestGroup == NULL)
    {
        // need to replicate pSrc into ppDestGroups
        pDestGroup = vktrace_SettingGroup_Create(vktrace_allocate_and_copy(pSrc->pName), ppDestGroups, pNumDestGroups);
        assert(pDestGroup != NULL);
    }

    if (pDestGroup != NULL)
    {
        // now add all the settings!
        unsigned int srcIndex;
        for (srcIndex = 0; srcIndex < pSrc->numSettings; srcIndex++)
        {
            // search for pre-existing setting in the dest group
            unsigned int destIndex;
            BOOL bFound = FALSE;
            for (destIndex = 0; destIndex < pDestGroup->numSettings; destIndex++)
            {
                if (strcmp(pDestGroup->pSettings[destIndex].pLongName, pSrc->pSettings[srcIndex].pLongName) == 0)
                {
                    bFound = TRUE;
                    break;
                }
            }

            if (bFound == FALSE)
            {
                vktrace_SettingGroup_Add_Info(&pSrc->pSettings[srcIndex], pDestGroup);
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
void vktrace_SettingGroup_Add_Info(vktrace_SettingInfo* pSrcInfo, vktrace_SettingGroup* pDestGroup)
{
    assert(pSrcInfo != NULL);
    assert(pDestGroup != NULL);
    if (pDestGroup != NULL)
    {
        // create a SettingInfo to store the copied information
        vktrace_SettingInfo info;
        vktrace_SettingInfo* pTmp;
        memset(&info, 0, sizeof(vktrace_SettingInfo));

        // copy necessary buffers so that deletion works correctly
        info.pShortName = vktrace_allocate_and_copy(pSrcInfo->pShortName);
        info.pLongName = vktrace_allocate_and_copy(pSrcInfo->pLongName);
        info.type = VKTRACE_SETTING_STRING;
        info.Data.ppChar = vktrace_malloc(sizeof(char**));
        *info.Data.ppChar = vktrace_SettingInfo_stringify_value(pSrcInfo);
        info.pDesc = NULL;

        // add it to the current group
        pTmp = pDestGroup->pSettings;
        pDestGroup->numSettings += 1;
        pDestGroup->pSettings = VKTRACE_NEW_ARRAY(vktrace_SettingInfo, pDestGroup->numSettings);
        if (pDestGroup->pSettings == NULL)
        {
            // failed to allocate new info array
            // restore original
            pDestGroup->numSettings -= 1;
            pDestGroup->pSettings = pTmp;
        }
        else
        {
            if (pTmp != NULL)
            {
                memcpy(pDestGroup->pSettings, pTmp, pDestGroup->numSettings * sizeof(vktrace_SettingInfo));
            }

            pDestGroup->pSettings[pDestGroup->numSettings - 1] = info;
        }
    }
}

// ------------------------------------------------------------------------------------------------
vktrace_SettingGroup* vktrace_SettingGroup_Create(const char* pGroupName, vktrace_SettingGroup** ppSettingGroups, unsigned int* pNumSettingGroups)
{
    vktrace_SettingGroup* pNewGroup = NULL;
    vktrace_SettingGroup* pTmp = *ppSettingGroups;
    unsigned int lastIndex = *pNumSettingGroups;

    (*pNumSettingGroups) += 1;

    *ppSettingGroups = VKTRACE_NEW_ARRAY(vktrace_SettingGroup, *pNumSettingGroups);
    if (*ppSettingGroups == NULL)
    {
        // out of memory!
        // Don't create the new group, and restore the list to it's original state
        (*pNumSettingGroups) -= 1;
        *ppSettingGroups = pTmp;
    }
    else
    {
        // copy old settings to new ones
        memcpy(*ppSettingGroups, pTmp, lastIndex * sizeof(vktrace_SettingGroup));

        // clean up old array
        VKTRACE_DELETE(pTmp);

        // initialize new group
        memset(&(*ppSettingGroups)[lastIndex], 0, sizeof(vktrace_SettingGroup));

        // name the new group
        pNewGroup = &(*ppSettingGroups)[lastIndex];
        pNewGroup->pName = pGroupName;
    }

    return pNewGroup;
}

// ------------------------------------------------------------------------------------------------
void vktrace_SettingGroup_update(vktrace_SettingGroup* pSrc, vktrace_SettingGroup* pDestGroups, unsigned int numDestGroups)
{
    unsigned int i;
    vktrace_SettingGroup* pGroup;
    for (i = 0; i < numDestGroups; i++)
    {
        pGroup = &pDestGroups[i];
        if (strcmp(pSrc->pName, pGroup->pName) == 0)
        {
            vktrace_SettingGroup_Apply_Overrides(pGroup, pSrc, 1);
            break;
        }
    }
}

// ------------------------------------------------------------------------------------------------
int vktrace_SettingGroup_Load_from_file(FILE* pFile, vktrace_SettingGroup** ppSettingGroups, unsigned int* pNumSettingGroups)
{
    int retVal = 0;
    char* line = VKTRACE_NEW_ARRAY(char, 1024);

    assert(pFile != NULL);
    assert(ppSettingGroups != NULL);
    assert(pNumSettingGroups != NULL);
    *pNumSettingGroups = 0;

    if (line == NULL)
    {
        vktrace_LogError("Out of memory while reading settings file.");
        retVal = -1;
    }
    else
    {
        vktrace_SettingGroup* pCurGroup = NULL;
        while (feof(pFile) == 0 && ferror(pFile) == 0)
        {
            char* lineStart;
            char* pOpenBracket;
            char* pCloseBracket;
            line = fgets(line, 1024, pFile);
            if (line == NULL)
            {
                break;
            }

            // if line ends with a newline, then replace it with a NULL
            if (line[strlen(line)-1] == '\n')
            {
                line[strlen(line)-1] = '\0';
            }

            // remove any leading whitespace
            lineStart = line;
            while (*lineStart == ' ') { ++lineStart; }

            // skip empty lines
            if (strlen(lineStart) == 0)
            {
                continue;
            }

            // if the line starts with "#" or "//", then consider it a comment and ignore it.
            // if the first 'word' is only "-- " then the remainder of the line is for application arguments
            // else first 'word' in line should be a long setting name and the rest of line is value for setting
            if (lineStart[0] == '#' || (lineStart[0] == '/' && lineStart[1] == '/'))
            {
                // its a comment, continue to next loop iteration
                continue;
            }

            pOpenBracket = strchr(lineStart, '[');
            pCloseBracket = strchr(lineStart, ']');
            if (pOpenBracket != NULL && pCloseBracket != NULL)
            {
                // a group was found!
                unsigned int i;
                char* pGroupName = vktrace_allocate_and_copy_n(pOpenBracket + 1,
                                                           (int) (pCloseBracket - pOpenBracket - 1));

                // Check to see if we already have this group
                pCurGroup = NULL;
                for (i = 0; i < *pNumSettingGroups; i++)
                {
                    if (strcmp((*ppSettingGroups)[i].pName, pGroupName) == 0)
                    {
                        // we already have this group!
                        pCurGroup = &(*ppSettingGroups)[i];
                        break;
                    }
                }

                if (pCurGroup == NULL)
                {
                    // Need to grow our list of groups!
                    pCurGroup = vktrace_SettingGroup_Create(pGroupName, ppSettingGroups, pNumSettingGroups);
                }
            }
            else
            {
                char* pTokName = strtok(lineStart, "=");
                char* pTokValue = strtok(NULL, "=");
                if (pTokName != NULL && pTokValue != NULL)
                {
                    // A setting name and value were found!
                    char* pValueStart = pTokValue;
                    char* pTmpEndName = pTokName;

                    assert(pCurGroup != NULL);
                    if (pCurGroup != NULL)
                    {
                        // create a SettingInfo to store this information
                        vktrace_SettingInfo info;
                        vktrace_SettingInfo* pTmp;
                        memset(&info, 0, sizeof(vktrace_SettingInfo));

                        // trim trailing whitespace by turning it into a null char
                        while (*pTmpEndName != '\0')
                        {
                            if (*pTmpEndName == ' ')
                            {
                                *pTmpEndName = '\0';
                                break;
                            }
                            else
                            {
                                ++pTmpEndName;
                            }
                        }

                        info.pLongName = vktrace_allocate_and_copy(pTokName);
                        info.type = VKTRACE_SETTING_STRING;

                        // remove leading whitespace from value
                        while (*pValueStart == ' ') { ++pValueStart; }
                        info.Data.ppChar = vktrace_malloc(sizeof(char**));
                        *info.Data.ppChar = vktrace_allocate_and_copy(pValueStart);

                        // add it to the current group
                        pTmp = pCurGroup->pSettings;
                        pCurGroup->numSettings += 1;
                        pCurGroup->pSettings = VKTRACE_NEW_ARRAY(vktrace_SettingInfo, pCurGroup->numSettings);
                        if (pCurGroup->pSettings == NULL)
                        {
                            // failed to allocate new info array
                            // restore original
                            pCurGroup->numSettings -= 1;
                            pCurGroup->pSettings = pTmp;
                        }
                        else
                        {
                            if (pTmp != NULL)
                            {
                                memcpy(pCurGroup->pSettings, pTmp, pCurGroup->numSettings * sizeof(vktrace_SettingInfo));
                            }

                            pCurGroup->pSettings[pCurGroup->numSettings - 1] = info;
                        }
                    }
                }
                else
                {
                    vktrace_LogWarning("Could not parse a line in settings file: '%s'.", line);
                }
            }
        }
    }

    VKTRACE_DELETE(line);

    return retVal;
}

// ------------------------------------------------------------------------------------------------
void vktrace_SettingGroup_Delete_Loaded(vktrace_SettingGroup** ppSettingGroups, unsigned int* pNumSettingGroups)
{
    unsigned int g;
    unsigned int s;
    assert(ppSettingGroups != NULL);
    assert(*ppSettingGroups != NULL);
    assert(pNumSettingGroups != NULL);

    for (g = 0; g < *pNumSettingGroups; g++)
    {
        vktrace_SettingGroup* pGroup = &(*ppSettingGroups)[g];
        vktrace_free((void*)pGroup->pName);
        pGroup->pName = NULL;

        for (s = 0; s < pGroup->numSettings; s++)
        {
            vktrace_free((void*)pGroup->pSettings[s].pShortName);
            pGroup->pSettings[s].pShortName = NULL;

            vktrace_free((void*)pGroup->pSettings[s].pLongName);
            pGroup->pSettings[s].pLongName = NULL;

            vktrace_free(*pGroup->pSettings[s].Data.ppChar);
            vktrace_free(pGroup->pSettings[s].Data.ppChar);
            pGroup->pSettings[s].Data.ppChar = NULL;

            vktrace_free((void*)pGroup->pSettings[s].pDesc);
            pGroup->pSettings[s].pDesc = NULL;

        }

        vktrace_free((void*)pGroup->pSettings);
        pGroup->pSettings = NULL;
    }

    vktrace_free(*ppSettingGroups);
    *pNumSettingGroups = 0;
}

// ------------------------------------------------------------------------------------------------
void vktrace_SettingGroup_Apply_Overrides(vktrace_SettingGroup* pSettingGroup, vktrace_SettingGroup* pOverrideGroups, unsigned int numOverrideGroups)
{
    unsigned int overrideGroupIndex;
    assert(pSettingGroup != NULL);
    assert(pOverrideGroups != NULL);

    // only override matching group (based on name)
    for (overrideGroupIndex = 0; overrideGroupIndex < numOverrideGroups; overrideGroupIndex++)
    {
        if (strcmp(pSettingGroup->pName, pOverrideGroups[overrideGroupIndex].pName) == 0)
        {
            unsigned int overrideSettingIndex;
            vktrace_SettingGroup* pOverride = &pOverrideGroups[overrideGroupIndex];

            for (overrideSettingIndex = 0; overrideSettingIndex < pOverride->numSettings; overrideSettingIndex++)
            {
                unsigned int baseSettingIndex;
                vktrace_SettingInfo* pOverrideSetting = &pOverride->pSettings[overrideSettingIndex];

                // override matching settings based on long name
                for (baseSettingIndex = 0; baseSettingIndex < pSettingGroup->numSettings; baseSettingIndex++)
                {
                    if (strcmp(pSettingGroup->pSettings[baseSettingIndex].pLongName, pOverrideSetting->pLongName) == 0)
                    {
                        char* pTmp = vktrace_SettingInfo_stringify_value(pOverrideSetting);
                        if (vktrace_SettingInfo_parse_value(&pSettingGroup->pSettings[baseSettingIndex], pTmp) == FALSE)
                        {
                            vktrace_LogWarning("Failed to parse override value.");
                        }
                        vktrace_free(pTmp);
                        break;
                    }
                }
            }
            break;
        }
    }
}

//-----------------------------------------------------------------------------
BOOL vktrace_SettingGroup_save(vktrace_SettingGroup* pSettingGroup, unsigned int numSettingGroups, FILE* pSettingsFile)
{
    BOOL retVal = TRUE;

    if (pSettingGroup == NULL)
    {
        vktrace_LogError("Cannot save a null group of settings.");
        retVal = FALSE;
    }

    if (pSettingsFile == NULL)
    {
        vktrace_LogError("Cannot save an unnamed settings file.");
        retVal = FALSE;
    }

    if (retVal == TRUE)
    {
        unsigned int g;
        unsigned int index;

        for (g = 0; g < numSettingGroups; g++)
        {
            // group name
            fputs("[", pSettingsFile);
            fputs(pSettingGroup[g].pName, pSettingsFile);
            fputs("]\n", pSettingsFile);

            // settings
            for (index = 0; index < pSettingGroup[g].numSettings; index++)
            {
                char* value = NULL;
                fputs("   ", pSettingsFile);
                fputs(pSettingGroup[g].pSettings[index].pLongName, pSettingsFile);
                fputs(" = ", pSettingsFile);
                value = vktrace_SettingInfo_stringify_value(&pSettingGroup[g].pSettings[index]);
                if (value != NULL)
                {
                    fputs(value, pSettingsFile);
                    vktrace_free(value);
                }
                else
                {
                    fputs("", pSettingsFile);
                }
                fputs("\n", pSettingsFile);
            }

            fputs("\n", pSettingsFile);
        }
    }

    return retVal;
}

//-----------------------------------------------------------------------------
int vktrace_SettingGroup_init_from_cmdline(vktrace_SettingGroup* pSettingGroup, int argc, char* argv[], char** ppOut_remaining_args)
{
    int i = 0;

    if (pSettingGroup != NULL)
    {
        vktrace_SettingInfo* pSettings = pSettingGroup->pSettings;
        unsigned int num_settings = pSettingGroup->numSettings;

        // update settings based on command line options
        for (i = 1; i < argc; )
        {
            unsigned int settingIndex;
            int consumed = 0;
            char* curArg = argv[i];

            // if the arg is only "--" then all following args are for the application;
            // if the arg starts with "-" then it is referring to a short name;
            // if the arg starts with "--" then it is referring to a long name.
            if (strcmp("--", curArg) == 0 && ppOut_remaining_args != NULL)
            {
                // all remaining args are for the application

                // increment past the current arg
                i += 1;
                consumed++;
                for (; i < argc; i++)
                {
                    if (*ppOut_remaining_args == NULL || strlen(*ppOut_remaining_args) == 0)
                    {
                        *ppOut_remaining_args = vktrace_allocate_and_copy(argv[i]);
                    }
                    else
                    {
                        *ppOut_remaining_args = vktrace_copy_and_append(*ppOut_remaining_args, " ", argv[i]);
                    }
                    consumed++;
                }
            }
            else
            {
                for (settingIndex = 0; settingIndex < num_settings; settingIndex++)
                {
                    const char* pSettingName = NULL;
                    curArg = argv[i];
                    if (strncmp("--", curArg, 2) == 0)
                    {
                        // long option name
                        pSettingName = pSettings[settingIndex].pLongName;
                        curArg += 2;
                    }
                    else if (strncmp("-", curArg, 1) == 0)
                    {
                        // short option name
                        pSettingName = pSettings[settingIndex].pShortName;
                        curArg += 1;
                    }

                    if (pSettingName != NULL && strcmp(curArg, pSettingName) == 0)
                    {
                        if (i+1 < argc &&
                            vktrace_SettingInfo_parse_value(&pSettings[settingIndex], argv[i+1]))
                        {
                            consumed += 2;
                        }
                        break;
                    }
                }
            }

            if (consumed == 0)
            {
                vktrace_SettingGroup_print(pSettingGroup);
                vktrace_SettingGroup_delete(pSettingGroup);
                return -1;
            }

            i += consumed;
        }
    }

    return 0;
}

// ------------------------------------------------------------------------------------------------
int vktrace_SettingGroup_init(vktrace_SettingGroup* pSettingGroup, FILE* pSettingsFile, int argc, char* argv[], const char** ppOut_remaining_args)
{
    if (pSettingGroup == NULL)
    {
        assert(!"No need to call vktrace_SettingGroup_init if the application has no settings");
        return 0;
    }

    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
    {
        vktrace_SettingGroup_print(pSettingGroup);
        return -1;
    }

    // Initially, set all options to their defaults
    vktrace_SettingGroup_reset_defaults(pSettingGroup);

    // Secondly set options based on settings file
    if (pSettingsFile != NULL)
    {
        vktrace_SettingGroup* pGroups = NULL;
        unsigned int numGroups = 0;
        if (vktrace_SettingGroup_Load_from_file(pSettingsFile, &pGroups, &numGroups) == -1)
        {
            vktrace_SettingGroup_print(pSettingGroup);
            return -1;
        }

        vktrace_SettingGroup_Apply_Overrides(pSettingGroup, pGroups, numGroups);

        vktrace_SettingGroup_Delete_Loaded(&pGroups, &numGroups);
    }

    // Thirdly set options based on cmd line args
    if (vktrace_SettingGroup_init_from_cmdline(pSettingGroup, argc, argv, (char **)ppOut_remaining_args) == -1)
    {
        return -1;
    }

    return 0;
}

// ------------------------------------------------------------------------------------------------
void vktrace_SettingGroup_delete(vktrace_SettingGroup* pSettingGroup)
{
    if (pSettingGroup != NULL)
    {
        unsigned int i;

        // need to delete all strings
        for (i = 0; i < pSettingGroup->numSettings; i++)
        {
            if (pSettingGroup->pSettings[i].type == VKTRACE_SETTING_STRING)
            {
                if (*(pSettingGroup->pSettings[i].Data.ppChar) != NULL)
                {
                    vktrace_free(*pSettingGroup->pSettings[i].Data.ppChar);
                    *pSettingGroup->pSettings[i].Data.ppChar = NULL;
                }
            }
        }
    }
}
