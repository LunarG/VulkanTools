//
//Copyright (C) 2014 LunarG, Inc.
//
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//    Neither the name of 3Dlabs Inc. Ltd. nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//POSSIBILITY OF SUCH DAMAGE.

//
// Author: John Kessenich, LunarG
//

//
// Translate SPIR-V to Top IR.
//

#define _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#define snprintf _snprintf
#endif

// LLVM includes
#pragma warning(push, 1)
#include "llvm/Support/CFG.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/SmallVector.h"
#pragma warning(pop)

#include <cstdio>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <stack>
#include <list>

// Glslang includes
#include "SPIRV/spirv.hpp"
namespace spv {
    #include "SPIRV/GLSL.std.450.h"
}

// LunarGLASS includes
#include "LunarGLASSTopIR.h"
#include "LunarGLASSManager.h"
#include "Exceptions.h"
#include "TopBuilder.h"
#include "metadata.h"
#include "Util.h"

// Adapter includes
#include "SpvToTop.h"

// Glslang includes, just because the GLSL backend reuses some enums for stage and version
#include "glslang/Public/ShaderLang.h"
#include "glslang/MachineIndependent/Versions.h"

namespace {

const unsigned int BadValue = 0xFFFFFFFF;

spv::Op GetOpCode(unsigned int word)
{
    return (spv::Op)(word & spv::OpCodeMask);
}

int GetWordCount(unsigned int word)
{
    return (int)(word >> spv::WordCountShift);
}

gla::EMdBuiltIn GetMdBuiltIn(spv::BuiltIn builtIn)
{
    switch (builtIn) {
    case spv::BuiltInNumWorkgroups:        return gla::EmbNumWorkGroups;
    case spv::BuiltInWorkgroupSize:        return gla::EmbWorkGroupSize;
    case spv::BuiltInWorkgroupId:          return gla::EmbWorkGroupId;
    case spv::BuiltInLocalInvocationId:    return gla::EmbLocalInvocationId;
    case spv::BuiltInGlobalInvocationId:   return gla::EmbGlobalInvocationId;
    case spv::BuiltInLocalInvocationIndex: return gla::EmbLocalInvocationIndex;
    case spv::BuiltInVertexId:             return gla::EmbVertexId;
    case spv::BuiltInVertexIndex:          return gla::EmbVertexIndex;
    case spv::BuiltInInstanceId:           return gla::EmbInstanceId;
    case spv::BuiltInPosition:             return gla::EmbPosition;
    case spv::BuiltInPointSize:            return gla::EmbPointSize;
    case spv::BuiltInClipDistance:         return gla::EmbClipDistance;
    case spv::BuiltInCullDistance:         return gla::EmbCullDistance;
    case spv::BuiltInInvocationId:         return gla::EmbInvocationId;
    case spv::BuiltInPrimitiveId:          return gla::EmbPrimitiveId;
    case spv::BuiltInLayer:                return gla::EmbLayer;
    case spv::BuiltInViewportIndex:        return gla::EmbViewportIndex;
    case spv::BuiltInPatchVertices:        return gla::EmbPatchVertices;
    case spv::BuiltInTessLevelOuter:       return gla::EmbTessLevelOuter;
    case spv::BuiltInTessLevelInner:       return gla::EmbTessLevelInner;
    case spv::BuiltInTessCoord:            return gla::EmbTessCoord;
    case spv::BuiltInFrontFacing:          return gla::EmbFace;
    case spv::BuiltInFragCoord:            return gla::EmbFragCoord;
    case spv::BuiltInPointCoord:           return gla::EmbPointCoord;
    case spv::BuiltInFragDepth:            return gla::EmbFragDepth;
    case spv::BuiltInSampleId:             return gla::EmbSampleId;
    case spv::BuiltInSamplePosition:       return gla::EmbSamplePosition;
    case spv::BuiltInSampleMask:           return gla::EmbSampleMask;
    case spv::BuiltInHelperInvocation:     return gla::EmbHelperInvocation;
    default:
        gla::UnsupportedFunctionality("built in variable", gla::EATContinue);
        return gla::EmbNone;
    }
}

const char* NonNullName(const char* name)
{
    return name ? name : "";
}

//
// Translator instance for translating a SPIR-V stream to LunarGLASS's LLVM-based Top IR.
//
class SpvToTopTranslator {
public:
    SpvToTopTranslator(const std::vector<unsigned int>& spirv, gla::Manager& manager);
    virtual ~SpvToTopTranslator();

    void makeTop();

protected:
    // a bag to hold type information that's lost going to LLVM (without metadata)
    struct MetaType {
        MetaType() : layout(gla::EMtlNone), combinedImageSampler(false), precision(gla::EMpNone), builtIn(gla::EmbNone), set(-1), binding(-1), location(gla::MaxUserLayoutLocation),
                     interpolationMethod((spv::Decoration)BadValue), interpolateTo((spv::Decoration)BadValue),
                     invariant(false), name(0) { }

        // set of things needed for !typeLayout metadata node
        gla::EMdTypeLayout layout;              // includes matrix information
        bool combinedImageSampler;
        gla::EMdPrecision precision;
        gla::EMdBuiltIn builtIn;
        short set;
        short matrixStride;
        int arrayStride;
        int binding;
        int location;
        spv::Decoration interpolationMethod;
        spv::Decoration interpolateTo;

        // metadata indicated in some other way
        bool invariant;
        const char* name;
        static const int bufSize = 12;
        char buf[bufSize];
    };
    void bumpMemberMetaData(std::vector<MetaType>*& memberMetaData, int member);

    // SPIR-V instruction processors
    void setEntryPoint(spv::ExecutionModel, spv::Id entryId);
    void setExecutionMode(spv::Id entryPoint, spv::ExecutionMode);
    void addDecoration(spv::Id, spv::Decoration);
    void addMemberDecoration(spv::Id, unsigned int member, spv::Decoration);
    void addMetaTypeDecoration(spv::Decoration decoration, MetaType& metaType);
    void addType(spv::Op, spv::Id, int numOperands);
    void addVariable(spv::Id resultId, spv::Id typeId, spv::StorageClass);
    gla::Builder::EStorageQualifier mapStorageClass(spv::StorageClass, bool isBuffer);
    void addConstant(spv::Op, spv::Id resultId, spv::Id typeId, int numOperands);
    void addConstantAggregate(spv::Id resultId, spv::Id typeId, int numOperands);
    int assignSlot(spv::Id resultId, int& numSlots);
    void decodeResult(bool type, spv::Id& typeId, spv::Id& resultId);
    const char* findAName(spv::Id choice1, spv::Id choice2 = 0);
    void translateInstruction(spv::Op opCode, int numOperands);
    llvm::Value* createUnaryOperation(spv::Op, gla::EMdPrecision, llvm::Type* resultType, llvm::Value* operand, bool hasSign, bool reduceComparison);
    llvm::Value* createBinaryOperation(spv::Op, gla::EMdPrecision, llvm::Value* left, llvm::Value* right, bool hasSign, bool reduceComparison, const char* name = 0);
    gla::ESamplerType convertImageType(spv::Id imageTypeId);
    llvm::Value* createSamplingCall(spv::Op, spv::Id type, spv::Id result, int numOperands);
    llvm::Value* createTextureQueryCall(spv::Op, spv::Id type, spv::Id result, int numOperands);
    llvm::Value* createConstructor(spv::Id resultId, spv::Id typeId, std::vector<llvm::Value*> constituents);
    void handleOpFunction(spv::Id& typeId, spv::Id& resultId);
    llvm::Function* makeFunction(spv::Id functionId, spv::Id returnTypeId, std::vector<spv::Id>& argTypeIds);
    spv::Id getNextLabelId();
    llvm::Value* createFunctionCall(gla::EMdPrecision precision, spv::Id typeId, spv::Id resultId, int numOperands);
    llvm::Value* createExternalInstruction(gla::EMdPrecision precision, spv::Id typeId, spv::Id resultId, int numOperands, const char* name);
    spv::Op getOpCode(spv::Id) const;
    spv::Id dereferenceTypeId(spv::Id typeId) const;
    spv::Id getArrayElementTypeId(spv::Id typeId) const;
    spv::Id getStructMemberTypeId(spv::Id typeId, int member) const;
    spv::Id getImageTypeId(spv::Id sampledImageTypeId) const;
    spv::Id getImageSampledType(spv::Id typeId) const;
    spv::Dim getImageDim(spv::Id typeId) const;
    bool isImageArrayed(spv::Id typeId) const;
    bool isImageDepth(spv::Id typeId) const;
    bool isImageMS(spv::Id typeId) const;

    bool inEntryPoint();
    bool isMatrix(spv::Id typeId) { return commonMap[typeId].metaType.layout == gla::EMtlColMajorMatrix || 
                                           commonMap[typeId].metaType.layout == gla::EMtlRowMajorMatrix; }
    void makeLabelBlock(spv::Id labelId);
    void createSwitch(int numOperands);

    // metadata parameter translators
    gla::EMdInputOutput     getMdQualifier      (spv::Id resultId) const;
    gla::EMdSampler         getMdSampler        (spv::Id typeId)   const;
    gla::EMdSamplerDim      getMdSamplerDim     (spv::Id typeId)   const;
    gla::EMdSamplerBaseType getMdSamplerBaseType(spv::Id typeId)   const;
    void getInterpolationLocationMethod(spv::Id id, gla::EInterpolationMethod& method, gla::EInterpolationLocation& location);

    // metadata creators

    // bias set by 1, so that we can simultaneously
    //  - tell the difference between "set=0" and nothing having been said, and
    //  - not be using the upper bits at all for all the common cases where there is no set
    int packSetBinding(MetaType& metaType) { return ((metaType.set + 1) << 16) | metaType.binding; }

    llvm::Value* makePermanentTypeProxy(llvm::Value*);
    llvm::MDNode* declareUniformMetadata(spv::Id resultId);
    llvm::MDNode* declareMdDefaultUniform(spv::Id resultId);
    llvm::MDNode* makeMdSampler(spv::Id typeId, llvm::Value*);
    llvm::MDNode* declareMdUniformBlock(gla::EMdInputOutput, spv::Id resultId);
    llvm::MDNode* declareMdType(spv::Id typeId, MetaType&);
    llvm::MDNode* makeInputOutputMetadata(spv::Id resultId, int slot, const char* kind);
    void makeOutputMetadata(spv::Id resultId, int slot, int numSlots);
    llvm::MDNode* makeInputMetadata(spv::Id resultId, int slot);

    // class data
    const std::vector<unsigned int>& spirv; // the SPIR-V stream of words
    int word;                               // next word to read from spirv
    int nextInst;                           // beginning of the next instruction
    gla::Manager& manager;                  // LunarGLASS manager
    llvm::LLVMContext &context;
    llvm::BasicBlock* shaderEntry;
    llvm::IRBuilder<> llvmBuilder;
    llvm::Module* module;
    gla::Metadata metadata;
    gla::Builder* glaBuilder;
    int version;
    int generator;
    spv::ExecutionModel currentModel;        // ...this is the 'one' currently being operated on
    spv::Id currentFunction;                 // function we are currently inside of
    llvm::Function::arg_iterator currentArg;   // the current argument for processing the function declaration
    int nextSlot;

    // map each <id> to the set of things commonly needed
    // TODO: optimize space down to what really is commonly needed
    unsigned int numIds;
    struct CommonAnnotations {
        CommonAnnotations() : instructionIndex(0), typeId(0), value(0),
                              isBlock(false), isBuffer(false), storageClass((spv::StorageClass)BadValue),
                              entryPoint((spv::ExecutionModel)BadValue), memberMetaData(0) { }
        int instructionIndex;                   // the location in the spirv of this instruction
        union {
            spv::Id typeId;                     // typeId is valid if indexed with a resultId
            llvm::Type* type;                   // type is valid if indexed with a typeId
        };
        union {
            llvm::Value* value;                 // for things using a value
            llvm::Function* function;           // for function id
            llvm::BasicBlock* block;            // for label id
        };
        MetaType metaType;
        bool isBlock;
        bool isBuffer;        // SSBO
        spv::StorageClass storageClass;
        spv::ExecutionModel entryPoint;
        std::vector<MetaType> *memberMetaData;
    };
    std::vector<CommonAnnotations> commonMap;
};

SpvToTopTranslator::SpvToTopTranslator(const std::vector<unsigned int>& spirv, gla::Manager& manager)
    : spirv(spirv), word(0),
      manager(manager), context(manager.getModule()->getContext()),
      shaderEntry(0), llvmBuilder(context),
      module(manager.getModule()), metadata(context, module),
      version(0), generator(0), currentModel((spv::ExecutionModel)BadValue), currentFunction(0),
      nextSlot(gla::MaxUserLayoutLocation)
{
    glaBuilder = new gla::Builder(llvmBuilder, &manager, metadata);
    glaBuilder->setNoPredecessorBlocks(false);
    glaBuilder->clearAccessChain();
    glaBuilder->setAccessChainDirectionRightToLeft(false);
}

SpvToTopTranslator::~SpvToTopTranslator()
{
}

// Make sure the memberMetaData is big enough to hold 0-based 'member'
void SpvToTopTranslator::bumpMemberMetaData(std::vector<MetaType>*& memberMetaData, int member)
{
    // Specification issue: it would be much better to know up front the number of members to decorate
    if (memberMetaData == 0)
        memberMetaData = new std::vector<MetaType>;
    if ((int)memberMetaData->size() < member + 1)
        memberMetaData->resize(member + 1);
}

//
// The top-level algorithm for translating SPIR-V to the LLVM-based Top IR.
//
// Goes sequentially through the entire SPIR-V module, one instruction at a time,
// handing out the work to helper methods.
//
// The result <id> and type <id> of each instruction are decoded here, as well
// as the number of operands.  But the operands themselves are decoded by the
// called methods.
//
void SpvToTopTranslator::makeTop()
{
    int size = (int)spirv.size();

    // Sanity check size
    if (size < 5)
        gla::UnsupportedFunctionality("SPIR-V is too short");

    // Magic number
    if (spirv[word++] != spv::MagicNumber)
        gla::UnsupportedFunctionality("Bad magic number for SPIR-V");

    version = spirv[word++];
    generator = spirv[word++];

    numIds = spirv[word++];
    commonMap.resize(numIds);

    if (spirv[word++] != 0)
        gla::UnsupportedFunctionality("Non-0 schema");

    // Walk the instructions
    while (word < size) {
        // First word
        unsigned int instructionStart = word;
        unsigned int firstWord = spirv[word];

        // OpCode
        spv::Op opCode = GetOpCode(firstWord);

        // Instruction size
        int instrSize = (int)(firstWord >> 16);
        nextInst = word + instrSize;

        word++;
        if (nextInst > size)
            gla::UnsupportedFunctionality("SPIR-V instruction terminated too early");

        // Hand off each instruction
        translateInstruction(opCode, instrSize - 1);

        // go to the next instruction, in a way stable WRT incorrect operand parsing
        word = nextInst;
    }
}

// Set an entry point for a model.
// Note:  currently only one entry point is supported.
void SpvToTopTranslator::setEntryPoint(spv::ExecutionModel model, spv::Id entryId)
{
    commonMap[entryId].entryPoint = model;
    if (currentModel != BadValue)
        gla::UnsupportedFunctionality("Multiple execution models in the same SPIR-V module (setting entry point)");
    currentModel = model;

    // Lock ourselves into a single model for now...
    switch (currentModel) {
    case spv::ExecutionModelVertex:                  manager.setStage(EShLangVertex);         break;
    case spv::ExecutionModelTessellationControl:     manager.setStage(EShLangTessControl);    break;
    case spv::ExecutionModelTessellationEvaluation:  manager.setStage(EShLangTessEvaluation); break;
    case spv::ExecutionModelGeometry:                manager.setStage(EShLangGeometry);       break;
    case spv::ExecutionModelFragment:                manager.setStage(EShLangFragment);       break;
    case spv::ExecutionModelGLCompute:               manager.setStage(EShLangCompute);        break;
    default:
        gla::UnsupportedFunctionality("SPIR-V execution model", model, gla::EATAbort);
    }
}

// Process an OpExecutionMode instruction.
// On entry, 'word' is the index of the first operand.
void SpvToTopTranslator::setExecutionMode(spv::Id entryPoint, spv::ExecutionMode mode)
{
    spv::ExecutionModel model = commonMap[entryPoint].entryPoint;
    assert(model != BadValue);

    if (currentModel != model)
        gla::UnsupportedFunctionality("Multiple execution models in the same SPIR-V module (setting input mode)", gla::EATContinue);

    switch (mode) {
    case spv::ExecutionModeInvocations:
        metadata.makeMdNamedInt(gla::InvocationsMdName, spirv[word++]);
        break;
    case spv::ExecutionModeSpacingEqual:
        metadata.makeMdNamedInt(gla::VertexSpacingMdName, gla::EMvsEqual);
        break;
    case spv::ExecutionModeSpacingFractionalEven:
        metadata.makeMdNamedInt(gla::VertexSpacingMdName, gla::EMvsFractionalEven);
        break;
    case spv::ExecutionModeSpacingFractionalOdd:
        metadata.makeMdNamedInt(gla::VertexSpacingMdName, gla::EMvsFractionalOdd);
        break;
    case spv::ExecutionModeVertexOrderCw:
        metadata.makeMdNamedInt(gla::VertexOrderMdName, gla::EMvoCw);
        break;
    case spv::ExecutionModeVertexOrderCcw:
        metadata.makeMdNamedInt(gla::VertexOrderMdName, gla::EMvoCcw);
        break;
    case spv::ExecutionModePixelCenterInteger:
        metadata.makeMdNamedInt(gla::PixelCenterIntegerMdName, 1);
        break;
    case spv::ExecutionModeOriginUpperLeft:
        metadata.makeMdNamedInt(gla::OriginUpperLeftMdName, 1);
        break;
    case spv::ExecutionModeOriginLowerLeft:
        break;
    case spv::ExecutionModePointMode:
        metadata.makeMdNamedInt(gla::PointModeMdName, 1);
        break;
    case spv::ExecutionModeInputPoints:
        metadata.makeMdNamedInt(gla::InputPrimitiveMdName, gla::EMlgPoints);
        break;
    case spv::ExecutionModeInputLines:
        metadata.makeMdNamedInt(gla::InputPrimitiveMdName, gla::EMlgLines);
        break;
    case spv::ExecutionModeInputLinesAdjacency:
        metadata.makeMdNamedInt(gla::InputPrimitiveMdName, gla::EMlgLinesAdjacency);
        break;
    case spv::ExecutionModeTriangles:
        metadata.makeMdNamedInt(gla::InputPrimitiveMdName, gla::EMlgTriangles);
        break;
    case spv::ExecutionModeInputTrianglesAdjacency:
        metadata.makeMdNamedInt(gla::InputPrimitiveMdName, gla::EMlgTrianglesAdjacency);
        break;
    case spv::ExecutionModeQuads:
        metadata.makeMdNamedInt(gla::InputPrimitiveMdName, gla::EMlgQuads);
        break;
    case spv::ExecutionModeIsolines:
        metadata.makeMdNamedInt(gla::InputPrimitiveMdName, gla::EMlgIsolines);
        break;
    case spv::ExecutionModeXfb:
        metadata.makeMdNamedInt(gla::XfbModeMdName, 1);
        break;
    case spv::ExecutionModeOutputVertices:
        metadata.makeMdNamedInt(gla::NumVerticesMdName, spirv[word++]);
        break;
    case spv::ExecutionModeOutputPoints:
        metadata.makeMdNamedInt(gla::OutputPrimitiveMdName, gla::EMlgPoints);
        break;
    case spv::ExecutionModeOutputLineStrip:
        metadata.makeMdNamedInt(gla::OutputPrimitiveMdName, gla::EMlgLineStrip);
        break;
    case spv::ExecutionModeOutputTriangleStrip:
        metadata.makeMdNamedInt(gla::OutputPrimitiveMdName, gla::EMlgTriangleStrip);
        break;

    case spv::ExecutionModeLocalSize:
    case spv::ExecutionModeEarlyFragmentTests:
    case spv::ExecutionModeDepthGreater:
    case spv::ExecutionModeDepthLess:
    case spv::ExecutionModeDepthUnchanged:
    case spv::ExecutionModeDepthReplacing:

    default:
        gla::UnsupportedFunctionality("execution mode", gla::EATContinue);
        break;
    }
}

// Process an OpDecorate instruction.
// It's been decoded up to but not including optional operands.
void SpvToTopTranslator::addDecoration(spv::Id id, spv::Decoration decoration)
{
    switch (decoration) {
    case spv::DecorationBlock:
        commonMap[id].isBlock = true;
        break;
    case spv::DecorationBufferBlock:
        commonMap[id].isBlock = true;
        commonMap[id].isBuffer = true;
        break;
    default:
        addMetaTypeDecoration(decoration, commonMap[id].metaType);
        break;
    }
}

// Process an OpMemberDecorate instruction.
// It's been decoded up to but not including optional operands.
void SpvToTopTranslator::addMemberDecoration(spv::Id structTypeId, unsigned int member, spv::Decoration decoration)
{
    bumpMemberMetaData(commonMap[structTypeId].memberMetaData, member);
    addMetaTypeDecoration(decoration, (*commonMap[structTypeId].memberMetaData)[member]);
}

// Process an OpDecorate instruction.
// It's been decoded up to but not including optional operands.
void SpvToTopTranslator::addMetaTypeDecoration(spv::Decoration decoration, MetaType& metaType)
{
    unsigned int num;

    switch (decoration) {
    case spv::DecorationRelaxedPrecision:
        metaType.precision = gla::EMpMedium;
        break;

    case spv::DecorationNoPerspective:
    case spv::DecorationFlat:
    case spv::DecorationPatch:
        metaType.interpolationMethod = decoration;
        break;

    case spv::DecorationGLSLShared:
        metaType.layout = gla::EMtlShared;
        break;
    case spv::DecorationGLSLPacked:
        metaType.layout = gla::EMtlPacked;
        break;
    case spv::DecorationRowMajor:
        metaType.layout = gla::EMtlRowMajorMatrix;
        break;
    case spv::DecorationColMajor:
        metaType.layout = gla::EMtlColMajorMatrix;
        break;
    case spv::DecorationMatrixStride:
        num = spirv[word++];
        metaType.matrixStride = (short)num;
        break;
    case spv::DecorationArrayStride:
        num = spirv[word++];
        metaType.arrayStride = (short)num;
        break;

    case spv::DecorationInvariant:
        metaType.invariant = true;
        break;

    case spv::DecorationDescriptorSet:
        num = spirv[word++];
        metaType.set = num;
        break;
    case spv::DecorationBinding:
        num = spirv[word++];
        metaType.binding = num;
        break;
    case spv::DecorationLocation:
        num = spirv[word++];
        metaType.location = num;
        break;
    case spv::DecorationBuiltIn:
        num = spirv[word++];
        metaType.builtIn = GetMdBuiltIn((spv::BuiltIn)num);
        break;
    case spv::DecorationOffset:
    {
        static bool once = false;
        if (! once) {
            gla::UnsupportedFunctionality("member offset", gla::EATContinue);
            once = true;
        }
        break;
    }

    case spv::DecorationStream:
    case spv::DecorationComponent:
    case spv::DecorationIndex:
    case spv::DecorationXfbBuffer:

    case spv::DecorationCentroid:
    case spv::DecorationSample:
    case spv::DecorationUniform:
    default:
        gla::UnsupportedFunctionality("metaType decoration ", decoration, gla::EATContinue);
        break;
    }
}

// Process an OpType instruction.
// On entry, 'word' is the index of the first operand.
void SpvToTopTranslator::addType(spv::Op typeClass, spv::Id resultId, int numOperands)
{
    unsigned int width;
    switch (typeClass) {

    // void
    case spv::OpTypeVoid:
        commonMap[resultId].type = gla::GetVoidType(context);
        break;

    // bool
    case spv::OpTypeBool:        
        commonMap[resultId].type = gla::GetBoolType(context);
        break;

    // float
    case spv::OpTypeFloat:
        width = spirv[word++];
        if (width != 32)
            gla::UnsupportedFunctionality("non-32-bit width OpType");
        commonMap[resultId].type = gla::GetFloatType(context);
        break;

    // int
    case spv::OpTypeInt:
        width = spirv[word++];
        if (width == 32) {
            if (spirv[word++] == 0u) {
                commonMap[resultId].metaType.layout = gla::EMtlUnsigned;
                commonMap[resultId].type = gla::GetUintType(context);
            } else
                commonMap[resultId].type = gla::GetIntType(context);                
        } else
            gla::UnsupportedFunctionality("non-32-bit int for OpType");

        break;

    // vector
    case spv::OpTypeVector:
    {
        llvm::Type* componentType = commonMap[spirv[word++]].type;
        unsigned int vectorSize = spirv[word++];
        commonMap[resultId].type = llvm::VectorType::get(componentType, vectorSize);
        break;
    }

    // array
    case spv::OpTypeArray:
    {
        llvm::Type* elementType = commonMap[spirv[word++]].type;
        unsigned int arraySizeId = spirv[word++];
        commonMap[resultId].type = llvm::ArrayType::get(elementType, gla::GetConstantInt(commonMap[arraySizeId].value));
        break;
    }

    // matrix
    case spv::OpTypeMatrix:
    {
        llvm::Type* columnType = commonMap[spirv[word++]].type;
        int cols = spirv[word++];
        int rows = gla::GetComponentCount(columnType);
        commonMap[resultId].type = glaBuilder->getMatrixType(columnType->getContainedType(0), cols, rows);
        if (commonMap[resultId].metaType.layout == gla::EMtlNone)
            commonMap[resultId].metaType.layout = gla::EMtlColMajorMatrix;
        break;
    }

    // images
    case spv::OpTypeImage:
        commonMap[resultId].type = gla::GetIntType(context);
        commonMap[resultId].metaType.layout = gla::EMtlSampler;
        break;
    case spv::OpTypeSampledImage:
        commonMap[resultId].type = gla::GetIntType(context);
        commonMap[resultId].metaType.layout = gla::EMtlSampler;
        commonMap[resultId].metaType.combinedImageSampler = true;
        break;
    case spv::OpTypeSampler:
        gla::UnsupportedFunctionality("OpTypeSampler");
        break;

    // run-time array
    case spv::OpTypeRuntimeArray:
        gla::UnsupportedFunctionality("run time array OpType");
        break;

    // structure
    case spv::OpTypeStruct:
    {
        std::vector<llvm::Type*> memberTypes;
        memberTypes.resize(numOperands);
        for (int m = 0; m < numOperands; ++m)
            memberTypes[m] = commonMap[spirv[word++]].type;
        commonMap[resultId].type = commonMap[resultId].metaType.name ? llvm::StructType::create(context, memberTypes, commonMap[resultId].metaType.name)
                                                                     : llvm::StructType::create(context, memberTypes);
        break;
    }

    // pointer
    case spv::OpTypePointer:
    {
        spv::StorageClass storageClass = (spv::StorageClass)spirv[word++];
        spv::Id pointee = spirv[word++];
        commonMap[resultId].type = glaBuilder->getPointerType(commonMap[pointee].type, mapStorageClass(storageClass, commonMap[pointee].isBuffer), 0);
        break;
    }

    // function
    case spv::OpTypeFunction:
        // Don't make an LLVM type for a function type, just use it for 
        // reference when a function uses it.  Maybe this will change when
        // function pointers need to be supported.
        break;

    default:
        gla::UnsupportedFunctionality("OpType type\n");
        break;
    }
}

// Process an OpVariable instruction.
// It has already been fully decoded.
void SpvToTopTranslator::addVariable(spv::Id resultId, spv::Id typeId, spv::StorageClass storageClass)
{
    // typeId should be the type of the result of the OpVariable instruction.
    // That is, a pointer, meaning the type of variable to create is a 
    // dereference of typeId.
    assert(llvm::isa<llvm::PointerType>(commonMap[typeId].type));
    llvm::Type* variableType = commonMap[typeId].type->getContainedType(0);

    commonMap[resultId].storageClass = storageClass;

    llvm::Constant* initializer = 0;
    int constantBuffer = 0;
    gla::Builder::EStorageQualifier glaQualifier = mapStorageClass(storageClass, commonMap[resultId].isBuffer);

    const char* name = commonMap[resultId].metaType.name;
    if (name) {
        if (name[0] == 0)
            name = "anon@";
    } else {
        if (commonMap[resultId].metaType.builtIn != gla::EmbNone) {
            snprintf(&commonMap[resultId].metaType.buf[0], MetaType::bufSize, "__glab%d_", commonMap[resultId].metaType.builtIn);
            name = &commonMap[resultId].metaType.buf[0];
            commonMap[resultId].metaType.name = name;
        } else if (storageClass != spv::StorageClassFunction)
            name = "nn";   // no name, but LLVM treats I/O as dead when there is no name
        else
            name = "";
    }

    commonMap[resultId].value = glaBuilder->createVariable(glaQualifier, constantBuffer, variableType, initializer, 0, name);

    // Set up any IO metadata
    int numSlots;
    int slot;
    switch (storageClass) {
    case spv::StorageClassOutput:
        slot = assignSlot(resultId, numSlots);
        makeOutputMetadata(resultId, slot, numSlots);
        break;
    case spv::StorageClassInput:
        slot = assignSlot(resultId, numSlots);
        makeInputMetadata(resultId, slot);
    case spv::StorageClassUniformConstant:
    case spv::StorageClassUniform:
        declareUniformMetadata(resultId);
        break;
    default:
        break;
    }
}

gla::Builder::EStorageQualifier SpvToTopTranslator::mapStorageClass(spv::StorageClass storageClass, bool isBuffer)
{
    switch (storageClass) {
    case spv::StorageClassFunction:
        return gla::Builder::ESQLocal;
    case spv::StorageClassUniformConstant:
    case spv::StorageClassUniform:
        return isBuffer ? gla::Builder::ESQBuffer : gla::Builder::ESQUniform;
    case spv::StorageClassInput:
        return gla::Builder::ESQInput;
    case spv::StorageClassOutput:
        return gla::Builder::ESQOutput;
    case spv::StorageClassPrivate:
        return gla::Builder::ESQGlobal;
    case spv::StorageClassWorkgroup:
        return gla::Builder::ESQShared;

    default:
        gla::UnsupportedFunctionality("storage class");
        break;
    }

    // should not normally be executed:
    return gla::Builder::ESQGlobal;
}

// Build a literal constant.
void SpvToTopTranslator::addConstant(spv::Op opCode, spv::Id resultId, spv::Id typeId, int numOperands)
{
    // vector of constants for LLVM
    std::vector<llvm::Constant*> llvmConsts;

    switch (opCode) {
    case spv::OpConstantTrue:
        llvmConsts.push_back(gla::MakeBoolConstant(context, 1));
        break;
    case spv::OpConstantFalse:
        llvmConsts.push_back(gla::MakeBoolConstant(context, 0));
        break;
    case spv::OpConstant:
        switch (getOpCode(typeId)) {
        case spv::OpTypeFloat:
            if (numOperands > 1)
                gla::UnsupportedFunctionality("non-single-precision constant");
            else
                llvmConsts.push_back(gla::MakeFloatConstant(context, *(const float*)(&spirv[word])));
            break;
        case spv::OpTypeInt:
            if (commonMap[typeId].type == commonMap[typeId].type->getInt1Ty(context))
                gla::UnsupportedFunctionality("1-bit integer");
            else if (commonMap[resultId].metaType.layout == gla::EMtlUnsigned)
                llvmConsts.push_back(gla::MakeUnsignedConstant(context, spirv[word]));
            else
                llvmConsts.push_back(gla::MakeIntConstant(context, (int)spirv[word]));
            break;
        default:
            gla::UnsupportedFunctionality("literal constant type");
            break;
        }
    }

    commonMap[resultId].value = glaBuilder->getConstant(llvmConsts, commonMap[typeId].type);
}

// Build up a hierarchical constant.
void SpvToTopTranslator::addConstantAggregate(spv::Id resultId, spv::Id typeId, int numOperands)
{
    // vector of constants for LLVM
    std::vector<llvm::Constant*> llvmConsts;
    for (int op = 0; op < numOperands; ++op)
        llvmConsts.push_back(llvm::dyn_cast<llvm::Constant>(commonMap[spirv[word++]].value));

    commonMap[resultId].value = glaBuilder->getConstant(llvmConsts, commonMap[typeId].type);
}

//
// Find and use the user-specified location as a slot, or if a location was not
// specified, pick the next non-user available slot. User-specified locations
// directly use the location specified, while non-user-specified will use locations
// starting after MaxUserLayoutLocation to avoid collisions.
//
// Ensure enough slots are consumed to cover the size of the data represented by the node symbol.
//
// 'numSlots' means number of GLSL locations when using logical IO.
//
int SpvToTopTranslator::assignSlot(spv::Id resultId, int& numSlots)
{
    // Base the numbers of slots on the front-end's computation, if possible, otherwise estimate it.
    numSlots = 1;

    //??
    //if (type.isArray() && ! type.getQualifier().isArrayedIo(glslangIntermediate->getStage()))
    //    numSlots = type.getArraySize();
    //if (type.isStruct() || type.isMatrix() || type.getBasicType() == spv::EbtDouble)
    //    gla::UnsupportedFunctionality("complex I/O type; use new glslang C++ interface", gla::EATContinue);
    //}

    int slot = commonMap[resultId].metaType.location;
    if (slot == gla::MaxUserLayoutLocation) {
        slot = nextSlot;
        nextSlot += numSlots;
    }

    return slot;
}

//
// The following is a set of helper functions for translating SPIR-V to metadata, 
// so that information not representable in LLVM does not get lost.
//

// Translate a SPIR-V variable to the kind of input/output qualifier needed for it in metadata.
gla::EMdInputOutput SpvToTopTranslator::getMdQualifier(spv::Id resultId) const
{
    gla::EMdInputOutput mdQualifier = gla::EMioNone;

    spv::Id typeId = dereferenceTypeId(commonMap[resultId].typeId);
    typeId = getArrayElementTypeId(typeId);
    if (commonMap[typeId].isBlock) {
        // storage class comes from the variable
        switch (commonMap[resultId].storageClass) {
        case spv::StorageClassInput:   mdQualifier = gla::EMioPipeInBlock;        break;
        case spv::StorageClassOutput:  mdQualifier = gla::EMioPipeOutBlock;       break;
        case spv::StorageClassUniform: mdQualifier = gla::EMioUniformBlockMember; break;
        //case spv::Storage  mdQualifier = gla::EMioBufferBlockMember; break;
        default:                                                                  break;
        }

        return mdQualifier;
    }

    // non-blocks...

    switch (commonMap[resultId].metaType.builtIn) {
    case gla::EmbPosition:    mdQualifier = gla::EMioVertexPosition; break;
    case gla::EmbPointSize:   mdQualifier = gla::EMioPointSize;      break;
    case gla::EmbClipVertex:  mdQualifier = gla::EMioClipVertex;     break;
    case gla::EmbInstanceId:  mdQualifier = gla::EMioInstanceId;     break;
    case gla::EmbVertexId:    mdQualifier = gla::EMioVertexId;       break;
    case gla::EmbVertexIndex: mdQualifier = gla::EMioVertexIndex;    break;
    case gla::EmbFragCoord:   mdQualifier = gla::EMioFragmentCoord;  break;
    case gla::EmbPointCoord:  mdQualifier = gla::EMioPointCoord;     break;
    case gla::EmbFace:        mdQualifier = gla::EMioFragmentFace;   break;
    case gla::EmbFragColor:   mdQualifier = gla::EMioPipeOut;        break;
    case gla::EmbFragDepth:   mdQualifier = gla::EMioFragmentDepth;  break;

    default:
        switch (commonMap[resultId].storageClass) {
        case spv::StorageClassUniformConstant: mdQualifier = gla::EMioDefaultUniform;     break;
        case spv::StorageClassInput:           mdQualifier = gla::EMioPipeIn;             break;
        case spv::StorageClassUniform:         mdQualifier = gla::EMioUniformBlockMember; break;
        case spv::StorageClassOutput:          mdQualifier = gla::EMioPipeOut;            break;
        default:
            gla::UnsupportedFunctionality("metadata storage class", commonMap[resultId].storageClass, gla::EATContinue);
            break;
        }
    }

    return mdQualifier;
}

// Translate a SPIR-V sampler type to the kind of image/texture needed for it in metadata.
gla::EMdSampler SpvToTopTranslator::getMdSampler(spv::Id typeId) const
{
    if (commonMap[typeId].metaType.combinedImageSampler)
        return gla::EMsTexture;
    else
        return gla::EMsImage;
}

// Translate a SPIR-V sampler type to the metadata's dimensionality.
gla::EMdSamplerDim SpvToTopTranslator::getMdSamplerDim(spv::Id typeId) const
{
    switch (getImageDim(typeId)) {
    case spv::Dim1D:     return gla::EMsd1D;
    case spv::Dim2D:     return gla::EMsd2D;
    case spv::Dim3D:     return gla::EMsd3D;
    case spv::DimCube:   return gla::EMsdCube;
    case spv::DimRect:   return gla::EMsdRect;
    case spv::DimBuffer: return gla::EMsdBuffer;
    default:
        gla::UnsupportedFunctionality("unknown sampler dimension", gla::EATContinue);
        return gla::EMsd2D;
    }
}

// Translate a SPIR-V sampler type to return (sampled) type needed for it in metadata.
gla::EMdSamplerBaseType SpvToTopTranslator::getMdSamplerBaseType(spv::Id typeId) const
{
    spv::Id sampledType = getImageSampledType(typeId);
    if (commonMap[sampledType].type->getTypeID() == llvm::Type::FloatTyID)
        return gla::EMsbFloat;
    else if (commonMap[sampledType].metaType.layout == gla::EMtlUnsigned)
        return gla::EMsbUint;
    else
        return gla::EMsbInt;
}

// Translate SPIR-V descriptions for type of varying/interpolation into Top IR's description.
void SpvToTopTranslator::getInterpolationLocationMethod(spv::Id id, gla::EInterpolationMethod& method, gla::EInterpolationLocation& location)
{
    switch (commonMap[id].metaType.interpolationMethod) {
    case spv::DecorationNoPerspective: method = gla::EIMNoperspective;  break;
    case spv::DecorationPatch:         method = gla::EIMPatch;          break;
    default:                           method = gla::EIMNone;           break;
    }

    switch (commonMap[id].metaType.interpolateTo) {
    case spv::DecorationSample:        location = gla::EILSample;       break;
    case spv::DecorationCentroid:      location = gla::EILCentroid;     break;
    default:                           location = gla::EILFragment;     break;
    }
}

// For making a global that won't be optimized away, so that metadata can have a type
// that's not statically consumed.
llvm::Value* SpvToTopTranslator::makePermanentTypeProxy(llvm::Value* value)
{
    // Make a type proxy that won't be optimized away (we still want the real llvm::Value to get optimized away when it can)
    llvm::Type* type = value->getType();
    while (type->getTypeID() == llvm::Type::PointerTyID)
        type = llvm::dyn_cast<llvm::PointerType>(type)->getContainedType(0);

    // Don't hook this global into the module, that will cause LLVM to optimize it away.
    llvm::Value* typeProxy = new llvm::GlobalVariable(type, true, llvm::GlobalVariable::ExternalLinkage, 0, value->getName() + "_typeProxy");
    manager.addToFreeList(typeProxy);

    return typeProxy;
}

// Create all the metadata needed to back up a uniform or block.
llvm::MDNode* SpvToTopTranslator::declareUniformMetadata(spv::Id resultId)
{
    llvm::MDNode* md;

    gla::EMdInputOutput ioType = getMdQualifier(resultId);
    switch (ioType) {
    case gla::EMioDefaultUniform:
        md = declareMdDefaultUniform(resultId);
        break;
    case gla::EMioUniformBlockMember:
    case gla::EMioBufferBlockMember:
    case gla::EMioBufferBlockMemberArrayed:
        md = declareMdUniformBlock(ioType, resultId);
        break;
    default:
        return 0;
    }

    return md;
}

// Make a !gla.uniform node, as per metadata.h, for a default uniform
// Assumes the resultId is for a variable declaration, meaning the typeId
// is a pointer to the actual type.
llvm::MDNode* SpvToTopTranslator::declareMdDefaultUniform(spv::Id resultId)
{
    // Deference the type
    spv::Id typeId = dereferenceTypeId(commonMap[resultId].typeId);
    typeId = getArrayElementTypeId(typeId);
    llvm::MDNode* samplerMd = makeMdSampler(typeId, commonMap[resultId].value);

    // Create hierarchical type information if it's an aggregate
    gla::EMdTypeLayout layout = commonMap[typeId].metaType.layout;
    llvm::MDNode* structure = 0;
    if (commonMap[typeId].type->getTypeID() == llvm::Type::StructTyID)
        structure = declareMdType(typeId, commonMap[resultId].metaType);

    // Make the main node
    return metadata.makeMdInputOutput(NonNullName(commonMap[resultId].metaType.name), gla::UniformListMdName, gla::EMioDefaultUniform,
                                      makePermanentTypeProxy(commonMap[resultId].value),
                                      layout, commonMap[resultId].metaType.precision, commonMap[resultId].metaType.location, samplerMd, structure,
                                      -1, commonMap[resultId].metaType.builtIn, packSetBinding(commonMap[resultId].metaType));
}

// Make a metadata description of a sampler's type.
llvm::MDNode* SpvToTopTranslator::makeMdSampler(spv::Id typeId, llvm::Value* value)
{
    // Figure out sampler information, if it's a sampler
    if (commonMap[typeId].metaType.layout != gla::EMtlSampler)
        return 0;

    llvm::Value* typeProxy = 0;
    if (! value) {
        // Don't hook this global into the module, that will cause LLVM to optimize it away.
        typeProxy = new llvm::GlobalVariable(commonMap[typeId].type, true, llvm::GlobalVariable::ExternalLinkage, 0, "sampler_typeProxy");
        manager.addToFreeList(typeProxy);
    } else
        typeProxy = makePermanentTypeProxy(value);

    return metadata.makeMdSampler(getMdSampler(typeId), typeProxy, getMdSamplerDim(typeId), isImageArrayed(typeId),
                                  isImageDepth(typeId), getMdSamplerBaseType(typeId));
}

// Make a !gla.uniform node, as per metadata.h, for a uniform block or buffer block
// (depending on ioType).
// Assumes the resultId is for a variable declaration, meaning the typeId
// is a pointer to the actual type.
llvm::MDNode* SpvToTopTranslator::declareMdUniformBlock(gla::EMdInputOutput ioType, spv::Id resultId)
{
    // Deference the type
    spv::Id typeId = dereferenceTypeId(commonMap[resultId].typeId);

    // Make hierachical type information
    llvm::MDNode* block = declareMdType(typeId, commonMap[resultId].metaType);

    // Make the main node
    return metadata.makeMdInputOutput(NonNullName(commonMap[resultId].metaType.name), gla::UniformListMdName, ioType, makePermanentTypeProxy(commonMap[resultId].value),
                                      commonMap[typeId].metaType.layout, commonMap[resultId].metaType.precision, commonMap[resultId].metaType.location, 0, block, -1,
                                      gla::EmbNone, packSetBinding(commonMap[resultId].metaType));
}

// Make a !aggregate node for the object, as per metadata.h, calling declareMdType with the type
// to recursively finish for hierarchical types.
llvm::MDNode* SpvToTopTranslator::declareMdType(spv::Id typeId, MetaType& metaType)
{
    // if contained type is an array, we actually need the type of the elements
    // (we need metadata for the element type, not the array itself)
    typeId = getArrayElementTypeId(typeId);

    // Figure out sampler information if it's a sampler
    llvm::MDNode* samplerMd = makeMdSampler(typeId, 0);
    std::vector<llvm::Value*> mdArgs;

    // name of aggregate, if an aggregate (struct or block)
    gla::EMdTypeLayout typeLayout;
    if (commonMap[typeId].type->getTypeID() == llvm::Type::StructTyID) {
        mdArgs.push_back(llvm::MDString::get(context, NonNullName(commonMap[typeId].metaType.name)));
        typeLayout = commonMap[typeId].metaType.layout;
    } else {
        mdArgs.push_back(llvm::MDString::get(context, ""));
        typeLayout = metaType.layout;
    }

    // !typeLayout
    mdArgs.push_back(metadata.makeMdTypeLayout(typeLayout, metaType.precision, metaType.location, samplerMd, -1, metaType.builtIn, packSetBinding(metaType)));

    if (commonMap[typeId].type->getTypeID() == llvm::Type::StructTyID) {
        int numMembers = (int)commonMap[typeId].type->getNumContainedTypes();

        // make sure we have enough metadata, if not enough name/decorations created it
        bumpMemberMetaData(commonMap[typeId].memberMetaData, numMembers - 1);

        std::vector<MetaType>& memberMetaData = *commonMap[typeId].memberMetaData;
        for (int t = 0; t < numMembers; ++t) {
            spv::Id containedTypeId = getStructMemberTypeId(typeId, t);

            // name of member
            mdArgs.push_back(llvm::MDString::get(context, memberMetaData[t].name));

            // type of member
            llvm::MDNode* mdType = declareMdType(containedTypeId, memberMetaData[t]);
            mdArgs.push_back(mdType);
        }
    }

    return llvm::MDNode::get(context, mdArgs);
}

// Make metadata node for either an 'in' or an 'out' variable/block.
// Assumes the resultId is for a variable declaration, meaning the typeId
// is a pointer to the actual type.
llvm::MDNode* SpvToTopTranslator::makeInputOutputMetadata(spv::Id resultId, int slot, const char* kind)
{
    // Deference the type
    spv::Id typeId = dereferenceTypeId(commonMap[resultId].typeId);
    typeId = getArrayElementTypeId(typeId);

    llvm::MDNode* aggregate = 0;
    if (commonMap[typeId].type->getTypeID() == llvm::Type::StructTyID) {
        // Make hierarchical type information, for the dereferenced type
        aggregate = declareMdType(typeId, commonMap[resultId].metaType);
    }

    gla::EInterpolationMethod interpMethod = gla::EIMNone;
    gla::EInterpolationLocation interpLocation = gla::EILFragment;
    getInterpolationLocationMethod(resultId, interpMethod, interpLocation);

    return metadata.makeMdInputOutput(NonNullName(commonMap[resultId].metaType.name), kind, getMdQualifier(resultId), makePermanentTypeProxy(commonMap[resultId].value),
                                      commonMap[typeId].metaType.layout, commonMap[resultId].metaType.precision, slot, 0, aggregate,
                                      gla::MakeInterpolationMode(interpMethod, interpLocation), commonMap[resultId].metaType.builtIn);
}

// Make metadata node for an 'out' variable/block and associate it with the 
// output-variable cache in the gla builder.
void SpvToTopTranslator::makeOutputMetadata(spv::Id resultId, int slot, int numSlots)
{
    llvm::MDNode* md = makeInputOutputMetadata(resultId, slot, gla::OutputListMdName);

    if (commonMap[resultId].metaType.invariant)
        module->getOrInsertNamedMetadata(gla::InvariantListMdName)->addOperand(md);
}

llvm::MDNode* SpvToTopTranslator::makeInputMetadata(spv::Id resultId, int slot)
{
    llvm::MDNode* mdNode = makeInputOutputMetadata(resultId, slot, gla::InputListMdName);

    return mdNode;
}

void SpvToTopTranslator::decodeResult(bool type, spv::Id& typeId, spv::Id& resultId)
{
    unsigned int instructionStart = word - 1;

    if (type)
        typeId = spirv[word++];
    
    resultId = spirv[word++];

    if (type && commonMap[resultId].typeId == 0)
        commonMap[resultId].typeId = typeId;

    // map instruction for future reference
    commonMap[resultId].instructionIndex = instructionStart;
}

const char* SpvToTopTranslator::findAName(spv::Id choice1, spv::Id choice2)
{
    if (commonMap[choice1].metaType.name)
        return commonMap[choice1].metaType.name;
    else if (choice2 != 0 && commonMap[choice2].metaType.name)
        return commonMap[choice1].metaType.name;
    else {
        // Look ahead to see if we're in a potential chain of instructions leading 
        // to a store that has the result named.  This is just an approximation.
        spv::Id chain = choice1;
        int trialInst = nextInst;
        for (int i = 0; i < 200; i++) {
            if (trialInst + 5 >= (int)spirv.size())
                break;
            spv::Op opcode = GetOpCode(spirv[trialInst]);
            switch (opcode) {
            case spv::OpStore:
                if (spirv[trialInst + 2] == chain ||
                    (chain == 0 && commonMap[spirv[trialInst + 1]].metaType.name))
                    chain = spirv[trialInst + 1];
                break;

            // up against something not worth crossing?
            case spv::OpFunctionEnd:
            case spv::OpSwitch:
            case spv::OpReturn:
            case spv::OpReturnValue:
            case spv::OpUnreachable:
                return 0;

            // something worth ignoring?
            case spv::OpLoopMerge:
            case spv::OpSelectionMerge:
            case spv::OpBranch:
            case spv::OpLabel:
            case spv::OpKill:
                break;

            // propagate access-chain base name into pointer name?
            case spv::OpAccessChain:
            {
                spv::Id baseId = spirv[trialInst + 3];
                spv::Id resultId = spirv[trialInst + 2];
                if (commonMap[resultId].metaType.name == 0)
                    commonMap[resultId].metaType.name = commonMap[baseId].metaType.name;
                break;
            }

            // Does this instruction consume it but not have a result?
            case spv::OpBranchConditional:  // probably "?:"
                if (spirv[trialInst + 1] == chain)
                    chain = 0;
                break;

            default:
                // hopefully a normal data-flow processing instruction, where the instruction has result
                if (chain != 0 && spirv[trialInst + 2] < numIds) {
                    for (int op = 3; op < GetWordCount(spirv[trialInst]); ++op) {
                        if (spirv[trialInst + op] == chain) {
                            chain = spirv[trialInst + 2];
                            break;
                        }
                    }
                }
                break;
            }
            const char* name = commonMap[chain].metaType.name;
            if (name != 0)
                return name;
            trialInst += GetWordCount(spirv[trialInst]);
        }
    }

    return 0;
}

void SpvToTopTranslator::translateInstruction(spv::Op opCode, int numOperands)
{
    spv::Id resultId;
    spv::Id typeId;

    switch (opCode) {
    case spv::OpSource:
    {
        spv::SourceLanguage source = (spv::SourceLanguage)spirv[word++];
        unsigned int sourceVersion = spirv[word++];
        manager.setVersion(sourceVersion);
        switch (source) {
        case spv::SourceLanguageESSL:
            manager.setProfile(EEsProfile);
            break;
        case spv::SourceLanguageGLSL:
            manager.setProfile(ECoreProfile);
            break;
        case spv::SourceLanguageOpenCL_C:
        case spv::SourceLanguageOpenCL_CPP:
        default:
            gla::UnsupportedFunctionality("non-GL profile", gla::EATContinue);
            manager.setProfile(ECoreProfile);
            break;
        }
        break;
    }
    case spv::OpSourceExtension:
        manager.addExtension((const char*)&spirv[word]);
        break;
    case spv::OpExtInstImport:
        break;
    case spv::OpMemoryModel:
        break;
    case spv::OpEntryPoint:
    {
        spv::ExecutionModel model = (spv::ExecutionModel)spirv[word++];
        spv::Id entry = spirv[word++];
        setEntryPoint(model, entry);
        break;
    }
    case spv::OpExecutionMode:
    {
        spv::Id entryPoint = (spv::ExecutionModel)spirv[word++];
        spv::ExecutionMode mode = (spv::ExecutionMode)spirv[word++];
        setExecutionMode(entryPoint, mode);
        if (numOperands > 2)
            gla::UnsupportedFunctionality("OpExecutionMode with more than two operands", gla::EATContinue);
        break;
    }
    case spv::OpCapability:
        break;
    case spv::OpTypeVoid:
    case spv::OpTypeBool:
    case spv::OpTypeInt:
    case spv::OpTypeFloat:
    case spv::OpTypeVector:
    case spv::OpTypeMatrix:
    case spv::OpTypeImage:
    case spv::OpTypeSampler:
    case spv::OpTypeSampledImage:
    case spv::OpTypeArray:
    case spv::OpTypeRuntimeArray:
    case spv::OpTypeStruct:
    case spv::OpTypePointer:
    case spv::OpTypeFunction:
        decodeResult(false, typeId, resultId);
        numOperands -= 1;
        addType(opCode, resultId, numOperands);
        break;
    case spv::OpConstantTrue:
    case spv::OpConstantFalse:
    case spv::OpConstant:
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        addConstant(opCode, resultId, typeId, numOperands);
        break;
    case spv::OpConstantComposite:
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        addConstantAggregate(resultId, typeId, numOperands);
        break;
    case spv::OpVariable:
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        addVariable(resultId, typeId, (spv::StorageClass)spirv[word++]);
        break;
    case spv::OpDecorate:
    {
        spv::Id id = spirv[word++];
        spv::Decoration decoration = (spv::Decoration)spirv[word++];
        addDecoration(id, decoration);
        break;
    }
    case spv::OpMemberDecorate:
    {
        spv::Id structTypeId = spirv[word++];
        unsigned int member = spirv[word++];
        spv::Decoration decoration = (spv::Decoration)spirv[word++];
        addMemberDecoration(structTypeId, member, decoration);
        break;
    }
    case spv::OpName:
    {
        spv::Id id = spirv[word++];
        commonMap[id].metaType.name = (const char *)&spirv[word];
        break;
    }
    case spv::OpMemberName:
    {
        spv::Id id = spirv[word++];
        unsigned int memberNumber = spirv[word++];
        const char* name = (const char *)&spirv[word];
        bumpMemberMetaData(commonMap[id].memberMetaData, memberNumber);
        (*commonMap[id].memberMetaData)[memberNumber].name = name;
        break;
    }

    case spv::OpString:
        gla::UnsupportedFunctionality("OpString", gla::EATContinue);
        break;
    case spv::OpLine:
        gla::UnsupportedFunctionality("OpLine", gla::EATContinue);
        break;

    case spv::OpLoad:
        decodeResult(true, typeId, resultId);
        commonMap[resultId].value = glaBuilder->createLoad(commonMap[spirv[word++]].value);
        break;
    case spv::OpStore:
    {
        llvm::Value* lvalue = commonMap[spirv[word++]].value;
        llvm::Value* rvalue = commonMap[spirv[word++]].value;
        glaBuilder->createStore(rvalue, lvalue);
        break;
    }

    case spv::OpFunction:
        decodeResult(true, typeId, resultId);
        handleOpFunction(typeId, resultId);
        currentArg = commonMap[currentFunction].function->arg_begin();
        break;
    case spv::OpFunctionParameter:
        decodeResult(true, typeId, resultId);
        commonMap[resultId].value = &(*currentArg);
        ++currentArg;
        break;
    case spv::OpFunctionEnd:
        glaBuilder->leaveFunction(inEntryPoint());
        currentFunction = 0;
        break;

    case spv::OpIAdd:
    case spv::OpFAdd:
    case spv::OpISub:
    case spv::OpFSub:
    case spv::OpIMul:
    case spv::OpFMul:
    case spv::OpUDiv:
    case spv::OpSDiv:
    case spv::OpFDiv:
    case spv::OpUMod:
    case spv::OpSRem:
    case spv::OpSMod:
    case spv::OpFRem:
    case spv::OpFMod:
    case spv::OpVectorTimesScalar:
    case spv::OpMatrixTimesScalar:
    case spv::OpVectorTimesMatrix:
    case spv::OpMatrixTimesVector:
    case spv::OpMatrixTimesMatrix:
    case spv::OpOuterProduct:
    case spv::OpShiftRightLogical:
    case spv::OpShiftRightArithmetic:
    case spv::OpShiftLeftLogical:
    case spv::OpLogicalOr:
    case spv::OpLogicalNotEqual:
    case spv::OpLogicalAnd:
    case spv::OpBitwiseOr:
    case spv::OpBitwiseXor:
    case spv::OpBitwiseAnd:
    case spv::OpIEqual:
    case spv::OpFOrdEqual:
    case spv::OpFUnordEqual:
    case spv::OpINotEqual:
    case spv::OpFOrdNotEqual:
    case spv::OpFUnordNotEqual:
    case spv::OpULessThan:
    case spv::OpSLessThan:
    case spv::OpFOrdLessThan:
    case spv::OpFUnordLessThan:
    case spv::OpUGreaterThan:
    case spv::OpSGreaterThan:
    case spv::OpFOrdGreaterThan:
    case spv::OpFUnordGreaterThan:
    case spv::OpULessThanEqual:
    case spv::OpSLessThanEqual:
    case spv::OpFOrdLessThanEqual:
    case spv::OpFUnordLessThanEqual:
    case spv::OpUGreaterThanEqual:
    case spv::OpSGreaterThanEqual:
    case spv::OpFOrdGreaterThanEqual:
    case spv::OpFUnordGreaterThanEqual:
    case spv::OpDot:
    {
        decodeResult(true, typeId, resultId);
        unsigned int left = spirv[word++];
        unsigned int right = spirv[word++];
        commonMap[resultId].value = createBinaryOperation(opCode, commonMap[resultId].metaType.precision, commonMap[left].value, commonMap[right].value, 
                                                          commonMap[left].metaType.layout == gla::EMtlNone, false, findAName(resultId));
        if (commonMap[resultId].value == 0)
            gla::UnsupportedFunctionality("binary operation");
        break;
    }
    case spv::OpSelect:
    {
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        spv::Id conditionId = spirv[word++];
        spv::Id trueId = spirv[word++];
        spv::Id falseId = spirv[word++];
        commonMap[resultId].value = llvmBuilder.CreateSelect(commonMap[conditionId].value, commonMap[trueId].value, commonMap[falseId].value);
        break;
    }
    case spv::OpAccessChain:
    {
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        spv::Id baseId = spirv[word++];

        llvm::SmallVector<llvm::Value*, 5> chain;
        chain.push_back(gla::MakeIntConstant(context, 0));
        llvm::Value* base = commonMap[baseId].value;
        for (int op = 1; op < numOperands; ++op)
            chain.push_back(commonMap[spirv[word++]].value);
        commonMap[resultId].value = llvmBuilder.CreateGEP(base, chain);
        if (commonMap[resultId].metaType.name == 0)
            commonMap[resultId].metaType.name = commonMap[baseId].metaType.name;
        break;
    }
    case spv::OpVectorShuffle:
    {
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        llvm::Value* vector1 = commonMap[spirv[word++]].value;
        llvm::Value* vector2 = commonMap[spirv[word++]].value;
        numOperands -= 2;
        unsigned numTargetComponents = gla::GetComponentCount(commonMap[typeId].type);
        assert(numOperands == numTargetComponents);

        if (vector1 == vector2) {
            llvm::SmallVector<int, 4> channels;
            for (int op = 0; op < numOperands; ++op)
                channels.push_back(spirv[word++]);
            commonMap[resultId].value = glaBuilder->createSwizzle(commonMap[resultId].metaType.precision, vector1, channels, commonMap[typeId].type);
        } else if (gla::GetComponentCount(vector1) == numTargetComponents) {
            // See if this is just updating vector1 with parts of vector2
            bool justUpdate = true;
            for (int i = 0; i < numOperands; ++i) {
                if (spirv[word + i] < numTargetComponents && spirv[word + i] != i)
                    justUpdate = false;
            }
            if (! justUpdate)
                gla::UnsupportedFunctionality("generalized shuffle with matching sizes");

            // Now, we know anything from vector1 is an identity shuffle,
            // so, just collect from vector2 and put into the first one.
            for (int i = 0; i < numOperands; ++i) {
                if (spirv[word + i] >= numTargetComponents) {
                    llvm::Value* comp = llvmBuilder.CreateExtractElement(vector2, gla::MakeIntConstant(context, spirv[word + i] - numTargetComponents));
                    vector1 = llvmBuilder.CreateInsertElement(vector1, comp, gla::MakeIntConstant(context, i));
                }
            }
            commonMap[resultId].value = vector1;
        } else {
            printf("%d %d %d %d \n", gla::GetComponentCount(commonMap[typeId].type), gla::GetComponentCount(vector1), gla::GetComponentCount(vector2), numOperands);
            gla::UnsupportedFunctionality("generalized shuffle with unmatching sizes");
        }

        break;
    }
    case spv::OpCompositeConstruct:
    {
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        std::vector<llvm::Value*> constituents;
        for (int i = 0; i < numOperands; ++i)
            constituents.push_back(commonMap[spirv[word++]].value);
        commonMap[resultId].value = createConstructor(resultId, typeId, constituents);
        break;
    }
    case spv::OpCompositeExtract:
    {
        decodeResult(true, typeId, resultId);
        llvm::Value* composite = commonMap[spirv[word++]].value;
        numOperands -= 3;

        // Build the indexes...
        // SPIR-V can go to down to components, but LLVM stops at vectors, so track type
        // to break out early.
        const llvm::Type* currentType = composite->getType();
        llvm::SmallVector<unsigned int, 4> indexes;
        int vectorIndex = -1;
        for (int i = 0; i < numOperands; ++i) {
            if (currentType->getTypeID() == llvm::Type::VectorTyID) {
                vectorIndex = spirv[word++];
                break;
            }
            indexes.push_back(spirv[word++]);
            currentType = currentType->getContainedType(std::min(indexes.back(), currentType->getNumContainedTypes() - 1));
        }

        // Do the operation
        if (vectorIndex == -1)
            commonMap[resultId].value = llvmBuilder.CreateExtractValue(composite, indexes);
        else {
            // The final index was into a vector.
            // If the vector is a subpart of the composite; first extract the vector
            llvm::Value* vector;
            if (indexes.size() > 0)
                vector = llvmBuilder.CreateExtractValue(composite, indexes);
            else
                vector = composite;
            commonMap[resultId].value = llvmBuilder.CreateExtractElement(vector, gla::MakeIntConstant(context, vectorIndex));
        }

        break;
    }
    case spv::OpCompositeInsert:
    {
        decodeResult(true, typeId, resultId);
        llvm::Value* object = commonMap[spirv[word++]].value;
        llvm::Value* composite = commonMap[spirv[word++]].value;
        numOperands -= 4;

        // Build the indexes...
        // SPIR-V can go to down to components, but LLVM stops at vectors, so track type
        // to break out early.
        const llvm::Type* currentType = composite->getType();
        llvm::SmallVector<unsigned int, 4> indexes;
        int vectorIndex = -1;
        for (int i = 0; i < numOperands; ++i) {
            if (currentType->getTypeID() == llvm::Type::VectorTyID) {
                vectorIndex = spirv[word++];
                break;
            }
            indexes.push_back(spirv[word++]);
            currentType = currentType->getContainedType(std::min(indexes.back(), currentType->getNumContainedTypes() - 1));
        }

        // Do the operation
        if (vectorIndex == -1)
            commonMap[resultId].value = llvmBuilder.CreateInsertValue(composite, object, indexes);
        else {
            // The final index was into a vector.
            // If the vector is a subpart of the composite; extract the vector, do the insert, then put the vector back.
            // If the vector is the composite, deal with it more directly.
            if (indexes.size() > 0) {
                llvm::Value* vector = llvmBuilder.CreateExtractValue(composite, indexes);
                vector = llvmBuilder.CreateInsertElement(vector, object, gla::MakeIntConstant(context, vectorIndex));
                commonMap[resultId].value = llvmBuilder.CreateInsertValue(composite, vector, indexes);
            } else
                commonMap[resultId].value = llvmBuilder.CreateInsertElement(composite, object, gla::MakeIntConstant(context, vectorIndex));
        }

        break;
    }
    case spv::OpVectorExtractDynamic:
    {
        decodeResult(true, typeId, resultId);
        llvm::Value* source = commonMap[spirv[word++]].value;
        llvm::Value* component = commonMap[spirv[word++]].value;
        commonMap[resultId].value = llvmBuilder.CreateExtractElement(source, component);
        break;
    }
    case spv::OpVectorInsertDynamic:
    {
        decodeResult(true, typeId, resultId);
        llvm::Value* target = commonMap[spirv[word++]].value;
        llvm::Value* source = commonMap[spirv[word++]].value;
        llvm::Value* component = commonMap[spirv[word++]].value;
        commonMap[resultId].value = llvmBuilder.CreateInsertElement(target, source, component);
        break;
    }
    case spv::OpUndef:
        decodeResult(true, typeId, resultId);
        commonMap[resultId].value = llvm::UndefValue::get(commonMap[typeId].type);
        break;
    case spv::OpPhi:
    {
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        llvm::PHINode* phi = llvmBuilder.CreatePHI(commonMap[typeId].type, numOperands);
        while (numOperands >= 2) {
            spv::Id variable = spirv[word++];
            spv::Id parent = spirv[word++];
            makeLabelBlock(parent);
            numOperands -= 2;
            phi->addIncoming(commonMap[variable].value, commonMap[parent].block);
        }
        commonMap[resultId].value = phi;
        break;
    }
    case spv::OpSampledImage:
        gla::UnsupportedFunctionality("OpSampler");
        break;

    case spv::OpImage:
    {
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        unsigned int operand = spirv[word++];
        commonMap[resultId].value = commonMap[operand].value;
        break;
    }

    case spv::OpImageSampleImplicitLod:
    case spv::OpImageSampleExplicitLod:
    case spv::OpImageSampleDrefImplicitLod:
    case spv::OpImageSampleDrefExplicitLod:
    case spv::OpImageSampleProjImplicitLod:
    case spv::OpImageSampleProjExplicitLod:
    case spv::OpImageSampleProjDrefImplicitLod:
    case spv::OpImageSampleProjDrefExplicitLod:
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        commonMap[resultId].value = createSamplingCall(opCode, typeId, resultId, numOperands);
        break;

    case spv::OpImageFetch:
        gla::UnsupportedFunctionality("OpTextureFetch instruction");
        break;

    case spv::OpImageGather:
    case spv::OpImageDrefGather:
        gla::UnsupportedFunctionality("OpTextureGather instruction");
        break;

    case spv::OpImageQuerySizeLod:
    case spv::OpImageQuerySize:
    case spv::OpImageQueryLod:
    case spv::OpImageQueryLevels:
    case spv::OpImageQuerySamples:
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        commonMap[resultId].value = createTextureQueryCall(opCode, typeId, resultId, numOperands);
        break;

    case spv::OpSNegate:
    case spv::OpFNegate:
    case spv::OpNot:
    case spv::OpLogicalNot:
    case spv::OpAny:
    case spv::OpAll:
    case spv::OpConvertFToU:
    case spv::OpConvertFToS:
    case spv::OpConvertSToF:
    case spv::OpConvertUToF:
    case spv::OpUConvert:
    case spv::OpSConvert:
    case spv::OpFConvert:
    case spv::OpBitcast:
    case spv::OpTranspose:
    case spv::OpIsNan:
    case spv::OpIsInf:
    case spv::OpArrayLength:
    case spv::OpDPdx:
    case spv::OpDPdy:
    case spv::OpFwidth:
    case spv::OpDPdxFine:
    case spv::OpDPdyFine:
    case spv::OpFwidthFine:
    case spv::OpDPdxCoarse:
    case spv::OpDPdyCoarse:
    case spv::OpFwidthCoarse:
    {
        decodeResult(true, typeId, resultId);
        unsigned int operand = spirv[word++];
        commonMap[resultId].value = createUnaryOperation(opCode, commonMap[resultId].metaType.precision, commonMap[typeId].type, commonMap[operand].value, 
                                                         commonMap[operand].metaType.layout == gla::EMtlNone, false);
        if (commonMap[resultId].value == 0)
            gla::UnsupportedFunctionality("unary operation ", opCode);
        break;
    }

    case spv::OpEmitStreamVertex:
        glaBuilder->createIntrinsicCall(gla::EMpNone, llvm::Intrinsic::gla_emitStreamVertex, commonMap[spirv[word++]].value);
        break;
    case spv::OpEndStreamPrimitive:
        glaBuilder->createIntrinsicCall(gla::EMpNone, llvm::Intrinsic::gla_endStreamPrimitive, commonMap[spirv[word++]].value);
        break;

    case spv::OpEmitVertex:
        glaBuilder->createIntrinsicCall(llvm::Intrinsic::gla_emitVertex);
        break;
    case spv::OpEndPrimitive:
        glaBuilder->createIntrinsicCall(llvm::Intrinsic::gla_endPrimitive);
        break;
    case spv::OpControlBarrier:
        glaBuilder->createIntrinsicCall(llvm::Intrinsic::gla_barrier);
        break;
    case spv::OpMemoryBarrier:
        // TODO: handle all the different kinds
        glaBuilder->createIntrinsicCall(llvm::Intrinsic::gla_memoryBarrier);
        break;

    case spv::OpFunctionCall:
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        commonMap[resultId].value = createFunctionCall(commonMap[resultId].metaType.precision, typeId, resultId, numOperands);
        break;

    case spv::OpExtInst:
        decodeResult(true, typeId, resultId);
        numOperands -= 2;
        commonMap[resultId].value = createExternalInstruction(commonMap[resultId].metaType.precision, typeId, resultId, numOperands, findAName(resultId));
        break;

    case spv::OpLabel:
        decodeResult(false, typeId, resultId);
        makeLabelBlock(resultId);
        llvmBuilder.SetInsertPoint(commonMap[resultId].block);
        break;

    case spv::OpLoopMerge:
        // going to LunarGLASS does not require preserving structured flow control
        break;
    case spv::OpSelectionMerge:
        // going to LunarGLASS does not require preserving structured flow control
        break;
    case spv::OpBranch:
    {
        spv::Id labelId = spirv[word++];
        makeLabelBlock(labelId);
        llvmBuilder.CreateBr(commonMap[labelId].block);
        break;
    }
    case spv::OpBranchConditional:
    {
        spv::Id conditionId = spirv[word++];
        spv::Id trueLabelId = spirv[word++];
        spv::Id falseLabelId = spirv[word++];
        makeLabelBlock(trueLabelId);
        makeLabelBlock(falseLabelId);
        llvmBuilder.CreateCondBr(commonMap[conditionId].value, commonMap[trueLabelId].block, commonMap[falseLabelId].block);
        break;
    }
    case spv::OpSwitch:
        createSwitch(numOperands);
        break;
    case spv::OpKill:
        glaBuilder->makeDiscard(inEntryPoint());
        // we might be missing a termination instruction in the now current block
        if (llvmBuilder.GetInsertBlock()->getTerminator() == 0 && GetOpCode(spirv[word]) == spv::OpLabel) {
            spv::Id labelId = spirv[word + 1];
            makeLabelBlock(labelId);
            llvmBuilder.CreateBr(commonMap[labelId].block);
        }
        break;
    case spv::OpReturn:
        if (inEntryPoint())
            glaBuilder->makeMainReturn();
        else
            glaBuilder->makeReturn();
        break;
    case spv::OpReturnValue:
        glaBuilder->makeReturn(false, commonMap[spirv[word++]].value);
        break;
    default:
        gla::UnsupportedFunctionality("OpCode ", opCode);
        break;
    }
}

// Handle an OpFunction, getting the information needed to make the gla function
// from the OpTypeFunction, so not yet visiting the OpFunctionParameters that follow it
void SpvToTopTranslator::handleOpFunction(spv::Id& typeId, spv::Id& resultId)
{
    currentFunction = resultId;

    spv::FunctionControlMask controlMask = (spv::FunctionControlMask)spirv[word++];
    spv::Id functionTypeId = spirv[word++];

    // peek ahead at the first label, as we're making the entry block early
    spv::Id firstLabelId = getNextLabelId();

    if (inEntryPoint()) {
        // Make the entry point function in LLVM.
        shaderEntry = glaBuilder->makeMain();
        const char* entryName = commonMap[resultId].metaType.name;
        if (entryName == 0)
            entryName = "main";
        metadata.addMdEntrypoint(entryName);
        commonMap[currentFunction].function = shaderEntry->getParent();
        commonMap[firstLabelId].block = shaderEntry;
        llvmBuilder.SetInsertPoint(shaderEntry);
    } else {
        // The function may have already been created by a forward call.
        // If not, make it now.
        llvm::Function* function;
        const unsigned int typeInstrIndex = commonMap[functionTypeId].instructionIndex;
        const int numParams = GetWordCount(spirv[typeInstrIndex]) - 3;
        const unsigned int returnTypeIndex = typeInstrIndex + 2;
        const unsigned int param0TypeIndex = typeInstrIndex + 3;
        if (commonMap[currentFunction].function == 0) {
            // Build up the formal parameter type list
            std::vector<spv::Id> paramTypeIds;
            for (int p = 0; p < numParams; ++p)
                paramTypeIds.push_back(spirv[param0TypeIndex + p]);                
            function = makeFunction(currentFunction, typeId, paramTypeIds);
            commonMap[currentFunction].function = function;
        } else
            function = commonMap[currentFunction].function;
        commonMap[firstLabelId].block = &function->getEntryBlock();

        llvmBuilder.SetInsertPoint(&function->getBasicBlockList().front());
    }
}

llvm::Function* SpvToTopTranslator::makeFunction(spv::Id functionId, spv::Id returnTypeId, std::vector<spv::Id>& argTypeIds)
{
    llvm::Type* retType = commonMap[returnTypeId].type;
    std::vector<llvm::Type*> paramTypes;
    for (int p = 0; p < (int)argTypeIds.size(); ++p)
        paramTypes.push_back(commonMap[argTypeIds[p]].type);

    // Make the function
    llvm::BasicBlock* entryBlock;  // seems this is just needed as a flag now
    llvm::Function* function = glaBuilder->makeFunctionEntry(retType, commonMap[functionId].metaType.name, paramTypes, &entryBlock);
    function->addFnAttr(llvm::Attribute::AlwaysInline);
    
    return function;
}

// Peek ahead in the spirv until an OpLabel is found, and return its id.
spv::Id SpvToTopTranslator::getNextLabelId()
{
    unsigned searchWord = word;
    do {
        if ((spirv[searchWord] & spv::OpCodeMask) == spv::OpLabel)
            return spirv[searchWord + 1];
        searchWord += spirv[searchWord] >> spv::WordCountShift;
        assert(searchWord < spirv.size());
    } while (true);
}

// Translate a SPIR-V OpFunctionCall to a Top IR call.
llvm::Value* SpvToTopTranslator::createFunctionCall(gla::EMdPrecision precision, spv::Id typeId, spv::Id resultId, int numOperands)
{
    // get the function operand
    spv::Id functionId = spirv[word++];
    numOperands -= 1;

    // If it's a forward reference, we need to create the function based on the 
    // signature of the call.  It is supposed to be validated to an exact match
    // of the function type.
    llvm::Function* function;
    if (commonMap[functionId].value == 0) {
        std::vector<spv::Id> argTypeIds;
        for (int a = 0; a < numOperands; ++a)
            argTypeIds.push_back(commonMap[spirv[word + a]].typeId);
        function = makeFunction(functionId, typeId, argTypeIds);
        commonMap[functionId].value = function;
    } else {
        // Grab the function's pointer from the previously created function
        function = llvm::dyn_cast<llvm::Function>(commonMap[functionId].value);
    }

    if (! function)
        gla::UnsupportedFunctionality("Call to undefined function");

    // Note: All the difficult semantics for various kinds of argument passing should be
    // handled going to SPIR-V, so that now things should just be pass by copy.

    // get the calling argument operands
    llvm::SmallVector<llvm::Value*, 4> llvmArgs;
    for (int op = 0; op < numOperands; ++op) {
        spv::Id argId = spirv[word++];
        llvmArgs.push_back(commonMap[argId].value);
    }

    // Make the call
    return llvmBuilder.Insert(llvm::CallInst::Create(function, llvmArgs));
}

// Translate a SPIR-V OpExtInst to a Top IR call.
llvm::Value* SpvToTopTranslator::createExternalInstruction(gla::EMdPrecision precision, spv::Id typeId, spv::Id resultId, int numOperands, const char* name)
{
    // get the set and instruction number
    spv::Id extInstSet = spirv[word++];
    unsigned int instEnum = spirv[word++];
    numOperands -= 2;

    // get the calling argument operands
    bool firstHasSign = false;
    bool firstIsFloat = false;
    std::vector<llvm::Value*> operands;
    for (int op = 0; op < numOperands; ++op) {
        spv::Id argId = spirv[word++];
        operands.push_back(commonMap[argId].value);
        if (op == 0) {
            firstIsFloat = gla::GetBasicTypeID(operands.front()) == llvm::Type::FloatTyID;
            if (! firstIsFloat)
                firstHasSign = commonMap[argId].metaType.layout == gla::EMtlNone;
        }
    }

    llvm::Intrinsic::ID intrinsicID = llvm::Intrinsic::ID(0);
    llvm::Value* result = 0;

    switch (instEnum) {
    case spv::GLSLstd450Round:
        intrinsicID = llvm::Intrinsic::gla_fRoundFast;
        break;
    case spv::GLSLstd450RoundEven:
        intrinsicID = llvm::Intrinsic::gla_fRoundEven;
        break;
    case spv::GLSLstd450Trunc:
        intrinsicID = llvm::Intrinsic::gla_fRoundZero;
        break;
    case spv::GLSLstd450FAbs:
        intrinsicID = llvm::Intrinsic::gla_fAbs;
        break;
    case spv::GLSLstd450SAbs:
        intrinsicID = llvm::Intrinsic::gla_abs;
        break;
    case spv::GLSLstd450FSign:
        intrinsicID = llvm::Intrinsic::gla_fSign;
        break;
    case spv::GLSLstd450SSign:
        intrinsicID = llvm::Intrinsic::gla_sign;
        break;
    case spv::GLSLstd450Floor:
        intrinsicID = llvm::Intrinsic::gla_fFloor;
        break;
    case spv::GLSLstd450Ceil:
        intrinsicID = llvm::Intrinsic::gla_fCeiling;
        break;
    case spv::GLSLstd450Fract:
        intrinsicID = llvm::Intrinsic::gla_fFraction;
        break;
    case spv::GLSLstd450Radians:
        intrinsicID = llvm::Intrinsic::gla_fRadians;
        break;
    case spv::GLSLstd450Degrees:
        intrinsicID = llvm::Intrinsic::gla_fDegrees;
        break;
    case spv::GLSLstd450Sin:
        intrinsicID = llvm::Intrinsic::gla_fSin;
        break;
    case spv::GLSLstd450Cos:
        intrinsicID = llvm::Intrinsic::gla_fCos;
        break;
    case spv::GLSLstd450Tan:
        intrinsicID = llvm::Intrinsic::gla_fTan;
        break;
    case spv::GLSLstd450Asin:
        intrinsicID = llvm::Intrinsic::gla_fAsin;
        break;
    case spv::GLSLstd450Acos:
        intrinsicID = llvm::Intrinsic::gla_fAcos;
        break;
    case spv::GLSLstd450Atan:
        intrinsicID = llvm::Intrinsic::gla_fAtan;
        break;
    case spv::GLSLstd450Sinh:
        intrinsicID = llvm::Intrinsic::gla_fSinh;
        break;
    case spv::GLSLstd450Cosh:
        intrinsicID = llvm::Intrinsic::gla_fCosh;
        break;
    case spv::GLSLstd450Tanh:
        intrinsicID = llvm::Intrinsic::gla_fTanh;
        break;
    case spv::GLSLstd450Asinh:
        intrinsicID = llvm::Intrinsic::gla_fAsinh;
        break;
    case spv::GLSLstd450Acosh:
        intrinsicID = llvm::Intrinsic::gla_fAcosh;
        break;
    case spv::GLSLstd450Atanh:
        intrinsicID = llvm::Intrinsic::gla_fAtanh;
        break;
    case spv::GLSLstd450Atan2:
        intrinsicID = llvm::Intrinsic::gla_fAtan2;
        break;
    case spv::GLSLstd450Pow:
        if (firstIsFloat)
            intrinsicID = llvm::Intrinsic::gla_fPow;
        else
            intrinsicID = llvm::Intrinsic::gla_fPowi;
        break;
    case spv::GLSLstd450Exp:
        intrinsicID = llvm::Intrinsic::gla_fExp;
        break;
    case spv::GLSLstd450Log:
        intrinsicID = llvm::Intrinsic::gla_fLog;
        break;
    case spv::GLSLstd450Exp2:
        intrinsicID = llvm::Intrinsic::gla_fExp2;
        break;
    case spv::GLSLstd450Log2:
        intrinsicID = llvm::Intrinsic::gla_fLog2;
        break;
    case spv::GLSLstd450Sqrt:
        intrinsicID = llvm::Intrinsic::gla_fSqrt;
        break;
    case spv::GLSLstd450InverseSqrt:
        intrinsicID = llvm::Intrinsic::gla_fInverseSqrt;
        break;
    case spv::GLSLstd450Determinant:
        return glaBuilder->createMatrixDeterminant(precision, operands.front());
    case spv::GLSLstd450MatrixInverse:
        return glaBuilder->createMatrixInverse(precision, operands.front());
    case spv::GLSLstd450Modf:
    {
        // modf()'s second operand is only an l-value to set the 2nd return value to

        // use a unary intrinsic form to make the call and get back the returned struct
        llvm::Value* structure = glaBuilder->createIntrinsicCall(precision, llvm::Intrinsic::gla_fModF, operands.front());

        // store integer part into second operand
        llvm::Value* intPart = llvmBuilder.CreateExtractValue(structure, 1);
        llvmBuilder.CreateStore(intPart, operands[1]);

        // leave the first part as the function-call's value
        return llvmBuilder.CreateExtractValue(structure, 0);
    }
    case spv::GLSLstd450FMin:
        intrinsicID = llvm::Intrinsic::gla_fMin;
        break;
    case spv::GLSLstd450SMin:
        intrinsicID = llvm::Intrinsic::gla_sMin;
        break;
    case spv::GLSLstd450UMin:
        intrinsicID = llvm::Intrinsic::gla_uMin;
        break;
    case spv::GLSLstd450FMax:
        intrinsicID = llvm::Intrinsic::gla_fMax;
        break;
    case spv::GLSLstd450SMax:
        intrinsicID = llvm::Intrinsic::gla_sMax;
        break;
    case spv::GLSLstd450UMax:
        intrinsicID = llvm::Intrinsic::gla_uMax;
        break;
    case spv::GLSLstd450FClamp:
        intrinsicID = llvm::Intrinsic::gla_fClamp;
        break;
    case spv::GLSLstd450SClamp:
        intrinsicID = llvm::Intrinsic::gla_sClamp;
        break;
    case spv::GLSLstd450UClamp:
        intrinsicID = llvm::Intrinsic::gla_uClamp;
        break;
    case spv::GLSLstd450FMix:
        intrinsicID = llvm::Intrinsic::gla_fMix;
        break;
    case spv::GLSLstd450IMix:
        gla::UnsupportedFunctionality("integer mix");
        break;
    case spv::GLSLstd450Step:
        intrinsicID = llvm::Intrinsic::gla_fStep;
        break;
    case spv::GLSLstd450SmoothStep:
        intrinsicID = llvm::Intrinsic::gla_fSmoothStep;
        break;
    case spv::GLSLstd450Fma:
        break;
    case spv::GLSLstd450Frexp:
        break;
    case spv::GLSLstd450Ldexp:
        break;
    case spv::GLSLstd450PackSnorm4x8:
        break;
    case spv::GLSLstd450PackUnorm4x8:
        break;
    case spv::GLSLstd450PackSnorm2x16:
        break;
    case spv::GLSLstd450PackUnorm2x16:
        break;
    case spv::GLSLstd450PackHalf2x16:
        break;
    case spv::GLSLstd450PackDouble2x32:
        break;
    case spv::GLSLstd450UnpackSnorm2x16:
        break;
    case spv::GLSLstd450UnpackUnorm2x16:
        break;
    case spv::GLSLstd450UnpackHalf2x16:
        break;
    case spv::GLSLstd450UnpackSnorm4x8:
        break;
    case spv::GLSLstd450UnpackUnorm4x8:
        break;
    case spv::GLSLstd450UnpackDouble2x32:
        break;
    case spv::GLSLstd450Length:
        intrinsicID = llvm::Intrinsic::gla_fLength;
        break;
    case spv::GLSLstd450Distance:
        intrinsicID = llvm::Intrinsic::gla_fDistance;
        break;
    case spv::GLSLstd450Cross:
        intrinsicID = llvm::Intrinsic::gla_fCross;
        break;
    case spv::GLSLstd450Normalize:
        intrinsicID = llvm::Intrinsic::gla_fNormalize;
        break;
    case spv::GLSLstd450FaceForward:
        intrinsicID = llvm::Intrinsic::gla_fFaceForward;
        break;
    case spv::GLSLstd450Reflect:
        intrinsicID = llvm::Intrinsic::gla_fReflect;
        break;
    case spv::GLSLstd450Refract:
        intrinsicID = llvm::Intrinsic::gla_fRefract;
        break;
    case spv::GLSLstd450FindILsb:
        break;
    case spv::GLSLstd450FindSMsb:
        break;
    case spv::GLSLstd450FindUMsb:
        break;
    case spv::GLSLstd450InterpolateAtCentroid:
        break;
    case spv::GLSLstd450InterpolateAtSample:
        break;
    case spv::GLSLstd450InterpolateAtOffset:
        break;
    }

    // If intrinsic was assigned, then call the intrinsic and return
    if (intrinsicID != llvm::Intrinsic::ID(0)) {
        switch (operands.size()) {
        case 0:
            result = glaBuilder->createIntrinsicCall(precision, intrinsicID);
            break;
        case 1:
            result = glaBuilder->createIntrinsicCall(precision, intrinsicID, operands[0], name);
            break;
        case 2:
            result = glaBuilder->createIntrinsicCall(precision, intrinsicID, operands[0], operands[1], name ? name : "misc2a");
            break;
        case 3:
            result = glaBuilder->createIntrinsicCall(precision, intrinsicID, operands[0], operands[1], operands[2], name ? name : "misc3a");
            break;
        default:
            // These do not exist yet
            assert(0 && "intrinsic with more than 3 operands");
            break;
        }
    }

    if (result == 0)
        gla::UnsupportedFunctionality("built-in call");

    return result;
}

llvm::Value* SpvToTopTranslator::createUnaryOperation(spv::Op op, gla::EMdPrecision precision, llvm::Type* resultType, llvm::Value* operand, bool hasSign, bool reduceComparison)
{
    llvm::Instruction::CastOps castOp = llvm::Instruction::CastOps(0);
    bool isFloat = gla::GetBasicTypeID(operand) == llvm::Type::FloatTyID;
    bool comparison = false;

    llvm::Intrinsic::ID intrinsicID = llvm::Intrinsic::ID(0);
    llvm::Value* result = 0;

    switch (op) {
    case spv::OpSNegate:
        result = llvmBuilder.CreateNeg(operand);
        glaBuilder->setInstructionPrecision(result, precision);
        return result;

    case spv::OpFNegate:
        if (gla::IsAggregate(operand)) {
            // emulate by subtracting from 0.0
            llvm::Value* zero = gla::MakeFloatConstant(context, 0.0);

            return glaBuilder->createMatrixOp(precision, llvm::Instruction::FSub, zero, operand);
        }

        if (gla::GetBasicTypeID(operand) == llvm::Type::FloatTyID)
            result = llvmBuilder.CreateFNeg(operand);
        else
            result = llvmBuilder.CreateNeg (operand);
        glaBuilder->setInstructionPrecision(result, precision);
        return result;

    case spv::OpNot:
    case spv::OpLogicalNot:
        return llvmBuilder.CreateNot(operand);
    case spv::OpAny:
        intrinsicID = llvm::Intrinsic::gla_any;
        break;
    case spv::OpAll:
        intrinsicID = llvm::Intrinsic::gla_all;
        break;
    case spv::OpConvertFToU:
        castOp = llvm::Instruction::FPToUI;
        break;
    case spv::OpConvertFToS:
        castOp = llvm::Instruction::FPToSI;
        break;
    case spv::OpConvertSToF:
        castOp = llvm::Instruction::SIToFP;
        break;
    case spv::OpConvertUToF:
        castOp = llvm::Instruction::UIToFP;
        break;
    case spv::OpUConvert:
        castOp = llvm::Instruction::UIToFP;// TODO
        break;
    case spv::OpSConvert:
        // TODO
        break;
    case spv::OpFConvert:
        // TODO
        break;
    case spv::OpBitcast:
        return llvmBuilder.CreateBitCast(operand, resultType);
        break;
    case spv::OpTranspose:
        return glaBuilder->createMatrixTranspose(precision, operand);
    case spv::OpIsNan:
        intrinsicID = llvm::Intrinsic::gla_fIsNan;
        break;
    case spv::OpIsInf:
        intrinsicID = llvm::Intrinsic::gla_fIsInf;
        break;
    case spv::OpArrayLength:
        break;
    case spv::OpDPdx:
        intrinsicID = llvm::Intrinsic::gla_fDFdx;
        break;
    case spv::OpDPdy:
        intrinsicID = llvm::Intrinsic::gla_fDFdy;
        break;
    case spv::OpFwidth:
        intrinsicID = llvm::Intrinsic::gla_fFilterWidth;
        break;
    case spv::OpDPdxFine:
        // TODO
        break;
    case spv::OpDPdyFine:
        // TODO
        break;
    case spv::OpFwidthFine:
        // TODO
        break;
    case spv::OpDPdxCoarse:
        // TODO
        break;
    case spv::OpDPdyCoarse:
        // TODO
        break;
    case spv::OpFwidthCoarse:
        // TODO
        break;
    default:
        break;
    }

    // If intrinsic was assigned, then call the Top IR intrinsic and return.
    if (intrinsicID != llvm::Intrinsic::ID(0))
        return glaBuilder->createIntrinsicCall(precision, intrinsicID, operand);

    if (castOp) {
        result = llvmBuilder.CreateCast(castOp, operand, resultType);
        glaBuilder->setInstructionPrecision(result, precision);
        return result;
    }

    return 0;
}

llvm::Value* SpvToTopTranslator::createBinaryOperation(spv::Op op, gla::EMdPrecision precision, llvm::Value* left, llvm::Value* right, bool hasSign, bool reduceComparison, const char* name)
{
    llvm::Instruction::BinaryOps binOp = llvm::Instruction::BinaryOps(0);
    bool leftIsFloat = (gla::GetBasicTypeID(left) == llvm::Type::FloatTyID);
    bool comparison = false;

    llvm::Intrinsic::ID intrinsicID = llvm::Intrinsic::ID(0);

    switch (op) {
    case spv::OpFAdd:
        binOp = llvm::Instruction::FAdd;
        break;
    case spv::OpIAdd:
        binOp = llvm::Instruction::Add;
        break;
    case spv::OpFSub:
        binOp = llvm::Instruction::FSub;
        break;
    case spv::OpISub:
        binOp = llvm::Instruction::Sub;
        break;
    case spv::OpFMul:
    case spv::OpVectorTimesScalar:
        binOp = llvm::Instruction::FMul;
        break;
    case spv::OpIMul:
        binOp = llvm::Instruction::Mul;
        break;
    case spv::OpMatrixTimesScalar:
    case spv::OpVectorTimesMatrix:
    case spv::OpMatrixTimesVector:
    case spv::OpMatrixTimesMatrix:
        return glaBuilder->createMatrixMultiply(precision, left, right);

    case spv::OpDot:
        switch (gla::GetComponentCount(left)) {
        case 2:
            intrinsicID = llvm::Intrinsic::gla_fDot2;
            break;
        case 3:
            intrinsicID = llvm::Intrinsic::gla_fDot3;
            break;
        case 4:
            intrinsicID = llvm::Intrinsic::gla_fDot4;
            break;
        default:
            assert(! "bad component count for dot");
            break;
        }
        break;

    case spv::OpUDiv:
        binOp = llvm::Instruction::UDiv;
        break;
    case spv::OpSDiv:
        binOp = llvm::Instruction::SDiv;
        break;
    case spv::OpFDiv:
        binOp = llvm::Instruction::FDiv;
        break;
    case spv::OpUMod:
        binOp = llvm::Instruction::URem;
        break;
    case spv::OpSRem:
        binOp = llvm::Instruction::SRem;
        break;
    case spv::OpSMod:
        binOp = llvm::Instruction::SRem;  // TODO is this correct?
        break;
    case spv::OpFRem:
        binOp = llvm::Instruction::FRem;
        break;
    case spv::OpFMod:
        binOp = llvm::Instruction::FRem;  // TODO is this correct?
        break;
    case spv::OpShiftRightLogical:
        binOp = llvm::Instruction::LShr;
        break;
    case spv::OpShiftRightArithmetic:
        binOp = llvm::Instruction::AShr;
        break;
    case spv::OpShiftLeftLogical:
        binOp = llvm::Instruction::Shl;
        break;
    case spv::OpLogicalAnd:
    case spv::OpBitwiseAnd:
        binOp = llvm::Instruction::And;
        break;
    case spv::OpLogicalOr:
    case spv::OpBitwiseOr:
        binOp = llvm::Instruction::Or;
        break;
    case spv::OpLogicalNotEqual:
    case spv::OpBitwiseXor:
        binOp = llvm::Instruction::Xor;
        break;

    case spv::OpIEqual:
    case spv::OpFOrdEqual:
    case spv::OpFUnordEqual:
    case spv::OpINotEqual:
    case spv::OpFOrdNotEqual:
    case spv::OpFUnordNotEqual:
    case spv::OpULessThan:
    case spv::OpSLessThan:
    case spv::OpFOrdLessThan:
    case spv::OpFUnordLessThan:
    case spv::OpUGreaterThan:
    case spv::OpSGreaterThan:
    case spv::OpFOrdGreaterThan:
    case spv::OpFUnordGreaterThan:
    case spv::OpULessThanEqual:
    case spv::OpSLessThanEqual:
    case spv::OpFOrdLessThanEqual:
    case spv::OpFUnordLessThanEqual:
    case spv::OpUGreaterThanEqual:
    case spv::OpSGreaterThanEqual:
    case spv::OpFOrdGreaterThanEqual:
    case spv::OpFUnordGreaterThanEqual:
        comparison = true;
        break;
    default:
        break;
    }

    // If intrinsic was assigned, then call the Top IR intrinsic and return.
    if (intrinsicID != llvm::Intrinsic::ID(0))
        return glaBuilder->createIntrinsicCall(precision, intrinsicID, left, right, name ? name : "misc2a");

    // If binOp was assigned, then make the LLVM instruction.
    if (binOp != 0) {
        if (gla::IsAggregate(left) || gla::IsAggregate(right))
            return glaBuilder->createMatrixOp(precision, binOp, left, right);

        glaBuilder->promoteScalar(precision, left, right);
        llvm::Value* value = llvmBuilder.CreateBinOp(binOp, left, right);
        glaBuilder->setInstructionPrecision(value, precision);

        return value;
    }

    if (! comparison)
        return 0;

    // Comparison instructions

    if (reduceComparison && (gla::IsVector(left) || gla::IsAggregate(left))) {
        bool equal;
        switch (op) {
        case spv::OpIEqual:
        case spv::OpFOrdEqual:
        case spv::OpFUnordEqual:
            equal = true;
            break;
        case spv::OpINotEqual:
        case spv::OpFOrdNotEqual:
        case spv::OpFUnordNotEqual:
            equal = false;
            break;
        default:
            gla::UnsupportedFunctionality("Comparison reduction");
            break;
        }

        return glaBuilder->createCompare(precision, left, right, equal);
    }
        
    llvm::FCmpInst::Predicate fpred = llvm::FCmpInst::Predicate(0);
    llvm::ICmpInst::Predicate ipred = llvm::ICmpInst::Predicate(0);
    switch (op) {
    case spv::OpIEqual:
        ipred = llvm::ICmpInst::ICMP_EQ;
        break;
    case spv::OpFOrdEqual:
        fpred = llvm::FCmpInst::FCMP_OEQ;
        break;
    case spv::OpINotEqual:
        ipred = llvm::ICmpInst::ICMP_NE;
        break;
    case spv::OpFOrdNotEqual:
        fpred = llvm::FCmpInst::FCMP_ONE;
        break;
    case spv::OpULessThan:
        ipred = llvm::ICmpInst::ICMP_ULT;
        break;
    case spv::OpSLessThan:
        ipred = llvm::ICmpInst::ICMP_SLT;
        break;
    case spv::OpFOrdLessThan:
        fpred = llvm::FCmpInst::FCMP_OLT;
        break;
    case spv::OpUGreaterThan:
        ipred = llvm::ICmpInst::ICMP_UGT;
        break;
    case spv::OpSGreaterThan:
        ipred = llvm::ICmpInst::ICMP_SGT;
        break;
    case spv::OpFOrdGreaterThan:
        fpred = llvm::FCmpInst::FCMP_OGT;
        break;
    case spv::OpULessThanEqual:
        ipred = llvm::ICmpInst::ICMP_ULE;
        break;
    case spv::OpSLessThanEqual:
        ipred = llvm::ICmpInst::ICMP_SLE;
        break;
    case spv::OpFOrdLessThanEqual:
        fpred = llvm::FCmpInst::FCMP_OLE;
        break;
    case spv::OpUGreaterThanEqual:
        ipred = llvm::ICmpInst::ICMP_UGE;
        break;
    case spv::OpSGreaterThanEqual:
        ipred = llvm::ICmpInst::ICMP_SGE;
        break;
    case spv::OpFOrdGreaterThanEqual:
        fpred = llvm::FCmpInst::FCMP_OGE;
        break;

    case spv::OpFUnordEqual:
    case spv::OpFUnordNotEqual:
    case spv::OpFUnordLessThan:
    case spv::OpFUnordGreaterThan:
    case spv::OpFUnordLessThanEqual:
    case spv::OpFUnordGreaterThanEqual:
        gla::UnsupportedFunctionality("Unordered compare");
        break;
    }

    if (fpred) {
        llvm::Value* result = llvmBuilder.CreateFCmp(fpred, left, right);
        glaBuilder->setInstructionPrecision(result, precision);

        return result;
    } else if (ipred != 0) {
        llvm::Value* result = llvmBuilder.CreateICmp(ipred, left, right);
        glaBuilder->setInstructionPrecision(result, precision);

        return result;
    }

    return 0;
}

// Change from a SPIR-V sampler type to a LunarGLASS sampler type
gla::ESamplerType SpvToTopTranslator::convertImageType(spv::Id imageTypeId)
{
    switch (getImageDim(imageTypeId)) {
    case spv::Dim1D:
        return gla::ESampler1D;
    case spv::Dim2D:
        if (isImageMS(imageTypeId))
            return gla::ESampler2DMS;
        else
            return gla::ESampler2D;
    case spv::Dim3D:
        return gla::ESampler3D;
    case spv::DimCube:
        return gla::ESamplerCube;
    case spv::DimRect:
        return gla::ESampler2DRect;
    case spv::DimBuffer:        
        return gla::ESamplerBuffer;
    default:
        gla::UnsupportedFunctionality("SPIR-V sampler dimensionality");
        return gla::ESampler2D;
    }
}

// Turn a SPIR-V OpTextureSample* instruction into gla texturing intrinsic.
llvm::Value* SpvToTopTranslator::createSamplingCall(spv::Op opcode, spv::Id typeId, spv::Id resultId, int numOperands)
{
    // First, decode the instruction

    // invariant fixed operands, sampled-image and coord
    spv::Id sampledImageId = spirv[word++];
    spv::Id coord = spirv[word++];
    numOperands -= 2;

    // Dref fixed operand
    spv::Id dref = BadValue;
    switch (opcode) {
        case spv::OpImageSampleDrefImplicitLod:
        case spv::OpImageSampleDrefExplicitLod:
        case spv::OpImageSampleProjDrefImplicitLod:
        case spv::OpImageSampleProjDrefExplicitLod:
            dref = spirv[word++];
            --numOperands;
            break;
        default:
            break;
    }

    // optional SPV image operands
    spv::Id bias = BadValue;
    spv::Id lod = BadValue;
    spv::Id gradx = BadValue;
    spv::Id grady = BadValue;
    spv::Id offset = BadValue;
    spv::Id offsets = BadValue;
    spv::Id sample = BadValue;
    if (numOperands > 0) {
        spv::ImageOperandsMask imageOperands = (spv::ImageOperandsMask)spirv[word++];
        if (imageOperands & spv::ImageOperandsBiasMask)
            bias = spirv[word++];
        if (imageOperands & spv::ImageOperandsLodMask)
            lod = spirv[word++];
        if (imageOperands & spv::ImageOperandsGradMask) {
            gradx = spirv[word++];
            grady = spirv[word++];
        }
        if (imageOperands & spv::ImageOperandsOffsetMask ||
            imageOperands & spv::ImageOperandsConstOffsetMask)
            offset = spirv[word++];
        if (imageOperands & spv::ImageOperandsConstOffsetsMask)
            offsets = spirv[word++];
        if (imageOperands & spv::ImageOperandsSampleMask)
            sample = spirv[word++];
    }

    // Second, set up information to set up to pass to the builder:

    // bias
    int flags = 0;
    gla::Builder::TextureParameters parameters = {};
    if (bias != BadValue) {
        flags |= gla::ETFBias;
        parameters.ETPBiasLod = commonMap[bias].value;
    }

    // sampler and gla dimensionality
    spv::Id sampledImageTypeId = commonMap[sampledImageId].typeId;
    gla::ESamplerType glaSamplerType = convertImageType(getImageTypeId(commonMap[sampledImageId].typeId));
    parameters.ETPSampler = commonMap[sampledImageId].value;

    // coord
    parameters.ETPCoords = commonMap[coord].value;

    // lod
    if (lod != BadValue) {
        flags |= gla::ETFBiasLodArg;
        flags |= gla::ETFLod;
        parameters.ETPBiasLod = commonMap[lod].value;
    }

    // offset
    if (offset != BadValue) {
        flags |= gla::ETFOffsetArg;
        parameters.ETPOffset = commonMap[offset].value;
    }

    // offsets
    if (offsets != BadValue)
        gla::UnsupportedFunctionality("image offsets");

    // gradient
    if (gradx != BadValue) {
        parameters.ETPGradX = commonMap[gradx].value;
        parameters.ETPGradY = commonMap[grady].value;
    }

    // proj
    switch (opcode) {
        case spv::OpImageSampleProjImplicitLod:
        case spv::OpImageSampleProjExplicitLod:
        case spv::OpImageSampleProjDrefImplicitLod:
        case spv::OpImageSampleProjDrefExplicitLod:
            flags |= gla::ETFProjected;
            break;
        default:
            break;
    }

    // Dref
    if (dref != BadValue) {
        // Note: for most cases, this needs to be put back with the coordinate,
        // which for the glslang front end already holds it.
        // TODO: do this correctly, in the general case.
        
        if (false /*! cube-arrayed*/) {
            flags |= gla::ETFRefZArg;
            parameters.ETPShadowRef = commonMap[dref].value;
        }
    }

    return glaBuilder->createTextureCall(commonMap[resultId].metaType.precision, commonMap[typeId].type, glaSamplerType, flags, parameters, findAName(resultId));
}

// Turn a SPIR-V OpTextureQuery* instruction into gla texturing intrinsic.
llvm::Value* SpvToTopTranslator::createTextureQueryCall(spv::Op opcode, spv::Id typeId, spv::Id resultId, int numOperands)
{
    // first operand is always the sampler
    spv::Id image = spirv[word++];

    // sometimes there is another operand, either lod or coords
    spv::Id extraArg = 0;
    if (numOperands > 1)
        extraArg = spirv[word++];

    llvm::Intrinsic::ID intrinsicID;
    switch (opcode) {
    case spv::OpImageQuerySizeLod:
        intrinsicID = llvm::Intrinsic::gla_queryTextureSize;
        break;
    case spv::OpImageQuerySize:
        intrinsicID = llvm::Intrinsic::gla_queryTextureSizeNoLod;
        break;
    case spv::OpImageQueryLod:
        intrinsicID = llvm::Intrinsic::gla_fQueryTextureLod;
        break;
    case spv::OpImageQueryLevels:
    case spv::OpImageQuerySamples:
    default:
        gla::UnsupportedFunctionality("SPIR-V texture query op");
        break;
    }

    gla::ESamplerType glaSamplerType = convertImageType(commonMap[image].typeId);

    return glaBuilder->createTextureQueryCall(gla::EMpNone, intrinsicID, commonMap[typeId].type, gla::MakeIntConstant(context, glaSamplerType),
                                              commonMap[image].value, commonMap[extraArg].value, findAName(resultId));
}

// OpCompositeConstruct
llvm::Value* SpvToTopTranslator::createConstructor(spv::Id resultId, spv::Id typeId, std::vector<llvm::Value*> constituents)
{
    llvm::Value* result;

    if (gla::IsVector(commonMap[typeId].type)) {
        // handle vectors differently, as they don't have a 1:1 mapping between constituents and components
        result = llvm::UndefValue::get(commonMap[typeId].type);
        result = glaBuilder->createConstructor(commonMap[resultId].metaType.precision, constituents, result);
    } else {
        // everything else (matrices, arrays, structs) should have a 1:1 mapping between constituents and components
        result = llvm::UndefValue::get(commonMap[typeId].type);
        for (unsigned i = 0; i < constituents.size(); ++i)
            result = llvmBuilder.CreateInsertValue(result, constituents[i], i);
    }

    return result;
}

spv::Op SpvToTopTranslator::getOpCode(spv::Id id) const
{
    return GetOpCode(spirv[commonMap[id].instructionIndex]);
}

spv::Id SpvToTopTranslator::dereferenceTypeId(spv::Id typeId) const
{
    // Look back at the OpTypePointer instruction
    const int OpTypePointerIdOffset = 3;
    return spirv[commonMap[typeId].instructionIndex + OpTypePointerIdOffset];
}

spv::Id SpvToTopTranslator::getArrayElementTypeId(spv::Id typeId) const
{
    // Look back at the OpType* instruction
    const int OpTypeArrayElementTypeOffset = 2;
    while (getOpCode(typeId) == spv::OpTypeArray)
        typeId = spirv[commonMap[typeId].instructionIndex + OpTypeArrayElementTypeOffset];

    return typeId;
}

spv::Id SpvToTopTranslator::getStructMemberTypeId(spv::Id typeId, int member) const
{
    // Look back at the OpType* instruction
    const int OpTypeStructMember0Offset = 2;
    return spirv[commonMap[typeId].instructionIndex + OpTypeStructMember0Offset + member];
}

spv::Id SpvToTopTranslator::getImageTypeId(spv::Id sampledImageTypeId) const
{
    return spirv[commonMap[sampledImageTypeId].instructionIndex + 2];
}

// Lookup the 'Sampled Type' operand in the image type
spv::Id SpvToTopTranslator::getImageSampledType(spv::Id typeId) const
{
    if (commonMap[typeId].metaType.combinedImageSampler)
        typeId = getImageTypeId(typeId);

    return spirv[commonMap[typeId].instructionIndex + 2];
}

// Lookup the 'Dim' operand in the image type
spv::Dim SpvToTopTranslator::getImageDim(spv::Id typeId) const
{
    if (commonMap[typeId].metaType.combinedImageSampler)
        typeId = getImageTypeId(typeId);

    return (spv::Dim)spirv[commonMap[typeId].instructionIndex + 3];
}

// Lookup the 'depth' operand in the image type
bool SpvToTopTranslator::isImageDepth(spv::Id typeId) const
{
    if (commonMap[typeId].metaType.combinedImageSampler)
        typeId = getImageTypeId(typeId);

    return spirv[commonMap[typeId].instructionIndex + 4] != 0;
}

// Lookup the 'arrayed' operand in the image type
bool SpvToTopTranslator::isImageArrayed(spv::Id typeId) const
{
    if (commonMap[typeId].metaType.combinedImageSampler)
        typeId = getImageTypeId(typeId);

    return spirv[commonMap[typeId].instructionIndex + 5] != 0;
}

// Lookup the 'Dim' operand in the image type
bool SpvToTopTranslator::isImageMS(spv::Id typeId) const
{
    if (commonMap[typeId].metaType.combinedImageSampler)
        typeId = getImageTypeId(typeId);

    return (spv::Dim)(spirv[commonMap[typeId].instructionIndex + 6] != 0);
}

bool SpvToTopTranslator::inEntryPoint()
{
    return commonMap[currentFunction].entryPoint != BadValue;
}

void SpvToTopTranslator::makeLabelBlock(spv::Id labelId)
{
    if (commonMap[labelId].block == 0)
        commonMap[labelId].block = llvm::BasicBlock::Create(context, "L", commonMap[currentFunction].function);
}

void SpvToTopTranslator::createSwitch(int numOperands)
{
    spv::Id selectorId = spirv[word++];
    spv::Id defaultId = spirv[word++];
    numOperands -= 2;

    assert((numOperands & 1) == 0);
    int numSegments = numOperands / 2;

    makeLabelBlock(defaultId);
    llvm::SwitchInst* switchInst = llvmBuilder.CreateSwitch(commonMap[selectorId].value, commonMap[defaultId].block, numSegments);
    for (int c = 0; c < numSegments; ++c) {
        unsigned int value = spirv[word++];
        spv::Id labelId = spirv[word++];
        makeLabelBlock(labelId);
        switchInst->addCase(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), value), commonMap[labelId].block);
    }
}

};  // end anonymous namespace

namespace gla {

// Translate SPIR-V to LunarGLASS Top IR
void SpvToTop(const std::vector<unsigned int>& spirv, gla::Manager& manager)
{
    manager.createContext();
    llvm::Module* topModule = new llvm::Module("SPIR-V", manager.getContext());
    manager.setModule(topModule);

    SpvToTopTranslator translator(spirv, manager);
    translator.makeTop();
}

}; // end gla namespace
