//===- TopToBottom.cpp - Translate Top IR to Bottom IR --------------------===//
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
// Translate Top IR to Bottom IR
//
//===----------------------------------------------------------------------===//

// LLVM includes
#pragma warning(push, 1)
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#pragma warning(pop)

// LunarGLASS includes
#include "Exceptions.h"
#include "Backend.h"
#include "PrivateManager.h"
#include "Options.h"

// LunarGLASS Passes
#include "Passes/Passes.h"


void gla::PrivateManager::translateTopToBottom()
{
#ifdef _WIN32
    unsigned int oldFormat = _set_output_format(_TWO_DIGIT_EXPONENT);
#endif

    runLLVMOptimizations1();

#ifdef _WIN32
    _set_output_format(oldFormat);
#endif

    int innerAoS, outerSoA;
    backEnd->getRegisterForm(outerSoA, innerAoS);
    if (outerSoA != 1)
        UnsupportedFunctionality("SoA in middle end: ", outerSoA);
    if (innerAoS != 4 && innerAoS != 1)
        UnsupportedFunctionality("AoS other than size 4 or 1 in middle end: ", innerAoS);
}

void gla::PrivateManager::dump(const char *heading)
{
    llvm::errs() << heading;
    module->dump();
}

static inline void RunOnModule(llvm::FunctionPassManager& pm, llvm::Module* m)
{
    pm.doInitialization();
    for (llvm::Module::iterator f = m->begin(), e = m->end(); f != e; ++f)
        pm.run(*f);
    pm.doFinalization();
}

static bool HasControlFlow(llvm::Module* m)
{
    for (llvm::Module::iterator f = m->begin(), e = m->end(); f != e; ++f) {
        for (llvm::Function::iterator b = f->begin(), be = f->end(); b != be; ++b) {
            if (b->getTerminator()->getNumSuccessors() > 1) {
                return true;
            }
        }
    }
    return false;
}

// Verify each function
static inline void VerifyModule(llvm::Module* module)
{
#ifndef NDEBUG

    llvm::FunctionPassManager verifier(module);
    verifier.add(llvm::createVerifierPass());
    RunOnModule(verifier, module);

#endif // NDEBUG
}

void gla::PrivateManager::runLLVMOptimizations1()
{
    VerifyModule(module);

    // TODO: generate code performance: When we have backend support for shuffles, or we canonicalize
    // shuffles into multiinserts, we can replace the InstSimplify passes with
    // InstCombine passes.

    // First, do some global (module-level) optimizations, which can free up
    // function passes to do more.
    llvm::PassManager globalPM;
    globalPM.add(llvm::createGlobalOptimizerPass());
    globalPM.add(llvm::createIPSCCPPass());
    globalPM.add(llvm::createConstantMergePass());
    globalPM.add(llvm::createInstructionSimplifierPass());
    if (options.optimizations.inlineThreshold)
        globalPM.add(llvm::createAlwaysInlinerPass());
    globalPM.add(llvm::createPromoteMemoryToRegisterPass());
    globalPM.run(*module);

    // Next, do interprocedural passes
    // TODO: generated code performance: If we ever have non-inlined functions, we'll want to add some interprocedural passes

    VerifyModule(module);

    // Set up the function-level optimizations we want
    // TODO: generated code performance: explore ordering of passes more
    llvm::FunctionPassManager passManager(module);


    // Add target data to unblock optimizations that require it
    // This matches default except for endianness (little) and pointer size/alignment (32)
    llvm::DataLayout* DL = new llvm::DataLayout("e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64");
    passManager.add(DL);

    // Create immutable passes once
    passManager.add(llvm::createBasicAliasAnalysisPass());
    passManager.add(llvm::createTypeBasedAliasAnalysisPass());

    // Provide the backend queries
    passManager.add(gla_llvm::createBackEndPointerPass(backEnd));

    // TODO: explore SimplifyLibCalls
    // TODO: compile-time performance: see if we can avoid running gvn/sccp multiple times

    // Early, simple optimizations to enable others/make others more efficient
    //passManager.add(llvm::createScalarReplAggregatesPass());
    passManager.add(llvm::createInstructionCombiningPass());
    passManager.add(llvm::createEarlyCSEPass());
    passManager.add(llvm::createCorrelatedValuePropagationPass());

    bool hasCf = HasControlFlow(module);

    if (hasCf) {
        passManager.add(llvm::createCFGSimplificationPass());
        passManager.add(llvm::createLoopSimplifyPass());
        passManager.add(gla_llvm::createCanonicalizeCFGPass());
        passManager.add(gla_llvm::createDecomposeInstsPass());
        passManager.add(gla_llvm::createCanonicalizeCFGPass());

        // TODO: Compile-time performance: something goes stale in FlattenConditionalAssignments (dom trees?).
        //       Running it multiple times here catches more, whereas running it multiple times internally does not help.
        //       Once that's fixed, most at this level can be eliminated.
        passManager.add(gla_llvm::createFlattenConditionalAssignmentsPass(options.optimizations.flattenHoistThreshold));
        passManager.add(gla_llvm::createFlattenConditionalAssignmentsPass(options.optimizations.flattenHoistThreshold));
        passManager.add(gla_llvm::createFlattenConditionalAssignmentsPass(options.optimizations.flattenHoistThreshold));

        passManager.add(gla_llvm::createCanonicalizeCFGPass());
    } else        
        passManager.add(gla_llvm::createDecomposeInstsPass());

    int innerAoS, outerSoA;
    backEnd->getRegisterForm(outerSoA, innerAoS);
    if (innerAoS == 1) {
        passManager.add(gla_llvm::createScalarizePass());
    }

    if (options.optimizations.reassociate)
        passManager.add(llvm::createReassociatePass());
    passManager.add(llvm::createInstructionCombiningPass());

    //if (options.optimizations.gvn)
    //    passManager.add(llvm::createGVNPass());
    passManager.add(llvm::createSCCPPass());

    if (hasCf) {
        passManager.add(llvm::createLoopSimplifyPass());
        passManager.add(gla_llvm::createCanonicalizeCFGPass());
        passManager.add(gla_llvm::createFlattenConditionalAssignmentsPass(options.optimizations.flattenHoistThreshold));
        passManager.add(gla_llvm::createFlattenConditionalAssignmentsPass(options.optimizations.flattenHoistThreshold));
        passManager.add(gla_llvm::createCanonicalizeCFGPass());
    }

    // Make multiinsert intrinsics, and clean up afterwards
    passManager.add(llvm::createInstructionCombiningPass());
    if (options.optimizations.coalesce)
        passManager.add(gla_llvm::createCoalesceInsertsPass());
    if (options.optimizations.adce)
        passManager.add(llvm::createAggressiveDCEPass());
    passManager.add(llvm::createInstructionCombiningPass());

    if (hasCf) {
        // Loop optimizations, and clean up afterwards
        passManager.add(llvm::createLICMPass());
        passManager.add(llvm::createIndVarSimplifyPass());
        if (options.optimizations.loopUnrollThreshold) {
            // Loop rotation creates a less desirable loop form for loops that do not get unrolled,
            // but is needed if a loop will be unrolled.
            passManager.add(llvm::createLoopRotatePass(options.optimizations.loopUnrollThreshold));
            passManager.add(llvm::createIndVarSimplifyPass());
            passManager.add(llvm::createLoopUnrollPass(options.optimizations.loopUnrollThreshold));
        }
        passManager.add(llvm::createLoopStrengthReducePass());
        if (options.optimizations.adce)
            passManager.add(llvm::createAggressiveDCEPass());

        passManager.add(llvm::createInstructionCombiningPass());
        //if (options.optimizations.gvn)
        //    passManager.add(llvm::createGVNPass());
        passManager.add(llvm::createSCCPPass());
    }

    // Run intrinisic combining
    passManager.add(gla_llvm::createCanonicalizeCFGPass());
    passManager.add(llvm::createInstructionCombiningPass());
    passManager.add(gla_llvm::createIntrinsicCombinePass());
    passManager.add(gla_llvm::createCanonicalizeCFGPass());

    //if (options.optimizations.gvn)
    //    passManager.add(llvm::createGVNPass());
    passManager.add(llvm::createSCCPPass());

    // TODO: generated code: Consider if we really want it. For some reason StandardPasses.h
    // doesn't have it listed.
    // passManager.add(llvm::createSinkingPass());

    // Run some post-redundancy-elimination passes
    //passManager.add(llvm::createScalarReplAggregatesPass());
    passManager.add(llvm::createInstructionCombiningPass());
    passManager.add(llvm::createCorrelatedValuePropagationPass());
    if (options.optimizations.adce)
        passManager.add(llvm::createAggressiveDCEPass());

    if (hasCf) {
        // LunarGLASS CFG optimizations
        passManager.add(llvm::createLoopSimplifyPass());
        passManager.add(gla_llvm::createCanonicalizeCFGPass());
        passManager.add(gla_llvm::createFlattenConditionalAssignmentsPass(options.optimizations.flattenHoistThreshold));
        passManager.add(gla_llvm::createCanonicalizeCFGPass());

        passManager.add(llvm::createInstructionCombiningPass());
        if (options.optimizations.adce)
            passManager.add(llvm::createAggressiveDCEPass());
    }

    RunOnModule(passManager, module);

    VerifyModule(module);

    // Post Function passes cleanup
    llvm::PassManager pm;
    pm.add(llvm::createInstructionCombiningPass());
    pm.add(llvm::createDeadStoreEliminationPass());
    if (options.optimizations.adce)
        pm.add(llvm::createAggressiveDCEPass());
    pm.add(llvm::createStripDeadPrototypesPass());
    
    // TODO: function-call functionality: Consider using the below in the presense of functions
    // pm.add(llvm::createGlobalDCEPass());

    pm.run(*module);

    VerifyModule(module);

    // TODO: Refactor the below use of GlobalOpt. Perhaps we want to repeat our
    // some function passes?

    llvm::PassManager modulePassManager;
    modulePassManager.add(llvm::createGlobalOptimizerPass());

    // Optimize the whole module
    bool changed = modulePassManager.run(*module);

    if (changed) {
        // removing globals created stack allocations we want to eliminate
        llvm::FunctionPassManager postGlobalManager(module);
        postGlobalManager.add(llvm::createPromoteMemoryToRegisterPass());

        // run across all functions
        postGlobalManager.doInitialization();
        for (llvm::Module::iterator function = module->begin(), lastFunction = module->end(); function != lastFunction; ++function) {
            postGlobalManager.run(*function);
        }
        postGlobalManager.doFinalization();
    }

    if (! backEnd->preferRegistersOverMemory()) {
        llvm::FunctionPassManager memoryPassManager(module);
        memoryPassManager.add(llvm::createDemoteRegisterToMemoryPass());

        memoryPassManager.doInitialization();
        for (llvm::Module::iterator function = module->begin(), lastFunction = module->end(); function != lastFunction; ++function) {
            memoryPassManager.run(*function);
        }
        memoryPassManager.doFinalization();
    }

    VerifyModule(module);

    // Put the IR into a canonical form for BottomTranslator.
    llvm::PassManager canonicalize;

    canonicalize.add(llvm::createIndVarSimplifyPass());
    canonicalize.add(gla_llvm::createCanonicalizeCFGPass());
    canonicalize.add(gla_llvm::createBackEndPointerPass(backEnd));
    canonicalize.add(gla_llvm::createGatherInstsPass());
    canonicalize.add(gla_llvm::createCanonicalizeInstsPass());
    canonicalize.add(llvm::createStripDeadPrototypesPass());
    canonicalize.run(*module);

    VerifyModule(module);
}
