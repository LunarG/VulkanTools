/*
 * Copyright (c) 2016-2021 Valve Corporation
 * Copyright (c) 2016-2021 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Mark Young <marky@lunarg.com>
 */

#ifdef VIA_WINDOWS_TARGET

#include "via_system_windows.hpp"

static bool g_is_wow64 = false;

ViaSystemWindows::ViaSystemWindows() : ViaSystem() {
    _generate_unique_file = false;
    _out_file = "";
    _directory_symbol = '\\';

    // Determine the user's home directory
    char home_drive[32];
    char generic_string[1024];
    if (0 == GetEnvironmentVariableA("HOMEDRIVE", home_drive, 31) ||
        0 == GetEnvironmentVariableA("HOMEPATH", generic_string, 1023)) {
        LogError("Failed to retrieve either HOMEDRIVE or HOMEPATH environment settings!");
        exit(-1);
    }
    _home_path = home_drive;
    _home_path += generic_string;
    _home_path += "\\";

    // Determine if this is a 32-bit executable running on 64-bit Windows
    g_is_wow64 = false;
    // IsWow64Process is not available on all supported versions of Windows.
    // Use GetModuleHandle to get a handle to the DLL that contains the function
    // and GetProcAddress to get a pointer to the function if available.
    typedef BOOL(WINAPI * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fpIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
    if (nullptr != fpIsWow64Process) {
        BOOL is_wow_64_process = FALSE;
        if (!fpIsWow64Process(GetCurrentProcess(), &is_wow_64_process)) {
            LogError("Failed to determine properly if 32-bit on Win64!");
        }
        g_is_wow64 = (is_wow_64_process == TRUE);
    }

    // Get the path to the current EXE
    char temp_c_string[1024];
    int bytes = GetModuleFileName(NULL, temp_c_string, 1023);
    if (0 < bytes) {
        // Replace all Windows directory symbols with Linux versions for C++ calls
        std::string to_cpp_string = temp_c_string;
        to_cpp_string = to_cpp_string.substr(0, to_cpp_string.rfind(_directory_symbol));
        _exe_path = ConvertPathFormat(to_cpp_string);
    } else {
        _exe_path = "";
    }

    if (0 != GetCurrentDirectoryA(1023, generic_string)) {
        _cur_path = ConvertPathFormat(generic_string);
    } else {
        _cur_path = "";
    }
}

bool ViaSystemWindows::IsAbsolutePath(const std::string &path) {
    if (path[0] == _directory_symbol || path[1] == ':') {
        return true;
    }
    return false;
}

// Registry utility functions to simplify reading data from the
// Windows registry.

const char g_uninstall_reg_path[] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

static bool ReadRegKeyString(HKEY regFolder, const char *keyPath, const char *valueName, const int maxLength, char *retString) {
    bool retVal = false;
    DWORD bufLen = maxLength;
    DWORD keyFlags = KEY_READ;
    HKEY hKey;
    LONG lret;

    if (g_is_wow64) {
        keyFlags |= KEY_WOW64_64KEY;
    }

    *retString = '\0';
    lret = RegOpenKeyExA(regFolder, keyPath, 0, keyFlags, &hKey);
    if (lret == ERROR_SUCCESS) {
        lret = RegQueryValueExA(hKey, valueName, NULL, NULL, (BYTE *)retString, &bufLen);
        if (lret == ERROR_SUCCESS) {
            retVal = true;
        }
        RegCloseKey(hKey);
    }

    return retVal;
}

static bool WriteRegKeyString(HKEY regFolder, const char *keyPath, char *valueName, char *valueValue) {
    bool retVal = false;
    DWORD keyFlags = KEY_WRITE;
    HKEY hKey;
    LONG lret;

    if (g_is_wow64) {
        keyFlags |= KEY_WOW64_64KEY;
    }

    lret = RegOpenKeyExA(regFolder, keyPath, 0, keyFlags, &hKey);
    if (lret == ERROR_SUCCESS) {
        lret = RegSetKeyValueA(hKey, NULL, valueName, REG_SZ, (BYTE *)valueValue, (DWORD)(strlen(valueValue)));
        if (lret == ERROR_SUCCESS) {
            retVal = true;
        }
        RegCloseKey(hKey);
    }

    return retVal;
}

static bool DeleteRegKeyString(HKEY regFolder, const char *keyPath, char *valueName) {
    bool retVal = false;
    DWORD keyFlags = KEY_WRITE;
    HKEY hKey;
    LONG lret;

    if (g_is_wow64) {
        keyFlags |= KEY_WOW64_64KEY;
    }

    lret = RegOpenKeyExA(regFolder, keyPath, 0, keyFlags, &hKey);
    if (lret == ERROR_SUCCESS) {
        lret = RegDeleteKeyValueA(hKey, NULL, valueName);
        if (lret == ERROR_SUCCESS) {
            retVal = true;
        }
        RegCloseKey(hKey);
    }

    return retVal;
}

static bool ReadRegKeyDword(HKEY regFolder, const char *keyPath, const char *valueName, unsigned int *returnInt) {
    bool retVal = false;
    DWORD bufLen = sizeof(DWORD);
    DWORD keyFlags = KEY_READ;
    HKEY hKey;
    LONG lret;

    if (g_is_wow64) {
        keyFlags |= KEY_WOW64_64KEY;
    }

    *returnInt = 0;
    lret = RegOpenKeyExA(regFolder, keyPath, 0, keyFlags, &hKey);
    if (lret == ERROR_SUCCESS) {
        lret = RegQueryValueExA(hKey, valueName, NULL, NULL, (BYTE *)returnInt, &bufLen);
        if (lret == ERROR_SUCCESS) {
            retVal = true;
        }
        RegCloseKey(hKey);
    }

    return retVal;
}

static bool FindNextRegKey(HKEY regFolder, const char *keyPath, const char *keySearch, const int itemIndex, const int maxLength,
                           char *retString) {
    bool retVal = false;
    DWORD bufLen = 1023;
    DWORD keyFlags = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE;
    HKEY hKey;
    LONG lret;
    int itemCount = 0;

    if (g_is_wow64) {
        keyFlags |= KEY_WOW64_64KEY;
    }

    *retString = '\0';
    lret = RegOpenKeyExA(regFolder, keyPath, 0, keyFlags, &hKey);
    if (lret == ERROR_SUCCESS) {
        DWORD index = 0;
        char keyName[1024];

        do {
            lret = RegEnumKeyExA(hKey, index, keyName, &bufLen, NULL, NULL, NULL, NULL);
            if (ERROR_SUCCESS != lret) {
                break;
            }
            if (strlen(keySearch) == 0 || NULL != strstr(keyName, keySearch)) {
                if (itemIndex == itemCount) {
                    strncpy_s(retString, maxLength, keyName, bufLen);
                    retVal = true;
                    break;
                } else {
                    itemCount++;
                }
            }
            bufLen = 1023;
            ++index;
        } while (true);
    }

    return retVal;
}

static bool FindNextRegValue(HKEY regFolder, const char *keyPath, const char *valueSearch, const int startIndex,
                             const int maxLength, char *retString, uint32_t *retValue) {
    bool retVal = false;
    DWORD bufLen = 1023;
    DWORD keyFlags = KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE;
    HKEY hKey = 0;
    LONG lret;

    if (g_is_wow64) {
        keyFlags |= KEY_WOW64_64KEY;
    }

    *retValue = 0;
    *retString = '\0';
    lret = RegOpenKeyExA(regFolder, keyPath, 0, keyFlags, &hKey);
    if (lret == ERROR_SUCCESS) {
        DWORD index = startIndex;
        char valueName[1024];

        do {
            DWORD type = REG_DWORD;
            DWORD value = 0;
            DWORD len = 4;
            valueName[0] = '\0';

            lret = RegEnumValueA(hKey, index, valueName, &bufLen, NULL, &type, (LPBYTE)&value, &len);
            if (ERROR_SUCCESS != lret) {
                break;
            }
            if (type == REG_DWORD) {
                *retValue = value;
            }
            if (strlen(valueSearch) == 0 || NULL != strstr(valueName, valueSearch)) {
                strncpy_s(retString, maxLength, valueName, bufLen);
                retVal = true;
                break;
            }

            bufLen = 10231;
            ++index;
        } while (true);
    }

    return retVal;
}

bool ViaSystemWindows::FindDriverIdsFromPlugAndPlay() {
    const char *software_component_guid = "{5c4c3332-344d-483c-8739-259e934c9cc8}";
    const char *display_guid = "{4d36e968-e325-11ce-bfc1-08002be10318}";
    const ULONG flags = CM_GETIDLIST_FILTER_CLASS | CM_GETIDLIST_FILTER_PRESENT;
    char child_guid[MAX_GUID_STRING_LEN + 2];  // +2 for brackets {}
    ULONG child_guid_size = sizeof(child_guid);
    DEVINST device_id = 0;
    DEVINST child_id = 0;
    char *device_names = NULL;
    ULONG device_names_size = 0;
    bool success = true;
    bool found = false;

    // Attempt to get the device names list.
    do {
        CM_Get_Device_ID_List_Size(&device_names_size, display_guid, flags);

        if (device_names != NULL) {
            free(device_names);
        }

        device_names = new char[device_names_size];
        if (nullptr == device_names) {
            goto out;
        }
    } while (CM_Get_Device_ID_List(display_guid, device_names, device_names_size, flags) == CR_BUFFER_SMALL);

    for (char *device_name = device_names; *device_name; device_name += strlen(device_name) + 1) {
        ULONG dev_node_status;
        ULONG dev_node_problem;
        // Locate the first device node and check it's status.  If no problems, then we'll want to
        // add it to the list of possible devices.
        if (CR_SUCCESS != CM_Locate_DevNode(&device_id, device_name, CM_LOCATE_DEVNODE_NORMAL)) {
            continue;
        }
        if (CR_SUCCESS != CM_Get_DevNode_Status(&dev_node_status, &dev_node_problem, device_id, 0)) {
            continue;
        }
        if ((dev_node_status & DN_HAS_PROBLEM) &&
            (dev_node_problem == CM_PROB_NEED_RESTART || dev_node_problem == DN_NEED_RESTART)) {
            continue;
        }

        // Add this device
        _device_ids.emplace_back(device_name, device_id);

        if (CR_SUCCESS != CM_Get_Child(&child_id, device_id, 0)) {
            continue;
        }

        do {
            char buffer[MAX_DEVICE_ID_LEN];
            CM_Get_Device_ID(child_id, buffer, MAX_DEVICE_ID_LEN, 0);

            if (CR_SUCCESS !=
                CM_Get_DevNode_Registry_Property(child_id, CM_DRP_CLASSGUID, NULL, &child_guid, &child_guid_size, 0)) {
                continue;
            }
            if (strcmp(child_guid, software_component_guid) != 0) {
                continue;
            }

            // Add this child as a device on interest
            _device_ids.emplace_back(buffer, device_id);

        } while (CM_Get_Sibling(&child_id, child_id, 0) == CR_SUCCESS);
    }

out:

    if (nullptr != device_names) {
        delete device_names;
    }

    return success;
}

// Find registry JSON files in the standard locations
void FindRegistryJsons(std::vector<HKEY> &registry_top_hkey, std::vector<std::string> &registry_locations,
                       std::vector<std::tuple<std::string, bool, std::string>> &json_paths) {
    uint32_t loop_size = static_cast<uint32_t>(registry_top_hkey.size());
    if (registry_top_hkey.size() > registry_locations.size()) {
        loop_size = static_cast<uint32_t>(registry_locations.size());
    }
    for (uint32_t iter = 0; iter < loop_size; iter++) {
        std::string full_registry_path;
        if (registry_top_hkey[iter] == HKEY_LOCAL_MACHINE) {
            full_registry_path = "HKEY_LOCAL_MACHINE\\";
        } else if (registry_top_hkey[iter] == HKEY_CURRENT_USER) {
            full_registry_path = "HKEY_CURRENT_USER\\";
        } else {
            full_registry_path = "<UNKNOWN>\\";
        }
        full_registry_path += registry_locations[iter];

        // Find the registry settings indicating the location of the driver
        // JSON files.
        uint32_t i = 0;
        uint32_t returned_value = 0;
        char cur_vulkan_driver_json[1024];
        while (FindNextRegValue(registry_top_hkey[iter], registry_locations[iter].c_str(), "", i, 1023, cur_vulkan_driver_json,
                                &returned_value)) {
            json_paths.emplace_back(full_registry_path, (returned_value == 0), cur_vulkan_driver_json);
            i++;
        }
    }
}

// Find registry JSON files in the driver-specific registry locations
bool ViaSystemWindows::FindDriverSpecificRegistryJsons(const std::string &key_name,
                                                       std::vector<std::tuple<std::string, bool, std::string>> &json_paths) {
    for (uint32_t device = 0; device < _device_ids.size(); device++) {
        HKEY hkey;
        DWORD required_size;
        DWORD data_type;
        char *path;
        DEVINST dev_id = std::get<1>(_device_ids[device]);
        if (CR_SUCCESS !=
            CM_Open_DevNode_Key(dev_id, KEY_QUERY_VALUE, 0, RegDisposition_OpenExisting, &hkey, CM_REGISTRY_SOFTWARE)) {
            continue;
        }

        // Figure out the size of the driver-specific path for this registry item
        if (ERROR_SUCCESS != RegQueryValueEx(hkey, key_name.c_str(), nullptr, nullptr, nullptr, &required_size)) {
            continue;
        }

        // Allocate something to temporarily store the information
        path = new char[required_size + 1];
        if (nullptr == path) {
            continue;
        }

        // Figure out the driver-specific path for this registry item
        if (ERROR_SUCCESS !=
            RegQueryValueEx(hkey, key_name.c_str(), nullptr, &data_type, reinterpret_cast<LPBYTE>(path), &required_size)) {
            delete path;
            continue;
        }
        if (data_type == REG_SZ) {
            json_paths.emplace_back(std::get<0>(_device_ids[device]), true, path);
        } else if (data_type == REG_MULTI_SZ) {
            // Grab each individual string out of the multi-string and add it here.
            for (char *cur_filename = path; cur_filename[0] != '\0'; cur_filename += strlen(cur_filename) + 1) {
                json_paths.emplace_back(std::get<0>(_device_ids[device]), true, cur_filename);
            }
        } else {
            // Bad type
        }
        delete path;
    }
    return true;
}

// Run the test in the specified directory with the corresponding
// command-line arguments.
// Returns 0 on no error, 1 if test file wasn't found, and -1
// on any other errors.
int ViaSystemWindows::RunTestInDirectory(std::string path, std::string test, std::string cmd_line) {
    int err_code = -1;
    char orig_dir[1024];
    orig_dir[0] = '\0';

    LogInfo("       Command-line: " + cmd_line);

    if (0 != GetCurrentDirectoryA(1023, orig_dir) && TRUE == SetCurrentDirectoryA(path.c_str())) {
        if (TRUE == PathFileExists(test.c_str())) {
            err_code = system(cmd_line.c_str());
        } else {
            // Path to specific exe doesn't exist
            err_code = 1;
            LogWarning(test + " not found.  Skipping.");
        }
        SetCurrentDirectoryA(orig_dir);
    } else {
        // Path to test doesn't exist.
        err_code = 1;
        LogWarning(test + " not found.  Skipping.");
    }
    return err_code;
}

// Determine what version an executable or library file is.
bool ViaSystemWindows::GetFileVersion(const std::string &filename, std::string &version_string) {
    DWORD ver_handle;
    UINT size = 0;
    LPBYTE buffer = NULL;
    DWORD ver_size = GetFileVersionInfoSize(filename.c_str(), &ver_handle);
    bool success = false;

    if (ver_size > 0) {
        LPSTR ver_data = (LPSTR)malloc(sizeof(char) * ver_size);

        if (GetFileVersionInfo(filename.c_str(), ver_handle, ver_size, ver_data)) {
            if (VerQueryValue(ver_data, "\\", (VOID FAR * FAR *)&buffer, &size)) {
                if (size) {
                    VS_FIXEDFILEINFO *ver_info = (VS_FIXEDFILEINFO *)buffer;
                    if (ver_info->dwSignature == 0xfeef04bd) {
                        version_string = std::to_string((ver_info->dwFileVersionMS >> 16) & 0xffff);
                        version_string += ".";
                        version_string += std::to_string((ver_info->dwFileVersionMS >> 0) & 0xffff);
                        version_string += ".";
                        version_string += std::to_string((ver_info->dwFileVersionLS >> 16) & 0xffff);
                        version_string += ".";
                        version_string += std::to_string((ver_info->dwFileVersionLS >> 0) & 0xffff);
                        version_string += ".";
                        success = true;
                    }
                }
            }
        }
        free(ver_data);
    }

    return success;
}

ViaSystem::ViaResults ViaSystemWindows::PrintSystemEnvironmentInfo() {
    ViaResults result = VIA_SUCCESSFUL;
    char generic_string[1024];
    std::string os_size;
    std::string converted_path;
    DWORD ser_ver = 0;
    int bytes;

    PrintBeginTable("Environment", 3);

#if _WIN64
    os_size = "64-bit";
#else
    // If WOW64 support is present, then it's a 64-bit Windows
    if (g_is_wow64) {
        os_size = "64-bit";
    } else {
        os_size = "32-bit";
    }
#endif

    ZeroMemory(&_os_version_info, sizeof(OSVERSIONINFOEX));
    _os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    ZeroMemory(&_system_info, sizeof(SYSTEM_INFO));
    GetSystemInfo(&_system_info);

    // Since this is Windows #ifdef code, determine the version of Windows
    // that the application is running on.  It's not trivial and has to
    // refer to items queried in the above structures as well as the
    // Windows registry.
    if (TRUE == GetVersionEx((LPOSVERSIONINFO)(&_os_version_info))) {
        std::string build_info;
        std::string branch_info;
        switch (_os_version_info.dwMajorVersion) {
            case 10:
                if (_os_version_info.wProductType == VER_NT_WORKSTATION) {
                    if (ReadRegKeyString(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion", "ProductName", 1023,
                                         generic_string)) {
                        _os_name = generic_string;
                        if (ReadRegKeyString(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild",
                                             1023, generic_string)) {
                            build_info = generic_string;
                        }
                        if (ReadRegKeyString(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion", "BuildBranch",
                                             1023, generic_string)) {
                            branch_info = generic_string;
                        }
                    } else {
                        _os_name = "Windows 10 (or newer)";
                    }
                } else {
                    _os_name = "Windows Server 2016 (or newer)";
                }
                break;
            case 6:
                switch (_os_version_info.dwMinorVersion) {
                    case 3:
                        if (_os_version_info.wProductType == VER_NT_WORKSTATION) {
                            if (ReadRegKeyString(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion",
                                                 "ProductName", 1023, generic_string)) {
                                _os_name = generic_string;

                                if (ReadRegKeyString(HKEY_LOCAL_MACHINE,
                                                     "Software\\Microsoft\\Windo"
                                                     "ws NT\\CurrentVersion",
                                                     "CurrentBuild", 1023, generic_string)) {
                                    build_info = generic_string;
                                }
                                if (ReadRegKeyString(HKEY_LOCAL_MACHINE,
                                                     "Software\\Microsoft\\Windo"
                                                     "ws NT\\CurrentVersion",
                                                     "BuildBranch", 1023, generic_string)) {
                                    branch_info = generic_string;
                                }
                            }
                        } else {
                            _os_name = "Windows Server 2012 R2 (or newer)";
                        }
                        break;
                    case 2:
                        if (_os_version_info.wProductType == VER_NT_WORKSTATION) {
                            if (ReadRegKeyString(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows NT\\CurrentVersion",
                                                 "ProductName", 1023, generic_string)) {
                                _os_name = generic_string;
                                if (ReadRegKeyString(HKEY_LOCAL_MACHINE,
                                                     "Software\\Microsoft\\Windo"
                                                     "ws NT\\CurrentVersion",
                                                     "CurrentBuild", 1023, generic_string)) {
                                    build_info = generic_string;
                                }
                                if (ReadRegKeyString(HKEY_LOCAL_MACHINE,
                                                     "Software\\Microsoft\\Windo"
                                                     "ws NT\\CurrentVersion",
                                                     "BuildBranch", 1023, generic_string)) {
                                    branch_info = generic_string;
                                }
                            }
                        } else {
                            _os_name = "Windows Server 2012 (or newer)";
                        }
                        break;
                    case 1:
                        if (_os_version_info.wProductType == VER_NT_WORKSTATION) {
                            _os_name = "Windows 7 (or newer)";
                        } else {
                            _os_name = "Windows Server 2008 R2 (or newer)";
                        }
                        break;
                    default:
                        if (_os_version_info.wProductType == VER_NT_WORKSTATION) {
                            _os_name = "Windows Vista (or newer)";
                        } else {
                            _os_name = "Windows Server 2008 (or newer)";
                        }
                        break;
                }
                break;
            case 5:
                ser_ver = GetSystemMetrics(SM_SERVERR2);
                switch (_os_version_info.dwMinorVersion) {
                    case 2:
                        if ((_os_version_info.wProductType == VER_NT_WORKSTATION) &&
                            (_system_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)) {
                            _os_name = "Windows XP Professional x64";
                        } else if (_os_version_info.wSuiteMask & VER_SUITE_WH_SERVER) {
                            _os_name = "Windows Home Server";
                        } else if (ser_ver != 0) {
                            _os_name = "Windows Server 2003 R2";
                        } else {
                            _os_name = "Windows Server 2003";
                        }
                        break;
                    case 1:
                        _os_name = "Windows XP";
                        break;
                    case 0:
                        _os_name = "Windows 2000";
                        break;
                    default:
                        _os_name = "Unknown Windows OS";
                        break;
                }
                break;
        }
        PrintBeginTableRow();
        PrintTableElement("Windows");
        PrintTableElement(_os_name);
        PrintTableElement(os_size);
        PrintEndTableRow();
        if (build_info.size() > 0) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Build");
            PrintTableElement(build_info);
            PrintEndTableRow();
        }
        if (branch_info.size() > 0) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Branch");
            PrintTableElement(branch_info);
            PrintEndTableRow();
        }
    } else {
        PrintBeginTableRow();
        PrintTableElement("Windows");
        PrintTableElement("Error retrieving Windows Version");
        PrintTableElement("");
        PrintEndTableRow();
        result = VIA_UNKNOWN_ERROR;
        goto out;
    }

    bytes = GetEnvironmentVariableA("SYSTEMROOT", generic_string, 1023);
    if (0 != bytes) {
        converted_path = ConvertPathFormat(generic_string);
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("System Root");
        PrintTableElement(converted_path);
        PrintEndTableRow();
    }
    bytes = GetEnvironmentVariableA("PROGRAMDATA", generic_string, 1023);
    if (0 != bytes) {
        converted_path = ConvertPathFormat(generic_string);
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("Program Data");
        PrintTableElement(converted_path);
        PrintEndTableRow();
    }
    bytes = GetEnvironmentVariableA("PROGRAMFILES", generic_string, 1023);
    if (0 != bytes) {
        converted_path = ConvertPathFormat(generic_string);
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("Program Files");
        PrintTableElement(converted_path);
        PrintEndTableRow();
    }
    bytes = GetEnvironmentVariableA("PROGRAMFILES(X86)", generic_string, 1023);
    if (0 != bytes) {
        converted_path = ConvertPathFormat(generic_string);
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("Program Files (x86)");
        PrintTableElement(converted_path);
        PrintEndTableRow();
    }
    bytes = GetEnvironmentVariableA("TEMP", generic_string, 1023);
    if (0 != bytes) {
        converted_path = ConvertPathFormat(generic_string);
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("TEMP");
        PrintTableElement(converted_path);
        PrintEndTableRow();
    }
    bytes = GetEnvironmentVariableA("TMP", generic_string, 1023);
    if (0 != bytes) {
        converted_path = ConvertPathFormat(generic_string);
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("TMP");
        PrintTableElement(converted_path);
        PrintEndTableRow();
    }
out:
    PrintEndTable();
    return result;
}

ViaSystem::ViaResults ViaSystemWindows::PrintSystemHardwareInfo() {
    ViaResults result = VIA_SUCCESSFUL;
    char generic_string[1024];
    DWORD sect_per_cluster = 0;
    DWORD bytes_per_sect = 0;
    DWORD num_free_cluster = 0;
    DWORD total_num_cluster = 0;

    MEMORYSTATUSEX mem_stat;
    ZeroMemory(&mem_stat, sizeof(MEMORYSTATUSEX));
    mem_stat.dwLength = sizeof(MEMORYSTATUSEX);

    PrintBeginTable("Hardware", 3);

    snprintf(generic_string, 1023, "%u", _system_info.dwNumberOfProcessors);
    PrintBeginTableRow();
    PrintTableElement("CPUs");
    PrintTableElement("Number of Logical Cores");
    PrintTableElement(generic_string);
    PrintEndTableRow();

    switch (_system_info.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Type");
            PrintTableElement("x86_64");
            PrintEndTableRow();
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Type");
            PrintTableElement("ARM");
            PrintEndTableRow();
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Type");
            PrintTableElement("IA64");
            PrintEndTableRow();
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Type");
            PrintTableElement("x86");
            PrintEndTableRow();
            break;
        default:
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Type");
            PrintTableElement("Unknown");
            PrintEndTableRow();
            break;
    }

    if (TRUE == GlobalMemoryStatusEx(&mem_stat)) {
        PrintBeginTableRow();
        PrintTableElement("Memory");
        PrintTableElement("Physical Available");
        if ((mem_stat.ullTotalPhys >> 40) > 0x0ULL) {
            snprintf(generic_string, 1023, "%u TB", static_cast<uint32_t>(mem_stat.ullTotalPhys >> 40));
            PrintTableElement(generic_string);
        } else if ((mem_stat.ullTotalPhys >> 30) > 0x0ULL) {
            snprintf(generic_string, 1023, "%u GB", static_cast<uint32_t>(mem_stat.ullTotalPhys >> 30));
            PrintTableElement(generic_string);
        } else if ((mem_stat.ullTotalPhys >> 20) > 0x0ULL) {
            snprintf(generic_string, 1023, "%u MB", static_cast<uint32_t>(mem_stat.ullTotalPhys >> 20));
            PrintTableElement(generic_string);
        } else if ((mem_stat.ullTotalPhys >> 10) > 0x0ULL) {
            snprintf(generic_string, 1023, "%u KB", static_cast<uint32_t>(mem_stat.ullTotalPhys >> 10));
            PrintTableElement(generic_string);
        } else {
            snprintf(generic_string, 1023, "%u bytes", static_cast<uint32_t>(mem_stat.ullTotalPhys));
            PrintTableElement(generic_string);
        }
        PrintEndTableRow();
    }

    if (TRUE == GetDiskFreeSpaceA(NULL, &sect_per_cluster, &bytes_per_sect, &num_free_cluster, &total_num_cluster)) {
        uint64_t bytes_free = (uint64_t)bytes_per_sect * (uint64_t)sect_per_cluster * (uint64_t)num_free_cluster;
        uint64_t bytes_total = (uint64_t)bytes_per_sect * (uint64_t)sect_per_cluster * (uint64_t)total_num_cluster;
        double perc_free = (double)bytes_free / (double)bytes_total;
        if ((bytes_total >> 40) > 0x0ULL) {
            PrintBeginTableRow();
            PrintTableElement("Disk Space");
            PrintTableElement("Total");
            PrintTableElement(std::to_string(static_cast<uint32_t>(bytes_total >> 40)) + " TB");
            PrintEndTableRow();
        } else if ((bytes_total >> 30) > 0x0ULL) {
            PrintBeginTableRow();
            PrintTableElement("Disk Space");
            PrintTableElement("Total");
            PrintTableElement(std::to_string(static_cast<uint32_t>(bytes_total >> 30)) + " GB");
            PrintEndTableRow();
        } else if ((bytes_total >> 20) > 0x0ULL) {
            PrintBeginTableRow();
            PrintTableElement("Disk Space");
            PrintTableElement("Total");
            PrintTableElement(std::to_string(static_cast<uint32_t>(bytes_total >> 20)) + " MB");
            PrintEndTableRow();
        } else if ((bytes_total >> 10) > 0x0ULL) {
            PrintBeginTableRow();
            PrintTableElement("Disk Space");
            PrintTableElement("Total");
            PrintTableElement(std::to_string(static_cast<uint32_t>(bytes_total >> 10)) + " KB");
            PrintEndTableRow();
        }
        snprintf(generic_string, 1023, "%4.2f%%", (static_cast<float>(perc_free) * 100.f));
        if ((bytes_free >> 40) > 0x0ULL) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Free");
            PrintTableElement(std::to_string(static_cast<uint32_t>(bytes_free >> 40)) + " TB");
            PrintEndTableRow();
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Free Perc");
            PrintTableElement(generic_string);
            PrintEndTableRow();
        } else if ((bytes_free >> 30) > 0x0ULL) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Free");
            PrintTableElement(std::to_string(static_cast<uint32_t>(bytes_free >> 30)) + " GB");
            PrintEndTableRow();
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Free Perc");
            PrintTableElement(generic_string);
            PrintEndTableRow();
        } else if ((bytes_free >> 20) > 0x0ULL) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Free");
            PrintTableElement(std::to_string(static_cast<uint32_t>(bytes_free >> 20)) + " MB");
            PrintEndTableRow();
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Free Perc");
            PrintTableElement(generic_string);
            PrintEndTableRow();
        } else if ((bytes_free >> 10) > 0x0ULL) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Free");
            PrintTableElement(std::to_string(static_cast<uint32_t>(bytes_free >> 10)) + " KB");
            PrintEndTableRow();
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Free Perc");
            PrintTableElement(generic_string);
            PrintEndTableRow();
        }
    }
    PrintEndTable();
    return result;
}

ViaSystem::ViaResults ViaSystemWindows::PrintSystemExecutableInfo() {
    ViaResults result = VIA_SUCCESSFUL;

    PrintBeginTable("Executable Info", 2);

    PrintBeginTableRow();
    PrintTableElement("Exe Directory");
    PrintTableElement(_exe_path);
    PrintEndTableRow();

    PrintBeginTableRow();
    PrintTableElement("Current Directory");
    PrintTableElement(_cur_path);
    PrintEndTableRow();

    PrintBeginTableRow();
    PrintTableElement("App Version");
    PrintTableElement(_app_version);
    PrintEndTableRow();

    PrintBeginTableRow();
    PrintTableElement("Vulkan API Version");
    PrintTableElement(std::to_string(VK_API_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE)) + "." +
        std::to_string(VK_API_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE)) + "." +
        std::to_string(VK_API_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE)));
    PrintEndTableRow();

    PrintBeginTableRow();
    PrintTableElement("Byte Format");
#if _WIN64 || __x86_64__ || __ppc64__
    PrintTableElement("64-bit");
#else
    PrintTableElement("32-bit");
#endif
    PrintEndTableRow();

    PrintEndTable();
    return result;
}

bool ViaSystemWindows::PrintDriverRegistryInfo(std::vector<std::tuple<std::string, bool, std::string>> &cur_driver_json,
                                               std::string system_path, bool &found_lib) {
    bool found_json = false;
    std::ifstream *stream = NULL;
    Json::Value root = Json::nullValue;
    Json::Value dev_exts = Json::nullValue;
    Json::Value inst_exts = Json::nullValue;
    Json::Reader reader;
    std::string full_driver_path;
    char generic_string[1024];
    uint32_t j = 0;
    std::string cur_reg_name;

    PrintBeginTableRow();
    PrintTableElement("Drivers in Registry");
    PrintTableElement("");
    PrintTableElement("");
    PrintTableElement("");
    PrintEndTableRow();

    for (uint32_t i = 0; i < cur_driver_json.size(); ++i) {
        std::string driver_json_name = std::get<0>(cur_driver_json[i]);
        std::string driver_json_path = std::get<2>(cur_driver_json[i]);
        if (cur_reg_name != driver_json_name) {
            PrintBeginTableRow();
            PrintTableElement(ConvertPathFormat(driver_json_name));
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();
            cur_reg_name = driver_json_name;
        }
        PrintBeginTableRow();
        PrintTableElement(ConvertPathFormat(driver_json_path), VIA_ALIGN_RIGHT);
        if (std::get<1>(cur_driver_json[i])) {
            PrintTableElement("Enabled");
        } else {
            PrintTableElement("Disabled");
        }
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
        cur_reg_name = driver_json_name;

        stream = new std::ifstream(driver_json_path.c_str(), std::ifstream::in);
        if (nullptr == stream || stream->fail()) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("Error reading JSON file");
            PrintTableElement(ConvertPathFormat(driver_json_path));
            PrintEndTableRow();
            continue;
        }

        if (!reader.parse(*stream, root, false) || root.isNull()) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("Error reading JSON file");
            PrintTableElement(reader.getFormattedErrorMessages());
            PrintEndTableRow();
            stream->close();
            delete stream;
            stream = NULL;
            continue;
        }

        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("JSON File Version");
        if (!root["file_format_version"].isNull()) {
            PrintTableElement(root["file_format_version"].asString());
        } else {
            PrintTableElement("MISSING!");
        }
        PrintEndTableRow();

        if (root["ICD"].isNull()) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("ICD Section");
            PrintTableElement("MISSING!");
            PrintEndTableRow();
            stream->close();
            delete stream;
            stream = NULL;
            continue;
        }

        found_json = true;

        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("API Version");
        if (!root["ICD"]["api_version"].isNull()) {
            PrintTableElement(root["ICD"]["api_version"].asString());
        } else {
            PrintTableElement("MISSING!");
        }
        PrintEndTableRow();

        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("Library Path");
        if (!root["ICD"]["library_path"].isNull()) {
            std::string driver_name = root["ICD"]["library_path"].asString();
            PrintTableElement(ConvertPathFormat(driver_name));
            PrintEndTableRow();

            if (DetermineJsonLibraryPath(driver_json_path.c_str(), driver_name.c_str(), full_driver_path)) {
                std::string system_name = system_path;
                system_name += "\\";
                system_name += driver_name;

                std::string version_string;
                if (GetFileVersion(full_driver_path, version_string)) {
                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("Library File Version");
                    PrintTableElement(version_string);
                    PrintEndTableRow();

                    found_lib = true;
                } else if (GetFileVersion(system_name.c_str(), version_string)) {
                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("Library File Version");
                    PrintTableElement(version_string);
                    PrintEndTableRow();

                    found_lib = true;
                } else {
                    snprintf(generic_string, 1023, "Failed to find driver %s or %s referenced by JSON %s",
                             ConvertPathFormat(root["ICD"]["library_path"].asString()).c_str(),
                             ConvertPathFormat(full_driver_path).c_str(), ConvertPathFormat(driver_json_path).c_str());
                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement(generic_string);
                    PrintEndTableRow();
                }
            } else {
                snprintf(generic_string, 1023, "Failed to find driver %s referenced by JSON %s",
                         ConvertPathFormat(full_driver_path).c_str(), ConvertPathFormat(driver_json_path).c_str());
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("");
                PrintTableElement("");
                PrintTableElement(generic_string);
                PrintEndTableRow();
            }
        } else {
            PrintTableElement("MISSING!");
            PrintEndTableRow();
        }

        char count_str[1024];
        j = 0;
        dev_exts = root["ICD"]["device_extensions"];
        if (!dev_exts.isNull() && dev_exts.isArray()) {
            snprintf(count_str, 1023, "%d", dev_exts.size());
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("Device Extensions");
            PrintTableElement(count_str);
            PrintEndTableRow();

            for (Json::ValueIterator dev_ext_it = dev_exts.begin(); dev_ext_it != dev_exts.end(); dev_ext_it++) {
                Json::Value dev_ext = (*dev_ext_it);
                Json::Value dev_ext_name = dev_ext["name"];
                if (!dev_ext_name.isNull()) {
                    snprintf(generic_string, 1023, "[%d]", j);

                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                    PrintTableElement(dev_ext_name.asString());
                    PrintEndTableRow();
                }
            }
        }
        inst_exts = root["ICD"]["instance_extensions"];
        j = 0;
        if (!inst_exts.isNull() && inst_exts.isArray()) {
            snprintf(count_str, 1023, "%d", inst_exts.size());
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("Instance Extensions");
            PrintTableElement(count_str);
            PrintEndTableRow();

            for (Json::ValueIterator inst_ext_it = inst_exts.begin(); inst_ext_it != inst_exts.end(); inst_ext_it++) {
                Json::Value inst_ext = (*inst_ext_it);
                Json::Value inst_ext_name = inst_ext["name"];
                if (!inst_ext_name.isNull()) {
                    snprintf(generic_string, 1023, "[%d]", j);

                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
                    PrintTableElement(inst_ext_name.asString());
                    PrintEndTableRow();
                }
            }
        }
        if (nullptr != stream) {
            stream->close();
            delete stream;
            stream = NULL;
        }
    }

    return found_json;
}

void ViaSystemWindows::PrintDriverEnvVarInfo(const char* var, const std::string& system_path, bool& found_json, bool& found_lib) {
    char env_value[1024];
    char generic_string[1024];

    // The user can override the driver file manually
    if (0 != GetEnvironmentVariableA(var, env_value, 1023) && 0 != strlen(env_value)) {
        WIN32_FIND_DATAA ffd;
        HANDLE hFind;
        uint32_t i = 0;
        char *tok = NULL;
        bool keep_looping = false;
        char full_driver_path[1024];
        bool found_this_lib = false;

        PrintBeginTableRow();
        PrintTableElement(var);
        PrintTableElement(env_value);
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();

        tok = strtok(env_value, ";");
        if (NULL != tok) {
            keep_looping = true;
            strncpy(full_driver_path, tok, 1023);
        } else {
            strncpy(full_driver_path, env_value, 1023);
        }

        do {
            snprintf(generic_string, 1023, "Driver %d", i++);
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement(generic_string, VIA_ALIGN_RIGHT);
            PrintTableElement(full_driver_path);
            PrintTableElement("");
            PrintEndTableRow();

            hFind = FindFirstFileA(full_driver_path, &ffd);
            if (hFind != INVALID_HANDLE_VALUE) {
                if (0 == (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    std::vector<std::tuple<std::string, bool, std::string>> cur_driver_jsons;
                    cur_driver_jsons.emplace_back(ffd.cFileName, true, full_driver_path);
                    if (PrintDriverRegistryInfo(cur_driver_jsons, system_path, found_this_lib)) {
                        found_json = true;
                        found_lib |= found_this_lib;
                    }
                }
                FindClose(hFind);
            } else {
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("Driver Not Found");
                PrintTableElement("");
                PrintTableElement("");
                PrintEndTableRow();
            }

            tok = strtok(NULL, ";");
            if (NULL == tok) {
                keep_looping = false;
            } else {
                strncpy(full_driver_path, tok, 1023);
            }
        } while (keep_looping);
    }
}

ViaSystem::ViaResults ViaSystemWindows::PrintSystemDriverInfo() {
    ViaResults result = VIA_SUCCESSFUL;
    PrintBeginTable("Vulkan Driver Info", 4);

    // Query any Graphics devices at this time
    if (!FindDriverIdsFromPlugAndPlay()) {
        result = VIA_MISSING_DRIVER_REGISTRY;
    } else {
        ViaResults res = VIA_SUCCESSFUL;
        std::vector<std::string> registry_locations;
        const char vulkan_public_reg_base[] = "SOFTWARE\\Khronos\\Vulkan\\Drivers";
        const char vulkan_driver_reg_key[] = "VulkanDriverName";
        const char vulkan_public_reg_base_wow64[] = "SOFTWARE\\WOW6432Node\\Khronos\\Vulkan\\Drivers";
        const char vulkan_driver_reg_key_wow64[] = "VulkanDriverNameWow";
        std::vector<std::tuple<std::string, bool, std::string>> driver_jsons;
        std::vector<HKEY> registry_top_hkey;
        std::string system_path;
        std::ifstream *stream = NULL;
        bool found_json = false;
        bool found_lib = false;
        bool found_this_lib = false;
        char generic_string[1024];
        GetEnvironmentVariableA("SYSTEMROOT", generic_string, 1024);
        system_path = generic_string;

#if _WIN64 || __x86_64__ || __ppc64__
        system_path += "\\system32\\";
        std::string reg_path = vulkan_public_reg_base;
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_LOCAL_MACHINE);
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_CURRENT_USER);
        FindDriverSpecificRegistryJsons(vulkan_driver_reg_key, driver_jsons);

#else
        if (g_is_wow64) {
            system_path += "\\sysWOW64\\";
            std::string reg_path = vulkan_public_reg_base_wow64;
            registry_locations.push_back(reg_path);
            registry_top_hkey.push_back(HKEY_LOCAL_MACHINE);
            registry_locations.push_back(reg_path);
            registry_top_hkey.push_back(HKEY_CURRENT_USER);
            FindDriverSpecificRegistryJsons(vulkan_driver_reg_key_wow64, driver_jsons);
        } else {
            system_path += "\\system32\\";
            std::string reg_path = vulkan_public_reg_base;
            registry_locations.push_back(reg_path);
            registry_top_hkey.push_back(HKEY_LOCAL_MACHINE);
            registry_locations.push_back(reg_path);
            registry_top_hkey.push_back(HKEY_CURRENT_USER);
            FindDriverSpecificRegistryJsons(vulkan_driver_reg_key, driver_jsons);
        }
#endif
        FindRegistryJsons(registry_top_hkey, registry_locations, driver_jsons);

        if (driver_jsons.size() > 0) {
            if (PrintDriverRegistryInfo(driver_jsons, system_path.c_str(), found_this_lib)) {
                found_json |= true;
                found_lib |= found_this_lib;
            }
        }
\
        PrintDriverEnvVarInfo("VK_DRIVER_FILES", system_path, found_json, found_lib);
        PrintDriverEnvVarInfo("VK_ICD_FILENAMES", system_path, found_json, found_lib);
        PrintDriverEnvVarInfo("VK_ADD_DRIVER_FILES", system_path, found_json, found_lib);
    }

    PrintEndTable();
    return result;
}

void ViaSystemWindows::PrintUninstallRegInfo(HKEY reg_folder, char *output_string, char *count_string, char *generic_string,
                                             char *version_string, unsigned int &install_count) {
    uint32_t i = 0;
    // Find all Vulkan Runtime keys in the registry, and loop through each.
    while (FindNextRegKey(reg_folder, g_uninstall_reg_path, "VulkanRT", i, 1023, output_string)) {
        snprintf(count_string, 1023, "[%d]", i++);

        snprintf(generic_string, 1023, "%s\\%s", g_uninstall_reg_path, output_string);

        // Get the version from the registry
        if (!ReadRegKeyString(reg_folder, generic_string, "DisplayVersion", 1023, version_string)) {
            strncpy(version_string, output_string, 1023);
        }

        // Get the install count for this runtime from the registry
        if (ReadRegKeyDword(reg_folder, generic_string, "InstallCount", &install_count)) {
            snprintf(output_string, 1023, "%s  [Install Count = %d]", version_string, install_count);
        } else {
            snprintf(output_string, 1023, "%s", version_string);
        }
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement(count_string, VIA_ALIGN_RIGHT);
        PrintTableElement(output_string);
        PrintEndTableRow();
    }
}

ViaSystem::ViaResults ViaSystemWindows::PrintSystemLoaderInfo() {
    ViaResults result = VIA_SUCCESSFUL;
    char generic_string[1024];
    char uninstall_version[1024];
    char count_string[1024];
    std::string version_string;
    char output_string[1024];
    char dll_search[1024];
    char dll_prefix[1024];
    uint32_t i = 0;
    uint32_t install_count = 0;
    FILE *fp = NULL;
    bool found = false;

    PrintBeginTable("Vulkan Runtimes", 3);

    PrintBeginTableRow();
    PrintTableElement("Runtimes In Registry");
    PrintTableElement(ConvertPathFormat(g_uninstall_reg_path));
    PrintTableElement("");
    PrintEndTableRow();

    PrintUninstallRegInfo(HKEY_LOCAL_MACHINE, output_string, count_string, generic_string, uninstall_version, install_count);
    PrintUninstallRegInfo(HKEY_CURRENT_USER, output_string, count_string, generic_string, uninstall_version, install_count);

    i = 0;
    GetEnvironmentVariableA("SYSTEMROOT", generic_string, 1024);
#if _WIN64 || __x86_64__ || __ppc64__
    snprintf(dll_prefix, 1023, "%s\\system32\\", generic_string);
#else
    if (g_is_wow64) {
        snprintf(dll_prefix, 1023, "%s\\sysWOW64\\", generic_string);
    } else {
        snprintf(dll_prefix, 1023, "%s\\system32\\", generic_string);
    }
#endif

    PrintBeginTableRow();
    PrintTableElement("Runtimes in System Folder");
    PrintTableElement(ConvertPathFormat(dll_prefix));
    PrintTableElement("");
    PrintEndTableRow();

    strncpy(dll_search, dll_prefix, 1023);
    strncat(dll_search, "Vulkan-*.dll", 1023);

    WIN32_FIND_DATAA ffd;
    HANDLE hFind = FindFirstFileA(dll_search, &ffd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (0 == (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                snprintf(count_string, 1023, "DLL %d", i++);

                PrintBeginTableRow();
                PrintTableElement(count_string, VIA_ALIGN_RIGHT);
                PrintTableElement(ffd.cFileName);

                snprintf(generic_string, 1023, "%s\\%s", dll_prefix, ffd.cFileName);
                if (GetFileVersion(generic_string, version_string)) {
                    snprintf(output_string, 1023, "Version %s", version_string.c_str());
                    PrintTableElement(output_string);
                } else {
                    PrintTableElement("");
                }
                PrintEndTableRow();
            }
        } while (FindNextFileA(hFind, &ffd) != 0);
        FindClose(hFind);
    }

    PrintBeginTableRow();
    PrintTableElement("Runtime Used by App");

    bool vulkan_loader_found = false;
    char vulkan_loader_path[MAX_PATH];

    DWORD processID = GetCurrentProcessId();
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    uint32_t num_mods;

    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != NULL) {
        BOOL is_wow;
        IsWow64Process(hProcess, &is_wow);
        uint32_t list_modules = LIST_MODULES_64BIT;
        if (is_wow) {
            list_modules = LIST_MODULES_32BIT;
        }
        if (EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, list_modules)) {
            num_mods = (cbNeeded / sizeof(HMODULE));
            for (uint32_t i = 0; i < num_mods; i++) {
                WCHAR szModName[MAX_PATH];
                if (GetMappedFileNameW(hProcess, hMods[i], szModName, sizeof(szModName))) {
                    if (wcsstr(szModName, L"vulkan-1.dll")) {
                        vulkan_loader_found = true;
                        // A Windows path retrieved in this way starts with a device path in a form like:
                        // "\Device\HarddiskVolume3\"
                        // This maps to a mounted drive like "C:\".
                        // We want to output the final path with the mounted drive name and not the device path.
                        // Here we get the mounted drive name the device path maps to.
                        WCHAR volume_path_name[MAX_PATH];
                        GetVolumePathNameW(szModName, volume_path_name, MAX_PATH);

                        // Now we need to find where the device path ends on our original path.
                        int count = 0;
                        uint32_t device_path_split_point = 0;
                        for (uint32_t i = 0; szModName[i] != '\0'; i++) {
                            if (szModName[i] == '\\') {
                                count++;

                                // 3 is the number of '\' characters the program will encounter
                                // before it reaches the end of the device path.
                                if (count == 3) {
                                    device_path_split_point = i + 1;
                                }
                            }
                        }

                        // Once we have the index where the device path ends and the system path begins,
                        // we can concatinate the system path onto the mounted drive name.
                        // This is the final path to the Vulkan Loader.
                        wcsncat(volume_path_name, &szModName[device_path_split_point], MAX_PATH);
                        size_t converted_chars = 0;
                        wcstombs_s(&converted_chars, vulkan_loader_path, volume_path_name, MAX_PATH);
                        break;
                    }
                }
            }
        }
        CloseHandle(hProcess);
    }

    if (vulkan_loader_found) {
        int cur_char = (int)strlen(vulkan_loader_path) - 1;
        while (vulkan_loader_path[cur_char] == '\n' || vulkan_loader_path[cur_char] == '\r' ||
               vulkan_loader_path[cur_char] == '\t' || vulkan_loader_path[cur_char] == ' ') {
            vulkan_loader_path[cur_char] = '\0';
            cur_char--;
        }

        if (GetFileVersion(vulkan_loader_path, version_string)) {
            PrintTableElement(ConvertPathFormat(vulkan_loader_path));
            PrintTableElement(version_string);
        } else {
            PrintTableElement(ConvertPathFormat(vulkan_loader_path));
            PrintTableElement("");
        }
        found = true;
    } else {
        PrintTableElement("Unknown");
        PrintTableElement("Unknown");
    }
    PrintEndTableRow();

    if (!found) {
        result = VIA_VULKAN_CANT_FIND_RUNTIME;
    }

    PrintEndTable();
    return result;
}

bool ViaSystemWindows::PrintSdkUninstallRegInfo(HKEY reg_folder, char *output_string, char *count_string, char *generic_string) {
    uint32_t i = 0;
    bool found = false;
    while (FindNextRegKey(reg_folder, g_uninstall_reg_path, "VulkanSDK", i, 1024, output_string)) {
        found = true;
        snprintf(count_string, 1023, "[%d]", i++);
        snprintf(generic_string, 1023, "%s\\%s", g_uninstall_reg_path, output_string);
        ReadRegKeyString(reg_folder, generic_string, "InstallDir", 1024, output_string);

        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement(count_string, VIA_ALIGN_RIGHT);
        PrintTableElement(ConvertPathFormat(output_string));
        PrintTableElement("");
        PrintEndTableRow();
    }
    return found;
}

bool ViaSystemWindows::PrintExplicitLayersRegInfo(std::vector<std::tuple<std::string, bool, std::string>> &cur_layer_json,
                                                  ViaSystem::ViaResults &res) {
    bool found = false;
    std::string cur_registry_loc;
    char temp_string[1024];

    PrintBeginTableRow();
    PrintTableElement("Explicit Layers in Registry");
    PrintTableElement("");
    PrintTableElement("");
    PrintTableElement("");
    PrintEndTableRow();

    for (uint32_t layer = 0; layer < static_cast<uint32_t>(cur_layer_json.size()); layer++) {
        std::string cur_layer_json_path = std::get<2>(cur_layer_json[layer]);

        if (cur_registry_loc != std::get<0>(cur_layer_json[layer])) {
            PrintBeginTableRow();
            PrintTableElement(ConvertPathFormat(std::get<0>(cur_layer_json[layer])));
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();
            cur_registry_loc = std::get<0>(cur_layer_json[layer]);
        }

        snprintf(temp_string, 1023, "[%d]", layer);
        PrintBeginTableRow();
        PrintTableElement(temp_string, VIA_ALIGN_RIGHT);
        PrintTableElement(ConvertPathFormat(cur_layer_json_path.c_str()));

        snprintf(temp_string, 1023, "0x%08x", std::get<1>(cur_layer_json[layer]));
        PrintTableElement(ConvertPathFormat(temp_string));
        PrintTableElement("");
        PrintEndTableRow();

        std::ifstream *stream = NULL;
        stream = new std::ifstream(cur_layer_json_path, std::ifstream::in);
        if (nullptr == stream || stream->fail()) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("Error reading JSON file!");
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();
        } else {
            Json::Value root = Json::nullValue;
            Json::Reader reader;
            if (!reader.parse(*stream, root, false) || root.isNull()) {
                // Report to the user the failure and their locations in the
                // document.
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("Error parsing JSON file!");
                PrintTableElement(reader.getFormattedErrorMessages());
                PrintTableElement("");
                PrintEndTableRow();
            } else {
                GenerateExplicitLayerJsonInfo(cur_layer_json_path.c_str(), root);
                found = true;
            }

            stream->close();
            delete stream;
            stream = NULL;
        }
    }
    return found;
}

bool ViaSystemWindows::PrintImplicitLayersRegInfo(std::vector<std::tuple<std::string, bool, std::string>> &cur_layer_json,
                                                  std::vector<std::string> &override_paths, ViaSystem::ViaResults &res) {
    bool found = false;
    std::string cur_registry_loc;
    char temp_string[1024];

    for (uint32_t layer = 0; layer < static_cast<uint32_t>(cur_layer_json.size()); layer++) {
        std::string cur_layer_json_path = std::get<2>(cur_layer_json[layer]);

        if (cur_registry_loc != std::get<0>(cur_layer_json[layer])) {
            PrintBeginTableRow();
            PrintTableElement(ConvertPathFormat(std::get<0>(cur_layer_json[layer])));
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();
            cur_registry_loc = std::get<0>(cur_layer_json[layer]);
        }

        snprintf(temp_string, 1023, "[%d]", layer);
        PrintBeginTableRow();
        PrintTableElement(temp_string, VIA_ALIGN_RIGHT);
        PrintTableElement(ConvertPathFormat(cur_layer_json_path).c_str());

        snprintf(temp_string, 1023, "0x%08x", std::get<1>(cur_layer_json[layer]));
        PrintTableElement(temp_string);
        PrintTableElement("");
        PrintEndTableRow();

        std::ifstream *stream = NULL;
        stream = new std::ifstream(cur_layer_json_path, std::ifstream::in);
        if (nullptr == stream || stream->fail()) {
            PrintBeginTableRow();
            PrintTableElement("");
            PrintTableElement("ERROR reading JSON file!");
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();
        } else {
            Json::Value root = Json::nullValue;
            Json::Reader reader;
            if (!reader.parse(*stream, root, false) || root.isNull()) {
                // Report to the user the failure and their locations in the
                // document.
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("ERROR parsing JSON file!");
                PrintTableElement(reader.getFormattedErrorMessages());
                PrintTableElement("");
                PrintEndTableRow();
            } else {
                GenerateImplicitLayerJsonInfo(ConvertPathFormat(cur_layer_json_path).c_str(), root, override_paths);
                found = true;
            }

            stream->close();
            delete stream;
            stream = NULL;
        }
    }
    return found;
}

ViaSystem::ViaResults ViaSystemWindows::PrintSystemSdkInfo() {
    ViaResults result = VIA_SUCCESSFUL;
    char generic_string[1024];
    char count_string[1024];
    char output_string[1024];
    char sdk_env_dir[1024];
    uint32_t i = 0;
    uint32_t j = 0;
    FILE *fp = NULL;
    bool found = false;
    std::vector<HKEY> registry_top_hkey;
    std::vector<std::string> registry_locations;
    std::vector<std::tuple<std::string, bool, std::string>> layer_jsons;
    const char vulkan_public_reg_base[] = "SOFTWARE\\Khronos\\Vulkan\\ExplicitLayers";
    const char vulkan_driver_reg_key[] = "VulkanExplicitLayerPaths";
    const char vulkan_public_reg_base_wow64[] = "SOFTWARE\\WOW6432Node\\Khronos\\Vulkan\\ExplicitLayers";
    const char vulkan_driver_reg_key_wow64[] = "VulkanExplicitLayerPathsWow";

    if (g_is_wow64) {
        std::string reg_path = vulkan_public_reg_base_wow64;
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_LOCAL_MACHINE);
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_CURRENT_USER);
        FindDriverSpecificRegistryJsons(vulkan_driver_reg_key_wow64, layer_jsons);
    } else {
        std::string reg_path = vulkan_public_reg_base;
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_LOCAL_MACHINE);
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_CURRENT_USER);
        FindDriverSpecificRegistryJsons(vulkan_driver_reg_key, layer_jsons);
    }
    FindRegistryJsons(registry_top_hkey, registry_locations, layer_jsons);

    PrintBeginTable("Vulkan SDKs", 4);

    PrintBeginTableRow();
    PrintTableElement("SDKs Found In Registry");
    PrintTableElement(ConvertPathFormat(g_uninstall_reg_path));
    PrintTableElement("");
    PrintTableElement("");
    PrintEndTableRow();

    found |= PrintSdkUninstallRegInfo(HKEY_LOCAL_MACHINE, output_string, count_string, generic_string);
    found |= PrintSdkUninstallRegInfo(HKEY_CURRENT_USER, output_string, count_string, generic_string);

    if (!found) {
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("NONE FOUND", VIA_ALIGN_RIGHT);
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
    }

    if (0 != GetEnvironmentVariableA("VK_SDK_PATH", sdk_env_dir, 1023)) {
        PrintBeginTableRow();
        PrintTableElement("VK_SDK_PATH");
        _found_sdk = true;
        _sdk_path = sdk_env_dir;
        PrintTableElement(ConvertPathFormat(sdk_env_dir));
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
    } else if (0 != GetEnvironmentVariableA("VULKAN_SDK", sdk_env_dir, 1023)) {
        PrintBeginTableRow();
        PrintTableElement("VULKAN_SDK");
        _found_sdk = true;
        _sdk_path = sdk_env_dir;
        PrintTableElement(ConvertPathFormat(sdk_env_dir));
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
    } else {
        PrintBeginTableRow();
        PrintTableElement("VK_SDK_PATH");
        PrintTableElement("No installed SDK");
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
    }

    PrintBeginTableRow();
    PrintTableElement("SDK Explicit Layers");
    PrintTableElement(ConvertPathFormat(generic_string));
    PrintTableElement("");
    PrintTableElement("");
    PrintEndTableRow();

    if (layer_jsons.size() > 0) {
        found |= PrintExplicitLayersRegInfo(layer_jsons, result);
    }

    if (!found) {
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("NONE FOUND", VIA_ALIGN_RIGHT);
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
    }

    PrintEndTable();
    return result;
}

ViaSystem::ViaResults ViaSystemWindows::PrintSystemImplicitLayerInfo() {
    ViaResults result = VIA_SUCCESSFUL;
    PrintBeginTable("Vulkan Implicit Layers", 4);

    FILE *fp = NULL;
    std::vector<HKEY> registry_top_hkey;
    std::vector<std::string> registry_locations;
    std::vector<std::tuple<std::string, bool, std::string>> layer_jsons;
    std::string system_path;
    char generic_string[1024];
    const char vulkan_public_reg_base[] = "SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers";
    const char vulkan_implicit_reg_key[] = "VulkanImplicitLayerPaths";
    const char vulkan_driver_reg_key[] = "VulkanImplicitLayers";
    const char vulkan_public_reg_base_wow64[] = "SOFTWARE\\WOW6432Node\\Khronos\\Vulkan\\ImplicitLayers";
    const char vulkan_implicit_reg_key_wow64[] = "VulkanImplicitLayerPathsWow";
    const char vulkan_driver_reg_key_wow64[] = "VulkanImplicitLayersWow";
    std::string reg_path;

    GetEnvironmentVariableA("SYSTEMROOT", generic_string, 1024);
    system_path = generic_string;

    if (g_is_wow64) {
        system_path += "\\sysWOW64\\";
        reg_path = vulkan_public_reg_base_wow64;
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_LOCAL_MACHINE);
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_CURRENT_USER);
        FindDriverSpecificRegistryJsons(vulkan_driver_reg_key_wow64, layer_jsons);
    } else {
        system_path += "\\system32\\";
        reg_path = vulkan_public_reg_base;
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_LOCAL_MACHINE);
        registry_locations.push_back(reg_path);
        registry_top_hkey.push_back(HKEY_CURRENT_USER);
        FindDriverSpecificRegistryJsons(vulkan_driver_reg_key, layer_jsons);
    }
    FindRegistryJsons(registry_top_hkey, registry_locations, layer_jsons);

    if (g_is_wow64) {
        FindDriverSpecificRegistryJsons(vulkan_implicit_reg_key_wow64, layer_jsons);
    } else {
        FindDriverSpecificRegistryJsons(vulkan_implicit_reg_key, layer_jsons);
    }
    FindRegistryJsons(registry_top_hkey, registry_locations, layer_jsons);

    if (layer_jsons.size() > 0) {
        PrintImplicitLayersRegInfo(layer_jsons, _layer_override_search_path, result);
    }

    PrintEndTable();
    return result;
}

ViaSystem::ViaResults ViaSystemWindows::FindAndPrintAllExplicitLayersInPath(const std::string &layer_path) {
    WIN32_FIND_DATAA ffd;
    HANDLE hFind;
    uint32_t i = 0;
    ViaResults res = VIA_SUCCESSFUL;
    char full_layer_path[1024];

    // Look for any JSON files in that folder.
    snprintf(full_layer_path, 1023, "%s\\*.json", layer_path.c_str());
    hFind = FindFirstFileA(full_layer_path, &ffd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (0 == (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string index_string = "[";
                index_string += std::to_string(i++);
                index_string += "]";
                std::string cur_json_path = layer_path;
                cur_json_path += "\\";
                cur_json_path += ffd.cFileName;

                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement(index_string, VIA_ALIGN_RIGHT);
                PrintTableElement(ffd.cFileName);
                PrintTableElement("");
                PrintEndTableRow();

                std::ifstream *stream = NULL;
                stream = new std::ifstream(cur_json_path, std::ifstream::in);
                if (nullptr == stream || stream->fail()) {
                    PrintBeginTableRow();
                    PrintTableElement("");
                    PrintTableElement("");
                    PrintTableElement("ERROR reading JSON file!");
                    PrintTableElement("");
                    PrintEndTableRow();
                } else {
                    Json::Value root = Json::nullValue;
                    Json::Reader reader;
                    if (!reader.parse(*stream, root, false) || root.isNull()) {
                        // Report to the user the failure and their
                        // locations in the document.
                        PrintBeginTableRow();
                        PrintTableElement("");
                        PrintTableElement("");
                        PrintTableElement("ERROR parsing JSON file!");
                        PrintTableElement(reader.getFormattedErrorMessages());
                        PrintEndTableRow();
                    } else {
                        GenerateExplicitLayerJsonInfo(cur_json_path.c_str(), root);
                    }

                    stream->close();
                    delete stream;
                    stream = NULL;
                }
            }
        } while (FindNextFileA(hFind, &ffd) != 0);

        FindClose(hFind);
    }

    return res;
}

ViaSystem::ViaResults ViaSystemWindows::PrintLayerEnvVar(const char* var, bool& printed_more_layers) {
    ViaResults result = VIA_SUCCESSFUL;
    char generic_string[1024];

    // If the user's system has the provied environment variable set, dump out the layer
    // information found in that folder.  This is important because if
    // a user is having problems with the layers, they may be using
    // non-standard layers.
    if (0 != GetEnvironmentVariableA(var, generic_string, 1023)) {
        std::string cur_layer_path;
        bool keep_looping = false;
        uint32_t path = 0;

        PrintBeginTableRow();
        PrintTableElement(var);
        PrintTableElement(generic_string);
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();

        // Variable may have multiple folders listed in it (colon
        // ';' delimited)
        char *tok = strtok(generic_string, ";");
        if (tok != NULL) {
            cur_layer_path = tok;
            keep_looping = true;
        } else {
            cur_layer_path = generic_string;
        }

        do {
            if (keep_looping) {
                PrintBeginTableRow();
                sprintf(generic_string, "Path %d", path++);
                PrintTableElement("");
                PrintTableElement(generic_string, VIA_ALIGN_CENTER);
                PrintTableElement(cur_layer_path);
                PrintTableElement("");
                PrintEndTableRow();
            }

            result = FindAndPrintAllExplicitLayersInPath(cur_layer_path);

            tok = strtok(NULL, ";");
            if (tok == NULL) {
                keep_looping = false;
            } else {
                cur_layer_path = tok;
            }
        } while (keep_looping);
        printed_more_layers = true;
    }
    return result;
}

ViaSystem::ViaResults ViaSystemWindows::PrintSystemExplicitLayerInfo() {
    ViaResults result = VIA_SUCCESSFUL;
    bool printed_more_layers = false;

    PrintBeginTable("Vulkan Explicit Layers", 4);

    if (0 != _layer_override_search_path.size()) {
        for (uint32_t cur_path = 0; cur_path < _layer_override_search_path.size(); cur_path++) {
            PrintBeginTableRow();
            PrintTableElement("Override Path");
            PrintTableElement(_layer_override_search_path[cur_path]);
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();

            result = FindAndPrintAllExplicitLayersInPath(_layer_override_search_path[cur_path]);
        }
        printed_more_layers = true;
    }

    ViaResults tmp_result = PrintLayerEnvVar("VK_LAYER_PATH", printed_more_layers);
    if (tmp_result != VIA_SUCCESSFUL) {
        result = tmp_result;
    }
    tmp_result = PrintLayerEnvVar("VK_ADD_LAYER_PATH", printed_more_layers);
    if (tmp_result != VIA_SUCCESSFUL) {
        result = tmp_result;
    }

    if (!printed_more_layers) {
        PrintBeginTableRow();
        PrintTableElement("No additional Explicit Layers Defined");
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
    }

    PrintEndTable();
    return result;
}

void ViaSystemWindows::PrintFileVersionInfo(const std::string &json_filename, const std::string &library) {
    // On Windows, we can query the file version, so do so.
    std::string full_layer_path;
    std::string file_version;
    if (DetermineJsonLibraryPath(json_filename, library, full_layer_path) && GetFileVersion(full_layer_path, file_version)) {
        PrintBeginTableRow();
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("Layer File Version");
        PrintTableElement(file_version);
        PrintEndTableRow();
    }
}

bool ViaSystemWindows::CheckExpiration(ViaSystem::OverrideExpiration expiration) {
    bool still_valid = false;
    uint16_t year = 0;
    uint8_t month = 0;
    uint8_t day = 0;
    uint8_t hour = 0;
    uint8_t minute = 0;

    SYSTEMTIME cur_time;
    GetSystemTime(&cur_time);
    year = static_cast<uint16_t>(cur_time.wYear);
    month = static_cast<uint8_t>(cur_time.wMonth);
    day = static_cast<uint8_t>(cur_time.wDay);
    hour = static_cast<uint8_t>(cur_time.wHour);
    minute = static_cast<uint8_t>(cur_time.wMinute);

    if (expiration.year > year || expiration.month > month || expiration.day > day || expiration.hour > hour ||
        expiration.minute > minute) {
        still_valid = true;
    }

    return still_valid;
}

ViaSystem::ViaResults ViaSystemWindows::PrintSystemSettingsFileInfo() {
    ViaResults result = VIA_SUCCESSFUL;
    const char settings_file_name[] = "vk_layer_settings.txt";
    std::vector<std::string> settings_files;

    PrintBeginTable("Vulkan Layer Settings File", 4);

    // If the settings path environment variable is set, use that.
    char generic_string[1024];
    if (0 != GetEnvironmentVariableA("VK_LAYER_SETTINGS_PATH", generic_string, 1023)) {
        std::string full_file = generic_string;
        full_file += '\\';
        full_file += settings_file_name;

        PrintBeginTableRow();
        PrintTableElement("VK_LAYER_SETTINGS_PATH");
        PrintTableElement(ConvertPathFormat(generic_string));
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
        GenerateSettingsFileJsonInfo(full_file);
    } else {
        PrintBeginTableRow();
        PrintTableElement("VK_LAYER_SETTINGS_PATH");
        PrintTableElement("Not Defined");
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
        const char vulkan_reg_base[] = "SOFTWARE\\Khronos\\Vulkan\\Settings";
        const char vulkan_reg_base_wow64[] = "SOFTWARE\\WOW6432Node\\Khronos\\Vulkan\\Settings";
        std::vector<std::string> registry_locations;
        std::vector<HKEY> registry_top_hkey;

        if (g_is_wow64) {
            std::string reg_path = vulkan_reg_base_wow64;
            registry_locations.push_back(reg_path);
            registry_top_hkey.push_back(HKEY_LOCAL_MACHINE);
            registry_locations.push_back(reg_path);
            registry_top_hkey.push_back(HKEY_CURRENT_USER);
        } else {
            std::string reg_path = vulkan_reg_base;
            registry_locations.push_back(reg_path);
            registry_top_hkey.push_back(HKEY_LOCAL_MACHINE);
            registry_locations.push_back(reg_path);
            registry_top_hkey.push_back(HKEY_CURRENT_USER);
        }

        size_t loop_size = registry_top_hkey.size();
        if (registry_top_hkey.size() > registry_locations.size()) {
            loop_size = registry_locations.size();
        }
        PrintBeginTableRow();
        PrintTableElement("Settings in Registry");
        PrintTableElement("");
        PrintTableElement("");
        PrintTableElement("");
        PrintEndTableRow();
        for (uint32_t iter = 0; iter < static_cast<uint32_t>(loop_size); iter++) {
            std::string full_registry_path;
            if (registry_top_hkey[iter] == HKEY_LOCAL_MACHINE) {
                full_registry_path = "HKEY_LOCAL_MACHINE\\";
            } else if (registry_top_hkey[iter] == HKEY_CURRENT_USER) {
                full_registry_path = "HKEY_CURRENT_USER\\";
            } else {
                full_registry_path = "<UNKNOWN>\\";
            }
            full_registry_path += registry_locations[iter];
            PrintBeginTableRow();
            PrintTableElement(ConvertPathFormat(full_registry_path), VIA_ALIGN_RIGHT);
            PrintTableElement("");
            PrintTableElement("");
            PrintTableElement("");
            PrintEndTableRow();

            // Find the registry settings indicating the location of the settings JSON files.
            uint32_t i = 0;
            uint32_t returned_value = 0;
            bool printed = false;
            char cur_vulkan_driver_json[1024];
            while (FindNextRegValue(registry_top_hkey[iter], registry_locations[iter].c_str(), "", i, 1023, cur_vulkan_driver_json,
                                    &returned_value)) {
                GenerateSettingsFileJsonInfo(cur_vulkan_driver_json);
                i++;
                printed = true;
            }
            if (!printed) {
                PrintBeginTableRow();
                PrintTableElement("");
                PrintTableElement("No Settings Found");
                PrintTableElement("");
                PrintTableElement("");
                PrintEndTableRow();
            }
        }
    }

    PrintEndTable();

    return result;
}

std::string ViaSystemWindows::GetEnvironmentalVariableValue(const std::string &env_var) {
    std::string return_value;
    char temp_buffer[1024];
    if (0 != GetEnvironmentVariableA(env_var.c_str(), temp_buffer, 1023)) {
        return_value = temp_buffer;
    }
    return return_value;
}

bool ViaSystemWindows::ExpandPathWithEnvVar(std::string &path) {
    char temporary_path[8192];
    if (!ExpandEnvironmentStrings(path.c_str(), temporary_path, 8191)) {
        return false;
    }
    path = temporary_path;
    return true;
}

#endif  // VIA_WINDOWS_TARGET
