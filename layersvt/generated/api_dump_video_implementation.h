
/* Copyright (c) 2015-2025 Valve Corporation
 * Copyright (c) 2015-2025 LunarG, Inc.
 * Copyright (c) 2015-2017, 2019, 2021 Google Inc.
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
 */

/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */

#pragma once

#include "api_dump.h"

//=========================== Enum Implementations ==========================//

template <ApiDumpFormat Format>
void dump_StdVideoH264ChromaFormatIdc(const StdVideoH264ChromaFormatIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_CHROMA_FORMAT_IDC_MONOCHROME:
            dump_enum<Format>(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_MONOCHROME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_CHROMA_FORMAT_IDC_420:
            dump_enum<Format>(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_420", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_CHROMA_FORMAT_IDC_422:
            dump_enum<Format>(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_422", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_CHROMA_FORMAT_IDC_444:
            dump_enum<Format>(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_444", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_CHROMA_FORMAT_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264ProfileIdc(const StdVideoH264ProfileIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_PROFILE_IDC_BASELINE:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PROFILE_IDC_BASELINE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_PROFILE_IDC_MAIN:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PROFILE_IDC_MAIN", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_PROFILE_IDC_HIGH:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PROFILE_IDC_HIGH", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_PROFILE_IDC_HIGH_444_PREDICTIVE:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PROFILE_IDC_HIGH_444_PREDICTIVE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_PROFILE_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PROFILE_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264LevelIdc(const StdVideoH264LevelIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_LEVEL_IDC_1_0:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_1_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_1_1:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_1_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_1_2:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_1_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_1_3:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_1_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_2_0:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_2_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_2_1:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_2_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_2_2:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_2_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_3_0:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_3_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_3_1:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_3_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_3_2:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_3_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_4_0:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_4_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_4_1:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_4_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_4_2:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_4_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_5_0:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_5_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_5_1:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_5_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_5_2:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_5_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_6_0:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_6_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_6_1:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_6_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_6_2:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_6_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_LEVEL_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_LEVEL_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264PocType(const StdVideoH264PocType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_POC_TYPE_0:
            dump_enum<Format>(settings, "STD_VIDEO_H264_POC_TYPE_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_POC_TYPE_1:
            dump_enum<Format>(settings, "STD_VIDEO_H264_POC_TYPE_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_POC_TYPE_2:
            dump_enum<Format>(settings, "STD_VIDEO_H264_POC_TYPE_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_POC_TYPE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_POC_TYPE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264AspectRatioIdc(const StdVideoH264AspectRatioIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_UNSPECIFIED:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_UNSPECIFIED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_SQUARE:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_SQUARE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_12_11:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_12_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_10_11:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_10_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_16_11:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_16_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_40_33:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_40_33", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_24_11:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_24_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_20_11:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_20_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_32_11:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_32_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_80_33:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_80_33", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_18_11:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_18_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_15_11:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_15_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_64_33:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_64_33", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_160_99:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_160_99", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_4_3:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_4_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_3_2:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_3_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_2_1:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_2_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_EXTENDED_SAR:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_EXTENDED_SAR", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_ASPECT_RATIO_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264WeightedBipredIdc(const StdVideoH264WeightedBipredIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_DEFAULT:
            dump_enum<Format>(settings, "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_DEFAULT", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_EXPLICIT:
            dump_enum<Format>(settings, "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_EXPLICIT", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_IMPLICIT:
            dump_enum<Format>(settings, "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_IMPLICIT", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264ModificationOfPicNumsIdc(const StdVideoH264ModificationOfPicNumsIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_SUBTRACT:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_SUBTRACT", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_ADD:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_ADD", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_LONG_TERM:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_LONG_TERM", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_END:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_END", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264MemMgmtControlOp(const StdVideoH264MemMgmtControlOp object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_END:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_END", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_SHORT_TERM:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_SHORT_TERM", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_LONG_TERM:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_LONG_TERM", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_LONG_TERM:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_LONG_TERM", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_SET_MAX_LONG_TERM_INDEX:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_SET_MAX_LONG_TERM_INDEX", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_ALL:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_ALL", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_CURRENT_AS_LONG_TERM:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_CURRENT_AS_LONG_TERM", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264CabacInitIdc(const StdVideoH264CabacInitIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_CABAC_INIT_IDC_0:
            dump_enum<Format>(settings, "STD_VIDEO_H264_CABAC_INIT_IDC_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_CABAC_INIT_IDC_1:
            dump_enum<Format>(settings, "STD_VIDEO_H264_CABAC_INIT_IDC_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_CABAC_INIT_IDC_2:
            dump_enum<Format>(settings, "STD_VIDEO_H264_CABAC_INIT_IDC_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_CABAC_INIT_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_CABAC_INIT_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264DisableDeblockingFilterIdc(const StdVideoH264DisableDeblockingFilterIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_DISABLED:
            dump_enum<Format>(settings, "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_DISABLED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_ENABLED:
            dump_enum<Format>(settings, "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_ENABLED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_PARTIAL:
            dump_enum<Format>(settings, "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_PARTIAL", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264SliceType(const StdVideoH264SliceType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_SLICE_TYPE_P:
            dump_enum<Format>(settings, "STD_VIDEO_H264_SLICE_TYPE_P", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_SLICE_TYPE_B:
            dump_enum<Format>(settings, "STD_VIDEO_H264_SLICE_TYPE_B", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_SLICE_TYPE_I:
            dump_enum<Format>(settings, "STD_VIDEO_H264_SLICE_TYPE_I", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_SLICE_TYPE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_SLICE_TYPE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264PictureType(const StdVideoH264PictureType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_PICTURE_TYPE_P:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PICTURE_TYPE_P", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_PICTURE_TYPE_B:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PICTURE_TYPE_B", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_PICTURE_TYPE_I:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PICTURE_TYPE_I", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_PICTURE_TYPE_IDR:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PICTURE_TYPE_IDR", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_PICTURE_TYPE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_PICTURE_TYPE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264NonVclNaluType(const StdVideoH264NonVclNaluType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H264_NON_VCL_NALU_TYPE_SPS:
            dump_enum<Format>(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_SPS", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_NON_VCL_NALU_TYPE_PPS:
            dump_enum<Format>(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_PPS", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_NON_VCL_NALU_TYPE_AUD:
            dump_enum<Format>(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_AUD", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_NON_VCL_NALU_TYPE_PREFIX:
            dump_enum<Format>(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_PREFIX", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_SEQUENCE:
            dump_enum<Format>(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_SEQUENCE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_STREAM:
            dump_enum<Format>(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_STREAM", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_NON_VCL_NALU_TYPE_PRECODED:
            dump_enum<Format>(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_PRECODED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H264_NON_VCL_NALU_TYPE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeH264FieldOrderCount(const StdVideoDecodeH264FieldOrderCount object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_TOP:
            dump_enum<Format>(settings, "STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_TOP", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_BOTTOM:
            dump_enum<Format>(settings, "STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_BOTTOM", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265ChromaFormatIdc(const StdVideoH265ChromaFormatIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H265_CHROMA_FORMAT_IDC_MONOCHROME:
            dump_enum<Format>(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_MONOCHROME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_CHROMA_FORMAT_IDC_420:
            dump_enum<Format>(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_420", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_CHROMA_FORMAT_IDC_422:
            dump_enum<Format>(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_422", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_CHROMA_FORMAT_IDC_444:
            dump_enum<Format>(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_444", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_CHROMA_FORMAT_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265ProfileIdc(const StdVideoH265ProfileIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H265_PROFILE_IDC_MAIN:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PROFILE_IDC_MAIN", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_PROFILE_IDC_MAIN_10:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PROFILE_IDC_MAIN_10", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_PROFILE_IDC_MAIN_STILL_PICTURE:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PROFILE_IDC_MAIN_STILL_PICTURE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_PROFILE_IDC_FORMAT_RANGE_EXTENSIONS:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PROFILE_IDC_FORMAT_RANGE_EXTENSIONS", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_PROFILE_IDC_SCC_EXTENSIONS:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PROFILE_IDC_SCC_EXTENSIONS", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_PROFILE_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PROFILE_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265LevelIdc(const StdVideoH265LevelIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H265_LEVEL_IDC_1_0:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_1_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_2_0:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_2_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_2_1:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_2_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_3_0:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_3_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_3_1:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_3_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_4_0:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_4_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_4_1:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_4_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_5_0:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_5_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_5_1:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_5_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_5_2:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_5_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_6_0:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_6_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_6_1:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_6_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_6_2:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_6_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_LEVEL_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H265_LEVEL_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265SliceType(const StdVideoH265SliceType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H265_SLICE_TYPE_B:
            dump_enum<Format>(settings, "STD_VIDEO_H265_SLICE_TYPE_B", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_SLICE_TYPE_P:
            dump_enum<Format>(settings, "STD_VIDEO_H265_SLICE_TYPE_P", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_SLICE_TYPE_I:
            dump_enum<Format>(settings, "STD_VIDEO_H265_SLICE_TYPE_I", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_SLICE_TYPE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H265_SLICE_TYPE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265PictureType(const StdVideoH265PictureType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H265_PICTURE_TYPE_P:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PICTURE_TYPE_P", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_PICTURE_TYPE_B:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PICTURE_TYPE_B", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_PICTURE_TYPE_I:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PICTURE_TYPE_I", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_PICTURE_TYPE_IDR:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PICTURE_TYPE_IDR", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_PICTURE_TYPE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H265_PICTURE_TYPE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265AspectRatioIdc(const StdVideoH265AspectRatioIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_UNSPECIFIED:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_UNSPECIFIED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_SQUARE:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_SQUARE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_12_11:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_12_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_10_11:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_10_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_16_11:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_16_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_40_33:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_40_33", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_24_11:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_24_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_20_11:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_20_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_32_11:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_32_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_80_33:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_80_33", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_18_11:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_18_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_15_11:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_15_11", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_64_33:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_64_33", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_160_99:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_160_99", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_4_3:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_4_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_3_2:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_3_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_2_1:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_2_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_EXTENDED_SAR:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_EXTENDED_SAR", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_H265_ASPECT_RATIO_IDC_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1Profile(const StdVideoAV1Profile object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_PROFILE_MAIN:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_PROFILE_MAIN", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_PROFILE_HIGH:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_PROFILE_HIGH", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_PROFILE_PROFESSIONAL:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_PROFILE_PROFESSIONAL", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_PROFILE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_PROFILE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1Level(const StdVideoAV1Level object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_LEVEL_2_0:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_2_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_2_1:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_2_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_2_2:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_2_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_2_3:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_2_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_3_0:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_3_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_3_1:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_3_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_3_2:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_3_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_3_3:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_3_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_4_0:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_4_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_4_1:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_4_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_4_2:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_4_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_4_3:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_4_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_5_0:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_5_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_5_1:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_5_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_5_2:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_5_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_5_3:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_5_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_6_0:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_6_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_6_1:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_6_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_6_2:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_6_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_6_3:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_6_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_7_0:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_7_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_7_1:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_7_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_7_2:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_7_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_7_3:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_7_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_LEVEL_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_LEVEL_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1FrameType(const StdVideoAV1FrameType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_FRAME_TYPE_KEY:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_TYPE_KEY", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_FRAME_TYPE_INTER:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_TYPE_INTER", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_FRAME_TYPE_INTRA_ONLY:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_TYPE_INTRA_ONLY", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_FRAME_TYPE_SWITCH:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_TYPE_SWITCH", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_FRAME_TYPE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_TYPE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1ReferenceName(const StdVideoAV1ReferenceName object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_REFERENCE_NAME_INTRA_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_REFERENCE_NAME_INTRA_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_REFERENCE_NAME_LAST_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_REFERENCE_NAME_LAST_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_REFERENCE_NAME_LAST2_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_REFERENCE_NAME_LAST2_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_REFERENCE_NAME_LAST3_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_REFERENCE_NAME_LAST3_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_REFERENCE_NAME_GOLDEN_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_REFERENCE_NAME_GOLDEN_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_REFERENCE_NAME_BWDREF_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_REFERENCE_NAME_BWDREF_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_REFERENCE_NAME_ALTREF2_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_REFERENCE_NAME_ALTREF2_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_REFERENCE_NAME_ALTREF_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_REFERENCE_NAME_ALTREF_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_REFERENCE_NAME_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_REFERENCE_NAME_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1InterpolationFilter(const StdVideoAV1InterpolationFilter object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SHARP:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SHARP", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_INTERPOLATION_FILTER_BILINEAR:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_BILINEAR", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_INTERPOLATION_FILTER_SWITCHABLE:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_SWITCHABLE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_INTERPOLATION_FILTER_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1TxMode(const StdVideoAV1TxMode object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_TX_MODE_ONLY_4X4:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TX_MODE_ONLY_4X4", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TX_MODE_LARGEST:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TX_MODE_LARGEST", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TX_MODE_SELECT:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TX_MODE_SELECT", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TX_MODE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TX_MODE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1FrameRestorationType(const StdVideoAV1FrameRestorationType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_NONE:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_NONE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_WIENER:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_WIENER", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SGRPROJ:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SGRPROJ", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SWITCHABLE:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SWITCHABLE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1ColorPrimaries(const StdVideoAV1ColorPrimaries object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_COLOR_PRIMARIES_BT_709:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_709", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_UNSPECIFIED:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_UNSPECIFIED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_M:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_M", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_B_G:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_B_G", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_BT_601:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_601", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_240:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_240", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_GENERIC_FILM:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_GENERIC_FILM", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_BT_2020:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_2020", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_XYZ:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_XYZ", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_431:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_431", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_432:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_432", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_EBU_3213:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_EBU_3213", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_COLOR_PRIMARIES_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1TransferCharacteristics(const StdVideoAV1TransferCharacteristics object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_0:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_709:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_709", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_UNSPECIFIED:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_UNSPECIFIED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_3:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_M:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_M", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_B_G:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_B_G", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_601:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_601", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_240:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_240", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LINEAR:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LINEAR", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100_SQRT10:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100_SQRT10", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_IEC_61966:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_IEC_61966", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_1361:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_1361", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SRGB:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SRGB", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_10_BIT:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_10_BIT", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_12_BIT:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_12_BIT", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_2084:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_2084", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_428:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_428", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_HLG:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_HLG", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1MatrixCoefficients(const StdVideoAV1MatrixCoefficients object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_IDENTITY:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_IDENTITY", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_709:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_709", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_UNSPECIFIED:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_UNSPECIFIED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_RESERVED_3:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_RESERVED_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_FCC:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_FCC", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_470_B_G:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_470_B_G", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_601:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_601", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_240:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_240", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_YCGCO:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_YCGCO", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_NCL:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_NCL", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_CL:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_CL", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_2085:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_2085", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_NCL:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_NCL", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_CL:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_CL", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_ICTCP:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_ICTCP", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_MATRIX_COEFFICIENTS_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1ChromaSamplePosition(const StdVideoAV1ChromaSamplePosition object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_UNKNOWN:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_UNKNOWN", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_VERTICAL:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_VERTICAL", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_COLOCATED:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_COLOCATED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_RESERVED:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_RESERVED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9Profile(const StdVideoVP9Profile object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_VP9_PROFILE_0:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_PROFILE_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_PROFILE_1:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_PROFILE_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_PROFILE_2:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_PROFILE_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_PROFILE_3:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_PROFILE_3", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_PROFILE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_PROFILE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9Level(const StdVideoVP9Level object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_VP9_LEVEL_1_0:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_1_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_1_1:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_1_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_2_0:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_2_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_2_1:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_2_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_3_0:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_3_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_3_1:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_3_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_4_0:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_4_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_4_1:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_4_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_5_0:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_5_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_5_1:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_5_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_5_2:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_5_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_6_0:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_6_0", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_6_1:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_6_1", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_6_2:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_6_2", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_LEVEL_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_LEVEL_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9FrameType(const StdVideoVP9FrameType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_VP9_FRAME_TYPE_KEY:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_FRAME_TYPE_KEY", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_FRAME_TYPE_NON_KEY:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_FRAME_TYPE_NON_KEY", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_FRAME_TYPE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_FRAME_TYPE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9ReferenceName(const StdVideoVP9ReferenceName object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_VP9_REFERENCE_NAME_INTRA_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_REFERENCE_NAME_INTRA_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_REFERENCE_NAME_LAST_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_REFERENCE_NAME_LAST_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_REFERENCE_NAME_GOLDEN_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_REFERENCE_NAME_GOLDEN_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_REFERENCE_NAME_ALTREF_FRAME:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_REFERENCE_NAME_ALTREF_FRAME", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_REFERENCE_NAME_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_REFERENCE_NAME_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9InterpolationFilter(const StdVideoVP9InterpolationFilter object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SHARP:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SHARP", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_INTERPOLATION_FILTER_BILINEAR:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_BILINEAR", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_INTERPOLATION_FILTER_SWITCHABLE:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_SWITCHABLE", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_INTERPOLATION_FILTER_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9ColorSpace(const StdVideoVP9ColorSpace object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case STD_VIDEO_VP9_COLOR_SPACE_UNKNOWN:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_COLOR_SPACE_UNKNOWN", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_COLOR_SPACE_BT_601:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_COLOR_SPACE_BT_601", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_COLOR_SPACE_BT_709:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_COLOR_SPACE_BT_709", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_COLOR_SPACE_SMPTE_170:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_COLOR_SPACE_SMPTE_170", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_COLOR_SPACE_SMPTE_240:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_COLOR_SPACE_SMPTE_240", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_COLOR_SPACE_BT_2020:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_COLOR_SPACE_BT_2020", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_COLOR_SPACE_RESERVED:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_COLOR_SPACE_RESERVED", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_COLOR_SPACE_RGB:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_COLOR_SPACE_RGB", static_cast<uint32_t>(object));
            break;
        case STD_VIDEO_VP9_COLOR_SPACE_INVALID:
            dump_enum<Format>(settings, "STD_VIDEO_VP9_COLOR_SPACE_INVALID", static_cast<uint32_t>(object));
            break;
        default:
            dump_enum_with_value<Format>(settings, "UNKNOWN", static_cast<uint32_t>(object));
    }
    dump_end<Format>(settings, OutputConstruct::value, indents);
}

//========================= Bitmask Implementations =========================//

//=========================== Flag Implementations ==========================//

//======================= Func Pointer Implementations ======================//

//====================== Struct and Union Implementations =====================//

template <ApiDumpFormat Format>
void dump_StdVideoH264SpsVuiFlags(const StdVideoH264SpsVuiFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.aspect_ratio_info_present_flag, settings, "uint32_t: 1", "aspect_ratio_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.overscan_info_present_flag, settings, "uint32_t: 1", "overscan_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.overscan_appropriate_flag, settings, "uint32_t: 1", "overscan_appropriate_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.video_signal_type_present_flag, settings, "uint32_t: 1", "video_signal_type_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.video_full_range_flag, settings, "uint32_t: 1", "video_full_range_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.color_description_present_flag, settings, "uint32_t: 1", "color_description_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_loc_info_present_flag, settings, "uint32_t: 1", "chroma_loc_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.timing_info_present_flag, settings, "uint32_t: 1", "timing_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.fixed_frame_rate_flag, settings, "uint32_t: 1", "fixed_frame_rate_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bitstream_restriction_flag, settings, "uint32_t: 1", "bitstream_restriction_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.nal_hrd_parameters_present_flag, settings, "uint32_t: 1", "nal_hrd_parameters_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vcl_hrd_parameters_present_flag, settings, "uint32_t: 1", "vcl_hrd_parameters_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264HrdParameters(const StdVideoH264HrdParameters& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.cpb_cnt_minus1, settings, "uint8_t", "cpb_cnt_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bit_rate_scale, settings, "uint8_t", "bit_rate_scale", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cpb_size_scale, settings, "uint8_t", "cpb_size_scale", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.bit_rate_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "bit_rate_value_minus1", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cpb_size_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "cpb_size_value_minus1", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cbr_flag, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "cbr_flag", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.initial_cpb_removal_delay_length_minus1, settings, "uint32_t", "initial_cpb_removal_delay_length_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cpb_removal_delay_length_minus1, settings, "uint32_t", "cpb_removal_delay_length_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.dpb_output_delay_length_minus1, settings, "uint32_t", "dpb_output_delay_length_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.time_offset_length, settings, "uint32_t", "time_offset_length", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264SequenceParameterSetVui(const StdVideoH264SequenceParameterSetVui& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH264SpsVuiFlags<Format>(object.flags, settings, "StdVideoH264SpsVuiFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264AspectRatioIdc<Format>(object.aspect_ratio_idc, settings, "StdVideoH264AspectRatioIdc", "aspect_ratio_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.sar_width, settings, "uint16_t", "sar_width", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.sar_height, settings, "uint16_t", "sar_height", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.video_format, settings, "uint8_t", "video_format", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.colour_primaries, settings, "uint8_t", "colour_primaries", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.transfer_characteristics, settings, "uint8_t", "transfer_characteristics", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.matrix_coefficients, settings, "uint8_t", "matrix_coefficients", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_units_in_tick, settings, "uint32_t", "num_units_in_tick", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.time_scale, settings, "uint32_t", "time_scale", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.max_num_reorder_frames, settings, "uint8_t", "max_num_reorder_frames", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.max_dec_frame_buffering, settings, "uint8_t", "max_dec_frame_buffering", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_sample_loc_type_top_field, settings, "uint8_t", "chroma_sample_loc_type_top_field", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_sample_loc_type_bottom_field, settings, "uint8_t", "chroma_sample_loc_type_bottom_field", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint32_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pHrdParameters, settings, "const StdVideoH264HrdParameters*", "pHrdParameters", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH264HrdParameters<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264SpsFlags(const StdVideoH264SpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.constraint_set0_flag, settings, "uint32_t: 1", "constraint_set0_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.constraint_set1_flag, settings, "uint32_t: 1", "constraint_set1_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.constraint_set2_flag, settings, "uint32_t: 1", "constraint_set2_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.constraint_set3_flag, settings, "uint32_t: 1", "constraint_set3_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.constraint_set4_flag, settings, "uint32_t: 1", "constraint_set4_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.constraint_set5_flag, settings, "uint32_t: 1", "constraint_set5_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.direct_8x8_inference_flag, settings, "uint32_t: 1", "direct_8x8_inference_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.mb_adaptive_frame_field_flag, settings, "uint32_t: 1", "mb_adaptive_frame_field_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_mbs_only_flag, settings, "uint32_t: 1", "frame_mbs_only_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_pic_order_always_zero_flag, settings, "uint32_t: 1", "delta_pic_order_always_zero_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.separate_colour_plane_flag, settings, "uint32_t: 1", "separate_colour_plane_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.gaps_in_frame_num_value_allowed_flag, settings, "uint32_t: 1", "gaps_in_frame_num_value_allowed_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.qpprime_y_zero_transform_bypass_flag, settings, "uint32_t: 1", "qpprime_y_zero_transform_bypass_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_cropping_flag, settings, "uint32_t: 1", "frame_cropping_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.seq_scaling_matrix_present_flag, settings, "uint32_t: 1", "seq_scaling_matrix_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vui_parameters_present_flag, settings, "uint32_t: 1", "vui_parameters_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264ScalingLists(const StdVideoH264ScalingLists& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint16_t>(object.scaling_list_present_mask, settings, "uint16_t", "scaling_list_present_mask", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.use_default_scaling_matrix_mask, settings, "uint16_t", "use_default_scaling_matrix_mask", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.ScalingList4x4, STD_VIDEO_H264_SCALING_LIST_4X4_NUM_LISTS, STD_VIDEO_H264_SCALING_LIST_4X4_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H264_SCALING_LIST_4X4_NUM_LISTS][STD_VIDEO_H264_SCALING_LIST_4X4_NUM_ELEMENTS]", "ScalingList4x4", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.ScalingList8x8, STD_VIDEO_H264_SCALING_LIST_8X8_NUM_LISTS, STD_VIDEO_H264_SCALING_LIST_8X8_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H264_SCALING_LIST_8X8_NUM_LISTS][STD_VIDEO_H264_SCALING_LIST_8X8_NUM_ELEMENTS]", "ScalingList8x8", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264SequenceParameterSet(const StdVideoH264SequenceParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH264SpsFlags<Format>(object.flags, settings, "StdVideoH264SpsFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264ProfileIdc<Format>(object.profile_idc, settings, "StdVideoH264ProfileIdc", "profile_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264LevelIdc<Format>(object.level_idc, settings, "StdVideoH264LevelIdc", "level_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264ChromaFormatIdc<Format>(object.chroma_format_idc, settings, "StdVideoH264ChromaFormatIdc", "chroma_format_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bit_depth_luma_minus8, settings, "uint8_t", "bit_depth_luma_minus8", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bit_depth_chroma_minus8, settings, "uint8_t", "bit_depth_chroma_minus8", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_max_frame_num_minus4, settings, "uint8_t", "log2_max_frame_num_minus4", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264PocType<Format>(object.pic_order_cnt_type, settings, "StdVideoH264PocType", "pic_order_cnt_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.offset_for_non_ref_pic, settings, "int32_t", "offset_for_non_ref_pic", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.offset_for_top_to_bottom_field, settings, "int32_t", "offset_for_top_to_bottom_field", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_max_pic_order_cnt_lsb_minus4, settings, "uint8_t", "log2_max_pic_order_cnt_lsb_minus4", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_frames_in_pic_order_cnt_cycle, settings, "uint8_t", "num_ref_frames_in_pic_order_cnt_cycle", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.max_num_ref_frames, settings, "uint8_t", "max_num_ref_frames", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pic_width_in_mbs_minus1, settings, "uint32_t", "pic_width_in_mbs_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pic_height_in_map_units_minus1, settings, "uint32_t", "pic_height_in_map_units_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_crop_left_offset, settings, "uint32_t", "frame_crop_left_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_crop_right_offset, settings, "uint32_t", "frame_crop_right_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_crop_top_offset, settings, "uint32_t", "frame_crop_top_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_crop_bottom_offset, settings, "uint32_t", "frame_crop_bottom_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved2, settings, "uint32_t", "reserved2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pOffsetForRefFrame, settings, "const int32_t*", "pOffsetForRefFrame", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, const int32_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pScalingLists, settings, "const StdVideoH264ScalingLists*", "pScalingLists", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH264ScalingLists<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pSequenceParameterSetVui, settings, "const StdVideoH264SequenceParameterSetVui*", "pSequenceParameterSetVui", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH264SequenceParameterSetVui<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264PpsFlags(const StdVideoH264PpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.transform_8x8_mode_flag, settings, "uint32_t: 1", "transform_8x8_mode_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.redundant_pic_cnt_present_flag, settings, "uint32_t: 1", "redundant_pic_cnt_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.constrained_intra_pred_flag, settings, "uint32_t: 1", "constrained_intra_pred_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.deblocking_filter_control_present_flag, settings, "uint32_t: 1", "deblocking_filter_control_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.weighted_pred_flag, settings, "uint32_t: 1", "weighted_pred_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bottom_field_pic_order_in_frame_present_flag, settings, "uint32_t: 1", "bottom_field_pic_order_in_frame_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.entropy_coding_mode_flag, settings, "uint32_t: 1", "entropy_coding_mode_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pic_scaling_matrix_present_flag, settings, "uint32_t: 1", "pic_scaling_matrix_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH264PictureParameterSet(const StdVideoH264PictureParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH264PpsFlags<Format>(object.flags, settings, "StdVideoH264PpsFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pic_parameter_set_id, settings, "uint8_t", "pic_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_l0_default_active_minus1, settings, "uint8_t", "num_ref_idx_l0_default_active_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_l1_default_active_minus1, settings, "uint8_t", "num_ref_idx_l1_default_active_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264WeightedBipredIdc<Format>(object.weighted_bipred_idc, settings, "StdVideoH264WeightedBipredIdc", "weighted_bipred_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.pic_init_qp_minus26, settings, "int8_t", "pic_init_qp_minus26", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.pic_init_qs_minus26, settings, "int8_t", "pic_init_qs_minus26", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.chroma_qp_index_offset, settings, "int8_t", "chroma_qp_index_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.second_chroma_qp_index_offset, settings, "int8_t", "second_chroma_qp_index_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pScalingLists, settings, "const StdVideoH264ScalingLists*", "pScalingLists", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH264ScalingLists<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeH264PictureInfoFlags(const StdVideoDecodeH264PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.field_pic_flag, settings, "uint32_t: 1", "field_pic_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.is_intra, settings, "uint32_t: 1", "is_intra", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.IdrPicFlag, settings, "uint32_t: 1", "IdrPicFlag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bottom_field_flag, settings, "uint32_t: 1", "bottom_field_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.is_reference, settings, "uint32_t: 1", "is_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.complementary_field_pair, settings, "uint32_t: 1", "complementary_field_pair", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeH264PictureInfo(const StdVideoDecodeH264PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoDecodeH264PictureInfoFlags<Format>(object.flags, settings, "StdVideoDecodeH264PictureInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pic_parameter_set_id, settings, "uint8_t", "pic_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.frame_num, settings, "uint16_t", "frame_num", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.idr_pic_id, settings, "uint16_t", "idr_pic_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, "int32_t[STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE]", "PicOrderCnt", "int32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeH264ReferenceInfoFlags(const StdVideoDecodeH264ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.top_field_flag, settings, "uint32_t: 1", "top_field_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bottom_field_flag, settings, "uint32_t: 1", "bottom_field_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.is_non_existing, settings, "uint32_t: 1", "is_non_existing", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeH264ReferenceInfo(const StdVideoDecodeH264ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoDecodeH264ReferenceInfoFlags<Format>(object.flags, settings, "StdVideoDecodeH264ReferenceInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.FrameNum, settings, "uint16_t", "FrameNum", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.reserved, settings, "uint16_t", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, "int32_t[STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE]", "PicOrderCnt", "int32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264WeightTableFlags(const StdVideoEncodeH264WeightTableFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.luma_weight_l0_flag, settings, "uint32_t", "luma_weight_l0_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_weight_l0_flag, settings, "uint32_t", "chroma_weight_l0_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.luma_weight_l1_flag, settings, "uint32_t", "luma_weight_l1_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_weight_l1_flag, settings, "uint32_t", "chroma_weight_l1_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264WeightTable(const StdVideoEncodeH264WeightTable& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH264WeightTableFlags<Format>(object.flags, settings, "StdVideoEncodeH264WeightTableFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.luma_log2_weight_denom, settings, "uint8_t", "luma_log2_weight_denom", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_log2_weight_denom, settings, "uint8_t", "chroma_log2_weight_denom", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.luma_weight_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "luma_weight_l0", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.luma_offset_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "luma_offset_l0", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.chroma_weight_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF][STD_VIDEO_H264_MAX_CHROMA_PLANES]", "chroma_weight_l0", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.chroma_offset_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF][STD_VIDEO_H264_MAX_CHROMA_PLANES]", "chroma_offset_l0", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.luma_weight_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "luma_weight_l1", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.luma_offset_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "luma_offset_l1", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.chroma_weight_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF][STD_VIDEO_H264_MAX_CHROMA_PLANES]", "chroma_weight_l1", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.chroma_offset_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF][STD_VIDEO_H264_MAX_CHROMA_PLANES]", "chroma_offset_l1", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264SliceHeaderFlags(const StdVideoEncodeH264SliceHeaderFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.direct_spatial_mv_pred_flag, settings, "uint32_t: 1", "direct_spatial_mv_pred_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_active_override_flag, settings, "uint32_t: 1", "num_ref_idx_active_override_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 30", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264PictureInfoFlags(const StdVideoEncodeH264PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.IdrPicFlag, settings, "uint32_t: 1", "IdrPicFlag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.is_reference, settings, "uint32_t: 1", "is_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.no_output_of_prior_pics_flag, settings, "uint32_t: 1", "no_output_of_prior_pics_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.long_term_reference_flag, settings, "uint32_t: 1", "long_term_reference_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.adaptive_ref_pic_marking_mode_flag, settings, "uint32_t: 1", "adaptive_ref_pic_marking_mode_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 27", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264ReferenceInfoFlags(const StdVideoEncodeH264ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 31", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264ReferenceListsInfoFlags(const StdVideoEncodeH264ReferenceListsInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.ref_pic_list_modification_flag_l0, settings, "uint32_t: 1", "ref_pic_list_modification_flag_l0", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.ref_pic_list_modification_flag_l1, settings, "uint32_t: 1", "ref_pic_list_modification_flag_l1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 30", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264RefListModEntry(const StdVideoEncodeH264RefListModEntry& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH264ModificationOfPicNumsIdc<Format>(object.modification_of_pic_nums_idc, settings, "StdVideoH264ModificationOfPicNumsIdc", "modification_of_pic_nums_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.abs_diff_pic_num_minus1, settings, "uint16_t", "abs_diff_pic_num_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.long_term_pic_num, settings, "uint16_t", "long_term_pic_num", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264RefPicMarkingEntry(const StdVideoEncodeH264RefPicMarkingEntry& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH264MemMgmtControlOp<Format>(object.memory_management_control_operation, settings, "StdVideoH264MemMgmtControlOp", "memory_management_control_operation", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.difference_of_pic_nums_minus1, settings, "uint16_t", "difference_of_pic_nums_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.long_term_pic_num, settings, "uint16_t", "long_term_pic_num", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.long_term_frame_idx, settings, "uint16_t", "long_term_frame_idx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.max_long_term_frame_idx_plus1, settings, "uint16_t", "max_long_term_frame_idx_plus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264ReferenceListsInfo(const StdVideoEncodeH264ReferenceListsInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH264ReferenceListsInfoFlags<Format>(object.flags, settings, "StdVideoEncodeH264ReferenceListsInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_l0_active_minus1, settings, "uint8_t", "num_ref_idx_l0_active_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_l1_active_minus1, settings, "uint8_t", "num_ref_idx_l1_active_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.RefPicList0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "RefPicList0", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.RefPicList1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "RefPicList1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.refList0ModOpCount, settings, "uint8_t", "refList0ModOpCount", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.refList1ModOpCount, settings, "uint8_t", "refList1ModOpCount", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.refPicMarkingOpCount, settings, "uint8_t", "refPicMarkingOpCount", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved1, 7, settings, "uint8_t[7]", "reserved1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pRefList0ModOperations, settings, "const StdVideoEncodeH264RefListModEntry*", "pRefList0ModOperations", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeH264RefListModEntry<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pRefList1ModOperations, settings, "const StdVideoEncodeH264RefListModEntry*", "pRefList1ModOperations", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeH264RefListModEntry<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pRefPicMarkingOperations, settings, "const StdVideoEncodeH264RefPicMarkingEntry*", "pRefPicMarkingOperations", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeH264RefPicMarkingEntry<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264PictureInfo(const StdVideoEncodeH264PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH264PictureInfoFlags<Format>(object.flags, settings, "StdVideoEncodeH264PictureInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pic_parameter_set_id, settings, "uint8_t", "pic_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.idr_pic_id, settings, "uint16_t", "idr_pic_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264PictureType<Format>(object.primary_pic_type, settings, "StdVideoH264PictureType", "primary_pic_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_num, settings, "uint32_t", "frame_num", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.PicOrderCnt, settings, "int32_t", "PicOrderCnt", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.temporal_id, settings, "uint8_t", "temporal_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved1, 3, settings, "uint8_t[3]", "reserved1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pRefLists, settings, "const StdVideoEncodeH264ReferenceListsInfo*", "pRefLists", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeH264ReferenceListsInfo<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264ReferenceInfo(const StdVideoEncodeH264ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH264ReferenceInfoFlags<Format>(object.flags, settings, "StdVideoEncodeH264ReferenceInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264PictureType<Format>(object.primary_pic_type, settings, "StdVideoH264PictureType", "primary_pic_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.FrameNum, settings, "uint32_t", "FrameNum", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.PicOrderCnt, settings, "int32_t", "PicOrderCnt", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.long_term_pic_num, settings, "uint16_t", "long_term_pic_num", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.long_term_frame_idx, settings, "uint16_t", "long_term_frame_idx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.temporal_id, settings, "uint8_t", "temporal_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH264SliceHeader(const StdVideoEncodeH264SliceHeader& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH264SliceHeaderFlags<Format>(object.flags, settings, "StdVideoEncodeH264SliceHeaderFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.first_mb_in_slice, settings, "uint32_t", "first_mb_in_slice", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264SliceType<Format>(object.slice_type, settings, "StdVideoH264SliceType", "slice_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_alpha_c0_offset_div2, settings, "int8_t", "slice_alpha_c0_offset_div2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_beta_offset_div2, settings, "int8_t", "slice_beta_offset_div2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_qp_delta, settings, "int8_t", "slice_qp_delta", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264CabacInitIdc<Format>(object.cabac_init_idc, settings, "StdVideoH264CabacInitIdc", "cabac_init_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH264DisableDeblockingFilterIdc<Format>(object.disable_deblocking_filter_idc, settings, "StdVideoH264DisableDeblockingFilterIdc", "disable_deblocking_filter_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pWeightTable, settings, "const StdVideoEncodeH264WeightTable*", "pWeightTable", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeH264WeightTable<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265DecPicBufMgr(const StdVideoH265DecPicBufMgr& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_single_array<Format>(object.max_latency_increase_plus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "max_latency_increase_plus1", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.max_dec_pic_buffering_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "max_dec_pic_buffering_minus1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.max_num_reorder_pics, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "max_num_reorder_pics", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265SubLayerHrdParameters(const StdVideoH265SubLayerHrdParameters& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_single_array<Format>(object.bit_rate_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "bit_rate_value_minus1", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cpb_size_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "cpb_size_value_minus1", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cpb_size_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "cpb_size_du_value_minus1", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.bit_rate_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "bit_rate_du_value_minus1", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cbr_flag, settings, "uint32_t", "cbr_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265HrdFlags(const StdVideoH265HrdFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.nal_hrd_parameters_present_flag, settings, "uint32_t: 1", "nal_hrd_parameters_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vcl_hrd_parameters_present_flag, settings, "uint32_t: 1", "vcl_hrd_parameters_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sub_pic_hrd_params_present_flag, settings, "uint32_t: 1", "sub_pic_hrd_params_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sub_pic_cpb_params_in_pic_timing_sei_flag, settings, "uint32_t: 1", "sub_pic_cpb_params_in_pic_timing_sei_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.fixed_pic_rate_general_flag, settings, "uint32_t: 8", "fixed_pic_rate_general_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.fixed_pic_rate_within_cvs_flag, settings, "uint32_t: 8", "fixed_pic_rate_within_cvs_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.low_delay_hrd_flag, settings, "uint32_t: 8", "low_delay_hrd_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265HrdParameters(const StdVideoH265HrdParameters& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH265HrdFlags<Format>(object.flags, settings, "StdVideoH265HrdFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.tick_divisor_minus2, settings, "uint8_t", "tick_divisor_minus2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.du_cpb_removal_delay_increment_length_minus1, settings, "uint8_t", "du_cpb_removal_delay_increment_length_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.dpb_output_delay_du_length_minus1, settings, "uint8_t", "dpb_output_delay_du_length_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bit_rate_scale, settings, "uint8_t", "bit_rate_scale", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cpb_size_scale, settings, "uint8_t", "cpb_size_scale", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cpb_size_du_scale, settings, "uint8_t", "cpb_size_du_scale", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.initial_cpb_removal_delay_length_minus1, settings, "uint8_t", "initial_cpb_removal_delay_length_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.au_cpb_removal_delay_length_minus1, settings, "uint8_t", "au_cpb_removal_delay_length_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.dpb_output_delay_length_minus1, settings, "uint8_t", "dpb_output_delay_length_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cpb_cnt_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "cpb_cnt_minus1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.elemental_duration_in_tc_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "elemental_duration_in_tc_minus1", "uint16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint16_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved, 3, settings, "uint16_t[3]", "reserved", "uint16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint16_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pSubLayerHrdParametersNal, settings, "const StdVideoH265SubLayerHrdParameters*", "pSubLayerHrdParametersNal", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265SubLayerHrdParameters<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pSubLayerHrdParametersVcl, settings, "const StdVideoH265SubLayerHrdParameters*", "pSubLayerHrdParametersVcl", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265SubLayerHrdParameters<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265VpsFlags(const StdVideoH265VpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.vps_temporal_id_nesting_flag, settings, "uint32_t: 1", "vps_temporal_id_nesting_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vps_sub_layer_ordering_info_present_flag, settings, "uint32_t: 1", "vps_sub_layer_ordering_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vps_timing_info_present_flag, settings, "uint32_t: 1", "vps_timing_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vps_poc_proportional_to_timing_flag, settings, "uint32_t: 1", "vps_poc_proportional_to_timing_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265ProfileTierLevelFlags(const StdVideoH265ProfileTierLevelFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.general_tier_flag, settings, "uint32_t: 1", "general_tier_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.general_progressive_source_flag, settings, "uint32_t: 1", "general_progressive_source_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.general_interlaced_source_flag, settings, "uint32_t: 1", "general_interlaced_source_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.general_non_packed_constraint_flag, settings, "uint32_t: 1", "general_non_packed_constraint_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.general_frame_only_constraint_flag, settings, "uint32_t: 1", "general_frame_only_constraint_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265ProfileTierLevel(const StdVideoH265ProfileTierLevel& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH265ProfileTierLevelFlags<Format>(object.flags, settings, "StdVideoH265ProfileTierLevelFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH265ProfileIdc<Format>(object.general_profile_idc, settings, "StdVideoH265ProfileIdc", "general_profile_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH265LevelIdc<Format>(object.general_level_idc, settings, "StdVideoH265LevelIdc", "general_level_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265VideoParameterSet(const StdVideoH265VideoParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH265VpsFlags<Format>(object.flags, settings, "StdVideoH265VpsFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vps_video_parameter_set_id, settings, "uint8_t", "vps_video_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vps_max_sub_layers_minus1, settings, "uint8_t", "vps_max_sub_layers_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vps_num_units_in_tick, settings, "uint32_t", "vps_num_units_in_tick", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vps_time_scale, settings, "uint32_t", "vps_time_scale", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vps_num_ticks_poc_diff_one_minus1, settings, "uint32_t", "vps_num_ticks_poc_diff_one_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved3, settings, "uint32_t", "reserved3", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pDecPicBufMgr, settings, "const StdVideoH265DecPicBufMgr*", "pDecPicBufMgr", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265DecPicBufMgr<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pHrdParameters, settings, "const StdVideoH265HrdParameters*", "pHrdParameters", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265HrdParameters<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pProfileTierLevel, settings, "const StdVideoH265ProfileTierLevel*", "pProfileTierLevel", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265ProfileTierLevel<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265ScalingLists(const StdVideoH265ScalingLists& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_double_array<Format>(object.ScalingList4x4, STD_VIDEO_H265_SCALING_LIST_4X4_NUM_LISTS, STD_VIDEO_H265_SCALING_LIST_4X4_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_4X4_NUM_LISTS][STD_VIDEO_H265_SCALING_LIST_4X4_NUM_ELEMENTS]", "ScalingList4x4", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.ScalingList8x8, STD_VIDEO_H265_SCALING_LIST_8X8_NUM_LISTS, STD_VIDEO_H265_SCALING_LIST_8X8_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_8X8_NUM_LISTS][STD_VIDEO_H265_SCALING_LIST_8X8_NUM_ELEMENTS]", "ScalingList8x8", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.ScalingList16x16, STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS, STD_VIDEO_H265_SCALING_LIST_16X16_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS][STD_VIDEO_H265_SCALING_LIST_16X16_NUM_ELEMENTS]", "ScalingList16x16", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.ScalingList32x32, STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS, STD_VIDEO_H265_SCALING_LIST_32X32_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS][STD_VIDEO_H265_SCALING_LIST_32X32_NUM_ELEMENTS]", "ScalingList32x32", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.ScalingListDCCoef16x16, STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS]", "ScalingListDCCoef16x16", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.ScalingListDCCoef32x32, STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS]", "ScalingListDCCoef32x32", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265SpsVuiFlags(const StdVideoH265SpsVuiFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.aspect_ratio_info_present_flag, settings, "uint32_t: 1", "aspect_ratio_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.overscan_info_present_flag, settings, "uint32_t: 1", "overscan_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.overscan_appropriate_flag, settings, "uint32_t: 1", "overscan_appropriate_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.video_signal_type_present_flag, settings, "uint32_t: 1", "video_signal_type_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.video_full_range_flag, settings, "uint32_t: 1", "video_full_range_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.colour_description_present_flag, settings, "uint32_t: 1", "colour_description_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_loc_info_present_flag, settings, "uint32_t: 1", "chroma_loc_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.neutral_chroma_indication_flag, settings, "uint32_t: 1", "neutral_chroma_indication_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.field_seq_flag, settings, "uint32_t: 1", "field_seq_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_field_info_present_flag, settings, "uint32_t: 1", "frame_field_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.default_display_window_flag, settings, "uint32_t: 1", "default_display_window_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vui_timing_info_present_flag, settings, "uint32_t: 1", "vui_timing_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vui_poc_proportional_to_timing_flag, settings, "uint32_t: 1", "vui_poc_proportional_to_timing_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vui_hrd_parameters_present_flag, settings, "uint32_t: 1", "vui_hrd_parameters_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bitstream_restriction_flag, settings, "uint32_t: 1", "bitstream_restriction_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.tiles_fixed_structure_flag, settings, "uint32_t: 1", "tiles_fixed_structure_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.motion_vectors_over_pic_boundaries_flag, settings, "uint32_t: 1", "motion_vectors_over_pic_boundaries_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.restricted_ref_pic_lists_flag, settings, "uint32_t: 1", "restricted_ref_pic_lists_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265SequenceParameterSetVui(const StdVideoH265SequenceParameterSetVui& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH265SpsVuiFlags<Format>(object.flags, settings, "StdVideoH265SpsVuiFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH265AspectRatioIdc<Format>(object.aspect_ratio_idc, settings, "StdVideoH265AspectRatioIdc", "aspect_ratio_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.sar_width, settings, "uint16_t", "sar_width", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.sar_height, settings, "uint16_t", "sar_height", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.video_format, settings, "uint8_t", "video_format", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.colour_primaries, settings, "uint8_t", "colour_primaries", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.transfer_characteristics, settings, "uint8_t", "transfer_characteristics", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.matrix_coeffs, settings, "uint8_t", "matrix_coeffs", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_sample_loc_type_top_field, settings, "uint8_t", "chroma_sample_loc_type_top_field", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_sample_loc_type_bottom_field, settings, "uint8_t", "chroma_sample_loc_type_bottom_field", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.def_disp_win_left_offset, settings, "uint16_t", "def_disp_win_left_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.def_disp_win_right_offset, settings, "uint16_t", "def_disp_win_right_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.def_disp_win_top_offset, settings, "uint16_t", "def_disp_win_top_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.def_disp_win_bottom_offset, settings, "uint16_t", "def_disp_win_bottom_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vui_num_units_in_tick, settings, "uint32_t", "vui_num_units_in_tick", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vui_time_scale, settings, "uint32_t", "vui_time_scale", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vui_num_ticks_poc_diff_one_minus1, settings, "uint32_t", "vui_num_ticks_poc_diff_one_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.min_spatial_segmentation_idc, settings, "uint16_t", "min_spatial_segmentation_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.reserved3, settings, "uint16_t", "reserved3", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.max_bytes_per_pic_denom, settings, "uint8_t", "max_bytes_per_pic_denom", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.max_bits_per_min_cu_denom, settings, "uint8_t", "max_bits_per_min_cu_denom", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_max_mv_length_horizontal, settings, "uint8_t", "log2_max_mv_length_horizontal", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_max_mv_length_vertical, settings, "uint8_t", "log2_max_mv_length_vertical", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pHrdParameters, settings, "const StdVideoH265HrdParameters*", "pHrdParameters", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265HrdParameters<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265PredictorPaletteEntries(const StdVideoH265PredictorPaletteEntries& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_double_array<Format>(object.PredictorPaletteEntries, STD_VIDEO_H265_PREDICTOR_PALETTE_COMPONENTS_LIST_SIZE, STD_VIDEO_H265_PREDICTOR_PALETTE_COMP_ENTRIES_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_PREDICTOR_PALETTE_COMPONENTS_LIST_SIZE][STD_VIDEO_H265_PREDICTOR_PALETTE_COMP_ENTRIES_LIST_SIZE]", "PredictorPaletteEntries", "uint16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint16_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265SpsFlags(const StdVideoH265SpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.sps_temporal_id_nesting_flag, settings, "uint32_t: 1", "sps_temporal_id_nesting_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.separate_colour_plane_flag, settings, "uint32_t: 1", "separate_colour_plane_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.conformance_window_flag, settings, "uint32_t: 1", "conformance_window_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_sub_layer_ordering_info_present_flag, settings, "uint32_t: 1", "sps_sub_layer_ordering_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.scaling_list_enabled_flag, settings, "uint32_t: 1", "scaling_list_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_scaling_list_data_present_flag, settings, "uint32_t: 1", "sps_scaling_list_data_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.amp_enabled_flag, settings, "uint32_t: 1", "amp_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sample_adaptive_offset_enabled_flag, settings, "uint32_t: 1", "sample_adaptive_offset_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pcm_enabled_flag, settings, "uint32_t: 1", "pcm_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pcm_loop_filter_disabled_flag, settings, "uint32_t: 1", "pcm_loop_filter_disabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.long_term_ref_pics_present_flag, settings, "uint32_t: 1", "long_term_ref_pics_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_temporal_mvp_enabled_flag, settings, "uint32_t: 1", "sps_temporal_mvp_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.strong_intra_smoothing_enabled_flag, settings, "uint32_t: 1", "strong_intra_smoothing_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.vui_parameters_present_flag, settings, "uint32_t: 1", "vui_parameters_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_extension_present_flag, settings, "uint32_t: 1", "sps_extension_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_range_extension_flag, settings, "uint32_t: 1", "sps_range_extension_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.transform_skip_rotation_enabled_flag, settings, "uint32_t: 1", "transform_skip_rotation_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.transform_skip_context_enabled_flag, settings, "uint32_t: 1", "transform_skip_context_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.implicit_rdpcm_enabled_flag, settings, "uint32_t: 1", "implicit_rdpcm_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.explicit_rdpcm_enabled_flag, settings, "uint32_t: 1", "explicit_rdpcm_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.extended_precision_processing_flag, settings, "uint32_t: 1", "extended_precision_processing_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.intra_smoothing_disabled_flag, settings, "uint32_t: 1", "intra_smoothing_disabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.high_precision_offsets_enabled_flag, settings, "uint32_t: 1", "high_precision_offsets_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.persistent_rice_adaptation_enabled_flag, settings, "uint32_t: 1", "persistent_rice_adaptation_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cabac_bypass_alignment_enabled_flag, settings, "uint32_t: 1", "cabac_bypass_alignment_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_scc_extension_flag, settings, "uint32_t: 1", "sps_scc_extension_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_curr_pic_ref_enabled_flag, settings, "uint32_t: 1", "sps_curr_pic_ref_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.palette_mode_enabled_flag, settings, "uint32_t: 1", "palette_mode_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_palette_predictor_initializers_present_flag, settings, "uint32_t: 1", "sps_palette_predictor_initializers_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.intra_boundary_filtering_disabled_flag, settings, "uint32_t: 1", "intra_boundary_filtering_disabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265ShortTermRefPicSetFlags(const StdVideoH265ShortTermRefPicSetFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.inter_ref_pic_set_prediction_flag, settings, "uint32_t: 1", "inter_ref_pic_set_prediction_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_rps_sign, settings, "uint32_t: 1", "delta_rps_sign", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265ShortTermRefPicSet(const StdVideoH265ShortTermRefPicSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH265ShortTermRefPicSetFlags<Format>(object.flags, settings, "StdVideoH265ShortTermRefPicSetFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_idx_minus1, settings, "uint32_t", "delta_idx_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.use_delta_flag, settings, "uint16_t", "use_delta_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.abs_delta_rps_minus1, settings, "uint16_t", "abs_delta_rps_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.used_by_curr_pic_flag, settings, "uint16_t", "used_by_curr_pic_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.used_by_curr_pic_s0_flag, settings, "uint16_t", "used_by_curr_pic_s0_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.used_by_curr_pic_s1_flag, settings, "uint16_t", "used_by_curr_pic_s1_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.reserved1, settings, "uint16_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved3, settings, "uint8_t", "reserved3", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_negative_pics, settings, "uint8_t", "num_negative_pics", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_positive_pics, settings, "uint8_t", "num_positive_pics", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.delta_poc_s0_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, "uint16_t[STD_VIDEO_H265_MAX_DPB_SIZE]", "delta_poc_s0_minus1", "uint16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint16_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.delta_poc_s1_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, "uint16_t[STD_VIDEO_H265_MAX_DPB_SIZE]", "delta_poc_s1_minus1", "uint16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint16_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265LongTermRefPicsSps(const StdVideoH265LongTermRefPicsSps& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.used_by_curr_pic_lt_sps_flag, settings, "uint32_t", "used_by_curr_pic_lt_sps_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.lt_ref_pic_poc_lsb_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, "uint32_t[STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS]", "lt_ref_pic_poc_lsb_sps", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265SequenceParameterSet(const StdVideoH265SequenceParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH265SpsFlags<Format>(object.flags, settings, "StdVideoH265SpsFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH265ChromaFormatIdc<Format>(object.chroma_format_idc, settings, "StdVideoH265ChromaFormatIdc", "chroma_format_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pic_width_in_luma_samples, settings, "uint32_t", "pic_width_in_luma_samples", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pic_height_in_luma_samples, settings, "uint32_t", "pic_height_in_luma_samples", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_max_sub_layers_minus1, settings, "uint8_t", "sps_max_sub_layers_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_seq_parameter_set_id, settings, "uint8_t", "sps_seq_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bit_depth_luma_minus8, settings, "uint8_t", "bit_depth_luma_minus8", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.bit_depth_chroma_minus8, settings, "uint8_t", "bit_depth_chroma_minus8", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_max_pic_order_cnt_lsb_minus4, settings, "uint8_t", "log2_max_pic_order_cnt_lsb_minus4", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_min_luma_coding_block_size_minus3, settings, "uint8_t", "log2_min_luma_coding_block_size_minus3", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_diff_max_min_luma_coding_block_size, settings, "uint8_t", "log2_diff_max_min_luma_coding_block_size", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_min_luma_transform_block_size_minus2, settings, "uint8_t", "log2_min_luma_transform_block_size_minus2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_diff_max_min_luma_transform_block_size, settings, "uint8_t", "log2_diff_max_min_luma_transform_block_size", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.max_transform_hierarchy_depth_inter, settings, "uint8_t", "max_transform_hierarchy_depth_inter", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.max_transform_hierarchy_depth_intra, settings, "uint8_t", "max_transform_hierarchy_depth_intra", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_short_term_ref_pic_sets, settings, "uint8_t", "num_short_term_ref_pic_sets", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_long_term_ref_pics_sps, settings, "uint8_t", "num_long_term_ref_pics_sps", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pcm_sample_bit_depth_luma_minus1, settings, "uint8_t", "pcm_sample_bit_depth_luma_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pcm_sample_bit_depth_chroma_minus1, settings, "uint8_t", "pcm_sample_bit_depth_chroma_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_min_pcm_luma_coding_block_size_minus3, settings, "uint8_t", "log2_min_pcm_luma_coding_block_size_minus3", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_diff_max_min_pcm_luma_coding_block_size, settings, "uint8_t", "log2_diff_max_min_pcm_luma_coding_block_size", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.palette_max_size, settings, "uint8_t", "palette_max_size", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_palette_max_predictor_size, settings, "uint8_t", "delta_palette_max_predictor_size", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.motion_vector_resolution_control_idc, settings, "uint8_t", "motion_vector_resolution_control_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_num_palette_predictor_initializers_minus1, settings, "uint8_t", "sps_num_palette_predictor_initializers_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.conf_win_left_offset, settings, "uint32_t", "conf_win_left_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.conf_win_right_offset, settings, "uint32_t", "conf_win_right_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.conf_win_top_offset, settings, "uint32_t", "conf_win_top_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.conf_win_bottom_offset, settings, "uint32_t", "conf_win_bottom_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pProfileTierLevel, settings, "const StdVideoH265ProfileTierLevel*", "pProfileTierLevel", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265ProfileTierLevel<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pDecPicBufMgr, settings, "const StdVideoH265DecPicBufMgr*", "pDecPicBufMgr", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265DecPicBufMgr<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pScalingLists, settings, "const StdVideoH265ScalingLists*", "pScalingLists", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265ScalingLists<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pShortTermRefPicSet, settings, "const StdVideoH265ShortTermRefPicSet*", "pShortTermRefPicSet", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265ShortTermRefPicSet<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pLongTermRefPicsSps, settings, "const StdVideoH265LongTermRefPicsSps*", "pLongTermRefPicsSps", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265LongTermRefPicsSps<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pSequenceParameterSetVui, settings, "const StdVideoH265SequenceParameterSetVui*", "pSequenceParameterSetVui", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265SequenceParameterSetVui<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pPredictorPaletteEntries, settings, "const StdVideoH265PredictorPaletteEntries*", "pPredictorPaletteEntries", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265PredictorPaletteEntries<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265PpsFlags(const StdVideoH265PpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.dependent_slice_segments_enabled_flag, settings, "uint32_t: 1", "dependent_slice_segments_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.output_flag_present_flag, settings, "uint32_t: 1", "output_flag_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sign_data_hiding_enabled_flag, settings, "uint32_t: 1", "sign_data_hiding_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cabac_init_present_flag, settings, "uint32_t: 1", "cabac_init_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.constrained_intra_pred_flag, settings, "uint32_t: 1", "constrained_intra_pred_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.transform_skip_enabled_flag, settings, "uint32_t: 1", "transform_skip_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cu_qp_delta_enabled_flag, settings, "uint32_t: 1", "cu_qp_delta_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_slice_chroma_qp_offsets_present_flag, settings, "uint32_t: 1", "pps_slice_chroma_qp_offsets_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.weighted_pred_flag, settings, "uint32_t: 1", "weighted_pred_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.weighted_bipred_flag, settings, "uint32_t: 1", "weighted_bipred_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.transquant_bypass_enabled_flag, settings, "uint32_t: 1", "transquant_bypass_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.tiles_enabled_flag, settings, "uint32_t: 1", "tiles_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.entropy_coding_sync_enabled_flag, settings, "uint32_t: 1", "entropy_coding_sync_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.uniform_spacing_flag, settings, "uint32_t: 1", "uniform_spacing_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.loop_filter_across_tiles_enabled_flag, settings, "uint32_t: 1", "loop_filter_across_tiles_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_loop_filter_across_slices_enabled_flag, settings, "uint32_t: 1", "pps_loop_filter_across_slices_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.deblocking_filter_control_present_flag, settings, "uint32_t: 1", "deblocking_filter_control_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.deblocking_filter_override_enabled_flag, settings, "uint32_t: 1", "deblocking_filter_override_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_deblocking_filter_disabled_flag, settings, "uint32_t: 1", "pps_deblocking_filter_disabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_scaling_list_data_present_flag, settings, "uint32_t: 1", "pps_scaling_list_data_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.lists_modification_present_flag, settings, "uint32_t: 1", "lists_modification_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.slice_segment_header_extension_present_flag, settings, "uint32_t: 1", "slice_segment_header_extension_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_extension_present_flag, settings, "uint32_t: 1", "pps_extension_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cross_component_prediction_enabled_flag, settings, "uint32_t: 1", "cross_component_prediction_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_qp_offset_list_enabled_flag, settings, "uint32_t: 1", "chroma_qp_offset_list_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_curr_pic_ref_enabled_flag, settings, "uint32_t: 1", "pps_curr_pic_ref_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.residual_adaptive_colour_transform_enabled_flag, settings, "uint32_t: 1", "residual_adaptive_colour_transform_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_slice_act_qp_offsets_present_flag, settings, "uint32_t: 1", "pps_slice_act_qp_offsets_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_palette_predictor_initializers_present_flag, settings, "uint32_t: 1", "pps_palette_predictor_initializers_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.monochrome_palette_flag, settings, "uint32_t: 1", "monochrome_palette_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_range_extension_flag, settings, "uint32_t: 1", "pps_range_extension_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoH265PictureParameterSet(const StdVideoH265PictureParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoH265PpsFlags<Format>(object.flags, settings, "StdVideoH265PpsFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_pic_parameter_set_id, settings, "uint8_t", "pps_pic_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_seq_parameter_set_id, settings, "uint8_t", "pps_seq_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_extra_slice_header_bits, settings, "uint8_t", "num_extra_slice_header_bits", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_l0_default_active_minus1, settings, "uint8_t", "num_ref_idx_l0_default_active_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_l1_default_active_minus1, settings, "uint8_t", "num_ref_idx_l1_default_active_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.init_qp_minus26, settings, "int8_t", "init_qp_minus26", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.diff_cu_qp_delta_depth, settings, "uint8_t", "diff_cu_qp_delta_depth", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.pps_cb_qp_offset, settings, "int8_t", "pps_cb_qp_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.pps_cr_qp_offset, settings, "int8_t", "pps_cr_qp_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.pps_beta_offset_div2, settings, "int8_t", "pps_beta_offset_div2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.pps_tc_offset_div2, settings, "int8_t", "pps_tc_offset_div2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_parallel_merge_level_minus2, settings, "uint8_t", "log2_parallel_merge_level_minus2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_max_transform_skip_block_size_minus2, settings, "uint8_t", "log2_max_transform_skip_block_size_minus2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.diff_cu_chroma_qp_offset_depth, settings, "uint8_t", "diff_cu_chroma_qp_offset_depth", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_qp_offset_list_len_minus1, settings, "uint8_t", "chroma_qp_offset_list_len_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cb_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, "int8_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE]", "cb_qp_offset_list", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cr_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, "int8_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE]", "cr_qp_offset_list", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_sao_offset_scale_luma, settings, "uint8_t", "log2_sao_offset_scale_luma", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.log2_sao_offset_scale_chroma, settings, "uint8_t", "log2_sao_offset_scale_chroma", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.pps_act_y_qp_offset_plus5, settings, "int8_t", "pps_act_y_qp_offset_plus5", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.pps_act_cb_qp_offset_plus5, settings, "int8_t", "pps_act_cb_qp_offset_plus5", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.pps_act_cr_qp_offset_plus3, settings, "int8_t", "pps_act_cr_qp_offset_plus3", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_num_palette_predictor_initializers, settings, "uint8_t", "pps_num_palette_predictor_initializers", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.luma_bit_depth_entry_minus8, settings, "uint8_t", "luma_bit_depth_entry_minus8", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.chroma_bit_depth_entry_minus8, settings, "uint8_t", "chroma_bit_depth_entry_minus8", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_tile_columns_minus1, settings, "uint8_t", "num_tile_columns_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_tile_rows_minus1, settings, "uint8_t", "num_tile_rows_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.column_width_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE]", "column_width_minus1", "uint16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint16_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.row_height_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE]", "row_height_minus1", "uint16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint16_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved3, settings, "uint32_t", "reserved3", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pScalingLists, settings, "const StdVideoH265ScalingLists*", "pScalingLists", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265ScalingLists<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pPredictorPaletteEntries, settings, "const StdVideoH265PredictorPaletteEntries*", "pPredictorPaletteEntries", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265PredictorPaletteEntries<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeH265PictureInfoFlags(const StdVideoDecodeH265PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.IrapPicFlag, settings, "uint32_t: 1", "IrapPicFlag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.IdrPicFlag, settings, "uint32_t: 1", "IdrPicFlag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.IsReference, settings, "uint32_t: 1", "IsReference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.short_term_ref_pic_set_sps_flag, settings, "uint32_t: 1", "short_term_ref_pic_set_sps_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeH265PictureInfo(const StdVideoDecodeH265PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoDecodeH265PictureInfoFlags<Format>(object.flags, settings, "StdVideoDecodeH265PictureInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_seq_parameter_set_id, settings, "uint8_t", "pps_seq_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_pic_parameter_set_id, settings, "uint8_t", "pps_pic_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.NumDeltaPocsOfRefRpsIdx, settings, "uint8_t", "NumDeltaPocsOfRefRpsIdx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.NumBitsForSTRefPicSetInSlice, settings, "uint16_t", "NumBitsForSTRefPicSetInSlice", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.reserved, settings, "uint16_t", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.RefPicSetStCurrBefore, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "RefPicSetStCurrBefore", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.RefPicSetStCurrAfter, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "RefPicSetStCurrAfter", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.RefPicSetLtCurr, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "RefPicSetLtCurr", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeH265ReferenceInfoFlags(const StdVideoDecodeH265ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.unused_for_reference, settings, "uint32_t: 1", "unused_for_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeH265ReferenceInfo(const StdVideoDecodeH265ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoDecodeH265ReferenceInfoFlags<Format>(object.flags, settings, "StdVideoDecodeH265ReferenceInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265WeightTableFlags(const StdVideoEncodeH265WeightTableFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint16_t>(object.luma_weight_l0_flag, settings, "uint16_t", "luma_weight_l0_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.chroma_weight_l0_flag, settings, "uint16_t", "chroma_weight_l0_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.luma_weight_l1_flag, settings, "uint16_t", "luma_weight_l1_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.chroma_weight_l1_flag, settings, "uint16_t", "chroma_weight_l1_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265WeightTable(const StdVideoEncodeH265WeightTable& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH265WeightTableFlags<Format>(object.flags, settings, "StdVideoEncodeH265WeightTableFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.luma_log2_weight_denom, settings, "uint8_t", "luma_log2_weight_denom", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.delta_chroma_log2_weight_denom, settings, "int8_t", "delta_chroma_log2_weight_denom", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.delta_luma_weight_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "delta_luma_weight_l0", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.luma_offset_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "luma_offset_l0", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.delta_chroma_weight_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF][STD_VIDEO_H265_MAX_CHROMA_PLANES]", "delta_chroma_weight_l0", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.delta_chroma_offset_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF][STD_VIDEO_H265_MAX_CHROMA_PLANES]", "delta_chroma_offset_l0", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.delta_luma_weight_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "delta_luma_weight_l1", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.luma_offset_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "luma_offset_l1", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.delta_chroma_weight_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF][STD_VIDEO_H265_MAX_CHROMA_PLANES]", "delta_chroma_weight_l1", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.delta_chroma_offset_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF][STD_VIDEO_H265_MAX_CHROMA_PLANES]", "delta_chroma_offset_l1", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265SliceSegmentHeaderFlags(const StdVideoEncodeH265SliceSegmentHeaderFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.first_slice_segment_in_pic_flag, settings, "uint32_t: 1", "first_slice_segment_in_pic_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.dependent_slice_segment_flag, settings, "uint32_t: 1", "dependent_slice_segment_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.slice_sao_luma_flag, settings, "uint32_t: 1", "slice_sao_luma_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.slice_sao_chroma_flag, settings, "uint32_t: 1", "slice_sao_chroma_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_active_override_flag, settings, "uint32_t: 1", "num_ref_idx_active_override_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.mvd_l1_zero_flag, settings, "uint32_t: 1", "mvd_l1_zero_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cabac_init_flag, settings, "uint32_t: 1", "cabac_init_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cu_chroma_qp_offset_enabled_flag, settings, "uint32_t: 1", "cu_chroma_qp_offset_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.deblocking_filter_override_flag, settings, "uint32_t: 1", "deblocking_filter_override_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.slice_deblocking_filter_disabled_flag, settings, "uint32_t: 1", "slice_deblocking_filter_disabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.collocated_from_l0_flag, settings, "uint32_t: 1", "collocated_from_l0_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.slice_loop_filter_across_slices_enabled_flag, settings, "uint32_t: 1", "slice_loop_filter_across_slices_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 20", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265SliceSegmentHeader(const StdVideoEncodeH265SliceSegmentHeader& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH265SliceSegmentHeaderFlags<Format>(object.flags, settings, "StdVideoEncodeH265SliceSegmentHeaderFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH265SliceType<Format>(object.slice_type, settings, "StdVideoH265SliceType", "slice_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.slice_segment_address, settings, "uint32_t", "slice_segment_address", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.collocated_ref_idx, settings, "uint8_t", "collocated_ref_idx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.MaxNumMergeCand, settings, "uint8_t", "MaxNumMergeCand", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_cb_qp_offset, settings, "int8_t", "slice_cb_qp_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_cr_qp_offset, settings, "int8_t", "slice_cr_qp_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_beta_offset_div2, settings, "int8_t", "slice_beta_offset_div2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_tc_offset_div2, settings, "int8_t", "slice_tc_offset_div2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_act_y_qp_offset, settings, "int8_t", "slice_act_y_qp_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_act_cb_qp_offset, settings, "int8_t", "slice_act_cb_qp_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_act_cr_qp_offset, settings, "int8_t", "slice_act_cr_qp_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.slice_qp_delta, settings, "int8_t", "slice_qp_delta", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.reserved1, settings, "uint16_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pWeightTable, settings, "const StdVideoEncodeH265WeightTable*", "pWeightTable", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeH265WeightTable<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265ReferenceListsInfoFlags(const StdVideoEncodeH265ReferenceListsInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.ref_pic_list_modification_flag_l0, settings, "uint32_t: 1", "ref_pic_list_modification_flag_l0", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.ref_pic_list_modification_flag_l1, settings, "uint32_t: 1", "ref_pic_list_modification_flag_l1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 30", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265ReferenceListsInfo(const StdVideoEncodeH265ReferenceListsInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH265ReferenceListsInfoFlags<Format>(object.flags, settings, "StdVideoEncodeH265ReferenceListsInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_l0_active_minus1, settings, "uint8_t", "num_ref_idx_l0_active_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ref_idx_l1_active_minus1, settings, "uint8_t", "num_ref_idx_l1_active_minus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.RefPicList0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "RefPicList0", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.RefPicList1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "RefPicList1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.list_entry_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "list_entry_l0", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.list_entry_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "list_entry_l1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265PictureInfoFlags(const StdVideoEncodeH265PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.is_reference, settings, "uint32_t: 1", "is_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.IrapPicFlag, settings, "uint32_t: 1", "IrapPicFlag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.discardable_flag, settings, "uint32_t: 1", "discardable_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cross_layer_bla_flag, settings, "uint32_t: 1", "cross_layer_bla_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pic_output_flag, settings, "uint32_t: 1", "pic_output_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.no_output_of_prior_pics_flag, settings, "uint32_t: 1", "no_output_of_prior_pics_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.short_term_ref_pic_set_sps_flag, settings, "uint32_t: 1", "short_term_ref_pic_set_sps_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.slice_temporal_mvp_enabled_flag, settings, "uint32_t: 1", "slice_temporal_mvp_enabled_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 23", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265LongTermRefPics(const StdVideoEncodeH265LongTermRefPics& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.num_long_term_sps, settings, "uint8_t", "num_long_term_sps", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_long_term_pics, settings, "uint8_t", "num_long_term_pics", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.lt_idx_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, "uint8_t[STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS]", "lt_idx_sps", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.poc_lsb_lt, STD_VIDEO_H265_MAX_LONG_TERM_PICS, settings, "uint8_t[STD_VIDEO_H265_MAX_LONG_TERM_PICS]", "poc_lsb_lt", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.used_by_curr_pic_lt_flag, settings, "uint16_t", "used_by_curr_pic_lt_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.delta_poc_msb_present_flag, STD_VIDEO_H265_MAX_DELTA_POC, settings, "uint8_t[STD_VIDEO_H265_MAX_DELTA_POC]", "delta_poc_msb_present_flag", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.delta_poc_msb_cycle_lt, STD_VIDEO_H265_MAX_DELTA_POC, settings, "uint8_t[STD_VIDEO_H265_MAX_DELTA_POC]", "delta_poc_msb_cycle_lt", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265PictureInfo(const StdVideoEncodeH265PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH265PictureInfoFlags<Format>(object.flags, settings, "StdVideoEncodeH265PictureInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH265PictureType<Format>(object.pic_type, settings, "StdVideoH265PictureType", "pic_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_seq_parameter_set_id, settings, "uint8_t", "pps_seq_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.pps_pic_parameter_set_id, settings, "uint8_t", "pps_pic_parameter_set_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.short_term_ref_pic_set_idx, settings, "uint8_t", "short_term_ref_pic_set_idx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.TemporalId, settings, "uint8_t", "TemporalId", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved1, 7, settings, "uint8_t[7]", "reserved1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pRefLists, settings, "const StdVideoEncodeH265ReferenceListsInfo*", "pRefLists", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeH265ReferenceListsInfo<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pShortTermRefPicSet, settings, "const StdVideoH265ShortTermRefPicSet*", "pShortTermRefPicSet", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoH265ShortTermRefPicSet<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pLongTermRefPics, settings, "const StdVideoEncodeH265LongTermRefPics*", "pLongTermRefPics", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeH265LongTermRefPics<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265ReferenceInfoFlags(const StdVideoEncodeH265ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.unused_for_reference, settings, "uint32_t: 1", "unused_for_reference", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 30", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeH265ReferenceInfo(const StdVideoEncodeH265ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeH265ReferenceInfoFlags<Format>(object.flags, settings, "StdVideoEncodeH265ReferenceInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoH265PictureType<Format>(object.pic_type, settings, "StdVideoH265PictureType", "pic_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.TemporalId, settings, "uint8_t", "TemporalId", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1ColorConfigFlags(const StdVideoAV1ColorConfigFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.mono_chrome, settings, "uint32_t: 1", "mono_chrome", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.color_range, settings, "uint32_t: 1", "color_range", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.separate_uv_delta_q, settings, "uint32_t: 1", "separate_uv_delta_q", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.color_description_present_flag, settings, "uint32_t: 1", "color_description_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 28", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1ColorConfig(const StdVideoAV1ColorConfig& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoAV1ColorConfigFlags<Format>(object.flags, settings, "StdVideoAV1ColorConfigFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.BitDepth, settings, "uint8_t", "BitDepth", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.subsampling_x, settings, "uint8_t", "subsampling_x", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.subsampling_y, settings, "uint8_t", "subsampling_y", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1ColorPrimaries<Format>(object.color_primaries, settings, "StdVideoAV1ColorPrimaries", "color_primaries", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1TransferCharacteristics<Format>(object.transfer_characteristics, settings, "StdVideoAV1TransferCharacteristics", "transfer_characteristics", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1MatrixCoefficients<Format>(object.matrix_coefficients, settings, "StdVideoAV1MatrixCoefficients", "matrix_coefficients", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1ChromaSamplePosition<Format>(object.chroma_sample_position, settings, "StdVideoAV1ChromaSamplePosition", "chroma_sample_position", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1TimingInfoFlags(const StdVideoAV1TimingInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.equal_picture_interval, settings, "uint32_t: 1", "equal_picture_interval", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 31", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1TimingInfo(const StdVideoAV1TimingInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoAV1TimingInfoFlags<Format>(object.flags, settings, "StdVideoAV1TimingInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_units_in_display_tick, settings, "uint32_t", "num_units_in_display_tick", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.time_scale, settings, "uint32_t", "time_scale", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_ticks_per_picture_minus_1, settings, "uint32_t", "num_ticks_per_picture_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1LoopFilterFlags(const StdVideoAV1LoopFilterFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.loop_filter_delta_enabled, settings, "uint32_t: 1", "loop_filter_delta_enabled", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.loop_filter_delta_update, settings, "uint32_t: 1", "loop_filter_delta_update", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 30", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1LoopFilter(const StdVideoAV1LoopFilter& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoAV1LoopFilterFlags<Format>(object.flags, settings, "StdVideoAV1LoopFilterFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.loop_filter_level, STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS]", "loop_filter_level", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.loop_filter_sharpness, settings, "uint8_t", "loop_filter_sharpness", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.update_ref_delta, settings, "uint8_t", "update_ref_delta", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.loop_filter_ref_deltas, STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME, settings, "int8_t[STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME]", "loop_filter_ref_deltas", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.update_mode_delta, settings, "uint8_t", "update_mode_delta", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.loop_filter_mode_deltas, STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS, settings, "int8_t[STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS]", "loop_filter_mode_deltas", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1QuantizationFlags(const StdVideoAV1QuantizationFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.using_qmatrix, settings, "uint32_t: 1", "using_qmatrix", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.diff_uv_delta, settings, "uint32_t: 1", "diff_uv_delta", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 30", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1Quantization(const StdVideoAV1Quantization& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoAV1QuantizationFlags<Format>(object.flags, settings, "StdVideoAV1QuantizationFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.base_q_idx, settings, "uint8_t", "base_q_idx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.DeltaQYDc, settings, "int8_t", "DeltaQYDc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.DeltaQUDc, settings, "int8_t", "DeltaQUDc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.DeltaQUAc, settings, "int8_t", "DeltaQUAc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.DeltaQVDc, settings, "int8_t", "DeltaQVDc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.DeltaQVAc, settings, "int8_t", "DeltaQVAc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.qm_y, settings, "uint8_t", "qm_y", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.qm_u, settings, "uint8_t", "qm_u", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.qm_v, settings, "uint8_t", "qm_v", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1Segmentation(const StdVideoAV1Segmentation& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_single_array<Format>(object.FeatureEnabled, STD_VIDEO_AV1_MAX_SEGMENTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_SEGMENTS]", "FeatureEnabled", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.FeatureData, STD_VIDEO_AV1_MAX_SEGMENTS, STD_VIDEO_AV1_SEG_LVL_MAX, settings, "int16_t[STD_VIDEO_AV1_MAX_SEGMENTS][STD_VIDEO_AV1_SEG_LVL_MAX]", "FeatureData", "int16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int16_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1TileInfoFlags(const StdVideoAV1TileInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.uniform_tile_spacing_flag, settings, "uint32_t: 1", "uniform_tile_spacing_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 31", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1TileInfo(const StdVideoAV1TileInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoAV1TileInfoFlags<Format>(object.flags, settings, "StdVideoAV1TileInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.TileCols, settings, "uint8_t", "TileCols", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.TileRows, settings, "uint8_t", "TileRows", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.context_update_tile_id, settings, "uint16_t", "context_update_tile_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.tile_size_bytes_minus_1, settings, "uint8_t", "tile_size_bytes_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved1, 7, settings, "uint8_t[7]", "reserved1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pMiColStarts, settings, "const uint16_t*", "pMiColStarts", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, const uint16_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pMiRowStarts, settings, "const uint16_t*", "pMiRowStarts", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, const uint16_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pWidthInSbsMinus1, settings, "const uint16_t*", "pWidthInSbsMinus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, const uint16_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pHeightInSbsMinus1, settings, "const uint16_t*", "pHeightInSbsMinus1", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, const uint16_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1CDEF(const StdVideoAV1CDEF& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.cdef_damping_minus_3, settings, "uint8_t", "cdef_damping_minus_3", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cdef_bits, settings, "uint8_t", "cdef_bits", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cdef_y_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "cdef_y_pri_strength", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cdef_y_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "cdef_y_sec_strength", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cdef_uv_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "cdef_uv_pri_strength", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.cdef_uv_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "cdef_uv_sec_strength", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1LoopRestoration(const StdVideoAV1LoopRestoration& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_single_array<Format>(object.FrameRestorationType, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, "StdVideoAV1FrameRestorationType[STD_VIDEO_AV1_MAX_NUM_PLANES]", "FrameRestorationType", "StdVideoAV1FrameRestorationType", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1FrameRestorationType<Format>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.LoopRestorationSize, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, "uint16_t[STD_VIDEO_AV1_MAX_NUM_PLANES]", "LoopRestorationSize", "uint16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint16_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1GlobalMotion(const StdVideoAV1GlobalMotion& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_single_array<Format>(object.GmType, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "GmType", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.gm_params, STD_VIDEO_AV1_NUM_REF_FRAMES, STD_VIDEO_AV1_GLOBAL_MOTION_PARAMS, settings, "int32_t[STD_VIDEO_AV1_NUM_REF_FRAMES][STD_VIDEO_AV1_GLOBAL_MOTION_PARAMS]", "gm_params", "int32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1FilmGrainFlags(const StdVideoAV1FilmGrainFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.chroma_scaling_from_luma, settings, "uint32_t: 1", "chroma_scaling_from_luma", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.overlap_flag, settings, "uint32_t: 1", "overlap_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.clip_to_restricted_range, settings, "uint32_t: 1", "clip_to_restricted_range", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.update_grain, settings, "uint32_t: 1", "update_grain", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 28", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1FilmGrain(const StdVideoAV1FilmGrain& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoAV1FilmGrainFlags<Format>(object.flags, settings, "StdVideoAV1FilmGrainFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.grain_scaling_minus_8, settings, "uint8_t", "grain_scaling_minus_8", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.ar_coeff_lag, settings, "uint8_t", "ar_coeff_lag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.ar_coeff_shift_minus_6, settings, "uint8_t", "ar_coeff_shift_minus_6", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.grain_scale_shift, settings, "uint8_t", "grain_scale_shift", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.grain_seed, settings, "uint16_t", "grain_seed", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.film_grain_params_ref_idx, settings, "uint8_t", "film_grain_params_ref_idx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_y_points, settings, "uint8_t", "num_y_points", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.point_y_value, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_Y_POINTS]", "point_y_value", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.point_y_scaling, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_Y_POINTS]", "point_y_scaling", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_cb_points, settings, "uint8_t", "num_cb_points", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.point_cb_value, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CB_POINTS]", "point_cb_value", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.point_cb_scaling, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CB_POINTS]", "point_cb_scaling", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_cr_points, settings, "uint8_t", "num_cr_points", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.point_cr_value, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CR_POINTS]", "point_cr_value", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.point_cr_scaling, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CR_POINTS]", "point_cr_scaling", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.ar_coeffs_y_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_LUMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_LUMA]", "ar_coeffs_y_plus_128", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.ar_coeffs_cb_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_CHROMA]", "ar_coeffs_cb_plus_128", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.ar_coeffs_cr_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_CHROMA]", "ar_coeffs_cr_plus_128", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cb_mult, settings, "uint8_t", "cb_mult", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cb_luma_mult, settings, "uint8_t", "cb_luma_mult", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.cb_offset, settings, "uint16_t", "cb_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cr_mult, settings, "uint8_t", "cr_mult", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.cr_luma_mult, settings, "uint8_t", "cr_luma_mult", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.cr_offset, settings, "uint16_t", "cr_offset", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1SequenceHeaderFlags(const StdVideoAV1SequenceHeaderFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.still_picture, settings, "uint32_t: 1", "still_picture", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reduced_still_picture_header, settings, "uint32_t: 1", "reduced_still_picture_header", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.use_128x128_superblock, settings, "uint32_t: 1", "use_128x128_superblock", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_filter_intra, settings, "uint32_t: 1", "enable_filter_intra", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_intra_edge_filter, settings, "uint32_t: 1", "enable_intra_edge_filter", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_interintra_compound, settings, "uint32_t: 1", "enable_interintra_compound", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_masked_compound, settings, "uint32_t: 1", "enable_masked_compound", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_warped_motion, settings, "uint32_t: 1", "enable_warped_motion", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_dual_filter, settings, "uint32_t: 1", "enable_dual_filter", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_order_hint, settings, "uint32_t: 1", "enable_order_hint", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_jnt_comp, settings, "uint32_t: 1", "enable_jnt_comp", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_ref_frame_mvs, settings, "uint32_t: 1", "enable_ref_frame_mvs", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_id_numbers_present_flag, settings, "uint32_t: 1", "frame_id_numbers_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_superres, settings, "uint32_t: 1", "enable_superres", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_cdef, settings, "uint32_t: 1", "enable_cdef", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.enable_restoration, settings, "uint32_t: 1", "enable_restoration", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.film_grain_params_present, settings, "uint32_t: 1", "film_grain_params_present", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.timing_info_present_flag, settings, "uint32_t: 1", "timing_info_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.initial_display_delay_present_flag, settings, "uint32_t: 1", "initial_display_delay_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 13", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoAV1SequenceHeader(const StdVideoAV1SequenceHeader& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoAV1SequenceHeaderFlags<Format>(object.flags, settings, "StdVideoAV1SequenceHeaderFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1Profile<Format>(object.seq_profile, settings, "StdVideoAV1Profile", "seq_profile", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_width_bits_minus_1, settings, "uint8_t", "frame_width_bits_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_height_bits_minus_1, settings, "uint8_t", "frame_height_bits_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.max_frame_width_minus_1, settings, "uint16_t", "max_frame_width_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.max_frame_height_minus_1, settings, "uint16_t", "max_frame_height_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_frame_id_length_minus_2, settings, "uint8_t", "delta_frame_id_length_minus_2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.additional_frame_id_length_minus_1, settings, "uint8_t", "additional_frame_id_length_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.order_hint_bits_minus_1, settings, "uint8_t", "order_hint_bits_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.seq_force_integer_mv, settings, "uint8_t", "seq_force_integer_mv", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.seq_force_screen_content_tools, settings, "uint8_t", "seq_force_screen_content_tools", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved1, 5, settings, "uint8_t[5]", "reserved1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pColorConfig, settings, "const StdVideoAV1ColorConfig*", "pColorConfig", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1ColorConfig<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pTimingInfo, settings, "const StdVideoAV1TimingInfo*", "pTimingInfo", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1TimingInfo<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeAV1PictureInfoFlags(const StdVideoDecodeAV1PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.error_resilient_mode, settings, "uint32_t: 1", "error_resilient_mode", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.disable_cdf_update, settings, "uint32_t: 1", "disable_cdf_update", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.use_superres, settings, "uint32_t: 1", "use_superres", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.render_and_frame_size_different, settings, "uint32_t: 1", "render_and_frame_size_different", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.allow_screen_content_tools, settings, "uint32_t: 1", "allow_screen_content_tools", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.is_filter_switchable, settings, "uint32_t: 1", "is_filter_switchable", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.force_integer_mv, settings, "uint32_t: 1", "force_integer_mv", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_size_override_flag, settings, "uint32_t: 1", "frame_size_override_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.buffer_removal_time_present_flag, settings, "uint32_t: 1", "buffer_removal_time_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.allow_intrabc, settings, "uint32_t: 1", "allow_intrabc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_refs_short_signaling, settings, "uint32_t: 1", "frame_refs_short_signaling", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.allow_high_precision_mv, settings, "uint32_t: 1", "allow_high_precision_mv", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.is_motion_mode_switchable, settings, "uint32_t: 1", "is_motion_mode_switchable", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.use_ref_frame_mvs, settings, "uint32_t: 1", "use_ref_frame_mvs", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.disable_frame_end_update_cdf, settings, "uint32_t: 1", "disable_frame_end_update_cdf", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.allow_warped_motion, settings, "uint32_t: 1", "allow_warped_motion", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reduced_tx_set, settings, "uint32_t: 1", "reduced_tx_set", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reference_select, settings, "uint32_t: 1", "reference_select", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.skip_mode_present, settings, "uint32_t: 1", "skip_mode_present", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_q_present, settings, "uint32_t: 1", "delta_q_present", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_lf_present, settings, "uint32_t: 1", "delta_lf_present", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_lf_multi, settings, "uint32_t: 1", "delta_lf_multi", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_update_map, settings, "uint32_t: 1", "segmentation_update_map", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_temporal_update, settings, "uint32_t: 1", "segmentation_temporal_update", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_update_data, settings, "uint32_t: 1", "segmentation_update_data", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.UsesLr, settings, "uint32_t: 1", "UsesLr", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.usesChromaLr, settings, "uint32_t: 1", "usesChromaLr", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.apply_grain, settings, "uint32_t: 1", "apply_grain", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 3", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeAV1PictureInfo(const StdVideoDecodeAV1PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoDecodeAV1PictureInfoFlags<Format>(object.flags, settings, "StdVideoDecodeAV1PictureInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1FrameType<Format>(object.frame_type, settings, "StdVideoAV1FrameType", "frame_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.current_frame_id, settings, "uint32_t", "current_frame_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.OrderHint, settings, "uint8_t", "OrderHint", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.primary_ref_frame, settings, "uint8_t", "primary_ref_frame", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.refresh_frame_flags, settings, "uint8_t", "refresh_frame_flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1InterpolationFilter<Format>(object.interpolation_filter, settings, "StdVideoAV1InterpolationFilter", "interpolation_filter", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1TxMode<Format>(object.TxMode, settings, "StdVideoAV1TxMode", "TxMode", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_q_res, settings, "uint8_t", "delta_q_res", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_lf_res, settings, "uint8_t", "delta_lf_res", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.SkipModeFrame, STD_VIDEO_AV1_SKIP_MODE_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_SKIP_MODE_FRAMES]", "SkipModeFrame", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.coded_denom, settings, "uint8_t", "coded_denom", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved2, 3, settings, "uint8_t[3]", "reserved2", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.OrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "OrderHints", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.expectedFrameId, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint32_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "expectedFrameId", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pTileInfo, settings, "const StdVideoAV1TileInfo*", "pTileInfo", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1TileInfo<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pQuantization, settings, "const StdVideoAV1Quantization*", "pQuantization", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1Quantization<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pSegmentation, settings, "const StdVideoAV1Segmentation*", "pSegmentation", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1Segmentation<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pLoopFilter, settings, "const StdVideoAV1LoopFilter*", "pLoopFilter", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1LoopFilter<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pCDEF, settings, "const StdVideoAV1CDEF*", "pCDEF", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1CDEF<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pLoopRestoration, settings, "const StdVideoAV1LoopRestoration*", "pLoopRestoration", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1LoopRestoration<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pGlobalMotion, settings, "const StdVideoAV1GlobalMotion*", "pGlobalMotion", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1GlobalMotion<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pFilmGrain, settings, "const StdVideoAV1FilmGrain*", "pFilmGrain", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1FilmGrain<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeAV1ReferenceInfoFlags(const StdVideoDecodeAV1ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.disable_frame_end_update_cdf, settings, "uint32_t: 1", "disable_frame_end_update_cdf", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 30", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeAV1ReferenceInfo(const StdVideoDecodeAV1ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoDecodeAV1ReferenceInfoFlags<Format>(object.flags, settings, "StdVideoDecodeAV1ReferenceInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_type, settings, "uint8_t", "frame_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.RefFrameSignBias, settings, "uint8_t", "RefFrameSignBias", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.OrderHint, settings, "uint8_t", "OrderHint", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.SavedOrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "SavedOrderHints", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeAV1DecoderModelInfo(const StdVideoEncodeAV1DecoderModelInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.buffer_delay_length_minus_1, settings, "uint8_t", "buffer_delay_length_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.buffer_removal_time_length_minus_1, settings, "uint8_t", "buffer_removal_time_length_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_presentation_time_length_minus_1, settings, "uint8_t", "frame_presentation_time_length_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.num_units_in_decoding_tick, settings, "uint32_t", "num_units_in_decoding_tick", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeAV1ExtensionHeader(const StdVideoEncodeAV1ExtensionHeader& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.temporal_id, settings, "uint8_t", "temporal_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.spatial_id, settings, "uint8_t", "spatial_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeAV1OperatingPointInfoFlags(const StdVideoEncodeAV1OperatingPointInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.decoder_model_present_for_this_op, settings, "uint32_t: 1", "decoder_model_present_for_this_op", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.low_delay_mode_flag, settings, "uint32_t: 1", "low_delay_mode_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.initial_display_delay_present_for_this_op, settings, "uint32_t: 1", "initial_display_delay_present_for_this_op", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 29", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeAV1OperatingPointInfo(const StdVideoEncodeAV1OperatingPointInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeAV1OperatingPointInfoFlags<Format>(object.flags, settings, "StdVideoEncodeAV1OperatingPointInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.operating_point_idc, settings, "uint16_t", "operating_point_idc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.seq_level_idx, settings, "uint8_t", "seq_level_idx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.seq_tier, settings, "uint8_t", "seq_tier", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.decoder_buffer_delay, settings, "uint32_t", "decoder_buffer_delay", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.encoder_buffer_delay, settings, "uint32_t", "encoder_buffer_delay", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.initial_display_delay_minus_1, settings, "uint8_t", "initial_display_delay_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeAV1PictureInfoFlags(const StdVideoEncodeAV1PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.error_resilient_mode, settings, "uint32_t: 1", "error_resilient_mode", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.disable_cdf_update, settings, "uint32_t: 1", "disable_cdf_update", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.use_superres, settings, "uint32_t: 1", "use_superres", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.render_and_frame_size_different, settings, "uint32_t: 1", "render_and_frame_size_different", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.allow_screen_content_tools, settings, "uint32_t: 1", "allow_screen_content_tools", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.is_filter_switchable, settings, "uint32_t: 1", "is_filter_switchable", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.force_integer_mv, settings, "uint32_t: 1", "force_integer_mv", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_size_override_flag, settings, "uint32_t: 1", "frame_size_override_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.buffer_removal_time_present_flag, settings, "uint32_t: 1", "buffer_removal_time_present_flag", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.allow_intrabc, settings, "uint32_t: 1", "allow_intrabc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_refs_short_signaling, settings, "uint32_t: 1", "frame_refs_short_signaling", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.allow_high_precision_mv, settings, "uint32_t: 1", "allow_high_precision_mv", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.is_motion_mode_switchable, settings, "uint32_t: 1", "is_motion_mode_switchable", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.use_ref_frame_mvs, settings, "uint32_t: 1", "use_ref_frame_mvs", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.disable_frame_end_update_cdf, settings, "uint32_t: 1", "disable_frame_end_update_cdf", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.allow_warped_motion, settings, "uint32_t: 1", "allow_warped_motion", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reduced_tx_set, settings, "uint32_t: 1", "reduced_tx_set", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.skip_mode_present, settings, "uint32_t: 1", "skip_mode_present", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_q_present, settings, "uint32_t: 1", "delta_q_present", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_lf_present, settings, "uint32_t: 1", "delta_lf_present", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_lf_multi, settings, "uint32_t: 1", "delta_lf_multi", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_update_map, settings, "uint32_t: 1", "segmentation_update_map", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_temporal_update, settings, "uint32_t: 1", "segmentation_temporal_update", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_update_data, settings, "uint32_t: 1", "segmentation_update_data", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.UsesLr, settings, "uint32_t: 1", "UsesLr", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.usesChromaLr, settings, "uint32_t: 1", "usesChromaLr", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.show_frame, settings, "uint32_t: 1", "show_frame", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.showable_frame, settings, "uint32_t: 1", "showable_frame", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 3", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeAV1PictureInfo(const StdVideoEncodeAV1PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeAV1PictureInfoFlags<Format>(object.flags, settings, "StdVideoEncodeAV1PictureInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1FrameType<Format>(object.frame_type, settings, "StdVideoAV1FrameType", "frame_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_presentation_time, settings, "uint32_t", "frame_presentation_time", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.current_frame_id, settings, "uint32_t", "current_frame_id", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.order_hint, settings, "uint8_t", "order_hint", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.primary_ref_frame, settings, "uint8_t", "primary_ref_frame", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.refresh_frame_flags, settings, "uint8_t", "refresh_frame_flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.coded_denom, settings, "uint8_t", "coded_denom", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.render_width_minus_1, settings, "uint16_t", "render_width_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint16_t>(object.render_height_minus_1, settings, "uint16_t", "render_height_minus_1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1InterpolationFilter<Format>(object.interpolation_filter, settings, "StdVideoAV1InterpolationFilter", "interpolation_filter", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1TxMode<Format>(object.TxMode, settings, "StdVideoAV1TxMode", "TxMode", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_q_res, settings, "uint8_t", "delta_q_res", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.delta_lf_res, settings, "uint8_t", "delta_lf_res", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.ref_order_hint, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "ref_order_hint", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.ref_frame_idx, STD_VIDEO_AV1_REFS_PER_FRAME, settings, "int8_t[STD_VIDEO_AV1_REFS_PER_FRAME]", "ref_frame_idx", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved1, 3, settings, "uint8_t[3]", "reserved1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.delta_frame_id_minus_1, STD_VIDEO_AV1_REFS_PER_FRAME, settings, "uint32_t[STD_VIDEO_AV1_REFS_PER_FRAME]", "delta_frame_id_minus_1", "uint32_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pTileInfo, settings, "const StdVideoAV1TileInfo*", "pTileInfo", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1TileInfo<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pQuantization, settings, "const StdVideoAV1Quantization*", "pQuantization", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1Quantization<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pSegmentation, settings, "const StdVideoAV1Segmentation*", "pSegmentation", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1Segmentation<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pLoopFilter, settings, "const StdVideoAV1LoopFilter*", "pLoopFilter", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1LoopFilter<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pCDEF, settings, "const StdVideoAV1CDEF*", "pCDEF", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1CDEF<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pLoopRestoration, settings, "const StdVideoAV1LoopRestoration*", "pLoopRestoration", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1LoopRestoration<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pGlobalMotion, settings, "const StdVideoAV1GlobalMotion*", "pGlobalMotion", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoAV1GlobalMotion<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pExtensionHeader, settings, "const StdVideoEncodeAV1ExtensionHeader*", "pExtensionHeader", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeAV1ExtensionHeader<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pBufferRemovalTimes, settings, "const uint32_t*", "pBufferRemovalTimes", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, const uint32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeAV1ReferenceInfoFlags(const StdVideoEncodeAV1ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.disable_frame_end_update_cdf, settings, "uint32_t: 1", "disable_frame_end_update_cdf", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 30", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoEncodeAV1ReferenceInfo(const StdVideoEncodeAV1ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoEncodeAV1ReferenceInfoFlags<Format>(object.flags, settings, "StdVideoEncodeAV1ReferenceInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.RefFrameId, settings, "uint32_t", "RefFrameId", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoAV1FrameType<Format>(object.frame_type, settings, "StdVideoAV1FrameType", "frame_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.OrderHint, settings, "uint8_t", "OrderHint", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved1, 3, settings, "uint8_t[3]", "reserved1", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pExtensionHeader, settings, "const StdVideoEncodeAV1ExtensionHeader*", "pExtensionHeader", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoEncodeAV1ExtensionHeader<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9ColorConfigFlags(const StdVideoVP9ColorConfigFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.color_range, settings, "uint32_t: 1", "color_range", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 31", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9ColorConfig(const StdVideoVP9ColorConfig& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoVP9ColorConfigFlags<Format>(object.flags, settings, "StdVideoVP9ColorConfigFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.BitDepth, settings, "uint8_t", "BitDepth", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.subsampling_x, settings, "uint8_t", "subsampling_x", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.subsampling_y, settings, "uint8_t", "subsampling_y", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoVP9ColorSpace<Format>(object.color_space, settings, "StdVideoVP9ColorSpace", "color_space", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9LoopFilterFlags(const StdVideoVP9LoopFilterFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.loop_filter_delta_enabled, settings, "uint32_t: 1", "loop_filter_delta_enabled", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.loop_filter_delta_update, settings, "uint32_t: 1", "loop_filter_delta_update", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 30", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9LoopFilter(const StdVideoVP9LoopFilter& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoVP9LoopFilterFlags<Format>(object.flags, settings, "StdVideoVP9LoopFilterFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.loop_filter_level, settings, "uint8_t", "loop_filter_level", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.loop_filter_sharpness, settings, "uint8_t", "loop_filter_sharpness", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.update_ref_delta, settings, "uint8_t", "update_ref_delta", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.loop_filter_ref_deltas, STD_VIDEO_VP9_MAX_REF_FRAMES, settings, "int8_t[STD_VIDEO_VP9_MAX_REF_FRAMES]", "loop_filter_ref_deltas", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.update_mode_delta, settings, "uint8_t", "update_mode_delta", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.loop_filter_mode_deltas, STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS, settings, "int8_t[STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS]", "loop_filter_mode_deltas", "int8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int32_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9SegmentationFlags(const StdVideoVP9SegmentationFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.segmentation_update_map, settings, "uint32_t: 1", "segmentation_update_map", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_temporal_update, settings, "uint32_t: 1", "segmentation_temporal_update", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_update_data, settings, "uint32_t: 1", "segmentation_update_data", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_abs_or_delta_update, settings, "uint32_t: 1", "segmentation_abs_or_delta_update", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 28", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoVP9Segmentation(const StdVideoVP9Segmentation& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoVP9SegmentationFlags<Format>(object.flags, settings, "StdVideoVP9SegmentationFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.segmentation_tree_probs, STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS]", "segmentation_tree_probs", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.segmentation_pred_prob, STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB]", "segmentation_pred_prob", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.FeatureEnabled, STD_VIDEO_VP9_MAX_SEGMENTS, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTS]", "FeatureEnabled", "uint8_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint32_t>);
    dump_separate_members<Format>(settings);
    dump_double_array<Format>(object.FeatureData, STD_VIDEO_VP9_MAX_SEGMENTS, STD_VIDEO_VP9_SEG_LVL_MAX, settings, "int16_t[STD_VIDEO_VP9_MAX_SEGMENTS][STD_VIDEO_VP9_SEG_LVL_MAX]", "FeatureData", "int16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, int16_t>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeVP9PictureInfoFlags(const StdVideoDecodeVP9PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_type<Format, uint32_t>(object.error_resilient_mode, settings, "uint32_t: 1", "error_resilient_mode", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.intra_only, settings, "uint32_t: 1", "intra_only", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.allow_high_precision_mv, settings, "uint32_t: 1", "allow_high_precision_mv", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.refresh_frame_context, settings, "uint32_t: 1", "refresh_frame_context", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_parallel_decoding_mode, settings, "uint32_t: 1", "frame_parallel_decoding_mode", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.show_frame, settings, "uint32_t: 1", "show_frame", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.UsePrevFrameMvs, settings, "uint32_t: 1", "UsePrevFrameMvs", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reserved, settings, "uint32_t: 24", "reserved", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}
template <ApiDumpFormat Format>
void dump_StdVideoDecodeVP9PictureInfo(const StdVideoDecodeVP9PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_start<Format>(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_StdVideoDecodeVP9PictureInfoFlags<Format>(object.flags, settings, "StdVideoDecodeVP9PictureInfoFlags", "flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoVP9Profile<Format>(object.profile, settings, "StdVideoVP9Profile", "profile", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoVP9FrameType<Format>(object.frame_type, settings, "StdVideoVP9FrameType", "frame_type", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.frame_context_idx, settings, "uint8_t", "frame_context_idx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.reset_frame_context, settings, "uint8_t", "reset_frame_context", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.refresh_frame_flags, settings, "uint8_t", "refresh_frame_flags", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.ref_frame_sign_bias_mask, settings, "uint8_t", "ref_frame_sign_bias_mask", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_StdVideoVP9InterpolationFilter<Format>(object.interpolation_filter, settings, "StdVideoVP9InterpolationFilter", "interpolation_filter", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.base_q_idx, settings, "uint8_t", "base_q_idx", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.delta_q_y_dc, settings, "int8_t", "delta_q_y_dc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.delta_q_uv_dc, settings, "int8_t", "delta_q_uv_dc", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, int32_t>(object.delta_q_uv_ac, settings, "int8_t", "delta_q_uv_ac", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.tile_cols_log2, settings, "uint8_t", "tile_cols_log2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_type<Format, uint32_t>(object.tile_rows_log2, settings, "uint8_t", "tile_rows_log2", indents + (Format == ApiDumpFormat::Json ? 2 : 1));
    dump_separate_members<Format>(settings);
    dump_single_array<Format>(object.reserved1, 3, settings, "uint16_t[3]", "reserved1", "uint16_t", indents + (Format == ApiDumpFormat::Json ? 2 : 1) + (Format == ApiDumpFormat::Json ? 2 : 1), dump_type<Format, uint16_t>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pColorConfig, settings, "const StdVideoVP9ColorConfig*", "pColorConfig", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoVP9ColorConfig<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pLoopFilter, settings, "const StdVideoVP9LoopFilter*", "pLoopFilter", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoVP9LoopFilter<Format>);
    dump_separate_members<Format>(settings);
    dump_pointer<Format>(object.pSegmentation, settings, "const StdVideoVP9Segmentation*", "pSegmentation", indents + (Format == ApiDumpFormat::Json ? 2 : 1), dump_StdVideoVP9Segmentation<Format>);
    dump_end<Format>(settings, OutputConstruct::api_struct, indents);
}

//======================== pNext Chain Implementation =======================//

//========================== Function Helpers ===============================//

//========================= Function Implementations ========================//
