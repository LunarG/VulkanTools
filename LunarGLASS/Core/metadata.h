//===- metadata.h - LLVM Metadata for LunarGLASS -============================//
//
// LunarGLASS: An Open Modular Shader Compiler Architecture
// Copyright (C) 2010-2014 LunarG, Inc.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//     Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//
//     Redistributions in binary form must reproduce the above
//     copyright notice, this list of conditions and the following
//     disclaimer in the documentation and/or other materials provided
//     with the distribution.
//
//     Neither the name of LunarG Inc. nor the names of its
//     contributors may be used to endorse or promote products derived
//     from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
//===----------------------------------------------------------------------===//
//
// Author: John Kessenich, LunarG
//
//===----------------------------------------------------------------------===//

#pragma once
#ifndef metadata_H
#define metadata_H

// LunarGLASS includes
#include "LunarGLASSTopIR.h"

// LLVM includes
#pragma warning(push, 1)
#include "llvm/IR/Module.h"
#include "llvm/IR/Metadata.h"
#pragma warning(pop)

namespace gla {

// Forms of metadata nodes, pointed to by instructions, by named metadata, or by other metadata.
//
// Only the names starting with "!gla." actually appear in the IR, the other names here are
// for ease of understanding the linkage between the nodes.
//
// NOTE: There are *two* forms the recursive type-walking metadata can appear in:
// - Single-Walk form: A single self-recursive node, the same one used to root
//   !gla.input/output/uniform nodes.  This enables a single type tree walker
//   through the metadata nodes.
// - Dual-Walk form: The type is rooted by a !gla.input/output/uniform node, which
//   points to a recursive !aggregate node.  This requires walking the LLVM type
//   in parallel with walking the metadata nodes.
// Only one of these forms should be used in a given module.
//
// Node forms:
//
//     !gla.entrypoint -> { name, EMIoEntrypoint }
//     Notes:
//         - contains the name of an source entry point into the shader; e.g., "main"
//
//     !gla.precision -> { EMdPrecision }
//
//     !gla.io is shorthand for one of !gla.input, !gla.output, !gla.uniform
//
//     !gla.io -> { name, EMdInputOutput, Value*, !typeLayout, !aggregate }
//     This is Dual-Walk form:
//         - the name is the name of the object (instance-name for a block)
//              * for a block with no instance name, the name here will be empty ("")
//         - Value* is a proxy for getting the LLVM type of the root of the type
//         - !aggregate is for blocks and structures
//              * it's a block when EMdInputOutput is EMio*Block*, !typeLayout will say how the block is laid out
//              * it's a structure when the EMdTypeLayout is EMtlAggregate
//         - for blocks, the instance name is the name above, while the interface name is the name in the !aggregate
//
//     !gla.io: { instanceName, EMdInputOutput, Value*, !typeLayout, typeName, !gla.io, !gla.io, !gla.io, ... }
//     This is Single-Walk form
//         - the instanceName is the name of the object instance (instance-name for a block)
//              * for a block with no instance name, the name here will be empty ("")
//         - Value* is a proxy for getting the LLVM type of this root or intermediate type in the tree
//              * this must be looked at to get the current level's arrayness information
//         - typeName is for block interface name or struct type name
//              * will be empty ("") if this level is not a struct or block
//              * it's a block when EMdInputOutput is EMio*Block*, !typeLayout will say how the block is laid out
//              * it's a structure when the EMdTypeLayout is EMtlAggregate
//              * the !gla.io operands are the child members, in order, of the type
//         - for blocks, the interface name is typeName
//
//     !sampler -> { EMdSampler, Value*, EMdSamplerDim, array, shadow, EMdSamplerBaseType }
//     Notes:
//         - EMdSampler says whether it is an image or texture, and if an image what its format is
//         - texel return value has the same type as Value*, modified by EMdSamplerBaseType (e.g., unsigned int)
//         - array is bool, true if it is a samplerArray
//         - shadow is bool, true if it is a samplerShadow
//
//     !typeLayout -> { EMdTypeLayout, EMdPrecision, location, !sampler, interpMode, EMdBuiltIn, binding, EMdQualifierShift mask, offset }
//     Notes:
//         - the EMdTypeLayout is how it is known if something is a matrix or unsigned integer,
//           because this is not in the LLVM type
//         - 'location'
//           - the *first* location of the variable, which can span many slots/locations
//           - is >= MaxUserLayoutLocation for non-user specified locations, to be patched later by a linker
//           - is < MaxUserLayoutLocation for user-assigned locations
//         - the intrinsic's slot can be different when reading a single slot out of the middle of a large input/output
//         - interpMode is present as an integer encoded for MakeInterpolationMode() and CrackInterpolationMode()
//           it can also be present if there is an EMdBuiltIn, and will be -1 if there is no interpMode
//         - EMdBuiltIn, if present, says what built-in variable is being represented.  It is optional.
//         - binding, if present, is the binding
//             - a binding of -1 means the source specified no binding
//         - EMdQualifierShift, if present, mask is a collection of additional qualifiers
//             - a value of 0 means no qualifiers were specified in the source
//         - offset, if present, is an integer offset for the object (as per GLSL)
//             - an offset of -1 means the source specified no offset
//
//     !aggregate -> { name, !typeLayout, list of members: name1, !aggregate1, name2, !aggregate2, ... }
//     Notes:
//         - this recursively represents nested types, paralleling the LLVM type, but complementing it
//           (e.g., an llvm array of array might have EMdTypeLayout of EMtlRowMajorMatrix)
//         - the name in operand 0 is the name of the type (interface name for a block)
//         - each contained member has a pair of operands, one for the member's name, one for the member's type
//
//     When aggregates are used, the starting point is always a !gla.uniform, !gla.output, or !gla.input node, which will
//     supply overall information and then point to a !aggregate for the hierarchical information.
//
//     Blocks can have two names:
//        1) the instance name used in a reference, which could be missing
//        2) the interface name used only in the declaration, which must be present
//     The instance name will be in the !gla.uniform/input/output node, while the interface name will be in the
//     !aggregate node pointed to.
//
// Forms of named metadata
//
//     !gla.inputs = !{ list of all pipeline !input }
//     !gla.outputs = !{ list of all pipeline !output }
//     !gla.uniforms = !{ list of all !uniform }
//     !gla.invariant = !{ subset list of output that were declared as invariant }
//     !gla.entrypoint = !{ list of all entry points }
//     !gla.noStaticUse = !{ subset of input/output/uniform that were not statically referenced in the source shader }
//     !gla.shared = !{ list of all workgroup-shared globals (the Value* of the global variables that are storage-qualified shared) }
//
//     !gla.inputPrimitive     = !{ !M } where !M = !{ i32 EMdLayoutGeometry }
//     !gla.outputPrimitive    = !{ !M } where !M = !{ i32 EMdLayoutGeometry }
//     !gla.xfbMode            = !{ !M } where !M = !{ i32 bool }
//     !gla.numVertices        = !{ !M } where !M = !{ i32 int }
//     !gla.vertexSpacing      = !{ !M } where !M = !{ i32 EMdVertexSpacing }
//     !gla.vertexOrder        = !{ !M } where !M = !{ i32 EMdVertexOrder }
//     !gla.pointMode          = !{ !M } where !M = !{ i32 bool }
//     !gla.invocations        = !{ !M } where !M = !{ i32 int }
//     !gla.pixelCenterInteger = !{ !M } where !M = !{ i32 bool }
//     !gla.originUpperLeft    = !{ !M } where !M = !{ i32 bool }
//     !gla.blendEquations     = !{ !M } where !M = !{ i32 mask of bits shifted by EMdBlendEquationShift amounts }
//     !gla.localSize          = !{ !M } where !M = !{ i32 x-size, int y-size, int z-size }
//

// Operand names
const char* const InputMdName     = "gla.input";
const char* const OutputMdName    = "gla.output";
const char* const UniformMdName   = "gla.uniform";
const char* const PrecisionMdName = "gla.precision";

// Named nodes
const char* const InputListMdName          = "gla.inputs";
const char* const OutputListMdName         = "gla.outputs";
const char* const UniformListMdName        = "gla.uniforms";
const char* const InvariantListMdName      = "gla.invariant";
const char* const EntrypointListMdName     = "gla.entrypoint";
const char* const NoStaticUseMdName        = "gla.noStaticUse";
const char* const WorkgroupSharedMdName    = "gla.shared";              // storage qualifier 'shared'

const char* const InputPrimitiveMdName     = "gla.inputPrimitive";
const char* const OutputPrimitiveMdName    = "gla.outputPrimitive";
const char* const XfbModeMdName            = "gla.xfbMode";
const char* const NumVerticesMdName        = "gla.numVertices";
const char* const VertexSpacingMdName      = "gla.vertexSpacing";
const char* const VertexOrderMdName        = "gla.vertexOrder";
const char* const PointModeMdName          = "gla.pointMode";
const char* const InvocationsMdName        = "gla.invocations";
const char* const PixelCenterIntegerMdName = "gla.pixelCenterInteger";
const char* const OriginUpperLeftMdName    = "gla.originUpperLeft";
const char* const BlendEquationMdName      = "gla.blendEquations";
const char* const LocalSizeMdName          = "gla.localSize";

// what kind of I/O:
enum EMdInputOutput {
    EMioNone,               // for something that is not I/O, or already had its EmdInputOutput in md pointing here

    // inputs
    EMioPipeIn,             // normal user-streamed input data: attributes, varyings, etc.
    EMioVertexId,
    EMioInstanceId,
    EMioVertexIndex,
    EMioInstanceIndex,
    EMioFragmentFace,
    EMioFragmentCoord,
    EMioPointCoord,

    // outputs
    EMioPipeOut,            // normal user-streamed output data, including fragment color
    EMioVertexPosition,
    EMioPointSize,
    EMioClipVertex,
    EMioFragmentDepth,

    // uniforms
    EMioDefaultUniform,      // a uniform variable not in a block
    EMioUniformBlockMember,  // uniform buffer (uniform block)
    EMioBufferBlockMember,   // shader storage buffer object (buffer block), with no run-time sized array, see also EMioBufferBlockMemberArrayed

    // Entry point into shader
    EMioEntrypoint,

    // in & out blocks
    EMioPipeOutBlock,         // output block
    EMioPipeInBlock,          // input block

    // uniforms
    EMioBufferBlockMemberArrayed, // EMioBufferBlockMember but with run-time sized array as the last member

    EMioCount,
};

// How the *interior* of a single, non-aggregate entity is laid out, supplemental to the "Value* for type"
// Also, how a block or structure is laid out, if applied to a block or structure
enum EMdTypeLayout {
    EMtlNone,

    // single-entity layouts
    EMtlUnsigned,           // unsigned integer type, other type information comes from the LLVM value->type
    EMtlRowMajorMatrix,
    EMtlColMajorMatrix,
    EMtlAggregate,
    EMtlSampler,

    // aggregate layouts
    EMtlShared,             // layout-qualifier identifier 'shared'
    EMtlStd140,
    EMtlStd430,
    EMtlPacked,

    // Atomic counter
    EMtlAtomicUint,

    EMtlCount,
};

// What kind of sampler
enum EMdSampler {
    EMsTexture,

    // Image with no format
    EMsImage,

    // Floating-point format image
    EMsRgba32f,
    EMsRgba16f,
    EMsR32f,
    EMsRgba8,
    EMsRgba8Snorm,
    EMsRg32f,
    EMsRg16f,
    EMsR11fG11fB10f,
    EMsR16f,
    EMsRgba16,
    EMsRgb10A2,
    EMsRg16,
    EMsRg8,
    EMsR16,
    EMsR8,
    EMsRgba16Snorm,
    EMsRg16Snorm,
    EMsRg8Snorm,
    EMsR16Snorm,
    EMsR8Snorm,

    // signed-int format image
    EMsRgba32i,
    EMsRgba16i,
    EMsRgba8i,
    EMsR32i,
    EMsRg32i,
    EMsRg16i,
    EMsRg8i,
    EMsR16i,
    EMsR8i,

    // unsigned-int format image
    EMsRgba32ui,
    EMsRgba16ui,
    EMsRgba8ui,
    EMsR32ui,
    EMsRg32ui,
    EMsRg16ui,
    EMsRg8ui,
    EMsR16ui,
    EMsR8ui,

    EMsCount,
};

// Dimensionality of sampler
enum EMdSamplerDim {
    EMsd1D,
    EMsd2D,
    EMsd3D,
    EMsdCube,
    EMsdRect,
    EMsdBuffer,
    EMsdCount,
};

// Return type of sampler
enum EMdSamplerBaseType {
    EMsbFloat,
    EMsbInt,
    EMsbUint,
    EMsbCount,
};

// ESSL precision qualifier
enum EMdPrecision {
    EMpNone,
    EMpLow,
    EMpMedium,
    EMpHigh,
    EMpCount,
};

// For input/output primitives
enum EMdLayoutGeometry {
    EMlgNone,
    EMlgPoints,
    EMlgLines,
    EMlgLinesAdjacency,
    EMlgLineStrip,
    EMlgTriangles,
    EMlgTrianglesAdjacency,
    EMlgTriangleStrip,
    EMlgQuads,
    EMlgIsolines,
    EMlgCount,
};

enum EMdVertexSpacing {
    EMvsNone,
    EMvsEqual,
    EMvsFractionalEven,
    EMvsFractionalOdd,
    EMvsCount,
};

enum EMdVertexOrder {
    EMvoNone,
    EMvoCw,
    EMvoCcw,
    EMvoCount,
};

enum EMdBuiltIn {
    EmbNone,
    EmbNumWorkGroups,
    EmbWorkGroupSize,
    EmbWorkGroupId,
    EmbLocalInvocationId,
    EmbGlobalInvocationId,
    EmbLocalInvocationIndex,
    EmbVertexId,
    EmbInstanceId,
    EmbVertexIndex,
    EmbInstanceIndex,
    EmbPosition,
    EmbPointSize,
    EmbClipVertex,
    EmbClipDistance,
    EmbCullDistance,
    EmbNormal,
    EmbVertex,
    EmbMultiTexCoord0,
    EmbMultiTexCoord1,
    EmbMultiTexCoord2,
    EmbMultiTexCoord3,
    EmbMultiTexCoord4,
    EmbMultiTexCoord5,
    EmbMultiTexCoord6,
    EmbMultiTexCoord7,
    EmbFrontColor,
    EmbBackColor,
    EmbFrontSecondaryColor,
    EmbBackSecondaryColor,
    EmbTexCoord,
    EmbFogFragCoord,
    EmbInvocationId,
    EmbPrimitiveId,
    EmbLayer,
    EmbViewportIndex,
    EmbPatchVertices,
    EmbTessLevelOuter,
    EmbTessLevelInner,
    EmbTessCoord,
    EmbColor,
    EmbSecondaryColor,
    EmbFace,
    EmbFragCoord,
    EmbPointCoord,
    EmbFragColor,
    EmbFragData,
    EmbFragDepth,
    EmbSampleId,
    EmbSamplePosition,
    EmbSampleMask,
    EmbHelperInvocation,
    EmbBoundingBox,
    EmbCount
};

// These are bit-shift amounts for various additional qualifiers.
enum EMdQualifierShift {
    EmqNonreadable,
    EmqNonwritable,
    EmqVolatile,
    EmqRestrict,
    EmqCoherent,

    EmqCount
};

enum EMdBlendEquationShift {
    // No 'EMeNone':
    // These are used as bit-shift amounts.  A mask of such shifts will have type 'int',
    // and in that space, 0 means no bits set, or none.  In this enum, 0 means (1 << 0), a bit is set.
    EmeMultiply,
    EmeScreen,
    EmeOverlay,
    EmeDarken,
    EmeLighten,
    EmeColordodge,
    EmeColorburn,
    EmeHardlight,
    EmeSoftlight,
    EmeDifference,
    EmeExclusion,
    EmeHslHue,
    EmeHslSaturation,
    EmeHslColor,
    EmeHslLuminosity,
    EmeAllEquations,

    EmeCount
};

//
// Crackers are for the consumer of the IR.
// They take an MD node, or instruction that might point to one, and decode it, as per the enums above.
//

inline bool CrackTypeLayout(const llvm::MDNode* md, EMdTypeLayout& layout, EMdPrecision& precision, int& location, const llvm::MDNode*& sampler, int& interpMode, EMdBuiltIn& builtIn)
{
    const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(0));
    if (! constInt)
        return false;
    layout = (EMdTypeLayout)constInt->getSExtValue();

    constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(1));
    if (! constInt)
        return false;
    precision = (EMdPrecision)constInt->getSExtValue();

    constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(2));
    if (! constInt)
        return false;
    location = (int)constInt->getSExtValue();

    llvm::Value* speculativeSampler = md->getOperand(3);
    if (speculativeSampler)
        sampler = llvm::dyn_cast<llvm::MDNode>(speculativeSampler);
    else
        sampler = 0;

    if (md->getNumOperands() >= 5) {
        const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(4));
        if (constInt)
            interpMode = (int)constInt->getZExtValue();
    } else
        interpMode = 0;

    if (md->getNumOperands() >= 6) {
        const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(5));
        if (constInt)
            builtIn = (EMdBuiltIn)constInt->getZExtValue();
    } else
        builtIn = EmbNone;

    return true;
}

inline bool CrackTypeLayout(const llvm::MDNode* md, EMdTypeLayout& layout, EMdPrecision& precision, int& location, const llvm::MDNode*& sampler, int& interpMode, EMdBuiltIn& builtIn,
                            int& binding, unsigned int& qualifiers, int& offset)
{
    CrackTypeLayout(md, layout, precision, location, sampler, interpMode, builtIn);
    if (md->getNumOperands() >= 7) {
        const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(6));
        if (constInt)
            binding = (int)constInt->getSExtValue();
    } else
        binding = -1;

    if (md->getNumOperands() >= 8) {
        const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(7));
        if (constInt)
            qualifiers = (unsigned int)constInt->getZExtValue();
    } else
        qualifiers = 0;

    if (md->getNumOperands() >= 9) {
        const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(8));
        if (constInt)
            offset = (int)constInt->getSExtValue();
    } else
        offset = 0;

    return true;
}

inline bool CrackIOMdType(const llvm::MDNode* md, llvm::Type*& type)
{
    if (! md->getOperand(2))
        return false;
    type = md->getOperand(2)->getType();

    return true;
}

inline bool CrackIOMd(const llvm::MDNode* md, std::string& symbolName, EMdInputOutput& qualifier, llvm::Type*& type,
                      EMdTypeLayout& layout, EMdPrecision& precision, int& location, const llvm::MDNode*& sampler, const llvm::MDNode*& aggregate,
                      int& interpMode, EMdBuiltIn& builtIn)
{
    symbolName = md->getOperand(0)->getName();

    const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(1));
    if (! constInt)
        return false;
    qualifier = (EMdInputOutput)constInt->getSExtValue();

    if (! md->getOperand(2))
        return false;
    type = md->getOperand(2)->getType();

    llvm::MDNode* layoutMd = llvm::dyn_cast<llvm::MDNode>(md->getOperand(3));
    if (! layoutMd)
        return false;
    if (! CrackTypeLayout(layoutMd, layout, precision, location, sampler, interpMode, builtIn))
        return false;

    if (md->getNumOperands() >= 5)
        aggregate = llvm::dyn_cast<llvm::MDNode>(md->getOperand(4));
    else
        aggregate = 0;

    return true;
}

inline bool CrackIOMd(const llvm::MDNode* md, std::string& symbolName, EMdInputOutput& qualifier, llvm::Type*& type,
                      EMdTypeLayout& layout, EMdPrecision& precision, int& location, const llvm::MDNode*& sampler, const llvm::MDNode*& aggregate,
                      int& interpMode, EMdBuiltIn& builtIn, int& binding, unsigned int& qualifiers, int& offset)
{
    symbolName = md->getOperand(0)->getName();

    const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(1));
    if (! constInt)
        return false;
    qualifier = (EMdInputOutput)constInt->getSExtValue();

    if (! md->getOperand(2))
        return false;
    type = md->getOperand(2)->getType();

    llvm::MDNode* layoutMd = llvm::dyn_cast<llvm::MDNode>(md->getOperand(3));
    if (! layoutMd)
        return false;
    if (! CrackTypeLayout(layoutMd, layout, precision, location, sampler, interpMode, builtIn, binding, qualifiers, offset))
        return false;

    if (md->getNumOperands() >= 5)
        aggregate = llvm::dyn_cast<llvm::MDNode>(md->getOperand(4));
    else
        aggregate = 0;

    return true;
}

inline bool CrackAggregateMd(const llvm::MDNode* md, std::string& symbolName,
                             EMdTypeLayout& layout, EMdPrecision& precision, int& location, const llvm::MDNode*& sampler, EMdBuiltIn& builtIn)
{
    symbolName = md->getOperand(0)->getName();
    int dummyInterpMode;

    llvm::MDNode* layoutMd = llvm::dyn_cast<llvm::MDNode>(md->getOperand(1));
    if (! layoutMd)
        return false;
    if (! CrackTypeLayout(layoutMd, layout, precision, location, sampler, dummyInterpMode, builtIn))
        return false;

    return true;
}

inline bool CrackAggregateMd(const llvm::MDNode* md, std::string& symbolName,
                             EMdTypeLayout& layout, EMdPrecision& precision, int& location, const llvm::MDNode*& sampler, EMdBuiltIn& builtIn,
                             int& binding, unsigned int& qualifiers, int& offset)
{
    symbolName = md->getOperand(0)->getName();
    int dummyInterpMode;

    llvm::MDNode* layoutMd = llvm::dyn_cast<llvm::MDNode>(md->getOperand(1));
    if (! layoutMd)
        return false;
    if (! CrackTypeLayout(layoutMd, layout, precision, location, sampler, dummyInterpMode, builtIn, binding, qualifiers, offset))
        return false;

    return true;
}

inline bool CrackIOMd(const llvm::Instruction* instruction, llvm::StringRef kind, std::string& symbolName, EMdInputOutput& qualifier, llvm::Type*& type,
                      EMdTypeLayout& layout, EMdPrecision& precision, int& location, const llvm::MDNode*& sampler, const llvm::MDNode*& aggregate,
                      int& interpMode, EMdBuiltIn& builtIn)
{
    const llvm::MDNode* md = instruction->getMetadata(kind);
    if (! md)
        return false;

    return CrackIOMd(md, symbolName, qualifier, type, layout, precision, location, sampler, aggregate, interpMode, builtIn);
}

inline bool CrackIOMd(const llvm::Instruction* instruction, llvm::StringRef kind, std::string& symbolName, EMdInputOutput& qualifier, llvm::Type*& type,
                      EMdTypeLayout& layout, EMdPrecision& precision, int& location, const llvm::MDNode*& sampler, const llvm::MDNode*& aggregate,
                      int& interpMode, EMdBuiltIn& builtIn, int& binding, unsigned int& qualifiers, int& offset)
{
    const llvm::MDNode* md = instruction->getMetadata(kind);
    if (! md)
        return false;

    return CrackIOMd(md, symbolName, qualifier, type, layout, precision, location, sampler, aggregate, interpMode, builtIn, binding, qualifiers, offset);
}

inline bool CrackSamplerMd(const llvm::MDNode* md, EMdSampler& sampler, llvm::Type*& type, EMdSamplerDim& dim, bool& isArray, bool& isShadow, EMdSamplerBaseType& baseType)
{
    const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(0));
    if (! constInt)
        return false;
    sampler = (EMdSampler)constInt->getSExtValue();

    if (! md->getOperand(1))
        return false;
    type = md->getOperand(1)->getType();

    constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(2));
    if (! constInt)
        return false;
    dim = (EMdSamplerDim)constInt->getSExtValue();

    constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(3));
    if (! constInt)
        return false;
    isArray = constInt->getSExtValue() != 0;

    constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(4));
    if (! constInt)
        return false;
    isShadow = constInt->getSExtValue() != 0;

    constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(5));
    if (! constInt)
        return false;
    baseType = (EMdSamplerBaseType)constInt->getSExtValue();

    return true;
}

inline bool CrackPrecisionMd(const llvm::Instruction* instruction, EMdPrecision& precision)
{
    precision = EMpNone;

    const llvm::MDNode* md = instruction->getMetadata(PrecisionMdName);
    if (! md)
        return false;

    const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(0));
    if (! constInt)
        return false;
    precision = (EMdPrecision)constInt->getSExtValue();

    return true;
}

// translate from 0-based counting to aggregate operand number
inline int GetAggregateMdNameOp(int index)
{
    return 2 + 2 * index;
}

inline int GetAggregateMdSubAggregateOp(int index)
{
    return 2 + 2 * index + 1;
}

// Just a short cut for when only the type is cared about, rather than cracking all the operands.
// It looks for both forms of whether this is a deferenced aggregate or a top-level interface md.
inline EMdTypeLayout GetMdTypeLayout(const llvm::MDNode *md)
{
    if (! md)
        return EMtlNone;

    // check for aggregate member form first
    llvm::MDNode* layoutMd = llvm::dyn_cast<llvm::MDNode>(md->getOperand(1));
    if (! layoutMd) {
        // check for top-level interface
        layoutMd = llvm::dyn_cast<llvm::MDNode>(md->getOperand(3));
        if (! layoutMd)
            return EMtlNone;
    }

    const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(layoutMd->getOperand(0));
    if (! constInt)
        return EMtlNone;

    return (EMdTypeLayout)constInt->getSExtValue();
}

// A shortcut for just getting the type of a sampler (int, uint, float) from an
// instruction's metadata
inline EMdSamplerBaseType GetMdSamplerBaseType(const llvm::MDNode* md)
{
    if (! md || md->getNumOperands() < 4)
        return EMsbFloat;

    md = llvm::dyn_cast<llvm::MDNode>(md->getOperand(3));
    if (! md || md->getNumOperands() < 4 || md->getOperand(3) == 0)
        return EMsbFloat;

    md = llvm::dyn_cast<llvm::MDNode>(md->getOperand(3));
    if (! md || md->getNumOperands() < 6 || md->getOperand(5) == 0)
        return EMsbFloat;

    const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(5));
    if (! constInt)
        return EMsbFloat;

    return (EMdSamplerBaseType)constInt->getSExtValue();
}

// Return the value of the integer metadata operand to the named metadata node.
// Return 0 if the named node is missing, or was there and it's metadata node was 0 or false.
inline int GetMdNamedInt(llvm::Module& module, const char* name)
{
    llvm::NamedMDNode* namedNode = module.getNamedMetadata(name);
    if (namedNode == nullptr)
        return 0;
    const llvm::MDNode* md = namedNode->getOperand(0);
    const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(0));
    if (! constInt)
        return 0;

    return (int)constInt->getSExtValue();
}

// Build the set of values of the integer metadata operand to the named metadata node.
// Return false if the named node is missing, or was there and it's metadata node was 0 or false.
inline bool GetMdNamedInts(llvm::Module& module, const char* name, std::vector<int>& ints)
{
    llvm::NamedMDNode* namedNode = module.getNamedMetadata(name);
    if (namedNode == nullptr)
        return false;
    const llvm::MDNode* md = namedNode->getOperand(0);
    if (md == nullptr)
        return false;
    for (unsigned int op = 0; op < md->getNumOperands(); ++op) {
        const llvm::ConstantInt* constInt = llvm::dyn_cast<llvm::ConstantInt>(md->getOperand(op));
        if (! constInt)
            return false;
        ints.push_back((int)constInt->getSExtValue());
    }

    return true;
}

//
// Metadata class is just for adapter while building the IR.
//
class Metadata {
public:
    Metadata(llvm::LLVMContext& c, llvm::Module* m) : context(c), module(m)
    {
        // cache the precision qualifier nodes, there are only 4 to reuse
        for (int i = 0; i < EMpCount; ++i) {
            llvm::Value* args[] = {
                gla::MakeIntConstant(context, i),
            };
            precisionMd[i] = llvm::MDNode::get(context, args);
        }
    }

    // "!gla.input/output/uniform ->" as per comment at top of file
    llvm::MDNode* makeMdInputOutput(llvm::StringRef symbolName, llvm::StringRef sectionName, EMdInputOutput qualifier,
                                    llvm::Value* typeProxy, EMdTypeLayout layout, EMdPrecision precision, int location,
                                    llvm::MDNode* sampler = 0, llvm::MDNode* aggregate = 0, int interpMode = -1, EMdBuiltIn builtIn = EmbNone,
                                    int binding = -1, int qualifiers = 0, int offset = -1)
    {
        llvm::MDNode* layoutMd = makeMdTypeLayout(layout, precision, location, sampler, interpMode, builtIn, binding, qualifiers, offset);

        llvm::MDNode* md;
        if (aggregate) {
            llvm::Value* args[] = {
                llvm::MDString::get(context, symbolName),
                gla::MakeIntConstant(context, qualifier),
                typeProxy,
                layoutMd,
                aggregate
            };
            md = llvm::MDNode::get(context, args);
        } else  {
            llvm::Value* args[] = {
                llvm::MDString::get(context, symbolName),
                gla::MakeIntConstant(context, qualifier),
                typeProxy,
                layoutMd
            };
            md = llvm::MDNode::get(context, args);
        }

        llvm::NamedMDNode* namedNode = module->getOrInsertNamedMetadata(sectionName);
        namedNode->addOperand(md);

        return md;
    }

    // "!gla.input/output/uniform ->" as per comment at top of file, for Single-Walk form
    llvm::MDNode* makeMdSingleTypeIo(llvm::StringRef symbolName, const char* typeName, EMdInputOutput qualifier,
                                     llvm::Value* typeProxy, llvm::MDNode* layoutMd, llvm::ArrayRef<llvm::MDNode*> members)
    {
        llvm::MDNode* md;
        llvm::SmallVector<llvm::Value*, 10> args;
        args.push_back(llvm::MDString::get(context, symbolName));
        args.push_back(gla::MakeIntConstant(context, qualifier));
        args.push_back(typeProxy);
        args.push_back(layoutMd);
        args.push_back(llvm::MDString::get(context, typeName));
        if (members.size() > 0) {
            args.append(members.begin(), members.end());
        }
        md = llvm::MDNode::get(context, args);

        return md;
    }

    // "!sampler ->" as per comment at top of file
    llvm::MDNode* makeMdSampler(EMdSampler sampler, llvm::Value* typeProxy, EMdSamplerDim dim, bool isArray, bool isShadow, EMdSamplerBaseType baseType)
    {
        llvm::Value* args[] = {
            gla::MakeIntConstant(context, sampler),
            typeProxy,
            gla::MakeIntConstant(context, dim),
            gla::MakeBoolConstant(context, isArray),
            gla::MakeBoolConstant(context, isShadow),
            gla::MakeIntConstant(context, baseType),
        };
        llvm::MDNode* md = llvm::MDNode::get(context, args);

        return md;
    }

    llvm::MDNode* makeMdPrecision(EMdPrecision precision)
    {
        // just use our precision cache

        return precisionMd[precision];
    }

    llvm::MDNode* makeMdTypeLayout(EMdTypeLayout layout, EMdPrecision precision, int location, llvm::MDNode* sampler, int interpMode = -1, EMdBuiltIn builtIn = EmbNone,
                                   int binding = -1, unsigned int qualifiers = 0, int offset = -1)
    {
        llvm::Value* args[] = {
            gla::MakeIntConstant(context, layout),
            gla::MakeIntConstant(context, precision),
            gla::MakeIntConstant(context, location),
            sampler,
            gla::MakeIntConstant(context, interpMode),
            gla::MakeIntConstant(context, builtIn),
            gla::MakeIntConstant(context, binding),
            gla::MakeUnsignedConstant(context, qualifiers),
            gla::MakeIntConstant(context, offset),
        };

        return llvm::MDNode::get(context, args);
    }

    void addMdEntrypoint(const char* name)
    {
        llvm::MDNode* md;
        llvm::Value* args[] = {
            llvm::MDString::get(context, name),
            gla::MakeIntConstant(context, EMioEntrypoint),
        };
        md = llvm::MDNode::get(context, args);

        llvm::NamedMDNode* namedNode = module->getOrInsertNamedMetadata(EntrypointListMdName);
        namedNode->addOperand(md);
    }

    void addNoStaticUse(llvm::MDNode* md)
    {
        llvm::NamedMDNode* namedNode = module->getOrInsertNamedMetadata(NoStaticUseMdName);
        namedNode->addOperand(md);
    }

    void addShared(llvm::Value* shared)
    {
        llvm::Value* args[] = {
            shared
        };
        llvm::MDNode* md = llvm::MDNode::get(context, args);
        llvm::NamedMDNode* namedNode = module->getOrInsertNamedMetadata(WorkgroupSharedMdName);
        namedNode->addOperand(md);
    }

    // Add a named metadata node of the form:
    //     !gla.name        = !{ int }
    // Where 'int' could also be considered a bool or enum.  See comment at top for long list
    // of examples where this can be used.
    void makeMdNamedInt(const char* name, int i)
    {
        llvm::NamedMDNode* namedNode = module->getOrInsertNamedMetadata(name);
        llvm::Value* layoutArgs[] = { gla::MakeIntConstant(context, i) };
        namedNode->addOperand(llvm::MDNode::get(context, layoutArgs));
    }

    void makeMdNamedInt(const char* name, int i1, int i2, int i3)
    {
        llvm::NamedMDNode* namedNode = module->getOrInsertNamedMetadata(name);
        llvm::Value* layoutArgs[] = { gla::MakeIntConstant(context, i1),
                                      gla::MakeIntConstant(context, i2),
                                      gla::MakeIntConstant(context, i3), };
        namedNode->addOperand(llvm::MDNode::get(context, layoutArgs));
    }

protected:
    llvm::LLVMContext& context;
    llvm::Module* module;
    llvm::MDNode* precisionMd[EMpCount];
};

};

#endif // metadata_H
