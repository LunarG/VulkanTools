/*
 * Copyright (c) 2019 ARM Limited. All rights reserved.
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
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <cstring>

#include <unordered_map>

#include "vktrace_common.h"
#include "vktrace_tracelog.h"
#include "vktrace_filelike.h"
#include "vktrace_trace_packet_utils.h"
#include "vktrace_vk_packet_id.h"

#include "vktracedump_main.h"

using namespace std;

struct vktracedump_params {
    const char* traceFile = NULL;
    const char* simpleDumpFile = NULL;
    const char* fullDumpFile = NULL;
    bool noAddr = false;
    bool dumpShader = false;
    bool saveAsHtml = false;
} g_params;

const char* DUMP_SETTING_FILE = "vk_layer_settings.txt";
const char* FULL_DUMP_FILE_UNSET = "full_dump_file_unset";
const char* SEPARATOR = " : ";
const char* SPACES = "               ";
const uint32_t COLUMN_WIDTH = 15;

static void print_usage() {
    cout << "vktracedump available options:" << endl;
    cout << "    -o <traceFile>        The trace file to open and parse" << endl;
    cout << "    -s <simpleDumpFile>   (Optional) The file to save the outputs of simple/brief API dump. Use 'stdout' to send "
            "outputs to stdout."
         << endl;
    cout << "    -f <fullDumpFile>     (Optional) The file to save the outputs of full/detailed API dump. Use 'stdout' to send "
            "outputs to stdout."
         << endl;
    cout << "    -ds                   Dump the shader binary code in pCode to shader dump files shader_<index>.hex.  Only works "
            "with \"-f <fullDumpFile>\" option."
         << endl;
    cout << "                          The file name shader_<index>.hex can be found in pCode in the <fullDumpFile> to associate "
            "with vkCreateShaderModule."
         << endl;
    cout << "    -dh                   Save full/detailed API dump as HTML format. (Default is text format)  Only works with \"-f "
            "<fullDumpFile>\" option."
         << endl;
    cout << "    -na                   Dump string \"address\" in place of hex addresses. (Default is false)  Only works with \"-f "
            "<fullDumpFile>\" option."
         << endl;
}

static int parse_args(int argc, char** argv) {
    for (int i = 1; i < argc;) {
        string arg(argv[i]);
        if (arg.compare("-o") == 0) {
            const char* traceFile = argv[i + 1];
            g_params.traceFile = traceFile;
            i = i + 2;
        } else if (arg.compare("-s") == 0) {
            g_params.simpleDumpFile = argv[i + 1];
            i = i + 2;
        } else if (arg.compare("-f") == 0) {
            g_params.fullDumpFile = argv[i + 1];
            i = i + 2;
        } else if (arg.compare("-ds") == 0) {
            g_params.dumpShader = true;
            i++;
        } else if (arg.compare("-dh") == 0) {
            g_params.saveAsHtml = true;
            i++;
        } else if (arg.compare("-na") == 0) {
            g_params.noAddr = true;
            i++;
        } else if (arg.compare("-h") == 0) {
            print_usage();
            exit(0);
        } else {
            return -1;
        }
    }
    if ((g_params.saveAsHtml || g_params.dumpShader || g_params.noAddr) && (g_params.fullDumpFile == NULL)) {
        // saveAsHtml, dumpShader and noAddress should be used with valid fullDumpFile option.
        return -1;
    }
    if (g_params.traceFile == NULL) {
        // traceFile option must be specified.
        return -1;
    }

    return 0;
}

static void dump_packet_brief(ostream& dumpFile, uint32_t frameNumber, vktrace_trace_packet_header* packet,
                              uint64_t currentPosition) {
    static size_t index = 0;
    static bool skipApi = false;
    if (index == 0) {
        dumpFile << setw(COLUMN_WIDTH) << "frame" << SEPARATOR << setw(COLUMN_WIDTH) << "thread id" << SEPARATOR
                 << setw(COLUMN_WIDTH) << "packet index" << SEPARATOR << setw(COLUMN_WIDTH) << "global pack id" << SEPARATOR
                 << setw(COLUMN_WIDTH) << "pack position" << SEPARATOR << setw(COLUMN_WIDTH) << "pack byte size" << SEPARATOR
                 << "API Call" << endl;
    }
    if (packet->packet_id != VKTRACE_TPI_VK_vkGetInstanceProcAddr && packet->packet_id != VKTRACE_TPI_VK_vkGetDeviceProcAddr) {
        skipApi = false;
        dumpFile << setw(COLUMN_WIDTH) << dec << frameNumber << SEPARATOR << setw(COLUMN_WIDTH) << dec << packet->thread_id
                 << SEPARATOR << setw(COLUMN_WIDTH) << dec << index << SEPARATOR << setw(COLUMN_WIDTH) << dec
                 << packet->global_packet_index << SEPARATOR << setw(COLUMN_WIDTH) << dec << currentPosition << SEPARATOR
                 << setw(COLUMN_WIDTH) << dec << packet->size << SEPARATOR
                 << vktrace_stringify_vk_packet_id((VKTRACE_TRACE_PACKET_ID_VK)packet->packet_id, packet) << endl;
    } else {
        if (!skipApi) {
            dumpFile << setw(COLUMN_WIDTH) << dec << frameNumber << SEPARATOR << SPACES << SEPARATOR << SPACES << SEPARATOR
                     << SPACES << SEPARATOR << SPACES << SEPARATOR << SPACES << SEPARATOR << "Skip "
                     << vktrace_vk_packet_id_name((VKTRACE_TRACE_PACKET_ID_VK)packet->packet_id) << " call(s)!" << endl;
            skipApi = true;
        }
    }
    index++;
}

static void dump_full_setup() {
    if (g_params.fullDumpFile) {
        // Remove existing dump setting file before creating a new one
        remove(DUMP_SETTING_FILE);

        vktrace_set_global_var("VK_LAYER_PATH", "");
        vktrace_set_global_var("VK_LAYER_SETTINGS_PATH", "");
        // Generate a default vk_layer_settings.txt if it does not exist or failed to be opened.
        ofstream settingFile;
        settingFile.open(DUMP_SETTING_FILE);
        settingFile << "#  ==============" << endl;
        settingFile << "#  lunarg_api_dump.output_format : Specifies the format used for output, can be Text (default -- "
                       "outputs plain text) or Html."
                    << endl;
        settingFile << "#  lunarg_api_dump.detailed : Setting this to TRUE causes parameter details to be dumped in addition "
                       "to API calls."
                    << endl;
        settingFile
            << "#  lunarg_api_dump.no_addr : Setting this to TRUE causes \"address\" to be dumped in place of hex addresses."
            << endl;
        settingFile << "#  lunarg_api_dump.file : Setting this to TRUE indicates that output should be written to file instead "
                       "of STDOUT."
                    << endl;
        settingFile << "#  lunarg_api_dump.log_filename : Specifies the file to dump to when \"file = TRUE\"." << endl;
        settingFile << "#  lunarg_api_dump.flush : Setting this to TRUE causes IO to be flushed each API call that is written."
                    << endl;
        settingFile << "#  lunarg_api_dump.indent_size : Specifies the number of spaces that a tab is equal to." << endl;
        settingFile << "#  lunarg_api_dump.show_types : Setting this to TRUE causes types to be dumped in addition to values."
                    << endl;
        settingFile << "#  lunarg_api_dump.name_size : The number of characters the name of a variable should consume, "
                       "assuming more are not required."
                    << endl;
        settingFile << "#  lunarg_api_dump.type_size : The number of characters the type of a variable should consume, "
                       "assuming more are not requires."
                    << endl;
        settingFile << "#  lunarg_api_dump.use_spaces : Setting this to TRUE causes all tabs characters to be replaced with spaces."
                    << endl;
        settingFile << "#  lunarg_api_dump.show_shader : Setting this to TRUE causes the shader binary code in pCode to be "
                       "also written to output."
                    << endl;
        settingFile << "#  ==============" << endl;
        if (g_params.saveAsHtml) {
            settingFile << "lunarg_api_dump.output_format = Html" << endl;
        } else {
            settingFile << "lunarg_api_dump.output_format = Text" << endl;
        }
        settingFile << "lunarg_api_dump.detailed = TRUE" << endl;
        settingFile << "lunarg_api_dump.no_addr = " << (g_params.noAddr ? "TRUE" : "FALSE") << endl;
        if (g_params.fullDumpFile && (!strcmp(g_params.fullDumpFile, "STDOUT") || !strcmp(g_params.fullDumpFile, "stdout"))) {
            settingFile << "lunarg_api_dump.file = FALSE" << endl;
        } else {
            settingFile << "lunarg_api_dump.file = TRUE" << endl;
        }
        settingFile << "lunarg_api_dump.log_filename = " << g_params.fullDumpFile << endl;
        settingFile << "lunarg_api_dump.flush = TRUE" << endl;
        settingFile << "lunarg_api_dump.indent_size = 4" << endl;
        settingFile << "lunarg_api_dump.show_types = TRUE" << endl;
        settingFile << "lunarg_api_dump.name_size = 32" << endl;
        settingFile << "lunarg_api_dump.type_size = 0" << endl;
        settingFile << "lunarg_api_dump.use_spaces = TRUE" << endl;
        if (g_params.dumpShader) {
            settingFile << "lunarg_api_dump.show_shader = TRUE" << endl;
        } else {
            settingFile << "lunarg_api_dump.show_shader = FALSE" << endl;
        }
        settingFile.close();
    }
}

int main(int argc, char** argv) {
    if (parse_args(argc, argv) < 0) {
        cout << "Error: invalid parameters!" << endl;
        print_usage();
        return -1;
    }

    FILE* tracefp = fopen(g_params.traceFile, "rb");
    if (tracefp == NULL) {
        cout << "Error: Open trace file \"" << g_params.traceFile << "\" fail !" << endl;
        return -1;
    }

    int ret = 0;
    FileLike* traceFile = NULL;
    traceFile = vktrace_FileLike_create_file(tracefp);

    vktrace_trace_file_header fileHeader;
    struct_gpuinfo gpuInfo;

    if (vktrace_FileLike_ReadRaw(traceFile, &fileHeader, sizeof(fileHeader))) {
        if (fileHeader.magic != VKTRACE_FILE_MAGIC) {
            cout << "\"" << g_params.traceFile << "\" "
                 << "is not a vktrace file !" << endl;
            ret = -1;
        } else if (sizeof(void*) != fileHeader.ptrsize) {
            cout << "Error: " << fileHeader.ptrsize * 8 << "bit trace file cannot be parsed by " << sizeof(void*) * 8
                 << "bit vktracedump!" << endl;
            ret = -1;
        } else {
            streambuf* buf = NULL;
            ofstream fileOutput;
            if (g_params.simpleDumpFile) {
                if (!strcmp(g_params.simpleDumpFile, "STDOUT") || !strcmp(g_params.simpleDumpFile, "stdout")) {
                    buf = cout.rdbuf();
                } else {
                    fileOutput.open(g_params.simpleDumpFile);
                    buf = fileOutput.rdbuf();
                }
            }
            ostream simpleDumpFile(buf);

            if (g_params.fullDumpFile) {
                dump_full_setup();
            }
            cout << setw(COLUMN_WIDTH) << left << "File Version:" << fileHeader.trace_file_version << endl;
            cout << setw(COLUMN_WIDTH) << left << "File Type:" << fileHeader.ptrsize * 8 << "bit" << endl;
            cout << setw(COLUMN_WIDTH) << left << "Arch:" << (char*)&fileHeader.arch << endl;
            cout << setw(COLUMN_WIDTH) << left << "OS:" << (char*)&fileHeader.os << endl;
            cout << setw(COLUMN_WIDTH) << left << "Endianess:" << (fileHeader.endianess ? "Big" : "Little") << endl;
            if (fileHeader.n_gpuinfo < 1 || fileHeader.n_gpuinfo > 1) {
                cout << "Warning: number of gpu info = " << fileHeader.n_gpuinfo << endl;
            }
            for (uint32_t i = 0; i < fileHeader.n_gpuinfo; i++) {
                if (vktrace_FileLike_ReadRaw(traceFile, &gpuInfo, sizeof(gpuInfo))) {
                    cout << setw(COLUMN_WIDTH) << left << "Vendor ID:"
                         << "0x" << hex << uppercase << (gpuInfo.gpu_id >> 32) << endl;
                    cout << setw(COLUMN_WIDTH) << left << "Device ID:"
                         << "0x" << hex << uppercase << (gpuInfo.gpu_id & UINT32_MAX) << endl;
                    cout << setw(COLUMN_WIDTH) << left << "Driver Ver:"
                         << "0x" << hex << uppercase << gpuInfo.gpu_drv_vers << endl;
                } else {
                    cout << "Error: Read gpu info fail!" << endl;
                    ret = -1;
                }
            }
            if (ret > -1) {
                uint32_t frameNumber = 0;
                uint32_t apiVersion = UINT32_MAX;
                char* pApplicationName = NULL;
                uint32_t applicationVersion = 0;
                char* pEngineName = NULL;
                uint32_t engineVersion = 0;
                char deviceName[VK_MAX_PHYSICAL_DEVICE_NAME_SIZE] = "";
                while (true) {
                    uint64_t currentPosition = vktrace_FileLike_GetCurrentPosition(traceFile);
                    vktrace_trace_packet_header* packet = vktrace_read_trace_packet(traceFile);
                    if (!packet) break;
                    if (packet->packet_id >= VKTRACE_TPI_VK_vkApiVersion) {
                        vktrace_trace_packet_header* pInterpretedHeader = interpret_trace_packet_vk(packet);
                        if (g_params.simpleDumpFile) {
                            dump_packet_brief(simpleDumpFile, frameNumber, pInterpretedHeader, currentPosition);
                        }
                        if (g_params.fullDumpFile) {
                            dump_packet(pInterpretedHeader);
                        }
                        switch (pInterpretedHeader->packet_id) {
                            case VKTRACE_TPI_VK_vkQueuePresentKHR: {
                                frameNumber++;
                            } break;
                            case VKTRACE_TPI_VK_vkGetPhysicalDeviceProperties: {
                                if (apiVersion == UINT32_MAX) {
                                    packet_vkGetPhysicalDeviceProperties* pPacket =
                                        (packet_vkGetPhysicalDeviceProperties*)(pInterpretedHeader->pBody);
                                    apiVersion = pPacket->pProperties->apiVersion;
                                    memcpy(deviceName, pPacket->pProperties->deviceName, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE);
                                }
                            } break;
                            case VKTRACE_TPI_VK_vkCreateInstance: {
                                packet_vkCreateInstance* pPacket = (packet_vkCreateInstance*)(pInterpretedHeader->pBody);
                                if (pApplicationName == NULL && pEngineName == NULL && pPacket->pCreateInfo->pApplicationInfo) {
                                    if (pPacket->pCreateInfo->pApplicationInfo->pApplicationName) {
                                        size_t applicationNameLen =
                                            strlen(pPacket->pCreateInfo->pApplicationInfo->pApplicationName);
                                        pApplicationName = (char*)malloc(applicationNameLen + 1);
                                        memcpy(pApplicationName, pPacket->pCreateInfo->pApplicationInfo->pApplicationName,
                                               applicationNameLen);
                                        pApplicationName[applicationNameLen] = '\0';
                                    }
                                    applicationVersion = pPacket->pCreateInfo->pApplicationInfo->applicationVersion;
                                    if (pPacket->pCreateInfo->pApplicationInfo->pEngineName) {
                                        size_t engineNameLen = strlen(pPacket->pCreateInfo->pApplicationInfo->pEngineName);
                                        pEngineName = (char*)malloc(engineNameLen + 1);
                                        memcpy(pEngineName, pPacket->pCreateInfo->pApplicationInfo->pEngineName, engineNameLen);
                                        pEngineName[engineNameLen] = '\0';
                                    }
                                    engineVersion = pPacket->pCreateInfo->pApplicationInfo->engineVersion;
                                }
                            } break;
                            default:
                                break;
                        }
                    }
                    vktrace_delete_trace_packet_no_lock(&packet);
                }
                if (apiVersion != UINT32_MAX) {
                    cout << setw(COLUMN_WIDTH) << left << "API Ver:" << dec << VK_VERSION_MAJOR(apiVersion) << "." << dec
                         << VK_VERSION_MINOR(apiVersion) << "." << dec << VK_VERSION_PATCH(apiVersion) << endl;
                    cout << setw(COLUMN_WIDTH) << left << "Device Name:" << deviceName << endl;
                }
                if (pApplicationName) {
                    cout << setw(COLUMN_WIDTH) << left << "App Name:" << pApplicationName << endl;
                    free(pApplicationName);
                    pApplicationName = NULL;
                } else {
                    cout << setw(COLUMN_WIDTH) << left << "App Name:"
                         << "NULL" << endl;
                }
                cout << setw(COLUMN_WIDTH) << left << "App Ver:" << applicationVersion << endl;
                if (pEngineName) {
                    cout << setw(COLUMN_WIDTH) << left << "Engine Name:" << pEngineName << endl;
                    free(pEngineName);
                    pEngineName = NULL;
                } else {
                    cout << setw(COLUMN_WIDTH) << left << "Engine Name:"
                         << "NULL" << endl;
                }
                cout << setw(COLUMN_WIDTH) << left << "Engine Ver:" << engineVersion << endl;
                cout << setw(COLUMN_WIDTH) << left << "Frames:" << frameNumber << endl;
            }
            if (g_params.simpleDumpFile && !strcmp(g_params.simpleDumpFile, "STDOUT") &&
                !strcmp(g_params.simpleDumpFile, "stdout")) {
                fileOutput.close();
            }
            if (g_params.fullDumpFile) {
                remove(DUMP_SETTING_FILE);
            }
        }
    } else {
        cout << "Error: Fail to read file header !" << endl;
        ret = -1;
    }

    fclose(tracefp);
    vktrace_free(traceFile);

    return ret;
}
