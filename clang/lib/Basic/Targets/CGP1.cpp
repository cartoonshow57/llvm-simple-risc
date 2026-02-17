//===--- CGP1.cpp - Implement CGP1 target feature support -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements CGP1 TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "CGP1.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/MacroBuilder.h"
#include "clang/Basic/TargetBuiltins.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;

static constexpr int NumBuiltins =
    CGP1::LastTSBuiltin - Builtin::FirstTSBuiltin;

static constexpr llvm::StringTable BuiltinStrings =
    CLANG_BUILTIN_STR_TABLE_START
#define BUILTIN CLANG_BUILTIN_STR_TABLE
#include "clang/Basic/BuiltinsCGP1.def"
    ;

static constexpr auto BuiltinInfos = Builtin::MakeInfos<NumBuiltins>({
#define BUILTIN CLANG_BUILTIN_ENTRY
#include "clang/Basic/BuiltinsCGP1.def"
});

llvm::SmallVector<Builtin::InfosShard>
CGP1TargetInfo::getTargetBuiltins() const {
  return {{&BuiltinStrings, BuiltinInfos}};
}

// CGP1 register names (32 general-purpose registers)
const char *const CGP1TargetInfo::GCCRegNames[] = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0",   "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0",   "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8",   "t9", "k0", "k1", "gp", "sp", "fp", "ra"
};

ArrayRef<const char *> CGP1TargetInfo::getGCCRegNames() const {
  return llvm::ArrayRef(GCCRegNames);
}

bool CGP1TargetInfo::isValidCPUName(StringRef Name) const {
  return llvm::StringSwitch<bool>(Name)
      .Case("cgp1", true)
      .Case("generic", true)
      .Default(false);
}

void CGP1TargetInfo::fillValidCPUList(
    SmallVectorImpl<StringRef> &Values) const {
  Values.emplace_back("cgp1");
  Values.emplace_back("generic");
}

bool CGP1TargetInfo::setCPU(const std::string &Name) {
  return isValidCPUName(Name);
}

bool CGP1TargetInfo::hasFeature(StringRef Feature) const {
  return llvm::StringSwitch<bool>(Feature)
      .Case("cgp1", true)
      .Case("gpu", true)
      .Default(false);
}

void CGP1TargetInfo::getTargetDefines(const LangOptions &Opts,
                                      MacroBuilder &Builder) const {
  // Define CGP1-specific macros
  Builder.defineMacro("__cgp1__");
  Builder.defineMacro("__CGP1__");
  
  // GPU-related defines
  Builder.defineMacro("__CGP1_GPU__");
  
  // Architecture info
  Builder.defineMacro("__CGP1_ARCH__", "1");
  
  // Pointer size
  Builder.defineMacro("__SIZEOF_POINTER__", "4");
  
  // Endianness
  Builder.defineMacro("__BIG_ENDIAN__");
  Builder.defineMacro("_BIG_ENDIAN");
}

