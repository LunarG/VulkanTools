
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

#include "api_dump_html.h"

//=========================== Type Implementations ==========================//

void dump_html_uint32_t(const uint32_t& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    dump_html_value(settings, object);
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_uint32_t(const uint32_t* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_uint32_t(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_uint8_t(const uint8_t& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    dump_html_value(settings, (uint32_t)object);
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_uint8_t(const uint8_t* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_uint8_t(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_uint16_t(const uint16_t& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    dump_html_value(settings, object);
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_uint16_t(const uint16_t* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_uint16_t(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_int32_t(const int32_t& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    dump_html_value(settings, object);
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_int32_t(const int32_t* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_int32_t(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_int8_t(const int8_t& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    dump_html_value(settings, (int32_t)object);
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_int8_t(const int8_t* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_int8_t(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_int16_t(const int16_t& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    dump_html_value(settings, object);
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_int16_t(const int16_t* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_int16_t(*object, settings, type_string, name, indents, object);
    }
}

//========================= Basetype Implementations ========================//

//======================= System Type Implementations =======================//

//=========================== Enum Implementations ==========================//

void dump_html_StdVideoH264ChromaFormatIdc(const StdVideoH264ChromaFormatIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_MONOCHROME (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_420 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_422 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_444 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_CHROMA_FORMAT_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264ChromaFormatIdc(const StdVideoH264ChromaFormatIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264ChromaFormatIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264ProfileIdc(const StdVideoH264ProfileIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 66:
            dump_html_value(settings, "STD_VIDEO_H264_PROFILE_IDC_BASELINE (", object, ")");
            break;
        case 77:
            dump_html_value(settings, "STD_VIDEO_H264_PROFILE_IDC_MAIN (", object, ")");
            break;
        case 100:
            dump_html_value(settings, "STD_VIDEO_H264_PROFILE_IDC_HIGH (", object, ")");
            break;
        case 244:
            dump_html_value(settings, "STD_VIDEO_H264_PROFILE_IDC_HIGH_444_PREDICTIVE (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_PROFILE_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264ProfileIdc(const StdVideoH264ProfileIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264ProfileIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264LevelIdc(const StdVideoH264LevelIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_1_0 (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_1_1 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_1_2 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_1_3 (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_2_0 (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_2_1 (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_2_2 (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_3_0 (", object, ")");
            break;
        case 8:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_3_1 (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_3_2 (", object, ")");
            break;
        case 10:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_4_0 (", object, ")");
            break;
        case 11:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_4_1 (", object, ")");
            break;
        case 12:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_4_2 (", object, ")");
            break;
        case 13:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_5_0 (", object, ")");
            break;
        case 14:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_5_1 (", object, ")");
            break;
        case 15:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_5_2 (", object, ")");
            break;
        case 16:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_6_0 (", object, ")");
            break;
        case 17:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_6_1 (", object, ")");
            break;
        case 18:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_6_2 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_LEVEL_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264LevelIdc(const StdVideoH264LevelIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264LevelIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264PocType(const StdVideoH264PocType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_POC_TYPE_0 (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_POC_TYPE_1 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_POC_TYPE_2 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_POC_TYPE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264PocType(const StdVideoH264PocType* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264PocType(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264AspectRatioIdc(const StdVideoH264AspectRatioIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_UNSPECIFIED (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_SQUARE (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_12_11 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_10_11 (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_16_11 (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_40_33 (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_24_11 (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_20_11 (", object, ")");
            break;
        case 8:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_32_11 (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_80_33 (", object, ")");
            break;
        case 10:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_18_11 (", object, ")");
            break;
        case 11:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_15_11 (", object, ")");
            break;
        case 12:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_64_33 (", object, ")");
            break;
        case 13:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_160_99 (", object, ")");
            break;
        case 14:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_4_3 (", object, ")");
            break;
        case 15:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_3_2 (", object, ")");
            break;
        case 16:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_2_1 (", object, ")");
            break;
        case 255:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_EXTENDED_SAR (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_ASPECT_RATIO_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264AspectRatioIdc(const StdVideoH264AspectRatioIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264AspectRatioIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264WeightedBipredIdc(const StdVideoH264WeightedBipredIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_DEFAULT (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_EXPLICIT (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_IMPLICIT (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264WeightedBipredIdc(const StdVideoH264WeightedBipredIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264WeightedBipredIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264ModificationOfPicNumsIdc(const StdVideoH264ModificationOfPicNumsIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_SUBTRACT (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_ADD (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_LONG_TERM (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_END (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264ModificationOfPicNumsIdc(const StdVideoH264ModificationOfPicNumsIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264ModificationOfPicNumsIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264MemMgmtControlOp(const StdVideoH264MemMgmtControlOp object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_END (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_SHORT_TERM (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_LONG_TERM (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_LONG_TERM (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_SET_MAX_LONG_TERM_INDEX (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_ALL (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_CURRENT_AS_LONG_TERM (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264MemMgmtControlOp(const StdVideoH264MemMgmtControlOp* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264MemMgmtControlOp(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264CabacInitIdc(const StdVideoH264CabacInitIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_CABAC_INIT_IDC_0 (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_CABAC_INIT_IDC_1 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_CABAC_INIT_IDC_2 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_CABAC_INIT_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264CabacInitIdc(const StdVideoH264CabacInitIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264CabacInitIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264DisableDeblockingFilterIdc(const StdVideoH264DisableDeblockingFilterIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_DISABLED (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_ENABLED (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_PARTIAL (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264DisableDeblockingFilterIdc(const StdVideoH264DisableDeblockingFilterIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264DisableDeblockingFilterIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264SliceType(const StdVideoH264SliceType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_SLICE_TYPE_P (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_SLICE_TYPE_B (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_SLICE_TYPE_I (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_SLICE_TYPE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264SliceType(const StdVideoH264SliceType* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264SliceType(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264PictureType(const StdVideoH264PictureType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_PICTURE_TYPE_P (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_PICTURE_TYPE_B (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_PICTURE_TYPE_I (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_H264_PICTURE_TYPE_IDR (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_PICTURE_TYPE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264PictureType(const StdVideoH264PictureType* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264PictureType(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264NonVclNaluType(const StdVideoH264NonVclNaluType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_SPS (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_PPS (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_AUD (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_PREFIX (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_SEQUENCE (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_STREAM (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_PRECODED (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H264_NON_VCL_NALU_TYPE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH264NonVclNaluType(const StdVideoH264NonVclNaluType* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264NonVclNaluType(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeH264FieldOrderCount(const StdVideoDecodeH264FieldOrderCount object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_TOP (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_BOTTOM (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoDecodeH264FieldOrderCount(const StdVideoDecodeH264FieldOrderCount* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeH264FieldOrderCount(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265ChromaFormatIdc(const StdVideoH265ChromaFormatIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_MONOCHROME (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_420 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_422 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_444 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H265_CHROMA_FORMAT_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH265ChromaFormatIdc(const StdVideoH265ChromaFormatIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265ChromaFormatIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265ProfileIdc(const StdVideoH265ProfileIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 1:
            dump_html_value(settings, "STD_VIDEO_H265_PROFILE_IDC_MAIN (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H265_PROFILE_IDC_MAIN_10 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H265_PROFILE_IDC_MAIN_STILL_PICTURE (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_H265_PROFILE_IDC_FORMAT_RANGE_EXTENSIONS (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_H265_PROFILE_IDC_SCC_EXTENSIONS (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H265_PROFILE_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH265ProfileIdc(const StdVideoH265ProfileIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265ProfileIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265LevelIdc(const StdVideoH265LevelIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_1_0 (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_2_0 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_2_1 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_3_0 (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_3_1 (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_4_0 (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_4_1 (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_5_0 (", object, ")");
            break;
        case 8:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_5_1 (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_5_2 (", object, ")");
            break;
        case 10:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_6_0 (", object, ")");
            break;
        case 11:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_6_1 (", object, ")");
            break;
        case 12:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_6_2 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H265_LEVEL_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH265LevelIdc(const StdVideoH265LevelIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265LevelIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265SliceType(const StdVideoH265SliceType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H265_SLICE_TYPE_B (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H265_SLICE_TYPE_P (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H265_SLICE_TYPE_I (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H265_SLICE_TYPE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH265SliceType(const StdVideoH265SliceType* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265SliceType(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265PictureType(const StdVideoH265PictureType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H265_PICTURE_TYPE_P (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H265_PICTURE_TYPE_B (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H265_PICTURE_TYPE_I (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H265_PICTURE_TYPE_IDR (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H265_PICTURE_TYPE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH265PictureType(const StdVideoH265PictureType* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265PictureType(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265AspectRatioIdc(const StdVideoH265AspectRatioIdc object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_UNSPECIFIED (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_SQUARE (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_12_11 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_10_11 (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_16_11 (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_40_33 (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_24_11 (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_20_11 (", object, ")");
            break;
        case 8:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_32_11 (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_80_33 (", object, ")");
            break;
        case 10:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_18_11 (", object, ")");
            break;
        case 11:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_15_11 (", object, ")");
            break;
        case 12:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_64_33 (", object, ")");
            break;
        case 13:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_160_99 (", object, ")");
            break;
        case 14:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_4_3 (", object, ")");
            break;
        case 15:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_3_2 (", object, ")");
            break;
        case 16:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_2_1 (", object, ")");
            break;
        case 255:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_EXTENDED_SAR (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_H265_ASPECT_RATIO_IDC_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoH265AspectRatioIdc(const StdVideoH265AspectRatioIdc* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265AspectRatioIdc(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1Profile(const StdVideoAV1Profile object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_PROFILE_MAIN (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_PROFILE_HIGH (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_PROFILE_PROFESSIONAL (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_PROFILE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1Profile(const StdVideoAV1Profile* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1Profile(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1Level(const StdVideoAV1Level object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_2_0 (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_2_1 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_2_2 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_2_3 (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_3_0 (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_3_1 (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_3_2 (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_3_3 (", object, ")");
            break;
        case 8:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_4_0 (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_4_1 (", object, ")");
            break;
        case 10:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_4_2 (", object, ")");
            break;
        case 11:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_4_3 (", object, ")");
            break;
        case 12:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_5_0 (", object, ")");
            break;
        case 13:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_5_1 (", object, ")");
            break;
        case 14:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_5_2 (", object, ")");
            break;
        case 15:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_5_3 (", object, ")");
            break;
        case 16:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_6_0 (", object, ")");
            break;
        case 17:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_6_1 (", object, ")");
            break;
        case 18:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_6_2 (", object, ")");
            break;
        case 19:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_6_3 (", object, ")");
            break;
        case 20:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_7_0 (", object, ")");
            break;
        case 21:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_7_1 (", object, ")");
            break;
        case 22:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_7_2 (", object, ")");
            break;
        case 23:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_7_3 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_LEVEL_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1Level(const StdVideoAV1Level* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1Level(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1FrameType(const StdVideoAV1FrameType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_TYPE_KEY (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_TYPE_INTER (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_TYPE_INTRA_ONLY (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_TYPE_SWITCH (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_TYPE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1FrameType(const StdVideoAV1FrameType* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1FrameType(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1ReferenceName(const StdVideoAV1ReferenceName object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_REFERENCE_NAME_INTRA_FRAME (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_REFERENCE_NAME_LAST_FRAME (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_REFERENCE_NAME_LAST2_FRAME (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_AV1_REFERENCE_NAME_LAST3_FRAME (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_AV1_REFERENCE_NAME_GOLDEN_FRAME (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_AV1_REFERENCE_NAME_BWDREF_FRAME (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_AV1_REFERENCE_NAME_ALTREF2_FRAME (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_AV1_REFERENCE_NAME_ALTREF_FRAME (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_REFERENCE_NAME_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1ReferenceName(const StdVideoAV1ReferenceName* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1ReferenceName(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1InterpolationFilter(const StdVideoAV1InterpolationFilter object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SHARP (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_BILINEAR (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_SWITCHABLE (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_INTERPOLATION_FILTER_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1InterpolationFilter(const StdVideoAV1InterpolationFilter* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1InterpolationFilter(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1TxMode(const StdVideoAV1TxMode object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_TX_MODE_ONLY_4X4 (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_TX_MODE_LARGEST (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_TX_MODE_SELECT (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_TX_MODE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1TxMode(const StdVideoAV1TxMode* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1TxMode(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1FrameRestorationType(const StdVideoAV1FrameRestorationType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_NONE (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_WIENER (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SGRPROJ (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SWITCHABLE (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1FrameRestorationType(const StdVideoAV1FrameRestorationType* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1FrameRestorationType(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1ColorPrimaries(const StdVideoAV1ColorPrimaries object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_709 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_UNSPECIFIED (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_M (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_B_G (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_601 (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_240 (", object, ")");
            break;
        case 8:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_GENERIC_FILM (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_2020 (", object, ")");
            break;
        case 10:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_XYZ (", object, ")");
            break;
        case 11:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_431 (", object, ")");
            break;
        case 12:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_432 (", object, ")");
            break;
        case 22:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_EBU_3213 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_COLOR_PRIMARIES_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1ColorPrimaries(const StdVideoAV1ColorPrimaries* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1ColorPrimaries(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1TransferCharacteristics(const StdVideoAV1TransferCharacteristics object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_0 (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_709 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_UNSPECIFIED (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_3 (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_M (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_B_G (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_601 (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_240 (", object, ")");
            break;
        case 8:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LINEAR (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100 (", object, ")");
            break;
        case 10:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100_SQRT10 (", object, ")");
            break;
        case 11:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_IEC_61966 (", object, ")");
            break;
        case 12:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_1361 (", object, ")");
            break;
        case 13:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SRGB (", object, ")");
            break;
        case 14:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_10_BIT (", object, ")");
            break;
        case 15:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_12_BIT (", object, ")");
            break;
        case 16:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_2084 (", object, ")");
            break;
        case 17:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_428 (", object, ")");
            break;
        case 18:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_HLG (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1TransferCharacteristics(const StdVideoAV1TransferCharacteristics* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1TransferCharacteristics(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1MatrixCoefficients(const StdVideoAV1MatrixCoefficients object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_IDENTITY (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_709 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_UNSPECIFIED (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_RESERVED_3 (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_FCC (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_470_B_G (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_601 (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_240 (", object, ")");
            break;
        case 8:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_YCGCO (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_NCL (", object, ")");
            break;
        case 10:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_CL (", object, ")");
            break;
        case 11:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_2085 (", object, ")");
            break;
        case 12:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_NCL (", object, ")");
            break;
        case 13:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_CL (", object, ")");
            break;
        case 14:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_ICTCP (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1MatrixCoefficients(const StdVideoAV1MatrixCoefficients* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1MatrixCoefficients(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1ChromaSamplePosition(const StdVideoAV1ChromaSamplePosition object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_UNKNOWN (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_VERTICAL (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_COLOCATED (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_RESERVED (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoAV1ChromaSamplePosition(const StdVideoAV1ChromaSamplePosition* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1ChromaSamplePosition(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9Profile(const StdVideoVP9Profile object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_VP9_PROFILE_0 (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_VP9_PROFILE_1 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_VP9_PROFILE_2 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_VP9_PROFILE_3 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_VP9_PROFILE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoVP9Profile(const StdVideoVP9Profile* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9Profile(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9Level(const StdVideoVP9Level object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_1_0 (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_1_1 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_2_0 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_2_1 (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_3_0 (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_3_1 (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_4_0 (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_4_1 (", object, ")");
            break;
        case 8:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_5_0 (", object, ")");
            break;
        case 9:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_5_1 (", object, ")");
            break;
        case 10:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_5_2 (", object, ")");
            break;
        case 11:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_6_0 (", object, ")");
            break;
        case 12:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_6_1 (", object, ")");
            break;
        case 13:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_6_2 (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_VP9_LEVEL_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoVP9Level(const StdVideoVP9Level* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9Level(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9FrameType(const StdVideoVP9FrameType object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_VP9_FRAME_TYPE_KEY (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_VP9_FRAME_TYPE_NON_KEY (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_VP9_FRAME_TYPE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoVP9FrameType(const StdVideoVP9FrameType* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9FrameType(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9ReferenceName(const StdVideoVP9ReferenceName object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_VP9_REFERENCE_NAME_INTRA_FRAME (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_VP9_REFERENCE_NAME_LAST_FRAME (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_VP9_REFERENCE_NAME_GOLDEN_FRAME (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_VP9_REFERENCE_NAME_ALTREF_FRAME (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_VP9_REFERENCE_NAME_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoVP9ReferenceName(const StdVideoVP9ReferenceName* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9ReferenceName(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9InterpolationFilter(const StdVideoVP9InterpolationFilter object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SHARP (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_BILINEAR (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_SWITCHABLE (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_VP9_INTERPOLATION_FILTER_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoVP9InterpolationFilter(const StdVideoVP9InterpolationFilter* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9InterpolationFilter(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9ColorSpace(const StdVideoVP9ColorSpace object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::value, type_name, var_name, indents, address);
    switch ((int64_t)object) {
        case 0:
            dump_html_value(settings, "STD_VIDEO_VP9_COLOR_SPACE_UNKNOWN (", object, ")");
            break;
        case 1:
            dump_html_value(settings, "STD_VIDEO_VP9_COLOR_SPACE_BT_601 (", object, ")");
            break;
        case 2:
            dump_html_value(settings, "STD_VIDEO_VP9_COLOR_SPACE_BT_709 (", object, ")");
            break;
        case 3:
            dump_html_value(settings, "STD_VIDEO_VP9_COLOR_SPACE_SMPTE_170 (", object, ")");
            break;
        case 4:
            dump_html_value(settings, "STD_VIDEO_VP9_COLOR_SPACE_SMPTE_240 (", object, ")");
            break;
        case 5:
            dump_html_value(settings, "STD_VIDEO_VP9_COLOR_SPACE_BT_2020 (", object, ")");
            break;
        case 6:
            dump_html_value(settings, "STD_VIDEO_VP9_COLOR_SPACE_RESERVED (", object, ")");
            break;
        case 7:
            dump_html_value(settings, "STD_VIDEO_VP9_COLOR_SPACE_RGB (", object, ")");
            break;
        case 0x7FFFFFFF:
            dump_html_value(settings, "STD_VIDEO_VP9_COLOR_SPACE_INVALID (", object, ")");
            break;
        default:
            dump_html_value(settings, "UNKNOWN (", object, ")");
    }
    dump_html_end(settings, OutputConstruct::value, indents);
}

void dump_html_StdVideoVP9ColorSpace(const StdVideoVP9ColorSpace* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9ColorSpace(*object, settings, type_string, name, indents, object);
    }
}

//========================= Bitmask Implementations =========================//

//=========================== Flag Implementations ==========================//

//======================= Func Pointer Implementations ======================//

//======================== Union Forward Declarations =======================//

//======================== pNext Chain Declarations =======================//

//========================== Struct Implementations =========================//

void dump_html_StdVideoH264SpsVuiFlags(const StdVideoH264SpsVuiFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.aspect_ratio_info_present_flag, settings, "uint32_t: 1", "aspect_ratio_info_present_flag", indents + 1);
    dump_html_uint32_t(object.overscan_info_present_flag, settings, "uint32_t: 1", "overscan_info_present_flag", indents + 1);
    dump_html_uint32_t(object.overscan_appropriate_flag, settings, "uint32_t: 1", "overscan_appropriate_flag", indents + 1);
    dump_html_uint32_t(object.video_signal_type_present_flag, settings, "uint32_t: 1", "video_signal_type_present_flag", indents + 1);
    dump_html_uint32_t(object.video_full_range_flag, settings, "uint32_t: 1", "video_full_range_flag", indents + 1);
    dump_html_uint32_t(object.color_description_present_flag, settings, "uint32_t: 1", "color_description_present_flag", indents + 1);
    dump_html_uint32_t(object.chroma_loc_info_present_flag, settings, "uint32_t: 1", "chroma_loc_info_present_flag", indents + 1);
    dump_html_uint32_t(object.timing_info_present_flag, settings, "uint32_t: 1", "timing_info_present_flag", indents + 1);
    dump_html_uint32_t(object.fixed_frame_rate_flag, settings, "uint32_t: 1", "fixed_frame_rate_flag", indents + 1);
    dump_html_uint32_t(object.bitstream_restriction_flag, settings, "uint32_t: 1", "bitstream_restriction_flag", indents + 1);
    dump_html_uint32_t(object.nal_hrd_parameters_present_flag, settings, "uint32_t: 1", "nal_hrd_parameters_present_flag", indents + 1);
    dump_html_uint32_t(object.vcl_hrd_parameters_present_flag, settings, "uint32_t: 1", "vcl_hrd_parameters_present_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH264SpsVuiFlags(const StdVideoH264SpsVuiFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264SpsVuiFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264HrdParameters(const StdVideoH264HrdParameters& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint8_t(object.cpb_cnt_minus1, settings, "uint8_t", "cpb_cnt_minus1", indents + 1);
    dump_html_uint8_t(object.bit_rate_scale, settings, "uint8_t", "bit_rate_scale", indents + 1);
    dump_html_uint8_t(object.cpb_size_scale, settings, "uint8_t", "cpb_size_scale", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_array_start(object.bit_rate_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "bit_rate_value_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_CPB_CNT_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "bit_rate_value_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.bit_rate_value_minus1[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.bit_rate_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.cpb_size_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "cpb_size_value_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_CPB_CNT_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "cpb_size_value_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.cpb_size_value_minus1[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cpb_size_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.cbr_flag, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "cbr_flag", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_CPB_CNT_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "cbr_flag" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.cbr_flag[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cbr_flag, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, indents + 1);
    dump_html_uint32_t(object.initial_cpb_removal_delay_length_minus1, settings, "uint32_t", "initial_cpb_removal_delay_length_minus1", indents + 1);
    dump_html_uint32_t(object.cpb_removal_delay_length_minus1, settings, "uint32_t", "cpb_removal_delay_length_minus1", indents + 1);
    dump_html_uint32_t(object.dpb_output_delay_length_minus1, settings, "uint32_t", "dpb_output_delay_length_minus1", indents + 1);
    dump_html_uint32_t(object.time_offset_length, settings, "uint32_t", "time_offset_length", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH264HrdParameters(const StdVideoH264HrdParameters* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264HrdParameters(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264SequenceParameterSetVui(const StdVideoH264SequenceParameterSetVui& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH264SpsVuiFlags(object.flags, settings, "StdVideoH264SpsVuiFlags", "flags", indents + 1);
    dump_html_StdVideoH264AspectRatioIdc(object.aspect_ratio_idc, settings, "StdVideoH264AspectRatioIdc", "aspect_ratio_idc", indents + 1);
    dump_html_uint16_t(object.sar_width, settings, "uint16_t", "sar_width", indents + 1);
    dump_html_uint16_t(object.sar_height, settings, "uint16_t", "sar_height", indents + 1);
    dump_html_uint8_t(object.video_format, settings, "uint8_t", "video_format", indents + 1);
    dump_html_uint8_t(object.colour_primaries, settings, "uint8_t", "colour_primaries", indents + 1);
    dump_html_uint8_t(object.transfer_characteristics, settings, "uint8_t", "transfer_characteristics", indents + 1);
    dump_html_uint8_t(object.matrix_coefficients, settings, "uint8_t", "matrix_coefficients", indents + 1);
    dump_html_uint32_t(object.num_units_in_tick, settings, "uint32_t", "num_units_in_tick", indents + 1);
    dump_html_uint32_t(object.time_scale, settings, "uint32_t", "time_scale", indents + 1);
    dump_html_uint8_t(object.max_num_reorder_frames, settings, "uint8_t", "max_num_reorder_frames", indents + 1);
    dump_html_uint8_t(object.max_dec_frame_buffering, settings, "uint8_t", "max_dec_frame_buffering", indents + 1);
    dump_html_uint8_t(object.chroma_sample_loc_type_top_field, settings, "uint8_t", "chroma_sample_loc_type_top_field", indents + 1);
    dump_html_uint8_t(object.chroma_sample_loc_type_bottom_field, settings, "uint8_t", "chroma_sample_loc_type_bottom_field", indents + 1);
    dump_html_uint32_t(object.reserved1, settings, "uint32_t", "reserved1", indents + 1);
    dump_html_StdVideoH264HrdParameters(object.pHrdParameters, settings, "const StdVideoH264HrdParameters*", "pHrdParameters", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH264SequenceParameterSetVui(const StdVideoH264SequenceParameterSetVui* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264SequenceParameterSetVui(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264SpsFlags(const StdVideoH264SpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.constraint_set0_flag, settings, "uint32_t: 1", "constraint_set0_flag", indents + 1);
    dump_html_uint32_t(object.constraint_set1_flag, settings, "uint32_t: 1", "constraint_set1_flag", indents + 1);
    dump_html_uint32_t(object.constraint_set2_flag, settings, "uint32_t: 1", "constraint_set2_flag", indents + 1);
    dump_html_uint32_t(object.constraint_set3_flag, settings, "uint32_t: 1", "constraint_set3_flag", indents + 1);
    dump_html_uint32_t(object.constraint_set4_flag, settings, "uint32_t: 1", "constraint_set4_flag", indents + 1);
    dump_html_uint32_t(object.constraint_set5_flag, settings, "uint32_t: 1", "constraint_set5_flag", indents + 1);
    dump_html_uint32_t(object.direct_8x8_inference_flag, settings, "uint32_t: 1", "direct_8x8_inference_flag", indents + 1);
    dump_html_uint32_t(object.mb_adaptive_frame_field_flag, settings, "uint32_t: 1", "mb_adaptive_frame_field_flag", indents + 1);
    dump_html_uint32_t(object.frame_mbs_only_flag, settings, "uint32_t: 1", "frame_mbs_only_flag", indents + 1);
    dump_html_uint32_t(object.delta_pic_order_always_zero_flag, settings, "uint32_t: 1", "delta_pic_order_always_zero_flag", indents + 1);
    dump_html_uint32_t(object.separate_colour_plane_flag, settings, "uint32_t: 1", "separate_colour_plane_flag", indents + 1);
    dump_html_uint32_t(object.gaps_in_frame_num_value_allowed_flag, settings, "uint32_t: 1", "gaps_in_frame_num_value_allowed_flag", indents + 1);
    dump_html_uint32_t(object.qpprime_y_zero_transform_bypass_flag, settings, "uint32_t: 1", "qpprime_y_zero_transform_bypass_flag", indents + 1);
    dump_html_uint32_t(object.frame_cropping_flag, settings, "uint32_t: 1", "frame_cropping_flag", indents + 1);
    dump_html_uint32_t(object.seq_scaling_matrix_present_flag, settings, "uint32_t: 1", "seq_scaling_matrix_present_flag", indents + 1);
    dump_html_uint32_t(object.vui_parameters_present_flag, settings, "uint32_t: 1", "vui_parameters_present_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH264SpsFlags(const StdVideoH264SpsFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264SpsFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264ScalingLists(const StdVideoH264ScalingLists& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint16_t(object.scaling_list_present_mask, settings, "uint16_t", "scaling_list_present_mask", indents + 1);
    dump_html_uint16_t(object.use_default_scaling_matrix_mask, settings, "uint16_t", "use_default_scaling_matrix_mask", indents + 1);
    dump_html_array_start(object.ScalingList4x4, STD_VIDEO_H264_SCALING_LIST_4X4_NUM_LISTS * STD_VIDEO_H264_SCALING_LIST_4X4_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H264_SCALING_LIST_4X4_NUM_LISTS][STD_VIDEO_H264_SCALING_LIST_4X4_NUM_ELEMENTS]", "ScalingList4x4", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_SCALING_LIST_4X4_NUM_LISTS; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H264_SCALING_LIST_4X4_NUM_ELEMENTS; ++j) {
            std::stringstream stream;
            stream << "ScalingList4x4" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_uint8_t(object.ScalingList4x4[i][j], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.ScalingList4x4, STD_VIDEO_H264_SCALING_LIST_4X4_NUM_LISTS * STD_VIDEO_H264_SCALING_LIST_4X4_NUM_ELEMENTS, settings, indents + 1);
    dump_html_array_start(object.ScalingList8x8, STD_VIDEO_H264_SCALING_LIST_8X8_NUM_LISTS * STD_VIDEO_H264_SCALING_LIST_8X8_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H264_SCALING_LIST_8X8_NUM_LISTS][STD_VIDEO_H264_SCALING_LIST_8X8_NUM_ELEMENTS]", "ScalingList8x8", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_SCALING_LIST_8X8_NUM_LISTS; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H264_SCALING_LIST_8X8_NUM_ELEMENTS; ++j) {
            std::stringstream stream;
            stream << "ScalingList8x8" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_uint8_t(object.ScalingList8x8[i][j], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.ScalingList8x8, STD_VIDEO_H264_SCALING_LIST_8X8_NUM_LISTS * STD_VIDEO_H264_SCALING_LIST_8X8_NUM_ELEMENTS, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH264ScalingLists(const StdVideoH264ScalingLists* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264ScalingLists(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264SequenceParameterSet(const StdVideoH264SequenceParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH264SpsFlags(object.flags, settings, "StdVideoH264SpsFlags", "flags", indents + 1);
    dump_html_StdVideoH264ProfileIdc(object.profile_idc, settings, "StdVideoH264ProfileIdc", "profile_idc", indents + 1);
    dump_html_StdVideoH264LevelIdc(object.level_idc, settings, "StdVideoH264LevelIdc", "level_idc", indents + 1);
    dump_html_StdVideoH264ChromaFormatIdc(object.chroma_format_idc, settings, "StdVideoH264ChromaFormatIdc", "chroma_format_idc", indents + 1);
    dump_html_uint8_t(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.bit_depth_luma_minus8, settings, "uint8_t", "bit_depth_luma_minus8", indents + 1);
    dump_html_uint8_t(object.bit_depth_chroma_minus8, settings, "uint8_t", "bit_depth_chroma_minus8", indents + 1);
    dump_html_uint8_t(object.log2_max_frame_num_minus4, settings, "uint8_t", "log2_max_frame_num_minus4", indents + 1);
    dump_html_StdVideoH264PocType(object.pic_order_cnt_type, settings, "StdVideoH264PocType", "pic_order_cnt_type", indents + 1);
    dump_html_int32_t(object.offset_for_non_ref_pic, settings, "int32_t", "offset_for_non_ref_pic", indents + 1);
    dump_html_int32_t(object.offset_for_top_to_bottom_field, settings, "int32_t", "offset_for_top_to_bottom_field", indents + 1);
    dump_html_uint8_t(object.log2_max_pic_order_cnt_lsb_minus4, settings, "uint8_t", "log2_max_pic_order_cnt_lsb_minus4", indents + 1);
    dump_html_uint8_t(object.num_ref_frames_in_pic_order_cnt_cycle, settings, "uint8_t", "num_ref_frames_in_pic_order_cnt_cycle", indents + 1);
    dump_html_uint8_t(object.max_num_ref_frames, settings, "uint8_t", "max_num_ref_frames", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_uint32_t(object.pic_width_in_mbs_minus1, settings, "uint32_t", "pic_width_in_mbs_minus1", indents + 1);
    dump_html_uint32_t(object.pic_height_in_map_units_minus1, settings, "uint32_t", "pic_height_in_map_units_minus1", indents + 1);
    dump_html_uint32_t(object.frame_crop_left_offset, settings, "uint32_t", "frame_crop_left_offset", indents + 1);
    dump_html_uint32_t(object.frame_crop_right_offset, settings, "uint32_t", "frame_crop_right_offset", indents + 1);
    dump_html_uint32_t(object.frame_crop_top_offset, settings, "uint32_t", "frame_crop_top_offset", indents + 1);
    dump_html_uint32_t(object.frame_crop_bottom_offset, settings, "uint32_t", "frame_crop_bottom_offset", indents + 1);
    dump_html_uint32_t(object.reserved2, settings, "uint32_t", "reserved2", indents + 1);
    dump_html_int32_t(object.pOffsetForRefFrame, settings, "const int32_t*", "pOffsetForRefFrame", indents + 1);
    dump_html_StdVideoH264ScalingLists(object.pScalingLists, settings, "const StdVideoH264ScalingLists*", "pScalingLists", indents + 1);
    dump_html_StdVideoH264SequenceParameterSetVui(object.pSequenceParameterSetVui, settings, "const StdVideoH264SequenceParameterSetVui*", "pSequenceParameterSetVui", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH264SequenceParameterSet(const StdVideoH264SequenceParameterSet* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264SequenceParameterSet(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264PpsFlags(const StdVideoH264PpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.transform_8x8_mode_flag, settings, "uint32_t: 1", "transform_8x8_mode_flag", indents + 1);
    dump_html_uint32_t(object.redundant_pic_cnt_present_flag, settings, "uint32_t: 1", "redundant_pic_cnt_present_flag", indents + 1);
    dump_html_uint32_t(object.constrained_intra_pred_flag, settings, "uint32_t: 1", "constrained_intra_pred_flag", indents + 1);
    dump_html_uint32_t(object.deblocking_filter_control_present_flag, settings, "uint32_t: 1", "deblocking_filter_control_present_flag", indents + 1);
    dump_html_uint32_t(object.weighted_pred_flag, settings, "uint32_t: 1", "weighted_pred_flag", indents + 1);
    dump_html_uint32_t(object.bottom_field_pic_order_in_frame_present_flag, settings, "uint32_t: 1", "bottom_field_pic_order_in_frame_present_flag", indents + 1);
    dump_html_uint32_t(object.entropy_coding_mode_flag, settings, "uint32_t: 1", "entropy_coding_mode_flag", indents + 1);
    dump_html_uint32_t(object.pic_scaling_matrix_present_flag, settings, "uint32_t: 1", "pic_scaling_matrix_present_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH264PpsFlags(const StdVideoH264PpsFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264PpsFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH264PictureParameterSet(const StdVideoH264PictureParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH264PpsFlags(object.flags, settings, "StdVideoH264PpsFlags", "flags", indents + 1);
    dump_html_uint8_t(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.pic_parameter_set_id, settings, "uint8_t", "pic_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.num_ref_idx_l0_default_active_minus1, settings, "uint8_t", "num_ref_idx_l0_default_active_minus1", indents + 1);
    dump_html_uint8_t(object.num_ref_idx_l1_default_active_minus1, settings, "uint8_t", "num_ref_idx_l1_default_active_minus1", indents + 1);
    dump_html_StdVideoH264WeightedBipredIdc(object.weighted_bipred_idc, settings, "StdVideoH264WeightedBipredIdc", "weighted_bipred_idc", indents + 1);
    dump_html_int8_t(object.pic_init_qp_minus26, settings, "int8_t", "pic_init_qp_minus26", indents + 1);
    dump_html_int8_t(object.pic_init_qs_minus26, settings, "int8_t", "pic_init_qs_minus26", indents + 1);
    dump_html_int8_t(object.chroma_qp_index_offset, settings, "int8_t", "chroma_qp_index_offset", indents + 1);
    dump_html_int8_t(object.second_chroma_qp_index_offset, settings, "int8_t", "second_chroma_qp_index_offset", indents + 1);
    dump_html_StdVideoH264ScalingLists(object.pScalingLists, settings, "const StdVideoH264ScalingLists*", "pScalingLists", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH264PictureParameterSet(const StdVideoH264PictureParameterSet* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH264PictureParameterSet(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeH264PictureInfoFlags(const StdVideoDecodeH264PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.field_pic_flag, settings, "uint32_t: 1", "field_pic_flag", indents + 1);
    dump_html_uint32_t(object.is_intra, settings, "uint32_t: 1", "is_intra", indents + 1);
    dump_html_uint32_t(object.IdrPicFlag, settings, "uint32_t: 1", "IdrPicFlag", indents + 1);
    dump_html_uint32_t(object.bottom_field_flag, settings, "uint32_t: 1", "bottom_field_flag", indents + 1);
    dump_html_uint32_t(object.is_reference, settings, "uint32_t: 1", "is_reference", indents + 1);
    dump_html_uint32_t(object.complementary_field_pair, settings, "uint32_t: 1", "complementary_field_pair", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeH264PictureInfoFlags(const StdVideoDecodeH264PictureInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeH264PictureInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeH264PictureInfo(const StdVideoDecodeH264PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoDecodeH264PictureInfoFlags(object.flags, settings, "StdVideoDecodeH264PictureInfoFlags", "flags", indents + 1);
    dump_html_uint8_t(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.pic_parameter_set_id, settings, "uint8_t", "pic_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_uint8_t(object.reserved2, settings, "uint8_t", "reserved2", indents + 1);
    dump_html_uint16_t(object.frame_num, settings, "uint16_t", "frame_num", indents + 1);
    dump_html_uint16_t(object.idr_pic_id, settings, "uint16_t", "idr_pic_id", indents + 1);
    dump_html_array_start(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, "int32_t[STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE]", "PicOrderCnt", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "PicOrderCnt" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int32_t(object.PicOrderCnt[i], settings, "int32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeH264PictureInfo(const StdVideoDecodeH264PictureInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeH264PictureInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeH264ReferenceInfoFlags(const StdVideoDecodeH264ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.top_field_flag, settings, "uint32_t: 1", "top_field_flag", indents + 1);
    dump_html_uint32_t(object.bottom_field_flag, settings, "uint32_t: 1", "bottom_field_flag", indents + 1);
    dump_html_uint32_t(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + 1);
    dump_html_uint32_t(object.is_non_existing, settings, "uint32_t: 1", "is_non_existing", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeH264ReferenceInfoFlags(const StdVideoDecodeH264ReferenceInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeH264ReferenceInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeH264ReferenceInfo(const StdVideoDecodeH264ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoDecodeH264ReferenceInfoFlags(object.flags, settings, "StdVideoDecodeH264ReferenceInfoFlags", "flags", indents + 1);
    dump_html_uint16_t(object.FrameNum, settings, "uint16_t", "FrameNum", indents + 1);
    dump_html_uint16_t(object.reserved, settings, "uint16_t", "reserved", indents + 1);
    dump_html_array_start(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, "int32_t[STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE]", "PicOrderCnt", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "PicOrderCnt" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int32_t(object.PicOrderCnt[i], settings, "int32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeH264ReferenceInfo(const StdVideoDecodeH264ReferenceInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeH264ReferenceInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264WeightTableFlags(const StdVideoEncodeH264WeightTableFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.luma_weight_l0_flag, settings, "uint32_t", "luma_weight_l0_flag", indents + 1);
    dump_html_uint32_t(object.chroma_weight_l0_flag, settings, "uint32_t", "chroma_weight_l0_flag", indents + 1);
    dump_html_uint32_t(object.luma_weight_l1_flag, settings, "uint32_t", "luma_weight_l1_flag", indents + 1);
    dump_html_uint32_t(object.chroma_weight_l1_flag, settings, "uint32_t", "chroma_weight_l1_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264WeightTableFlags(const StdVideoEncodeH264WeightTableFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264WeightTableFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264WeightTable(const StdVideoEncodeH264WeightTable& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH264WeightTableFlags(object.flags, settings, "StdVideoEncodeH264WeightTableFlags", "flags", indents + 1);
    dump_html_uint8_t(object.luma_log2_weight_denom, settings, "uint8_t", "luma_log2_weight_denom", indents + 1);
    dump_html_uint8_t(object.chroma_log2_weight_denom, settings, "uint8_t", "chroma_log2_weight_denom", indents + 1);
    dump_html_array_start(object.luma_weight_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "luma_weight_l0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "luma_weight_l0" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.luma_weight_l0[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.luma_weight_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.luma_offset_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "luma_offset_l0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "luma_offset_l0" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.luma_offset_l0[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.luma_offset_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.chroma_weight_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF * STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF][STD_VIDEO_H264_MAX_CHROMA_PLANES]", "chroma_weight_l0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H264_MAX_CHROMA_PLANES; ++j) {
            std::stringstream stream;
            stream << "chroma_weight_l0" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int8_t(object.chroma_weight_l0[i][j], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.chroma_weight_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF * STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, indents + 1);
    dump_html_array_start(object.chroma_offset_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF * STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF][STD_VIDEO_H264_MAX_CHROMA_PLANES]", "chroma_offset_l0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H264_MAX_CHROMA_PLANES; ++j) {
            std::stringstream stream;
            stream << "chroma_offset_l0" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int8_t(object.chroma_offset_l0[i][j], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.chroma_offset_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF * STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, indents + 1);
    dump_html_array_start(object.luma_weight_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "luma_weight_l1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "luma_weight_l1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.luma_weight_l1[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.luma_weight_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.luma_offset_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "luma_offset_l1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "luma_offset_l1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.luma_offset_l1[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.luma_offset_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.chroma_weight_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF * STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF][STD_VIDEO_H264_MAX_CHROMA_PLANES]", "chroma_weight_l1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H264_MAX_CHROMA_PLANES; ++j) {
            std::stringstream stream;
            stream << "chroma_weight_l1" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int8_t(object.chroma_weight_l1[i][j], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.chroma_weight_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF * STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, indents + 1);
    dump_html_array_start(object.chroma_offset_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF * STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF][STD_VIDEO_H264_MAX_CHROMA_PLANES]", "chroma_offset_l1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H264_MAX_CHROMA_PLANES; ++j) {
            std::stringstream stream;
            stream << "chroma_offset_l1" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int8_t(object.chroma_offset_l1[i][j], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.chroma_offset_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF * STD_VIDEO_H264_MAX_CHROMA_PLANES, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264WeightTable(const StdVideoEncodeH264WeightTable* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264WeightTable(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264SliceHeaderFlags(const StdVideoEncodeH264SliceHeaderFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.direct_spatial_mv_pred_flag, settings, "uint32_t: 1", "direct_spatial_mv_pred_flag", indents + 1);
    dump_html_uint32_t(object.num_ref_idx_active_override_flag, settings, "uint32_t: 1", "num_ref_idx_active_override_flag", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 30", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264SliceHeaderFlags(const StdVideoEncodeH264SliceHeaderFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264SliceHeaderFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264PictureInfoFlags(const StdVideoEncodeH264PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.IdrPicFlag, settings, "uint32_t: 1", "IdrPicFlag", indents + 1);
    dump_html_uint32_t(object.is_reference, settings, "uint32_t: 1", "is_reference", indents + 1);
    dump_html_uint32_t(object.no_output_of_prior_pics_flag, settings, "uint32_t: 1", "no_output_of_prior_pics_flag", indents + 1);
    dump_html_uint32_t(object.long_term_reference_flag, settings, "uint32_t: 1", "long_term_reference_flag", indents + 1);
    dump_html_uint32_t(object.adaptive_ref_pic_marking_mode_flag, settings, "uint32_t: 1", "adaptive_ref_pic_marking_mode_flag", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 27", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264PictureInfoFlags(const StdVideoEncodeH264PictureInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264PictureInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264ReferenceInfoFlags(const StdVideoEncodeH264ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 31", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264ReferenceInfoFlags(const StdVideoEncodeH264ReferenceInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264ReferenceInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264ReferenceListsInfoFlags(const StdVideoEncodeH264ReferenceListsInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.ref_pic_list_modification_flag_l0, settings, "uint32_t: 1", "ref_pic_list_modification_flag_l0", indents + 1);
    dump_html_uint32_t(object.ref_pic_list_modification_flag_l1, settings, "uint32_t: 1", "ref_pic_list_modification_flag_l1", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 30", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264ReferenceListsInfoFlags(const StdVideoEncodeH264ReferenceListsInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264ReferenceListsInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264RefListModEntry(const StdVideoEncodeH264RefListModEntry& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH264ModificationOfPicNumsIdc(object.modification_of_pic_nums_idc, settings, "StdVideoH264ModificationOfPicNumsIdc", "modification_of_pic_nums_idc", indents + 1);
    dump_html_uint16_t(object.abs_diff_pic_num_minus1, settings, "uint16_t", "abs_diff_pic_num_minus1", indents + 1);
    dump_html_uint16_t(object.long_term_pic_num, settings, "uint16_t", "long_term_pic_num", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264RefListModEntry(const StdVideoEncodeH264RefListModEntry* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264RefListModEntry(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264RefPicMarkingEntry(const StdVideoEncodeH264RefPicMarkingEntry& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH264MemMgmtControlOp(object.memory_management_control_operation, settings, "StdVideoH264MemMgmtControlOp", "memory_management_control_operation", indents + 1);
    dump_html_uint16_t(object.difference_of_pic_nums_minus1, settings, "uint16_t", "difference_of_pic_nums_minus1", indents + 1);
    dump_html_uint16_t(object.long_term_pic_num, settings, "uint16_t", "long_term_pic_num", indents + 1);
    dump_html_uint16_t(object.long_term_frame_idx, settings, "uint16_t", "long_term_frame_idx", indents + 1);
    dump_html_uint16_t(object.max_long_term_frame_idx_plus1, settings, "uint16_t", "max_long_term_frame_idx_plus1", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264RefPicMarkingEntry(const StdVideoEncodeH264RefPicMarkingEntry* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264RefPicMarkingEntry(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264ReferenceListsInfo(const StdVideoEncodeH264ReferenceListsInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH264ReferenceListsInfoFlags(object.flags, settings, "StdVideoEncodeH264ReferenceListsInfoFlags", "flags", indents + 1);
    dump_html_uint8_t(object.num_ref_idx_l0_active_minus1, settings, "uint8_t", "num_ref_idx_l0_active_minus1", indents + 1);
    dump_html_uint8_t(object.num_ref_idx_l1_active_minus1, settings, "uint8_t", "num_ref_idx_l1_active_minus1", indents + 1);
    dump_html_array_start(object.RefPicList0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "RefPicList0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "RefPicList0" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.RefPicList0[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.RefPicList0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.RefPicList1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "RefPicList1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H264_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "RefPicList1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.RefPicList1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.RefPicList1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_uint8_t(object.refList0ModOpCount, settings, "uint8_t", "refList0ModOpCount", indents + 1);
    dump_html_uint8_t(object.refList1ModOpCount, settings, "uint8_t", "refList1ModOpCount", indents + 1);
    dump_html_uint8_t(object.refPicMarkingOpCount, settings, "uint8_t", "refPicMarkingOpCount", indents + 1);
    dump_html_array_start(object.reserved1, 7, settings, "uint8_t[7]", "reserved1", indents + 1);
    for (size_t i = 0; i < 7; ++i) {
        std::stringstream stream;
        stream << "reserved1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.reserved1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved1, 7, settings, indents + 1);
    dump_html_StdVideoEncodeH264RefListModEntry(object.pRefList0ModOperations, settings, "const StdVideoEncodeH264RefListModEntry*", "pRefList0ModOperations", indents + 1);
    dump_html_StdVideoEncodeH264RefListModEntry(object.pRefList1ModOperations, settings, "const StdVideoEncodeH264RefListModEntry*", "pRefList1ModOperations", indents + 1);
    dump_html_StdVideoEncodeH264RefPicMarkingEntry(object.pRefPicMarkingOperations, settings, "const StdVideoEncodeH264RefPicMarkingEntry*", "pRefPicMarkingOperations", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264ReferenceListsInfo(const StdVideoEncodeH264ReferenceListsInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264ReferenceListsInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264PictureInfo(const StdVideoEncodeH264PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH264PictureInfoFlags(object.flags, settings, "StdVideoEncodeH264PictureInfoFlags", "flags", indents + 1);
    dump_html_uint8_t(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.pic_parameter_set_id, settings, "uint8_t", "pic_parameter_set_id", indents + 1);
    dump_html_uint16_t(object.idr_pic_id, settings, "uint16_t", "idr_pic_id", indents + 1);
    dump_html_StdVideoH264PictureType(object.primary_pic_type, settings, "StdVideoH264PictureType", "primary_pic_type", indents + 1);
    dump_html_uint32_t(object.frame_num, settings, "uint32_t", "frame_num", indents + 1);
    dump_html_int32_t(object.PicOrderCnt, settings, "int32_t", "PicOrderCnt", indents + 1);
    dump_html_uint8_t(object.temporal_id, settings, "uint8_t", "temporal_id", indents + 1);
    dump_html_array_start(object.reserved1, 3, settings, "uint8_t[3]", "reserved1", indents + 1);
    for (size_t i = 0; i < 3; ++i) {
        std::stringstream stream;
        stream << "reserved1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.reserved1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved1, 3, settings, indents + 1);
    dump_html_StdVideoEncodeH264ReferenceListsInfo(object.pRefLists, settings, "const StdVideoEncodeH264ReferenceListsInfo*", "pRefLists", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264PictureInfo(const StdVideoEncodeH264PictureInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264PictureInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264ReferenceInfo(const StdVideoEncodeH264ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH264ReferenceInfoFlags(object.flags, settings, "StdVideoEncodeH264ReferenceInfoFlags", "flags", indents + 1);
    dump_html_StdVideoH264PictureType(object.primary_pic_type, settings, "StdVideoH264PictureType", "primary_pic_type", indents + 1);
    dump_html_uint32_t(object.FrameNum, settings, "uint32_t", "FrameNum", indents + 1);
    dump_html_int32_t(object.PicOrderCnt, settings, "int32_t", "PicOrderCnt", indents + 1);
    dump_html_uint16_t(object.long_term_pic_num, settings, "uint16_t", "long_term_pic_num", indents + 1);
    dump_html_uint16_t(object.long_term_frame_idx, settings, "uint16_t", "long_term_frame_idx", indents + 1);
    dump_html_uint8_t(object.temporal_id, settings, "uint8_t", "temporal_id", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264ReferenceInfo(const StdVideoEncodeH264ReferenceInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264ReferenceInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH264SliceHeader(const StdVideoEncodeH264SliceHeader& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH264SliceHeaderFlags(object.flags, settings, "StdVideoEncodeH264SliceHeaderFlags", "flags", indents + 1);
    dump_html_uint32_t(object.first_mb_in_slice, settings, "uint32_t", "first_mb_in_slice", indents + 1);
    dump_html_StdVideoH264SliceType(object.slice_type, settings, "StdVideoH264SliceType", "slice_type", indents + 1);
    dump_html_int8_t(object.slice_alpha_c0_offset_div2, settings, "int8_t", "slice_alpha_c0_offset_div2", indents + 1);
    dump_html_int8_t(object.slice_beta_offset_div2, settings, "int8_t", "slice_beta_offset_div2", indents + 1);
    dump_html_int8_t(object.slice_qp_delta, settings, "int8_t", "slice_qp_delta", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_StdVideoH264CabacInitIdc(object.cabac_init_idc, settings, "StdVideoH264CabacInitIdc", "cabac_init_idc", indents + 1);
    dump_html_StdVideoH264DisableDeblockingFilterIdc(object.disable_deblocking_filter_idc, settings, "StdVideoH264DisableDeblockingFilterIdc", "disable_deblocking_filter_idc", indents + 1);
    dump_html_StdVideoEncodeH264WeightTable(object.pWeightTable, settings, "const StdVideoEncodeH264WeightTable*", "pWeightTable", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH264SliceHeader(const StdVideoEncodeH264SliceHeader* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH264SliceHeader(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265DecPicBufMgr(const StdVideoH265DecPicBufMgr& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_array_start(object.max_latency_increase_plus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "max_latency_increase_plus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SUBLAYERS_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "max_latency_increase_plus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.max_latency_increase_plus1[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.max_latency_increase_plus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.max_dec_pic_buffering_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "max_dec_pic_buffering_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SUBLAYERS_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "max_dec_pic_buffering_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.max_dec_pic_buffering_minus1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.max_dec_pic_buffering_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.max_num_reorder_pics, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "max_num_reorder_pics", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SUBLAYERS_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "max_num_reorder_pics" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.max_num_reorder_pics[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.max_num_reorder_pics, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265DecPicBufMgr(const StdVideoH265DecPicBufMgr* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265DecPicBufMgr(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265SubLayerHrdParameters(const StdVideoH265SubLayerHrdParameters& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_array_start(object.bit_rate_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "bit_rate_value_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_CPB_CNT_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "bit_rate_value_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.bit_rate_value_minus1[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.bit_rate_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.cpb_size_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "cpb_size_value_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_CPB_CNT_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "cpb_size_value_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.cpb_size_value_minus1[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cpb_size_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.cpb_size_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "cpb_size_du_value_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_CPB_CNT_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "cpb_size_du_value_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.cpb_size_du_value_minus1[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cpb_size_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.bit_rate_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "bit_rate_du_value_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_CPB_CNT_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "bit_rate_du_value_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.bit_rate_du_value_minus1[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.bit_rate_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, indents + 1);
    dump_html_uint32_t(object.cbr_flag, settings, "uint32_t", "cbr_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265SubLayerHrdParameters(const StdVideoH265SubLayerHrdParameters* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265SubLayerHrdParameters(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265HrdFlags(const StdVideoH265HrdFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.nal_hrd_parameters_present_flag, settings, "uint32_t: 1", "nal_hrd_parameters_present_flag", indents + 1);
    dump_html_uint32_t(object.vcl_hrd_parameters_present_flag, settings, "uint32_t: 1", "vcl_hrd_parameters_present_flag", indents + 1);
    dump_html_uint32_t(object.sub_pic_hrd_params_present_flag, settings, "uint32_t: 1", "sub_pic_hrd_params_present_flag", indents + 1);
    dump_html_uint32_t(object.sub_pic_cpb_params_in_pic_timing_sei_flag, settings, "uint32_t: 1", "sub_pic_cpb_params_in_pic_timing_sei_flag", indents + 1);
    dump_html_uint32_t(object.fixed_pic_rate_general_flag, settings, "uint32_t: 8", "fixed_pic_rate_general_flag", indents + 1);
    dump_html_uint32_t(object.fixed_pic_rate_within_cvs_flag, settings, "uint32_t: 8", "fixed_pic_rate_within_cvs_flag", indents + 1);
    dump_html_uint32_t(object.low_delay_hrd_flag, settings, "uint32_t: 8", "low_delay_hrd_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265HrdFlags(const StdVideoH265HrdFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265HrdFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265HrdParameters(const StdVideoH265HrdParameters& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH265HrdFlags(object.flags, settings, "StdVideoH265HrdFlags", "flags", indents + 1);
    dump_html_uint8_t(object.tick_divisor_minus2, settings, "uint8_t", "tick_divisor_minus2", indents + 1);
    dump_html_uint8_t(object.du_cpb_removal_delay_increment_length_minus1, settings, "uint8_t", "du_cpb_removal_delay_increment_length_minus1", indents + 1);
    dump_html_uint8_t(object.dpb_output_delay_du_length_minus1, settings, "uint8_t", "dpb_output_delay_du_length_minus1", indents + 1);
    dump_html_uint8_t(object.bit_rate_scale, settings, "uint8_t", "bit_rate_scale", indents + 1);
    dump_html_uint8_t(object.cpb_size_scale, settings, "uint8_t", "cpb_size_scale", indents + 1);
    dump_html_uint8_t(object.cpb_size_du_scale, settings, "uint8_t", "cpb_size_du_scale", indents + 1);
    dump_html_uint8_t(object.initial_cpb_removal_delay_length_minus1, settings, "uint8_t", "initial_cpb_removal_delay_length_minus1", indents + 1);
    dump_html_uint8_t(object.au_cpb_removal_delay_length_minus1, settings, "uint8_t", "au_cpb_removal_delay_length_minus1", indents + 1);
    dump_html_uint8_t(object.dpb_output_delay_length_minus1, settings, "uint8_t", "dpb_output_delay_length_minus1", indents + 1);
    dump_html_array_start(object.cpb_cnt_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "cpb_cnt_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SUBLAYERS_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "cpb_cnt_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.cpb_cnt_minus1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cpb_cnt_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.elemental_duration_in_tc_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "elemental_duration_in_tc_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SUBLAYERS_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "elemental_duration_in_tc_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint16_t(object.elemental_duration_in_tc_minus1[i], settings, "uint16_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.elemental_duration_in_tc_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.reserved, 3, settings, "uint16_t[3]", "reserved", indents + 1);
    for (size_t i = 0; i < 3; ++i) {
        std::stringstream stream;
        stream << "reserved" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint16_t(object.reserved[i], settings, "uint16_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved, 3, settings, indents + 1);
    dump_html_StdVideoH265SubLayerHrdParameters(object.pSubLayerHrdParametersNal, settings, "const StdVideoH265SubLayerHrdParameters*", "pSubLayerHrdParametersNal", indents + 1);
    dump_html_StdVideoH265SubLayerHrdParameters(object.pSubLayerHrdParametersVcl, settings, "const StdVideoH265SubLayerHrdParameters*", "pSubLayerHrdParametersVcl", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265HrdParameters(const StdVideoH265HrdParameters* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265HrdParameters(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265VpsFlags(const StdVideoH265VpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.vps_temporal_id_nesting_flag, settings, "uint32_t: 1", "vps_temporal_id_nesting_flag", indents + 1);
    dump_html_uint32_t(object.vps_sub_layer_ordering_info_present_flag, settings, "uint32_t: 1", "vps_sub_layer_ordering_info_present_flag", indents + 1);
    dump_html_uint32_t(object.vps_timing_info_present_flag, settings, "uint32_t: 1", "vps_timing_info_present_flag", indents + 1);
    dump_html_uint32_t(object.vps_poc_proportional_to_timing_flag, settings, "uint32_t: 1", "vps_poc_proportional_to_timing_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265VpsFlags(const StdVideoH265VpsFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265VpsFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265ProfileTierLevelFlags(const StdVideoH265ProfileTierLevelFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.general_tier_flag, settings, "uint32_t: 1", "general_tier_flag", indents + 1);
    dump_html_uint32_t(object.general_progressive_source_flag, settings, "uint32_t: 1", "general_progressive_source_flag", indents + 1);
    dump_html_uint32_t(object.general_interlaced_source_flag, settings, "uint32_t: 1", "general_interlaced_source_flag", indents + 1);
    dump_html_uint32_t(object.general_non_packed_constraint_flag, settings, "uint32_t: 1", "general_non_packed_constraint_flag", indents + 1);
    dump_html_uint32_t(object.general_frame_only_constraint_flag, settings, "uint32_t: 1", "general_frame_only_constraint_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265ProfileTierLevelFlags(const StdVideoH265ProfileTierLevelFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265ProfileTierLevelFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265ProfileTierLevel(const StdVideoH265ProfileTierLevel& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH265ProfileTierLevelFlags(object.flags, settings, "StdVideoH265ProfileTierLevelFlags", "flags", indents + 1);
    dump_html_StdVideoH265ProfileIdc(object.general_profile_idc, settings, "StdVideoH265ProfileIdc", "general_profile_idc", indents + 1);
    dump_html_StdVideoH265LevelIdc(object.general_level_idc, settings, "StdVideoH265LevelIdc", "general_level_idc", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265ProfileTierLevel(const StdVideoH265ProfileTierLevel* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265ProfileTierLevel(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265VideoParameterSet(const StdVideoH265VideoParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH265VpsFlags(object.flags, settings, "StdVideoH265VpsFlags", "flags", indents + 1);
    dump_html_uint8_t(object.vps_video_parameter_set_id, settings, "uint8_t", "vps_video_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.vps_max_sub_layers_minus1, settings, "uint8_t", "vps_max_sub_layers_minus1", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_uint8_t(object.reserved2, settings, "uint8_t", "reserved2", indents + 1);
    dump_html_uint32_t(object.vps_num_units_in_tick, settings, "uint32_t", "vps_num_units_in_tick", indents + 1);
    dump_html_uint32_t(object.vps_time_scale, settings, "uint32_t", "vps_time_scale", indents + 1);
    dump_html_uint32_t(object.vps_num_ticks_poc_diff_one_minus1, settings, "uint32_t", "vps_num_ticks_poc_diff_one_minus1", indents + 1);
    dump_html_uint32_t(object.reserved3, settings, "uint32_t", "reserved3", indents + 1);
    dump_html_StdVideoH265DecPicBufMgr(object.pDecPicBufMgr, settings, "const StdVideoH265DecPicBufMgr*", "pDecPicBufMgr", indents + 1);
    dump_html_StdVideoH265HrdParameters(object.pHrdParameters, settings, "const StdVideoH265HrdParameters*", "pHrdParameters", indents + 1);
    dump_html_StdVideoH265ProfileTierLevel(object.pProfileTierLevel, settings, "const StdVideoH265ProfileTierLevel*", "pProfileTierLevel", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265VideoParameterSet(const StdVideoH265VideoParameterSet* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265VideoParameterSet(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265ScalingLists(const StdVideoH265ScalingLists& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_array_start(object.ScalingList4x4, STD_VIDEO_H265_SCALING_LIST_4X4_NUM_LISTS * STD_VIDEO_H265_SCALING_LIST_4X4_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_4X4_NUM_LISTS][STD_VIDEO_H265_SCALING_LIST_4X4_NUM_ELEMENTS]", "ScalingList4x4", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SCALING_LIST_4X4_NUM_LISTS; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H265_SCALING_LIST_4X4_NUM_ELEMENTS; ++j) {
            std::stringstream stream;
            stream << "ScalingList4x4" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_uint8_t(object.ScalingList4x4[i][j], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.ScalingList4x4, STD_VIDEO_H265_SCALING_LIST_4X4_NUM_LISTS * STD_VIDEO_H265_SCALING_LIST_4X4_NUM_ELEMENTS, settings, indents + 1);
    dump_html_array_start(object.ScalingList8x8, STD_VIDEO_H265_SCALING_LIST_8X8_NUM_LISTS * STD_VIDEO_H265_SCALING_LIST_8X8_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_8X8_NUM_LISTS][STD_VIDEO_H265_SCALING_LIST_8X8_NUM_ELEMENTS]", "ScalingList8x8", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SCALING_LIST_8X8_NUM_LISTS; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H265_SCALING_LIST_8X8_NUM_ELEMENTS; ++j) {
            std::stringstream stream;
            stream << "ScalingList8x8" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_uint8_t(object.ScalingList8x8[i][j], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.ScalingList8x8, STD_VIDEO_H265_SCALING_LIST_8X8_NUM_LISTS * STD_VIDEO_H265_SCALING_LIST_8X8_NUM_ELEMENTS, settings, indents + 1);
    dump_html_array_start(object.ScalingList16x16, STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS * STD_VIDEO_H265_SCALING_LIST_16X16_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS][STD_VIDEO_H265_SCALING_LIST_16X16_NUM_ELEMENTS]", "ScalingList16x16", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H265_SCALING_LIST_16X16_NUM_ELEMENTS; ++j) {
            std::stringstream stream;
            stream << "ScalingList16x16" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_uint8_t(object.ScalingList16x16[i][j], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.ScalingList16x16, STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS * STD_VIDEO_H265_SCALING_LIST_16X16_NUM_ELEMENTS, settings, indents + 1);
    dump_html_array_start(object.ScalingList32x32, STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS * STD_VIDEO_H265_SCALING_LIST_32X32_NUM_ELEMENTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS][STD_VIDEO_H265_SCALING_LIST_32X32_NUM_ELEMENTS]", "ScalingList32x32", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H265_SCALING_LIST_32X32_NUM_ELEMENTS; ++j) {
            std::stringstream stream;
            stream << "ScalingList32x32" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_uint8_t(object.ScalingList32x32[i][j], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.ScalingList32x32, STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS * STD_VIDEO_H265_SCALING_LIST_32X32_NUM_ELEMENTS, settings, indents + 1);
    dump_html_array_start(object.ScalingListDCCoef16x16, STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS]", "ScalingListDCCoef16x16", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS; ++i) {
        std::stringstream stream;
        stream << "ScalingListDCCoef16x16" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.ScalingListDCCoef16x16[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.ScalingListDCCoef16x16, STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS, settings, indents + 1);
    dump_html_array_start(object.ScalingListDCCoef32x32, STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS]", "ScalingListDCCoef32x32", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS; ++i) {
        std::stringstream stream;
        stream << "ScalingListDCCoef32x32" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.ScalingListDCCoef32x32[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.ScalingListDCCoef32x32, STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265ScalingLists(const StdVideoH265ScalingLists* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265ScalingLists(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265SpsVuiFlags(const StdVideoH265SpsVuiFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.aspect_ratio_info_present_flag, settings, "uint32_t: 1", "aspect_ratio_info_present_flag", indents + 1);
    dump_html_uint32_t(object.overscan_info_present_flag, settings, "uint32_t: 1", "overscan_info_present_flag", indents + 1);
    dump_html_uint32_t(object.overscan_appropriate_flag, settings, "uint32_t: 1", "overscan_appropriate_flag", indents + 1);
    dump_html_uint32_t(object.video_signal_type_present_flag, settings, "uint32_t: 1", "video_signal_type_present_flag", indents + 1);
    dump_html_uint32_t(object.video_full_range_flag, settings, "uint32_t: 1", "video_full_range_flag", indents + 1);
    dump_html_uint32_t(object.colour_description_present_flag, settings, "uint32_t: 1", "colour_description_present_flag", indents + 1);
    dump_html_uint32_t(object.chroma_loc_info_present_flag, settings, "uint32_t: 1", "chroma_loc_info_present_flag", indents + 1);
    dump_html_uint32_t(object.neutral_chroma_indication_flag, settings, "uint32_t: 1", "neutral_chroma_indication_flag", indents + 1);
    dump_html_uint32_t(object.field_seq_flag, settings, "uint32_t: 1", "field_seq_flag", indents + 1);
    dump_html_uint32_t(object.frame_field_info_present_flag, settings, "uint32_t: 1", "frame_field_info_present_flag", indents + 1);
    dump_html_uint32_t(object.default_display_window_flag, settings, "uint32_t: 1", "default_display_window_flag", indents + 1);
    dump_html_uint32_t(object.vui_timing_info_present_flag, settings, "uint32_t: 1", "vui_timing_info_present_flag", indents + 1);
    dump_html_uint32_t(object.vui_poc_proportional_to_timing_flag, settings, "uint32_t: 1", "vui_poc_proportional_to_timing_flag", indents + 1);
    dump_html_uint32_t(object.vui_hrd_parameters_present_flag, settings, "uint32_t: 1", "vui_hrd_parameters_present_flag", indents + 1);
    dump_html_uint32_t(object.bitstream_restriction_flag, settings, "uint32_t: 1", "bitstream_restriction_flag", indents + 1);
    dump_html_uint32_t(object.tiles_fixed_structure_flag, settings, "uint32_t: 1", "tiles_fixed_structure_flag", indents + 1);
    dump_html_uint32_t(object.motion_vectors_over_pic_boundaries_flag, settings, "uint32_t: 1", "motion_vectors_over_pic_boundaries_flag", indents + 1);
    dump_html_uint32_t(object.restricted_ref_pic_lists_flag, settings, "uint32_t: 1", "restricted_ref_pic_lists_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265SpsVuiFlags(const StdVideoH265SpsVuiFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265SpsVuiFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265SequenceParameterSetVui(const StdVideoH265SequenceParameterSetVui& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH265SpsVuiFlags(object.flags, settings, "StdVideoH265SpsVuiFlags", "flags", indents + 1);
    dump_html_StdVideoH265AspectRatioIdc(object.aspect_ratio_idc, settings, "StdVideoH265AspectRatioIdc", "aspect_ratio_idc", indents + 1);
    dump_html_uint16_t(object.sar_width, settings, "uint16_t", "sar_width", indents + 1);
    dump_html_uint16_t(object.sar_height, settings, "uint16_t", "sar_height", indents + 1);
    dump_html_uint8_t(object.video_format, settings, "uint8_t", "video_format", indents + 1);
    dump_html_uint8_t(object.colour_primaries, settings, "uint8_t", "colour_primaries", indents + 1);
    dump_html_uint8_t(object.transfer_characteristics, settings, "uint8_t", "transfer_characteristics", indents + 1);
    dump_html_uint8_t(object.matrix_coeffs, settings, "uint8_t", "matrix_coeffs", indents + 1);
    dump_html_uint8_t(object.chroma_sample_loc_type_top_field, settings, "uint8_t", "chroma_sample_loc_type_top_field", indents + 1);
    dump_html_uint8_t(object.chroma_sample_loc_type_bottom_field, settings, "uint8_t", "chroma_sample_loc_type_bottom_field", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_uint8_t(object.reserved2, settings, "uint8_t", "reserved2", indents + 1);
    dump_html_uint16_t(object.def_disp_win_left_offset, settings, "uint16_t", "def_disp_win_left_offset", indents + 1);
    dump_html_uint16_t(object.def_disp_win_right_offset, settings, "uint16_t", "def_disp_win_right_offset", indents + 1);
    dump_html_uint16_t(object.def_disp_win_top_offset, settings, "uint16_t", "def_disp_win_top_offset", indents + 1);
    dump_html_uint16_t(object.def_disp_win_bottom_offset, settings, "uint16_t", "def_disp_win_bottom_offset", indents + 1);
    dump_html_uint32_t(object.vui_num_units_in_tick, settings, "uint32_t", "vui_num_units_in_tick", indents + 1);
    dump_html_uint32_t(object.vui_time_scale, settings, "uint32_t", "vui_time_scale", indents + 1);
    dump_html_uint32_t(object.vui_num_ticks_poc_diff_one_minus1, settings, "uint32_t", "vui_num_ticks_poc_diff_one_minus1", indents + 1);
    dump_html_uint16_t(object.min_spatial_segmentation_idc, settings, "uint16_t", "min_spatial_segmentation_idc", indents + 1);
    dump_html_uint16_t(object.reserved3, settings, "uint16_t", "reserved3", indents + 1);
    dump_html_uint8_t(object.max_bytes_per_pic_denom, settings, "uint8_t", "max_bytes_per_pic_denom", indents + 1);
    dump_html_uint8_t(object.max_bits_per_min_cu_denom, settings, "uint8_t", "max_bits_per_min_cu_denom", indents + 1);
    dump_html_uint8_t(object.log2_max_mv_length_horizontal, settings, "uint8_t", "log2_max_mv_length_horizontal", indents + 1);
    dump_html_uint8_t(object.log2_max_mv_length_vertical, settings, "uint8_t", "log2_max_mv_length_vertical", indents + 1);
    dump_html_StdVideoH265HrdParameters(object.pHrdParameters, settings, "const StdVideoH265HrdParameters*", "pHrdParameters", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265SequenceParameterSetVui(const StdVideoH265SequenceParameterSetVui* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265SequenceParameterSetVui(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265PredictorPaletteEntries(const StdVideoH265PredictorPaletteEntries& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_array_start(object.PredictorPaletteEntries, STD_VIDEO_H265_PREDICTOR_PALETTE_COMPONENTS_LIST_SIZE * STD_VIDEO_H265_PREDICTOR_PALETTE_COMP_ENTRIES_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_PREDICTOR_PALETTE_COMPONENTS_LIST_SIZE][STD_VIDEO_H265_PREDICTOR_PALETTE_COMP_ENTRIES_LIST_SIZE]", "PredictorPaletteEntries", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_PREDICTOR_PALETTE_COMPONENTS_LIST_SIZE; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H265_PREDICTOR_PALETTE_COMP_ENTRIES_LIST_SIZE; ++j) {
            std::stringstream stream;
            stream << "PredictorPaletteEntries" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_uint16_t(object.PredictorPaletteEntries[i][j], settings, "uint16_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.PredictorPaletteEntries, STD_VIDEO_H265_PREDICTOR_PALETTE_COMPONENTS_LIST_SIZE * STD_VIDEO_H265_PREDICTOR_PALETTE_COMP_ENTRIES_LIST_SIZE, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265PredictorPaletteEntries(const StdVideoH265PredictorPaletteEntries* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265PredictorPaletteEntries(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265SpsFlags(const StdVideoH265SpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.sps_temporal_id_nesting_flag, settings, "uint32_t: 1", "sps_temporal_id_nesting_flag", indents + 1);
    dump_html_uint32_t(object.separate_colour_plane_flag, settings, "uint32_t: 1", "separate_colour_plane_flag", indents + 1);
    dump_html_uint32_t(object.conformance_window_flag, settings, "uint32_t: 1", "conformance_window_flag", indents + 1);
    dump_html_uint32_t(object.sps_sub_layer_ordering_info_present_flag, settings, "uint32_t: 1", "sps_sub_layer_ordering_info_present_flag", indents + 1);
    dump_html_uint32_t(object.scaling_list_enabled_flag, settings, "uint32_t: 1", "scaling_list_enabled_flag", indents + 1);
    dump_html_uint32_t(object.sps_scaling_list_data_present_flag, settings, "uint32_t: 1", "sps_scaling_list_data_present_flag", indents + 1);
    dump_html_uint32_t(object.amp_enabled_flag, settings, "uint32_t: 1", "amp_enabled_flag", indents + 1);
    dump_html_uint32_t(object.sample_adaptive_offset_enabled_flag, settings, "uint32_t: 1", "sample_adaptive_offset_enabled_flag", indents + 1);
    dump_html_uint32_t(object.pcm_enabled_flag, settings, "uint32_t: 1", "pcm_enabled_flag", indents + 1);
    dump_html_uint32_t(object.pcm_loop_filter_disabled_flag, settings, "uint32_t: 1", "pcm_loop_filter_disabled_flag", indents + 1);
    dump_html_uint32_t(object.long_term_ref_pics_present_flag, settings, "uint32_t: 1", "long_term_ref_pics_present_flag", indents + 1);
    dump_html_uint32_t(object.sps_temporal_mvp_enabled_flag, settings, "uint32_t: 1", "sps_temporal_mvp_enabled_flag", indents + 1);
    dump_html_uint32_t(object.strong_intra_smoothing_enabled_flag, settings, "uint32_t: 1", "strong_intra_smoothing_enabled_flag", indents + 1);
    dump_html_uint32_t(object.vui_parameters_present_flag, settings, "uint32_t: 1", "vui_parameters_present_flag", indents + 1);
    dump_html_uint32_t(object.sps_extension_present_flag, settings, "uint32_t: 1", "sps_extension_present_flag", indents + 1);
    dump_html_uint32_t(object.sps_range_extension_flag, settings, "uint32_t: 1", "sps_range_extension_flag", indents + 1);
    dump_html_uint32_t(object.transform_skip_rotation_enabled_flag, settings, "uint32_t: 1", "transform_skip_rotation_enabled_flag", indents + 1);
    dump_html_uint32_t(object.transform_skip_context_enabled_flag, settings, "uint32_t: 1", "transform_skip_context_enabled_flag", indents + 1);
    dump_html_uint32_t(object.implicit_rdpcm_enabled_flag, settings, "uint32_t: 1", "implicit_rdpcm_enabled_flag", indents + 1);
    dump_html_uint32_t(object.explicit_rdpcm_enabled_flag, settings, "uint32_t: 1", "explicit_rdpcm_enabled_flag", indents + 1);
    dump_html_uint32_t(object.extended_precision_processing_flag, settings, "uint32_t: 1", "extended_precision_processing_flag", indents + 1);
    dump_html_uint32_t(object.intra_smoothing_disabled_flag, settings, "uint32_t: 1", "intra_smoothing_disabled_flag", indents + 1);
    dump_html_uint32_t(object.high_precision_offsets_enabled_flag, settings, "uint32_t: 1", "high_precision_offsets_enabled_flag", indents + 1);
    dump_html_uint32_t(object.persistent_rice_adaptation_enabled_flag, settings, "uint32_t: 1", "persistent_rice_adaptation_enabled_flag", indents + 1);
    dump_html_uint32_t(object.cabac_bypass_alignment_enabled_flag, settings, "uint32_t: 1", "cabac_bypass_alignment_enabled_flag", indents + 1);
    dump_html_uint32_t(object.sps_scc_extension_flag, settings, "uint32_t: 1", "sps_scc_extension_flag", indents + 1);
    dump_html_uint32_t(object.sps_curr_pic_ref_enabled_flag, settings, "uint32_t: 1", "sps_curr_pic_ref_enabled_flag", indents + 1);
    dump_html_uint32_t(object.palette_mode_enabled_flag, settings, "uint32_t: 1", "palette_mode_enabled_flag", indents + 1);
    dump_html_uint32_t(object.sps_palette_predictor_initializers_present_flag, settings, "uint32_t: 1", "sps_palette_predictor_initializers_present_flag", indents + 1);
    dump_html_uint32_t(object.intra_boundary_filtering_disabled_flag, settings, "uint32_t: 1", "intra_boundary_filtering_disabled_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265SpsFlags(const StdVideoH265SpsFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265SpsFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265ShortTermRefPicSetFlags(const StdVideoH265ShortTermRefPicSetFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.inter_ref_pic_set_prediction_flag, settings, "uint32_t: 1", "inter_ref_pic_set_prediction_flag", indents + 1);
    dump_html_uint32_t(object.delta_rps_sign, settings, "uint32_t: 1", "delta_rps_sign", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265ShortTermRefPicSetFlags(const StdVideoH265ShortTermRefPicSetFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265ShortTermRefPicSetFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265ShortTermRefPicSet(const StdVideoH265ShortTermRefPicSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH265ShortTermRefPicSetFlags(object.flags, settings, "StdVideoH265ShortTermRefPicSetFlags", "flags", indents + 1);
    dump_html_uint32_t(object.delta_idx_minus1, settings, "uint32_t", "delta_idx_minus1", indents + 1);
    dump_html_uint16_t(object.use_delta_flag, settings, "uint16_t", "use_delta_flag", indents + 1);
    dump_html_uint16_t(object.abs_delta_rps_minus1, settings, "uint16_t", "abs_delta_rps_minus1", indents + 1);
    dump_html_uint16_t(object.used_by_curr_pic_flag, settings, "uint16_t", "used_by_curr_pic_flag", indents + 1);
    dump_html_uint16_t(object.used_by_curr_pic_s0_flag, settings, "uint16_t", "used_by_curr_pic_s0_flag", indents + 1);
    dump_html_uint16_t(object.used_by_curr_pic_s1_flag, settings, "uint16_t", "used_by_curr_pic_s1_flag", indents + 1);
    dump_html_uint16_t(object.reserved1, settings, "uint16_t", "reserved1", indents + 1);
    dump_html_uint8_t(object.reserved2, settings, "uint8_t", "reserved2", indents + 1);
    dump_html_uint8_t(object.reserved3, settings, "uint8_t", "reserved3", indents + 1);
    dump_html_uint8_t(object.num_negative_pics, settings, "uint8_t", "num_negative_pics", indents + 1);
    dump_html_uint8_t(object.num_positive_pics, settings, "uint8_t", "num_positive_pics", indents + 1);
    dump_html_array_start(object.delta_poc_s0_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, "uint16_t[STD_VIDEO_H265_MAX_DPB_SIZE]", "delta_poc_s0_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_DPB_SIZE; ++i) {
        std::stringstream stream;
        stream << "delta_poc_s0_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint16_t(object.delta_poc_s0_minus1[i], settings, "uint16_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.delta_poc_s0_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, indents + 1);
    dump_html_array_start(object.delta_poc_s1_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, "uint16_t[STD_VIDEO_H265_MAX_DPB_SIZE]", "delta_poc_s1_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_DPB_SIZE; ++i) {
        std::stringstream stream;
        stream << "delta_poc_s1_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint16_t(object.delta_poc_s1_minus1[i], settings, "uint16_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.delta_poc_s1_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265ShortTermRefPicSet(const StdVideoH265ShortTermRefPicSet* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265ShortTermRefPicSet(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265LongTermRefPicsSps(const StdVideoH265LongTermRefPicsSps& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.used_by_curr_pic_lt_sps_flag, settings, "uint32_t", "used_by_curr_pic_lt_sps_flag", indents + 1);
    dump_html_array_start(object.lt_ref_pic_poc_lsb_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, "uint32_t[STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS]", "lt_ref_pic_poc_lsb_sps", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS; ++i) {
        std::stringstream stream;
        stream << "lt_ref_pic_poc_lsb_sps" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.lt_ref_pic_poc_lsb_sps[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.lt_ref_pic_poc_lsb_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265LongTermRefPicsSps(const StdVideoH265LongTermRefPicsSps* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265LongTermRefPicsSps(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265SequenceParameterSet(const StdVideoH265SequenceParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH265SpsFlags(object.flags, settings, "StdVideoH265SpsFlags", "flags", indents + 1);
    dump_html_StdVideoH265ChromaFormatIdc(object.chroma_format_idc, settings, "StdVideoH265ChromaFormatIdc", "chroma_format_idc", indents + 1);
    dump_html_uint32_t(object.pic_width_in_luma_samples, settings, "uint32_t", "pic_width_in_luma_samples", indents + 1);
    dump_html_uint32_t(object.pic_height_in_luma_samples, settings, "uint32_t", "pic_height_in_luma_samples", indents + 1);
    dump_html_uint8_t(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.sps_max_sub_layers_minus1, settings, "uint8_t", "sps_max_sub_layers_minus1", indents + 1);
    dump_html_uint8_t(object.sps_seq_parameter_set_id, settings, "uint8_t", "sps_seq_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.bit_depth_luma_minus8, settings, "uint8_t", "bit_depth_luma_minus8", indents + 1);
    dump_html_uint8_t(object.bit_depth_chroma_minus8, settings, "uint8_t", "bit_depth_chroma_minus8", indents + 1);
    dump_html_uint8_t(object.log2_max_pic_order_cnt_lsb_minus4, settings, "uint8_t", "log2_max_pic_order_cnt_lsb_minus4", indents + 1);
    dump_html_uint8_t(object.log2_min_luma_coding_block_size_minus3, settings, "uint8_t", "log2_min_luma_coding_block_size_minus3", indents + 1);
    dump_html_uint8_t(object.log2_diff_max_min_luma_coding_block_size, settings, "uint8_t", "log2_diff_max_min_luma_coding_block_size", indents + 1);
    dump_html_uint8_t(object.log2_min_luma_transform_block_size_minus2, settings, "uint8_t", "log2_min_luma_transform_block_size_minus2", indents + 1);
    dump_html_uint8_t(object.log2_diff_max_min_luma_transform_block_size, settings, "uint8_t", "log2_diff_max_min_luma_transform_block_size", indents + 1);
    dump_html_uint8_t(object.max_transform_hierarchy_depth_inter, settings, "uint8_t", "max_transform_hierarchy_depth_inter", indents + 1);
    dump_html_uint8_t(object.max_transform_hierarchy_depth_intra, settings, "uint8_t", "max_transform_hierarchy_depth_intra", indents + 1);
    dump_html_uint8_t(object.num_short_term_ref_pic_sets, settings, "uint8_t", "num_short_term_ref_pic_sets", indents + 1);
    dump_html_uint8_t(object.num_long_term_ref_pics_sps, settings, "uint8_t", "num_long_term_ref_pics_sps", indents + 1);
    dump_html_uint8_t(object.pcm_sample_bit_depth_luma_minus1, settings, "uint8_t", "pcm_sample_bit_depth_luma_minus1", indents + 1);
    dump_html_uint8_t(object.pcm_sample_bit_depth_chroma_minus1, settings, "uint8_t", "pcm_sample_bit_depth_chroma_minus1", indents + 1);
    dump_html_uint8_t(object.log2_min_pcm_luma_coding_block_size_minus3, settings, "uint8_t", "log2_min_pcm_luma_coding_block_size_minus3", indents + 1);
    dump_html_uint8_t(object.log2_diff_max_min_pcm_luma_coding_block_size, settings, "uint8_t", "log2_diff_max_min_pcm_luma_coding_block_size", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_uint8_t(object.reserved2, settings, "uint8_t", "reserved2", indents + 1);
    dump_html_uint8_t(object.palette_max_size, settings, "uint8_t", "palette_max_size", indents + 1);
    dump_html_uint8_t(object.delta_palette_max_predictor_size, settings, "uint8_t", "delta_palette_max_predictor_size", indents + 1);
    dump_html_uint8_t(object.motion_vector_resolution_control_idc, settings, "uint8_t", "motion_vector_resolution_control_idc", indents + 1);
    dump_html_uint8_t(object.sps_num_palette_predictor_initializers_minus1, settings, "uint8_t", "sps_num_palette_predictor_initializers_minus1", indents + 1);
    dump_html_uint32_t(object.conf_win_left_offset, settings, "uint32_t", "conf_win_left_offset", indents + 1);
    dump_html_uint32_t(object.conf_win_right_offset, settings, "uint32_t", "conf_win_right_offset", indents + 1);
    dump_html_uint32_t(object.conf_win_top_offset, settings, "uint32_t", "conf_win_top_offset", indents + 1);
    dump_html_uint32_t(object.conf_win_bottom_offset, settings, "uint32_t", "conf_win_bottom_offset", indents + 1);
    dump_html_StdVideoH265ProfileTierLevel(object.pProfileTierLevel, settings, "const StdVideoH265ProfileTierLevel*", "pProfileTierLevel", indents + 1);
    dump_html_StdVideoH265DecPicBufMgr(object.pDecPicBufMgr, settings, "const StdVideoH265DecPicBufMgr*", "pDecPicBufMgr", indents + 1);
    dump_html_StdVideoH265ScalingLists(object.pScalingLists, settings, "const StdVideoH265ScalingLists*", "pScalingLists", indents + 1);
    dump_html_StdVideoH265ShortTermRefPicSet(object.pShortTermRefPicSet, settings, "const StdVideoH265ShortTermRefPicSet*", "pShortTermRefPicSet", indents + 1);
    dump_html_StdVideoH265LongTermRefPicsSps(object.pLongTermRefPicsSps, settings, "const StdVideoH265LongTermRefPicsSps*", "pLongTermRefPicsSps", indents + 1);
    dump_html_StdVideoH265SequenceParameterSetVui(object.pSequenceParameterSetVui, settings, "const StdVideoH265SequenceParameterSetVui*", "pSequenceParameterSetVui", indents + 1);
    dump_html_StdVideoH265PredictorPaletteEntries(object.pPredictorPaletteEntries, settings, "const StdVideoH265PredictorPaletteEntries*", "pPredictorPaletteEntries", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265SequenceParameterSet(const StdVideoH265SequenceParameterSet* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265SequenceParameterSet(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265PpsFlags(const StdVideoH265PpsFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.dependent_slice_segments_enabled_flag, settings, "uint32_t: 1", "dependent_slice_segments_enabled_flag", indents + 1);
    dump_html_uint32_t(object.output_flag_present_flag, settings, "uint32_t: 1", "output_flag_present_flag", indents + 1);
    dump_html_uint32_t(object.sign_data_hiding_enabled_flag, settings, "uint32_t: 1", "sign_data_hiding_enabled_flag", indents + 1);
    dump_html_uint32_t(object.cabac_init_present_flag, settings, "uint32_t: 1", "cabac_init_present_flag", indents + 1);
    dump_html_uint32_t(object.constrained_intra_pred_flag, settings, "uint32_t: 1", "constrained_intra_pred_flag", indents + 1);
    dump_html_uint32_t(object.transform_skip_enabled_flag, settings, "uint32_t: 1", "transform_skip_enabled_flag", indents + 1);
    dump_html_uint32_t(object.cu_qp_delta_enabled_flag, settings, "uint32_t: 1", "cu_qp_delta_enabled_flag", indents + 1);
    dump_html_uint32_t(object.pps_slice_chroma_qp_offsets_present_flag, settings, "uint32_t: 1", "pps_slice_chroma_qp_offsets_present_flag", indents + 1);
    dump_html_uint32_t(object.weighted_pred_flag, settings, "uint32_t: 1", "weighted_pred_flag", indents + 1);
    dump_html_uint32_t(object.weighted_bipred_flag, settings, "uint32_t: 1", "weighted_bipred_flag", indents + 1);
    dump_html_uint32_t(object.transquant_bypass_enabled_flag, settings, "uint32_t: 1", "transquant_bypass_enabled_flag", indents + 1);
    dump_html_uint32_t(object.tiles_enabled_flag, settings, "uint32_t: 1", "tiles_enabled_flag", indents + 1);
    dump_html_uint32_t(object.entropy_coding_sync_enabled_flag, settings, "uint32_t: 1", "entropy_coding_sync_enabled_flag", indents + 1);
    dump_html_uint32_t(object.uniform_spacing_flag, settings, "uint32_t: 1", "uniform_spacing_flag", indents + 1);
    dump_html_uint32_t(object.loop_filter_across_tiles_enabled_flag, settings, "uint32_t: 1", "loop_filter_across_tiles_enabled_flag", indents + 1);
    dump_html_uint32_t(object.pps_loop_filter_across_slices_enabled_flag, settings, "uint32_t: 1", "pps_loop_filter_across_slices_enabled_flag", indents + 1);
    dump_html_uint32_t(object.deblocking_filter_control_present_flag, settings, "uint32_t: 1", "deblocking_filter_control_present_flag", indents + 1);
    dump_html_uint32_t(object.deblocking_filter_override_enabled_flag, settings, "uint32_t: 1", "deblocking_filter_override_enabled_flag", indents + 1);
    dump_html_uint32_t(object.pps_deblocking_filter_disabled_flag, settings, "uint32_t: 1", "pps_deblocking_filter_disabled_flag", indents + 1);
    dump_html_uint32_t(object.pps_scaling_list_data_present_flag, settings, "uint32_t: 1", "pps_scaling_list_data_present_flag", indents + 1);
    dump_html_uint32_t(object.lists_modification_present_flag, settings, "uint32_t: 1", "lists_modification_present_flag", indents + 1);
    dump_html_uint32_t(object.slice_segment_header_extension_present_flag, settings, "uint32_t: 1", "slice_segment_header_extension_present_flag", indents + 1);
    dump_html_uint32_t(object.pps_extension_present_flag, settings, "uint32_t: 1", "pps_extension_present_flag", indents + 1);
    dump_html_uint32_t(object.cross_component_prediction_enabled_flag, settings, "uint32_t: 1", "cross_component_prediction_enabled_flag", indents + 1);
    dump_html_uint32_t(object.chroma_qp_offset_list_enabled_flag, settings, "uint32_t: 1", "chroma_qp_offset_list_enabled_flag", indents + 1);
    dump_html_uint32_t(object.pps_curr_pic_ref_enabled_flag, settings, "uint32_t: 1", "pps_curr_pic_ref_enabled_flag", indents + 1);
    dump_html_uint32_t(object.residual_adaptive_colour_transform_enabled_flag, settings, "uint32_t: 1", "residual_adaptive_colour_transform_enabled_flag", indents + 1);
    dump_html_uint32_t(object.pps_slice_act_qp_offsets_present_flag, settings, "uint32_t: 1", "pps_slice_act_qp_offsets_present_flag", indents + 1);
    dump_html_uint32_t(object.pps_palette_predictor_initializers_present_flag, settings, "uint32_t: 1", "pps_palette_predictor_initializers_present_flag", indents + 1);
    dump_html_uint32_t(object.monochrome_palette_flag, settings, "uint32_t: 1", "monochrome_palette_flag", indents + 1);
    dump_html_uint32_t(object.pps_range_extension_flag, settings, "uint32_t: 1", "pps_range_extension_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265PpsFlags(const StdVideoH265PpsFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265PpsFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoH265PictureParameterSet(const StdVideoH265PictureParameterSet& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoH265PpsFlags(object.flags, settings, "StdVideoH265PpsFlags", "flags", indents + 1);
    dump_html_uint8_t(object.pps_pic_parameter_set_id, settings, "uint8_t", "pps_pic_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.pps_seq_parameter_set_id, settings, "uint8_t", "pps_seq_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.num_extra_slice_header_bits, settings, "uint8_t", "num_extra_slice_header_bits", indents + 1);
    dump_html_uint8_t(object.num_ref_idx_l0_default_active_minus1, settings, "uint8_t", "num_ref_idx_l0_default_active_minus1", indents + 1);
    dump_html_uint8_t(object.num_ref_idx_l1_default_active_minus1, settings, "uint8_t", "num_ref_idx_l1_default_active_minus1", indents + 1);
    dump_html_int8_t(object.init_qp_minus26, settings, "int8_t", "init_qp_minus26", indents + 1);
    dump_html_uint8_t(object.diff_cu_qp_delta_depth, settings, "uint8_t", "diff_cu_qp_delta_depth", indents + 1);
    dump_html_int8_t(object.pps_cb_qp_offset, settings, "int8_t", "pps_cb_qp_offset", indents + 1);
    dump_html_int8_t(object.pps_cr_qp_offset, settings, "int8_t", "pps_cr_qp_offset", indents + 1);
    dump_html_int8_t(object.pps_beta_offset_div2, settings, "int8_t", "pps_beta_offset_div2", indents + 1);
    dump_html_int8_t(object.pps_tc_offset_div2, settings, "int8_t", "pps_tc_offset_div2", indents + 1);
    dump_html_uint8_t(object.log2_parallel_merge_level_minus2, settings, "uint8_t", "log2_parallel_merge_level_minus2", indents + 1);
    dump_html_uint8_t(object.log2_max_transform_skip_block_size_minus2, settings, "uint8_t", "log2_max_transform_skip_block_size_minus2", indents + 1);
    dump_html_uint8_t(object.diff_cu_chroma_qp_offset_depth, settings, "uint8_t", "diff_cu_chroma_qp_offset_depth", indents + 1);
    dump_html_uint8_t(object.chroma_qp_offset_list_len_minus1, settings, "uint8_t", "chroma_qp_offset_list_len_minus1", indents + 1);
    dump_html_array_start(object.cb_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, "int8_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE]", "cb_qp_offset_list", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "cb_qp_offset_list" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.cb_qp_offset_list[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cb_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.cr_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, "int8_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE]", "cr_qp_offset_list", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "cr_qp_offset_list" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.cr_qp_offset_list[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cr_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, indents + 1);
    dump_html_uint8_t(object.log2_sao_offset_scale_luma, settings, "uint8_t", "log2_sao_offset_scale_luma", indents + 1);
    dump_html_uint8_t(object.log2_sao_offset_scale_chroma, settings, "uint8_t", "log2_sao_offset_scale_chroma", indents + 1);
    dump_html_int8_t(object.pps_act_y_qp_offset_plus5, settings, "int8_t", "pps_act_y_qp_offset_plus5", indents + 1);
    dump_html_int8_t(object.pps_act_cb_qp_offset_plus5, settings, "int8_t", "pps_act_cb_qp_offset_plus5", indents + 1);
    dump_html_int8_t(object.pps_act_cr_qp_offset_plus3, settings, "int8_t", "pps_act_cr_qp_offset_plus3", indents + 1);
    dump_html_uint8_t(object.pps_num_palette_predictor_initializers, settings, "uint8_t", "pps_num_palette_predictor_initializers", indents + 1);
    dump_html_uint8_t(object.luma_bit_depth_entry_minus8, settings, "uint8_t", "luma_bit_depth_entry_minus8", indents + 1);
    dump_html_uint8_t(object.chroma_bit_depth_entry_minus8, settings, "uint8_t", "chroma_bit_depth_entry_minus8", indents + 1);
    dump_html_uint8_t(object.num_tile_columns_minus1, settings, "uint8_t", "num_tile_columns_minus1", indents + 1);
    dump_html_uint8_t(object.num_tile_rows_minus1, settings, "uint8_t", "num_tile_rows_minus1", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_uint8_t(object.reserved2, settings, "uint8_t", "reserved2", indents + 1);
    dump_html_array_start(object.column_width_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE]", "column_width_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "column_width_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint16_t(object.column_width_minus1[i], settings, "uint16_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.column_width_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.row_height_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE]", "row_height_minus1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "row_height_minus1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint16_t(object.row_height_minus1[i], settings, "uint16_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.row_height_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE, settings, indents + 1);
    dump_html_uint32_t(object.reserved3, settings, "uint32_t", "reserved3", indents + 1);
    dump_html_StdVideoH265ScalingLists(object.pScalingLists, settings, "const StdVideoH265ScalingLists*", "pScalingLists", indents + 1);
    dump_html_StdVideoH265PredictorPaletteEntries(object.pPredictorPaletteEntries, settings, "const StdVideoH265PredictorPaletteEntries*", "pPredictorPaletteEntries", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoH265PictureParameterSet(const StdVideoH265PictureParameterSet* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoH265PictureParameterSet(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeH265PictureInfoFlags(const StdVideoDecodeH265PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.IrapPicFlag, settings, "uint32_t: 1", "IrapPicFlag", indents + 1);
    dump_html_uint32_t(object.IdrPicFlag, settings, "uint32_t: 1", "IdrPicFlag", indents + 1);
    dump_html_uint32_t(object.IsReference, settings, "uint32_t: 1", "IsReference", indents + 1);
    dump_html_uint32_t(object.short_term_ref_pic_set_sps_flag, settings, "uint32_t: 1", "short_term_ref_pic_set_sps_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeH265PictureInfoFlags(const StdVideoDecodeH265PictureInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeH265PictureInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeH265PictureInfo(const StdVideoDecodeH265PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoDecodeH265PictureInfoFlags(object.flags, settings, "StdVideoDecodeH265PictureInfoFlags", "flags", indents + 1);
    dump_html_uint8_t(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.pps_seq_parameter_set_id, settings, "uint8_t", "pps_seq_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.pps_pic_parameter_set_id, settings, "uint8_t", "pps_pic_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.NumDeltaPocsOfRefRpsIdx, settings, "uint8_t", "NumDeltaPocsOfRefRpsIdx", indents + 1);
    dump_html_int32_t(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + 1);
    dump_html_uint16_t(object.NumBitsForSTRefPicSetInSlice, settings, "uint16_t", "NumBitsForSTRefPicSetInSlice", indents + 1);
    dump_html_uint16_t(object.reserved, settings, "uint16_t", "reserved", indents + 1);
    dump_html_array_start(object.RefPicSetStCurrBefore, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "RefPicSetStCurrBefore", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "RefPicSetStCurrBefore" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.RefPicSetStCurrBefore[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.RefPicSetStCurrBefore, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.RefPicSetStCurrAfter, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "RefPicSetStCurrAfter", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "RefPicSetStCurrAfter" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.RefPicSetStCurrAfter[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.RefPicSetStCurrAfter, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, indents + 1);
    dump_html_array_start(object.RefPicSetLtCurr, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "RefPicSetLtCurr", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE; ++i) {
        std::stringstream stream;
        stream << "RefPicSetLtCurr" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.RefPicSetLtCurr[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.RefPicSetLtCurr, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeH265PictureInfo(const StdVideoDecodeH265PictureInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeH265PictureInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeH265ReferenceInfoFlags(const StdVideoDecodeH265ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + 1);
    dump_html_uint32_t(object.unused_for_reference, settings, "uint32_t: 1", "unused_for_reference", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeH265ReferenceInfoFlags(const StdVideoDecodeH265ReferenceInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeH265ReferenceInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeH265ReferenceInfo(const StdVideoDecodeH265ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoDecodeH265ReferenceInfoFlags(object.flags, settings, "StdVideoDecodeH265ReferenceInfoFlags", "flags", indents + 1);
    dump_html_int32_t(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeH265ReferenceInfo(const StdVideoDecodeH265ReferenceInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeH265ReferenceInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265WeightTableFlags(const StdVideoEncodeH265WeightTableFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint16_t(object.luma_weight_l0_flag, settings, "uint16_t", "luma_weight_l0_flag", indents + 1);
    dump_html_uint16_t(object.chroma_weight_l0_flag, settings, "uint16_t", "chroma_weight_l0_flag", indents + 1);
    dump_html_uint16_t(object.luma_weight_l1_flag, settings, "uint16_t", "luma_weight_l1_flag", indents + 1);
    dump_html_uint16_t(object.chroma_weight_l1_flag, settings, "uint16_t", "chroma_weight_l1_flag", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265WeightTableFlags(const StdVideoEncodeH265WeightTableFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265WeightTableFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265WeightTable(const StdVideoEncodeH265WeightTable& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH265WeightTableFlags(object.flags, settings, "StdVideoEncodeH265WeightTableFlags", "flags", indents + 1);
    dump_html_uint8_t(object.luma_log2_weight_denom, settings, "uint8_t", "luma_log2_weight_denom", indents + 1);
    dump_html_int8_t(object.delta_chroma_log2_weight_denom, settings, "int8_t", "delta_chroma_log2_weight_denom", indents + 1);
    dump_html_array_start(object.delta_luma_weight_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "delta_luma_weight_l0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "delta_luma_weight_l0" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.delta_luma_weight_l0[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.delta_luma_weight_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.luma_offset_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "luma_offset_l0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "luma_offset_l0" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.luma_offset_l0[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.luma_offset_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.delta_chroma_weight_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF * STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF][STD_VIDEO_H265_MAX_CHROMA_PLANES]", "delta_chroma_weight_l0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H265_MAX_CHROMA_PLANES; ++j) {
            std::stringstream stream;
            stream << "delta_chroma_weight_l0" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int8_t(object.delta_chroma_weight_l0[i][j], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.delta_chroma_weight_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF * STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, indents + 1);
    dump_html_array_start(object.delta_chroma_offset_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF * STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF][STD_VIDEO_H265_MAX_CHROMA_PLANES]", "delta_chroma_offset_l0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H265_MAX_CHROMA_PLANES; ++j) {
            std::stringstream stream;
            stream << "delta_chroma_offset_l0" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int8_t(object.delta_chroma_offset_l0[i][j], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.delta_chroma_offset_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF * STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, indents + 1);
    dump_html_array_start(object.delta_luma_weight_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "delta_luma_weight_l1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "delta_luma_weight_l1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.delta_luma_weight_l1[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.delta_luma_weight_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.luma_offset_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "luma_offset_l1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "luma_offset_l1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.luma_offset_l1[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.luma_offset_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.delta_chroma_weight_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF * STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF][STD_VIDEO_H265_MAX_CHROMA_PLANES]", "delta_chroma_weight_l1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H265_MAX_CHROMA_PLANES; ++j) {
            std::stringstream stream;
            stream << "delta_chroma_weight_l1" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int8_t(object.delta_chroma_weight_l1[i][j], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.delta_chroma_weight_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF * STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, indents + 1);
    dump_html_array_start(object.delta_chroma_offset_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF * STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF][STD_VIDEO_H265_MAX_CHROMA_PLANES]", "delta_chroma_offset_l1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        for (size_t j = 0; j < STD_VIDEO_H265_MAX_CHROMA_PLANES; ++j) {
            std::stringstream stream;
            stream << "delta_chroma_offset_l1" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int8_t(object.delta_chroma_offset_l1[i][j], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.delta_chroma_offset_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF * STD_VIDEO_H265_MAX_CHROMA_PLANES, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265WeightTable(const StdVideoEncodeH265WeightTable* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265WeightTable(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265SliceSegmentHeaderFlags(const StdVideoEncodeH265SliceSegmentHeaderFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.first_slice_segment_in_pic_flag, settings, "uint32_t: 1", "first_slice_segment_in_pic_flag", indents + 1);
    dump_html_uint32_t(object.dependent_slice_segment_flag, settings, "uint32_t: 1", "dependent_slice_segment_flag", indents + 1);
    dump_html_uint32_t(object.slice_sao_luma_flag, settings, "uint32_t: 1", "slice_sao_luma_flag", indents + 1);
    dump_html_uint32_t(object.slice_sao_chroma_flag, settings, "uint32_t: 1", "slice_sao_chroma_flag", indents + 1);
    dump_html_uint32_t(object.num_ref_idx_active_override_flag, settings, "uint32_t: 1", "num_ref_idx_active_override_flag", indents + 1);
    dump_html_uint32_t(object.mvd_l1_zero_flag, settings, "uint32_t: 1", "mvd_l1_zero_flag", indents + 1);
    dump_html_uint32_t(object.cabac_init_flag, settings, "uint32_t: 1", "cabac_init_flag", indents + 1);
    dump_html_uint32_t(object.cu_chroma_qp_offset_enabled_flag, settings, "uint32_t: 1", "cu_chroma_qp_offset_enabled_flag", indents + 1);
    dump_html_uint32_t(object.deblocking_filter_override_flag, settings, "uint32_t: 1", "deblocking_filter_override_flag", indents + 1);
    dump_html_uint32_t(object.slice_deblocking_filter_disabled_flag, settings, "uint32_t: 1", "slice_deblocking_filter_disabled_flag", indents + 1);
    dump_html_uint32_t(object.collocated_from_l0_flag, settings, "uint32_t: 1", "collocated_from_l0_flag", indents + 1);
    dump_html_uint32_t(object.slice_loop_filter_across_slices_enabled_flag, settings, "uint32_t: 1", "slice_loop_filter_across_slices_enabled_flag", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 20", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265SliceSegmentHeaderFlags(const StdVideoEncodeH265SliceSegmentHeaderFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265SliceSegmentHeaderFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265SliceSegmentHeader(const StdVideoEncodeH265SliceSegmentHeader& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH265SliceSegmentHeaderFlags(object.flags, settings, "StdVideoEncodeH265SliceSegmentHeaderFlags", "flags", indents + 1);
    dump_html_StdVideoH265SliceType(object.slice_type, settings, "StdVideoH265SliceType", "slice_type", indents + 1);
    dump_html_uint32_t(object.slice_segment_address, settings, "uint32_t", "slice_segment_address", indents + 1);
    dump_html_uint8_t(object.collocated_ref_idx, settings, "uint8_t", "collocated_ref_idx", indents + 1);
    dump_html_uint8_t(object.MaxNumMergeCand, settings, "uint8_t", "MaxNumMergeCand", indents + 1);
    dump_html_int8_t(object.slice_cb_qp_offset, settings, "int8_t", "slice_cb_qp_offset", indents + 1);
    dump_html_int8_t(object.slice_cr_qp_offset, settings, "int8_t", "slice_cr_qp_offset", indents + 1);
    dump_html_int8_t(object.slice_beta_offset_div2, settings, "int8_t", "slice_beta_offset_div2", indents + 1);
    dump_html_int8_t(object.slice_tc_offset_div2, settings, "int8_t", "slice_tc_offset_div2", indents + 1);
    dump_html_int8_t(object.slice_act_y_qp_offset, settings, "int8_t", "slice_act_y_qp_offset", indents + 1);
    dump_html_int8_t(object.slice_act_cb_qp_offset, settings, "int8_t", "slice_act_cb_qp_offset", indents + 1);
    dump_html_int8_t(object.slice_act_cr_qp_offset, settings, "int8_t", "slice_act_cr_qp_offset", indents + 1);
    dump_html_int8_t(object.slice_qp_delta, settings, "int8_t", "slice_qp_delta", indents + 1);
    dump_html_uint16_t(object.reserved1, settings, "uint16_t", "reserved1", indents + 1);
    dump_html_StdVideoEncodeH265WeightTable(object.pWeightTable, settings, "const StdVideoEncodeH265WeightTable*", "pWeightTable", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265SliceSegmentHeader(const StdVideoEncodeH265SliceSegmentHeader* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265SliceSegmentHeader(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265ReferenceListsInfoFlags(const StdVideoEncodeH265ReferenceListsInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.ref_pic_list_modification_flag_l0, settings, "uint32_t: 1", "ref_pic_list_modification_flag_l0", indents + 1);
    dump_html_uint32_t(object.ref_pic_list_modification_flag_l1, settings, "uint32_t: 1", "ref_pic_list_modification_flag_l1", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 30", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265ReferenceListsInfoFlags(const StdVideoEncodeH265ReferenceListsInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265ReferenceListsInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265ReferenceListsInfo(const StdVideoEncodeH265ReferenceListsInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH265ReferenceListsInfoFlags(object.flags, settings, "StdVideoEncodeH265ReferenceListsInfoFlags", "flags", indents + 1);
    dump_html_uint8_t(object.num_ref_idx_l0_active_minus1, settings, "uint8_t", "num_ref_idx_l0_active_minus1", indents + 1);
    dump_html_uint8_t(object.num_ref_idx_l1_active_minus1, settings, "uint8_t", "num_ref_idx_l1_active_minus1", indents + 1);
    dump_html_array_start(object.RefPicList0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "RefPicList0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "RefPicList0" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.RefPicList0[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.RefPicList0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.RefPicList1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "RefPicList1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "RefPicList1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.RefPicList1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.RefPicList1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.list_entry_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "list_entry_l0", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "list_entry_l0" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.list_entry_l0[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.list_entry_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_array_start(object.list_entry_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "list_entry_l1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_NUM_LIST_REF; ++i) {
        std::stringstream stream;
        stream << "list_entry_l1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.list_entry_l1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.list_entry_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265ReferenceListsInfo(const StdVideoEncodeH265ReferenceListsInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265ReferenceListsInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265PictureInfoFlags(const StdVideoEncodeH265PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.is_reference, settings, "uint32_t: 1", "is_reference", indents + 1);
    dump_html_uint32_t(object.IrapPicFlag, settings, "uint32_t: 1", "IrapPicFlag", indents + 1);
    dump_html_uint32_t(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + 1);
    dump_html_uint32_t(object.discardable_flag, settings, "uint32_t: 1", "discardable_flag", indents + 1);
    dump_html_uint32_t(object.cross_layer_bla_flag, settings, "uint32_t: 1", "cross_layer_bla_flag", indents + 1);
    dump_html_uint32_t(object.pic_output_flag, settings, "uint32_t: 1", "pic_output_flag", indents + 1);
    dump_html_uint32_t(object.no_output_of_prior_pics_flag, settings, "uint32_t: 1", "no_output_of_prior_pics_flag", indents + 1);
    dump_html_uint32_t(object.short_term_ref_pic_set_sps_flag, settings, "uint32_t: 1", "short_term_ref_pic_set_sps_flag", indents + 1);
    dump_html_uint32_t(object.slice_temporal_mvp_enabled_flag, settings, "uint32_t: 1", "slice_temporal_mvp_enabled_flag", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 23", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265PictureInfoFlags(const StdVideoEncodeH265PictureInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265PictureInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265LongTermRefPics(const StdVideoEncodeH265LongTermRefPics& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint8_t(object.num_long_term_sps, settings, "uint8_t", "num_long_term_sps", indents + 1);
    dump_html_uint8_t(object.num_long_term_pics, settings, "uint8_t", "num_long_term_pics", indents + 1);
    dump_html_array_start(object.lt_idx_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, "uint8_t[STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS]", "lt_idx_sps", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS; ++i) {
        std::stringstream stream;
        stream << "lt_idx_sps" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.lt_idx_sps[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.lt_idx_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, indents + 1);
    dump_html_array_start(object.poc_lsb_lt, STD_VIDEO_H265_MAX_LONG_TERM_PICS, settings, "uint8_t[STD_VIDEO_H265_MAX_LONG_TERM_PICS]", "poc_lsb_lt", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_LONG_TERM_PICS; ++i) {
        std::stringstream stream;
        stream << "poc_lsb_lt" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.poc_lsb_lt[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.poc_lsb_lt, STD_VIDEO_H265_MAX_LONG_TERM_PICS, settings, indents + 1);
    dump_html_uint16_t(object.used_by_curr_pic_lt_flag, settings, "uint16_t", "used_by_curr_pic_lt_flag", indents + 1);
    dump_html_array_start(object.delta_poc_msb_present_flag, STD_VIDEO_H265_MAX_DELTA_POC, settings, "uint8_t[STD_VIDEO_H265_MAX_DELTA_POC]", "delta_poc_msb_present_flag", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_DELTA_POC; ++i) {
        std::stringstream stream;
        stream << "delta_poc_msb_present_flag" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.delta_poc_msb_present_flag[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.delta_poc_msb_present_flag, STD_VIDEO_H265_MAX_DELTA_POC, settings, indents + 1);
    dump_html_array_start(object.delta_poc_msb_cycle_lt, STD_VIDEO_H265_MAX_DELTA_POC, settings, "uint8_t[STD_VIDEO_H265_MAX_DELTA_POC]", "delta_poc_msb_cycle_lt", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_H265_MAX_DELTA_POC; ++i) {
        std::stringstream stream;
        stream << "delta_poc_msb_cycle_lt" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.delta_poc_msb_cycle_lt[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.delta_poc_msb_cycle_lt, STD_VIDEO_H265_MAX_DELTA_POC, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265LongTermRefPics(const StdVideoEncodeH265LongTermRefPics* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265LongTermRefPics(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265PictureInfo(const StdVideoEncodeH265PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH265PictureInfoFlags(object.flags, settings, "StdVideoEncodeH265PictureInfoFlags", "flags", indents + 1);
    dump_html_StdVideoH265PictureType(object.pic_type, settings, "StdVideoH265PictureType", "pic_type", indents + 1);
    dump_html_uint8_t(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.pps_seq_parameter_set_id, settings, "uint8_t", "pps_seq_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.pps_pic_parameter_set_id, settings, "uint8_t", "pps_pic_parameter_set_id", indents + 1);
    dump_html_uint8_t(object.short_term_ref_pic_set_idx, settings, "uint8_t", "short_term_ref_pic_set_idx", indents + 1);
    dump_html_int32_t(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + 1);
    dump_html_uint8_t(object.TemporalId, settings, "uint8_t", "TemporalId", indents + 1);
    dump_html_array_start(object.reserved1, 7, settings, "uint8_t[7]", "reserved1", indents + 1);
    for (size_t i = 0; i < 7; ++i) {
        std::stringstream stream;
        stream << "reserved1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.reserved1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved1, 7, settings, indents + 1);
    dump_html_StdVideoEncodeH265ReferenceListsInfo(object.pRefLists, settings, "const StdVideoEncodeH265ReferenceListsInfo*", "pRefLists", indents + 1);
    dump_html_StdVideoH265ShortTermRefPicSet(object.pShortTermRefPicSet, settings, "const StdVideoH265ShortTermRefPicSet*", "pShortTermRefPicSet", indents + 1);
    dump_html_StdVideoEncodeH265LongTermRefPics(object.pLongTermRefPics, settings, "const StdVideoEncodeH265LongTermRefPics*", "pLongTermRefPics", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265PictureInfo(const StdVideoEncodeH265PictureInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265PictureInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265ReferenceInfoFlags(const StdVideoEncodeH265ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.used_for_long_term_reference, settings, "uint32_t: 1", "used_for_long_term_reference", indents + 1);
    dump_html_uint32_t(object.unused_for_reference, settings, "uint32_t: 1", "unused_for_reference", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 30", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265ReferenceInfoFlags(const StdVideoEncodeH265ReferenceInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265ReferenceInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeH265ReferenceInfo(const StdVideoEncodeH265ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeH265ReferenceInfoFlags(object.flags, settings, "StdVideoEncodeH265ReferenceInfoFlags", "flags", indents + 1);
    dump_html_StdVideoH265PictureType(object.pic_type, settings, "StdVideoH265PictureType", "pic_type", indents + 1);
    dump_html_int32_t(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + 1);
    dump_html_uint8_t(object.TemporalId, settings, "uint8_t", "TemporalId", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeH265ReferenceInfo(const StdVideoEncodeH265ReferenceInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeH265ReferenceInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1ColorConfigFlags(const StdVideoAV1ColorConfigFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.mono_chrome, settings, "uint32_t: 1", "mono_chrome", indents + 1);
    dump_html_uint32_t(object.color_range, settings, "uint32_t: 1", "color_range", indents + 1);
    dump_html_uint32_t(object.separate_uv_delta_q, settings, "uint32_t: 1", "separate_uv_delta_q", indents + 1);
    dump_html_uint32_t(object.color_description_present_flag, settings, "uint32_t: 1", "color_description_present_flag", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 28", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1ColorConfigFlags(const StdVideoAV1ColorConfigFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1ColorConfigFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1ColorConfig(const StdVideoAV1ColorConfig& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoAV1ColorConfigFlags(object.flags, settings, "StdVideoAV1ColorConfigFlags", "flags", indents + 1);
    dump_html_uint8_t(object.BitDepth, settings, "uint8_t", "BitDepth", indents + 1);
    dump_html_uint8_t(object.subsampling_x, settings, "uint8_t", "subsampling_x", indents + 1);
    dump_html_uint8_t(object.subsampling_y, settings, "uint8_t", "subsampling_y", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_StdVideoAV1ColorPrimaries(object.color_primaries, settings, "StdVideoAV1ColorPrimaries", "color_primaries", indents + 1);
    dump_html_StdVideoAV1TransferCharacteristics(object.transfer_characteristics, settings, "StdVideoAV1TransferCharacteristics", "transfer_characteristics", indents + 1);
    dump_html_StdVideoAV1MatrixCoefficients(object.matrix_coefficients, settings, "StdVideoAV1MatrixCoefficients", "matrix_coefficients", indents + 1);
    dump_html_StdVideoAV1ChromaSamplePosition(object.chroma_sample_position, settings, "StdVideoAV1ChromaSamplePosition", "chroma_sample_position", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1ColorConfig(const StdVideoAV1ColorConfig* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1ColorConfig(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1TimingInfoFlags(const StdVideoAV1TimingInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.equal_picture_interval, settings, "uint32_t: 1", "equal_picture_interval", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 31", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1TimingInfoFlags(const StdVideoAV1TimingInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1TimingInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1TimingInfo(const StdVideoAV1TimingInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoAV1TimingInfoFlags(object.flags, settings, "StdVideoAV1TimingInfoFlags", "flags", indents + 1);
    dump_html_uint32_t(object.num_units_in_display_tick, settings, "uint32_t", "num_units_in_display_tick", indents + 1);
    dump_html_uint32_t(object.time_scale, settings, "uint32_t", "time_scale", indents + 1);
    dump_html_uint32_t(object.num_ticks_per_picture_minus_1, settings, "uint32_t", "num_ticks_per_picture_minus_1", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1TimingInfo(const StdVideoAV1TimingInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1TimingInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1LoopFilterFlags(const StdVideoAV1LoopFilterFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.loop_filter_delta_enabled, settings, "uint32_t: 1", "loop_filter_delta_enabled", indents + 1);
    dump_html_uint32_t(object.loop_filter_delta_update, settings, "uint32_t: 1", "loop_filter_delta_update", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 30", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1LoopFilterFlags(const StdVideoAV1LoopFilterFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1LoopFilterFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1LoopFilter(const StdVideoAV1LoopFilter& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoAV1LoopFilterFlags(object.flags, settings, "StdVideoAV1LoopFilterFlags", "flags", indents + 1);
    dump_html_array_start(object.loop_filter_level, STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS]", "loop_filter_level", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS; ++i) {
        std::stringstream stream;
        stream << "loop_filter_level" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.loop_filter_level[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.loop_filter_level, STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS, settings, indents + 1);
    dump_html_uint8_t(object.loop_filter_sharpness, settings, "uint8_t", "loop_filter_sharpness", indents + 1);
    dump_html_uint8_t(object.update_ref_delta, settings, "uint8_t", "update_ref_delta", indents + 1);
    dump_html_array_start(object.loop_filter_ref_deltas, STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME, settings, "int8_t[STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME]", "loop_filter_ref_deltas", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME; ++i) {
        std::stringstream stream;
        stream << "loop_filter_ref_deltas" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.loop_filter_ref_deltas[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.loop_filter_ref_deltas, STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME, settings, indents + 1);
    dump_html_uint8_t(object.update_mode_delta, settings, "uint8_t", "update_mode_delta", indents + 1);
    dump_html_array_start(object.loop_filter_mode_deltas, STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS, settings, "int8_t[STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS]", "loop_filter_mode_deltas", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS; ++i) {
        std::stringstream stream;
        stream << "loop_filter_mode_deltas" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.loop_filter_mode_deltas[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.loop_filter_mode_deltas, STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1LoopFilter(const StdVideoAV1LoopFilter* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1LoopFilter(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1QuantizationFlags(const StdVideoAV1QuantizationFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.using_qmatrix, settings, "uint32_t: 1", "using_qmatrix", indents + 1);
    dump_html_uint32_t(object.diff_uv_delta, settings, "uint32_t: 1", "diff_uv_delta", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 30", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1QuantizationFlags(const StdVideoAV1QuantizationFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1QuantizationFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1Quantization(const StdVideoAV1Quantization& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoAV1QuantizationFlags(object.flags, settings, "StdVideoAV1QuantizationFlags", "flags", indents + 1);
    dump_html_uint8_t(object.base_q_idx, settings, "uint8_t", "base_q_idx", indents + 1);
    dump_html_int8_t(object.DeltaQYDc, settings, "int8_t", "DeltaQYDc", indents + 1);
    dump_html_int8_t(object.DeltaQUDc, settings, "int8_t", "DeltaQUDc", indents + 1);
    dump_html_int8_t(object.DeltaQUAc, settings, "int8_t", "DeltaQUAc", indents + 1);
    dump_html_int8_t(object.DeltaQVDc, settings, "int8_t", "DeltaQVDc", indents + 1);
    dump_html_int8_t(object.DeltaQVAc, settings, "int8_t", "DeltaQVAc", indents + 1);
    dump_html_uint8_t(object.qm_y, settings, "uint8_t", "qm_y", indents + 1);
    dump_html_uint8_t(object.qm_u, settings, "uint8_t", "qm_u", indents + 1);
    dump_html_uint8_t(object.qm_v, settings, "uint8_t", "qm_v", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1Quantization(const StdVideoAV1Quantization* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1Quantization(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1Segmentation(const StdVideoAV1Segmentation& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_array_start(object.FeatureEnabled, STD_VIDEO_AV1_MAX_SEGMENTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_SEGMENTS]", "FeatureEnabled", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_SEGMENTS; ++i) {
        std::stringstream stream;
        stream << "FeatureEnabled" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.FeatureEnabled[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.FeatureEnabled, STD_VIDEO_AV1_MAX_SEGMENTS, settings, indents + 1);
    dump_html_array_start(object.FeatureData, STD_VIDEO_AV1_MAX_SEGMENTS * STD_VIDEO_AV1_SEG_LVL_MAX, settings, "int16_t[STD_VIDEO_AV1_MAX_SEGMENTS][STD_VIDEO_AV1_SEG_LVL_MAX]", "FeatureData", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_SEGMENTS; ++i) {
        for (size_t j = 0; j < STD_VIDEO_AV1_SEG_LVL_MAX; ++j) {
            std::stringstream stream;
            stream << "FeatureData" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int16_t(object.FeatureData[i][j], settings, "int16_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.FeatureData, STD_VIDEO_AV1_MAX_SEGMENTS * STD_VIDEO_AV1_SEG_LVL_MAX, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1Segmentation(const StdVideoAV1Segmentation* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1Segmentation(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1TileInfoFlags(const StdVideoAV1TileInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.uniform_tile_spacing_flag, settings, "uint32_t: 1", "uniform_tile_spacing_flag", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 31", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1TileInfoFlags(const StdVideoAV1TileInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1TileInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1TileInfo(const StdVideoAV1TileInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoAV1TileInfoFlags(object.flags, settings, "StdVideoAV1TileInfoFlags", "flags", indents + 1);
    dump_html_uint8_t(object.TileCols, settings, "uint8_t", "TileCols", indents + 1);
    dump_html_uint8_t(object.TileRows, settings, "uint8_t", "TileRows", indents + 1);
    dump_html_uint16_t(object.context_update_tile_id, settings, "uint16_t", "context_update_tile_id", indents + 1);
    dump_html_uint8_t(object.tile_size_bytes_minus_1, settings, "uint8_t", "tile_size_bytes_minus_1", indents + 1);
    dump_html_array_start(object.reserved1, 7, settings, "uint8_t[7]", "reserved1", indents + 1);
    for (size_t i = 0; i < 7; ++i) {
        std::stringstream stream;
        stream << "reserved1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.reserved1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved1, 7, settings, indents + 1);
    dump_html_uint16_t(object.pMiColStarts, settings, "const uint16_t*", "pMiColStarts", indents + 1);
    dump_html_uint16_t(object.pMiRowStarts, settings, "const uint16_t*", "pMiRowStarts", indents + 1);
    dump_html_uint16_t(object.pWidthInSbsMinus1, settings, "const uint16_t*", "pWidthInSbsMinus1", indents + 1);
    dump_html_uint16_t(object.pHeightInSbsMinus1, settings, "const uint16_t*", "pHeightInSbsMinus1", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1TileInfo(const StdVideoAV1TileInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1TileInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1CDEF(const StdVideoAV1CDEF& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint8_t(object.cdef_damping_minus_3, settings, "uint8_t", "cdef_damping_minus_3", indents + 1);
    dump_html_uint8_t(object.cdef_bits, settings, "uint8_t", "cdef_bits", indents + 1);
    dump_html_array_start(object.cdef_y_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "cdef_y_pri_strength", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS; ++i) {
        std::stringstream stream;
        stream << "cdef_y_pri_strength" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.cdef_y_pri_strength[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cdef_y_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, indents + 1);
    dump_html_array_start(object.cdef_y_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "cdef_y_sec_strength", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS; ++i) {
        std::stringstream stream;
        stream << "cdef_y_sec_strength" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.cdef_y_sec_strength[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cdef_y_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, indents + 1);
    dump_html_array_start(object.cdef_uv_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "cdef_uv_pri_strength", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS; ++i) {
        std::stringstream stream;
        stream << "cdef_uv_pri_strength" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.cdef_uv_pri_strength[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cdef_uv_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, indents + 1);
    dump_html_array_start(object.cdef_uv_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "cdef_uv_sec_strength", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS; ++i) {
        std::stringstream stream;
        stream << "cdef_uv_sec_strength" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.cdef_uv_sec_strength[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.cdef_uv_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1CDEF(const StdVideoAV1CDEF* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1CDEF(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1LoopRestoration(const StdVideoAV1LoopRestoration& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_array_start(object.FrameRestorationType, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, "StdVideoAV1FrameRestorationType[STD_VIDEO_AV1_MAX_NUM_PLANES]", "FrameRestorationType", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_PLANES; ++i) {
        std::stringstream stream;
        stream << "FrameRestorationType" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_StdVideoAV1FrameRestorationType(object.FrameRestorationType[i], settings, "StdVideoAV1FrameRestorationType", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.FrameRestorationType, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, indents + 1);
    dump_html_array_start(object.LoopRestorationSize, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, "uint16_t[STD_VIDEO_AV1_MAX_NUM_PLANES]", "LoopRestorationSize", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_PLANES; ++i) {
        std::stringstream stream;
        stream << "LoopRestorationSize" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint16_t(object.LoopRestorationSize[i], settings, "uint16_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.LoopRestorationSize, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1LoopRestoration(const StdVideoAV1LoopRestoration* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1LoopRestoration(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1GlobalMotion(const StdVideoAV1GlobalMotion& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_array_start(object.GmType, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "GmType", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_NUM_REF_FRAMES; ++i) {
        std::stringstream stream;
        stream << "GmType" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.GmType[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.GmType, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, indents + 1);
    dump_html_array_start(object.gm_params, STD_VIDEO_AV1_NUM_REF_FRAMES * STD_VIDEO_AV1_GLOBAL_MOTION_PARAMS, settings, "int32_t[STD_VIDEO_AV1_NUM_REF_FRAMES][STD_VIDEO_AV1_GLOBAL_MOTION_PARAMS]", "gm_params", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_NUM_REF_FRAMES; ++i) {
        for (size_t j = 0; j < STD_VIDEO_AV1_GLOBAL_MOTION_PARAMS; ++j) {
            std::stringstream stream;
            stream << "gm_params" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int32_t(object.gm_params[i][j], settings, "int32_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.gm_params, STD_VIDEO_AV1_NUM_REF_FRAMES * STD_VIDEO_AV1_GLOBAL_MOTION_PARAMS, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1GlobalMotion(const StdVideoAV1GlobalMotion* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1GlobalMotion(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1FilmGrainFlags(const StdVideoAV1FilmGrainFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.chroma_scaling_from_luma, settings, "uint32_t: 1", "chroma_scaling_from_luma", indents + 1);
    dump_html_uint32_t(object.overlap_flag, settings, "uint32_t: 1", "overlap_flag", indents + 1);
    dump_html_uint32_t(object.clip_to_restricted_range, settings, "uint32_t: 1", "clip_to_restricted_range", indents + 1);
    dump_html_uint32_t(object.update_grain, settings, "uint32_t: 1", "update_grain", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 28", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1FilmGrainFlags(const StdVideoAV1FilmGrainFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1FilmGrainFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1FilmGrain(const StdVideoAV1FilmGrain& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoAV1FilmGrainFlags(object.flags, settings, "StdVideoAV1FilmGrainFlags", "flags", indents + 1);
    dump_html_uint8_t(object.grain_scaling_minus_8, settings, "uint8_t", "grain_scaling_minus_8", indents + 1);
    dump_html_uint8_t(object.ar_coeff_lag, settings, "uint8_t", "ar_coeff_lag", indents + 1);
    dump_html_uint8_t(object.ar_coeff_shift_minus_6, settings, "uint8_t", "ar_coeff_shift_minus_6", indents + 1);
    dump_html_uint8_t(object.grain_scale_shift, settings, "uint8_t", "grain_scale_shift", indents + 1);
    dump_html_uint16_t(object.grain_seed, settings, "uint16_t", "grain_seed", indents + 1);
    dump_html_uint8_t(object.film_grain_params_ref_idx, settings, "uint8_t", "film_grain_params_ref_idx", indents + 1);
    dump_html_uint8_t(object.num_y_points, settings, "uint8_t", "num_y_points", indents + 1);
    dump_html_array_start(object.point_y_value, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_Y_POINTS]", "point_y_value", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_Y_POINTS; ++i) {
        std::stringstream stream;
        stream << "point_y_value" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.point_y_value[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.point_y_value, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, indents + 1);
    dump_html_array_start(object.point_y_scaling, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_Y_POINTS]", "point_y_scaling", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_Y_POINTS; ++i) {
        std::stringstream stream;
        stream << "point_y_scaling" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.point_y_scaling[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.point_y_scaling, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, indents + 1);
    dump_html_uint8_t(object.num_cb_points, settings, "uint8_t", "num_cb_points", indents + 1);
    dump_html_array_start(object.point_cb_value, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CB_POINTS]", "point_cb_value", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_CB_POINTS; ++i) {
        std::stringstream stream;
        stream << "point_cb_value" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.point_cb_value[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.point_cb_value, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, indents + 1);
    dump_html_array_start(object.point_cb_scaling, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CB_POINTS]", "point_cb_scaling", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_CB_POINTS; ++i) {
        std::stringstream stream;
        stream << "point_cb_scaling" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.point_cb_scaling[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.point_cb_scaling, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, indents + 1);
    dump_html_uint8_t(object.num_cr_points, settings, "uint8_t", "num_cr_points", indents + 1);
    dump_html_array_start(object.point_cr_value, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CR_POINTS]", "point_cr_value", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_CR_POINTS; ++i) {
        std::stringstream stream;
        stream << "point_cr_value" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.point_cr_value[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.point_cr_value, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, indents + 1);
    dump_html_array_start(object.point_cr_scaling, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CR_POINTS]", "point_cr_scaling", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_CR_POINTS; ++i) {
        std::stringstream stream;
        stream << "point_cr_scaling" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.point_cr_scaling[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.point_cr_scaling, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, indents + 1);
    dump_html_array_start(object.ar_coeffs_y_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_LUMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_LUMA]", "ar_coeffs_y_plus_128", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_POS_LUMA; ++i) {
        std::stringstream stream;
        stream << "ar_coeffs_y_plus_128" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.ar_coeffs_y_plus_128[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.ar_coeffs_y_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_LUMA, settings, indents + 1);
    dump_html_array_start(object.ar_coeffs_cb_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_CHROMA]", "ar_coeffs_cb_plus_128", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_POS_CHROMA; ++i) {
        std::stringstream stream;
        stream << "ar_coeffs_cb_plus_128" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.ar_coeffs_cb_plus_128[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.ar_coeffs_cb_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, indents + 1);
    dump_html_array_start(object.ar_coeffs_cr_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_CHROMA]", "ar_coeffs_cr_plus_128", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_MAX_NUM_POS_CHROMA; ++i) {
        std::stringstream stream;
        stream << "ar_coeffs_cr_plus_128" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.ar_coeffs_cr_plus_128[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.ar_coeffs_cr_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, indents + 1);
    dump_html_uint8_t(object.cb_mult, settings, "uint8_t", "cb_mult", indents + 1);
    dump_html_uint8_t(object.cb_luma_mult, settings, "uint8_t", "cb_luma_mult", indents + 1);
    dump_html_uint16_t(object.cb_offset, settings, "uint16_t", "cb_offset", indents + 1);
    dump_html_uint8_t(object.cr_mult, settings, "uint8_t", "cr_mult", indents + 1);
    dump_html_uint8_t(object.cr_luma_mult, settings, "uint8_t", "cr_luma_mult", indents + 1);
    dump_html_uint16_t(object.cr_offset, settings, "uint16_t", "cr_offset", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1FilmGrain(const StdVideoAV1FilmGrain* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1FilmGrain(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1SequenceHeaderFlags(const StdVideoAV1SequenceHeaderFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.still_picture, settings, "uint32_t: 1", "still_picture", indents + 1);
    dump_html_uint32_t(object.reduced_still_picture_header, settings, "uint32_t: 1", "reduced_still_picture_header", indents + 1);
    dump_html_uint32_t(object.use_128x128_superblock, settings, "uint32_t: 1", "use_128x128_superblock", indents + 1);
    dump_html_uint32_t(object.enable_filter_intra, settings, "uint32_t: 1", "enable_filter_intra", indents + 1);
    dump_html_uint32_t(object.enable_intra_edge_filter, settings, "uint32_t: 1", "enable_intra_edge_filter", indents + 1);
    dump_html_uint32_t(object.enable_interintra_compound, settings, "uint32_t: 1", "enable_interintra_compound", indents + 1);
    dump_html_uint32_t(object.enable_masked_compound, settings, "uint32_t: 1", "enable_masked_compound", indents + 1);
    dump_html_uint32_t(object.enable_warped_motion, settings, "uint32_t: 1", "enable_warped_motion", indents + 1);
    dump_html_uint32_t(object.enable_dual_filter, settings, "uint32_t: 1", "enable_dual_filter", indents + 1);
    dump_html_uint32_t(object.enable_order_hint, settings, "uint32_t: 1", "enable_order_hint", indents + 1);
    dump_html_uint32_t(object.enable_jnt_comp, settings, "uint32_t: 1", "enable_jnt_comp", indents + 1);
    dump_html_uint32_t(object.enable_ref_frame_mvs, settings, "uint32_t: 1", "enable_ref_frame_mvs", indents + 1);
    dump_html_uint32_t(object.frame_id_numbers_present_flag, settings, "uint32_t: 1", "frame_id_numbers_present_flag", indents + 1);
    dump_html_uint32_t(object.enable_superres, settings, "uint32_t: 1", "enable_superres", indents + 1);
    dump_html_uint32_t(object.enable_cdef, settings, "uint32_t: 1", "enable_cdef", indents + 1);
    dump_html_uint32_t(object.enable_restoration, settings, "uint32_t: 1", "enable_restoration", indents + 1);
    dump_html_uint32_t(object.film_grain_params_present, settings, "uint32_t: 1", "film_grain_params_present", indents + 1);
    dump_html_uint32_t(object.timing_info_present_flag, settings, "uint32_t: 1", "timing_info_present_flag", indents + 1);
    dump_html_uint32_t(object.initial_display_delay_present_flag, settings, "uint32_t: 1", "initial_display_delay_present_flag", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 13", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1SequenceHeaderFlags(const StdVideoAV1SequenceHeaderFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1SequenceHeaderFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoAV1SequenceHeader(const StdVideoAV1SequenceHeader& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoAV1SequenceHeaderFlags(object.flags, settings, "StdVideoAV1SequenceHeaderFlags", "flags", indents + 1);
    dump_html_StdVideoAV1Profile(object.seq_profile, settings, "StdVideoAV1Profile", "seq_profile", indents + 1);
    dump_html_uint8_t(object.frame_width_bits_minus_1, settings, "uint8_t", "frame_width_bits_minus_1", indents + 1);
    dump_html_uint8_t(object.frame_height_bits_minus_1, settings, "uint8_t", "frame_height_bits_minus_1", indents + 1);
    dump_html_uint16_t(object.max_frame_width_minus_1, settings, "uint16_t", "max_frame_width_minus_1", indents + 1);
    dump_html_uint16_t(object.max_frame_height_minus_1, settings, "uint16_t", "max_frame_height_minus_1", indents + 1);
    dump_html_uint8_t(object.delta_frame_id_length_minus_2, settings, "uint8_t", "delta_frame_id_length_minus_2", indents + 1);
    dump_html_uint8_t(object.additional_frame_id_length_minus_1, settings, "uint8_t", "additional_frame_id_length_minus_1", indents + 1);
    dump_html_uint8_t(object.order_hint_bits_minus_1, settings, "uint8_t", "order_hint_bits_minus_1", indents + 1);
    dump_html_uint8_t(object.seq_force_integer_mv, settings, "uint8_t", "seq_force_integer_mv", indents + 1);
    dump_html_uint8_t(object.seq_force_screen_content_tools, settings, "uint8_t", "seq_force_screen_content_tools", indents + 1);
    dump_html_array_start(object.reserved1, 5, settings, "uint8_t[5]", "reserved1", indents + 1);
    for (size_t i = 0; i < 5; ++i) {
        std::stringstream stream;
        stream << "reserved1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.reserved1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved1, 5, settings, indents + 1);
    dump_html_StdVideoAV1ColorConfig(object.pColorConfig, settings, "const StdVideoAV1ColorConfig*", "pColorConfig", indents + 1);
    dump_html_StdVideoAV1TimingInfo(object.pTimingInfo, settings, "const StdVideoAV1TimingInfo*", "pTimingInfo", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoAV1SequenceHeader(const StdVideoAV1SequenceHeader* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoAV1SequenceHeader(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeAV1PictureInfoFlags(const StdVideoDecodeAV1PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.error_resilient_mode, settings, "uint32_t: 1", "error_resilient_mode", indents + 1);
    dump_html_uint32_t(object.disable_cdf_update, settings, "uint32_t: 1", "disable_cdf_update", indents + 1);
    dump_html_uint32_t(object.use_superres, settings, "uint32_t: 1", "use_superres", indents + 1);
    dump_html_uint32_t(object.render_and_frame_size_different, settings, "uint32_t: 1", "render_and_frame_size_different", indents + 1);
    dump_html_uint32_t(object.allow_screen_content_tools, settings, "uint32_t: 1", "allow_screen_content_tools", indents + 1);
    dump_html_uint32_t(object.is_filter_switchable, settings, "uint32_t: 1", "is_filter_switchable", indents + 1);
    dump_html_uint32_t(object.force_integer_mv, settings, "uint32_t: 1", "force_integer_mv", indents + 1);
    dump_html_uint32_t(object.frame_size_override_flag, settings, "uint32_t: 1", "frame_size_override_flag", indents + 1);
    dump_html_uint32_t(object.buffer_removal_time_present_flag, settings, "uint32_t: 1", "buffer_removal_time_present_flag", indents + 1);
    dump_html_uint32_t(object.allow_intrabc, settings, "uint32_t: 1", "allow_intrabc", indents + 1);
    dump_html_uint32_t(object.frame_refs_short_signaling, settings, "uint32_t: 1", "frame_refs_short_signaling", indents + 1);
    dump_html_uint32_t(object.allow_high_precision_mv, settings, "uint32_t: 1", "allow_high_precision_mv", indents + 1);
    dump_html_uint32_t(object.is_motion_mode_switchable, settings, "uint32_t: 1", "is_motion_mode_switchable", indents + 1);
    dump_html_uint32_t(object.use_ref_frame_mvs, settings, "uint32_t: 1", "use_ref_frame_mvs", indents + 1);
    dump_html_uint32_t(object.disable_frame_end_update_cdf, settings, "uint32_t: 1", "disable_frame_end_update_cdf", indents + 1);
    dump_html_uint32_t(object.allow_warped_motion, settings, "uint32_t: 1", "allow_warped_motion", indents + 1);
    dump_html_uint32_t(object.reduced_tx_set, settings, "uint32_t: 1", "reduced_tx_set", indents + 1);
    dump_html_uint32_t(object.reference_select, settings, "uint32_t: 1", "reference_select", indents + 1);
    dump_html_uint32_t(object.skip_mode_present, settings, "uint32_t: 1", "skip_mode_present", indents + 1);
    dump_html_uint32_t(object.delta_q_present, settings, "uint32_t: 1", "delta_q_present", indents + 1);
    dump_html_uint32_t(object.delta_lf_present, settings, "uint32_t: 1", "delta_lf_present", indents + 1);
    dump_html_uint32_t(object.delta_lf_multi, settings, "uint32_t: 1", "delta_lf_multi", indents + 1);
    dump_html_uint32_t(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + 1);
    dump_html_uint32_t(object.segmentation_update_map, settings, "uint32_t: 1", "segmentation_update_map", indents + 1);
    dump_html_uint32_t(object.segmentation_temporal_update, settings, "uint32_t: 1", "segmentation_temporal_update", indents + 1);
    dump_html_uint32_t(object.segmentation_update_data, settings, "uint32_t: 1", "segmentation_update_data", indents + 1);
    dump_html_uint32_t(object.UsesLr, settings, "uint32_t: 1", "UsesLr", indents + 1);
    dump_html_uint32_t(object.usesChromaLr, settings, "uint32_t: 1", "usesChromaLr", indents + 1);
    dump_html_uint32_t(object.apply_grain, settings, "uint32_t: 1", "apply_grain", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 3", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeAV1PictureInfoFlags(const StdVideoDecodeAV1PictureInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeAV1PictureInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeAV1PictureInfo(const StdVideoDecodeAV1PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoDecodeAV1PictureInfoFlags(object.flags, settings, "StdVideoDecodeAV1PictureInfoFlags", "flags", indents + 1);
    dump_html_StdVideoAV1FrameType(object.frame_type, settings, "StdVideoAV1FrameType", "frame_type", indents + 1);
    dump_html_uint32_t(object.current_frame_id, settings, "uint32_t", "current_frame_id", indents + 1);
    dump_html_uint8_t(object.OrderHint, settings, "uint8_t", "OrderHint", indents + 1);
    dump_html_uint8_t(object.primary_ref_frame, settings, "uint8_t", "primary_ref_frame", indents + 1);
    dump_html_uint8_t(object.refresh_frame_flags, settings, "uint8_t", "refresh_frame_flags", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_StdVideoAV1InterpolationFilter(object.interpolation_filter, settings, "StdVideoAV1InterpolationFilter", "interpolation_filter", indents + 1);
    dump_html_StdVideoAV1TxMode(object.TxMode, settings, "StdVideoAV1TxMode", "TxMode", indents + 1);
    dump_html_uint8_t(object.delta_q_res, settings, "uint8_t", "delta_q_res", indents + 1);
    dump_html_uint8_t(object.delta_lf_res, settings, "uint8_t", "delta_lf_res", indents + 1);
    dump_html_array_start(object.SkipModeFrame, STD_VIDEO_AV1_SKIP_MODE_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_SKIP_MODE_FRAMES]", "SkipModeFrame", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_SKIP_MODE_FRAMES; ++i) {
        std::stringstream stream;
        stream << "SkipModeFrame" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.SkipModeFrame[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.SkipModeFrame, STD_VIDEO_AV1_SKIP_MODE_FRAMES, settings, indents + 1);
    dump_html_uint8_t(object.coded_denom, settings, "uint8_t", "coded_denom", indents + 1);
    dump_html_array_start(object.reserved2, 3, settings, "uint8_t[3]", "reserved2", indents + 1);
    for (size_t i = 0; i < 3; ++i) {
        std::stringstream stream;
        stream << "reserved2" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.reserved2[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved2, 3, settings, indents + 1);
    dump_html_array_start(object.OrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "OrderHints", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_NUM_REF_FRAMES; ++i) {
        std::stringstream stream;
        stream << "OrderHints" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.OrderHints[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.OrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, indents + 1);
    dump_html_array_start(object.expectedFrameId, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint32_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "expectedFrameId", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_NUM_REF_FRAMES; ++i) {
        std::stringstream stream;
        stream << "expectedFrameId" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.expectedFrameId[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.expectedFrameId, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, indents + 1);
    dump_html_StdVideoAV1TileInfo(object.pTileInfo, settings, "const StdVideoAV1TileInfo*", "pTileInfo", indents + 1);
    dump_html_StdVideoAV1Quantization(object.pQuantization, settings, "const StdVideoAV1Quantization*", "pQuantization", indents + 1);
    dump_html_StdVideoAV1Segmentation(object.pSegmentation, settings, "const StdVideoAV1Segmentation*", "pSegmentation", indents + 1);
    dump_html_StdVideoAV1LoopFilter(object.pLoopFilter, settings, "const StdVideoAV1LoopFilter*", "pLoopFilter", indents + 1);
    dump_html_StdVideoAV1CDEF(object.pCDEF, settings, "const StdVideoAV1CDEF*", "pCDEF", indents + 1);
    dump_html_StdVideoAV1LoopRestoration(object.pLoopRestoration, settings, "const StdVideoAV1LoopRestoration*", "pLoopRestoration", indents + 1);
    dump_html_StdVideoAV1GlobalMotion(object.pGlobalMotion, settings, "const StdVideoAV1GlobalMotion*", "pGlobalMotion", indents + 1);
    dump_html_StdVideoAV1FilmGrain(object.pFilmGrain, settings, "const StdVideoAV1FilmGrain*", "pFilmGrain", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeAV1PictureInfo(const StdVideoDecodeAV1PictureInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeAV1PictureInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeAV1ReferenceInfoFlags(const StdVideoDecodeAV1ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.disable_frame_end_update_cdf, settings, "uint32_t: 1", "disable_frame_end_update_cdf", indents + 1);
    dump_html_uint32_t(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 30", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeAV1ReferenceInfoFlags(const StdVideoDecodeAV1ReferenceInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeAV1ReferenceInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeAV1ReferenceInfo(const StdVideoDecodeAV1ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoDecodeAV1ReferenceInfoFlags(object.flags, settings, "StdVideoDecodeAV1ReferenceInfoFlags", "flags", indents + 1);
    dump_html_uint8_t(object.frame_type, settings, "uint8_t", "frame_type", indents + 1);
    dump_html_uint8_t(object.RefFrameSignBias, settings, "uint8_t", "RefFrameSignBias", indents + 1);
    dump_html_uint8_t(object.OrderHint, settings, "uint8_t", "OrderHint", indents + 1);
    dump_html_array_start(object.SavedOrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "SavedOrderHints", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_NUM_REF_FRAMES; ++i) {
        std::stringstream stream;
        stream << "SavedOrderHints" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.SavedOrderHints[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.SavedOrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeAV1ReferenceInfo(const StdVideoDecodeAV1ReferenceInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeAV1ReferenceInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeAV1DecoderModelInfo(const StdVideoEncodeAV1DecoderModelInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint8_t(object.buffer_delay_length_minus_1, settings, "uint8_t", "buffer_delay_length_minus_1", indents + 1);
    dump_html_uint8_t(object.buffer_removal_time_length_minus_1, settings, "uint8_t", "buffer_removal_time_length_minus_1", indents + 1);
    dump_html_uint8_t(object.frame_presentation_time_length_minus_1, settings, "uint8_t", "frame_presentation_time_length_minus_1", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_uint32_t(object.num_units_in_decoding_tick, settings, "uint32_t", "num_units_in_decoding_tick", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeAV1DecoderModelInfo(const StdVideoEncodeAV1DecoderModelInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeAV1DecoderModelInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeAV1ExtensionHeader(const StdVideoEncodeAV1ExtensionHeader& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint8_t(object.temporal_id, settings, "uint8_t", "temporal_id", indents + 1);
    dump_html_uint8_t(object.spatial_id, settings, "uint8_t", "spatial_id", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeAV1ExtensionHeader(const StdVideoEncodeAV1ExtensionHeader* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeAV1ExtensionHeader(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeAV1OperatingPointInfoFlags(const StdVideoEncodeAV1OperatingPointInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.decoder_model_present_for_this_op, settings, "uint32_t: 1", "decoder_model_present_for_this_op", indents + 1);
    dump_html_uint32_t(object.low_delay_mode_flag, settings, "uint32_t: 1", "low_delay_mode_flag", indents + 1);
    dump_html_uint32_t(object.initial_display_delay_present_for_this_op, settings, "uint32_t: 1", "initial_display_delay_present_for_this_op", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 29", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeAV1OperatingPointInfoFlags(const StdVideoEncodeAV1OperatingPointInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeAV1OperatingPointInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeAV1OperatingPointInfo(const StdVideoEncodeAV1OperatingPointInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeAV1OperatingPointInfoFlags(object.flags, settings, "StdVideoEncodeAV1OperatingPointInfoFlags", "flags", indents + 1);
    dump_html_uint16_t(object.operating_point_idc, settings, "uint16_t", "operating_point_idc", indents + 1);
    dump_html_uint8_t(object.seq_level_idx, settings, "uint8_t", "seq_level_idx", indents + 1);
    dump_html_uint8_t(object.seq_tier, settings, "uint8_t", "seq_tier", indents + 1);
    dump_html_uint32_t(object.decoder_buffer_delay, settings, "uint32_t", "decoder_buffer_delay", indents + 1);
    dump_html_uint32_t(object.encoder_buffer_delay, settings, "uint32_t", "encoder_buffer_delay", indents + 1);
    dump_html_uint8_t(object.initial_display_delay_minus_1, settings, "uint8_t", "initial_display_delay_minus_1", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeAV1OperatingPointInfo(const StdVideoEncodeAV1OperatingPointInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeAV1OperatingPointInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeAV1PictureInfoFlags(const StdVideoEncodeAV1PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.error_resilient_mode, settings, "uint32_t: 1", "error_resilient_mode", indents + 1);
    dump_html_uint32_t(object.disable_cdf_update, settings, "uint32_t: 1", "disable_cdf_update", indents + 1);
    dump_html_uint32_t(object.use_superres, settings, "uint32_t: 1", "use_superres", indents + 1);
    dump_html_uint32_t(object.render_and_frame_size_different, settings, "uint32_t: 1", "render_and_frame_size_different", indents + 1);
    dump_html_uint32_t(object.allow_screen_content_tools, settings, "uint32_t: 1", "allow_screen_content_tools", indents + 1);
    dump_html_uint32_t(object.is_filter_switchable, settings, "uint32_t: 1", "is_filter_switchable", indents + 1);
    dump_html_uint32_t(object.force_integer_mv, settings, "uint32_t: 1", "force_integer_mv", indents + 1);
    dump_html_uint32_t(object.frame_size_override_flag, settings, "uint32_t: 1", "frame_size_override_flag", indents + 1);
    dump_html_uint32_t(object.buffer_removal_time_present_flag, settings, "uint32_t: 1", "buffer_removal_time_present_flag", indents + 1);
    dump_html_uint32_t(object.allow_intrabc, settings, "uint32_t: 1", "allow_intrabc", indents + 1);
    dump_html_uint32_t(object.frame_refs_short_signaling, settings, "uint32_t: 1", "frame_refs_short_signaling", indents + 1);
    dump_html_uint32_t(object.allow_high_precision_mv, settings, "uint32_t: 1", "allow_high_precision_mv", indents + 1);
    dump_html_uint32_t(object.is_motion_mode_switchable, settings, "uint32_t: 1", "is_motion_mode_switchable", indents + 1);
    dump_html_uint32_t(object.use_ref_frame_mvs, settings, "uint32_t: 1", "use_ref_frame_mvs", indents + 1);
    dump_html_uint32_t(object.disable_frame_end_update_cdf, settings, "uint32_t: 1", "disable_frame_end_update_cdf", indents + 1);
    dump_html_uint32_t(object.allow_warped_motion, settings, "uint32_t: 1", "allow_warped_motion", indents + 1);
    dump_html_uint32_t(object.reduced_tx_set, settings, "uint32_t: 1", "reduced_tx_set", indents + 1);
    dump_html_uint32_t(object.skip_mode_present, settings, "uint32_t: 1", "skip_mode_present", indents + 1);
    dump_html_uint32_t(object.delta_q_present, settings, "uint32_t: 1", "delta_q_present", indents + 1);
    dump_html_uint32_t(object.delta_lf_present, settings, "uint32_t: 1", "delta_lf_present", indents + 1);
    dump_html_uint32_t(object.delta_lf_multi, settings, "uint32_t: 1", "delta_lf_multi", indents + 1);
    dump_html_uint32_t(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + 1);
    dump_html_uint32_t(object.segmentation_update_map, settings, "uint32_t: 1", "segmentation_update_map", indents + 1);
    dump_html_uint32_t(object.segmentation_temporal_update, settings, "uint32_t: 1", "segmentation_temporal_update", indents + 1);
    dump_html_uint32_t(object.segmentation_update_data, settings, "uint32_t: 1", "segmentation_update_data", indents + 1);
    dump_html_uint32_t(object.UsesLr, settings, "uint32_t: 1", "UsesLr", indents + 1);
    dump_html_uint32_t(object.usesChromaLr, settings, "uint32_t: 1", "usesChromaLr", indents + 1);
    dump_html_uint32_t(object.show_frame, settings, "uint32_t: 1", "show_frame", indents + 1);
    dump_html_uint32_t(object.showable_frame, settings, "uint32_t: 1", "showable_frame", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 3", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeAV1PictureInfoFlags(const StdVideoEncodeAV1PictureInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeAV1PictureInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeAV1PictureInfo(const StdVideoEncodeAV1PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeAV1PictureInfoFlags(object.flags, settings, "StdVideoEncodeAV1PictureInfoFlags", "flags", indents + 1);
    dump_html_StdVideoAV1FrameType(object.frame_type, settings, "StdVideoAV1FrameType", "frame_type", indents + 1);
    dump_html_uint32_t(object.frame_presentation_time, settings, "uint32_t", "frame_presentation_time", indents + 1);
    dump_html_uint32_t(object.current_frame_id, settings, "uint32_t", "current_frame_id", indents + 1);
    dump_html_uint8_t(object.order_hint, settings, "uint8_t", "order_hint", indents + 1);
    dump_html_uint8_t(object.primary_ref_frame, settings, "uint8_t", "primary_ref_frame", indents + 1);
    dump_html_uint8_t(object.refresh_frame_flags, settings, "uint8_t", "refresh_frame_flags", indents + 1);
    dump_html_uint8_t(object.coded_denom, settings, "uint8_t", "coded_denom", indents + 1);
    dump_html_uint16_t(object.render_width_minus_1, settings, "uint16_t", "render_width_minus_1", indents + 1);
    dump_html_uint16_t(object.render_height_minus_1, settings, "uint16_t", "render_height_minus_1", indents + 1);
    dump_html_StdVideoAV1InterpolationFilter(object.interpolation_filter, settings, "StdVideoAV1InterpolationFilter", "interpolation_filter", indents + 1);
    dump_html_StdVideoAV1TxMode(object.TxMode, settings, "StdVideoAV1TxMode", "TxMode", indents + 1);
    dump_html_uint8_t(object.delta_q_res, settings, "uint8_t", "delta_q_res", indents + 1);
    dump_html_uint8_t(object.delta_lf_res, settings, "uint8_t", "delta_lf_res", indents + 1);
    dump_html_array_start(object.ref_order_hint, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "ref_order_hint", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_NUM_REF_FRAMES; ++i) {
        std::stringstream stream;
        stream << "ref_order_hint" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.ref_order_hint[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.ref_order_hint, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, indents + 1);
    dump_html_array_start(object.ref_frame_idx, STD_VIDEO_AV1_REFS_PER_FRAME, settings, "int8_t[STD_VIDEO_AV1_REFS_PER_FRAME]", "ref_frame_idx", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_REFS_PER_FRAME; ++i) {
        std::stringstream stream;
        stream << "ref_frame_idx" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.ref_frame_idx[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.ref_frame_idx, STD_VIDEO_AV1_REFS_PER_FRAME, settings, indents + 1);
    dump_html_array_start(object.reserved1, 3, settings, "uint8_t[3]", "reserved1", indents + 1);
    for (size_t i = 0; i < 3; ++i) {
        std::stringstream stream;
        stream << "reserved1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.reserved1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved1, 3, settings, indents + 1);
    dump_html_array_start(object.delta_frame_id_minus_1, STD_VIDEO_AV1_REFS_PER_FRAME, settings, "uint32_t[STD_VIDEO_AV1_REFS_PER_FRAME]", "delta_frame_id_minus_1", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_AV1_REFS_PER_FRAME; ++i) {
        std::stringstream stream;
        stream << "delta_frame_id_minus_1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint32_t(object.delta_frame_id_minus_1[i], settings, "uint32_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.delta_frame_id_minus_1, STD_VIDEO_AV1_REFS_PER_FRAME, settings, indents + 1);
    dump_html_StdVideoAV1TileInfo(object.pTileInfo, settings, "const StdVideoAV1TileInfo*", "pTileInfo", indents + 1);
    dump_html_StdVideoAV1Quantization(object.pQuantization, settings, "const StdVideoAV1Quantization*", "pQuantization", indents + 1);
    dump_html_StdVideoAV1Segmentation(object.pSegmentation, settings, "const StdVideoAV1Segmentation*", "pSegmentation", indents + 1);
    dump_html_StdVideoAV1LoopFilter(object.pLoopFilter, settings, "const StdVideoAV1LoopFilter*", "pLoopFilter", indents + 1);
    dump_html_StdVideoAV1CDEF(object.pCDEF, settings, "const StdVideoAV1CDEF*", "pCDEF", indents + 1);
    dump_html_StdVideoAV1LoopRestoration(object.pLoopRestoration, settings, "const StdVideoAV1LoopRestoration*", "pLoopRestoration", indents + 1);
    dump_html_StdVideoAV1GlobalMotion(object.pGlobalMotion, settings, "const StdVideoAV1GlobalMotion*", "pGlobalMotion", indents + 1);
    dump_html_StdVideoEncodeAV1ExtensionHeader(object.pExtensionHeader, settings, "const StdVideoEncodeAV1ExtensionHeader*", "pExtensionHeader", indents + 1);
    dump_html_uint32_t(object.pBufferRemovalTimes, settings, "const uint32_t*", "pBufferRemovalTimes", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeAV1PictureInfo(const StdVideoEncodeAV1PictureInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeAV1PictureInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeAV1ReferenceInfoFlags(const StdVideoEncodeAV1ReferenceInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.disable_frame_end_update_cdf, settings, "uint32_t: 1", "disable_frame_end_update_cdf", indents + 1);
    dump_html_uint32_t(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 30", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeAV1ReferenceInfoFlags(const StdVideoEncodeAV1ReferenceInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeAV1ReferenceInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoEncodeAV1ReferenceInfo(const StdVideoEncodeAV1ReferenceInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoEncodeAV1ReferenceInfoFlags(object.flags, settings, "StdVideoEncodeAV1ReferenceInfoFlags", "flags", indents + 1);
    dump_html_uint32_t(object.RefFrameId, settings, "uint32_t", "RefFrameId", indents + 1);
    dump_html_StdVideoAV1FrameType(object.frame_type, settings, "StdVideoAV1FrameType", "frame_type", indents + 1);
    dump_html_uint8_t(object.OrderHint, settings, "uint8_t", "OrderHint", indents + 1);
    dump_html_array_start(object.reserved1, 3, settings, "uint8_t[3]", "reserved1", indents + 1);
    for (size_t i = 0; i < 3; ++i) {
        std::stringstream stream;
        stream << "reserved1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.reserved1[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved1, 3, settings, indents + 1);
    dump_html_StdVideoEncodeAV1ExtensionHeader(object.pExtensionHeader, settings, "const StdVideoEncodeAV1ExtensionHeader*", "pExtensionHeader", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoEncodeAV1ReferenceInfo(const StdVideoEncodeAV1ReferenceInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoEncodeAV1ReferenceInfo(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9ColorConfigFlags(const StdVideoVP9ColorConfigFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.color_range, settings, "uint32_t: 1", "color_range", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 31", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoVP9ColorConfigFlags(const StdVideoVP9ColorConfigFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9ColorConfigFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9ColorConfig(const StdVideoVP9ColorConfig& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoVP9ColorConfigFlags(object.flags, settings, "StdVideoVP9ColorConfigFlags", "flags", indents + 1);
    dump_html_uint8_t(object.BitDepth, settings, "uint8_t", "BitDepth", indents + 1);
    dump_html_uint8_t(object.subsampling_x, settings, "uint8_t", "subsampling_x", indents + 1);
    dump_html_uint8_t(object.subsampling_y, settings, "uint8_t", "subsampling_y", indents + 1);
    dump_html_uint8_t(object.reserved1, settings, "uint8_t", "reserved1", indents + 1);
    dump_html_StdVideoVP9ColorSpace(object.color_space, settings, "StdVideoVP9ColorSpace", "color_space", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoVP9ColorConfig(const StdVideoVP9ColorConfig* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9ColorConfig(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9LoopFilterFlags(const StdVideoVP9LoopFilterFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.loop_filter_delta_enabled, settings, "uint32_t: 1", "loop_filter_delta_enabled", indents + 1);
    dump_html_uint32_t(object.loop_filter_delta_update, settings, "uint32_t: 1", "loop_filter_delta_update", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 30", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoVP9LoopFilterFlags(const StdVideoVP9LoopFilterFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9LoopFilterFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9LoopFilter(const StdVideoVP9LoopFilter& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoVP9LoopFilterFlags(object.flags, settings, "StdVideoVP9LoopFilterFlags", "flags", indents + 1);
    dump_html_uint8_t(object.loop_filter_level, settings, "uint8_t", "loop_filter_level", indents + 1);
    dump_html_uint8_t(object.loop_filter_sharpness, settings, "uint8_t", "loop_filter_sharpness", indents + 1);
    dump_html_uint8_t(object.update_ref_delta, settings, "uint8_t", "update_ref_delta", indents + 1);
    dump_html_array_start(object.loop_filter_ref_deltas, STD_VIDEO_VP9_MAX_REF_FRAMES, settings, "int8_t[STD_VIDEO_VP9_MAX_REF_FRAMES]", "loop_filter_ref_deltas", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_VP9_MAX_REF_FRAMES; ++i) {
        std::stringstream stream;
        stream << "loop_filter_ref_deltas" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.loop_filter_ref_deltas[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.loop_filter_ref_deltas, STD_VIDEO_VP9_MAX_REF_FRAMES, settings, indents + 1);
    dump_html_uint8_t(object.update_mode_delta, settings, "uint8_t", "update_mode_delta", indents + 1);
    dump_html_array_start(object.loop_filter_mode_deltas, STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS, settings, "int8_t[STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS]", "loop_filter_mode_deltas", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS; ++i) {
        std::stringstream stream;
        stream << "loop_filter_mode_deltas" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_int8_t(object.loop_filter_mode_deltas[i], settings, "int8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.loop_filter_mode_deltas, STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoVP9LoopFilter(const StdVideoVP9LoopFilter* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9LoopFilter(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9SegmentationFlags(const StdVideoVP9SegmentationFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.segmentation_update_map, settings, "uint32_t: 1", "segmentation_update_map", indents + 1);
    dump_html_uint32_t(object.segmentation_temporal_update, settings, "uint32_t: 1", "segmentation_temporal_update", indents + 1);
    dump_html_uint32_t(object.segmentation_update_data, settings, "uint32_t: 1", "segmentation_update_data", indents + 1);
    dump_html_uint32_t(object.segmentation_abs_or_delta_update, settings, "uint32_t: 1", "segmentation_abs_or_delta_update", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 28", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoVP9SegmentationFlags(const StdVideoVP9SegmentationFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9SegmentationFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoVP9Segmentation(const StdVideoVP9Segmentation& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoVP9SegmentationFlags(object.flags, settings, "StdVideoVP9SegmentationFlags", "flags", indents + 1);
    dump_html_array_start(object.segmentation_tree_probs, STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS]", "segmentation_tree_probs", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS; ++i) {
        std::stringstream stream;
        stream << "segmentation_tree_probs" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.segmentation_tree_probs[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.segmentation_tree_probs, STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS, settings, indents + 1);
    dump_html_array_start(object.segmentation_pred_prob, STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB]", "segmentation_pred_prob", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB; ++i) {
        std::stringstream stream;
        stream << "segmentation_pred_prob" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.segmentation_pred_prob[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.segmentation_pred_prob, STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB, settings, indents + 1);
    dump_html_array_start(object.FeatureEnabled, STD_VIDEO_VP9_MAX_SEGMENTS, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTS]", "FeatureEnabled", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_VP9_MAX_SEGMENTS; ++i) {
        std::stringstream stream;
        stream << "FeatureEnabled" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint8_t(object.FeatureEnabled[i], settings, "uint8_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.FeatureEnabled, STD_VIDEO_VP9_MAX_SEGMENTS, settings, indents + 1);
    dump_html_array_start(object.FeatureData, STD_VIDEO_VP9_MAX_SEGMENTS * STD_VIDEO_VP9_SEG_LVL_MAX, settings, "int16_t[STD_VIDEO_VP9_MAX_SEGMENTS][STD_VIDEO_VP9_SEG_LVL_MAX]", "FeatureData", indents + 1);
    for (size_t i = 0; i < STD_VIDEO_VP9_MAX_SEGMENTS; ++i) {
        for (size_t j = 0; j < STD_VIDEO_VP9_SEG_LVL_MAX; ++j) {
            std::stringstream stream;
            stream << "FeatureData" << "[" << i << "][" << j << "]";
            std::string indexName = stream.str();
            dump_html_int16_t(object.FeatureData[i][j], settings, "int16_t", indexName.c_str(), indents + 1 + 1);
        }
    }
    dump_html_array_end(object.FeatureData, STD_VIDEO_VP9_MAX_SEGMENTS * STD_VIDEO_VP9_SEG_LVL_MAX, settings, indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoVP9Segmentation(const StdVideoVP9Segmentation* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoVP9Segmentation(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeVP9PictureInfoFlags(const StdVideoDecodeVP9PictureInfoFlags& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_uint32_t(object.error_resilient_mode, settings, "uint32_t: 1", "error_resilient_mode", indents + 1);
    dump_html_uint32_t(object.intra_only, settings, "uint32_t: 1", "intra_only", indents + 1);
    dump_html_uint32_t(object.allow_high_precision_mv, settings, "uint32_t: 1", "allow_high_precision_mv", indents + 1);
    dump_html_uint32_t(object.refresh_frame_context, settings, "uint32_t: 1", "refresh_frame_context", indents + 1);
    dump_html_uint32_t(object.frame_parallel_decoding_mode, settings, "uint32_t: 1", "frame_parallel_decoding_mode", indents + 1);
    dump_html_uint32_t(object.segmentation_enabled, settings, "uint32_t: 1", "segmentation_enabled", indents + 1);
    dump_html_uint32_t(object.show_frame, settings, "uint32_t: 1", "show_frame", indents + 1);
    dump_html_uint32_t(object.UsePrevFrameMvs, settings, "uint32_t: 1", "UsePrevFrameMvs", indents + 1);
    dump_html_uint32_t(object.reserved, settings, "uint32_t: 24", "reserved", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeVP9PictureInfoFlags(const StdVideoDecodeVP9PictureInfoFlags* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeVP9PictureInfoFlags(*object, settings, type_string, name, indents, object);
    }
}
void dump_html_StdVideoDecodeVP9PictureInfo(const StdVideoDecodeVP9PictureInfo& object, const ApiDumpSettings& settings, const char* type_name, const char* var_name, int indents, const void* address = nullptr) {
    dump_html_start(settings, OutputConstruct::api_struct, type_name, var_name, indents, address);
    dump_html_StdVideoDecodeVP9PictureInfoFlags(object.flags, settings, "StdVideoDecodeVP9PictureInfoFlags", "flags", indents + 1);
    dump_html_StdVideoVP9Profile(object.profile, settings, "StdVideoVP9Profile", "profile", indents + 1);
    dump_html_StdVideoVP9FrameType(object.frame_type, settings, "StdVideoVP9FrameType", "frame_type", indents + 1);
    dump_html_uint8_t(object.frame_context_idx, settings, "uint8_t", "frame_context_idx", indents + 1);
    dump_html_uint8_t(object.reset_frame_context, settings, "uint8_t", "reset_frame_context", indents + 1);
    dump_html_uint8_t(object.refresh_frame_flags, settings, "uint8_t", "refresh_frame_flags", indents + 1);
    dump_html_uint8_t(object.ref_frame_sign_bias_mask, settings, "uint8_t", "ref_frame_sign_bias_mask", indents + 1);
    dump_html_StdVideoVP9InterpolationFilter(object.interpolation_filter, settings, "StdVideoVP9InterpolationFilter", "interpolation_filter", indents + 1);
    dump_html_uint8_t(object.base_q_idx, settings, "uint8_t", "base_q_idx", indents + 1);
    dump_html_int8_t(object.delta_q_y_dc, settings, "int8_t", "delta_q_y_dc", indents + 1);
    dump_html_int8_t(object.delta_q_uv_dc, settings, "int8_t", "delta_q_uv_dc", indents + 1);
    dump_html_int8_t(object.delta_q_uv_ac, settings, "int8_t", "delta_q_uv_ac", indents + 1);
    dump_html_uint8_t(object.tile_cols_log2, settings, "uint8_t", "tile_cols_log2", indents + 1);
    dump_html_uint8_t(object.tile_rows_log2, settings, "uint8_t", "tile_rows_log2", indents + 1);
    dump_html_array_start(object.reserved1, 3, settings, "uint16_t[3]", "reserved1", indents + 1);
    for (size_t i = 0; i < 3; ++i) {
        std::stringstream stream;
        stream << "reserved1" << "[" << i << "]";
        std::string indexName = stream.str();
        dump_html_uint16_t(object.reserved1[i], settings, "uint16_t", indexName.c_str(), indents + 1 + 1);
    }
    dump_html_array_end(object.reserved1, 3, settings, indents + 1);
    dump_html_StdVideoVP9ColorConfig(object.pColorConfig, settings, "const StdVideoVP9ColorConfig*", "pColorConfig", indents + 1);
    dump_html_StdVideoVP9LoopFilter(object.pLoopFilter, settings, "const StdVideoVP9LoopFilter*", "pLoopFilter", indents + 1);
    dump_html_StdVideoVP9Segmentation(object.pSegmentation, settings, "const StdVideoVP9Segmentation*", "pSegmentation", indents + 1);
    dump_html_end(settings, OutputConstruct::api_struct, indents);
}

void dump_html_StdVideoDecodeVP9PictureInfo(const StdVideoDecodeVP9PictureInfo* object, const ApiDumpSettings& settings, const char* type_string, const char* name, int indents) {
    if (object == NULL) {
        dump_html_nullptr(settings, type_string, name, indents);
    } else {
        dump_html_StdVideoDecodeVP9PictureInfo(*object, settings, type_string, name, indents, object);
    }
}

//========================== Union Implementations ==========================//

//======================== pNext Chain Implementation =======================//

//========================== Function Helpers ===============================//

//========================= Function Implementations ========================//
