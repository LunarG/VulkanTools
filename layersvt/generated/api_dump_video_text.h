
/* Copyright (c) 2015-2023 Valve Corporation
 * Copyright (c) 2015-2023 LunarG, Inc.
 * Copyright (c) 2015-2016, 2019 Google Inc.
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
 * Author: Lenny Komow <lenny@lunarg.com>
 * Author: Shannon McPherson <shannon@lunarg.com>
 * Author: Charles Giessen <charles@lunarg.com>
 */

/*
 * This file is generated from the Khronos Vulkan XML API Registry.
 */

#pragma once

#include "api_dump_text.h"


//=========================== Type Implementations ==========================//

void dump_text_uint32_t(uint32_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << object;
}
void dump_text_uint8_t(uint8_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << (uint32_t) object;
}
void dump_text_uint16_t(uint16_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << object;
}
void dump_text_int32_t(int32_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << object;
}
void dump_text_int8_t(int8_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << (int32_t) object;
}
void dump_text_int16_t(int16_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << object;
}

//========================= Basetype Implementations ========================//






//======================= System Type Implementations =======================//


//========================== Handle Implementations =========================//


//=========================== Enum Implementations ==========================//

void dump_text_StdVideoH264ChromaFormatIdc(StdVideoH264ChromaFormatIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_CHROMA_FORMAT_IDC_MONOCHROME (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_CHROMA_FORMAT_IDC_420 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_CHROMA_FORMAT_IDC_422 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H264_CHROMA_FORMAT_IDC_444 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_CHROMA_FORMAT_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264ProfileIdc(StdVideoH264ProfileIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 66:
        settings.stream() << "STD_VIDEO_H264_PROFILE_IDC_BASELINE (";
        break;
    case 77:
        settings.stream() << "STD_VIDEO_H264_PROFILE_IDC_MAIN (";
        break;
    case 100:
        settings.stream() << "STD_VIDEO_H264_PROFILE_IDC_HIGH (";
        break;
    case 244:
        settings.stream() << "STD_VIDEO_H264_PROFILE_IDC_HIGH_444_PREDICTIVE (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_PROFILE_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264LevelIdc(StdVideoH264LevelIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_1_0 (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_1_1 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_1_2 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_1_3 (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_2_0 (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_2_1 (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_2_2 (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_3_0 (";
        break;
    case 8:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_3_1 (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_3_2 (";
        break;
    case 10:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_4_0 (";
        break;
    case 11:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_4_1 (";
        break;
    case 12:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_4_2 (";
        break;
    case 13:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_5_0 (";
        break;
    case 14:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_5_1 (";
        break;
    case 15:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_5_2 (";
        break;
    case 16:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_6_0 (";
        break;
    case 17:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_6_1 (";
        break;
    case 18:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_6_2 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_LEVEL_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264PocType(StdVideoH264PocType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_POC_TYPE_0 (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_POC_TYPE_1 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_POC_TYPE_2 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_POC_TYPE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264AspectRatioIdc(StdVideoH264AspectRatioIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_UNSPECIFIED (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_SQUARE (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_12_11 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_10_11 (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_16_11 (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_40_33 (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_24_11 (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_20_11 (";
        break;
    case 8:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_32_11 (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_80_33 (";
        break;
    case 10:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_18_11 (";
        break;
    case 11:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_15_11 (";
        break;
    case 12:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_64_33 (";
        break;
    case 13:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_160_99 (";
        break;
    case 14:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_4_3 (";
        break;
    case 15:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_3_2 (";
        break;
    case 16:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_2_1 (";
        break;
    case 255:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_EXTENDED_SAR (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_ASPECT_RATIO_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264WeightedBipredIdc(StdVideoH264WeightedBipredIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_DEFAULT (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_EXPLICIT (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_IMPLICIT (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264ModificationOfPicNumsIdc(StdVideoH264ModificationOfPicNumsIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_SUBTRACT (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_ADD (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_LONG_TERM (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_END (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264MemMgmtControlOp(StdVideoH264MemMgmtControlOp object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_END (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_SHORT_TERM (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_LONG_TERM (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_LONG_TERM (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_SET_MAX_LONG_TERM_INDEX (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_ALL (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_CURRENT_AS_LONG_TERM (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264CabacInitIdc(StdVideoH264CabacInitIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_CABAC_INIT_IDC_0 (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_CABAC_INIT_IDC_1 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_CABAC_INIT_IDC_2 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_CABAC_INIT_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264DisableDeblockingFilterIdc(StdVideoH264DisableDeblockingFilterIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_DISABLED (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_ENABLED (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_PARTIAL (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264SliceType(StdVideoH264SliceType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_SLICE_TYPE_P (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_SLICE_TYPE_B (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_SLICE_TYPE_I (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_SLICE_TYPE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264PictureType(StdVideoH264PictureType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_PICTURE_TYPE_P (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_PICTURE_TYPE_B (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_PICTURE_TYPE_I (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_H264_PICTURE_TYPE_IDR (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_PICTURE_TYPE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH264NonVclNaluType(StdVideoH264NonVclNaluType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H264_NON_VCL_NALU_TYPE_SPS (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H264_NON_VCL_NALU_TYPE_PPS (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H264_NON_VCL_NALU_TYPE_AUD (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H264_NON_VCL_NALU_TYPE_PREFIX (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_SEQUENCE (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_STREAM (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_H264_NON_VCL_NALU_TYPE_PRECODED (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H264_NON_VCL_NALU_TYPE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoDecodeH264FieldOrderCount(StdVideoDecodeH264FieldOrderCount object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_TOP (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_BOTTOM (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH265ChromaFormatIdc(StdVideoH265ChromaFormatIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H265_CHROMA_FORMAT_IDC_MONOCHROME (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H265_CHROMA_FORMAT_IDC_420 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H265_CHROMA_FORMAT_IDC_422 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H265_CHROMA_FORMAT_IDC_444 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H265_CHROMA_FORMAT_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH265ProfileIdc(StdVideoH265ProfileIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 1:
        settings.stream() << "STD_VIDEO_H265_PROFILE_IDC_MAIN (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H265_PROFILE_IDC_MAIN_10 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H265_PROFILE_IDC_MAIN_STILL_PICTURE (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_H265_PROFILE_IDC_FORMAT_RANGE_EXTENSIONS (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_H265_PROFILE_IDC_SCC_EXTENSIONS (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H265_PROFILE_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH265LevelIdc(StdVideoH265LevelIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_1_0 (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_2_0 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_2_1 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_3_0 (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_3_1 (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_4_0 (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_4_1 (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_5_0 (";
        break;
    case 8:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_5_1 (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_5_2 (";
        break;
    case 10:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_6_0 (";
        break;
    case 11:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_6_1 (";
        break;
    case 12:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_6_2 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H265_LEVEL_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH265SliceType(StdVideoH265SliceType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H265_SLICE_TYPE_B (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H265_SLICE_TYPE_P (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H265_SLICE_TYPE_I (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H265_SLICE_TYPE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH265PictureType(StdVideoH265PictureType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H265_PICTURE_TYPE_P (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H265_PICTURE_TYPE_B (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H265_PICTURE_TYPE_I (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H265_PICTURE_TYPE_IDR (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H265_PICTURE_TYPE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoH265AspectRatioIdc(StdVideoH265AspectRatioIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_UNSPECIFIED (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_SQUARE (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_12_11 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_10_11 (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_16_11 (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_40_33 (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_24_11 (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_20_11 (";
        break;
    case 8:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_32_11 (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_80_33 (";
        break;
    case 10:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_18_11 (";
        break;
    case 11:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_15_11 (";
        break;
    case 12:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_64_33 (";
        break;
    case 13:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_160_99 (";
        break;
    case 14:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_4_3 (";
        break;
    case 15:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_3_2 (";
        break;
    case 16:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_2_1 (";
        break;
    case 255:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_EXTENDED_SAR (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_H265_ASPECT_RATIO_IDC_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1Profile(StdVideoAV1Profile object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_PROFILE_MAIN (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_PROFILE_HIGH (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_PROFILE_PROFESSIONAL (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_PROFILE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1Level(StdVideoAV1Level object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_2_0 (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_2_1 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_2_2 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_2_3 (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_3_0 (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_3_1 (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_3_2 (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_3_3 (";
        break;
    case 8:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_4_0 (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_4_1 (";
        break;
    case 10:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_4_2 (";
        break;
    case 11:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_4_3 (";
        break;
    case 12:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_5_0 (";
        break;
    case 13:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_5_1 (";
        break;
    case 14:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_5_2 (";
        break;
    case 15:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_5_3 (";
        break;
    case 16:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_6_0 (";
        break;
    case 17:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_6_1 (";
        break;
    case 18:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_6_2 (";
        break;
    case 19:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_6_3 (";
        break;
    case 20:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_7_0 (";
        break;
    case 21:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_7_1 (";
        break;
    case 22:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_7_2 (";
        break;
    case 23:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_7_3 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_LEVEL_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1FrameType(StdVideoAV1FrameType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_FRAME_TYPE_KEY (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_FRAME_TYPE_INTER (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_FRAME_TYPE_INTRA_ONLY (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_AV1_FRAME_TYPE_SWITCH (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_FRAME_TYPE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1ReferenceName(StdVideoAV1ReferenceName object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_REFERENCE_NAME_INTRA_FRAME (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_REFERENCE_NAME_LAST_FRAME (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_REFERENCE_NAME_LAST2_FRAME (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_AV1_REFERENCE_NAME_LAST3_FRAME (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_AV1_REFERENCE_NAME_GOLDEN_FRAME (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_AV1_REFERENCE_NAME_BWDREF_FRAME (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_AV1_REFERENCE_NAME_ALTREF2_FRAME (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_AV1_REFERENCE_NAME_ALTREF_FRAME (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_REFERENCE_NAME_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1InterpolationFilter(StdVideoAV1InterpolationFilter object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SHARP (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_AV1_INTERPOLATION_FILTER_BILINEAR (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_AV1_INTERPOLATION_FILTER_SWITCHABLE (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_INTERPOLATION_FILTER_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1TxMode(StdVideoAV1TxMode object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_TX_MODE_ONLY_4X4 (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_TX_MODE_LARGEST (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_TX_MODE_SELECT (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_TX_MODE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1FrameRestorationType(StdVideoAV1FrameRestorationType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_NONE (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_WIENER (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SGRPROJ (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SWITCHABLE (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1ColorPrimaries(StdVideoAV1ColorPrimaries object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 1:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_709 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_UNSPECIFIED (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_M (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_B_G (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_601 (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_240 (";
        break;
    case 8:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_GENERIC_FILM (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_BT_2020 (";
        break;
    case 10:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_XYZ (";
        break;
    case 11:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_431 (";
        break;
    case 12:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_432 (";
        break;
    case 22:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_EBU_3213 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_COLOR_PRIMARIES_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1TransferCharacteristics(StdVideoAV1TransferCharacteristics object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_0 (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_709 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_UNSPECIFIED (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_3 (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_M (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_B_G (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_601 (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_240 (";
        break;
    case 8:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LINEAR (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100 (";
        break;
    case 10:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100_SQRT10 (";
        break;
    case 11:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_IEC_61966 (";
        break;
    case 12:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_1361 (";
        break;
    case 13:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SRGB (";
        break;
    case 14:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_10_BIT (";
        break;
    case 15:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_12_BIT (";
        break;
    case 16:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_2084 (";
        break;
    case 17:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_428 (";
        break;
    case 18:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_HLG (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1MatrixCoefficients(StdVideoAV1MatrixCoefficients object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_IDENTITY (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_709 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_UNSPECIFIED (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_RESERVED_3 (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_FCC (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_470_B_G (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_601 (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_240 (";
        break;
    case 8:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_YCGCO (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_NCL (";
        break;
    case 10:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_CL (";
        break;
    case 11:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_2085 (";
        break;
    case 12:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_NCL (";
        break;
    case 13:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_CL (";
        break;
    case 14:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_ICTCP (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_MATRIX_COEFFICIENTS_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoAV1ChromaSamplePosition(StdVideoAV1ChromaSamplePosition object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_UNKNOWN (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_VERTICAL (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_COLOCATED (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_RESERVED (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoVP9Profile(StdVideoVP9Profile object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_VP9_PROFILE_0 (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_VP9_PROFILE_1 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_VP9_PROFILE_2 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_VP9_PROFILE_3 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_VP9_PROFILE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoVP9Level(StdVideoVP9Level object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_1_0 (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_1_1 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_2_0 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_2_1 (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_3_0 (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_3_1 (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_4_0 (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_4_1 (";
        break;
    case 8:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_5_0 (";
        break;
    case 9:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_5_1 (";
        break;
    case 10:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_5_2 (";
        break;
    case 11:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_6_0 (";
        break;
    case 12:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_6_1 (";
        break;
    case 13:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_6_2 (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_VP9_LEVEL_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoVP9FrameType(StdVideoVP9FrameType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_VP9_FRAME_TYPE_KEY (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_VP9_FRAME_TYPE_NON_KEY (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_VP9_FRAME_TYPE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoVP9ReferenceName(StdVideoVP9ReferenceName object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_VP9_REFERENCE_NAME_INTRA_FRAME (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_VP9_REFERENCE_NAME_LAST_FRAME (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_VP9_REFERENCE_NAME_GOLDEN_FRAME (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_VP9_REFERENCE_NAME_ALTREF_FRAME (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_VP9_REFERENCE_NAME_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoVP9InterpolationFilter(StdVideoVP9InterpolationFilter object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SHARP (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_VP9_INTERPOLATION_FILTER_BILINEAR (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_VP9_INTERPOLATION_FILTER_SWITCHABLE (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_VP9_INTERPOLATION_FILTER_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}
void dump_text_StdVideoVP9ColorSpace(StdVideoVP9ColorSpace object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "STD_VIDEO_VP9_COLOR_SPACE_UNKNOWN (";
        break;
    case 1:
        settings.stream() << "STD_VIDEO_VP9_COLOR_SPACE_BT_601 (";
        break;
    case 2:
        settings.stream() << "STD_VIDEO_VP9_COLOR_SPACE_BT_709 (";
        break;
    case 3:
        settings.stream() << "STD_VIDEO_VP9_COLOR_SPACE_SMPTE_170 (";
        break;
    case 4:
        settings.stream() << "STD_VIDEO_VP9_COLOR_SPACE_SMPTE_240 (";
        break;
    case 5:
        settings.stream() << "STD_VIDEO_VP9_COLOR_SPACE_BT_2020 (";
        break;
    case 6:
        settings.stream() << "STD_VIDEO_VP9_COLOR_SPACE_RESERVED (";
        break;
    case 7:
        settings.stream() << "STD_VIDEO_VP9_COLOR_SPACE_RGB (";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "STD_VIDEO_VP9_COLOR_SPACE_INVALID (";
        break;
    default:
        settings.stream() << "UNKNOWN (";
    }
    settings.stream() << object << ")";
}

//========================= Bitmask Implementations =========================//


//=========================== Flag Implementations ==========================//




//======================= Func Pointer Implementations ======================//


//======================== Union Forward Declarations =======================//


//======================== pNext Chain Declarations =======================//


//========================== Struct Implementations =========================//

void dump_text_StdVideoH264SpsVuiFlags(const StdVideoH264SpsVuiFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.aspect_ratio_info_present_flag, settings, "uint32_t", "aspect_ratio_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.overscan_info_present_flag, settings, "uint32_t", "overscan_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.overscan_appropriate_flag, settings, "uint32_t", "overscan_appropriate_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.video_signal_type_present_flag, settings, "uint32_t", "video_signal_type_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.video_full_range_flag, settings, "uint32_t", "video_full_range_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.color_description_present_flag, settings, "uint32_t", "color_description_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.chroma_loc_info_present_flag, settings, "uint32_t", "chroma_loc_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.timing_info_present_flag, settings, "uint32_t", "timing_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.fixed_frame_rate_flag, settings, "uint32_t", "fixed_frame_rate_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.bitstream_restriction_flag, settings, "uint32_t", "bitstream_restriction_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.nal_hrd_parameters_present_flag, settings, "uint32_t", "nal_hrd_parameters_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vcl_hrd_parameters_present_flag, settings, "uint32_t", "vcl_hrd_parameters_present_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH264HrdParameters(const StdVideoH264HrdParameters& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint8_t>(object.cpb_cnt_minus1, settings, "uint8_t", "cpb_cnt_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.bit_rate_scale, settings, "uint8_t", "bit_rate_scale", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.cpb_size_scale, settings, "uint8_t", "cpb_size_scale", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint32_t>(object.bit_rate_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "uint32_t", "bit_rate_value_minus1", indents + 1, dump_text_uint32_t); // AQA
    dump_text_array<const uint32_t>(object.cpb_size_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "uint32_t", "cpb_size_value_minus1", indents + 1, dump_text_uint32_t); // AQA
    dump_text_array<const uint8_t>(object.cbr_flag, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "uint8_t", "cbr_flag", indents + 1, dump_text_uint8_t); // AQA
    dump_text_value<const uint32_t>(object.initial_cpb_removal_delay_length_minus1, settings, "uint32_t", "initial_cpb_removal_delay_length_minus1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.cpb_removal_delay_length_minus1, settings, "uint32_t", "cpb_removal_delay_length_minus1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.dpb_output_delay_length_minus1, settings, "uint32_t", "dpb_output_delay_length_minus1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.time_offset_length, settings, "uint32_t", "time_offset_length", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH264SequenceParameterSetVui(const StdVideoH264SequenceParameterSetVui& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH264SpsVuiFlags>(object.flags, settings, "StdVideoH264SpsVuiFlags", "flags", indents + 1, dump_text_StdVideoH264SpsVuiFlags);  // AET
    dump_text_value<const StdVideoH264AspectRatioIdc>(object.aspect_ratio_idc, settings, "StdVideoH264AspectRatioIdc", "aspect_ratio_idc", indents + 1, dump_text_StdVideoH264AspectRatioIdc);  // AET
    dump_text_value<const uint16_t>(object.sar_width, settings, "uint16_t", "sar_width", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.sar_height, settings, "uint16_t", "sar_height", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.video_format, settings, "uint8_t", "video_format", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.colour_primaries, settings, "uint8_t", "colour_primaries", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.transfer_characteristics, settings, "uint8_t", "transfer_characteristics", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.matrix_coefficients, settings, "uint8_t", "matrix_coefficients", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint32_t>(object.num_units_in_tick, settings, "uint32_t", "num_units_in_tick", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.time_scale, settings, "uint32_t", "time_scale", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint8_t>(object.max_num_reorder_frames, settings, "uint8_t", "max_num_reorder_frames", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.max_dec_frame_buffering, settings, "uint8_t", "max_dec_frame_buffering", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.chroma_sample_loc_type_top_field, settings, "uint8_t", "chroma_sample_loc_type_top_field", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.chroma_sample_loc_type_bottom_field, settings, "uint8_t", "chroma_sample_loc_type_bottom_field", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint32_t>(object.reserved1, settings, "uint32_t", "reserved1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_pointer<const StdVideoH264HrdParameters>(object.pHrdParameters, settings, "const StdVideoH264HrdParameters*", "pHrdParameters", indents + 1, dump_text_StdVideoH264HrdParameters);
}
void dump_text_StdVideoH264SpsFlags(const StdVideoH264SpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.constraint_set0_flag, settings, "uint32_t", "constraint_set0_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.constraint_set1_flag, settings, "uint32_t", "constraint_set1_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.constraint_set2_flag, settings, "uint32_t", "constraint_set2_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.constraint_set3_flag, settings, "uint32_t", "constraint_set3_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.constraint_set4_flag, settings, "uint32_t", "constraint_set4_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.constraint_set5_flag, settings, "uint32_t", "constraint_set5_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.direct_8x8_inference_flag, settings, "uint32_t", "direct_8x8_inference_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.mb_adaptive_frame_field_flag, settings, "uint32_t", "mb_adaptive_frame_field_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_mbs_only_flag, settings, "uint32_t", "frame_mbs_only_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.delta_pic_order_always_zero_flag, settings, "uint32_t", "delta_pic_order_always_zero_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.separate_colour_plane_flag, settings, "uint32_t", "separate_colour_plane_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.gaps_in_frame_num_value_allowed_flag, settings, "uint32_t", "gaps_in_frame_num_value_allowed_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.qpprime_y_zero_transform_bypass_flag, settings, "uint32_t", "qpprime_y_zero_transform_bypass_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_cropping_flag, settings, "uint32_t", "frame_cropping_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.seq_scaling_matrix_present_flag, settings, "uint32_t", "seq_scaling_matrix_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vui_parameters_present_flag, settings, "uint32_t", "vui_parameters_present_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH264ScalingLists(const StdVideoH264ScalingLists& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint16_t>(object.scaling_list_present_mask, settings, "uint16_t", "scaling_list_present_mask", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.use_default_scaling_matrix_mask, settings, "uint16_t", "use_default_scaling_matrix_mask", indents + 1, dump_text_uint16_t);  // AET
}
void dump_text_StdVideoH264SequenceParameterSet(const StdVideoH264SequenceParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH264SpsFlags>(object.flags, settings, "StdVideoH264SpsFlags", "flags", indents + 1, dump_text_StdVideoH264SpsFlags);  // AET
    dump_text_value<const StdVideoH264ProfileIdc>(object.profile_idc, settings, "StdVideoH264ProfileIdc", "profile_idc", indents + 1, dump_text_StdVideoH264ProfileIdc);  // AET
    dump_text_value<const StdVideoH264LevelIdc>(object.level_idc, settings, "StdVideoH264LevelIdc", "level_idc", indents + 1, dump_text_StdVideoH264LevelIdc);  // AET
    dump_text_value<const StdVideoH264ChromaFormatIdc>(object.chroma_format_idc, settings, "StdVideoH264ChromaFormatIdc", "chroma_format_idc", indents + 1, dump_text_StdVideoH264ChromaFormatIdc);  // AET
    dump_text_value<const uint8_t>(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.bit_depth_luma_minus8, settings, "uint8_t", "bit_depth_luma_minus8", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.bit_depth_chroma_minus8, settings, "uint8_t", "bit_depth_chroma_minus8", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_max_frame_num_minus4, settings, "uint8_t", "log2_max_frame_num_minus4", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const StdVideoH264PocType>(object.pic_order_cnt_type, settings, "StdVideoH264PocType", "pic_order_cnt_type", indents + 1, dump_text_StdVideoH264PocType);  // AET
    dump_text_value<const int32_t>(object.offset_for_non_ref_pic, settings, "int32_t", "offset_for_non_ref_pic", indents + 1, dump_text_int32_t);  // AET
    dump_text_value<const int32_t>(object.offset_for_top_to_bottom_field, settings, "int32_t", "offset_for_top_to_bottom_field", indents + 1, dump_text_int32_t);  // AET
    dump_text_value<const uint8_t>(object.log2_max_pic_order_cnt_lsb_minus4, settings, "uint8_t", "log2_max_pic_order_cnt_lsb_minus4", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_ref_frames_in_pic_order_cnt_cycle, settings, "uint8_t", "num_ref_frames_in_pic_order_cnt_cycle", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.max_num_ref_frames, settings, "uint8_t", "max_num_ref_frames", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint32_t>(object.pic_width_in_mbs_minus1, settings, "uint32_t", "pic_width_in_mbs_minus1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pic_height_in_map_units_minus1, settings, "uint32_t", "pic_height_in_map_units_minus1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_crop_left_offset, settings, "uint32_t", "frame_crop_left_offset", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_crop_right_offset, settings, "uint32_t", "frame_crop_right_offset", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_crop_top_offset, settings, "uint32_t", "frame_crop_top_offset", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_crop_bottom_offset, settings, "uint32_t", "frame_crop_bottom_offset", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved2, settings, "uint32_t", "reserved2", indents + 1, dump_text_uint32_t);  // AET
    dump_text_pointer<const int32_t>(object.pOffsetForRefFrame, settings, "const int32_t*", "pOffsetForRefFrame", indents + 1, dump_text_int32_t);
    dump_text_pointer<const StdVideoH264ScalingLists>(object.pScalingLists, settings, "const StdVideoH264ScalingLists*", "pScalingLists", indents + 1, dump_text_StdVideoH264ScalingLists);
    dump_text_pointer<const StdVideoH264SequenceParameterSetVui>(object.pSequenceParameterSetVui, settings, "const StdVideoH264SequenceParameterSetVui*", "pSequenceParameterSetVui", indents + 1, dump_text_StdVideoH264SequenceParameterSetVui);
}
void dump_text_StdVideoH264PpsFlags(const StdVideoH264PpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.transform_8x8_mode_flag, settings, "uint32_t", "transform_8x8_mode_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.redundant_pic_cnt_present_flag, settings, "uint32_t", "redundant_pic_cnt_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.constrained_intra_pred_flag, settings, "uint32_t", "constrained_intra_pred_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.deblocking_filter_control_present_flag, settings, "uint32_t", "deblocking_filter_control_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.weighted_pred_flag, settings, "uint32_t", "weighted_pred_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.bottom_field_pic_order_in_frame_present_flag, settings, "uint32_t", "bottom_field_pic_order_in_frame_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.entropy_coding_mode_flag, settings, "uint32_t", "entropy_coding_mode_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pic_scaling_matrix_present_flag, settings, "uint32_t", "pic_scaling_matrix_present_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH264PictureParameterSet(const StdVideoH264PictureParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH264PpsFlags>(object.flags, settings, "StdVideoH264PpsFlags", "flags", indents + 1, dump_text_StdVideoH264PpsFlags);  // AET
    dump_text_value<const uint8_t>(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pic_parameter_set_id, settings, "uint8_t", "pic_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_ref_idx_l0_default_active_minus1, settings, "uint8_t", "num_ref_idx_l0_default_active_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_ref_idx_l1_default_active_minus1, settings, "uint8_t", "num_ref_idx_l1_default_active_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const StdVideoH264WeightedBipredIdc>(object.weighted_bipred_idc, settings, "StdVideoH264WeightedBipredIdc", "weighted_bipred_idc", indents + 1, dump_text_StdVideoH264WeightedBipredIdc);  // AET
    dump_text_value<const int8_t>(object.pic_init_qp_minus26, settings, "int8_t", "pic_init_qp_minus26", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.pic_init_qs_minus26, settings, "int8_t", "pic_init_qs_minus26", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.chroma_qp_index_offset, settings, "int8_t", "chroma_qp_index_offset", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.second_chroma_qp_index_offset, settings, "int8_t", "second_chroma_qp_index_offset", indents + 1, dump_text_int8_t);  // AET
    dump_text_pointer<const StdVideoH264ScalingLists>(object.pScalingLists, settings, "const StdVideoH264ScalingLists*", "pScalingLists", indents + 1, dump_text_StdVideoH264ScalingLists);
}
void dump_text_StdVideoDecodeH264PictureInfoFlags(const StdVideoDecodeH264PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.field_pic_flag, settings, "uint32_t", "field_pic_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.is_intra, settings, "uint32_t", "is_intra", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.IdrPicFlag, settings, "uint32_t", "IdrPicFlag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.bottom_field_flag, settings, "uint32_t", "bottom_field_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.is_reference, settings, "uint32_t", "is_reference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.complementary_field_pair, settings, "uint32_t", "complementary_field_pair", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoDecodeH264PictureInfo(const StdVideoDecodeH264PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoDecodeH264PictureInfoFlags>(object.flags, settings, "StdVideoDecodeH264PictureInfoFlags", "flags", indents + 1, dump_text_StdVideoDecodeH264PictureInfoFlags);  // AET
    dump_text_value<const uint8_t>(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pic_parameter_set_id, settings, "uint8_t", "pic_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint16_t>(object.frame_num, settings, "uint16_t", "frame_num", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.idr_pic_id, settings, "uint16_t", "idr_pic_id", indents + 1, dump_text_uint16_t);  // AET
    dump_text_array<const int32_t>(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, "int32_t[STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE]", "int32_t", "PicOrderCnt", indents + 1, dump_text_int32_t); // AQA
}
void dump_text_StdVideoDecodeH264ReferenceInfoFlags(const StdVideoDecodeH264ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.top_field_flag, settings, "uint32_t", "top_field_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.bottom_field_flag, settings, "uint32_t", "bottom_field_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.used_for_long_term_reference, settings, "uint32_t", "used_for_long_term_reference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.is_non_existing, settings, "uint32_t", "is_non_existing", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoDecodeH264ReferenceInfo(const StdVideoDecodeH264ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoDecodeH264ReferenceInfoFlags>(object.flags, settings, "StdVideoDecodeH264ReferenceInfoFlags", "flags", indents + 1, dump_text_StdVideoDecodeH264ReferenceInfoFlags);  // AET
    dump_text_value<const uint16_t>(object.FrameNum, settings, "uint16_t", "FrameNum", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.reserved, settings, "uint16_t", "reserved", indents + 1, dump_text_uint16_t);  // AET
    dump_text_array<const int32_t>(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, "int32_t[STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE]", "int32_t", "PicOrderCnt", indents + 1, dump_text_int32_t); // AQA
}
void dump_text_StdVideoEncodeH264WeightTableFlags(const StdVideoEncodeH264WeightTableFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.luma_weight_l0_flag, settings, "uint32_t", "luma_weight_l0_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.chroma_weight_l0_flag, settings, "uint32_t", "chroma_weight_l0_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.luma_weight_l1_flag, settings, "uint32_t", "luma_weight_l1_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.chroma_weight_l1_flag, settings, "uint32_t", "chroma_weight_l1_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeH264WeightTable(const StdVideoEncodeH264WeightTable& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH264WeightTableFlags>(object.flags, settings, "StdVideoEncodeH264WeightTableFlags", "flags", indents + 1, dump_text_StdVideoEncodeH264WeightTableFlags);  // AET
    dump_text_value<const uint8_t>(object.luma_log2_weight_denom, settings, "uint8_t", "luma_log2_weight_denom", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.chroma_log2_weight_denom, settings, "uint8_t", "chroma_log2_weight_denom", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const int8_t>(object.luma_weight_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "int8_t", "luma_weight_l0", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const int8_t>(object.luma_offset_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "int8_t", "luma_offset_l0", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const int8_t>(object.luma_weight_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "int8_t", "luma_weight_l1", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const int8_t>(object.luma_offset_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "int8_t", "luma_offset_l1", indents + 1, dump_text_int8_t); // AQA
}
void dump_text_StdVideoEncodeH264SliceHeaderFlags(const StdVideoEncodeH264SliceHeaderFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.direct_spatial_mv_pred_flag, settings, "uint32_t", "direct_spatial_mv_pred_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.num_ref_idx_active_override_flag, settings, "uint32_t", "num_ref_idx_active_override_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeH264PictureInfoFlags(const StdVideoEncodeH264PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.IdrPicFlag, settings, "uint32_t", "IdrPicFlag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.is_reference, settings, "uint32_t", "is_reference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.no_output_of_prior_pics_flag, settings, "uint32_t", "no_output_of_prior_pics_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.long_term_reference_flag, settings, "uint32_t", "long_term_reference_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.adaptive_ref_pic_marking_mode_flag, settings, "uint32_t", "adaptive_ref_pic_marking_mode_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeH264ReferenceInfoFlags(const StdVideoEncodeH264ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.used_for_long_term_reference, settings, "uint32_t", "used_for_long_term_reference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeH264ReferenceListsInfoFlags(const StdVideoEncodeH264ReferenceListsInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.ref_pic_list_modification_flag_l0, settings, "uint32_t", "ref_pic_list_modification_flag_l0", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.ref_pic_list_modification_flag_l1, settings, "uint32_t", "ref_pic_list_modification_flag_l1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeH264RefListModEntry(const StdVideoEncodeH264RefListModEntry& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH264ModificationOfPicNumsIdc>(object.modification_of_pic_nums_idc, settings, "StdVideoH264ModificationOfPicNumsIdc", "modification_of_pic_nums_idc", indents + 1, dump_text_StdVideoH264ModificationOfPicNumsIdc);  // AET
    dump_text_value<const uint16_t>(object.abs_diff_pic_num_minus1, settings, "uint16_t", "abs_diff_pic_num_minus1", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.long_term_pic_num, settings, "uint16_t", "long_term_pic_num", indents + 1, dump_text_uint16_t);  // AET
}
void dump_text_StdVideoEncodeH264RefPicMarkingEntry(const StdVideoEncodeH264RefPicMarkingEntry& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH264MemMgmtControlOp>(object.memory_management_control_operation, settings, "StdVideoH264MemMgmtControlOp", "memory_management_control_operation", indents + 1, dump_text_StdVideoH264MemMgmtControlOp);  // AET
    dump_text_value<const uint16_t>(object.difference_of_pic_nums_minus1, settings, "uint16_t", "difference_of_pic_nums_minus1", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.long_term_pic_num, settings, "uint16_t", "long_term_pic_num", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.long_term_frame_idx, settings, "uint16_t", "long_term_frame_idx", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.max_long_term_frame_idx_plus1, settings, "uint16_t", "max_long_term_frame_idx_plus1", indents + 1, dump_text_uint16_t);  // AET
}
void dump_text_StdVideoEncodeH264ReferenceListsInfo(const StdVideoEncodeH264ReferenceListsInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH264ReferenceListsInfoFlags>(object.flags, settings, "StdVideoEncodeH264ReferenceListsInfoFlags", "flags", indents + 1, dump_text_StdVideoEncodeH264ReferenceListsInfoFlags);  // AET
    dump_text_value<const uint8_t>(object.num_ref_idx_l0_active_minus1, settings, "uint8_t", "num_ref_idx_l0_active_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_ref_idx_l1_active_minus1, settings, "uint8_t", "num_ref_idx_l1_active_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.RefPicList0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "uint8_t", "RefPicList0", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.RefPicList1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "uint8_t", "RefPicList1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_value<const uint8_t>(object.refList0ModOpCount, settings, "uint8_t", "refList0ModOpCount", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.refList1ModOpCount, settings, "uint8_t", "refList1ModOpCount", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.refPicMarkingOpCount, settings, "uint8_t", "refPicMarkingOpCount", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.reserved1, 7, settings, "uint8_t[7]", "uint8_t", "reserved1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_pointer<const StdVideoEncodeH264RefListModEntry>(object.pRefList0ModOperations, settings, "const StdVideoEncodeH264RefListModEntry*", "pRefList0ModOperations", indents + 1, dump_text_StdVideoEncodeH264RefListModEntry);
    dump_text_pointer<const StdVideoEncodeH264RefListModEntry>(object.pRefList1ModOperations, settings, "const StdVideoEncodeH264RefListModEntry*", "pRefList1ModOperations", indents + 1, dump_text_StdVideoEncodeH264RefListModEntry);
    dump_text_pointer<const StdVideoEncodeH264RefPicMarkingEntry>(object.pRefPicMarkingOperations, settings, "const StdVideoEncodeH264RefPicMarkingEntry*", "pRefPicMarkingOperations", indents + 1, dump_text_StdVideoEncodeH264RefPicMarkingEntry);
}
void dump_text_StdVideoEncodeH264PictureInfo(const StdVideoEncodeH264PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH264PictureInfoFlags>(object.flags, settings, "StdVideoEncodeH264PictureInfoFlags", "flags", indents + 1, dump_text_StdVideoEncodeH264PictureInfoFlags);  // AET
    dump_text_value<const uint8_t>(object.seq_parameter_set_id, settings, "uint8_t", "seq_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pic_parameter_set_id, settings, "uint8_t", "pic_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint16_t>(object.idr_pic_id, settings, "uint16_t", "idr_pic_id", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const StdVideoH264PictureType>(object.primary_pic_type, settings, "StdVideoH264PictureType", "primary_pic_type", indents + 1, dump_text_StdVideoH264PictureType);  // AET
    dump_text_value<const uint32_t>(object.frame_num, settings, "uint32_t", "frame_num", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const int32_t>(object.PicOrderCnt, settings, "int32_t", "PicOrderCnt", indents + 1, dump_text_int32_t);  // AET
    dump_text_value<const uint8_t>(object.temporal_id, settings, "uint8_t", "temporal_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.reserved1, 3, settings, "uint8_t[3]", "uint8_t", "reserved1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_pointer<const StdVideoEncodeH264ReferenceListsInfo>(object.pRefLists, settings, "const StdVideoEncodeH264ReferenceListsInfo*", "pRefLists", indents + 1, dump_text_StdVideoEncodeH264ReferenceListsInfo);
}
void dump_text_StdVideoEncodeH264ReferenceInfo(const StdVideoEncodeH264ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH264ReferenceInfoFlags>(object.flags, settings, "StdVideoEncodeH264ReferenceInfoFlags", "flags", indents + 1, dump_text_StdVideoEncodeH264ReferenceInfoFlags);  // AET
    dump_text_value<const StdVideoH264PictureType>(object.primary_pic_type, settings, "StdVideoH264PictureType", "primary_pic_type", indents + 1, dump_text_StdVideoH264PictureType);  // AET
    dump_text_value<const uint32_t>(object.FrameNum, settings, "uint32_t", "FrameNum", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const int32_t>(object.PicOrderCnt, settings, "int32_t", "PicOrderCnt", indents + 1, dump_text_int32_t);  // AET
    dump_text_value<const uint16_t>(object.long_term_pic_num, settings, "uint16_t", "long_term_pic_num", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.long_term_frame_idx, settings, "uint16_t", "long_term_frame_idx", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.temporal_id, settings, "uint8_t", "temporal_id", indents + 1, dump_text_uint8_t);  // AET
}
void dump_text_StdVideoEncodeH264SliceHeader(const StdVideoEncodeH264SliceHeader& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH264SliceHeaderFlags>(object.flags, settings, "StdVideoEncodeH264SliceHeaderFlags", "flags", indents + 1, dump_text_StdVideoEncodeH264SliceHeaderFlags);  // AET
    dump_text_value<const uint32_t>(object.first_mb_in_slice, settings, "uint32_t", "first_mb_in_slice", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const StdVideoH264SliceType>(object.slice_type, settings, "StdVideoH264SliceType", "slice_type", indents + 1, dump_text_StdVideoH264SliceType);  // AET
    dump_text_value<const int8_t>(object.slice_alpha_c0_offset_div2, settings, "int8_t", "slice_alpha_c0_offset_div2", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.slice_beta_offset_div2, settings, "int8_t", "slice_beta_offset_div2", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.slice_qp_delta, settings, "int8_t", "slice_qp_delta", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const StdVideoH264CabacInitIdc>(object.cabac_init_idc, settings, "StdVideoH264CabacInitIdc", "cabac_init_idc", indents + 1, dump_text_StdVideoH264CabacInitIdc);  // AET
    dump_text_value<const StdVideoH264DisableDeblockingFilterIdc>(object.disable_deblocking_filter_idc, settings, "StdVideoH264DisableDeblockingFilterIdc", "disable_deblocking_filter_idc", indents + 1, dump_text_StdVideoH264DisableDeblockingFilterIdc);  // AET
    dump_text_pointer<const StdVideoEncodeH264WeightTable>(object.pWeightTable, settings, "const StdVideoEncodeH264WeightTable*", "pWeightTable", indents + 1, dump_text_StdVideoEncodeH264WeightTable);
}
void dump_text_StdVideoH265DecPicBufMgr(const StdVideoH265DecPicBufMgr& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_array<const uint32_t>(object.max_latency_increase_plus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint32_t", "max_latency_increase_plus1", indents + 1, dump_text_uint32_t); // AQA
    dump_text_array<const uint8_t>(object.max_dec_pic_buffering_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint8_t", "max_dec_pic_buffering_minus1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.max_num_reorder_pics, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint8_t", "max_num_reorder_pics", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoH265SubLayerHrdParameters(const StdVideoH265SubLayerHrdParameters& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_array<const uint32_t>(object.bit_rate_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "uint32_t", "bit_rate_value_minus1", indents + 1, dump_text_uint32_t); // AQA
    dump_text_array<const uint32_t>(object.cpb_size_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "uint32_t", "cpb_size_value_minus1", indents + 1, dump_text_uint32_t); // AQA
    dump_text_array<const uint32_t>(object.cpb_size_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "uint32_t", "cpb_size_du_value_minus1", indents + 1, dump_text_uint32_t); // AQA
    dump_text_array<const uint32_t>(object.bit_rate_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "uint32_t", "bit_rate_du_value_minus1", indents + 1, dump_text_uint32_t); // AQA
    dump_text_value<const uint32_t>(object.cbr_flag, settings, "uint32_t", "cbr_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH265HrdFlags(const StdVideoH265HrdFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.nal_hrd_parameters_present_flag, settings, "uint32_t", "nal_hrd_parameters_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vcl_hrd_parameters_present_flag, settings, "uint32_t", "vcl_hrd_parameters_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sub_pic_hrd_params_present_flag, settings, "uint32_t", "sub_pic_hrd_params_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sub_pic_cpb_params_in_pic_timing_sei_flag, settings, "uint32_t", "sub_pic_cpb_params_in_pic_timing_sei_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.fixed_pic_rate_general_flag, settings, "uint32_t", "fixed_pic_rate_general_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.fixed_pic_rate_within_cvs_flag, settings, "uint32_t", "fixed_pic_rate_within_cvs_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.low_delay_hrd_flag, settings, "uint32_t", "low_delay_hrd_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH265HrdParameters(const StdVideoH265HrdParameters& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH265HrdFlags>(object.flags, settings, "StdVideoH265HrdFlags", "flags", indents + 1, dump_text_StdVideoH265HrdFlags);  // AET
    dump_text_value<const uint8_t>(object.tick_divisor_minus2, settings, "uint8_t", "tick_divisor_minus2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.du_cpb_removal_delay_increment_length_minus1, settings, "uint8_t", "du_cpb_removal_delay_increment_length_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.dpb_output_delay_du_length_minus1, settings, "uint8_t", "dpb_output_delay_du_length_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.bit_rate_scale, settings, "uint8_t", "bit_rate_scale", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.cpb_size_scale, settings, "uint8_t", "cpb_size_scale", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.cpb_size_du_scale, settings, "uint8_t", "cpb_size_du_scale", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.initial_cpb_removal_delay_length_minus1, settings, "uint8_t", "initial_cpb_removal_delay_length_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.au_cpb_removal_delay_length_minus1, settings, "uint8_t", "au_cpb_removal_delay_length_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.dpb_output_delay_length_minus1, settings, "uint8_t", "dpb_output_delay_length_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.cpb_cnt_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint8_t", "cpb_cnt_minus1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint16_t>(object.elemental_duration_in_tc_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint16_t", "elemental_duration_in_tc_minus1", indents + 1, dump_text_uint16_t); // AQA
    dump_text_array<const uint16_t>(object.reserved, 3, settings, "uint16_t[3]", "uint16_t", "reserved", indents + 1, dump_text_uint16_t); // AQA
    dump_text_pointer<const StdVideoH265SubLayerHrdParameters>(object.pSubLayerHrdParametersNal, settings, "const StdVideoH265SubLayerHrdParameters*", "pSubLayerHrdParametersNal", indents + 1, dump_text_StdVideoH265SubLayerHrdParameters);
    dump_text_pointer<const StdVideoH265SubLayerHrdParameters>(object.pSubLayerHrdParametersVcl, settings, "const StdVideoH265SubLayerHrdParameters*", "pSubLayerHrdParametersVcl", indents + 1, dump_text_StdVideoH265SubLayerHrdParameters);
}
void dump_text_StdVideoH265VpsFlags(const StdVideoH265VpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.vps_temporal_id_nesting_flag, settings, "uint32_t", "vps_temporal_id_nesting_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vps_sub_layer_ordering_info_present_flag, settings, "uint32_t", "vps_sub_layer_ordering_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vps_timing_info_present_flag, settings, "uint32_t", "vps_timing_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vps_poc_proportional_to_timing_flag, settings, "uint32_t", "vps_poc_proportional_to_timing_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH265ProfileTierLevelFlags(const StdVideoH265ProfileTierLevelFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.general_tier_flag, settings, "uint32_t", "general_tier_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.general_progressive_source_flag, settings, "uint32_t", "general_progressive_source_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.general_interlaced_source_flag, settings, "uint32_t", "general_interlaced_source_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.general_non_packed_constraint_flag, settings, "uint32_t", "general_non_packed_constraint_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.general_frame_only_constraint_flag, settings, "uint32_t", "general_frame_only_constraint_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH265ProfileTierLevel(const StdVideoH265ProfileTierLevel& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH265ProfileTierLevelFlags>(object.flags, settings, "StdVideoH265ProfileTierLevelFlags", "flags", indents + 1, dump_text_StdVideoH265ProfileTierLevelFlags);  // AET
    dump_text_value<const StdVideoH265ProfileIdc>(object.general_profile_idc, settings, "StdVideoH265ProfileIdc", "general_profile_idc", indents + 1, dump_text_StdVideoH265ProfileIdc);  // AET
    dump_text_value<const StdVideoH265LevelIdc>(object.general_level_idc, settings, "StdVideoH265LevelIdc", "general_level_idc", indents + 1, dump_text_StdVideoH265LevelIdc);  // AET
}
void dump_text_StdVideoH265VideoParameterSet(const StdVideoH265VideoParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH265VpsFlags>(object.flags, settings, "StdVideoH265VpsFlags", "flags", indents + 1, dump_text_StdVideoH265VpsFlags);  // AET
    dump_text_value<const uint8_t>(object.vps_video_parameter_set_id, settings, "uint8_t", "vps_video_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.vps_max_sub_layers_minus1, settings, "uint8_t", "vps_max_sub_layers_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint32_t>(object.vps_num_units_in_tick, settings, "uint32_t", "vps_num_units_in_tick", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vps_time_scale, settings, "uint32_t", "vps_time_scale", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vps_num_ticks_poc_diff_one_minus1, settings, "uint32_t", "vps_num_ticks_poc_diff_one_minus1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved3, settings, "uint32_t", "reserved3", indents + 1, dump_text_uint32_t);  // AET
    dump_text_pointer<const StdVideoH265DecPicBufMgr>(object.pDecPicBufMgr, settings, "const StdVideoH265DecPicBufMgr*", "pDecPicBufMgr", indents + 1, dump_text_StdVideoH265DecPicBufMgr);
    dump_text_pointer<const StdVideoH265HrdParameters>(object.pHrdParameters, settings, "const StdVideoH265HrdParameters*", "pHrdParameters", indents + 1, dump_text_StdVideoH265HrdParameters);
    dump_text_pointer<const StdVideoH265ProfileTierLevel>(object.pProfileTierLevel, settings, "const StdVideoH265ProfileTierLevel*", "pProfileTierLevel", indents + 1, dump_text_StdVideoH265ProfileTierLevel);
}
void dump_text_StdVideoH265ScalingLists(const StdVideoH265ScalingLists& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_array<const uint8_t>(object.ScalingListDCCoef16x16, STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS]", "uint8_t", "ScalingListDCCoef16x16", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.ScalingListDCCoef32x32, STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS]", "uint8_t", "ScalingListDCCoef32x32", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoH265SpsVuiFlags(const StdVideoH265SpsVuiFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.aspect_ratio_info_present_flag, settings, "uint32_t", "aspect_ratio_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.overscan_info_present_flag, settings, "uint32_t", "overscan_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.overscan_appropriate_flag, settings, "uint32_t", "overscan_appropriate_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.video_signal_type_present_flag, settings, "uint32_t", "video_signal_type_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.video_full_range_flag, settings, "uint32_t", "video_full_range_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.colour_description_present_flag, settings, "uint32_t", "colour_description_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.chroma_loc_info_present_flag, settings, "uint32_t", "chroma_loc_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.neutral_chroma_indication_flag, settings, "uint32_t", "neutral_chroma_indication_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.field_seq_flag, settings, "uint32_t", "field_seq_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_field_info_present_flag, settings, "uint32_t", "frame_field_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.default_display_window_flag, settings, "uint32_t", "default_display_window_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vui_timing_info_present_flag, settings, "uint32_t", "vui_timing_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vui_poc_proportional_to_timing_flag, settings, "uint32_t", "vui_poc_proportional_to_timing_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vui_hrd_parameters_present_flag, settings, "uint32_t", "vui_hrd_parameters_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.bitstream_restriction_flag, settings, "uint32_t", "bitstream_restriction_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.tiles_fixed_structure_flag, settings, "uint32_t", "tiles_fixed_structure_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.motion_vectors_over_pic_boundaries_flag, settings, "uint32_t", "motion_vectors_over_pic_boundaries_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.restricted_ref_pic_lists_flag, settings, "uint32_t", "restricted_ref_pic_lists_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH265SequenceParameterSetVui(const StdVideoH265SequenceParameterSetVui& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH265SpsVuiFlags>(object.flags, settings, "StdVideoH265SpsVuiFlags", "flags", indents + 1, dump_text_StdVideoH265SpsVuiFlags);  // AET
    dump_text_value<const StdVideoH265AspectRatioIdc>(object.aspect_ratio_idc, settings, "StdVideoH265AspectRatioIdc", "aspect_ratio_idc", indents + 1, dump_text_StdVideoH265AspectRatioIdc);  // AET
    dump_text_value<const uint16_t>(object.sar_width, settings, "uint16_t", "sar_width", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.sar_height, settings, "uint16_t", "sar_height", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.video_format, settings, "uint8_t", "video_format", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.colour_primaries, settings, "uint8_t", "colour_primaries", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.transfer_characteristics, settings, "uint8_t", "transfer_characteristics", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.matrix_coeffs, settings, "uint8_t", "matrix_coeffs", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.chroma_sample_loc_type_top_field, settings, "uint8_t", "chroma_sample_loc_type_top_field", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.chroma_sample_loc_type_bottom_field, settings, "uint8_t", "chroma_sample_loc_type_bottom_field", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint16_t>(object.def_disp_win_left_offset, settings, "uint16_t", "def_disp_win_left_offset", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.def_disp_win_right_offset, settings, "uint16_t", "def_disp_win_right_offset", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.def_disp_win_top_offset, settings, "uint16_t", "def_disp_win_top_offset", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.def_disp_win_bottom_offset, settings, "uint16_t", "def_disp_win_bottom_offset", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint32_t>(object.vui_num_units_in_tick, settings, "uint32_t", "vui_num_units_in_tick", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vui_time_scale, settings, "uint32_t", "vui_time_scale", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vui_num_ticks_poc_diff_one_minus1, settings, "uint32_t", "vui_num_ticks_poc_diff_one_minus1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint16_t>(object.min_spatial_segmentation_idc, settings, "uint16_t", "min_spatial_segmentation_idc", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.reserved3, settings, "uint16_t", "reserved3", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.max_bytes_per_pic_denom, settings, "uint8_t", "max_bytes_per_pic_denom", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.max_bits_per_min_cu_denom, settings, "uint8_t", "max_bits_per_min_cu_denom", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_max_mv_length_horizontal, settings, "uint8_t", "log2_max_mv_length_horizontal", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_max_mv_length_vertical, settings, "uint8_t", "log2_max_mv_length_vertical", indents + 1, dump_text_uint8_t);  // AET
    dump_text_pointer<const StdVideoH265HrdParameters>(object.pHrdParameters, settings, "const StdVideoH265HrdParameters*", "pHrdParameters", indents + 1, dump_text_StdVideoH265HrdParameters);
}
void dump_text_StdVideoH265PredictorPaletteEntries(const StdVideoH265PredictorPaletteEntries& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
}
void dump_text_StdVideoH265SpsFlags(const StdVideoH265SpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.sps_temporal_id_nesting_flag, settings, "uint32_t", "sps_temporal_id_nesting_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.separate_colour_plane_flag, settings, "uint32_t", "separate_colour_plane_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.conformance_window_flag, settings, "uint32_t", "conformance_window_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sps_sub_layer_ordering_info_present_flag, settings, "uint32_t", "sps_sub_layer_ordering_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.scaling_list_enabled_flag, settings, "uint32_t", "scaling_list_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sps_scaling_list_data_present_flag, settings, "uint32_t", "sps_scaling_list_data_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.amp_enabled_flag, settings, "uint32_t", "amp_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sample_adaptive_offset_enabled_flag, settings, "uint32_t", "sample_adaptive_offset_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pcm_enabled_flag, settings, "uint32_t", "pcm_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pcm_loop_filter_disabled_flag, settings, "uint32_t", "pcm_loop_filter_disabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.long_term_ref_pics_present_flag, settings, "uint32_t", "long_term_ref_pics_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sps_temporal_mvp_enabled_flag, settings, "uint32_t", "sps_temporal_mvp_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.strong_intra_smoothing_enabled_flag, settings, "uint32_t", "strong_intra_smoothing_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.vui_parameters_present_flag, settings, "uint32_t", "vui_parameters_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sps_extension_present_flag, settings, "uint32_t", "sps_extension_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sps_range_extension_flag, settings, "uint32_t", "sps_range_extension_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.transform_skip_rotation_enabled_flag, settings, "uint32_t", "transform_skip_rotation_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.transform_skip_context_enabled_flag, settings, "uint32_t", "transform_skip_context_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.implicit_rdpcm_enabled_flag, settings, "uint32_t", "implicit_rdpcm_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.explicit_rdpcm_enabled_flag, settings, "uint32_t", "explicit_rdpcm_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.extended_precision_processing_flag, settings, "uint32_t", "extended_precision_processing_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.intra_smoothing_disabled_flag, settings, "uint32_t", "intra_smoothing_disabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.high_precision_offsets_enabled_flag, settings, "uint32_t", "high_precision_offsets_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.persistent_rice_adaptation_enabled_flag, settings, "uint32_t", "persistent_rice_adaptation_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.cabac_bypass_alignment_enabled_flag, settings, "uint32_t", "cabac_bypass_alignment_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sps_scc_extension_flag, settings, "uint32_t", "sps_scc_extension_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sps_curr_pic_ref_enabled_flag, settings, "uint32_t", "sps_curr_pic_ref_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.palette_mode_enabled_flag, settings, "uint32_t", "palette_mode_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sps_palette_predictor_initializers_present_flag, settings, "uint32_t", "sps_palette_predictor_initializers_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.intra_boundary_filtering_disabled_flag, settings, "uint32_t", "intra_boundary_filtering_disabled_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH265ShortTermRefPicSetFlags(const StdVideoH265ShortTermRefPicSetFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.inter_ref_pic_set_prediction_flag, settings, "uint32_t", "inter_ref_pic_set_prediction_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.delta_rps_sign, settings, "uint32_t", "delta_rps_sign", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH265ShortTermRefPicSet(const StdVideoH265ShortTermRefPicSet& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH265ShortTermRefPicSetFlags>(object.flags, settings, "StdVideoH265ShortTermRefPicSetFlags", "flags", indents + 1, dump_text_StdVideoH265ShortTermRefPicSetFlags);  // AET
    dump_text_value<const uint32_t>(object.delta_idx_minus1, settings, "uint32_t", "delta_idx_minus1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint16_t>(object.use_delta_flag, settings, "uint16_t", "use_delta_flag", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.abs_delta_rps_minus1, settings, "uint16_t", "abs_delta_rps_minus1", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.used_by_curr_pic_flag, settings, "uint16_t", "used_by_curr_pic_flag", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.used_by_curr_pic_s0_flag, settings, "uint16_t", "used_by_curr_pic_s0_flag", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.used_by_curr_pic_s1_flag, settings, "uint16_t", "used_by_curr_pic_s1_flag", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.reserved1, settings, "uint16_t", "reserved1", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved3, settings, "uint8_t", "reserved3", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_negative_pics, settings, "uint8_t", "num_negative_pics", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_positive_pics, settings, "uint8_t", "num_positive_pics", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint16_t>(object.delta_poc_s0_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, "uint16_t[STD_VIDEO_H265_MAX_DPB_SIZE]", "uint16_t", "delta_poc_s0_minus1", indents + 1, dump_text_uint16_t); // AQA
    dump_text_array<const uint16_t>(object.delta_poc_s1_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, "uint16_t[STD_VIDEO_H265_MAX_DPB_SIZE]", "uint16_t", "delta_poc_s1_minus1", indents + 1, dump_text_uint16_t); // AQA
}
void dump_text_StdVideoH265LongTermRefPicsSps(const StdVideoH265LongTermRefPicsSps& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.used_by_curr_pic_lt_sps_flag, settings, "uint32_t", "used_by_curr_pic_lt_sps_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_array<const uint32_t>(object.lt_ref_pic_poc_lsb_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, "uint32_t[STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS]", "uint32_t", "lt_ref_pic_poc_lsb_sps", indents + 1, dump_text_uint32_t); // AQA
}
void dump_text_StdVideoH265SequenceParameterSet(const StdVideoH265SequenceParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH265SpsFlags>(object.flags, settings, "StdVideoH265SpsFlags", "flags", indents + 1, dump_text_StdVideoH265SpsFlags);  // AET
    dump_text_value<const StdVideoH265ChromaFormatIdc>(object.chroma_format_idc, settings, "StdVideoH265ChromaFormatIdc", "chroma_format_idc", indents + 1, dump_text_StdVideoH265ChromaFormatIdc);  // AET
    dump_text_value<const uint32_t>(object.pic_width_in_luma_samples, settings, "uint32_t", "pic_width_in_luma_samples", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pic_height_in_luma_samples, settings, "uint32_t", "pic_height_in_luma_samples", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint8_t>(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.sps_max_sub_layers_minus1, settings, "uint8_t", "sps_max_sub_layers_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.sps_seq_parameter_set_id, settings, "uint8_t", "sps_seq_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.bit_depth_luma_minus8, settings, "uint8_t", "bit_depth_luma_minus8", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.bit_depth_chroma_minus8, settings, "uint8_t", "bit_depth_chroma_minus8", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_max_pic_order_cnt_lsb_minus4, settings, "uint8_t", "log2_max_pic_order_cnt_lsb_minus4", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_min_luma_coding_block_size_minus3, settings, "uint8_t", "log2_min_luma_coding_block_size_minus3", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_diff_max_min_luma_coding_block_size, settings, "uint8_t", "log2_diff_max_min_luma_coding_block_size", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_min_luma_transform_block_size_minus2, settings, "uint8_t", "log2_min_luma_transform_block_size_minus2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_diff_max_min_luma_transform_block_size, settings, "uint8_t", "log2_diff_max_min_luma_transform_block_size", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.max_transform_hierarchy_depth_inter, settings, "uint8_t", "max_transform_hierarchy_depth_inter", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.max_transform_hierarchy_depth_intra, settings, "uint8_t", "max_transform_hierarchy_depth_intra", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_short_term_ref_pic_sets, settings, "uint8_t", "num_short_term_ref_pic_sets", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_long_term_ref_pics_sps, settings, "uint8_t", "num_long_term_ref_pics_sps", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pcm_sample_bit_depth_luma_minus1, settings, "uint8_t", "pcm_sample_bit_depth_luma_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pcm_sample_bit_depth_chroma_minus1, settings, "uint8_t", "pcm_sample_bit_depth_chroma_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_min_pcm_luma_coding_block_size_minus3, settings, "uint8_t", "log2_min_pcm_luma_coding_block_size_minus3", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_diff_max_min_pcm_luma_coding_block_size, settings, "uint8_t", "log2_diff_max_min_pcm_luma_coding_block_size", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.palette_max_size, settings, "uint8_t", "palette_max_size", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.delta_palette_max_predictor_size, settings, "uint8_t", "delta_palette_max_predictor_size", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.motion_vector_resolution_control_idc, settings, "uint8_t", "motion_vector_resolution_control_idc", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.sps_num_palette_predictor_initializers_minus1, settings, "uint8_t", "sps_num_palette_predictor_initializers_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint32_t>(object.conf_win_left_offset, settings, "uint32_t", "conf_win_left_offset", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.conf_win_right_offset, settings, "uint32_t", "conf_win_right_offset", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.conf_win_top_offset, settings, "uint32_t", "conf_win_top_offset", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.conf_win_bottom_offset, settings, "uint32_t", "conf_win_bottom_offset", indents + 1, dump_text_uint32_t);  // AET
    dump_text_pointer<const StdVideoH265ProfileTierLevel>(object.pProfileTierLevel, settings, "const StdVideoH265ProfileTierLevel*", "pProfileTierLevel", indents + 1, dump_text_StdVideoH265ProfileTierLevel);
    dump_text_pointer<const StdVideoH265DecPicBufMgr>(object.pDecPicBufMgr, settings, "const StdVideoH265DecPicBufMgr*", "pDecPicBufMgr", indents + 1, dump_text_StdVideoH265DecPicBufMgr);
    dump_text_pointer<const StdVideoH265ScalingLists>(object.pScalingLists, settings, "const StdVideoH265ScalingLists*", "pScalingLists", indents + 1, dump_text_StdVideoH265ScalingLists);
    dump_text_pointer<const StdVideoH265ShortTermRefPicSet>(object.pShortTermRefPicSet, settings, "const StdVideoH265ShortTermRefPicSet*", "pShortTermRefPicSet", indents + 1, dump_text_StdVideoH265ShortTermRefPicSet);
    dump_text_pointer<const StdVideoH265LongTermRefPicsSps>(object.pLongTermRefPicsSps, settings, "const StdVideoH265LongTermRefPicsSps*", "pLongTermRefPicsSps", indents + 1, dump_text_StdVideoH265LongTermRefPicsSps);
    dump_text_pointer<const StdVideoH265SequenceParameterSetVui>(object.pSequenceParameterSetVui, settings, "const StdVideoH265SequenceParameterSetVui*", "pSequenceParameterSetVui", indents + 1, dump_text_StdVideoH265SequenceParameterSetVui);
    dump_text_pointer<const StdVideoH265PredictorPaletteEntries>(object.pPredictorPaletteEntries, settings, "const StdVideoH265PredictorPaletteEntries*", "pPredictorPaletteEntries", indents + 1, dump_text_StdVideoH265PredictorPaletteEntries);
}
void dump_text_StdVideoH265PpsFlags(const StdVideoH265PpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.dependent_slice_segments_enabled_flag, settings, "uint32_t", "dependent_slice_segments_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.output_flag_present_flag, settings, "uint32_t", "output_flag_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.sign_data_hiding_enabled_flag, settings, "uint32_t", "sign_data_hiding_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.cabac_init_present_flag, settings, "uint32_t", "cabac_init_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.constrained_intra_pred_flag, settings, "uint32_t", "constrained_intra_pred_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.transform_skip_enabled_flag, settings, "uint32_t", "transform_skip_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.cu_qp_delta_enabled_flag, settings, "uint32_t", "cu_qp_delta_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pps_slice_chroma_qp_offsets_present_flag, settings, "uint32_t", "pps_slice_chroma_qp_offsets_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.weighted_pred_flag, settings, "uint32_t", "weighted_pred_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.weighted_bipred_flag, settings, "uint32_t", "weighted_bipred_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.transquant_bypass_enabled_flag, settings, "uint32_t", "transquant_bypass_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.tiles_enabled_flag, settings, "uint32_t", "tiles_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.entropy_coding_sync_enabled_flag, settings, "uint32_t", "entropy_coding_sync_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.uniform_spacing_flag, settings, "uint32_t", "uniform_spacing_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.loop_filter_across_tiles_enabled_flag, settings, "uint32_t", "loop_filter_across_tiles_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pps_loop_filter_across_slices_enabled_flag, settings, "uint32_t", "pps_loop_filter_across_slices_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.deblocking_filter_control_present_flag, settings, "uint32_t", "deblocking_filter_control_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.deblocking_filter_override_enabled_flag, settings, "uint32_t", "deblocking_filter_override_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pps_deblocking_filter_disabled_flag, settings, "uint32_t", "pps_deblocking_filter_disabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pps_scaling_list_data_present_flag, settings, "uint32_t", "pps_scaling_list_data_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.lists_modification_present_flag, settings, "uint32_t", "lists_modification_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.slice_segment_header_extension_present_flag, settings, "uint32_t", "slice_segment_header_extension_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pps_extension_present_flag, settings, "uint32_t", "pps_extension_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.cross_component_prediction_enabled_flag, settings, "uint32_t", "cross_component_prediction_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.chroma_qp_offset_list_enabled_flag, settings, "uint32_t", "chroma_qp_offset_list_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pps_curr_pic_ref_enabled_flag, settings, "uint32_t", "pps_curr_pic_ref_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.residual_adaptive_colour_transform_enabled_flag, settings, "uint32_t", "residual_adaptive_colour_transform_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pps_slice_act_qp_offsets_present_flag, settings, "uint32_t", "pps_slice_act_qp_offsets_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pps_palette_predictor_initializers_present_flag, settings, "uint32_t", "pps_palette_predictor_initializers_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.monochrome_palette_flag, settings, "uint32_t", "monochrome_palette_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pps_range_extension_flag, settings, "uint32_t", "pps_range_extension_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoH265PictureParameterSet(const StdVideoH265PictureParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoH265PpsFlags>(object.flags, settings, "StdVideoH265PpsFlags", "flags", indents + 1, dump_text_StdVideoH265PpsFlags);  // AET
    dump_text_value<const uint8_t>(object.pps_pic_parameter_set_id, settings, "uint8_t", "pps_pic_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pps_seq_parameter_set_id, settings, "uint8_t", "pps_seq_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_extra_slice_header_bits, settings, "uint8_t", "num_extra_slice_header_bits", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_ref_idx_l0_default_active_minus1, settings, "uint8_t", "num_ref_idx_l0_default_active_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_ref_idx_l1_default_active_minus1, settings, "uint8_t", "num_ref_idx_l1_default_active_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const int8_t>(object.init_qp_minus26, settings, "int8_t", "init_qp_minus26", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const uint8_t>(object.diff_cu_qp_delta_depth, settings, "uint8_t", "diff_cu_qp_delta_depth", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const int8_t>(object.pps_cb_qp_offset, settings, "int8_t", "pps_cb_qp_offset", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.pps_cr_qp_offset, settings, "int8_t", "pps_cr_qp_offset", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.pps_beta_offset_div2, settings, "int8_t", "pps_beta_offset_div2", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.pps_tc_offset_div2, settings, "int8_t", "pps_tc_offset_div2", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_parallel_merge_level_minus2, settings, "uint8_t", "log2_parallel_merge_level_minus2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_max_transform_skip_block_size_minus2, settings, "uint8_t", "log2_max_transform_skip_block_size_minus2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.diff_cu_chroma_qp_offset_depth, settings, "uint8_t", "diff_cu_chroma_qp_offset_depth", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.chroma_qp_offset_list_len_minus1, settings, "uint8_t", "chroma_qp_offset_list_len_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const int8_t>(object.cb_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, "int8_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE]", "int8_t", "cb_qp_offset_list", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const int8_t>(object.cr_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, "int8_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE]", "int8_t", "cr_qp_offset_list", indents + 1, dump_text_int8_t); // AQA
    dump_text_value<const uint8_t>(object.log2_sao_offset_scale_luma, settings, "uint8_t", "log2_sao_offset_scale_luma", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.log2_sao_offset_scale_chroma, settings, "uint8_t", "log2_sao_offset_scale_chroma", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const int8_t>(object.pps_act_y_qp_offset_plus5, settings, "int8_t", "pps_act_y_qp_offset_plus5", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.pps_act_cb_qp_offset_plus5, settings, "int8_t", "pps_act_cb_qp_offset_plus5", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.pps_act_cr_qp_offset_plus3, settings, "int8_t", "pps_act_cr_qp_offset_plus3", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const uint8_t>(object.pps_num_palette_predictor_initializers, settings, "uint8_t", "pps_num_palette_predictor_initializers", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.luma_bit_depth_entry_minus8, settings, "uint8_t", "luma_bit_depth_entry_minus8", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.chroma_bit_depth_entry_minus8, settings, "uint8_t", "chroma_bit_depth_entry_minus8", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_tile_columns_minus1, settings, "uint8_t", "num_tile_columns_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_tile_rows_minus1, settings, "uint8_t", "num_tile_rows_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved2, settings, "uint8_t", "reserved2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint16_t>(object.column_width_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE]", "uint16_t", "column_width_minus1", indents + 1, dump_text_uint16_t); // AQA
    dump_text_array<const uint16_t>(object.row_height_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE]", "uint16_t", "row_height_minus1", indents + 1, dump_text_uint16_t); // AQA
    dump_text_value<const uint32_t>(object.reserved3, settings, "uint32_t", "reserved3", indents + 1, dump_text_uint32_t);  // AET
    dump_text_pointer<const StdVideoH265ScalingLists>(object.pScalingLists, settings, "const StdVideoH265ScalingLists*", "pScalingLists", indents + 1, dump_text_StdVideoH265ScalingLists);
    dump_text_pointer<const StdVideoH265PredictorPaletteEntries>(object.pPredictorPaletteEntries, settings, "const StdVideoH265PredictorPaletteEntries*", "pPredictorPaletteEntries", indents + 1, dump_text_StdVideoH265PredictorPaletteEntries);
}
void dump_text_StdVideoDecodeH265PictureInfoFlags(const StdVideoDecodeH265PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.IrapPicFlag, settings, "uint32_t", "IrapPicFlag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.IdrPicFlag, settings, "uint32_t", "IdrPicFlag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.IsReference, settings, "uint32_t", "IsReference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.short_term_ref_pic_set_sps_flag, settings, "uint32_t", "short_term_ref_pic_set_sps_flag", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoDecodeH265PictureInfo(const StdVideoDecodeH265PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoDecodeH265PictureInfoFlags>(object.flags, settings, "StdVideoDecodeH265PictureInfoFlags", "flags", indents + 1, dump_text_StdVideoDecodeH265PictureInfoFlags);  // AET
    dump_text_value<const uint8_t>(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pps_seq_parameter_set_id, settings, "uint8_t", "pps_seq_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pps_pic_parameter_set_id, settings, "uint8_t", "pps_pic_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.NumDeltaPocsOfRefRpsIdx, settings, "uint8_t", "NumDeltaPocsOfRefRpsIdx", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const int32_t>(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + 1, dump_text_int32_t);  // AET
    dump_text_value<const uint16_t>(object.NumBitsForSTRefPicSetInSlice, settings, "uint16_t", "NumBitsForSTRefPicSetInSlice", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.reserved, settings, "uint16_t", "reserved", indents + 1, dump_text_uint16_t);  // AET
    dump_text_array<const uint8_t>(object.RefPicSetStCurrBefore, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "uint8_t", "RefPicSetStCurrBefore", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.RefPicSetStCurrAfter, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "uint8_t", "RefPicSetStCurrAfter", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.RefPicSetLtCurr, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "uint8_t", "RefPicSetLtCurr", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoDecodeH265ReferenceInfoFlags(const StdVideoDecodeH265ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.used_for_long_term_reference, settings, "uint32_t", "used_for_long_term_reference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.unused_for_reference, settings, "uint32_t", "unused_for_reference", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoDecodeH265ReferenceInfo(const StdVideoDecodeH265ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoDecodeH265ReferenceInfoFlags>(object.flags, settings, "StdVideoDecodeH265ReferenceInfoFlags", "flags", indents + 1, dump_text_StdVideoDecodeH265ReferenceInfoFlags);  // AET
    dump_text_value<const int32_t>(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + 1, dump_text_int32_t);  // AET
}
void dump_text_StdVideoEncodeH265WeightTableFlags(const StdVideoEncodeH265WeightTableFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint16_t>(object.luma_weight_l0_flag, settings, "uint16_t", "luma_weight_l0_flag", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.chroma_weight_l0_flag, settings, "uint16_t", "chroma_weight_l0_flag", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.luma_weight_l1_flag, settings, "uint16_t", "luma_weight_l1_flag", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.chroma_weight_l1_flag, settings, "uint16_t", "chroma_weight_l1_flag", indents + 1, dump_text_uint16_t);  // AET
}
void dump_text_StdVideoEncodeH265WeightTable(const StdVideoEncodeH265WeightTable& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH265WeightTableFlags>(object.flags, settings, "StdVideoEncodeH265WeightTableFlags", "flags", indents + 1, dump_text_StdVideoEncodeH265WeightTableFlags);  // AET
    dump_text_value<const uint8_t>(object.luma_log2_weight_denom, settings, "uint8_t", "luma_log2_weight_denom", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const int8_t>(object.delta_chroma_log2_weight_denom, settings, "int8_t", "delta_chroma_log2_weight_denom", indents + 1, dump_text_int8_t);  // AET
    dump_text_array<const int8_t>(object.delta_luma_weight_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "int8_t", "delta_luma_weight_l0", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const int8_t>(object.luma_offset_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "int8_t", "luma_offset_l0", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const int8_t>(object.delta_luma_weight_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "int8_t", "delta_luma_weight_l1", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const int8_t>(object.luma_offset_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "int8_t", "luma_offset_l1", indents + 1, dump_text_int8_t); // AQA
}
void dump_text_StdVideoEncodeH265SliceSegmentHeaderFlags(const StdVideoEncodeH265SliceSegmentHeaderFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.first_slice_segment_in_pic_flag, settings, "uint32_t", "first_slice_segment_in_pic_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.dependent_slice_segment_flag, settings, "uint32_t", "dependent_slice_segment_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.slice_sao_luma_flag, settings, "uint32_t", "slice_sao_luma_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.slice_sao_chroma_flag, settings, "uint32_t", "slice_sao_chroma_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.num_ref_idx_active_override_flag, settings, "uint32_t", "num_ref_idx_active_override_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.mvd_l1_zero_flag, settings, "uint32_t", "mvd_l1_zero_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.cabac_init_flag, settings, "uint32_t", "cabac_init_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.cu_chroma_qp_offset_enabled_flag, settings, "uint32_t", "cu_chroma_qp_offset_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.deblocking_filter_override_flag, settings, "uint32_t", "deblocking_filter_override_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.slice_deblocking_filter_disabled_flag, settings, "uint32_t", "slice_deblocking_filter_disabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.collocated_from_l0_flag, settings, "uint32_t", "collocated_from_l0_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.slice_loop_filter_across_slices_enabled_flag, settings, "uint32_t", "slice_loop_filter_across_slices_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeH265SliceSegmentHeader(const StdVideoEncodeH265SliceSegmentHeader& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH265SliceSegmentHeaderFlags>(object.flags, settings, "StdVideoEncodeH265SliceSegmentHeaderFlags", "flags", indents + 1, dump_text_StdVideoEncodeH265SliceSegmentHeaderFlags);  // AET
    dump_text_value<const StdVideoH265SliceType>(object.slice_type, settings, "StdVideoH265SliceType", "slice_type", indents + 1, dump_text_StdVideoH265SliceType);  // AET
    dump_text_value<const uint32_t>(object.slice_segment_address, settings, "uint32_t", "slice_segment_address", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint8_t>(object.collocated_ref_idx, settings, "uint8_t", "collocated_ref_idx", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.MaxNumMergeCand, settings, "uint8_t", "MaxNumMergeCand", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const int8_t>(object.slice_cb_qp_offset, settings, "int8_t", "slice_cb_qp_offset", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.slice_cr_qp_offset, settings, "int8_t", "slice_cr_qp_offset", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.slice_beta_offset_div2, settings, "int8_t", "slice_beta_offset_div2", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.slice_tc_offset_div2, settings, "int8_t", "slice_tc_offset_div2", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.slice_act_y_qp_offset, settings, "int8_t", "slice_act_y_qp_offset", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.slice_act_cb_qp_offset, settings, "int8_t", "slice_act_cb_qp_offset", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.slice_act_cr_qp_offset, settings, "int8_t", "slice_act_cr_qp_offset", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.slice_qp_delta, settings, "int8_t", "slice_qp_delta", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const uint16_t>(object.reserved1, settings, "uint16_t", "reserved1", indents + 1, dump_text_uint16_t);  // AET
    dump_text_pointer<const StdVideoEncodeH265WeightTable>(object.pWeightTable, settings, "const StdVideoEncodeH265WeightTable*", "pWeightTable", indents + 1, dump_text_StdVideoEncodeH265WeightTable);
}
void dump_text_StdVideoEncodeH265ReferenceListsInfoFlags(const StdVideoEncodeH265ReferenceListsInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.ref_pic_list_modification_flag_l0, settings, "uint32_t", "ref_pic_list_modification_flag_l0", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.ref_pic_list_modification_flag_l1, settings, "uint32_t", "ref_pic_list_modification_flag_l1", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeH265ReferenceListsInfo(const StdVideoEncodeH265ReferenceListsInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH265ReferenceListsInfoFlags>(object.flags, settings, "StdVideoEncodeH265ReferenceListsInfoFlags", "flags", indents + 1, dump_text_StdVideoEncodeH265ReferenceListsInfoFlags);  // AET
    dump_text_value<const uint8_t>(object.num_ref_idx_l0_active_minus1, settings, "uint8_t", "num_ref_idx_l0_active_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_ref_idx_l1_active_minus1, settings, "uint8_t", "num_ref_idx_l1_active_minus1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.RefPicList0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "uint8_t", "RefPicList0", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.RefPicList1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "uint8_t", "RefPicList1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.list_entry_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "uint8_t", "list_entry_l0", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.list_entry_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "uint8_t", "list_entry_l1", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoEncodeH265PictureInfoFlags(const StdVideoEncodeH265PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.is_reference, settings, "uint32_t", "is_reference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.IrapPicFlag, settings, "uint32_t", "IrapPicFlag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.used_for_long_term_reference, settings, "uint32_t", "used_for_long_term_reference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.discardable_flag, settings, "uint32_t", "discardable_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.cross_layer_bla_flag, settings, "uint32_t", "cross_layer_bla_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.pic_output_flag, settings, "uint32_t", "pic_output_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.no_output_of_prior_pics_flag, settings, "uint32_t", "no_output_of_prior_pics_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.short_term_ref_pic_set_sps_flag, settings, "uint32_t", "short_term_ref_pic_set_sps_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.slice_temporal_mvp_enabled_flag, settings, "uint32_t", "slice_temporal_mvp_enabled_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeH265LongTermRefPics(const StdVideoEncodeH265LongTermRefPics& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint8_t>(object.num_long_term_sps, settings, "uint8_t", "num_long_term_sps", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_long_term_pics, settings, "uint8_t", "num_long_term_pics", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.lt_idx_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, "uint8_t[STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS]", "uint8_t", "lt_idx_sps", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.poc_lsb_lt, STD_VIDEO_H265_MAX_LONG_TERM_PICS, settings, "uint8_t[STD_VIDEO_H265_MAX_LONG_TERM_PICS]", "uint8_t", "poc_lsb_lt", indents + 1, dump_text_uint8_t); // AQA
    dump_text_value<const uint16_t>(object.used_by_curr_pic_lt_flag, settings, "uint16_t", "used_by_curr_pic_lt_flag", indents + 1, dump_text_uint16_t);  // AET
    dump_text_array<const uint8_t>(object.delta_poc_msb_present_flag, STD_VIDEO_H265_MAX_DELTA_POC, settings, "uint8_t[STD_VIDEO_H265_MAX_DELTA_POC]", "uint8_t", "delta_poc_msb_present_flag", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.delta_poc_msb_cycle_lt, STD_VIDEO_H265_MAX_DELTA_POC, settings, "uint8_t[STD_VIDEO_H265_MAX_DELTA_POC]", "uint8_t", "delta_poc_msb_cycle_lt", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoEncodeH265PictureInfo(const StdVideoEncodeH265PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH265PictureInfoFlags>(object.flags, settings, "StdVideoEncodeH265PictureInfoFlags", "flags", indents + 1, dump_text_StdVideoEncodeH265PictureInfoFlags);  // AET
    dump_text_value<const StdVideoH265PictureType>(object.pic_type, settings, "StdVideoH265PictureType", "pic_type", indents + 1, dump_text_StdVideoH265PictureType);  // AET
    dump_text_value<const uint8_t>(object.sps_video_parameter_set_id, settings, "uint8_t", "sps_video_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pps_seq_parameter_set_id, settings, "uint8_t", "pps_seq_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.pps_pic_parameter_set_id, settings, "uint8_t", "pps_pic_parameter_set_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.short_term_ref_pic_set_idx, settings, "uint8_t", "short_term_ref_pic_set_idx", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const int32_t>(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + 1, dump_text_int32_t);  // AET
    dump_text_value<const uint8_t>(object.TemporalId, settings, "uint8_t", "TemporalId", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.reserved1, 7, settings, "uint8_t[7]", "uint8_t", "reserved1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_pointer<const StdVideoEncodeH265ReferenceListsInfo>(object.pRefLists, settings, "const StdVideoEncodeH265ReferenceListsInfo*", "pRefLists", indents + 1, dump_text_StdVideoEncodeH265ReferenceListsInfo);
    dump_text_pointer<const StdVideoH265ShortTermRefPicSet>(object.pShortTermRefPicSet, settings, "const StdVideoH265ShortTermRefPicSet*", "pShortTermRefPicSet", indents + 1, dump_text_StdVideoH265ShortTermRefPicSet);
    dump_text_pointer<const StdVideoEncodeH265LongTermRefPics>(object.pLongTermRefPics, settings, "const StdVideoEncodeH265LongTermRefPics*", "pLongTermRefPics", indents + 1, dump_text_StdVideoEncodeH265LongTermRefPics);
}
void dump_text_StdVideoEncodeH265ReferenceInfoFlags(const StdVideoEncodeH265ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.used_for_long_term_reference, settings, "uint32_t", "used_for_long_term_reference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.unused_for_reference, settings, "uint32_t", "unused_for_reference", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeH265ReferenceInfo(const StdVideoEncodeH265ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeH265ReferenceInfoFlags>(object.flags, settings, "StdVideoEncodeH265ReferenceInfoFlags", "flags", indents + 1, dump_text_StdVideoEncodeH265ReferenceInfoFlags);  // AET
    dump_text_value<const StdVideoH265PictureType>(object.pic_type, settings, "StdVideoH265PictureType", "pic_type", indents + 1, dump_text_StdVideoH265PictureType);  // AET
    dump_text_value<const int32_t>(object.PicOrderCntVal, settings, "int32_t", "PicOrderCntVal", indents + 1, dump_text_int32_t);  // AET
    dump_text_value<const uint8_t>(object.TemporalId, settings, "uint8_t", "TemporalId", indents + 1, dump_text_uint8_t);  // AET
}
void dump_text_StdVideoAV1ColorConfigFlags(const StdVideoAV1ColorConfigFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.mono_chrome, settings, "uint32_t", "mono_chrome", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.color_range, settings, "uint32_t", "color_range", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.separate_uv_delta_q, settings, "uint32_t", "separate_uv_delta_q", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.color_description_present_flag, settings, "uint32_t", "color_description_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoAV1ColorConfig(const StdVideoAV1ColorConfig& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoAV1ColorConfigFlags>(object.flags, settings, "StdVideoAV1ColorConfigFlags", "flags", indents + 1, dump_text_StdVideoAV1ColorConfigFlags);  // AET
    dump_text_value<const uint8_t>(object.BitDepth, settings, "uint8_t", "BitDepth", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.subsampling_x, settings, "uint8_t", "subsampling_x", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.subsampling_y, settings, "uint8_t", "subsampling_y", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const StdVideoAV1ColorPrimaries>(object.color_primaries, settings, "StdVideoAV1ColorPrimaries", "color_primaries", indents + 1, dump_text_StdVideoAV1ColorPrimaries);  // AET
    dump_text_value<const StdVideoAV1TransferCharacteristics>(object.transfer_characteristics, settings, "StdVideoAV1TransferCharacteristics", "transfer_characteristics", indents + 1, dump_text_StdVideoAV1TransferCharacteristics);  // AET
    dump_text_value<const StdVideoAV1MatrixCoefficients>(object.matrix_coefficients, settings, "StdVideoAV1MatrixCoefficients", "matrix_coefficients", indents + 1, dump_text_StdVideoAV1MatrixCoefficients);  // AET
    dump_text_value<const StdVideoAV1ChromaSamplePosition>(object.chroma_sample_position, settings, "StdVideoAV1ChromaSamplePosition", "chroma_sample_position", indents + 1, dump_text_StdVideoAV1ChromaSamplePosition);  // AET
}
void dump_text_StdVideoAV1TimingInfoFlags(const StdVideoAV1TimingInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.equal_picture_interval, settings, "uint32_t", "equal_picture_interval", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoAV1TimingInfo(const StdVideoAV1TimingInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoAV1TimingInfoFlags>(object.flags, settings, "StdVideoAV1TimingInfoFlags", "flags", indents + 1, dump_text_StdVideoAV1TimingInfoFlags);  // AET
    dump_text_value<const uint32_t>(object.num_units_in_display_tick, settings, "uint32_t", "num_units_in_display_tick", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.time_scale, settings, "uint32_t", "time_scale", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.num_ticks_per_picture_minus_1, settings, "uint32_t", "num_ticks_per_picture_minus_1", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoAV1LoopFilterFlags(const StdVideoAV1LoopFilterFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.loop_filter_delta_enabled, settings, "uint32_t", "loop_filter_delta_enabled", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.loop_filter_delta_update, settings, "uint32_t", "loop_filter_delta_update", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoAV1LoopFilter(const StdVideoAV1LoopFilter& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoAV1LoopFilterFlags>(object.flags, settings, "StdVideoAV1LoopFilterFlags", "flags", indents + 1, dump_text_StdVideoAV1LoopFilterFlags);  // AET
    dump_text_array<const uint8_t>(object.loop_filter_level, STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS]", "uint8_t", "loop_filter_level", indents + 1, dump_text_uint8_t); // AQA
    dump_text_value<const uint8_t>(object.loop_filter_sharpness, settings, "uint8_t", "loop_filter_sharpness", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.update_ref_delta, settings, "uint8_t", "update_ref_delta", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const int8_t>(object.loop_filter_ref_deltas, STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME, settings, "int8_t[STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME]", "int8_t", "loop_filter_ref_deltas", indents + 1, dump_text_int8_t); // AQA
    dump_text_value<const uint8_t>(object.update_mode_delta, settings, "uint8_t", "update_mode_delta", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const int8_t>(object.loop_filter_mode_deltas, STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS, settings, "int8_t[STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS]", "int8_t", "loop_filter_mode_deltas", indents + 1, dump_text_int8_t); // AQA
}
void dump_text_StdVideoAV1QuantizationFlags(const StdVideoAV1QuantizationFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.using_qmatrix, settings, "uint32_t", "using_qmatrix", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.diff_uv_delta, settings, "uint32_t", "diff_uv_delta", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoAV1Quantization(const StdVideoAV1Quantization& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoAV1QuantizationFlags>(object.flags, settings, "StdVideoAV1QuantizationFlags", "flags", indents + 1, dump_text_StdVideoAV1QuantizationFlags);  // AET
    dump_text_value<const uint8_t>(object.base_q_idx, settings, "uint8_t", "base_q_idx", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const int8_t>(object.DeltaQYDc, settings, "int8_t", "DeltaQYDc", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.DeltaQUDc, settings, "int8_t", "DeltaQUDc", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.DeltaQUAc, settings, "int8_t", "DeltaQUAc", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.DeltaQVDc, settings, "int8_t", "DeltaQVDc", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.DeltaQVAc, settings, "int8_t", "DeltaQVAc", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const uint8_t>(object.qm_y, settings, "uint8_t", "qm_y", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.qm_u, settings, "uint8_t", "qm_u", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.qm_v, settings, "uint8_t", "qm_v", indents + 1, dump_text_uint8_t);  // AET
}
void dump_text_StdVideoAV1Segmentation(const StdVideoAV1Segmentation& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_array<const uint8_t>(object.FeatureEnabled, STD_VIDEO_AV1_MAX_SEGMENTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_SEGMENTS]", "uint8_t", "FeatureEnabled", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoAV1TileInfoFlags(const StdVideoAV1TileInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.uniform_tile_spacing_flag, settings, "uint32_t", "uniform_tile_spacing_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoAV1TileInfo(const StdVideoAV1TileInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoAV1TileInfoFlags>(object.flags, settings, "StdVideoAV1TileInfoFlags", "flags", indents + 1, dump_text_StdVideoAV1TileInfoFlags);  // AET
    dump_text_value<const uint8_t>(object.TileCols, settings, "uint8_t", "TileCols", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.TileRows, settings, "uint8_t", "TileRows", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint16_t>(object.context_update_tile_id, settings, "uint16_t", "context_update_tile_id", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.tile_size_bytes_minus_1, settings, "uint8_t", "tile_size_bytes_minus_1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.reserved1, 7, settings, "uint8_t[7]", "uint8_t", "reserved1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_pointer<const uint16_t>(object.pMiColStarts, settings, "const uint16_t*", "pMiColStarts", indents + 1, dump_text_uint16_t);
    dump_text_pointer<const uint16_t>(object.pMiRowStarts, settings, "const uint16_t*", "pMiRowStarts", indents + 1, dump_text_uint16_t);
    dump_text_pointer<const uint16_t>(object.pWidthInSbsMinus1, settings, "const uint16_t*", "pWidthInSbsMinus1", indents + 1, dump_text_uint16_t);
    dump_text_pointer<const uint16_t>(object.pHeightInSbsMinus1, settings, "const uint16_t*", "pHeightInSbsMinus1", indents + 1, dump_text_uint16_t);
}
void dump_text_StdVideoAV1CDEF(const StdVideoAV1CDEF& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint8_t>(object.cdef_damping_minus_3, settings, "uint8_t", "cdef_damping_minus_3", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.cdef_bits, settings, "uint8_t", "cdef_bits", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.cdef_y_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "uint8_t", "cdef_y_pri_strength", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.cdef_y_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "uint8_t", "cdef_y_sec_strength", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.cdef_uv_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "uint8_t", "cdef_uv_pri_strength", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.cdef_uv_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "uint8_t", "cdef_uv_sec_strength", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoAV1LoopRestoration(const StdVideoAV1LoopRestoration& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_array<const StdVideoAV1FrameRestorationType>(object.FrameRestorationType, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, "StdVideoAV1FrameRestorationType[STD_VIDEO_AV1_MAX_NUM_PLANES]", "StdVideoAV1FrameRestorationType", "FrameRestorationType", indents + 1, dump_text_StdVideoAV1FrameRestorationType); // AQA
    dump_text_array<const uint16_t>(object.LoopRestorationSize, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, "uint16_t[STD_VIDEO_AV1_MAX_NUM_PLANES]", "uint16_t", "LoopRestorationSize", indents + 1, dump_text_uint16_t); // AQA
}
void dump_text_StdVideoAV1GlobalMotion(const StdVideoAV1GlobalMotion& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_array<const uint8_t>(object.GmType, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint8_t", "GmType", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoAV1FilmGrainFlags(const StdVideoAV1FilmGrainFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.chroma_scaling_from_luma, settings, "uint32_t", "chroma_scaling_from_luma", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.overlap_flag, settings, "uint32_t", "overlap_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.clip_to_restricted_range, settings, "uint32_t", "clip_to_restricted_range", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.update_grain, settings, "uint32_t", "update_grain", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoAV1FilmGrain(const StdVideoAV1FilmGrain& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoAV1FilmGrainFlags>(object.flags, settings, "StdVideoAV1FilmGrainFlags", "flags", indents + 1, dump_text_StdVideoAV1FilmGrainFlags);  // AET
    dump_text_value<const uint8_t>(object.grain_scaling_minus_8, settings, "uint8_t", "grain_scaling_minus_8", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.ar_coeff_lag, settings, "uint8_t", "ar_coeff_lag", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.ar_coeff_shift_minus_6, settings, "uint8_t", "ar_coeff_shift_minus_6", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.grain_scale_shift, settings, "uint8_t", "grain_scale_shift", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint16_t>(object.grain_seed, settings, "uint16_t", "grain_seed", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.film_grain_params_ref_idx, settings, "uint8_t", "film_grain_params_ref_idx", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.num_y_points, settings, "uint8_t", "num_y_points", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.point_y_value, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_Y_POINTS]", "uint8_t", "point_y_value", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.point_y_scaling, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_Y_POINTS]", "uint8_t", "point_y_scaling", indents + 1, dump_text_uint8_t); // AQA
    dump_text_value<const uint8_t>(object.num_cb_points, settings, "uint8_t", "num_cb_points", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.point_cb_value, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CB_POINTS]", "uint8_t", "point_cb_value", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.point_cb_scaling, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CB_POINTS]", "uint8_t", "point_cb_scaling", indents + 1, dump_text_uint8_t); // AQA
    dump_text_value<const uint8_t>(object.num_cr_points, settings, "uint8_t", "num_cr_points", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.point_cr_value, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CR_POINTS]", "uint8_t", "point_cr_value", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.point_cr_scaling, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CR_POINTS]", "uint8_t", "point_cr_scaling", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const int8_t>(object.ar_coeffs_y_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_LUMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_LUMA]", "int8_t", "ar_coeffs_y_plus_128", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const int8_t>(object.ar_coeffs_cb_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_CHROMA]", "int8_t", "ar_coeffs_cb_plus_128", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const int8_t>(object.ar_coeffs_cr_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_CHROMA]", "int8_t", "ar_coeffs_cr_plus_128", indents + 1, dump_text_int8_t); // AQA
    dump_text_value<const uint8_t>(object.cb_mult, settings, "uint8_t", "cb_mult", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.cb_luma_mult, settings, "uint8_t", "cb_luma_mult", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint16_t>(object.cb_offset, settings, "uint16_t", "cb_offset", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.cr_mult, settings, "uint8_t", "cr_mult", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.cr_luma_mult, settings, "uint8_t", "cr_luma_mult", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint16_t>(object.cr_offset, settings, "uint16_t", "cr_offset", indents + 1, dump_text_uint16_t);  // AET
}
void dump_text_StdVideoAV1SequenceHeaderFlags(const StdVideoAV1SequenceHeaderFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.still_picture, settings, "uint32_t", "still_picture", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reduced_still_picture_header, settings, "uint32_t", "reduced_still_picture_header", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.use_128x128_superblock, settings, "uint32_t", "use_128x128_superblock", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_filter_intra, settings, "uint32_t", "enable_filter_intra", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_intra_edge_filter, settings, "uint32_t", "enable_intra_edge_filter", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_interintra_compound, settings, "uint32_t", "enable_interintra_compound", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_masked_compound, settings, "uint32_t", "enable_masked_compound", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_warped_motion, settings, "uint32_t", "enable_warped_motion", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_dual_filter, settings, "uint32_t", "enable_dual_filter", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_order_hint, settings, "uint32_t", "enable_order_hint", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_jnt_comp, settings, "uint32_t", "enable_jnt_comp", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_ref_frame_mvs, settings, "uint32_t", "enable_ref_frame_mvs", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_id_numbers_present_flag, settings, "uint32_t", "frame_id_numbers_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_superres, settings, "uint32_t", "enable_superres", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_cdef, settings, "uint32_t", "enable_cdef", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.enable_restoration, settings, "uint32_t", "enable_restoration", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.film_grain_params_present, settings, "uint32_t", "film_grain_params_present", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.timing_info_present_flag, settings, "uint32_t", "timing_info_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.initial_display_delay_present_flag, settings, "uint32_t", "initial_display_delay_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoAV1SequenceHeader(const StdVideoAV1SequenceHeader& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoAV1SequenceHeaderFlags>(object.flags, settings, "StdVideoAV1SequenceHeaderFlags", "flags", indents + 1, dump_text_StdVideoAV1SequenceHeaderFlags);  // AET
    dump_text_value<const StdVideoAV1Profile>(object.seq_profile, settings, "StdVideoAV1Profile", "seq_profile", indents + 1, dump_text_StdVideoAV1Profile);  // AET
    dump_text_value<const uint8_t>(object.frame_width_bits_minus_1, settings, "uint8_t", "frame_width_bits_minus_1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.frame_height_bits_minus_1, settings, "uint8_t", "frame_height_bits_minus_1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint16_t>(object.max_frame_width_minus_1, settings, "uint16_t", "max_frame_width_minus_1", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.max_frame_height_minus_1, settings, "uint16_t", "max_frame_height_minus_1", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.delta_frame_id_length_minus_2, settings, "uint8_t", "delta_frame_id_length_minus_2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.additional_frame_id_length_minus_1, settings, "uint8_t", "additional_frame_id_length_minus_1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.order_hint_bits_minus_1, settings, "uint8_t", "order_hint_bits_minus_1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.seq_force_integer_mv, settings, "uint8_t", "seq_force_integer_mv", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.seq_force_screen_content_tools, settings, "uint8_t", "seq_force_screen_content_tools", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.reserved1, 5, settings, "uint8_t[5]", "uint8_t", "reserved1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_pointer<const StdVideoAV1ColorConfig>(object.pColorConfig, settings, "const StdVideoAV1ColorConfig*", "pColorConfig", indents + 1, dump_text_StdVideoAV1ColorConfig);
    dump_text_pointer<const StdVideoAV1TimingInfo>(object.pTimingInfo, settings, "const StdVideoAV1TimingInfo*", "pTimingInfo", indents + 1, dump_text_StdVideoAV1TimingInfo);
}
void dump_text_StdVideoDecodeAV1PictureInfoFlags(const StdVideoDecodeAV1PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.error_resilient_mode, settings, "uint32_t", "error_resilient_mode", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.disable_cdf_update, settings, "uint32_t", "disable_cdf_update", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.use_superres, settings, "uint32_t", "use_superres", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.render_and_frame_size_different, settings, "uint32_t", "render_and_frame_size_different", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.allow_screen_content_tools, settings, "uint32_t", "allow_screen_content_tools", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.is_filter_switchable, settings, "uint32_t", "is_filter_switchable", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.force_integer_mv, settings, "uint32_t", "force_integer_mv", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_size_override_flag, settings, "uint32_t", "frame_size_override_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.buffer_removal_time_present_flag, settings, "uint32_t", "buffer_removal_time_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.allow_intrabc, settings, "uint32_t", "allow_intrabc", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_refs_short_signaling, settings, "uint32_t", "frame_refs_short_signaling", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.allow_high_precision_mv, settings, "uint32_t", "allow_high_precision_mv", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.is_motion_mode_switchable, settings, "uint32_t", "is_motion_mode_switchable", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.use_ref_frame_mvs, settings, "uint32_t", "use_ref_frame_mvs", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.disable_frame_end_update_cdf, settings, "uint32_t", "disable_frame_end_update_cdf", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.allow_warped_motion, settings, "uint32_t", "allow_warped_motion", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reduced_tx_set, settings, "uint32_t", "reduced_tx_set", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reference_select, settings, "uint32_t", "reference_select", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.skip_mode_present, settings, "uint32_t", "skip_mode_present", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.delta_q_present, settings, "uint32_t", "delta_q_present", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.delta_lf_present, settings, "uint32_t", "delta_lf_present", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.delta_lf_multi, settings, "uint32_t", "delta_lf_multi", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_enabled, settings, "uint32_t", "segmentation_enabled", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_update_map, settings, "uint32_t", "segmentation_update_map", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_temporal_update, settings, "uint32_t", "segmentation_temporal_update", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_update_data, settings, "uint32_t", "segmentation_update_data", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.UsesLr, settings, "uint32_t", "UsesLr", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.usesChromaLr, settings, "uint32_t", "usesChromaLr", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.apply_grain, settings, "uint32_t", "apply_grain", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoDecodeAV1PictureInfo(const StdVideoDecodeAV1PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoDecodeAV1PictureInfoFlags>(object.flags, settings, "StdVideoDecodeAV1PictureInfoFlags", "flags", indents + 1, dump_text_StdVideoDecodeAV1PictureInfoFlags);  // AET
    dump_text_value<const StdVideoAV1FrameType>(object.frame_type, settings, "StdVideoAV1FrameType", "frame_type", indents + 1, dump_text_StdVideoAV1FrameType);  // AET
    dump_text_value<const uint32_t>(object.current_frame_id, settings, "uint32_t", "current_frame_id", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint8_t>(object.OrderHint, settings, "uint8_t", "OrderHint", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.primary_ref_frame, settings, "uint8_t", "primary_ref_frame", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.refresh_frame_flags, settings, "uint8_t", "refresh_frame_flags", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const StdVideoAV1InterpolationFilter>(object.interpolation_filter, settings, "StdVideoAV1InterpolationFilter", "interpolation_filter", indents + 1, dump_text_StdVideoAV1InterpolationFilter);  // AET
    dump_text_value<const StdVideoAV1TxMode>(object.TxMode, settings, "StdVideoAV1TxMode", "TxMode", indents + 1, dump_text_StdVideoAV1TxMode);  // AET
    dump_text_value<const uint8_t>(object.delta_q_res, settings, "uint8_t", "delta_q_res", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.delta_lf_res, settings, "uint8_t", "delta_lf_res", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.SkipModeFrame, STD_VIDEO_AV1_SKIP_MODE_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_SKIP_MODE_FRAMES]", "uint8_t", "SkipModeFrame", indents + 1, dump_text_uint8_t); // AQA
    dump_text_value<const uint8_t>(object.coded_denom, settings, "uint8_t", "coded_denom", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.reserved2, 3, settings, "uint8_t[3]", "uint8_t", "reserved2", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.OrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint8_t", "OrderHints", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint32_t>(object.expectedFrameId, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint32_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint32_t", "expectedFrameId", indents + 1, dump_text_uint32_t); // AQA
    dump_text_pointer<const StdVideoAV1TileInfo>(object.pTileInfo, settings, "const StdVideoAV1TileInfo*", "pTileInfo", indents + 1, dump_text_StdVideoAV1TileInfo);
    dump_text_pointer<const StdVideoAV1Quantization>(object.pQuantization, settings, "const StdVideoAV1Quantization*", "pQuantization", indents + 1, dump_text_StdVideoAV1Quantization);
    dump_text_pointer<const StdVideoAV1Segmentation>(object.pSegmentation, settings, "const StdVideoAV1Segmentation*", "pSegmentation", indents + 1, dump_text_StdVideoAV1Segmentation);
    dump_text_pointer<const StdVideoAV1LoopFilter>(object.pLoopFilter, settings, "const StdVideoAV1LoopFilter*", "pLoopFilter", indents + 1, dump_text_StdVideoAV1LoopFilter);
    dump_text_pointer<const StdVideoAV1CDEF>(object.pCDEF, settings, "const StdVideoAV1CDEF*", "pCDEF", indents + 1, dump_text_StdVideoAV1CDEF);
    dump_text_pointer<const StdVideoAV1LoopRestoration>(object.pLoopRestoration, settings, "const StdVideoAV1LoopRestoration*", "pLoopRestoration", indents + 1, dump_text_StdVideoAV1LoopRestoration);
    dump_text_pointer<const StdVideoAV1GlobalMotion>(object.pGlobalMotion, settings, "const StdVideoAV1GlobalMotion*", "pGlobalMotion", indents + 1, dump_text_StdVideoAV1GlobalMotion);
    dump_text_pointer<const StdVideoAV1FilmGrain>(object.pFilmGrain, settings, "const StdVideoAV1FilmGrain*", "pFilmGrain", indents + 1, dump_text_StdVideoAV1FilmGrain);
}
void dump_text_StdVideoDecodeAV1ReferenceInfoFlags(const StdVideoDecodeAV1ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.disable_frame_end_update_cdf, settings, "uint32_t", "disable_frame_end_update_cdf", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_enabled, settings, "uint32_t", "segmentation_enabled", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoDecodeAV1ReferenceInfo(const StdVideoDecodeAV1ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoDecodeAV1ReferenceInfoFlags>(object.flags, settings, "StdVideoDecodeAV1ReferenceInfoFlags", "flags", indents + 1, dump_text_StdVideoDecodeAV1ReferenceInfoFlags);  // AET
    dump_text_value<const uint8_t>(object.frame_type, settings, "uint8_t", "frame_type", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.RefFrameSignBias, settings, "uint8_t", "RefFrameSignBias", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.OrderHint, settings, "uint8_t", "OrderHint", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.SavedOrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint8_t", "SavedOrderHints", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoEncodeAV1DecoderModelInfo(const StdVideoEncodeAV1DecoderModelInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint8_t>(object.buffer_delay_length_minus_1, settings, "uint8_t", "buffer_delay_length_minus_1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.buffer_removal_time_length_minus_1, settings, "uint8_t", "buffer_removal_time_length_minus_1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.frame_presentation_time_length_minus_1, settings, "uint8_t", "frame_presentation_time_length_minus_1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint32_t>(object.num_units_in_decoding_tick, settings, "uint32_t", "num_units_in_decoding_tick", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeAV1ExtensionHeader(const StdVideoEncodeAV1ExtensionHeader& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint8_t>(object.temporal_id, settings, "uint8_t", "temporal_id", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.spatial_id, settings, "uint8_t", "spatial_id", indents + 1, dump_text_uint8_t);  // AET
}
void dump_text_StdVideoEncodeAV1OperatingPointInfoFlags(const StdVideoEncodeAV1OperatingPointInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.decoder_model_present_for_this_op, settings, "uint32_t", "decoder_model_present_for_this_op", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.low_delay_mode_flag, settings, "uint32_t", "low_delay_mode_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.initial_display_delay_present_for_this_op, settings, "uint32_t", "initial_display_delay_present_for_this_op", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeAV1OperatingPointInfo(const StdVideoEncodeAV1OperatingPointInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeAV1OperatingPointInfoFlags>(object.flags, settings, "StdVideoEncodeAV1OperatingPointInfoFlags", "flags", indents + 1, dump_text_StdVideoEncodeAV1OperatingPointInfoFlags);  // AET
    dump_text_value<const uint16_t>(object.operating_point_idc, settings, "uint16_t", "operating_point_idc", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint8_t>(object.seq_level_idx, settings, "uint8_t", "seq_level_idx", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.seq_tier, settings, "uint8_t", "seq_tier", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint32_t>(object.decoder_buffer_delay, settings, "uint32_t", "decoder_buffer_delay", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.encoder_buffer_delay, settings, "uint32_t", "encoder_buffer_delay", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint8_t>(object.initial_display_delay_minus_1, settings, "uint8_t", "initial_display_delay_minus_1", indents + 1, dump_text_uint8_t);  // AET
}
void dump_text_StdVideoEncodeAV1PictureInfoFlags(const StdVideoEncodeAV1PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.error_resilient_mode, settings, "uint32_t", "error_resilient_mode", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.disable_cdf_update, settings, "uint32_t", "disable_cdf_update", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.use_superres, settings, "uint32_t", "use_superres", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.render_and_frame_size_different, settings, "uint32_t", "render_and_frame_size_different", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.allow_screen_content_tools, settings, "uint32_t", "allow_screen_content_tools", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.is_filter_switchable, settings, "uint32_t", "is_filter_switchable", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.force_integer_mv, settings, "uint32_t", "force_integer_mv", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_size_override_flag, settings, "uint32_t", "frame_size_override_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.buffer_removal_time_present_flag, settings, "uint32_t", "buffer_removal_time_present_flag", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.allow_intrabc, settings, "uint32_t", "allow_intrabc", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_refs_short_signaling, settings, "uint32_t", "frame_refs_short_signaling", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.allow_high_precision_mv, settings, "uint32_t", "allow_high_precision_mv", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.is_motion_mode_switchable, settings, "uint32_t", "is_motion_mode_switchable", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.use_ref_frame_mvs, settings, "uint32_t", "use_ref_frame_mvs", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.disable_frame_end_update_cdf, settings, "uint32_t", "disable_frame_end_update_cdf", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.allow_warped_motion, settings, "uint32_t", "allow_warped_motion", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reduced_tx_set, settings, "uint32_t", "reduced_tx_set", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.skip_mode_present, settings, "uint32_t", "skip_mode_present", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.delta_q_present, settings, "uint32_t", "delta_q_present", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.delta_lf_present, settings, "uint32_t", "delta_lf_present", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.delta_lf_multi, settings, "uint32_t", "delta_lf_multi", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_enabled, settings, "uint32_t", "segmentation_enabled", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_update_map, settings, "uint32_t", "segmentation_update_map", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_temporal_update, settings, "uint32_t", "segmentation_temporal_update", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_update_data, settings, "uint32_t", "segmentation_update_data", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.UsesLr, settings, "uint32_t", "UsesLr", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.usesChromaLr, settings, "uint32_t", "usesChromaLr", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.show_frame, settings, "uint32_t", "show_frame", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.showable_frame, settings, "uint32_t", "showable_frame", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeAV1PictureInfo(const StdVideoEncodeAV1PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeAV1PictureInfoFlags>(object.flags, settings, "StdVideoEncodeAV1PictureInfoFlags", "flags", indents + 1, dump_text_StdVideoEncodeAV1PictureInfoFlags);  // AET
    dump_text_value<const StdVideoAV1FrameType>(object.frame_type, settings, "StdVideoAV1FrameType", "frame_type", indents + 1, dump_text_StdVideoAV1FrameType);  // AET
    dump_text_value<const uint32_t>(object.frame_presentation_time, settings, "uint32_t", "frame_presentation_time", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.current_frame_id, settings, "uint32_t", "current_frame_id", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint8_t>(object.order_hint, settings, "uint8_t", "order_hint", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.primary_ref_frame, settings, "uint8_t", "primary_ref_frame", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.refresh_frame_flags, settings, "uint8_t", "refresh_frame_flags", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.coded_denom, settings, "uint8_t", "coded_denom", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint16_t>(object.render_width_minus_1, settings, "uint16_t", "render_width_minus_1", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const uint16_t>(object.render_height_minus_1, settings, "uint16_t", "render_height_minus_1", indents + 1, dump_text_uint16_t);  // AET
    dump_text_value<const StdVideoAV1InterpolationFilter>(object.interpolation_filter, settings, "StdVideoAV1InterpolationFilter", "interpolation_filter", indents + 1, dump_text_StdVideoAV1InterpolationFilter);  // AET
    dump_text_value<const StdVideoAV1TxMode>(object.TxMode, settings, "StdVideoAV1TxMode", "TxMode", indents + 1, dump_text_StdVideoAV1TxMode);  // AET
    dump_text_value<const uint8_t>(object.delta_q_res, settings, "uint8_t", "delta_q_res", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.delta_lf_res, settings, "uint8_t", "delta_lf_res", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.ref_order_hint, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint8_t", "ref_order_hint", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const int8_t>(object.ref_frame_idx, STD_VIDEO_AV1_REFS_PER_FRAME, settings, "int8_t[STD_VIDEO_AV1_REFS_PER_FRAME]", "int8_t", "ref_frame_idx", indents + 1, dump_text_int8_t); // AQA
    dump_text_array<const uint8_t>(object.reserved1, 3, settings, "uint8_t[3]", "uint8_t", "reserved1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint32_t>(object.delta_frame_id_minus_1, STD_VIDEO_AV1_REFS_PER_FRAME, settings, "uint32_t[STD_VIDEO_AV1_REFS_PER_FRAME]", "uint32_t", "delta_frame_id_minus_1", indents + 1, dump_text_uint32_t); // AQA
    dump_text_pointer<const StdVideoAV1TileInfo>(object.pTileInfo, settings, "const StdVideoAV1TileInfo*", "pTileInfo", indents + 1, dump_text_StdVideoAV1TileInfo);
    dump_text_pointer<const StdVideoAV1Quantization>(object.pQuantization, settings, "const StdVideoAV1Quantization*", "pQuantization", indents + 1, dump_text_StdVideoAV1Quantization);
    dump_text_pointer<const StdVideoAV1Segmentation>(object.pSegmentation, settings, "const StdVideoAV1Segmentation*", "pSegmentation", indents + 1, dump_text_StdVideoAV1Segmentation);
    dump_text_pointer<const StdVideoAV1LoopFilter>(object.pLoopFilter, settings, "const StdVideoAV1LoopFilter*", "pLoopFilter", indents + 1, dump_text_StdVideoAV1LoopFilter);
    dump_text_pointer<const StdVideoAV1CDEF>(object.pCDEF, settings, "const StdVideoAV1CDEF*", "pCDEF", indents + 1, dump_text_StdVideoAV1CDEF);
    dump_text_pointer<const StdVideoAV1LoopRestoration>(object.pLoopRestoration, settings, "const StdVideoAV1LoopRestoration*", "pLoopRestoration", indents + 1, dump_text_StdVideoAV1LoopRestoration);
    dump_text_pointer<const StdVideoAV1GlobalMotion>(object.pGlobalMotion, settings, "const StdVideoAV1GlobalMotion*", "pGlobalMotion", indents + 1, dump_text_StdVideoAV1GlobalMotion);
    dump_text_pointer<const StdVideoEncodeAV1ExtensionHeader>(object.pExtensionHeader, settings, "const StdVideoEncodeAV1ExtensionHeader*", "pExtensionHeader", indents + 1, dump_text_StdVideoEncodeAV1ExtensionHeader);
    dump_text_pointer<const uint32_t>(object.pBufferRemovalTimes, settings, "const uint32_t*", "pBufferRemovalTimes", indents + 1, dump_text_uint32_t);
}
void dump_text_StdVideoEncodeAV1ReferenceInfoFlags(const StdVideoEncodeAV1ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.disable_frame_end_update_cdf, settings, "uint32_t", "disable_frame_end_update_cdf", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_enabled, settings, "uint32_t", "segmentation_enabled", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoEncodeAV1ReferenceInfo(const StdVideoEncodeAV1ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoEncodeAV1ReferenceInfoFlags>(object.flags, settings, "StdVideoEncodeAV1ReferenceInfoFlags", "flags", indents + 1, dump_text_StdVideoEncodeAV1ReferenceInfoFlags);  // AET
    dump_text_value<const uint32_t>(object.RefFrameId, settings, "uint32_t", "RefFrameId", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const StdVideoAV1FrameType>(object.frame_type, settings, "StdVideoAV1FrameType", "frame_type", indents + 1, dump_text_StdVideoAV1FrameType);  // AET
    dump_text_value<const uint8_t>(object.OrderHint, settings, "uint8_t", "OrderHint", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint8_t>(object.reserved1, 3, settings, "uint8_t[3]", "uint8_t", "reserved1", indents + 1, dump_text_uint8_t); // AQA
    dump_text_pointer<const StdVideoEncodeAV1ExtensionHeader>(object.pExtensionHeader, settings, "const StdVideoEncodeAV1ExtensionHeader*", "pExtensionHeader", indents + 1, dump_text_StdVideoEncodeAV1ExtensionHeader);
}
void dump_text_StdVideoVP9ColorConfigFlags(const StdVideoVP9ColorConfigFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.color_range, settings, "uint32_t", "color_range", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoVP9ColorConfig(const StdVideoVP9ColorConfig& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoVP9ColorConfigFlags>(object.flags, settings, "StdVideoVP9ColorConfigFlags", "flags", indents + 1, dump_text_StdVideoVP9ColorConfigFlags);  // AET
    dump_text_value<const uint8_t>(object.BitDepth, settings, "uint8_t", "BitDepth", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.subsampling_x, settings, "uint8_t", "subsampling_x", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.subsampling_y, settings, "uint8_t", "subsampling_y", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reserved1, settings, "uint8_t", "reserved1", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const StdVideoVP9ColorSpace>(object.color_space, settings, "StdVideoVP9ColorSpace", "color_space", indents + 1, dump_text_StdVideoVP9ColorSpace);  // AET
}
void dump_text_StdVideoVP9LoopFilterFlags(const StdVideoVP9LoopFilterFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.loop_filter_delta_enabled, settings, "uint32_t", "loop_filter_delta_enabled", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.loop_filter_delta_update, settings, "uint32_t", "loop_filter_delta_update", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoVP9LoopFilter(const StdVideoVP9LoopFilter& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoVP9LoopFilterFlags>(object.flags, settings, "StdVideoVP9LoopFilterFlags", "flags", indents + 1, dump_text_StdVideoVP9LoopFilterFlags);  // AET
    dump_text_value<const uint8_t>(object.loop_filter_level, settings, "uint8_t", "loop_filter_level", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.loop_filter_sharpness, settings, "uint8_t", "loop_filter_sharpness", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.update_ref_delta, settings, "uint8_t", "update_ref_delta", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const int8_t>(object.loop_filter_ref_deltas, STD_VIDEO_VP9_MAX_REF_FRAMES, settings, "int8_t[STD_VIDEO_VP9_MAX_REF_FRAMES]", "int8_t", "loop_filter_ref_deltas", indents + 1, dump_text_int8_t); // AQA
    dump_text_value<const uint8_t>(object.update_mode_delta, settings, "uint8_t", "update_mode_delta", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const int8_t>(object.loop_filter_mode_deltas, STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS, settings, "int8_t[STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS]", "int8_t", "loop_filter_mode_deltas", indents + 1, dump_text_int8_t); // AQA
}
void dump_text_StdVideoVP9SegmentationFlags(const StdVideoVP9SegmentationFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.segmentation_update_map, settings, "uint32_t", "segmentation_update_map", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_temporal_update, settings, "uint32_t", "segmentation_temporal_update", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_update_data, settings, "uint32_t", "segmentation_update_data", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_abs_or_delta_update, settings, "uint32_t", "segmentation_abs_or_delta_update", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoVP9Segmentation(const StdVideoVP9Segmentation& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoVP9SegmentationFlags>(object.flags, settings, "StdVideoVP9SegmentationFlags", "flags", indents + 1, dump_text_StdVideoVP9SegmentationFlags);  // AET
    dump_text_array<const uint8_t>(object.segmentation_tree_probs, STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS]", "uint8_t", "segmentation_tree_probs", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.segmentation_pred_prob, STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB]", "uint8_t", "segmentation_pred_prob", indents + 1, dump_text_uint8_t); // AQA
    dump_text_array<const uint8_t>(object.FeatureEnabled, STD_VIDEO_VP9_MAX_SEGMENTS, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTS]", "uint8_t", "FeatureEnabled", indents + 1, dump_text_uint8_t); // AQA
}
void dump_text_StdVideoDecodeVP9PictureInfoFlags(const StdVideoDecodeVP9PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const uint32_t>(object.error_resilient_mode, settings, "uint32_t", "error_resilient_mode", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.intra_only, settings, "uint32_t", "intra_only", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.allow_high_precision_mv, settings, "uint32_t", "allow_high_precision_mv", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.refresh_frame_context, settings, "uint32_t", "refresh_frame_context", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.frame_parallel_decoding_mode, settings, "uint32_t", "frame_parallel_decoding_mode", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.segmentation_enabled, settings, "uint32_t", "segmentation_enabled", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.show_frame, settings, "uint32_t", "show_frame", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.UsePrevFrameMvs, settings, "uint32_t", "UsePrevFrameMvs", indents + 1, dump_text_uint32_t);  // AET
    dump_text_value<const uint32_t>(object.reserved, settings, "uint32_t", "reserved", indents + 1, dump_text_uint32_t);  // AET
}
void dump_text_StdVideoDecodeVP9PictureInfo(const StdVideoDecodeVP9PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    if(settings.showAddress())
        settings.stream() << &object << ":\n";
    else
        settings.stream() << "address:\n";
    dump_text_value<const StdVideoDecodeVP9PictureInfoFlags>(object.flags, settings, "StdVideoDecodeVP9PictureInfoFlags", "flags", indents + 1, dump_text_StdVideoDecodeVP9PictureInfoFlags);  // AET
    dump_text_value<const StdVideoVP9Profile>(object.profile, settings, "StdVideoVP9Profile", "profile", indents + 1, dump_text_StdVideoVP9Profile);  // AET
    dump_text_value<const StdVideoVP9FrameType>(object.frame_type, settings, "StdVideoVP9FrameType", "frame_type", indents + 1, dump_text_StdVideoVP9FrameType);  // AET
    dump_text_value<const uint8_t>(object.frame_context_idx, settings, "uint8_t", "frame_context_idx", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.reset_frame_context, settings, "uint8_t", "reset_frame_context", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.refresh_frame_flags, settings, "uint8_t", "refresh_frame_flags", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.ref_frame_sign_bias_mask, settings, "uint8_t", "ref_frame_sign_bias_mask", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const StdVideoVP9InterpolationFilter>(object.interpolation_filter, settings, "StdVideoVP9InterpolationFilter", "interpolation_filter", indents + 1, dump_text_StdVideoVP9InterpolationFilter);  // AET
    dump_text_value<const uint8_t>(object.base_q_idx, settings, "uint8_t", "base_q_idx", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const int8_t>(object.delta_q_y_dc, settings, "int8_t", "delta_q_y_dc", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.delta_q_uv_dc, settings, "int8_t", "delta_q_uv_dc", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const int8_t>(object.delta_q_uv_ac, settings, "int8_t", "delta_q_uv_ac", indents + 1, dump_text_int8_t);  // AET
    dump_text_value<const uint8_t>(object.tile_cols_log2, settings, "uint8_t", "tile_cols_log2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_value<const uint8_t>(object.tile_rows_log2, settings, "uint8_t", "tile_rows_log2", indents + 1, dump_text_uint8_t);  // AET
    dump_text_array<const uint16_t>(object.reserved1, 3, settings, "uint16_t[3]", "uint16_t", "reserved1", indents + 1, dump_text_uint16_t); // AQA
    dump_text_pointer<const StdVideoVP9ColorConfig>(object.pColorConfig, settings, "const StdVideoVP9ColorConfig*", "pColorConfig", indents + 1, dump_text_StdVideoVP9ColorConfig);
    dump_text_pointer<const StdVideoVP9LoopFilter>(object.pLoopFilter, settings, "const StdVideoVP9LoopFilter*", "pLoopFilter", indents + 1, dump_text_StdVideoVP9LoopFilter);
    dump_text_pointer<const StdVideoVP9Segmentation>(object.pSegmentation, settings, "const StdVideoVP9Segmentation*", "pSegmentation", indents + 1, dump_text_StdVideoVP9Segmentation);
}

//========================== Union Implementations ==========================//


//======================== pNext Chain Implementation =======================//


//========================== Function Helpers ===============================//


//========================= Function Implementations ========================//



