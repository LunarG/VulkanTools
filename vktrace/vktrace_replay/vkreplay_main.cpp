/**************************************************************************
 *
 * Copyright 2015-2018 Valve Corporation
 * Copyright (C) 2015-2018 LunarG, Inc.
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
 * Author: David Pinedo <david@lunarg.com>
 **************************************************************************/

#include <stdio.h>
#include <string>
#include <algorithm>
#include <inttypes.h>
#if defined(ANDROID)
#include <sstream>
#include <android/log.h>
#include <android_native_app_glue.h>
#endif
#include "vktrace_common.h"
#include "vktrace_tracelog.h"
#include "vktrace_filelike.h"
#include "vktrace_trace_packet_utils.h"
#include "vkreplay_main.h"
#include "vkreplay_factory.h"
#include "vkreplay_seq.h"
#include "vkreplay_vkdisplay.h"
#include "screenshot_parsing.h"
#include "vktrace_vk_packet_id.h"

vkreplayer_settings replaySettings = {NULL, 1, UINT_MAX, UINT_MAX, true, NULL, NULL, NULL, NULL, FALSE};

#if defined(ANDROID)
const char* env_var_screenshot_frames = "debug.vulkan.screenshot";
const char* env_var_screenshot_format = "debug.vulkan.screenshot.format";
#else
const char* env_var_screenshot_frames = "VK_SCREENSHOT_FRAMES";
const char* env_var_screenshot_format = "VK_SCREENSHOT_FORMAT";
#endif

vktrace_SettingInfo g_settings_info[] = {
    {"o",
     "Open",
     VKTRACE_SETTING_STRING,
     {&replaySettings.pTraceFilePath},
     {&replaySettings.pTraceFilePath},
     TRUE,
     "The trace file to open and replay."},
    {"t",
     "TraceFile",
     VKTRACE_SETTING_STRING,
     {&replaySettings.pTraceFilePath},
     {&replaySettings.pTraceFilePath},
     TRUE,
     "The trace file to open and replay. (Deprecated)"},
    {"pltf",
     "PreloadTraceFile",
     VKTRACE_SETTING_BOOL,
     {&replaySettings.preloadTraceFile},
     {&replaySettings.preloadTraceFile},
     TRUE,
     "Preload tracefile to memory before replay. (NumLoops need to be 1.)"},
    {"l",
     "NumLoops",
     VKTRACE_SETTING_UINT,
     {&replaySettings.numLoops},
     {&replaySettings.numLoops},
     TRUE,
     "The number of times to replay the trace file or loop range."},
    {"lsf",
     "LoopStartFrame",
     VKTRACE_SETTING_UINT,
     {&replaySettings.loopStartFrame},
     {&replaySettings.loopStartFrame},
     TRUE,
     "The start frame number of the loop range."},
    {"lef",
     "LoopEndFrame",
     VKTRACE_SETTING_UINT,
     {&replaySettings.loopEndFrame},
     {&replaySettings.loopEndFrame},
     TRUE,
     "The end frame number of the loop range."},
    {"c",
     "CompatibilityMode",
     VKTRACE_SETTING_BOOL,
     {&replaySettings.compatibilityMode},
     {&replaySettings.compatibilityMode},
     TRUE,
     "Use compatibiltiy mode, i.e. convert memory indices to replay device indices, default is TRUE."},
    {"s",
     "Screenshot",
     VKTRACE_SETTING_STRING,
     {&replaySettings.screenshotList},
     {&replaySettings.screenshotList},
     TRUE,
     "Generate screenshots. <string> is one of:\n\
                                         comma separated list of frames\n\
                                         <start>-<count>-<interval>\n\
                                         \"all\""},
    {"sf",
     "ScreenshotFormat",
     VKTRACE_SETTING_STRING,
     {&replaySettings.screenshotColorFormat},
     {&replaySettings.screenshotColorFormat},
     TRUE,
     "Color Space format of screenshot files. Formats are UNORM, SNORM, USCALED, SSCALED, UINT, SINT, SRGB"},
#if defined(PLATFORM_LINUX)
    {"ds",
     "DisplayServer",
     VKTRACE_SETTING_STRING,
     {&replaySettings.displayServer},
     {&replaySettings.displayServer},
     TRUE,
     "Display server used for replay. Options are \"xcb\", \"wayland\"."},
#endif
#if defined(_DEBUG)
    {"v",
     "Verbosity",
     VKTRACE_SETTING_STRING,
     {&replaySettings.verbosity},
     {&replaySettings.verbosity},
     TRUE,
     "Verbosity mode. Modes are \"quiet\", \"errors\", \"warnings\", \"full\", "
     "\"debug\"."},
#else
    {"v",
     "Verbosity",
     VKTRACE_SETTING_STRING,
     {&replaySettings.verbosity},
     {&replaySettings.verbosity},
     TRUE,
     "Verbosity mode. Modes are \"quiet\", \"errors\", \"warnings\", "
     "\"full\"."},
#endif
};

vktrace_SettingGroup g_replaySettingGroup = {"vkreplay", sizeof(g_settings_info) / sizeof(g_settings_info[0]), &g_settings_info[0]};

namespace vktrace_replay {
static void* preloadTraceFile(void* ptr) {
    Sequencer* pSequencer = (Sequencer*)ptr;
    const uint64_t bufferedPacketCount = 8192;  // Preload buffered packet count before replay
    pSequencer->preload_trace_file(bufferedPacketCount);
    return NULL;
}

int main_loop(vktrace_replay::ReplayDisplay display, Sequencer& seq, vktrace_trace_packet_replay_library* replayerArray[]) {
    pthread_t threadPreloadTraceFile;
    if (replaySettings.preloadTraceFile) {
        vktrace_LogAlways("Preloading trace file...");
        pthread_create(&threadPreloadTraceFile, NULL, preloadTraceFile, (void*)&seq);
        while (!seq.ready_to_replay()) {
            usleep(10);
        }
        vktrace_LogAlways("Preloading trace file...Ready to replay.");
    }

    int err = 0;
    vktrace_trace_packet_header* packet;
    unsigned int res;
    vktrace_trace_packet_replay_library* replayer = NULL;
    vktrace_trace_packet_message* msgPacket;
    struct seqBookmark startingPacket;

    bool trace_running = true;
    unsigned int prevFrameNumber = UINT_MAX;

    if (replaySettings.loopEndFrame != UINT_MAX) {
        // Increase by 1 because it is comparing with the frame number which is increased right after vkQueuePresentKHR being
        // called.
        // e.g. when frame number is 3, maybe frame 2 is just finished replaying and frame 3 is not started yet.
        replaySettings.loopEndFrame += 1;
    }

    // record the location of looping start packet
    seq.record_bookmark();
    seq.get_bookmark(startingPacket);
    uint64_t totalLoops = replaySettings.numLoops;
    uint64_t totalLoopFrames = 0;
    uint64_t start_time = vktrace_get_time();
    uint64_t end_time;
    uint64_t start_frame = replaySettings.loopStartFrame == UINT_MAX ? 0 : replaySettings.loopStartFrame;
    uint64_t end_frame = UINT_MAX;
    const char* screenshot_list = replaySettings.screenshotList;
    while (replaySettings.numLoops > 0) {
        if (replaySettings.numLoops > 1 && replaySettings.screenshotList != NULL) {
            // Don't take screenshots until the last loop
            replaySettings.screenshotList = NULL;
        } else if (replaySettings.numLoops == 1 && replaySettings.screenshotList != screenshot_list) {
            // Re-enable screenshots on last loop if they have been disabled
            replaySettings.screenshotList = screenshot_list;
        }

        while (trace_running) {
            display.process_event();
            if (display.get_quit_status()) {
                goto out;
            }
            if (display.get_pause_status()) {
                continue;
            } else {
                packet = seq.get_next_packet();
                if (!packet) break;
            }

            switch (packet->packet_id) {
                case VKTRACE_TPI_MESSAGE:
                    msgPacket = vktrace_interpret_body_as_trace_packet_message(packet);
                    vktrace_LogAlways("Packet %lu: Traced Message (%s): %s", packet->global_packet_index,
                                      vktrace_LogLevelToShortString(msgPacket->type), msgPacket->message);
                    break;
                case VKTRACE_TPI_MARKER_CHECKPOINT:
                    break;
                case VKTRACE_TPI_MARKER_API_BOUNDARY:
                    break;
                case VKTRACE_TPI_MARKER_API_GROUP_BEGIN:
                    break;
                case VKTRACE_TPI_MARKER_API_GROUP_END:
                    break;
                case VKTRACE_TPI_MARKER_TERMINATE_PROCESS:
                    break;
                case VKTRACE_TPI_PORTABILITY_TABLE:
                    break;
                // TODO processing code for all the above cases
                default: {
                    if (packet->tracer_id >= VKTRACE_MAX_TRACER_ID_ARRAY_SIZE || packet->tracer_id == VKTRACE_TID_RESERVED) {
                        vktrace_LogError("Tracer_id from packet num packet %d invalid.", packet->packet_id);
                        continue;
                    }
                    replayer = replayerArray[packet->tracer_id];
                    if (replayer == NULL) {
                        vktrace_LogWarning("Tracer_id %d has no valid replayer.", packet->tracer_id);
                        continue;
                    }
                    if (packet->packet_id >= VKTRACE_TPI_VK_vkApiVersion) {
                        // replay the API packet
                        res = replayer->Replay(replayer->Interpret(packet));
                        if (res != VKTRACE_REPLAY_SUCCESS) {
                            vktrace_LogError("Failed to replay packet_id %d, with global_packet_index %d.", packet->packet_id,
                                             packet->global_packet_index);
                            static BOOL QuitOnAnyError = FALSE;
                            if (QuitOnAnyError) {
                                err = -1;
                                goto out;
                            }
                        }

                        // frame control logic
                        unsigned int frameNumber = replayer->GetFrameNumber();
                        if (prevFrameNumber != frameNumber) {
                            prevFrameNumber = frameNumber;

                            // Only set the loop start location and start_time in the first loop when loopStartFrame is not 0
                            if (frameNumber == start_frame && start_frame > 0 && replaySettings.numLoops == totalLoops) {
                                // record the location of looping start packet
                                seq.record_bookmark();
                                seq.get_bookmark(startingPacket);
                                start_time = vktrace_get_time();
                            }

                            if (frameNumber == replaySettings.loopEndFrame) {
                                trace_running = false;
                            }
                        }

                    } else {
                        vktrace_LogError("Bad packet type id=%d, index=%d.", packet->packet_id, packet->global_packet_index);
                        err = -1;
                        goto out;
                    }
                }
            }
        }
        replaySettings.numLoops--;
        vktrace_LogVerbose("Loop number %d completed. Remaining loops:%d", replaySettings.numLoops + 1, replaySettings.numLoops);

        if (end_frame == UINT_MAX)
            end_frame = replaySettings.loopEndFrame == UINT_MAX
                            ? replayer->GetFrameNumber()
                            : std::min((unsigned int)replayer->GetFrameNumber(), replaySettings.loopEndFrame);
        totalLoopFrames += end_frame - start_frame;

        seq.set_bookmark(startingPacket);
        trace_running = true;
        if (replayer != NULL) {
            replayer->ResetFrameNumber(replaySettings.loopStartFrame);
        }
    }
    end_time = vktrace_get_time();
    if (end_time > start_time) {
        double fps = static_cast<double>(totalLoopFrames) / (end_time - start_time) * 1000000000;
        vktrace_LogAlways("%f fps, %f seconds, %" PRIu64 " frame%s, %" PRIu64 " loop%s, framerange %" PRId64 "-%" PRId64, fps,
                          static_cast<double>(end_time - start_time) / 1000000000, totalLoopFrames, totalLoopFrames > 1 ? "s" : "",
                          totalLoops, totalLoops > 1 ? "s" : "", start_frame, end_frame - 1);
    } else {
        vktrace_LogError("fps error!");
    }

out:
    if (replaySettings.preloadTraceFile) {
        seq.stop_preload();
        pthread_join(threadPreloadTraceFile, NULL);
    }
    seq.clean_up();
    if (replaySettings.screenshotList != NULL) {
        vktrace_free((char*)replaySettings.screenshotList);
        replaySettings.screenshotList = NULL;
    }
    return err;
}
}  // namespace vktrace_replay

using namespace vktrace_replay;

void loggingCallback(VktraceLogLevel level, const char* pMessage) {
    if (level == VKTRACE_LOG_NONE) return;

#if defined(ANDROID)
    switch (level) {
        case VKTRACE_LOG_DEBUG:
            __android_log_print(ANDROID_LOG_DEBUG, "vkreplay", "%s", pMessage);
            break;
        case VKTRACE_LOG_ERROR:
            __android_log_print(ANDROID_LOG_ERROR, "vkreplay", "%s", pMessage);
            break;
        case VKTRACE_LOG_WARNING:
            __android_log_print(ANDROID_LOG_WARN, "vkreplay", "%s", pMessage);
            break;
        case VKTRACE_LOG_VERBOSE:
            __android_log_print(ANDROID_LOG_VERBOSE, "vkreplay", "%s", pMessage);
            break;
        default:
            __android_log_print(ANDROID_LOG_INFO, "vkreplay", "%s", pMessage);
            break;
    }
#else
    switch (level) {
        case VKTRACE_LOG_DEBUG:
            printf("vkreplay debug: %s\n", pMessage);
            break;
        case VKTRACE_LOG_ERROR:
            printf("vkreplay error: %s\n", pMessage);
            break;
        case VKTRACE_LOG_WARNING:
            printf("vkreplay warning: %s\n", pMessage);
            break;
        case VKTRACE_LOG_VERBOSE:
            printf("vkreplay info: %s\n", pMessage);
            break;
        default:
            printf("%s\n", pMessage);
            break;
    }
    fflush(stdout);

#if defined(_DEBUG)
#if defined(WIN32)
    OutputDebugString(pMessage);
#endif
#endif
#endif  // ANDROID
}

static void freePortabilityTablePackets() {
    for (size_t i = 0; i < portabilityTablePackets.size(); i++) {
        vktrace_trace_packet_header* pPacket = (vktrace_trace_packet_header*)portabilityTablePackets[i];
        if (pPacket) {
            vktrace_free(pPacket);
        }
    }
}

std::vector<uint64_t> portabilityTable;
static bool preloadPortabilityTablePackets() {
    uint64_t originalFilePos = vktrace_FileLike_GetCurrentPosition(traceFile);
    uint64_t portabilityTableTotalPacketSize = 0;

    for (size_t i = 0; i < portabilityTable.size(); i++) {
        if (!vktrace_FileLike_SetCurrentPosition(traceFile, portabilityTable[i])) {
            return false;
        }
        vktrace_trace_packet_header* pPacket = vktrace_read_trace_packet(traceFile);
        if (!pPacket) {
            return false;
        }
        pPacket = interpret_trace_packet_vk(pPacket);
        portabilityTablePackets[i] = (uintptr_t)pPacket;
        portabilityTableTotalPacketSize += pPacket->size;
    }

    vktrace_LogVerbose("Total packet size preloaded for portability table: %" PRIu64 " bytes", portabilityTableTotalPacketSize);

    if (!vktrace_FileLike_SetCurrentPosition(traceFile, originalFilePos)) {
        freePortabilityTablePackets();
        return false;
    }
    return true;
}

static bool readPortabilityTable() {
    uint64_t tableSize;
    uint64_t originalFilePos;

    originalFilePos = vktrace_FileLike_GetCurrentPosition(traceFile);
    if (UINT64_MAX == originalFilePos) return false;
    if (!vktrace_FileLike_SetCurrentPosition(traceFile, traceFile->mFileLen - sizeof(uint64_t))) return false;
    if (!vktrace_FileLike_ReadRaw(traceFile, &tableSize, sizeof(uint64_t))) return false;
    if (tableSize != 0) {
        if (!vktrace_FileLike_SetCurrentPosition(traceFile, traceFile->mFileLen - ((tableSize + 1) * sizeof(uint64_t))))
            return false;
        portabilityTable.resize((size_t)tableSize);
        portabilityTablePackets.resize((size_t)tableSize);
        if (!vktrace_FileLike_ReadRaw(traceFile, &portabilityTable[0], sizeof(uint64_t) * tableSize)) return false;
    }
    if (!vktrace_FileLike_SetCurrentPosition(traceFile, originalFilePos)) return false;
    vktrace_LogDebug("portabilityTable size=%" PRIu64 "\n", tableSize);
    return true;
}

int vkreplay_main(int argc, char** argv, vktrace_replay::ReplayDisplayImp* pDisp = nullptr) {
    int err = 0;
    vktrace_SettingGroup* pAllSettings = NULL;
    unsigned int numAllSettings = 0;

    // Default verbosity level
    vktrace_LogSetCallback(loggingCallback);
    vktrace_LogSetLevel(VKTRACE_LOG_ERROR);

    // apply settings from cmd-line args
    if (vktrace_SettingGroup_init_from_cmdline(&g_replaySettingGroup, argc, argv, &replaySettings.pTraceFilePath) != 0) {
        // invalid options specified
        if (pAllSettings != NULL) {
            vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
        }
        return -1;
    }

    if (replaySettings.loopStartFrame > replaySettings.loopEndFrame) {
        vktrace_LogError("Bad loop frame range");
        return -1;
    }

    // merge settings so that new settings will get written into the settings file
    vktrace_SettingGroup_merge(&g_replaySettingGroup, &pAllSettings, &numAllSettings);

    // Force NumLoops option to 1 if pre-load is enabled, because the trace file loaded into memory may be overwritten during replay
    // which will cause error in the second or later loops.
    if (replaySettings.preloadTraceFile && replaySettings.numLoops != 1) {
        vktrace_LogError("PreloadTraceFile is enabled.  Force NumLoops to 1!");
        vktrace_LogError("Please don't enable PreloadTraceFile if you want to replay the trace file multiple times!");
        replaySettings.numLoops = 1;
    }

    // Set verbosity level
    if (replaySettings.verbosity == NULL || !strcmp(replaySettings.verbosity, "errors"))
        replaySettings.verbosity = "errors";
    else if (!strcmp(replaySettings.verbosity, "quiet"))
        vktrace_LogSetLevel(VKTRACE_LOG_NONE);
    else if (!strcmp(replaySettings.verbosity, "warnings"))
        vktrace_LogSetLevel(VKTRACE_LOG_WARNING);
    else if (!strcmp(replaySettings.verbosity, "full"))
        vktrace_LogSetLevel(VKTRACE_LOG_VERBOSE);
#if defined(_DEBUG)
    else if (!strcmp(replaySettings.verbosity, "debug"))
        vktrace_LogSetLevel(VKTRACE_LOG_DEBUG);
#endif
    else {
        vktrace_SettingGroup_print(&g_replaySettingGroup);
        // invalid options specified
        if (pAllSettings != NULL) {
            vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
        }
        return -1;
    }

    // Set up environment for screenshot
    if (replaySettings.screenshotList != NULL) {
        if (!screenshot::checkParsingFrameRange(replaySettings.screenshotList)) {
            vktrace_LogError("Screenshot range error");
            vktrace_SettingGroup_print(&g_replaySettingGroup);
            if (pAllSettings != NULL) {
                vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
            }
            return -1;
        } else {
            // Set env var that communicates list to ScreenShot layer
            vktrace_set_global_var(env_var_screenshot_frames, replaySettings.screenshotList);
        }

        // Set up environment for screenshot color space format
        if (replaySettings.screenshotColorFormat != NULL && replaySettings.screenshotList != NULL) {
            vktrace_set_global_var(env_var_screenshot_format, replaySettings.screenshotColorFormat);
        } else if (replaySettings.screenshotColorFormat != NULL && replaySettings.screenshotList == NULL) {
            vktrace_LogWarning("Screenshot format should be used when screenshot enabled!");
            vktrace_set_global_var(env_var_screenshot_format, "");
        } else {
            vktrace_set_global_var(env_var_screenshot_format, "");
        }
    }

    // open the trace file
    char* pTraceFile = replaySettings.pTraceFilePath;
    vktrace_trace_file_header fileHeader;
    vktrace_trace_file_header* pFileHeader;  // File header, including gpuinfo structs

    FILE* tracefp;

    if (pTraceFile != NULL && strlen(pTraceFile) > 0) {
        tracefp = fopen(pTraceFile, "rb");
        if (tracefp == NULL) {
            vktrace_LogError("Cannot open trace file: '%s'.", pTraceFile);
            // invalid options specified
            if (pAllSettings != NULL) {
                vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
            }
            vktrace_free(pTraceFile);
            return -1;
        }
    } else {
        vktrace_LogError("No trace file specified.");
        vktrace_SettingGroup_print(&g_replaySettingGroup);
        if (pAllSettings != NULL) {
            vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
        }
        return -1;
    }

    // read the header
    traceFile = vktrace_FileLike_create_file(tracefp);
    if (vktrace_FileLike_ReadRaw(traceFile, &fileHeader, sizeof(fileHeader)) == false) {
        vktrace_LogError("Unable to read header from file.");
        if (pAllSettings != NULL) {
            vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
        }
        fclose(tracefp);
        vktrace_free(pTraceFile);
        vktrace_free(traceFile);
        return -1;
    }

    // set global version num
    vktrace_set_trace_version(fileHeader.trace_file_version);

    // Make sure trace file version is supported.
    // We can't play trace files with a version prior to the minimum compatible version.
    // We also won't attempt to play trace files that are newer than this replayer.
    if (fileHeader.trace_file_version < VKTRACE_TRACE_FILE_VERSION_MINIMUM_COMPATIBLE ||
        fileHeader.trace_file_version > VKTRACE_TRACE_FILE_VERSION) {
        vktrace_LogError(
            "Trace file version %u is not compatible with this replayer version (%u).\nYou'll need to make a new trace file, or "
            "use "
            "the appropriate replayer.",
            fileHeader.trace_file_version, VKTRACE_TRACE_FILE_VERSION_MINIMUM_COMPATIBLE);
        fclose(tracefp);
        vktrace_free(pTraceFile);
        vktrace_free(traceFile);
        return -1;
    }

    // Make sure magic number in trace file is valid and we have at least one gpuinfo struct
    if (fileHeader.magic != VKTRACE_FILE_MAGIC || fileHeader.n_gpuinfo < 1) {
        vktrace_LogError("%s does not appear to be a valid Vulkan trace file.", pTraceFile);
        fclose(tracefp);
        vktrace_free(pTraceFile);
        vktrace_free(traceFile);
        return -1;
    }

    // Allocate a new header that includes space for all gpuinfo structs
    if (!(pFileHeader = (vktrace_trace_file_header*)vktrace_malloc(sizeof(vktrace_trace_file_header) +
                                                                   (size_t)(fileHeader.n_gpuinfo * sizeof(struct_gpuinfo))))) {
        vktrace_LogError("Can't allocate space for trace file header.");
        if (pAllSettings != NULL) {
            vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
        }
        fclose(tracefp);
        vktrace_free(pTraceFile);
        vktrace_free(traceFile);
        return -1;
    }

    // Copy the file header, and append the gpuinfo array
    *pFileHeader = fileHeader;
    if (vktrace_FileLike_ReadRaw(traceFile, pFileHeader + 1, pFileHeader->n_gpuinfo * sizeof(struct_gpuinfo)) == false) {
        vktrace_LogError("Unable to read header from file.");
        if (pAllSettings != NULL) {
            vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
        }
        fclose(tracefp);
        vktrace_free(pTraceFile);
        vktrace_free(traceFile);
        vktrace_free(pFileHeader);
        return -1;
    }

    // read portability table if it exists
    if (pFileHeader->portability_table_valid) pFileHeader->portability_table_valid = readPortabilityTable();
    if (pFileHeader->portability_table_valid) pFileHeader->portability_table_valid = preloadPortabilityTablePackets();
    if (!pFileHeader->portability_table_valid)
        vktrace_LogAlways("Trace file does not appear to contain portability table. Will not attempt to map memoryType indices.");

    // load any API specific driver libraries and init replayer objects
    uint8_t tidApi = VKTRACE_TID_RESERVED;
    vktrace_trace_packet_replay_library* replayer[VKTRACE_MAX_TRACER_ID_ARRAY_SIZE];
    ReplayFactory makeReplayer;

#if defined(PLATFORM_LINUX) && !defined(ANDROID)
    // Choose default display server if unset
    if (replaySettings.displayServer == NULL) {
        auto session = getenv("XDG_SESSION_TYPE");
        if (session == NULL || strcmp(session, "x11") == 0) {
            replaySettings.displayServer = "xcb";
        } else if (strcmp(session, "wayland") == 0) {
            replaySettings.displayServer = "wayland";
        }
    }
#endif

    // Create window. Initial size is 100x100. It will later get resized to the size
    // used by the traced app. The resize will happen  during playback of swapchain functions.
    vktrace_replay::ReplayDisplay disp(100, 100);

    // Create display
    if (GetDisplayImplementation(replaySettings.displayServer, &pDisp) == -1) {
        if (pAllSettings != NULL) {
            vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
        }
        fclose(tracefp);
        vktrace_free(pTraceFile);
        vktrace_free(traceFile);
        if (pFileHeader->portability_table_valid) freePortabilityTablePackets();
        vktrace_free(pFileHeader);
        return -1;
    }

    disp.set_implementation(pDisp);
//**********************************************************
#if defined(_DEBUG)
    static BOOL debugStartup = FALSE;  // TRUE
    while (debugStartup)
        ;
#endif
    //***********************************************************

    for (int i = 0; i < VKTRACE_MAX_TRACER_ID_ARRAY_SIZE; i++) {
        replayer[i] = NULL;
    }

    for (uint64_t i = 0; i < pFileHeader->tracer_count; i++) {
        uint8_t tracerId = pFileHeader->tracer_id_array[i].id;
        tidApi = tracerId;

        const VKTRACE_TRACER_REPLAYER_INFO* pReplayerInfo = &(gs_tracerReplayerInfo[tracerId]);

        if (pReplayerInfo->tracerId != tracerId) {
            vktrace_LogError("Replayer info for TracerId (%d) failed consistency check.", tracerId);
            assert(!"TracerId in VKTRACE_TRACER_REPLAYER_INFO does not match the requested tracerId. The array needs to be corrected.");
        } else if (pReplayerInfo->needsReplayer == TRUE) {
            // Have our factory create the necessary replayer
            replayer[tracerId] = makeReplayer.Create(tracerId);

            if (replayer[tracerId] == NULL) {
                // replayer failed to be created
                if (pAllSettings != NULL) {
                    vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
                }
                fclose(tracefp);
                vktrace_free(pTraceFile);
                vktrace_free(traceFile);
                if (pFileHeader->portability_table_valid) freePortabilityTablePackets();
                vktrace_free(pFileHeader);
                return -1;
            }

            // merge the replayer's settings into the list of all settings so that we can output a comprehensive settings file later
            // on.
            vktrace_SettingGroup_merge(replayer[tracerId]->GetSettings(), &pAllSettings, &numAllSettings);

            // update the replayer with the loaded settings
            replayer[tracerId]->UpdateFromSettings(pAllSettings, numAllSettings);

            // Initialize the replayer
            err = replayer[tracerId]->Initialize(&disp, &replaySettings, pFileHeader);
            if (err) {
                vktrace_LogError("Couldn't Initialize replayer for TracerId %d.", tracerId);
                if (pAllSettings != NULL) {
                    vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
                }
                fclose(tracefp);
                vktrace_free(pTraceFile);
                vktrace_free(traceFile);
                if (pFileHeader->portability_table_valid) freePortabilityTablePackets();
                vktrace_free(pFileHeader);
                return err;
            }
        }
    }

    if (tidApi == VKTRACE_TID_RESERVED) {
        vktrace_LogError("No API specified in tracefile for replaying.");
        if (pAllSettings != NULL) {
            vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
        }
        fclose(tracefp);
        vktrace_free(pTraceFile);
        vktrace_free(traceFile);
        if (pFileHeader->portability_table_valid) freePortabilityTablePackets();
        vktrace_free(pFileHeader);
        return -1;
    }

    // main loop
    Sequencer sequencer(traceFile);
    err = vktrace_replay::main_loop(disp, sequencer, replayer);

    for (int i = 0; i < VKTRACE_MAX_TRACER_ID_ARRAY_SIZE; i++) {
        if (replayer[i] != NULL) {
            replayer[i]->Deinitialize();
            makeReplayer.Destroy(&replayer[i]);
        }
    }

    if (pAllSettings != NULL) {
        vktrace_SettingGroup_Delete_Loaded(&pAllSettings, &numAllSettings);
    }

    fclose(tracefp);
    vktrace_free(pTraceFile);
    vktrace_free(traceFile);
    if (pFileHeader->portability_table_valid) freePortabilityTablePackets();
    vktrace_free(pFileHeader);

    return err;
}

#if defined(ANDROID)
static bool initialized = false;
static bool active = false;

// Convert Intents to argv
// Ported from Hologram sample, only difference is flexible key
std::vector<std::string> get_args(android_app& app, const char* intent_extra_data_key) {
    std::vector<std::string> args;
    JavaVM& vm = *app.activity->vm;
    JNIEnv* p_env;
    if (vm.AttachCurrentThread(&p_env, nullptr) != JNI_OK) return args;

    JNIEnv& env = *p_env;
    jobject activity = app.activity->clazz;
    jmethodID get_intent_method = env.GetMethodID(env.GetObjectClass(activity), "getIntent", "()Landroid/content/Intent;");
    jobject intent = env.CallObjectMethod(activity, get_intent_method);
    jmethodID get_string_extra_method =
        env.GetMethodID(env.GetObjectClass(intent), "getStringExtra", "(Ljava/lang/String;)Ljava/lang/String;");
    jvalue get_string_extra_args;
    get_string_extra_args.l = env.NewStringUTF(intent_extra_data_key);
    jstring extra_str = static_cast<jstring>(env.CallObjectMethodA(intent, get_string_extra_method, &get_string_extra_args));

    std::string args_str;
    if (extra_str) {
        const char* extra_utf = env.GetStringUTFChars(extra_str, nullptr);
        args_str = extra_utf;
        env.ReleaseStringUTFChars(extra_str, extra_utf);
        env.DeleteLocalRef(extra_str);
    }

    env.DeleteLocalRef(get_string_extra_args.l);
    env.DeleteLocalRef(intent);
    vm.DetachCurrentThread();

    // split args_str
    std::stringstream ss(args_str);
    std::string arg;
    while (std::getline(ss, arg, ' ')) {
        if (!arg.empty()) args.push_back(arg);
    }

    return args;
}

static int32_t processInput(struct android_app* app, AInputEvent* event) {
    if ((app->userData != nullptr) && (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)) {
        vkDisplayAndroid* display = reinterpret_cast<vkDisplayAndroid*>(app->userData);

        // TODO: Distinguish between tap and swipe actions; swipe to advance to next frame when paused.
        int32_t action = AMotionEvent_getAction(event);
        if (action == AMOTION_EVENT_ACTION_UP) {
            display->set_pause_status(!display->get_pause_status());
            return 1;
        }
    }

    return 0;
}

static void processCommand(struct android_app* app, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {
            if (app->window) {
                initialized = true;
            }
            break;
        }
        case APP_CMD_GAINED_FOCUS: {
            active = true;
            break;
        }
        case APP_CMD_LOST_FOCUS: {
            active = false;
            break;
        }
    }
}

// Start with carbon copy of main() and convert it to support Android, then diff them and move common code to helpers.
void android_main(struct android_app* app) {
    const char* appTag = "vkreplay";

    // This will be set by the vkDisplay object.
    app->userData = nullptr;

    int vulkanSupport = InitVulkan();
    if (vulkanSupport == 0) {
        __android_log_print(ANDROID_LOG_ERROR, appTag, "No Vulkan support found");
        return;
    }

    app->onAppCmd = processCommand;
    app->onInputEvent = processInput;

    while (1) {
        int events;
        struct android_poll_source* source;
        while (ALooper_pollAll(active ? 0 : -1, NULL, &events, (void**)&source) >= 0) {
            if (source) {
                source->process(app, source);
            }

            if (app->destroyRequested != 0) {
                // anything to clean up?
                return;
            }
        }

        if (initialized && active) {
            // Parse Intents into argc, argv
            // Use the following key to send arguments to gtest, i.e.
            // --es args "-v\ debug\ -t\ /sdcard/cube0.vktrace"
            const char key[] = "args";
            std::vector<std::string> args = get_args(*app, key);

            int argc = args.size() + 1;

            char** argv = (char**)malloc(argc * sizeof(char*));
            argv[0] = (char*)"vkreplay";
            for (int i = 0; i < args.size(); i++) argv[i + 1] = (char*)args[i].c_str();

            __android_log_print(ANDROID_LOG_INFO, appTag, "argc = %i", argc);
            for (int i = 0; i < argc; i++) __android_log_print(ANDROID_LOG_INFO, appTag, "argv[%i] = %s", i, argv[i]);

            // sleep to allow attaching debugger
            // sleep(10);

            auto pDisp = new vkDisplayAndroid(app);

            // Call into common code
            int err = vkreplay_main(argc, argv, pDisp);
            __android_log_print(ANDROID_LOG_DEBUG, appTag, "vkreplay_main returned %i", err);

            ANativeActivity_finish(app->activity);
            free(argv);

            // Kill the process
            // This is not a necessarily good practice.  But it works to make sure the process is killed after replaying a trace
            // file.  So user will not need to run "adb shell am force-stop come.example.vkreplay" afterwards.
            exit(err);
        }
    }
}

#else  // ANDROID

extern "C" int main(int argc, char** argv) { return vkreplay_main(argc, argv); }

#endif
