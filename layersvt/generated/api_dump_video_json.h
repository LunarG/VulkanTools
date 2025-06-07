
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

#include "api_dump_json.h"

//=========================== Type Implementations ==========================//

void dump_json_uint32_t(uint32_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << "\"" << object << "\"";
}
void dump_json_uint8_t(uint8_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << "\"" << (uint32_t) object << "\"";
}
void dump_json_uint16_t(uint16_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << "\"" << object << "\"";
}
void dump_json_int32_t(int32_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << "\"" << object << "\"";
}
void dump_json_int8_t(int8_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << "\"" << (int32_t) object << "\"";
}
void dump_json_int16_t(int16_t object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << "\"" << object << "\"";
}

//========================= Basetype Implementations ========================//






//======================= System Type Implementations =======================//


//========================== Handle Implementations =========================//


//=========================== Enum Implementations ==========================//

void dump_json_StdVideoH264ChromaFormatIdc(StdVideoH264ChromaFormatIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_CHROMA_FORMAT_IDC_MONOCHROME\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_CHROMA_FORMAT_IDC_420\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_CHROMA_FORMAT_IDC_422\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H264_CHROMA_FORMAT_IDC_444\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_CHROMA_FORMAT_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264ProfileIdc(StdVideoH264ProfileIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 66:
        settings.stream() << "\"STD_VIDEO_H264_PROFILE_IDC_BASELINE\"";
        break;
    case 77:
        settings.stream() << "\"STD_VIDEO_H264_PROFILE_IDC_MAIN\"";
        break;
    case 100:
        settings.stream() << "\"STD_VIDEO_H264_PROFILE_IDC_HIGH\"";
        break;
    case 244:
        settings.stream() << "\"STD_VIDEO_H264_PROFILE_IDC_HIGH_444_PREDICTIVE\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_PROFILE_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264LevelIdc(StdVideoH264LevelIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_1_0\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_1_1\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_1_2\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_1_3\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_2_0\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_2_1\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_2_2\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_3_0\"";
        break;
    case 8:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_3_1\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_3_2\"";
        break;
    case 10:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_4_0\"";
        break;
    case 11:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_4_1\"";
        break;
    case 12:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_4_2\"";
        break;
    case 13:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_5_0\"";
        break;
    case 14:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_5_1\"";
        break;
    case 15:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_5_2\"";
        break;
    case 16:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_6_0\"";
        break;
    case 17:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_6_1\"";
        break;
    case 18:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_6_2\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_LEVEL_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264PocType(StdVideoH264PocType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_POC_TYPE_0\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_POC_TYPE_1\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_POC_TYPE_2\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_POC_TYPE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264AspectRatioIdc(StdVideoH264AspectRatioIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_UNSPECIFIED\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_SQUARE\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_12_11\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_10_11\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_16_11\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_40_33\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_24_11\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_20_11\"";
        break;
    case 8:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_32_11\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_80_33\"";
        break;
    case 10:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_18_11\"";
        break;
    case 11:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_15_11\"";
        break;
    case 12:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_64_33\"";
        break;
    case 13:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_160_99\"";
        break;
    case 14:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_4_3\"";
        break;
    case 15:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_3_2\"";
        break;
    case 16:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_2_1\"";
        break;
    case 255:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_EXTENDED_SAR\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_ASPECT_RATIO_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264WeightedBipredIdc(StdVideoH264WeightedBipredIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_DEFAULT\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_EXPLICIT\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_IMPLICIT\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_WEIGHTED_BIPRED_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264ModificationOfPicNumsIdc(StdVideoH264ModificationOfPicNumsIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_SUBTRACT\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_SHORT_TERM_ADD\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_LONG_TERM\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_END\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_MODIFICATION_OF_PIC_NUMS_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264MemMgmtControlOp(StdVideoH264MemMgmtControlOp object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_END\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_SHORT_TERM\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_LONG_TERM\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_LONG_TERM\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_SET_MAX_LONG_TERM_INDEX\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_UNMARK_ALL\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_MARK_CURRENT_AS_LONG_TERM\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_MEM_MGMT_CONTROL_OP_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264CabacInitIdc(StdVideoH264CabacInitIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_CABAC_INIT_IDC_0\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_CABAC_INIT_IDC_1\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_CABAC_INIT_IDC_2\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_CABAC_INIT_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264DisableDeblockingFilterIdc(StdVideoH264DisableDeblockingFilterIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_DISABLED\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_ENABLED\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_PARTIAL\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_DISABLE_DEBLOCKING_FILTER_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264SliceType(StdVideoH264SliceType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_SLICE_TYPE_P\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_SLICE_TYPE_B\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_SLICE_TYPE_I\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_SLICE_TYPE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264PictureType(StdVideoH264PictureType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_PICTURE_TYPE_P\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_PICTURE_TYPE_B\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_PICTURE_TYPE_I\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_H264_PICTURE_TYPE_IDR\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_PICTURE_TYPE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH264NonVclNaluType(StdVideoH264NonVclNaluType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H264_NON_VCL_NALU_TYPE_SPS\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H264_NON_VCL_NALU_TYPE_PPS\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H264_NON_VCL_NALU_TYPE_AUD\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H264_NON_VCL_NALU_TYPE_PREFIX\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_SEQUENCE\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_H264_NON_VCL_NALU_TYPE_END_OF_STREAM\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_H264_NON_VCL_NALU_TYPE_PRECODED\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H264_NON_VCL_NALU_TYPE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoDecodeH264FieldOrderCount(StdVideoDecodeH264FieldOrderCount object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_TOP\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_BOTTOM\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH265ChromaFormatIdc(StdVideoH265ChromaFormatIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H265_CHROMA_FORMAT_IDC_MONOCHROME\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H265_CHROMA_FORMAT_IDC_420\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H265_CHROMA_FORMAT_IDC_422\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H265_CHROMA_FORMAT_IDC_444\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H265_CHROMA_FORMAT_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH265ProfileIdc(StdVideoH265ProfileIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 1:
        settings.stream() << "\"STD_VIDEO_H265_PROFILE_IDC_MAIN\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H265_PROFILE_IDC_MAIN_10\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H265_PROFILE_IDC_MAIN_STILL_PICTURE\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_H265_PROFILE_IDC_FORMAT_RANGE_EXTENSIONS\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_H265_PROFILE_IDC_SCC_EXTENSIONS\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H265_PROFILE_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH265LevelIdc(StdVideoH265LevelIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_1_0\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_2_0\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_2_1\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_3_0\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_3_1\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_4_0\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_4_1\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_5_0\"";
        break;
    case 8:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_5_1\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_5_2\"";
        break;
    case 10:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_6_0\"";
        break;
    case 11:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_6_1\"";
        break;
    case 12:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_6_2\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H265_LEVEL_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH265SliceType(StdVideoH265SliceType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H265_SLICE_TYPE_B\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H265_SLICE_TYPE_P\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H265_SLICE_TYPE_I\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H265_SLICE_TYPE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH265PictureType(StdVideoH265PictureType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H265_PICTURE_TYPE_P\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H265_PICTURE_TYPE_B\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H265_PICTURE_TYPE_I\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H265_PICTURE_TYPE_IDR\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H265_PICTURE_TYPE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoH265AspectRatioIdc(StdVideoH265AspectRatioIdc object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_UNSPECIFIED\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_SQUARE\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_12_11\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_10_11\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_16_11\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_40_33\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_24_11\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_20_11\"";
        break;
    case 8:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_32_11\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_80_33\"";
        break;
    case 10:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_18_11\"";
        break;
    case 11:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_15_11\"";
        break;
    case 12:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_64_33\"";
        break;
    case 13:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_160_99\"";
        break;
    case 14:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_4_3\"";
        break;
    case 15:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_3_2\"";
        break;
    case 16:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_2_1\"";
        break;
    case 255:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_EXTENDED_SAR\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_H265_ASPECT_RATIO_IDC_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1Profile(StdVideoAV1Profile object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_PROFILE_MAIN\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_PROFILE_HIGH\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_PROFILE_PROFESSIONAL\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_PROFILE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1Level(StdVideoAV1Level object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_2_0\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_2_1\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_2_2\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_2_3\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_3_0\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_3_1\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_3_2\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_3_3\"";
        break;
    case 8:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_4_0\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_4_1\"";
        break;
    case 10:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_4_2\"";
        break;
    case 11:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_4_3\"";
        break;
    case 12:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_5_0\"";
        break;
    case 13:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_5_1\"";
        break;
    case 14:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_5_2\"";
        break;
    case 15:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_5_3\"";
        break;
    case 16:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_6_0\"";
        break;
    case 17:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_6_1\"";
        break;
    case 18:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_6_2\"";
        break;
    case 19:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_6_3\"";
        break;
    case 20:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_7_0\"";
        break;
    case 21:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_7_1\"";
        break;
    case 22:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_7_2\"";
        break;
    case 23:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_7_3\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_LEVEL_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1FrameType(StdVideoAV1FrameType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_TYPE_KEY\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_TYPE_INTER\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_TYPE_INTRA_ONLY\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_TYPE_SWITCH\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_TYPE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1ReferenceName(StdVideoAV1ReferenceName object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_REFERENCE_NAME_INTRA_FRAME\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_REFERENCE_NAME_LAST_FRAME\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_REFERENCE_NAME_LAST2_FRAME\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_AV1_REFERENCE_NAME_LAST3_FRAME\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_AV1_REFERENCE_NAME_GOLDEN_FRAME\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_AV1_REFERENCE_NAME_BWDREF_FRAME\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_AV1_REFERENCE_NAME_ALTREF2_FRAME\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_AV1_REFERENCE_NAME_ALTREF_FRAME\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_REFERENCE_NAME_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1InterpolationFilter(StdVideoAV1InterpolationFilter object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_INTERPOLATION_FILTER_EIGHTTAP_SHARP\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_AV1_INTERPOLATION_FILTER_BILINEAR\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_AV1_INTERPOLATION_FILTER_SWITCHABLE\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_INTERPOLATION_FILTER_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1TxMode(StdVideoAV1TxMode object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_TX_MODE_ONLY_4X4\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_TX_MODE_LARGEST\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_TX_MODE_SELECT\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_TX_MODE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1FrameRestorationType(StdVideoAV1FrameRestorationType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_NONE\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_WIENER\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SGRPROJ\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_SWITCHABLE\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_FRAME_RESTORATION_TYPE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1ColorPrimaries(StdVideoAV1ColorPrimaries object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_BT_709\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_UNSPECIFIED\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_M\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_BT_470_B_G\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_BT_601\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_240\"";
        break;
    case 8:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_GENERIC_FILM\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_BT_2020\"";
        break;
    case 10:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_XYZ\"";
        break;
    case 11:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_431\"";
        break;
    case 12:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_SMPTE_432\"";
        break;
    case 22:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_EBU_3213\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_COLOR_PRIMARIES_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1TransferCharacteristics(StdVideoAV1TransferCharacteristics object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_0\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_709\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_UNSPECIFIED\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_RESERVED_3\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_M\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_470_B_G\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_601\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_240\"";
        break;
    case 8:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LINEAR\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100\"";
        break;
    case 10:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_LOG_100_SQRT10\"";
        break;
    case 11:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_IEC_61966\"";
        break;
    case 12:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_1361\"";
        break;
    case 13:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SRGB\"";
        break;
    case 14:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_10_BIT\"";
        break;
    case 15:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_BT_2020_12_BIT\"";
        break;
    case 16:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_2084\"";
        break;
    case 17:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_SMPTE_428\"";
        break;
    case 18:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_HLG\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_TRANSFER_CHARACTERISTICS_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1MatrixCoefficients(StdVideoAV1MatrixCoefficients object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_IDENTITY\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_709\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_UNSPECIFIED\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_RESERVED_3\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_FCC\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_470_B_G\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_601\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_240\"";
        break;
    case 8:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_YCGCO\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_NCL\"";
        break;
    case 10:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_BT_2020_CL\"";
        break;
    case 11:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_SMPTE_2085\"";
        break;
    case 12:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_NCL\"";
        break;
    case 13:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_CHROMAT_CL\"";
        break;
    case 14:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_ICTCP\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_MATRIX_COEFFICIENTS_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoAV1ChromaSamplePosition(StdVideoAV1ChromaSamplePosition object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_UNKNOWN\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_VERTICAL\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_COLOCATED\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_RESERVED\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_AV1_CHROMA_SAMPLE_POSITION_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoVP9Profile(StdVideoVP9Profile object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_VP9_PROFILE_0\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_VP9_PROFILE_1\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_VP9_PROFILE_2\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_VP9_PROFILE_3\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_VP9_PROFILE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoVP9Level(StdVideoVP9Level object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_1_0\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_1_1\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_2_0\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_2_1\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_3_0\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_3_1\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_4_0\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_4_1\"";
        break;
    case 8:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_5_0\"";
        break;
    case 9:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_5_1\"";
        break;
    case 10:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_5_2\"";
        break;
    case 11:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_6_0\"";
        break;
    case 12:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_6_1\"";
        break;
    case 13:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_6_2\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_VP9_LEVEL_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoVP9FrameType(StdVideoVP9FrameType object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_VP9_FRAME_TYPE_KEY\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_VP9_FRAME_TYPE_NON_KEY\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_VP9_FRAME_TYPE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoVP9ReferenceName(StdVideoVP9ReferenceName object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_VP9_REFERENCE_NAME_INTRA_FRAME\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_VP9_REFERENCE_NAME_LAST_FRAME\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_VP9_REFERENCE_NAME_GOLDEN_FRAME\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_VP9_REFERENCE_NAME_ALTREF_FRAME\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_VP9_REFERENCE_NAME_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoVP9InterpolationFilter(StdVideoVP9InterpolationFilter object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SMOOTH\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_VP9_INTERPOLATION_FILTER_EIGHTTAP_SHARP\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_VP9_INTERPOLATION_FILTER_BILINEAR\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_VP9_INTERPOLATION_FILTER_SWITCHABLE\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_VP9_INTERPOLATION_FILTER_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}
void dump_json_StdVideoVP9ColorSpace(StdVideoVP9ColorSpace object, const ApiDumpSettings& settings, int indents)
{
    switch((int64_t) object)
    {
    case 0:
        settings.stream() << "\"STD_VIDEO_VP9_COLOR_SPACE_UNKNOWN\"";
        break;
    case 1:
        settings.stream() << "\"STD_VIDEO_VP9_COLOR_SPACE_BT_601\"";
        break;
    case 2:
        settings.stream() << "\"STD_VIDEO_VP9_COLOR_SPACE_BT_709\"";
        break;
    case 3:
        settings.stream() << "\"STD_VIDEO_VP9_COLOR_SPACE_SMPTE_170\"";
        break;
    case 4:
        settings.stream() << "\"STD_VIDEO_VP9_COLOR_SPACE_SMPTE_240\"";
        break;
    case 5:
        settings.stream() << "\"STD_VIDEO_VP9_COLOR_SPACE_BT_2020\"";
        break;
    case 6:
        settings.stream() << "\"STD_VIDEO_VP9_COLOR_SPACE_RESERVED\"";
        break;
    case 7:
        settings.stream() << "\"STD_VIDEO_VP9_COLOR_SPACE_RGB\"";
        break;
    case 0x7FFFFFFF:
        settings.stream() << "\"STD_VIDEO_VP9_COLOR_SPACE_INVALID\"";
        break;
    default:
        settings.stream() << "\"UNKNOWN (" << object << ")\"";
    }
}

//========================= Bitmask Implementations =========================//


//=========================== Flag Implementations ==========================//




//======================= Func Pointer Implementations ======================//


//======================== Union Forward Declarations =======================//


//======================== pNext Chain Declarations =======================//


//========================== Struct Implementations =========================//

void dump_json_StdVideoH264SpsVuiFlags(const StdVideoH264SpsVuiFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.aspect_ratio_info_present_flag, NULL, settings, "uint32_t", "aspect_ratio_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.overscan_info_present_flag, NULL, settings, "uint32_t", "overscan_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.overscan_appropriate_flag, NULL, settings, "uint32_t", "overscan_appropriate_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.video_signal_type_present_flag, NULL, settings, "uint32_t", "video_signal_type_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.video_full_range_flag, NULL, settings, "uint32_t", "video_full_range_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.color_description_present_flag, NULL, settings, "uint32_t", "color_description_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.chroma_loc_info_present_flag, NULL, settings, "uint32_t", "chroma_loc_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.timing_info_present_flag, NULL, settings, "uint32_t", "timing_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.fixed_frame_rate_flag, NULL, settings, "uint32_t", "fixed_frame_rate_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.bitstream_restriction_flag, NULL, settings, "uint32_t", "bitstream_restriction_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.nal_hrd_parameters_present_flag, NULL, settings, "uint32_t", "nal_hrd_parameters_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vcl_hrd_parameters_present_flag, NULL, settings, "uint32_t", "vcl_hrd_parameters_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH264HrdParameters(const StdVideoH264HrdParameters& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint8_t>(object.cpb_cnt_minus1, NULL, settings, "uint8_t", "cpb_cnt_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.bit_rate_scale, NULL, settings, "uint8_t", "bit_rate_scale", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.cpb_size_scale, NULL, settings, "uint8_t", "cpb_size_scale", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint32_t>(object.bit_rate_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "uint32_t", "bit_rate_value_minus1", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint32_t>(object.cpb_size_value_minus1, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "uint32_t", "cpb_size_value_minus1", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.cbr_flag, STD_VIDEO_H264_CPB_CNT_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H264_CPB_CNT_LIST_SIZE]", "uint8_t", "cbr_flag", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.initial_cpb_removal_delay_length_minus1, NULL, settings, "uint32_t", "initial_cpb_removal_delay_length_minus1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.cpb_removal_delay_length_minus1, NULL, settings, "uint32_t", "cpb_removal_delay_length_minus1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.dpb_output_delay_length_minus1, NULL, settings, "uint32_t", "dpb_output_delay_length_minus1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.time_offset_length, NULL, settings, "uint32_t", "time_offset_length", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH264SequenceParameterSetVui(const StdVideoH264SequenceParameterSetVui& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH264SpsVuiFlags>(object.flags, NULL, settings, "StdVideoH264SpsVuiFlags", "flags", true, false, indents + 1, dump_json_StdVideoH264SpsVuiFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264AspectRatioIdc>(object.aspect_ratio_idc, NULL, settings, "StdVideoH264AspectRatioIdc", "aspect_ratio_idc", false, false, indents + 1, dump_json_StdVideoH264AspectRatioIdc);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.sar_width, NULL, settings, "uint16_t", "sar_width", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.sar_height, NULL, settings, "uint16_t", "sar_height", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.video_format, NULL, settings, "uint8_t", "video_format", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.colour_primaries, NULL, settings, "uint8_t", "colour_primaries", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.transfer_characteristics, NULL, settings, "uint8_t", "transfer_characteristics", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.matrix_coefficients, NULL, settings, "uint8_t", "matrix_coefficients", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.num_units_in_tick, NULL, settings, "uint32_t", "num_units_in_tick", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.time_scale, NULL, settings, "uint32_t", "time_scale", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.max_num_reorder_frames, NULL, settings, "uint8_t", "max_num_reorder_frames", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.max_dec_frame_buffering, NULL, settings, "uint8_t", "max_dec_frame_buffering", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.chroma_sample_loc_type_top_field, NULL, settings, "uint8_t", "chroma_sample_loc_type_top_field", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.chroma_sample_loc_type_bottom_field, NULL, settings, "uint8_t", "chroma_sample_loc_type_bottom_field", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved1, NULL, settings, "uint32_t", "reserved1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH264HrdParameters>(object.pHrdParameters, settings, "const StdVideoH264HrdParameters*", "pHrdParameters", true, false, indents + 1, dump_json_StdVideoH264HrdParameters);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH264SpsFlags(const StdVideoH264SpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.constraint_set0_flag, NULL, settings, "uint32_t", "constraint_set0_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.constraint_set1_flag, NULL, settings, "uint32_t", "constraint_set1_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.constraint_set2_flag, NULL, settings, "uint32_t", "constraint_set2_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.constraint_set3_flag, NULL, settings, "uint32_t", "constraint_set3_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.constraint_set4_flag, NULL, settings, "uint32_t", "constraint_set4_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.constraint_set5_flag, NULL, settings, "uint32_t", "constraint_set5_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.direct_8x8_inference_flag, NULL, settings, "uint32_t", "direct_8x8_inference_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.mb_adaptive_frame_field_flag, NULL, settings, "uint32_t", "mb_adaptive_frame_field_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_mbs_only_flag, NULL, settings, "uint32_t", "frame_mbs_only_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.delta_pic_order_always_zero_flag, NULL, settings, "uint32_t", "delta_pic_order_always_zero_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.separate_colour_plane_flag, NULL, settings, "uint32_t", "separate_colour_plane_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.gaps_in_frame_num_value_allowed_flag, NULL, settings, "uint32_t", "gaps_in_frame_num_value_allowed_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.qpprime_y_zero_transform_bypass_flag, NULL, settings, "uint32_t", "qpprime_y_zero_transform_bypass_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_cropping_flag, NULL, settings, "uint32_t", "frame_cropping_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.seq_scaling_matrix_present_flag, NULL, settings, "uint32_t", "seq_scaling_matrix_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vui_parameters_present_flag, NULL, settings, "uint32_t", "vui_parameters_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH264ScalingLists(const StdVideoH264ScalingLists& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint16_t>(object.scaling_list_present_mask, NULL, settings, "uint16_t", "scaling_list_present_mask", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.use_default_scaling_matrix_mask, NULL, settings, "uint16_t", "use_default_scaling_matrix_mask", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH264SequenceParameterSet(const StdVideoH264SequenceParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH264SpsFlags>(object.flags, NULL, settings, "StdVideoH264SpsFlags", "flags", true, false, indents + 1, dump_json_StdVideoH264SpsFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264ProfileIdc>(object.profile_idc, NULL, settings, "StdVideoH264ProfileIdc", "profile_idc", false, false, indents + 1, dump_json_StdVideoH264ProfileIdc);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264LevelIdc>(object.level_idc, NULL, settings, "StdVideoH264LevelIdc", "level_idc", false, false, indents + 1, dump_json_StdVideoH264LevelIdc);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264ChromaFormatIdc>(object.chroma_format_idc, NULL, settings, "StdVideoH264ChromaFormatIdc", "chroma_format_idc", false, false, indents + 1, dump_json_StdVideoH264ChromaFormatIdc);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.seq_parameter_set_id, NULL, settings, "uint8_t", "seq_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.bit_depth_luma_minus8, NULL, settings, "uint8_t", "bit_depth_luma_minus8", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.bit_depth_chroma_minus8, NULL, settings, "uint8_t", "bit_depth_chroma_minus8", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_max_frame_num_minus4, NULL, settings, "uint8_t", "log2_max_frame_num_minus4", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264PocType>(object.pic_order_cnt_type, NULL, settings, "StdVideoH264PocType", "pic_order_cnt_type", false, false, indents + 1, dump_json_StdVideoH264PocType);
    settings.stream() << ",\n";
    dump_json_value<const int32_t>(object.offset_for_non_ref_pic, NULL, settings, "int32_t", "offset_for_non_ref_pic", false, false, indents + 1, dump_json_int32_t);
    settings.stream() << ",\n";
    dump_json_value<const int32_t>(object.offset_for_top_to_bottom_field, NULL, settings, "int32_t", "offset_for_top_to_bottom_field", false, false, indents + 1, dump_json_int32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_max_pic_order_cnt_lsb_minus4, NULL, settings, "uint8_t", "log2_max_pic_order_cnt_lsb_minus4", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_ref_frames_in_pic_order_cnt_cycle, NULL, settings, "uint8_t", "num_ref_frames_in_pic_order_cnt_cycle", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.max_num_ref_frames, NULL, settings, "uint8_t", "max_num_ref_frames", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pic_width_in_mbs_minus1, NULL, settings, "uint32_t", "pic_width_in_mbs_minus1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pic_height_in_map_units_minus1, NULL, settings, "uint32_t", "pic_height_in_map_units_minus1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_crop_left_offset, NULL, settings, "uint32_t", "frame_crop_left_offset", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_crop_right_offset, NULL, settings, "uint32_t", "frame_crop_right_offset", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_crop_top_offset, NULL, settings, "uint32_t", "frame_crop_top_offset", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_crop_bottom_offset, NULL, settings, "uint32_t", "frame_crop_bottom_offset", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved2, NULL, settings, "uint32_t", "reserved2", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_pointer<const int32_t>(object.pOffsetForRefFrame, settings, "const int32_t*", "pOffsetForRefFrame", false, false, indents + 1, dump_json_int32_t);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH264ScalingLists>(object.pScalingLists, settings, "const StdVideoH264ScalingLists*", "pScalingLists", true, false, indents + 1, dump_json_StdVideoH264ScalingLists);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH264SequenceParameterSetVui>(object.pSequenceParameterSetVui, settings, "const StdVideoH264SequenceParameterSetVui*", "pSequenceParameterSetVui", true, false, indents + 1, dump_json_StdVideoH264SequenceParameterSetVui);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH264PpsFlags(const StdVideoH264PpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.transform_8x8_mode_flag, NULL, settings, "uint32_t", "transform_8x8_mode_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.redundant_pic_cnt_present_flag, NULL, settings, "uint32_t", "redundant_pic_cnt_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.constrained_intra_pred_flag, NULL, settings, "uint32_t", "constrained_intra_pred_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.deblocking_filter_control_present_flag, NULL, settings, "uint32_t", "deblocking_filter_control_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.weighted_pred_flag, NULL, settings, "uint32_t", "weighted_pred_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.bottom_field_pic_order_in_frame_present_flag, NULL, settings, "uint32_t", "bottom_field_pic_order_in_frame_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.entropy_coding_mode_flag, NULL, settings, "uint32_t", "entropy_coding_mode_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pic_scaling_matrix_present_flag, NULL, settings, "uint32_t", "pic_scaling_matrix_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH264PictureParameterSet(const StdVideoH264PictureParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH264PpsFlags>(object.flags, NULL, settings, "StdVideoH264PpsFlags", "flags", true, false, indents + 1, dump_json_StdVideoH264PpsFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.seq_parameter_set_id, NULL, settings, "uint8_t", "seq_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pic_parameter_set_id, NULL, settings, "uint8_t", "pic_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_ref_idx_l0_default_active_minus1, NULL, settings, "uint8_t", "num_ref_idx_l0_default_active_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_ref_idx_l1_default_active_minus1, NULL, settings, "uint8_t", "num_ref_idx_l1_default_active_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264WeightedBipredIdc>(object.weighted_bipred_idc, NULL, settings, "StdVideoH264WeightedBipredIdc", "weighted_bipred_idc", false, false, indents + 1, dump_json_StdVideoH264WeightedBipredIdc);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.pic_init_qp_minus26, NULL, settings, "int8_t", "pic_init_qp_minus26", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.pic_init_qs_minus26, NULL, settings, "int8_t", "pic_init_qs_minus26", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.chroma_qp_index_offset, NULL, settings, "int8_t", "chroma_qp_index_offset", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.second_chroma_qp_index_offset, NULL, settings, "int8_t", "second_chroma_qp_index_offset", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH264ScalingLists>(object.pScalingLists, settings, "const StdVideoH264ScalingLists*", "pScalingLists", true, false, indents + 1, dump_json_StdVideoH264ScalingLists);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeH264PictureInfoFlags(const StdVideoDecodeH264PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.field_pic_flag, NULL, settings, "uint32_t", "field_pic_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.is_intra, NULL, settings, "uint32_t", "is_intra", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.IdrPicFlag, NULL, settings, "uint32_t", "IdrPicFlag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.bottom_field_flag, NULL, settings, "uint32_t", "bottom_field_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.is_reference, NULL, settings, "uint32_t", "is_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.complementary_field_pair, NULL, settings, "uint32_t", "complementary_field_pair", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeH264PictureInfo(const StdVideoDecodeH264PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoDecodeH264PictureInfoFlags>(object.flags, NULL, settings, "StdVideoDecodeH264PictureInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoDecodeH264PictureInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.seq_parameter_set_id, NULL, settings, "uint8_t", "seq_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pic_parameter_set_id, NULL, settings, "uint8_t", "pic_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved2, NULL, settings, "uint8_t", "reserved2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.frame_num, NULL, settings, "uint16_t", "frame_num", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.idr_pic_id, NULL, settings, "uint16_t", "idr_pic_id", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_array<const int32_t>(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, "int32_t[STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE]", "int32_t", "PicOrderCnt", false, false, indents + 1, dump_json_int32_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeH264ReferenceInfoFlags(const StdVideoDecodeH264ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.top_field_flag, NULL, settings, "uint32_t", "top_field_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.bottom_field_flag, NULL, settings, "uint32_t", "bottom_field_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.used_for_long_term_reference, NULL, settings, "uint32_t", "used_for_long_term_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.is_non_existing, NULL, settings, "uint32_t", "is_non_existing", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeH264ReferenceInfo(const StdVideoDecodeH264ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoDecodeH264ReferenceInfoFlags>(object.flags, NULL, settings, "StdVideoDecodeH264ReferenceInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoDecodeH264ReferenceInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.FrameNum, NULL, settings, "uint16_t", "FrameNum", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.reserved, NULL, settings, "uint16_t", "reserved", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_array<const int32_t>(object.PicOrderCnt, STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE, settings, "int32_t[STD_VIDEO_DECODE_H264_FIELD_ORDER_COUNT_LIST_SIZE]", "int32_t", "PicOrderCnt", false, false, indents + 1, dump_json_int32_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264WeightTableFlags(const StdVideoEncodeH264WeightTableFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.luma_weight_l0_flag, NULL, settings, "uint32_t", "luma_weight_l0_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.chroma_weight_l0_flag, NULL, settings, "uint32_t", "chroma_weight_l0_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.luma_weight_l1_flag, NULL, settings, "uint32_t", "luma_weight_l1_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.chroma_weight_l1_flag, NULL, settings, "uint32_t", "chroma_weight_l1_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264WeightTable(const StdVideoEncodeH264WeightTable& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH264WeightTableFlags>(object.flags, NULL, settings, "StdVideoEncodeH264WeightTableFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH264WeightTableFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.luma_log2_weight_denom, NULL, settings, "uint8_t", "luma_log2_weight_denom", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.chroma_log2_weight_denom, NULL, settings, "uint8_t", "chroma_log2_weight_denom", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.luma_weight_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "int8_t", "luma_weight_l0", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.luma_offset_l0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "int8_t", "luma_offset_l0", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.luma_weight_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "int8_t", "luma_weight_l1", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.luma_offset_l1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "int8_t", "luma_offset_l1", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264SliceHeaderFlags(const StdVideoEncodeH264SliceHeaderFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.direct_spatial_mv_pred_flag, NULL, settings, "uint32_t", "direct_spatial_mv_pred_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.num_ref_idx_active_override_flag, NULL, settings, "uint32_t", "num_ref_idx_active_override_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264PictureInfoFlags(const StdVideoEncodeH264PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.IdrPicFlag, NULL, settings, "uint32_t", "IdrPicFlag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.is_reference, NULL, settings, "uint32_t", "is_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.no_output_of_prior_pics_flag, NULL, settings, "uint32_t", "no_output_of_prior_pics_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.long_term_reference_flag, NULL, settings, "uint32_t", "long_term_reference_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.adaptive_ref_pic_marking_mode_flag, NULL, settings, "uint32_t", "adaptive_ref_pic_marking_mode_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264ReferenceInfoFlags(const StdVideoEncodeH264ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.used_for_long_term_reference, NULL, settings, "uint32_t", "used_for_long_term_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264ReferenceListsInfoFlags(const StdVideoEncodeH264ReferenceListsInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.ref_pic_list_modification_flag_l0, NULL, settings, "uint32_t", "ref_pic_list_modification_flag_l0", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.ref_pic_list_modification_flag_l1, NULL, settings, "uint32_t", "ref_pic_list_modification_flag_l1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264RefListModEntry(const StdVideoEncodeH264RefListModEntry& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH264ModificationOfPicNumsIdc>(object.modification_of_pic_nums_idc, NULL, settings, "StdVideoH264ModificationOfPicNumsIdc", "modification_of_pic_nums_idc", false, false, indents + 1, dump_json_StdVideoH264ModificationOfPicNumsIdc);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.abs_diff_pic_num_minus1, NULL, settings, "uint16_t", "abs_diff_pic_num_minus1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.long_term_pic_num, NULL, settings, "uint16_t", "long_term_pic_num", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264RefPicMarkingEntry(const StdVideoEncodeH264RefPicMarkingEntry& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH264MemMgmtControlOp>(object.memory_management_control_operation, NULL, settings, "StdVideoH264MemMgmtControlOp", "memory_management_control_operation", false, false, indents + 1, dump_json_StdVideoH264MemMgmtControlOp);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.difference_of_pic_nums_minus1, NULL, settings, "uint16_t", "difference_of_pic_nums_minus1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.long_term_pic_num, NULL, settings, "uint16_t", "long_term_pic_num", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.long_term_frame_idx, NULL, settings, "uint16_t", "long_term_frame_idx", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.max_long_term_frame_idx_plus1, NULL, settings, "uint16_t", "max_long_term_frame_idx_plus1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264ReferenceListsInfo(const StdVideoEncodeH264ReferenceListsInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH264ReferenceListsInfoFlags>(object.flags, NULL, settings, "StdVideoEncodeH264ReferenceListsInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH264ReferenceListsInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_ref_idx_l0_active_minus1, NULL, settings, "uint8_t", "num_ref_idx_l0_active_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_ref_idx_l1_active_minus1, NULL, settings, "uint8_t", "num_ref_idx_l1_active_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.RefPicList0, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "uint8_t", "RefPicList0", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.RefPicList1, STD_VIDEO_H264_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H264_MAX_NUM_LIST_REF]", "uint8_t", "RefPicList1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.refList0ModOpCount, NULL, settings, "uint8_t", "refList0ModOpCount", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.refList1ModOpCount, NULL, settings, "uint8_t", "refList1ModOpCount", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.refPicMarkingOpCount, NULL, settings, "uint8_t", "refPicMarkingOpCount", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.reserved1, 7, settings, "uint8_t[7]", "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeH264RefListModEntry>(object.pRefList0ModOperations, settings, "const StdVideoEncodeH264RefListModEntry*", "pRefList0ModOperations", true, false, indents + 1, dump_json_StdVideoEncodeH264RefListModEntry);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeH264RefListModEntry>(object.pRefList1ModOperations, settings, "const StdVideoEncodeH264RefListModEntry*", "pRefList1ModOperations", true, false, indents + 1, dump_json_StdVideoEncodeH264RefListModEntry);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeH264RefPicMarkingEntry>(object.pRefPicMarkingOperations, settings, "const StdVideoEncodeH264RefPicMarkingEntry*", "pRefPicMarkingOperations", true, false, indents + 1, dump_json_StdVideoEncodeH264RefPicMarkingEntry);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264PictureInfo(const StdVideoEncodeH264PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH264PictureInfoFlags>(object.flags, NULL, settings, "StdVideoEncodeH264PictureInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH264PictureInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.seq_parameter_set_id, NULL, settings, "uint8_t", "seq_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pic_parameter_set_id, NULL, settings, "uint8_t", "pic_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.idr_pic_id, NULL, settings, "uint16_t", "idr_pic_id", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264PictureType>(object.primary_pic_type, NULL, settings, "StdVideoH264PictureType", "primary_pic_type", false, false, indents + 1, dump_json_StdVideoH264PictureType);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_num, NULL, settings, "uint32_t", "frame_num", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const int32_t>(object.PicOrderCnt, NULL, settings, "int32_t", "PicOrderCnt", false, false, indents + 1, dump_json_int32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.temporal_id, NULL, settings, "uint8_t", "temporal_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.reserved1, 3, settings, "uint8_t[3]", "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeH264ReferenceListsInfo>(object.pRefLists, settings, "const StdVideoEncodeH264ReferenceListsInfo*", "pRefLists", true, false, indents + 1, dump_json_StdVideoEncodeH264ReferenceListsInfo);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264ReferenceInfo(const StdVideoEncodeH264ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH264ReferenceInfoFlags>(object.flags, NULL, settings, "StdVideoEncodeH264ReferenceInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH264ReferenceInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264PictureType>(object.primary_pic_type, NULL, settings, "StdVideoH264PictureType", "primary_pic_type", false, false, indents + 1, dump_json_StdVideoH264PictureType);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.FrameNum, NULL, settings, "uint32_t", "FrameNum", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const int32_t>(object.PicOrderCnt, NULL, settings, "int32_t", "PicOrderCnt", false, false, indents + 1, dump_json_int32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.long_term_pic_num, NULL, settings, "uint16_t", "long_term_pic_num", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.long_term_frame_idx, NULL, settings, "uint16_t", "long_term_frame_idx", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.temporal_id, NULL, settings, "uint8_t", "temporal_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH264SliceHeader(const StdVideoEncodeH264SliceHeader& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH264SliceHeaderFlags>(object.flags, NULL, settings, "StdVideoEncodeH264SliceHeaderFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH264SliceHeaderFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.first_mb_in_slice, NULL, settings, "uint32_t", "first_mb_in_slice", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264SliceType>(object.slice_type, NULL, settings, "StdVideoH264SliceType", "slice_type", false, false, indents + 1, dump_json_StdVideoH264SliceType);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_alpha_c0_offset_div2, NULL, settings, "int8_t", "slice_alpha_c0_offset_div2", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_beta_offset_div2, NULL, settings, "int8_t", "slice_beta_offset_div2", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_qp_delta, NULL, settings, "int8_t", "slice_qp_delta", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264CabacInitIdc>(object.cabac_init_idc, NULL, settings, "StdVideoH264CabacInitIdc", "cabac_init_idc", false, false, indents + 1, dump_json_StdVideoH264CabacInitIdc);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH264DisableDeblockingFilterIdc>(object.disable_deblocking_filter_idc, NULL, settings, "StdVideoH264DisableDeblockingFilterIdc", "disable_deblocking_filter_idc", false, false, indents + 1, dump_json_StdVideoH264DisableDeblockingFilterIdc);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeH264WeightTable>(object.pWeightTable, settings, "const StdVideoEncodeH264WeightTable*", "pWeightTable", true, false, indents + 1, dump_json_StdVideoEncodeH264WeightTable);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265DecPicBufMgr(const StdVideoH265DecPicBufMgr& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_array<const uint32_t>(object.max_latency_increase_plus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint32_t", "max_latency_increase_plus1", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.max_dec_pic_buffering_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint8_t", "max_dec_pic_buffering_minus1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.max_num_reorder_pics, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint8_t", "max_num_reorder_pics", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265SubLayerHrdParameters(const StdVideoH265SubLayerHrdParameters& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_array<const uint32_t>(object.bit_rate_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "uint32_t", "bit_rate_value_minus1", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint32_t>(object.cpb_size_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "uint32_t", "cpb_size_value_minus1", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint32_t>(object.cpb_size_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "uint32_t", "cpb_size_du_value_minus1", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint32_t>(object.bit_rate_du_value_minus1, STD_VIDEO_H265_CPB_CNT_LIST_SIZE, settings, "uint32_t[STD_VIDEO_H265_CPB_CNT_LIST_SIZE]", "uint32_t", "bit_rate_du_value_minus1", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.cbr_flag, NULL, settings, "uint32_t", "cbr_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265HrdFlags(const StdVideoH265HrdFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.nal_hrd_parameters_present_flag, NULL, settings, "uint32_t", "nal_hrd_parameters_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vcl_hrd_parameters_present_flag, NULL, settings, "uint32_t", "vcl_hrd_parameters_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sub_pic_hrd_params_present_flag, NULL, settings, "uint32_t", "sub_pic_hrd_params_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sub_pic_cpb_params_in_pic_timing_sei_flag, NULL, settings, "uint32_t", "sub_pic_cpb_params_in_pic_timing_sei_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.fixed_pic_rate_general_flag, NULL, settings, "uint32_t", "fixed_pic_rate_general_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.fixed_pic_rate_within_cvs_flag, NULL, settings, "uint32_t", "fixed_pic_rate_within_cvs_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.low_delay_hrd_flag, NULL, settings, "uint32_t", "low_delay_hrd_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265HrdParameters(const StdVideoH265HrdParameters& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH265HrdFlags>(object.flags, NULL, settings, "StdVideoH265HrdFlags", "flags", true, false, indents + 1, dump_json_StdVideoH265HrdFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.tick_divisor_minus2, NULL, settings, "uint8_t", "tick_divisor_minus2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.du_cpb_removal_delay_increment_length_minus1, NULL, settings, "uint8_t", "du_cpb_removal_delay_increment_length_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.dpb_output_delay_du_length_minus1, NULL, settings, "uint8_t", "dpb_output_delay_du_length_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.bit_rate_scale, NULL, settings, "uint8_t", "bit_rate_scale", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.cpb_size_scale, NULL, settings, "uint8_t", "cpb_size_scale", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.cpb_size_du_scale, NULL, settings, "uint8_t", "cpb_size_du_scale", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.initial_cpb_removal_delay_length_minus1, NULL, settings, "uint8_t", "initial_cpb_removal_delay_length_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.au_cpb_removal_delay_length_minus1, NULL, settings, "uint8_t", "au_cpb_removal_delay_length_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.dpb_output_delay_length_minus1, NULL, settings, "uint8_t", "dpb_output_delay_length_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.cpb_cnt_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint8_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint8_t", "cpb_cnt_minus1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint16_t>(object.elemental_duration_in_tc_minus1, STD_VIDEO_H265_SUBLAYERS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_SUBLAYERS_LIST_SIZE]", "uint16_t", "elemental_duration_in_tc_minus1", false, false, indents + 1, dump_json_uint16_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint16_t>(object.reserved, 3, settings, "uint16_t[3]", "uint16_t", "reserved", false, false, indents + 1, dump_json_uint16_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265SubLayerHrdParameters>(object.pSubLayerHrdParametersNal, settings, "const StdVideoH265SubLayerHrdParameters*", "pSubLayerHrdParametersNal", true, false, indents + 1, dump_json_StdVideoH265SubLayerHrdParameters);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265SubLayerHrdParameters>(object.pSubLayerHrdParametersVcl, settings, "const StdVideoH265SubLayerHrdParameters*", "pSubLayerHrdParametersVcl", true, false, indents + 1, dump_json_StdVideoH265SubLayerHrdParameters);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265VpsFlags(const StdVideoH265VpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.vps_temporal_id_nesting_flag, NULL, settings, "uint32_t", "vps_temporal_id_nesting_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vps_sub_layer_ordering_info_present_flag, NULL, settings, "uint32_t", "vps_sub_layer_ordering_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vps_timing_info_present_flag, NULL, settings, "uint32_t", "vps_timing_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vps_poc_proportional_to_timing_flag, NULL, settings, "uint32_t", "vps_poc_proportional_to_timing_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265ProfileTierLevelFlags(const StdVideoH265ProfileTierLevelFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.general_tier_flag, NULL, settings, "uint32_t", "general_tier_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.general_progressive_source_flag, NULL, settings, "uint32_t", "general_progressive_source_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.general_interlaced_source_flag, NULL, settings, "uint32_t", "general_interlaced_source_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.general_non_packed_constraint_flag, NULL, settings, "uint32_t", "general_non_packed_constraint_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.general_frame_only_constraint_flag, NULL, settings, "uint32_t", "general_frame_only_constraint_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265ProfileTierLevel(const StdVideoH265ProfileTierLevel& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH265ProfileTierLevelFlags>(object.flags, NULL, settings, "StdVideoH265ProfileTierLevelFlags", "flags", true, false, indents + 1, dump_json_StdVideoH265ProfileTierLevelFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH265ProfileIdc>(object.general_profile_idc, NULL, settings, "StdVideoH265ProfileIdc", "general_profile_idc", false, false, indents + 1, dump_json_StdVideoH265ProfileIdc);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH265LevelIdc>(object.general_level_idc, NULL, settings, "StdVideoH265LevelIdc", "general_level_idc", false, false, indents + 1, dump_json_StdVideoH265LevelIdc);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265VideoParameterSet(const StdVideoH265VideoParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH265VpsFlags>(object.flags, NULL, settings, "StdVideoH265VpsFlags", "flags", true, false, indents + 1, dump_json_StdVideoH265VpsFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.vps_video_parameter_set_id, NULL, settings, "uint8_t", "vps_video_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.vps_max_sub_layers_minus1, NULL, settings, "uint8_t", "vps_max_sub_layers_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved2, NULL, settings, "uint8_t", "reserved2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vps_num_units_in_tick, NULL, settings, "uint32_t", "vps_num_units_in_tick", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vps_time_scale, NULL, settings, "uint32_t", "vps_time_scale", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vps_num_ticks_poc_diff_one_minus1, NULL, settings, "uint32_t", "vps_num_ticks_poc_diff_one_minus1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved3, NULL, settings, "uint32_t", "reserved3", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265DecPicBufMgr>(object.pDecPicBufMgr, settings, "const StdVideoH265DecPicBufMgr*", "pDecPicBufMgr", true, false, indents + 1, dump_json_StdVideoH265DecPicBufMgr);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265HrdParameters>(object.pHrdParameters, settings, "const StdVideoH265HrdParameters*", "pHrdParameters", true, false, indents + 1, dump_json_StdVideoH265HrdParameters);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265ProfileTierLevel>(object.pProfileTierLevel, settings, "const StdVideoH265ProfileTierLevel*", "pProfileTierLevel", true, false, indents + 1, dump_json_StdVideoH265ProfileTierLevel);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265ScalingLists(const StdVideoH265ScalingLists& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.ScalingListDCCoef16x16, STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_16X16_NUM_LISTS]", "uint8_t", "ScalingListDCCoef16x16", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.ScalingListDCCoef32x32, STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS, settings, "uint8_t[STD_VIDEO_H265_SCALING_LIST_32X32_NUM_LISTS]", "uint8_t", "ScalingListDCCoef32x32", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265SpsVuiFlags(const StdVideoH265SpsVuiFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.aspect_ratio_info_present_flag, NULL, settings, "uint32_t", "aspect_ratio_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.overscan_info_present_flag, NULL, settings, "uint32_t", "overscan_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.overscan_appropriate_flag, NULL, settings, "uint32_t", "overscan_appropriate_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.video_signal_type_present_flag, NULL, settings, "uint32_t", "video_signal_type_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.video_full_range_flag, NULL, settings, "uint32_t", "video_full_range_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.colour_description_present_flag, NULL, settings, "uint32_t", "colour_description_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.chroma_loc_info_present_flag, NULL, settings, "uint32_t", "chroma_loc_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.neutral_chroma_indication_flag, NULL, settings, "uint32_t", "neutral_chroma_indication_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.field_seq_flag, NULL, settings, "uint32_t", "field_seq_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_field_info_present_flag, NULL, settings, "uint32_t", "frame_field_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.default_display_window_flag, NULL, settings, "uint32_t", "default_display_window_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vui_timing_info_present_flag, NULL, settings, "uint32_t", "vui_timing_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vui_poc_proportional_to_timing_flag, NULL, settings, "uint32_t", "vui_poc_proportional_to_timing_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vui_hrd_parameters_present_flag, NULL, settings, "uint32_t", "vui_hrd_parameters_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.bitstream_restriction_flag, NULL, settings, "uint32_t", "bitstream_restriction_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.tiles_fixed_structure_flag, NULL, settings, "uint32_t", "tiles_fixed_structure_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.motion_vectors_over_pic_boundaries_flag, NULL, settings, "uint32_t", "motion_vectors_over_pic_boundaries_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.restricted_ref_pic_lists_flag, NULL, settings, "uint32_t", "restricted_ref_pic_lists_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265SequenceParameterSetVui(const StdVideoH265SequenceParameterSetVui& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH265SpsVuiFlags>(object.flags, NULL, settings, "StdVideoH265SpsVuiFlags", "flags", true, false, indents + 1, dump_json_StdVideoH265SpsVuiFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH265AspectRatioIdc>(object.aspect_ratio_idc, NULL, settings, "StdVideoH265AspectRatioIdc", "aspect_ratio_idc", false, false, indents + 1, dump_json_StdVideoH265AspectRatioIdc);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.sar_width, NULL, settings, "uint16_t", "sar_width", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.sar_height, NULL, settings, "uint16_t", "sar_height", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.video_format, NULL, settings, "uint8_t", "video_format", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.colour_primaries, NULL, settings, "uint8_t", "colour_primaries", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.transfer_characteristics, NULL, settings, "uint8_t", "transfer_characteristics", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.matrix_coeffs, NULL, settings, "uint8_t", "matrix_coeffs", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.chroma_sample_loc_type_top_field, NULL, settings, "uint8_t", "chroma_sample_loc_type_top_field", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.chroma_sample_loc_type_bottom_field, NULL, settings, "uint8_t", "chroma_sample_loc_type_bottom_field", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved2, NULL, settings, "uint8_t", "reserved2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.def_disp_win_left_offset, NULL, settings, "uint16_t", "def_disp_win_left_offset", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.def_disp_win_right_offset, NULL, settings, "uint16_t", "def_disp_win_right_offset", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.def_disp_win_top_offset, NULL, settings, "uint16_t", "def_disp_win_top_offset", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.def_disp_win_bottom_offset, NULL, settings, "uint16_t", "def_disp_win_bottom_offset", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vui_num_units_in_tick, NULL, settings, "uint32_t", "vui_num_units_in_tick", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vui_time_scale, NULL, settings, "uint32_t", "vui_time_scale", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vui_num_ticks_poc_diff_one_minus1, NULL, settings, "uint32_t", "vui_num_ticks_poc_diff_one_minus1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.min_spatial_segmentation_idc, NULL, settings, "uint16_t", "min_spatial_segmentation_idc", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.reserved3, NULL, settings, "uint16_t", "reserved3", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.max_bytes_per_pic_denom, NULL, settings, "uint8_t", "max_bytes_per_pic_denom", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.max_bits_per_min_cu_denom, NULL, settings, "uint8_t", "max_bits_per_min_cu_denom", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_max_mv_length_horizontal, NULL, settings, "uint8_t", "log2_max_mv_length_horizontal", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_max_mv_length_vertical, NULL, settings, "uint8_t", "log2_max_mv_length_vertical", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265HrdParameters>(object.pHrdParameters, settings, "const StdVideoH265HrdParameters*", "pHrdParameters", true, false, indents + 1, dump_json_StdVideoH265HrdParameters);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265PredictorPaletteEntries(const StdVideoH265PredictorPaletteEntries& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265SpsFlags(const StdVideoH265SpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.sps_temporal_id_nesting_flag, NULL, settings, "uint32_t", "sps_temporal_id_nesting_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.separate_colour_plane_flag, NULL, settings, "uint32_t", "separate_colour_plane_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.conformance_window_flag, NULL, settings, "uint32_t", "conformance_window_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sps_sub_layer_ordering_info_present_flag, NULL, settings, "uint32_t", "sps_sub_layer_ordering_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.scaling_list_enabled_flag, NULL, settings, "uint32_t", "scaling_list_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sps_scaling_list_data_present_flag, NULL, settings, "uint32_t", "sps_scaling_list_data_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.amp_enabled_flag, NULL, settings, "uint32_t", "amp_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sample_adaptive_offset_enabled_flag, NULL, settings, "uint32_t", "sample_adaptive_offset_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pcm_enabled_flag, NULL, settings, "uint32_t", "pcm_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pcm_loop_filter_disabled_flag, NULL, settings, "uint32_t", "pcm_loop_filter_disabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.long_term_ref_pics_present_flag, NULL, settings, "uint32_t", "long_term_ref_pics_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sps_temporal_mvp_enabled_flag, NULL, settings, "uint32_t", "sps_temporal_mvp_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.strong_intra_smoothing_enabled_flag, NULL, settings, "uint32_t", "strong_intra_smoothing_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.vui_parameters_present_flag, NULL, settings, "uint32_t", "vui_parameters_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sps_extension_present_flag, NULL, settings, "uint32_t", "sps_extension_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sps_range_extension_flag, NULL, settings, "uint32_t", "sps_range_extension_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.transform_skip_rotation_enabled_flag, NULL, settings, "uint32_t", "transform_skip_rotation_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.transform_skip_context_enabled_flag, NULL, settings, "uint32_t", "transform_skip_context_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.implicit_rdpcm_enabled_flag, NULL, settings, "uint32_t", "implicit_rdpcm_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.explicit_rdpcm_enabled_flag, NULL, settings, "uint32_t", "explicit_rdpcm_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.extended_precision_processing_flag, NULL, settings, "uint32_t", "extended_precision_processing_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.intra_smoothing_disabled_flag, NULL, settings, "uint32_t", "intra_smoothing_disabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.high_precision_offsets_enabled_flag, NULL, settings, "uint32_t", "high_precision_offsets_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.persistent_rice_adaptation_enabled_flag, NULL, settings, "uint32_t", "persistent_rice_adaptation_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.cabac_bypass_alignment_enabled_flag, NULL, settings, "uint32_t", "cabac_bypass_alignment_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sps_scc_extension_flag, NULL, settings, "uint32_t", "sps_scc_extension_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sps_curr_pic_ref_enabled_flag, NULL, settings, "uint32_t", "sps_curr_pic_ref_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.palette_mode_enabled_flag, NULL, settings, "uint32_t", "palette_mode_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sps_palette_predictor_initializers_present_flag, NULL, settings, "uint32_t", "sps_palette_predictor_initializers_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.intra_boundary_filtering_disabled_flag, NULL, settings, "uint32_t", "intra_boundary_filtering_disabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265ShortTermRefPicSetFlags(const StdVideoH265ShortTermRefPicSetFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.inter_ref_pic_set_prediction_flag, NULL, settings, "uint32_t", "inter_ref_pic_set_prediction_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.delta_rps_sign, NULL, settings, "uint32_t", "delta_rps_sign", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265ShortTermRefPicSet(const StdVideoH265ShortTermRefPicSet& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH265ShortTermRefPicSetFlags>(object.flags, NULL, settings, "StdVideoH265ShortTermRefPicSetFlags", "flags", true, false, indents + 1, dump_json_StdVideoH265ShortTermRefPicSetFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.delta_idx_minus1, NULL, settings, "uint32_t", "delta_idx_minus1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.use_delta_flag, NULL, settings, "uint16_t", "use_delta_flag", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.abs_delta_rps_minus1, NULL, settings, "uint16_t", "abs_delta_rps_minus1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.used_by_curr_pic_flag, NULL, settings, "uint16_t", "used_by_curr_pic_flag", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.used_by_curr_pic_s0_flag, NULL, settings, "uint16_t", "used_by_curr_pic_s0_flag", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.used_by_curr_pic_s1_flag, NULL, settings, "uint16_t", "used_by_curr_pic_s1_flag", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.reserved1, NULL, settings, "uint16_t", "reserved1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved2, NULL, settings, "uint8_t", "reserved2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved3, NULL, settings, "uint8_t", "reserved3", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_negative_pics, NULL, settings, "uint8_t", "num_negative_pics", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_positive_pics, NULL, settings, "uint8_t", "num_positive_pics", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint16_t>(object.delta_poc_s0_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, "uint16_t[STD_VIDEO_H265_MAX_DPB_SIZE]", "uint16_t", "delta_poc_s0_minus1", false, false, indents + 1, dump_json_uint16_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint16_t>(object.delta_poc_s1_minus1, STD_VIDEO_H265_MAX_DPB_SIZE, settings, "uint16_t[STD_VIDEO_H265_MAX_DPB_SIZE]", "uint16_t", "delta_poc_s1_minus1", false, false, indents + 1, dump_json_uint16_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265LongTermRefPicsSps(const StdVideoH265LongTermRefPicsSps& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.used_by_curr_pic_lt_sps_flag, NULL, settings, "uint32_t", "used_by_curr_pic_lt_sps_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_array<const uint32_t>(object.lt_ref_pic_poc_lsb_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, "uint32_t[STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS]", "uint32_t", "lt_ref_pic_poc_lsb_sps", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265SequenceParameterSet(const StdVideoH265SequenceParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH265SpsFlags>(object.flags, NULL, settings, "StdVideoH265SpsFlags", "flags", true, false, indents + 1, dump_json_StdVideoH265SpsFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH265ChromaFormatIdc>(object.chroma_format_idc, NULL, settings, "StdVideoH265ChromaFormatIdc", "chroma_format_idc", false, false, indents + 1, dump_json_StdVideoH265ChromaFormatIdc);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pic_width_in_luma_samples, NULL, settings, "uint32_t", "pic_width_in_luma_samples", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pic_height_in_luma_samples, NULL, settings, "uint32_t", "pic_height_in_luma_samples", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.sps_video_parameter_set_id, NULL, settings, "uint8_t", "sps_video_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.sps_max_sub_layers_minus1, NULL, settings, "uint8_t", "sps_max_sub_layers_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.sps_seq_parameter_set_id, NULL, settings, "uint8_t", "sps_seq_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.bit_depth_luma_minus8, NULL, settings, "uint8_t", "bit_depth_luma_minus8", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.bit_depth_chroma_minus8, NULL, settings, "uint8_t", "bit_depth_chroma_minus8", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_max_pic_order_cnt_lsb_minus4, NULL, settings, "uint8_t", "log2_max_pic_order_cnt_lsb_minus4", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_min_luma_coding_block_size_minus3, NULL, settings, "uint8_t", "log2_min_luma_coding_block_size_minus3", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_diff_max_min_luma_coding_block_size, NULL, settings, "uint8_t", "log2_diff_max_min_luma_coding_block_size", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_min_luma_transform_block_size_minus2, NULL, settings, "uint8_t", "log2_min_luma_transform_block_size_minus2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_diff_max_min_luma_transform_block_size, NULL, settings, "uint8_t", "log2_diff_max_min_luma_transform_block_size", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.max_transform_hierarchy_depth_inter, NULL, settings, "uint8_t", "max_transform_hierarchy_depth_inter", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.max_transform_hierarchy_depth_intra, NULL, settings, "uint8_t", "max_transform_hierarchy_depth_intra", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_short_term_ref_pic_sets, NULL, settings, "uint8_t", "num_short_term_ref_pic_sets", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_long_term_ref_pics_sps, NULL, settings, "uint8_t", "num_long_term_ref_pics_sps", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pcm_sample_bit_depth_luma_minus1, NULL, settings, "uint8_t", "pcm_sample_bit_depth_luma_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pcm_sample_bit_depth_chroma_minus1, NULL, settings, "uint8_t", "pcm_sample_bit_depth_chroma_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_min_pcm_luma_coding_block_size_minus3, NULL, settings, "uint8_t", "log2_min_pcm_luma_coding_block_size_minus3", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_diff_max_min_pcm_luma_coding_block_size, NULL, settings, "uint8_t", "log2_diff_max_min_pcm_luma_coding_block_size", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved2, NULL, settings, "uint8_t", "reserved2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.palette_max_size, NULL, settings, "uint8_t", "palette_max_size", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.delta_palette_max_predictor_size, NULL, settings, "uint8_t", "delta_palette_max_predictor_size", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.motion_vector_resolution_control_idc, NULL, settings, "uint8_t", "motion_vector_resolution_control_idc", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.sps_num_palette_predictor_initializers_minus1, NULL, settings, "uint8_t", "sps_num_palette_predictor_initializers_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.conf_win_left_offset, NULL, settings, "uint32_t", "conf_win_left_offset", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.conf_win_right_offset, NULL, settings, "uint32_t", "conf_win_right_offset", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.conf_win_top_offset, NULL, settings, "uint32_t", "conf_win_top_offset", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.conf_win_bottom_offset, NULL, settings, "uint32_t", "conf_win_bottom_offset", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265ProfileTierLevel>(object.pProfileTierLevel, settings, "const StdVideoH265ProfileTierLevel*", "pProfileTierLevel", true, false, indents + 1, dump_json_StdVideoH265ProfileTierLevel);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265DecPicBufMgr>(object.pDecPicBufMgr, settings, "const StdVideoH265DecPicBufMgr*", "pDecPicBufMgr", true, false, indents + 1, dump_json_StdVideoH265DecPicBufMgr);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265ScalingLists>(object.pScalingLists, settings, "const StdVideoH265ScalingLists*", "pScalingLists", true, false, indents + 1, dump_json_StdVideoH265ScalingLists);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265ShortTermRefPicSet>(object.pShortTermRefPicSet, settings, "const StdVideoH265ShortTermRefPicSet*", "pShortTermRefPicSet", true, false, indents + 1, dump_json_StdVideoH265ShortTermRefPicSet);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265LongTermRefPicsSps>(object.pLongTermRefPicsSps, settings, "const StdVideoH265LongTermRefPicsSps*", "pLongTermRefPicsSps", true, false, indents + 1, dump_json_StdVideoH265LongTermRefPicsSps);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265SequenceParameterSetVui>(object.pSequenceParameterSetVui, settings, "const StdVideoH265SequenceParameterSetVui*", "pSequenceParameterSetVui", true, false, indents + 1, dump_json_StdVideoH265SequenceParameterSetVui);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265PredictorPaletteEntries>(object.pPredictorPaletteEntries, settings, "const StdVideoH265PredictorPaletteEntries*", "pPredictorPaletteEntries", true, false, indents + 1, dump_json_StdVideoH265PredictorPaletteEntries);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265PpsFlags(const StdVideoH265PpsFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.dependent_slice_segments_enabled_flag, NULL, settings, "uint32_t", "dependent_slice_segments_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.output_flag_present_flag, NULL, settings, "uint32_t", "output_flag_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.sign_data_hiding_enabled_flag, NULL, settings, "uint32_t", "sign_data_hiding_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.cabac_init_present_flag, NULL, settings, "uint32_t", "cabac_init_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.constrained_intra_pred_flag, NULL, settings, "uint32_t", "constrained_intra_pred_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.transform_skip_enabled_flag, NULL, settings, "uint32_t", "transform_skip_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.cu_qp_delta_enabled_flag, NULL, settings, "uint32_t", "cu_qp_delta_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pps_slice_chroma_qp_offsets_present_flag, NULL, settings, "uint32_t", "pps_slice_chroma_qp_offsets_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.weighted_pred_flag, NULL, settings, "uint32_t", "weighted_pred_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.weighted_bipred_flag, NULL, settings, "uint32_t", "weighted_bipred_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.transquant_bypass_enabled_flag, NULL, settings, "uint32_t", "transquant_bypass_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.tiles_enabled_flag, NULL, settings, "uint32_t", "tiles_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.entropy_coding_sync_enabled_flag, NULL, settings, "uint32_t", "entropy_coding_sync_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.uniform_spacing_flag, NULL, settings, "uint32_t", "uniform_spacing_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.loop_filter_across_tiles_enabled_flag, NULL, settings, "uint32_t", "loop_filter_across_tiles_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pps_loop_filter_across_slices_enabled_flag, NULL, settings, "uint32_t", "pps_loop_filter_across_slices_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.deblocking_filter_control_present_flag, NULL, settings, "uint32_t", "deblocking_filter_control_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.deblocking_filter_override_enabled_flag, NULL, settings, "uint32_t", "deblocking_filter_override_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pps_deblocking_filter_disabled_flag, NULL, settings, "uint32_t", "pps_deblocking_filter_disabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pps_scaling_list_data_present_flag, NULL, settings, "uint32_t", "pps_scaling_list_data_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.lists_modification_present_flag, NULL, settings, "uint32_t", "lists_modification_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.slice_segment_header_extension_present_flag, NULL, settings, "uint32_t", "slice_segment_header_extension_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pps_extension_present_flag, NULL, settings, "uint32_t", "pps_extension_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.cross_component_prediction_enabled_flag, NULL, settings, "uint32_t", "cross_component_prediction_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.chroma_qp_offset_list_enabled_flag, NULL, settings, "uint32_t", "chroma_qp_offset_list_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pps_curr_pic_ref_enabled_flag, NULL, settings, "uint32_t", "pps_curr_pic_ref_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.residual_adaptive_colour_transform_enabled_flag, NULL, settings, "uint32_t", "residual_adaptive_colour_transform_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pps_slice_act_qp_offsets_present_flag, NULL, settings, "uint32_t", "pps_slice_act_qp_offsets_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pps_palette_predictor_initializers_present_flag, NULL, settings, "uint32_t", "pps_palette_predictor_initializers_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.monochrome_palette_flag, NULL, settings, "uint32_t", "monochrome_palette_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pps_range_extension_flag, NULL, settings, "uint32_t", "pps_range_extension_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoH265PictureParameterSet(const StdVideoH265PictureParameterSet& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoH265PpsFlags>(object.flags, NULL, settings, "StdVideoH265PpsFlags", "flags", true, false, indents + 1, dump_json_StdVideoH265PpsFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pps_pic_parameter_set_id, NULL, settings, "uint8_t", "pps_pic_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pps_seq_parameter_set_id, NULL, settings, "uint8_t", "pps_seq_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.sps_video_parameter_set_id, NULL, settings, "uint8_t", "sps_video_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_extra_slice_header_bits, NULL, settings, "uint8_t", "num_extra_slice_header_bits", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_ref_idx_l0_default_active_minus1, NULL, settings, "uint8_t", "num_ref_idx_l0_default_active_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_ref_idx_l1_default_active_minus1, NULL, settings, "uint8_t", "num_ref_idx_l1_default_active_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.init_qp_minus26, NULL, settings, "int8_t", "init_qp_minus26", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.diff_cu_qp_delta_depth, NULL, settings, "uint8_t", "diff_cu_qp_delta_depth", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.pps_cb_qp_offset, NULL, settings, "int8_t", "pps_cb_qp_offset", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.pps_cr_qp_offset, NULL, settings, "int8_t", "pps_cr_qp_offset", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.pps_beta_offset_div2, NULL, settings, "int8_t", "pps_beta_offset_div2", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.pps_tc_offset_div2, NULL, settings, "int8_t", "pps_tc_offset_div2", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_parallel_merge_level_minus2, NULL, settings, "uint8_t", "log2_parallel_merge_level_minus2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_max_transform_skip_block_size_minus2, NULL, settings, "uint8_t", "log2_max_transform_skip_block_size_minus2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.diff_cu_chroma_qp_offset_depth, NULL, settings, "uint8_t", "diff_cu_chroma_qp_offset_depth", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.chroma_qp_offset_list_len_minus1, NULL, settings, "uint8_t", "chroma_qp_offset_list_len_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.cb_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, "int8_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE]", "int8_t", "cb_qp_offset_list", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.cr_qp_offset_list, STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE, settings, "int8_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_LIST_SIZE]", "int8_t", "cr_qp_offset_list", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_sao_offset_scale_luma, NULL, settings, "uint8_t", "log2_sao_offset_scale_luma", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.log2_sao_offset_scale_chroma, NULL, settings, "uint8_t", "log2_sao_offset_scale_chroma", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.pps_act_y_qp_offset_plus5, NULL, settings, "int8_t", "pps_act_y_qp_offset_plus5", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.pps_act_cb_qp_offset_plus5, NULL, settings, "int8_t", "pps_act_cb_qp_offset_plus5", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.pps_act_cr_qp_offset_plus3, NULL, settings, "int8_t", "pps_act_cr_qp_offset_plus3", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pps_num_palette_predictor_initializers, NULL, settings, "uint8_t", "pps_num_palette_predictor_initializers", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.luma_bit_depth_entry_minus8, NULL, settings, "uint8_t", "luma_bit_depth_entry_minus8", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.chroma_bit_depth_entry_minus8, NULL, settings, "uint8_t", "chroma_bit_depth_entry_minus8", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_tile_columns_minus1, NULL, settings, "uint8_t", "num_tile_columns_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_tile_rows_minus1, NULL, settings, "uint8_t", "num_tile_rows_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved2, NULL, settings, "uint8_t", "reserved2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint16_t>(object.column_width_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_COLS_LIST_SIZE]", "uint16_t", "column_width_minus1", false, false, indents + 1, dump_json_uint16_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint16_t>(object.row_height_minus1, STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE, settings, "uint16_t[STD_VIDEO_H265_CHROMA_QP_OFFSET_TILE_ROWS_LIST_SIZE]", "uint16_t", "row_height_minus1", false, false, indents + 1, dump_json_uint16_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved3, NULL, settings, "uint32_t", "reserved3", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265ScalingLists>(object.pScalingLists, settings, "const StdVideoH265ScalingLists*", "pScalingLists", true, false, indents + 1, dump_json_StdVideoH265ScalingLists);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265PredictorPaletteEntries>(object.pPredictorPaletteEntries, settings, "const StdVideoH265PredictorPaletteEntries*", "pPredictorPaletteEntries", true, false, indents + 1, dump_json_StdVideoH265PredictorPaletteEntries);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeH265PictureInfoFlags(const StdVideoDecodeH265PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.IrapPicFlag, NULL, settings, "uint32_t", "IrapPicFlag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.IdrPicFlag, NULL, settings, "uint32_t", "IdrPicFlag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.IsReference, NULL, settings, "uint32_t", "IsReference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.short_term_ref_pic_set_sps_flag, NULL, settings, "uint32_t", "short_term_ref_pic_set_sps_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeH265PictureInfo(const StdVideoDecodeH265PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoDecodeH265PictureInfoFlags>(object.flags, NULL, settings, "StdVideoDecodeH265PictureInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoDecodeH265PictureInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.sps_video_parameter_set_id, NULL, settings, "uint8_t", "sps_video_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pps_seq_parameter_set_id, NULL, settings, "uint8_t", "pps_seq_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pps_pic_parameter_set_id, NULL, settings, "uint8_t", "pps_pic_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.NumDeltaPocsOfRefRpsIdx, NULL, settings, "uint8_t", "NumDeltaPocsOfRefRpsIdx", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const int32_t>(object.PicOrderCntVal, NULL, settings, "int32_t", "PicOrderCntVal", false, false, indents + 1, dump_json_int32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.NumBitsForSTRefPicSetInSlice, NULL, settings, "uint16_t", "NumBitsForSTRefPicSetInSlice", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.reserved, NULL, settings, "uint16_t", "reserved", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.RefPicSetStCurrBefore, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "uint8_t", "RefPicSetStCurrBefore", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.RefPicSetStCurrAfter, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "uint8_t", "RefPicSetStCurrAfter", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.RefPicSetLtCurr, STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE, settings, "uint8_t[STD_VIDEO_DECODE_H265_REF_PIC_SET_LIST_SIZE]", "uint8_t", "RefPicSetLtCurr", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeH265ReferenceInfoFlags(const StdVideoDecodeH265ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.used_for_long_term_reference, NULL, settings, "uint32_t", "used_for_long_term_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.unused_for_reference, NULL, settings, "uint32_t", "unused_for_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeH265ReferenceInfo(const StdVideoDecodeH265ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoDecodeH265ReferenceInfoFlags>(object.flags, NULL, settings, "StdVideoDecodeH265ReferenceInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoDecodeH265ReferenceInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const int32_t>(object.PicOrderCntVal, NULL, settings, "int32_t", "PicOrderCntVal", false, false, indents + 1, dump_json_int32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265WeightTableFlags(const StdVideoEncodeH265WeightTableFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint16_t>(object.luma_weight_l0_flag, NULL, settings, "uint16_t", "luma_weight_l0_flag", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.chroma_weight_l0_flag, NULL, settings, "uint16_t", "chroma_weight_l0_flag", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.luma_weight_l1_flag, NULL, settings, "uint16_t", "luma_weight_l1_flag", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.chroma_weight_l1_flag, NULL, settings, "uint16_t", "chroma_weight_l1_flag", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265WeightTable(const StdVideoEncodeH265WeightTable& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH265WeightTableFlags>(object.flags, NULL, settings, "StdVideoEncodeH265WeightTableFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH265WeightTableFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.luma_log2_weight_denom, NULL, settings, "uint8_t", "luma_log2_weight_denom", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.delta_chroma_log2_weight_denom, NULL, settings, "int8_t", "delta_chroma_log2_weight_denom", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.delta_luma_weight_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "int8_t", "delta_luma_weight_l0", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.luma_offset_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "int8_t", "luma_offset_l0", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.delta_luma_weight_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "int8_t", "delta_luma_weight_l1", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.luma_offset_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "int8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "int8_t", "luma_offset_l1", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    settings.stream() << ",\n";
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265SliceSegmentHeaderFlags(const StdVideoEncodeH265SliceSegmentHeaderFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.first_slice_segment_in_pic_flag, NULL, settings, "uint32_t", "first_slice_segment_in_pic_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.dependent_slice_segment_flag, NULL, settings, "uint32_t", "dependent_slice_segment_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.slice_sao_luma_flag, NULL, settings, "uint32_t", "slice_sao_luma_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.slice_sao_chroma_flag, NULL, settings, "uint32_t", "slice_sao_chroma_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.num_ref_idx_active_override_flag, NULL, settings, "uint32_t", "num_ref_idx_active_override_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.mvd_l1_zero_flag, NULL, settings, "uint32_t", "mvd_l1_zero_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.cabac_init_flag, NULL, settings, "uint32_t", "cabac_init_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.cu_chroma_qp_offset_enabled_flag, NULL, settings, "uint32_t", "cu_chroma_qp_offset_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.deblocking_filter_override_flag, NULL, settings, "uint32_t", "deblocking_filter_override_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.slice_deblocking_filter_disabled_flag, NULL, settings, "uint32_t", "slice_deblocking_filter_disabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.collocated_from_l0_flag, NULL, settings, "uint32_t", "collocated_from_l0_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.slice_loop_filter_across_slices_enabled_flag, NULL, settings, "uint32_t", "slice_loop_filter_across_slices_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265SliceSegmentHeader(const StdVideoEncodeH265SliceSegmentHeader& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH265SliceSegmentHeaderFlags>(object.flags, NULL, settings, "StdVideoEncodeH265SliceSegmentHeaderFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH265SliceSegmentHeaderFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH265SliceType>(object.slice_type, NULL, settings, "StdVideoH265SliceType", "slice_type", false, false, indents + 1, dump_json_StdVideoH265SliceType);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.slice_segment_address, NULL, settings, "uint32_t", "slice_segment_address", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.collocated_ref_idx, NULL, settings, "uint8_t", "collocated_ref_idx", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.MaxNumMergeCand, NULL, settings, "uint8_t", "MaxNumMergeCand", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_cb_qp_offset, NULL, settings, "int8_t", "slice_cb_qp_offset", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_cr_qp_offset, NULL, settings, "int8_t", "slice_cr_qp_offset", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_beta_offset_div2, NULL, settings, "int8_t", "slice_beta_offset_div2", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_tc_offset_div2, NULL, settings, "int8_t", "slice_tc_offset_div2", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_act_y_qp_offset, NULL, settings, "int8_t", "slice_act_y_qp_offset", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_act_cb_qp_offset, NULL, settings, "int8_t", "slice_act_cb_qp_offset", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_act_cr_qp_offset, NULL, settings, "int8_t", "slice_act_cr_qp_offset", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.slice_qp_delta, NULL, settings, "int8_t", "slice_qp_delta", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.reserved1, NULL, settings, "uint16_t", "reserved1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeH265WeightTable>(object.pWeightTable, settings, "const StdVideoEncodeH265WeightTable*", "pWeightTable", true, false, indents + 1, dump_json_StdVideoEncodeH265WeightTable);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265ReferenceListsInfoFlags(const StdVideoEncodeH265ReferenceListsInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.ref_pic_list_modification_flag_l0, NULL, settings, "uint32_t", "ref_pic_list_modification_flag_l0", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.ref_pic_list_modification_flag_l1, NULL, settings, "uint32_t", "ref_pic_list_modification_flag_l1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265ReferenceListsInfo(const StdVideoEncodeH265ReferenceListsInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH265ReferenceListsInfoFlags>(object.flags, NULL, settings, "StdVideoEncodeH265ReferenceListsInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH265ReferenceListsInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_ref_idx_l0_active_minus1, NULL, settings, "uint8_t", "num_ref_idx_l0_active_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_ref_idx_l1_active_minus1, NULL, settings, "uint8_t", "num_ref_idx_l1_active_minus1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.RefPicList0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "uint8_t", "RefPicList0", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.RefPicList1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "uint8_t", "RefPicList1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.list_entry_l0, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "uint8_t", "list_entry_l0", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.list_entry_l1, STD_VIDEO_H265_MAX_NUM_LIST_REF, settings, "uint8_t[STD_VIDEO_H265_MAX_NUM_LIST_REF]", "uint8_t", "list_entry_l1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265PictureInfoFlags(const StdVideoEncodeH265PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.is_reference, NULL, settings, "uint32_t", "is_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.IrapPicFlag, NULL, settings, "uint32_t", "IrapPicFlag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.used_for_long_term_reference, NULL, settings, "uint32_t", "used_for_long_term_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.discardable_flag, NULL, settings, "uint32_t", "discardable_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.cross_layer_bla_flag, NULL, settings, "uint32_t", "cross_layer_bla_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.pic_output_flag, NULL, settings, "uint32_t", "pic_output_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.no_output_of_prior_pics_flag, NULL, settings, "uint32_t", "no_output_of_prior_pics_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.short_term_ref_pic_set_sps_flag, NULL, settings, "uint32_t", "short_term_ref_pic_set_sps_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.slice_temporal_mvp_enabled_flag, NULL, settings, "uint32_t", "slice_temporal_mvp_enabled_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265LongTermRefPics(const StdVideoEncodeH265LongTermRefPics& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint8_t>(object.num_long_term_sps, NULL, settings, "uint8_t", "num_long_term_sps", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_long_term_pics, NULL, settings, "uint8_t", "num_long_term_pics", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.lt_idx_sps, STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS, settings, "uint8_t[STD_VIDEO_H265_MAX_LONG_TERM_REF_PICS_SPS]", "uint8_t", "lt_idx_sps", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.poc_lsb_lt, STD_VIDEO_H265_MAX_LONG_TERM_PICS, settings, "uint8_t[STD_VIDEO_H265_MAX_LONG_TERM_PICS]", "uint8_t", "poc_lsb_lt", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.used_by_curr_pic_lt_flag, NULL, settings, "uint16_t", "used_by_curr_pic_lt_flag", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.delta_poc_msb_present_flag, STD_VIDEO_H265_MAX_DELTA_POC, settings, "uint8_t[STD_VIDEO_H265_MAX_DELTA_POC]", "uint8_t", "delta_poc_msb_present_flag", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.delta_poc_msb_cycle_lt, STD_VIDEO_H265_MAX_DELTA_POC, settings, "uint8_t[STD_VIDEO_H265_MAX_DELTA_POC]", "uint8_t", "delta_poc_msb_cycle_lt", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265PictureInfo(const StdVideoEncodeH265PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH265PictureInfoFlags>(object.flags, NULL, settings, "StdVideoEncodeH265PictureInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH265PictureInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH265PictureType>(object.pic_type, NULL, settings, "StdVideoH265PictureType", "pic_type", false, false, indents + 1, dump_json_StdVideoH265PictureType);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.sps_video_parameter_set_id, NULL, settings, "uint8_t", "sps_video_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pps_seq_parameter_set_id, NULL, settings, "uint8_t", "pps_seq_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.pps_pic_parameter_set_id, NULL, settings, "uint8_t", "pps_pic_parameter_set_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.short_term_ref_pic_set_idx, NULL, settings, "uint8_t", "short_term_ref_pic_set_idx", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const int32_t>(object.PicOrderCntVal, NULL, settings, "int32_t", "PicOrderCntVal", false, false, indents + 1, dump_json_int32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.TemporalId, NULL, settings, "uint8_t", "TemporalId", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.reserved1, 7, settings, "uint8_t[7]", "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeH265ReferenceListsInfo>(object.pRefLists, settings, "const StdVideoEncodeH265ReferenceListsInfo*", "pRefLists", true, false, indents + 1, dump_json_StdVideoEncodeH265ReferenceListsInfo);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoH265ShortTermRefPicSet>(object.pShortTermRefPicSet, settings, "const StdVideoH265ShortTermRefPicSet*", "pShortTermRefPicSet", true, false, indents + 1, dump_json_StdVideoH265ShortTermRefPicSet);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeH265LongTermRefPics>(object.pLongTermRefPics, settings, "const StdVideoEncodeH265LongTermRefPics*", "pLongTermRefPics", true, false, indents + 1, dump_json_StdVideoEncodeH265LongTermRefPics);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265ReferenceInfoFlags(const StdVideoEncodeH265ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.used_for_long_term_reference, NULL, settings, "uint32_t", "used_for_long_term_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.unused_for_reference, NULL, settings, "uint32_t", "unused_for_reference", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeH265ReferenceInfo(const StdVideoEncodeH265ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeH265ReferenceInfoFlags>(object.flags, NULL, settings, "StdVideoEncodeH265ReferenceInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeH265ReferenceInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoH265PictureType>(object.pic_type, NULL, settings, "StdVideoH265PictureType", "pic_type", false, false, indents + 1, dump_json_StdVideoH265PictureType);
    settings.stream() << ",\n";
    dump_json_value<const int32_t>(object.PicOrderCntVal, NULL, settings, "int32_t", "PicOrderCntVal", false, false, indents + 1, dump_json_int32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.TemporalId, NULL, settings, "uint8_t", "TemporalId", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1ColorConfigFlags(const StdVideoAV1ColorConfigFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.mono_chrome, NULL, settings, "uint32_t", "mono_chrome", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.color_range, NULL, settings, "uint32_t", "color_range", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.separate_uv_delta_q, NULL, settings, "uint32_t", "separate_uv_delta_q", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.color_description_present_flag, NULL, settings, "uint32_t", "color_description_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1ColorConfig(const StdVideoAV1ColorConfig& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoAV1ColorConfigFlags>(object.flags, NULL, settings, "StdVideoAV1ColorConfigFlags", "flags", true, false, indents + 1, dump_json_StdVideoAV1ColorConfigFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.BitDepth, NULL, settings, "uint8_t", "BitDepth", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.subsampling_x, NULL, settings, "uint8_t", "subsampling_x", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.subsampling_y, NULL, settings, "uint8_t", "subsampling_y", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1ColorPrimaries>(object.color_primaries, NULL, settings, "StdVideoAV1ColorPrimaries", "color_primaries", false, false, indents + 1, dump_json_StdVideoAV1ColorPrimaries);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1TransferCharacteristics>(object.transfer_characteristics, NULL, settings, "StdVideoAV1TransferCharacteristics", "transfer_characteristics", false, false, indents + 1, dump_json_StdVideoAV1TransferCharacteristics);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1MatrixCoefficients>(object.matrix_coefficients, NULL, settings, "StdVideoAV1MatrixCoefficients", "matrix_coefficients", false, false, indents + 1, dump_json_StdVideoAV1MatrixCoefficients);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1ChromaSamplePosition>(object.chroma_sample_position, NULL, settings, "StdVideoAV1ChromaSamplePosition", "chroma_sample_position", false, false, indents + 1, dump_json_StdVideoAV1ChromaSamplePosition);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1TimingInfoFlags(const StdVideoAV1TimingInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.equal_picture_interval, NULL, settings, "uint32_t", "equal_picture_interval", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1TimingInfo(const StdVideoAV1TimingInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoAV1TimingInfoFlags>(object.flags, NULL, settings, "StdVideoAV1TimingInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoAV1TimingInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.num_units_in_display_tick, NULL, settings, "uint32_t", "num_units_in_display_tick", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.time_scale, NULL, settings, "uint32_t", "time_scale", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.num_ticks_per_picture_minus_1, NULL, settings, "uint32_t", "num_ticks_per_picture_minus_1", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1LoopFilterFlags(const StdVideoAV1LoopFilterFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.loop_filter_delta_enabled, NULL, settings, "uint32_t", "loop_filter_delta_enabled", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.loop_filter_delta_update, NULL, settings, "uint32_t", "loop_filter_delta_update", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1LoopFilter(const StdVideoAV1LoopFilter& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoAV1LoopFilterFlags>(object.flags, NULL, settings, "StdVideoAV1LoopFilterFlags", "flags", true, false, indents + 1, dump_json_StdVideoAV1LoopFilterFlags);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.loop_filter_level, STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_LOOP_FILTER_STRENGTHS]", "uint8_t", "loop_filter_level", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.loop_filter_sharpness, NULL, settings, "uint8_t", "loop_filter_sharpness", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.update_ref_delta, NULL, settings, "uint8_t", "update_ref_delta", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.loop_filter_ref_deltas, STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME, settings, "int8_t[STD_VIDEO_AV1_TOTAL_REFS_PER_FRAME]", "int8_t", "loop_filter_ref_deltas", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.update_mode_delta, NULL, settings, "uint8_t", "update_mode_delta", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.loop_filter_mode_deltas, STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS, settings, "int8_t[STD_VIDEO_AV1_LOOP_FILTER_ADJUSTMENTS]", "int8_t", "loop_filter_mode_deltas", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1QuantizationFlags(const StdVideoAV1QuantizationFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.using_qmatrix, NULL, settings, "uint32_t", "using_qmatrix", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.diff_uv_delta, NULL, settings, "uint32_t", "diff_uv_delta", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1Quantization(const StdVideoAV1Quantization& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoAV1QuantizationFlags>(object.flags, NULL, settings, "StdVideoAV1QuantizationFlags", "flags", true, false, indents + 1, dump_json_StdVideoAV1QuantizationFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.base_q_idx, NULL, settings, "uint8_t", "base_q_idx", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.DeltaQYDc, NULL, settings, "int8_t", "DeltaQYDc", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.DeltaQUDc, NULL, settings, "int8_t", "DeltaQUDc", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.DeltaQUAc, NULL, settings, "int8_t", "DeltaQUAc", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.DeltaQVDc, NULL, settings, "int8_t", "DeltaQVDc", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.DeltaQVAc, NULL, settings, "int8_t", "DeltaQVAc", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.qm_y, NULL, settings, "uint8_t", "qm_y", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.qm_u, NULL, settings, "uint8_t", "qm_u", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.qm_v, NULL, settings, "uint8_t", "qm_v", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1Segmentation(const StdVideoAV1Segmentation& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_array<const uint8_t>(object.FeatureEnabled, STD_VIDEO_AV1_MAX_SEGMENTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_SEGMENTS]", "uint8_t", "FeatureEnabled", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1TileInfoFlags(const StdVideoAV1TileInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.uniform_tile_spacing_flag, NULL, settings, "uint32_t", "uniform_tile_spacing_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1TileInfo(const StdVideoAV1TileInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoAV1TileInfoFlags>(object.flags, NULL, settings, "StdVideoAV1TileInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoAV1TileInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.TileCols, NULL, settings, "uint8_t", "TileCols", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.TileRows, NULL, settings, "uint8_t", "TileRows", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.context_update_tile_id, NULL, settings, "uint16_t", "context_update_tile_id", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.tile_size_bytes_minus_1, NULL, settings, "uint8_t", "tile_size_bytes_minus_1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.reserved1, 7, settings, "uint8_t[7]", "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const uint16_t>(object.pMiColStarts, settings, "const uint16_t*", "pMiColStarts", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_pointer<const uint16_t>(object.pMiRowStarts, settings, "const uint16_t*", "pMiRowStarts", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_pointer<const uint16_t>(object.pWidthInSbsMinus1, settings, "const uint16_t*", "pWidthInSbsMinus1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_pointer<const uint16_t>(object.pHeightInSbsMinus1, settings, "const uint16_t*", "pHeightInSbsMinus1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1CDEF(const StdVideoAV1CDEF& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint8_t>(object.cdef_damping_minus_3, NULL, settings, "uint8_t", "cdef_damping_minus_3", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.cdef_bits, NULL, settings, "uint8_t", "cdef_bits", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.cdef_y_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "uint8_t", "cdef_y_pri_strength", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.cdef_y_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "uint8_t", "cdef_y_sec_strength", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.cdef_uv_pri_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "uint8_t", "cdef_uv_pri_strength", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.cdef_uv_sec_strength, STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS, settings, "uint8_t[STD_VIDEO_AV1_MAX_CDEF_FILTER_STRENGTHS]", "uint8_t", "cdef_uv_sec_strength", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1LoopRestoration(const StdVideoAV1LoopRestoration& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_array<const StdVideoAV1FrameRestorationType>(object.FrameRestorationType, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, "StdVideoAV1FrameRestorationType[STD_VIDEO_AV1_MAX_NUM_PLANES]", "StdVideoAV1FrameRestorationType", "FrameRestorationType", false, false, indents + 1, dump_json_StdVideoAV1FrameRestorationType); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint16_t>(object.LoopRestorationSize, STD_VIDEO_AV1_MAX_NUM_PLANES, settings, "uint16_t[STD_VIDEO_AV1_MAX_NUM_PLANES]", "uint16_t", "LoopRestorationSize", false, false, indents + 1, dump_json_uint16_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1GlobalMotion(const StdVideoAV1GlobalMotion& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_array<const uint8_t>(object.GmType, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint8_t", "GmType", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1FilmGrainFlags(const StdVideoAV1FilmGrainFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.chroma_scaling_from_luma, NULL, settings, "uint32_t", "chroma_scaling_from_luma", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.overlap_flag, NULL, settings, "uint32_t", "overlap_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.clip_to_restricted_range, NULL, settings, "uint32_t", "clip_to_restricted_range", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.update_grain, NULL, settings, "uint32_t", "update_grain", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1FilmGrain(const StdVideoAV1FilmGrain& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoAV1FilmGrainFlags>(object.flags, NULL, settings, "StdVideoAV1FilmGrainFlags", "flags", true, false, indents + 1, dump_json_StdVideoAV1FilmGrainFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.grain_scaling_minus_8, NULL, settings, "uint8_t", "grain_scaling_minus_8", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.ar_coeff_lag, NULL, settings, "uint8_t", "ar_coeff_lag", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.ar_coeff_shift_minus_6, NULL, settings, "uint8_t", "ar_coeff_shift_minus_6", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.grain_scale_shift, NULL, settings, "uint8_t", "grain_scale_shift", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.grain_seed, NULL, settings, "uint16_t", "grain_seed", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.film_grain_params_ref_idx, NULL, settings, "uint8_t", "film_grain_params_ref_idx", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_y_points, NULL, settings, "uint8_t", "num_y_points", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.point_y_value, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_Y_POINTS]", "uint8_t", "point_y_value", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.point_y_scaling, STD_VIDEO_AV1_MAX_NUM_Y_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_Y_POINTS]", "uint8_t", "point_y_scaling", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_cb_points, NULL, settings, "uint8_t", "num_cb_points", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.point_cb_value, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CB_POINTS]", "uint8_t", "point_cb_value", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.point_cb_scaling, STD_VIDEO_AV1_MAX_NUM_CB_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CB_POINTS]", "uint8_t", "point_cb_scaling", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.num_cr_points, NULL, settings, "uint8_t", "num_cr_points", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.point_cr_value, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CR_POINTS]", "uint8_t", "point_cr_value", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.point_cr_scaling, STD_VIDEO_AV1_MAX_NUM_CR_POINTS, settings, "uint8_t[STD_VIDEO_AV1_MAX_NUM_CR_POINTS]", "uint8_t", "point_cr_scaling", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.ar_coeffs_y_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_LUMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_LUMA]", "int8_t", "ar_coeffs_y_plus_128", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.ar_coeffs_cb_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_CHROMA]", "int8_t", "ar_coeffs_cb_plus_128", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.ar_coeffs_cr_plus_128, STD_VIDEO_AV1_MAX_NUM_POS_CHROMA, settings, "int8_t[STD_VIDEO_AV1_MAX_NUM_POS_CHROMA]", "int8_t", "ar_coeffs_cr_plus_128", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.cb_mult, NULL, settings, "uint8_t", "cb_mult", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.cb_luma_mult, NULL, settings, "uint8_t", "cb_luma_mult", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.cb_offset, NULL, settings, "uint16_t", "cb_offset", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.cr_mult, NULL, settings, "uint8_t", "cr_mult", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.cr_luma_mult, NULL, settings, "uint8_t", "cr_luma_mult", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.cr_offset, NULL, settings, "uint16_t", "cr_offset", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1SequenceHeaderFlags(const StdVideoAV1SequenceHeaderFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.still_picture, NULL, settings, "uint32_t", "still_picture", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reduced_still_picture_header, NULL, settings, "uint32_t", "reduced_still_picture_header", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.use_128x128_superblock, NULL, settings, "uint32_t", "use_128x128_superblock", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_filter_intra, NULL, settings, "uint32_t", "enable_filter_intra", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_intra_edge_filter, NULL, settings, "uint32_t", "enable_intra_edge_filter", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_interintra_compound, NULL, settings, "uint32_t", "enable_interintra_compound", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_masked_compound, NULL, settings, "uint32_t", "enable_masked_compound", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_warped_motion, NULL, settings, "uint32_t", "enable_warped_motion", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_dual_filter, NULL, settings, "uint32_t", "enable_dual_filter", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_order_hint, NULL, settings, "uint32_t", "enable_order_hint", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_jnt_comp, NULL, settings, "uint32_t", "enable_jnt_comp", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_ref_frame_mvs, NULL, settings, "uint32_t", "enable_ref_frame_mvs", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_id_numbers_present_flag, NULL, settings, "uint32_t", "frame_id_numbers_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_superres, NULL, settings, "uint32_t", "enable_superres", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_cdef, NULL, settings, "uint32_t", "enable_cdef", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.enable_restoration, NULL, settings, "uint32_t", "enable_restoration", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.film_grain_params_present, NULL, settings, "uint32_t", "film_grain_params_present", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.timing_info_present_flag, NULL, settings, "uint32_t", "timing_info_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.initial_display_delay_present_flag, NULL, settings, "uint32_t", "initial_display_delay_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoAV1SequenceHeader(const StdVideoAV1SequenceHeader& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoAV1SequenceHeaderFlags>(object.flags, NULL, settings, "StdVideoAV1SequenceHeaderFlags", "flags", true, false, indents + 1, dump_json_StdVideoAV1SequenceHeaderFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1Profile>(object.seq_profile, NULL, settings, "StdVideoAV1Profile", "seq_profile", false, false, indents + 1, dump_json_StdVideoAV1Profile);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.frame_width_bits_minus_1, NULL, settings, "uint8_t", "frame_width_bits_minus_1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.frame_height_bits_minus_1, NULL, settings, "uint8_t", "frame_height_bits_minus_1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.max_frame_width_minus_1, NULL, settings, "uint16_t", "max_frame_width_minus_1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.max_frame_height_minus_1, NULL, settings, "uint16_t", "max_frame_height_minus_1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.delta_frame_id_length_minus_2, NULL, settings, "uint8_t", "delta_frame_id_length_minus_2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.additional_frame_id_length_minus_1, NULL, settings, "uint8_t", "additional_frame_id_length_minus_1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.order_hint_bits_minus_1, NULL, settings, "uint8_t", "order_hint_bits_minus_1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.seq_force_integer_mv, NULL, settings, "uint8_t", "seq_force_integer_mv", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.seq_force_screen_content_tools, NULL, settings, "uint8_t", "seq_force_screen_content_tools", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.reserved1, 5, settings, "uint8_t[5]", "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1ColorConfig>(object.pColorConfig, settings, "const StdVideoAV1ColorConfig*", "pColorConfig", true, false, indents + 1, dump_json_StdVideoAV1ColorConfig);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1TimingInfo>(object.pTimingInfo, settings, "const StdVideoAV1TimingInfo*", "pTimingInfo", true, false, indents + 1, dump_json_StdVideoAV1TimingInfo);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeAV1PictureInfoFlags(const StdVideoDecodeAV1PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.error_resilient_mode, NULL, settings, "uint32_t", "error_resilient_mode", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.disable_cdf_update, NULL, settings, "uint32_t", "disable_cdf_update", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.use_superres, NULL, settings, "uint32_t", "use_superres", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.render_and_frame_size_different, NULL, settings, "uint32_t", "render_and_frame_size_different", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.allow_screen_content_tools, NULL, settings, "uint32_t", "allow_screen_content_tools", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.is_filter_switchable, NULL, settings, "uint32_t", "is_filter_switchable", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.force_integer_mv, NULL, settings, "uint32_t", "force_integer_mv", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_size_override_flag, NULL, settings, "uint32_t", "frame_size_override_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.buffer_removal_time_present_flag, NULL, settings, "uint32_t", "buffer_removal_time_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.allow_intrabc, NULL, settings, "uint32_t", "allow_intrabc", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_refs_short_signaling, NULL, settings, "uint32_t", "frame_refs_short_signaling", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.allow_high_precision_mv, NULL, settings, "uint32_t", "allow_high_precision_mv", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.is_motion_mode_switchable, NULL, settings, "uint32_t", "is_motion_mode_switchable", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.use_ref_frame_mvs, NULL, settings, "uint32_t", "use_ref_frame_mvs", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.disable_frame_end_update_cdf, NULL, settings, "uint32_t", "disable_frame_end_update_cdf", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.allow_warped_motion, NULL, settings, "uint32_t", "allow_warped_motion", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reduced_tx_set, NULL, settings, "uint32_t", "reduced_tx_set", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reference_select, NULL, settings, "uint32_t", "reference_select", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.skip_mode_present, NULL, settings, "uint32_t", "skip_mode_present", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.delta_q_present, NULL, settings, "uint32_t", "delta_q_present", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.delta_lf_present, NULL, settings, "uint32_t", "delta_lf_present", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.delta_lf_multi, NULL, settings, "uint32_t", "delta_lf_multi", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_enabled, NULL, settings, "uint32_t", "segmentation_enabled", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_update_map, NULL, settings, "uint32_t", "segmentation_update_map", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_temporal_update, NULL, settings, "uint32_t", "segmentation_temporal_update", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_update_data, NULL, settings, "uint32_t", "segmentation_update_data", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.UsesLr, NULL, settings, "uint32_t", "UsesLr", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.usesChromaLr, NULL, settings, "uint32_t", "usesChromaLr", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.apply_grain, NULL, settings, "uint32_t", "apply_grain", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeAV1PictureInfo(const StdVideoDecodeAV1PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoDecodeAV1PictureInfoFlags>(object.flags, NULL, settings, "StdVideoDecodeAV1PictureInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoDecodeAV1PictureInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1FrameType>(object.frame_type, NULL, settings, "StdVideoAV1FrameType", "frame_type", false, false, indents + 1, dump_json_StdVideoAV1FrameType);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.current_frame_id, NULL, settings, "uint32_t", "current_frame_id", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.OrderHint, NULL, settings, "uint8_t", "OrderHint", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.primary_ref_frame, NULL, settings, "uint8_t", "primary_ref_frame", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.refresh_frame_flags, NULL, settings, "uint8_t", "refresh_frame_flags", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1InterpolationFilter>(object.interpolation_filter, NULL, settings, "StdVideoAV1InterpolationFilter", "interpolation_filter", false, false, indents + 1, dump_json_StdVideoAV1InterpolationFilter);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1TxMode>(object.TxMode, NULL, settings, "StdVideoAV1TxMode", "TxMode", false, false, indents + 1, dump_json_StdVideoAV1TxMode);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.delta_q_res, NULL, settings, "uint8_t", "delta_q_res", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.delta_lf_res, NULL, settings, "uint8_t", "delta_lf_res", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.SkipModeFrame, STD_VIDEO_AV1_SKIP_MODE_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_SKIP_MODE_FRAMES]", "uint8_t", "SkipModeFrame", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.coded_denom, NULL, settings, "uint8_t", "coded_denom", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.reserved2, 3, settings, "uint8_t[3]", "uint8_t", "reserved2", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.OrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint8_t", "OrderHints", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint32_t>(object.expectedFrameId, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint32_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint32_t", "expectedFrameId", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1TileInfo>(object.pTileInfo, settings, "const StdVideoAV1TileInfo*", "pTileInfo", true, false, indents + 1, dump_json_StdVideoAV1TileInfo);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1Quantization>(object.pQuantization, settings, "const StdVideoAV1Quantization*", "pQuantization", true, false, indents + 1, dump_json_StdVideoAV1Quantization);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1Segmentation>(object.pSegmentation, settings, "const StdVideoAV1Segmentation*", "pSegmentation", true, false, indents + 1, dump_json_StdVideoAV1Segmentation);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1LoopFilter>(object.pLoopFilter, settings, "const StdVideoAV1LoopFilter*", "pLoopFilter", true, false, indents + 1, dump_json_StdVideoAV1LoopFilter);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1CDEF>(object.pCDEF, settings, "const StdVideoAV1CDEF*", "pCDEF", true, false, indents + 1, dump_json_StdVideoAV1CDEF);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1LoopRestoration>(object.pLoopRestoration, settings, "const StdVideoAV1LoopRestoration*", "pLoopRestoration", true, false, indents + 1, dump_json_StdVideoAV1LoopRestoration);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1GlobalMotion>(object.pGlobalMotion, settings, "const StdVideoAV1GlobalMotion*", "pGlobalMotion", true, false, indents + 1, dump_json_StdVideoAV1GlobalMotion);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1FilmGrain>(object.pFilmGrain, settings, "const StdVideoAV1FilmGrain*", "pFilmGrain", true, false, indents + 1, dump_json_StdVideoAV1FilmGrain);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeAV1ReferenceInfoFlags(const StdVideoDecodeAV1ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.disable_frame_end_update_cdf, NULL, settings, "uint32_t", "disable_frame_end_update_cdf", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_enabled, NULL, settings, "uint32_t", "segmentation_enabled", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeAV1ReferenceInfo(const StdVideoDecodeAV1ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoDecodeAV1ReferenceInfoFlags>(object.flags, NULL, settings, "StdVideoDecodeAV1ReferenceInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoDecodeAV1ReferenceInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.frame_type, NULL, settings, "uint8_t", "frame_type", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.RefFrameSignBias, NULL, settings, "uint8_t", "RefFrameSignBias", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.OrderHint, NULL, settings, "uint8_t", "OrderHint", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.SavedOrderHints, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint8_t", "SavedOrderHints", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeAV1DecoderModelInfo(const StdVideoEncodeAV1DecoderModelInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint8_t>(object.buffer_delay_length_minus_1, NULL, settings, "uint8_t", "buffer_delay_length_minus_1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.buffer_removal_time_length_minus_1, NULL, settings, "uint8_t", "buffer_removal_time_length_minus_1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.frame_presentation_time_length_minus_1, NULL, settings, "uint8_t", "frame_presentation_time_length_minus_1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.num_units_in_decoding_tick, NULL, settings, "uint32_t", "num_units_in_decoding_tick", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeAV1ExtensionHeader(const StdVideoEncodeAV1ExtensionHeader& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint8_t>(object.temporal_id, NULL, settings, "uint8_t", "temporal_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.spatial_id, NULL, settings, "uint8_t", "spatial_id", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeAV1OperatingPointInfoFlags(const StdVideoEncodeAV1OperatingPointInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.decoder_model_present_for_this_op, NULL, settings, "uint32_t", "decoder_model_present_for_this_op", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.low_delay_mode_flag, NULL, settings, "uint32_t", "low_delay_mode_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.initial_display_delay_present_for_this_op, NULL, settings, "uint32_t", "initial_display_delay_present_for_this_op", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeAV1OperatingPointInfo(const StdVideoEncodeAV1OperatingPointInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeAV1OperatingPointInfoFlags>(object.flags, NULL, settings, "StdVideoEncodeAV1OperatingPointInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeAV1OperatingPointInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.operating_point_idc, NULL, settings, "uint16_t", "operating_point_idc", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.seq_level_idx, NULL, settings, "uint8_t", "seq_level_idx", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.seq_tier, NULL, settings, "uint8_t", "seq_tier", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.decoder_buffer_delay, NULL, settings, "uint32_t", "decoder_buffer_delay", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.encoder_buffer_delay, NULL, settings, "uint32_t", "encoder_buffer_delay", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.initial_display_delay_minus_1, NULL, settings, "uint8_t", "initial_display_delay_minus_1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeAV1PictureInfoFlags(const StdVideoEncodeAV1PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.error_resilient_mode, NULL, settings, "uint32_t", "error_resilient_mode", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.disable_cdf_update, NULL, settings, "uint32_t", "disable_cdf_update", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.use_superres, NULL, settings, "uint32_t", "use_superres", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.render_and_frame_size_different, NULL, settings, "uint32_t", "render_and_frame_size_different", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.allow_screen_content_tools, NULL, settings, "uint32_t", "allow_screen_content_tools", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.is_filter_switchable, NULL, settings, "uint32_t", "is_filter_switchable", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.force_integer_mv, NULL, settings, "uint32_t", "force_integer_mv", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_size_override_flag, NULL, settings, "uint32_t", "frame_size_override_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.buffer_removal_time_present_flag, NULL, settings, "uint32_t", "buffer_removal_time_present_flag", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.allow_intrabc, NULL, settings, "uint32_t", "allow_intrabc", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_refs_short_signaling, NULL, settings, "uint32_t", "frame_refs_short_signaling", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.allow_high_precision_mv, NULL, settings, "uint32_t", "allow_high_precision_mv", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.is_motion_mode_switchable, NULL, settings, "uint32_t", "is_motion_mode_switchable", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.use_ref_frame_mvs, NULL, settings, "uint32_t", "use_ref_frame_mvs", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.disable_frame_end_update_cdf, NULL, settings, "uint32_t", "disable_frame_end_update_cdf", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.allow_warped_motion, NULL, settings, "uint32_t", "allow_warped_motion", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reduced_tx_set, NULL, settings, "uint32_t", "reduced_tx_set", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.skip_mode_present, NULL, settings, "uint32_t", "skip_mode_present", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.delta_q_present, NULL, settings, "uint32_t", "delta_q_present", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.delta_lf_present, NULL, settings, "uint32_t", "delta_lf_present", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.delta_lf_multi, NULL, settings, "uint32_t", "delta_lf_multi", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_enabled, NULL, settings, "uint32_t", "segmentation_enabled", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_update_map, NULL, settings, "uint32_t", "segmentation_update_map", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_temporal_update, NULL, settings, "uint32_t", "segmentation_temporal_update", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_update_data, NULL, settings, "uint32_t", "segmentation_update_data", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.UsesLr, NULL, settings, "uint32_t", "UsesLr", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.usesChromaLr, NULL, settings, "uint32_t", "usesChromaLr", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.show_frame, NULL, settings, "uint32_t", "show_frame", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.showable_frame, NULL, settings, "uint32_t", "showable_frame", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeAV1PictureInfo(const StdVideoEncodeAV1PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeAV1PictureInfoFlags>(object.flags, NULL, settings, "StdVideoEncodeAV1PictureInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeAV1PictureInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1FrameType>(object.frame_type, NULL, settings, "StdVideoAV1FrameType", "frame_type", false, false, indents + 1, dump_json_StdVideoAV1FrameType);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_presentation_time, NULL, settings, "uint32_t", "frame_presentation_time", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.current_frame_id, NULL, settings, "uint32_t", "current_frame_id", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.order_hint, NULL, settings, "uint8_t", "order_hint", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.primary_ref_frame, NULL, settings, "uint8_t", "primary_ref_frame", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.refresh_frame_flags, NULL, settings, "uint8_t", "refresh_frame_flags", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.coded_denom, NULL, settings, "uint8_t", "coded_denom", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.render_width_minus_1, NULL, settings, "uint16_t", "render_width_minus_1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const uint16_t>(object.render_height_minus_1, NULL, settings, "uint16_t", "render_height_minus_1", false, false, indents + 1, dump_json_uint16_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1InterpolationFilter>(object.interpolation_filter, NULL, settings, "StdVideoAV1InterpolationFilter", "interpolation_filter", false, false, indents + 1, dump_json_StdVideoAV1InterpolationFilter);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1TxMode>(object.TxMode, NULL, settings, "StdVideoAV1TxMode", "TxMode", false, false, indents + 1, dump_json_StdVideoAV1TxMode);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.delta_q_res, NULL, settings, "uint8_t", "delta_q_res", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.delta_lf_res, NULL, settings, "uint8_t", "delta_lf_res", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.ref_order_hint, STD_VIDEO_AV1_NUM_REF_FRAMES, settings, "uint8_t[STD_VIDEO_AV1_NUM_REF_FRAMES]", "uint8_t", "ref_order_hint", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.ref_frame_idx, STD_VIDEO_AV1_REFS_PER_FRAME, settings, "int8_t[STD_VIDEO_AV1_REFS_PER_FRAME]", "int8_t", "ref_frame_idx", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.reserved1, 3, settings, "uint8_t[3]", "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint32_t>(object.delta_frame_id_minus_1, STD_VIDEO_AV1_REFS_PER_FRAME, settings, "uint32_t[STD_VIDEO_AV1_REFS_PER_FRAME]", "uint32_t", "delta_frame_id_minus_1", false, false, indents + 1, dump_json_uint32_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1TileInfo>(object.pTileInfo, settings, "const StdVideoAV1TileInfo*", "pTileInfo", true, false, indents + 1, dump_json_StdVideoAV1TileInfo);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1Quantization>(object.pQuantization, settings, "const StdVideoAV1Quantization*", "pQuantization", true, false, indents + 1, dump_json_StdVideoAV1Quantization);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1Segmentation>(object.pSegmentation, settings, "const StdVideoAV1Segmentation*", "pSegmentation", true, false, indents + 1, dump_json_StdVideoAV1Segmentation);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1LoopFilter>(object.pLoopFilter, settings, "const StdVideoAV1LoopFilter*", "pLoopFilter", true, false, indents + 1, dump_json_StdVideoAV1LoopFilter);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1CDEF>(object.pCDEF, settings, "const StdVideoAV1CDEF*", "pCDEF", true, false, indents + 1, dump_json_StdVideoAV1CDEF);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1LoopRestoration>(object.pLoopRestoration, settings, "const StdVideoAV1LoopRestoration*", "pLoopRestoration", true, false, indents + 1, dump_json_StdVideoAV1LoopRestoration);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoAV1GlobalMotion>(object.pGlobalMotion, settings, "const StdVideoAV1GlobalMotion*", "pGlobalMotion", true, false, indents + 1, dump_json_StdVideoAV1GlobalMotion);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeAV1ExtensionHeader>(object.pExtensionHeader, settings, "const StdVideoEncodeAV1ExtensionHeader*", "pExtensionHeader", true, false, indents + 1, dump_json_StdVideoEncodeAV1ExtensionHeader);
    settings.stream() << ",\n";
    dump_json_pointer<const uint32_t>(object.pBufferRemovalTimes, settings, "const uint32_t*", "pBufferRemovalTimes", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeAV1ReferenceInfoFlags(const StdVideoEncodeAV1ReferenceInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.disable_frame_end_update_cdf, NULL, settings, "uint32_t", "disable_frame_end_update_cdf", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_enabled, NULL, settings, "uint32_t", "segmentation_enabled", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoEncodeAV1ReferenceInfo(const StdVideoEncodeAV1ReferenceInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoEncodeAV1ReferenceInfoFlags>(object.flags, NULL, settings, "StdVideoEncodeAV1ReferenceInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoEncodeAV1ReferenceInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.RefFrameId, NULL, settings, "uint32_t", "RefFrameId", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoAV1FrameType>(object.frame_type, NULL, settings, "StdVideoAV1FrameType", "frame_type", false, false, indents + 1, dump_json_StdVideoAV1FrameType);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.OrderHint, NULL, settings, "uint8_t", "OrderHint", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.reserved1, 3, settings, "uint8_t[3]", "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoEncodeAV1ExtensionHeader>(object.pExtensionHeader, settings, "const StdVideoEncodeAV1ExtensionHeader*", "pExtensionHeader", true, false, indents + 1, dump_json_StdVideoEncodeAV1ExtensionHeader);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoVP9ColorConfigFlags(const StdVideoVP9ColorConfigFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.color_range, NULL, settings, "uint32_t", "color_range", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoVP9ColorConfig(const StdVideoVP9ColorConfig& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoVP9ColorConfigFlags>(object.flags, NULL, settings, "StdVideoVP9ColorConfigFlags", "flags", true, false, indents + 1, dump_json_StdVideoVP9ColorConfigFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.BitDepth, NULL, settings, "uint8_t", "BitDepth", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.subsampling_x, NULL, settings, "uint8_t", "subsampling_x", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.subsampling_y, NULL, settings, "uint8_t", "subsampling_y", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reserved1, NULL, settings, "uint8_t", "reserved1", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoVP9ColorSpace>(object.color_space, NULL, settings, "StdVideoVP9ColorSpace", "color_space", false, false, indents + 1, dump_json_StdVideoVP9ColorSpace);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoVP9LoopFilterFlags(const StdVideoVP9LoopFilterFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.loop_filter_delta_enabled, NULL, settings, "uint32_t", "loop_filter_delta_enabled", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.loop_filter_delta_update, NULL, settings, "uint32_t", "loop_filter_delta_update", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoVP9LoopFilter(const StdVideoVP9LoopFilter& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoVP9LoopFilterFlags>(object.flags, NULL, settings, "StdVideoVP9LoopFilterFlags", "flags", true, false, indents + 1, dump_json_StdVideoVP9LoopFilterFlags);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.loop_filter_level, NULL, settings, "uint8_t", "loop_filter_level", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.loop_filter_sharpness, NULL, settings, "uint8_t", "loop_filter_sharpness", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.update_ref_delta, NULL, settings, "uint8_t", "update_ref_delta", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.loop_filter_ref_deltas, STD_VIDEO_VP9_MAX_REF_FRAMES, settings, "int8_t[STD_VIDEO_VP9_MAX_REF_FRAMES]", "int8_t", "loop_filter_ref_deltas", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.update_mode_delta, NULL, settings, "uint8_t", "update_mode_delta", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const int8_t>(object.loop_filter_mode_deltas, STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS, settings, "int8_t[STD_VIDEO_VP9_LOOP_FILTER_ADJUSTMENTS]", "int8_t", "loop_filter_mode_deltas", false, false, indents + 1, dump_json_int8_t); // IQA
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoVP9SegmentationFlags(const StdVideoVP9SegmentationFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.segmentation_update_map, NULL, settings, "uint32_t", "segmentation_update_map", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_temporal_update, NULL, settings, "uint32_t", "segmentation_temporal_update", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_update_data, NULL, settings, "uint32_t", "segmentation_update_data", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_abs_or_delta_update, NULL, settings, "uint32_t", "segmentation_abs_or_delta_update", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoVP9Segmentation(const StdVideoVP9Segmentation& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoVP9SegmentationFlags>(object.flags, NULL, settings, "StdVideoVP9SegmentationFlags", "flags", true, false, indents + 1, dump_json_StdVideoVP9SegmentationFlags);
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.segmentation_tree_probs, STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTATION_TREE_PROBS]", "uint8_t", "segmentation_tree_probs", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.segmentation_pred_prob, STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTATION_PRED_PROB]", "uint8_t", "segmentation_pred_prob", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    dump_json_array<const uint8_t>(object.FeatureEnabled, STD_VIDEO_VP9_MAX_SEGMENTS, settings, "uint8_t[STD_VIDEO_VP9_MAX_SEGMENTS]", "uint8_t", "FeatureEnabled", false, false, indents + 1, dump_json_uint8_t); // IQA
    settings.stream() << ",\n";
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeVP9PictureInfoFlags(const StdVideoDecodeVP9PictureInfoFlags& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const uint32_t>(object.error_resilient_mode, NULL, settings, "uint32_t", "error_resilient_mode", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.intra_only, NULL, settings, "uint32_t", "intra_only", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.allow_high_precision_mv, NULL, settings, "uint32_t", "allow_high_precision_mv", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.refresh_frame_context, NULL, settings, "uint32_t", "refresh_frame_context", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.frame_parallel_decoding_mode, NULL, settings, "uint32_t", "frame_parallel_decoding_mode", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.segmentation_enabled, NULL, settings, "uint32_t", "segmentation_enabled", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.show_frame, NULL, settings, "uint32_t", "show_frame", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.UsePrevFrameMvs, NULL, settings, "uint32_t", "UsePrevFrameMvs", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << ",\n";
    dump_json_value<const uint32_t>(object.reserved, NULL, settings, "uint32_t", "reserved", false, false, indents + 1, dump_json_uint32_t);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}
void dump_json_StdVideoDecodeVP9PictureInfo(const StdVideoDecodeVP9PictureInfo& object, const ApiDumpSettings& settings, int indents)
{
    settings.stream() << settings.indentation(indents) << "[\n";
    dump_json_value<const StdVideoDecodeVP9PictureInfoFlags>(object.flags, NULL, settings, "StdVideoDecodeVP9PictureInfoFlags", "flags", true, false, indents + 1, dump_json_StdVideoDecodeVP9PictureInfoFlags);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoVP9Profile>(object.profile, NULL, settings, "StdVideoVP9Profile", "profile", false, false, indents + 1, dump_json_StdVideoVP9Profile);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoVP9FrameType>(object.frame_type, NULL, settings, "StdVideoVP9FrameType", "frame_type", false, false, indents + 1, dump_json_StdVideoVP9FrameType);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.frame_context_idx, NULL, settings, "uint8_t", "frame_context_idx", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.reset_frame_context, NULL, settings, "uint8_t", "reset_frame_context", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.refresh_frame_flags, NULL, settings, "uint8_t", "refresh_frame_flags", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.ref_frame_sign_bias_mask, NULL, settings, "uint8_t", "ref_frame_sign_bias_mask", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const StdVideoVP9InterpolationFilter>(object.interpolation_filter, NULL, settings, "StdVideoVP9InterpolationFilter", "interpolation_filter", false, false, indents + 1, dump_json_StdVideoVP9InterpolationFilter);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.base_q_idx, NULL, settings, "uint8_t", "base_q_idx", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.delta_q_y_dc, NULL, settings, "int8_t", "delta_q_y_dc", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.delta_q_uv_dc, NULL, settings, "int8_t", "delta_q_uv_dc", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const int8_t>(object.delta_q_uv_ac, NULL, settings, "int8_t", "delta_q_uv_ac", false, false, indents + 1, dump_json_int8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.tile_cols_log2, NULL, settings, "uint8_t", "tile_cols_log2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_value<const uint8_t>(object.tile_rows_log2, NULL, settings, "uint8_t", "tile_rows_log2", false, false, indents + 1, dump_json_uint8_t);
    settings.stream() << ",\n";
    dump_json_array<const uint16_t>(object.reserved1, 3, settings, "uint16_t[3]", "uint16_t", "reserved1", false, false, indents + 1, dump_json_uint16_t); // IQA
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoVP9ColorConfig>(object.pColorConfig, settings, "const StdVideoVP9ColorConfig*", "pColorConfig", true, false, indents + 1, dump_json_StdVideoVP9ColorConfig);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoVP9LoopFilter>(object.pLoopFilter, settings, "const StdVideoVP9LoopFilter*", "pLoopFilter", true, false, indents + 1, dump_json_StdVideoVP9LoopFilter);
    settings.stream() << ",\n";
    dump_json_pointer<const StdVideoVP9Segmentation>(object.pSegmentation, settings, "const StdVideoVP9Segmentation*", "pSegmentation", true, false, indents + 1, dump_json_StdVideoVP9Segmentation);
    settings.stream() << "\n" << settings.indentation(indents) << "]";
}

//========================== Union Implementations ==========================//


//======================== pNext Chain Implementation =======================//


//========================= Function Helpers ================================//

// Display parameter values


//========================= Function Implementations ========================//



