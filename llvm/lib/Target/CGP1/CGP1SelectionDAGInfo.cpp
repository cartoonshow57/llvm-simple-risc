//===-- CGP1SelectionDAGInfo.cpp - CGP1 SelectionDAG Info -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the CGP1SelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#include "CGP1SelectionDAGInfo.h"

#define GET_SDNODE_DESC
#include "CGP1GenSDNodeInfo.inc"

#define DEBUG_TYPE "cgp1-selectiondag-info"

using namespace llvm;

CGP1SelectionDAGInfo::CGP1SelectionDAGInfo()
    : SelectionDAGGenTargetInfo(CGP1GenSDNodeInfo) {}

SDValue CGP1SelectionDAGInfo::EmitTargetCodeForMemcpy(
    SelectionDAG & /*DAG*/, const SDLoc & /*dl*/, SDValue /*Chain*/,
    SDValue /*Dst*/, SDValue /*Src*/, SDValue Size, Align /*Alignment*/,
    bool /*isVolatile*/, bool /*AlwaysInline*/,
    MachinePointerInfo /*DstPtrInfo*/,
    MachinePointerInfo /*SrcPtrInfo*/) const {
  ConstantSDNode *ConstantSize = dyn_cast<ConstantSDNode>(Size);
  if (!ConstantSize)
    return SDValue();

  return SDValue();
}
