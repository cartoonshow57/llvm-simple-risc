//===--- CGP1.h - Declare CGP1 target feature support -----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares CGP1 TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_CGP1_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_CGP1_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"

namespace clang {
namespace targets {

// CGP1 GPU Target (32-bit big endian)
class LLVM_LIBRARY_VISIBILITY CGP1TargetInfo : public TargetInfo {
  static const char *const GCCRegNames[];

public:
  CGP1TargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple) {
    // CGP1 GPU Data Layout:
    // E = big endian
    // m:m = name mangling
    // p:32:32 = default (private) pointers are 32-bit
    // p1:32:32 = global memory pointers (address space 1)
    // p3:32:32 = shared/local memory pointers (address space 3)
    // p4:32:32 = constant memory pointers (address space 4)
    // i8:8:32 = i8 is 8-bit with 32-bit alignment
    // i16:16:32 = i16 is 16-bit with 32-bit alignment
    // i64:64 = i64 is 64-bit aligned
    // n32 = only 32-bit native integers
    // S64 = 64-bit natural stack alignment
    resetDataLayout("E-m:m-p:32:32-p1:32:32-p3:32:32-p4:32:32"
                    "-i8:8:32-i16:16:32-i64:64-n32-S64");

    // 32-bit pointers
    PointerWidth = PointerAlign = 32;
    IntWidth = IntAlign = 32;
    LongWidth = LongAlign = 32;
    LongLongWidth = LongLongAlign = 64;

    // Size types
    SizeType = UnsignedInt;
    PtrDiffType = SignedInt;
    IntPtrType = SignedInt;

    // Register passing
    RegParmMax = 4;

    // Use standard va_list
    UseAddrSpaceMapMangling = true;
  }

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  bool isValidCPUName(StringRef Name) const override;

  void fillValidCPUList(SmallVectorImpl<StringRef> &Values) const override;

  bool setCPU(const std::string &Name) override;

  bool hasFeature(StringRef Feature) const override;

  ArrayRef<const char *> getGCCRegNames() const override;

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override {
    return std::nullopt;
  }

  BuiltinVaListKind getBuiltinVaListKind() const override {
    return TargetInfo::VoidPtrBuiltinVaList;
  }

  llvm::SmallVector<Builtin::InfosShard> getTargetBuiltins() const override;

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &info) const override {
    return false;
  }

  std::string_view getClobbers() const override { return ""; }

  bool hasBitIntType() const override { return true; }

  // GPU address space mapping
  LangAS getOpenCLTypeAddrSpace(OpenCLTypeKind TK) const override {
    switch (TK) {
    case OCLTK_Default:
      return LangAS::opencl_generic;
    case OCLTK_Image:
    case OCLTK_Pipe:
      return LangAS::opencl_global;
    default:
      return TargetInfo::getOpenCLTypeAddrSpace(TK);
    }
  }
};

} // namespace targets
} // namespace clang

#endif // LLVM_CLANG_LIB_BASIC_TARGETS_CGP1_H

