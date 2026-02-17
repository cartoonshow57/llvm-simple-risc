//@s cgp1-isel-lowering-cpp-init end

//===- CGP1IselLowering.cpp - CGP1 DAG Lowering Implementation -----------===//
#include "CGP1ISelLowering.h"
#include "MCTargetDesc/CGP1MCTargetDesc.h"
#include "CGP1Subtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/IntrinsicsCGP1.h"

using namespace llvm;

#define DEBUG_TYPE "cgp1-isel"
//- cgp1-isel-lowering-cpp-init

//@s cgp1-isel-lowering-cpp-1 end
CGP1TargetLowering::CGP1TargetLowering(const TargetMachine &TM,
                                       const CGP1Subtarget &STI)
    : TargetLowering(TM) {
  // Register integer register class
  addRegisterClass(MVT::i32, &CGP1::GPRRegClass);
  
  // Register floating-point register class (single precision)
  addRegisterClass(MVT::f32, &CGP1::GPRRegClass);

  // Set up the register for stack pointer
  setStackPointerRegisterToSaveRestore(CGP1::R13);

  // Boolean results are i32
  setBooleanContents(ZeroOrOneBooleanContent);

  // We only support i32 natively. All larger integer types must be expanded.
  for (MVT VT : {MVT::i64, MVT::i128}) {
    setOperationAction(ISD::ADD, VT, Expand);
    setOperationAction(ISD::SUB, VT, Expand);
    setOperationAction(ISD::MUL, VT, Expand);
    setOperationAction(ISD::SDIV, VT, Expand);
    setOperationAction(ISD::UDIV, VT, Expand);
    setOperationAction(ISD::SREM, VT, Expand);
    setOperationAction(ISD::UREM, VT, Expand);
    setOperationAction(ISD::AND, VT, Expand);
    setOperationAction(ISD::OR, VT, Expand);
    setOperationAction(ISD::XOR, VT, Expand);
    setOperationAction(ISD::SHL, VT, Expand);
    setOperationAction(ISD::SRL, VT, Expand);
    setOperationAction(ISD::SRA, VT, Expand);
    setOperationAction(ISD::ROTL, VT, Expand);
    setOperationAction(ISD::ROTR, VT, Expand);
    setOperationAction(ISD::CTLZ, VT, Expand);
    setOperationAction(ISD::CTTZ, VT, Expand);
    setOperationAction(ISD::CTPOP, VT, Expand);
    setOperationAction(ISD::SELECT, VT, Expand);
    setOperationAction(ISD::SELECT_CC, VT, Expand);
    setOperationAction(ISD::SETCC, VT, Expand);
    // Type conversions - use custom lowering for i32, expand for others
    if (VT == MVT::i32) {
      setOperationAction(ISD::SIGN_EXTEND, VT, Custom);
      setOperationAction(ISD::ZERO_EXTEND, VT, Custom);
      setOperationAction(ISD::ANY_EXTEND, VT, Custom);
      setOperationAction(ISD::TRUNCATE, VT, Custom);
      setOperationAction(ISD::SIGN_EXTEND_INREG, VT, Custom);
    } else {
      setOperationAction(ISD::SIGN_EXTEND, VT, Expand);
      setOperationAction(ISD::ZERO_EXTEND, VT, Expand);
      setOperationAction(ISD::ANY_EXTEND, VT, Expand);
      setOperationAction(ISD::TRUNCATE, VT, Expand);
      setOperationAction(ISD::SIGN_EXTEND_INREG, VT, Expand);
    }
  }

  // i32 operations that are natively supported
  setOperationAction(ISD::MUL, MVT::i32, Legal);
  setOperationAction(ISD::SDIV, MVT::i32, Legal);
  setOperationAction(ISD::UDIV, MVT::i32, Legal);
  setOperationAction(ISD::SREM, MVT::i32, Legal);
  setOperationAction(ISD::UREM, MVT::i32, Legal);

  // i32 operations that need custom lowering
  setOperationAction(ISD::ROTL, MVT::i32, Custom);
  setOperationAction(ISD::ROTR, MVT::i32, Custom);
  // Bit counting operations - let LLVM expand (complex to implement efficiently)
  setOperationAction(ISD::CTLZ, MVT::i32, Expand);
  setOperationAction(ISD::CTTZ, MVT::i32, Expand);
  setOperationAction(ISD::CTPOP, MVT::i32, Expand);
  // Multiply high operations - need 64-bit support, let LLVM expand
  setOperationAction(ISD::MULHS, MVT::i32, Expand);
  setOperationAction(ISD::MULHU, MVT::i32, Expand);
  setOperationAction(ISD::SMUL_LOHI, MVT::i32, Expand);
  setOperationAction(ISD::UMUL_LOHI, MVT::i32, Expand);

  // SELECT_CC expands to SELECT + SETCC
  setOperationAction(ISD::SELECT_CC, MVT::i32, Expand);
  
  // BR_CC needs custom lowering to emit compare + conditional branch
  setOperationAction(ISD::BR_CC, MVT::i32, Custom);
  
  // SETCC needs custom lowering to produce 0/1 result
  setOperationAction(ISD::SETCC, MVT::i32, Custom);
  
  // BRCOND expands to BR_CC
  setOperationAction(ISD::BRCOND, MVT::Other, Expand);
  
  // SELECT needs custom lowering
  setOperationAction(ISD::SELECT, MVT::i32, Custom);
  
  // Expand BR_JT (jump tables for switch statements - not supported)
  setOperationAction(ISD::BR_JT, MVT::Other, Expand);

  // Target intrinsic handling (e.g., thread-id, block-id, etc.)
  setOperationAction(ISD::INTRINSIC_WO_CHAIN, MVT::i32, Custom);
  setOperationAction(ISD::INTRINSIC_WO_CHAIN, MVT::Other, Custom);
  setOperationAction(ISD::INTRINSIC_W_CHAIN, MVT::i32, Custom);
  setOperationAction(ISD::INTRINSIC_W_CHAIN, MVT::Other, Custom);
  
  // Intrinsics with side effects (e.g., barrier)
  setOperationAction(ISD::INTRINSIC_VOID, MVT::Other, Custom);

  // Global address lowering
  setOperationAction(ISD::GlobalAddress, MVT::i32, Custom);
  setOperationAction(ISD::ExternalSymbol, MVT::i32, Custom);

  // Frame index lowering
  setOperationAction(ISD::FrameIndex, MVT::i32, Custom);
  
  // Constant pool lowering (for float constants)
  setOperationAction(ISD::ConstantPool, MVT::i32, Custom);
  
  // Float constants need custom lowering to load their bit representation
  setOperationAction(ISD::ConstantFP, MVT::f32, Custom);

  //===----------------------------------------------------------------------===//
  // Floating-Point Operations (f32 single precision)
  //===----------------------------------------------------------------------===//
  
  // Basic FP arithmetic is natively supported
  setOperationAction(ISD::FADD, MVT::f32, Legal);
  setOperationAction(ISD::FSUB, MVT::f32, Legal);
  setOperationAction(ISD::FMUL, MVT::f32, Legal);
  setOperationAction(ISD::FDIV, MVT::f32, Legal);
  setOperationAction(ISD::FNEG, MVT::f32, Legal);
  setOperationAction(ISD::FABS, MVT::f32, Legal);
  
  // FP comparisons - we use custom lowering to emit compare + branch
  setOperationAction(ISD::SETCC, MVT::f32, Custom);
  setOperationAction(ISD::SELECT_CC, MVT::f32, Expand);
  setOperationAction(ISD::BR_CC, MVT::f32, Custom);
  
  // FP to/from integer conversions
  // Unsigned conversions are natively supported
  setOperationAction(ISD::FP_TO_UINT, MVT::i32, Legal);
  setOperationAction(ISD::UINT_TO_FP, MVT::i32, Legal);
  // Signed conversions - use custom lowering to handle sign
  setOperationAction(ISD::FP_TO_SINT, MVT::i32, Custom);
  setOperationAction(ISD::SINT_TO_FP, MVT::i32, Custom);
  
  // FP rounding operations - natively supported
  setOperationAction(ISD::FTRUNC, MVT::f32, Legal);
  setOperationAction(ISD::FROUND, MVT::f32, Legal);
  setOperationAction(ISD::FCEIL, MVT::f32, Legal);
  setOperationAction(ISD::FFLOOR, MVT::f32, Legal);
  
  // FP operations that need expansion (not natively supported yet)
  setOperationAction(ISD::FSQRT, MVT::f32, Expand);
  setOperationAction(ISD::FSIN, MVT::f32, Expand);
  setOperationAction(ISD::FCOS, MVT::f32, Expand);
  setOperationAction(ISD::FPOW, MVT::f32, Expand);
  setOperationAction(ISD::FLOG, MVT::f32, Expand);
  setOperationAction(ISD::FLOG2, MVT::f32, Expand);
  setOperationAction(ISD::FLOG10, MVT::f32, Expand);
  setOperationAction(ISD::FEXP, MVT::f32, Expand);
  setOperationAction(ISD::FEXP2, MVT::f32, Expand);
  setOperationAction(ISD::FRINT, MVT::f32, Custom);
  setOperationAction(ISD::FNEARBYINT, MVT::f32, Custom);
  setOperationAction(ISD::FCOPYSIGN, MVT::f32, Custom);
  // FP min/max - natively supported
  setOperationAction(ISD::FMINNUM, MVT::f32, Legal);
  setOperationAction(ISD::FMAXNUM, MVT::f32, Legal);
  setOperationAction(ISD::FMA, MVT::f32, Custom);
  
  // Double precision (f64) is not supported - expand everything
  setOperationAction(ISD::FADD, MVT::f64, Expand);
  setOperationAction(ISD::FSUB, MVT::f64, Expand);
  setOperationAction(ISD::FMUL, MVT::f64, Expand);
  setOperationAction(ISD::FDIV, MVT::f64, Expand);
  setOperationAction(ISD::FNEG, MVT::f64, Expand);
  setOperationAction(ISD::FABS, MVT::f64, Expand);
  setOperationAction(ISD::SETCC, MVT::f64, Expand);
  setOperationAction(ISD::SELECT_CC, MVT::f64, Expand);
  setOperationAction(ISD::BR_CC, MVT::f64, Expand);

  computeRegisterProperties(STI.getRegisterInfo());
}
//- cgp1-isel-lowering-cpp-1

//@s cgp1-isel-lowering-ret-lower-1 end
SDValue
CGP1TargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                bool isVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &dl, SelectionDAG &DAG) const {
  //- cgp1-isel-lowering-ret-lower-1
  
  // Check if this is a kernel function (SIMT entry point)
  // Kernels use EndKernel (STOP_CORE) instead of Ret
  MachineFunction &MF = DAG.getMachineFunction();
  bool IsKernel = isKernelFunction(MF.getFunction());
  
  //@s cgp1-isel-lowering-ret-lower-2 end
  // Handle single return value of i32 or f32
  // Kernels should not have return values
  if (Outs.size() > 1) {
    report_fatal_error(
        "Multiple return values not supported\n"
        "This could be because the return type is a struct or a large integer "
        "that got split into multiple registers",
        false);
  }
  
  if (IsKernel && Outs.size() > 0) {
    report_fatal_error("Kernel functions cannot return values", false);
  }
  //- cgp1-isel-lowering-ret-lower-2
    
  //@s cgp1-isel-lowering-ret-lower-3 end
  if (Outs.size() == 0) {
    // Kernels terminate with EndKernel (STOP_CORE), device functions use Ret
    if (IsKernel)
      return DAG.getNode(CGP1ISD::EndKernel, dl, MVT::Other, Chain);
    return DAG.getNode(CGP1ISD::Ret, dl, MVT::Other, Chain);
  }
  //- cgp1-isel-lowering-ret-lower-3

  //@s cgp1-isel-lowering-ret-lower-4 end
  SDValue Glue;
  SmallVector<SDValue, 3> RetOps(1, Chain);

  const ISD::OutputArg &Out = Outs[0];
  const SDValue &OutVal = OutVals[0];

  if (Out.ArgVT == MVT::i32) {
    Chain = DAG.getCopyToReg(Chain, dl, CGP1::R1, OutVal, Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(CGP1::R1, Out.VT));
  } else if (Out.ArgVT == MVT::f32) {
    Chain = DAG.getCopyToReg(Chain, dl, CGP1::R1, OutVal, Glue);
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(CGP1::R1, Out.VT));
  } else {
    report_fatal_error("Only i32 or f32 return values are supported", false);
  }

  RetOps[0] = Chain;
  RetOps.push_back(Glue);

  return DAG.getNode(CGP1ISD::Ret, dl,MVT::Other, RetOps);
}
//- cgp1-isel-lowering-ret-lower-4

//@s cgp1-isel-lowering-cpp-2 end

// LowerFormalArguments - Handle function arguments
// For CGP1: first 4 integer args in $a0-$a3, first 8 FP args in $f4-$f11, rest on stack
SDValue CGP1TargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool isVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  MachineFunction &MF = DAG.getMachineFunction();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();

  // Detect kernel functions (SIMT entry points)
  bool IsKernel = isKernelFunction(MF.getFunction());
  if (IsKernel && isVarArg)
    report_fatal_error("Varargs not supported in CGP1 kernels", false);

  // For kernels, we use R14 (LR) to pass the thread ID (threadIdx.x).
  // LR is otherwise unused in kernels since they don't return.
  // We also use R11 for block_id_x, R10 for block_dim_x, and R9 for grid_dim_x.
  if (IsKernel) {
    // Thread ID
    Register VRegThreadID = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
    RegInfo.addLiveIn(CGP1::R14, VRegThreadID);
    
    // Block ID
    Register VRegBlockID = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
    RegInfo.addLiveIn(CGP1::R11, VRegBlockID);
    
    // Block Dim
    Register VRegBlockDim = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
    RegInfo.addLiveIn(CGP1::R10, VRegBlockDim);
    
    // Grid Dim
    Register VRegGridDim = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
    RegInfo.addLiveIn(CGP1::R9, VRegGridDim);
  }

  // Integer argument registers
  static const MCPhysReg IntArgRegs[] = {CGP1::R1, CGP1::R2, CGP1::R3, CGP1::R4};
  unsigned NumIntArgRegs = std::size(IntArgRegs);
  // Floating-point argument registers
  static const MCPhysReg FPArgRegs[] = {CGP1::R1, CGP1::R2, CGP1::R3, CGP1::R4,
                                        CGP1::R5, CGP1::R6, CGP1::R7, CGP1::R8};
  unsigned NumFPArgRegs = std::size(FPArgRegs);
  unsigned NextIntReg = 0;
  unsigned NextFPReg = 0;
  unsigned StackOffset = 0; // Stack arguments start at SP + 0 (after return address)

  for (unsigned i = 0, e = Ins.size(); i != e; ++i) {
    const ISD::InputArg &In = Ins[i];

    if (In.VT == MVT::i32) {
      if (NextIntReg < NumIntArgRegs) {
      Register VReg = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
        RegInfo.addLiveIn(IntArgRegs[NextIntReg++], VReg);
      SDValue ArgValue = DAG.getCopyFromReg(Chain, dl, VReg, MVT::i32);
      Chain = ArgValue.getValue(1);
      InVals.push_back(ArgValue);
    } else {
        // Stack argument: load from SP + StackOffset
        SDValue SPOffset = DAG.getIntPtrConstant(StackOffset, dl);
        SDValue SP = DAG.getCopyFromReg(Chain, dl, CGP1::R13, MVT::i32);
        SDValue Addr = DAG.getNode(ISD::ADD, dl, MVT::i32, SP, SPOffset);
        SDValue ArgValue = DAG.getLoad(MVT::i32, dl, Chain, Addr, MachinePointerInfo());
        Chain = ArgValue.getValue(1);
        InVals.push_back(ArgValue);
        StackOffset += 4; // 4 bytes for i32
      }
    } else if (In.VT == MVT::f32) {
      if (NextFPReg < NumFPArgRegs) {
        Register VReg = RegInfo.createVirtualRegister(&CGP1::GPRRegClass);
        RegInfo.addLiveIn(FPArgRegs[NextFPReg++], VReg);
        SDValue ArgValue = DAG.getCopyFromReg(Chain, dl, VReg, MVT::f32);
        Chain = ArgValue.getValue(1);
        InVals.push_back(ArgValue);
      } else {
        // Stack argument: load from SP + StackOffset using LWC1
        SDValue SPOffset = DAG.getIntPtrConstant(StackOffset, dl);
        SDValue SP = DAG.getCopyFromReg(Chain, dl, CGP1::R13, MVT::i32);
        SDValue Addr = DAG.getNode(ISD::ADD, dl, MVT::i32, SP, SPOffset);
        SDValue ArgValue = DAG.getLoad(MVT::f32, dl, Chain, Addr, MachinePointerInfo());
        Chain = ArgValue.getValue(1);
        InVals.push_back(ArgValue);
        StackOffset += 4; // 4 bytes for f32
      }
    } else {
      report_fatal_error("Only i32 or f32 arguments are supported", false);
    }
  }

  return Chain;
}

SDValue CGP1TargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                      SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &DL = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  CallingConv::ID CallConv = CLI.CallConv;
  bool IsVarArg = CLI.IsVarArg;

  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  // CGP1 calling convention:
  //   Integer args: $a0-$a3
  //   FP args: $f4-$f11
  static const MCPhysReg IntArgRegs[] = {CGP1::R1, CGP1::R2, CGP1::R3, CGP1::R4};
  unsigned NumIntArgRegs = std::size(IntArgRegs);
  static const MCPhysReg FPArgRegs[] = {CGP1::R1, CGP1::R2, CGP1::R3, CGP1::R4,
                                        CGP1::R5, CGP1::R6, CGP1::R7, CGP1::R8};
  unsigned NumFPArgRegs = std::size(FPArgRegs);
  unsigned NextIntReg = 0;
  unsigned NextFPReg = 0;

  // Analyze the arguments
  unsigned NumBytes = 0;
  for (unsigned i = 0, e = Outs.size(); i != e; ++i) {
    if (Outs[i].VT == MVT::i32) {
      if (NextIntReg < NumIntArgRegs) {
        NextIntReg++;
      } else {
        // Stack argument - each takes 4 bytes (aligned)
        NumBytes += 4;
      }
    } else if (Outs[i].VT == MVT::f32) {
      if (NextFPReg < NumFPArgRegs) {
        NextFPReg++;
      } else {
        NumBytes += 4;
      }
    } else {
      report_fatal_error("Only i32 or f32 call arguments are supported", false);
    }
    if (NextIntReg > NumIntArgRegs || NextFPReg > NumFPArgRegs) {
      // Stack argument - each takes 4 bytes (aligned)
      NumBytes += 4;
    }
  }

  // Round up to 8-byte alignment for stack
  NumBytes = alignTo(NumBytes, 8);

  // Adjust the stack pointer (reserve space for outgoing args)
  Chain = DAG.getCALLSEQ_START(Chain, NumBytes, 0, DL);

  SmallVector<std::pair<Register, SDValue>, 8> RegsToPass;
  SmallVector<SDValue, 8> MemOpChains;

  // Process each argument
  NextIntReg = 0;
  NextFPReg = 0;
  for (unsigned i = 0, e = Outs.size(); i != e; ++i) {
    SDValue Arg = OutVals[i];
    ISD::OutputArg &Out = Outs[i];

    if (Out.VT == MVT::i32) {
      if (Arg.getValueType() != MVT::i32)
      Arg = DAG.getNode(ISD::ANY_EXTEND, DL, MVT::i32, Arg);
      if (NextIntReg < NumIntArgRegs) {
        RegsToPass.push_back(std::make_pair(IntArgRegs[NextIntReg++], Arg));
      } else {
        unsigned StackOffset = (NextIntReg - NumIntArgRegs) * 4;
        NextIntReg++;
        SDValue StackPtr = DAG.getCopyFromReg(Chain, DL, CGP1::R13, MVT::i32);
        SDValue PtrOff = DAG.getIntPtrConstant(StackOffset, DL);
        SDValue Addr = DAG.getNode(ISD::ADD, DL, MVT::i32, StackPtr, PtrOff);
        MemOpChains.push_back(
            DAG.getStore(Chain, DL, Arg, Addr, MachinePointerInfo()));
      }
    } else if (Out.VT == MVT::f32) {
      if (Arg.getValueType() != MVT::f32)
        Arg = DAG.getNode(ISD::FP_ROUND, DL, MVT::f32, Arg);
      if (NextFPReg < NumFPArgRegs) {
        RegsToPass.push_back(std::make_pair(FPArgRegs[NextFPReg++], Arg));
    } else {
        unsigned StackOffset = (NextFPReg - NumFPArgRegs) * 4;
        NextFPReg++;
      SDValue StackPtr = DAG.getCopyFromReg(Chain, DL, CGP1::R13, MVT::i32);
      SDValue PtrOff = DAG.getIntPtrConstant(StackOffset, DL);
      SDValue Addr = DAG.getNode(ISD::ADD, DL, MVT::i32, StackPtr, PtrOff);
      MemOpChains.push_back(
          DAG.getStore(Chain, DL, Arg, Addr, MachinePointerInfo()));
      }
    } else {
      report_fatal_error("Only i32 or f32 call arguments are supported", false);
    }
  }

  // Emit all stores for stack arguments
  if (!MemOpChains.empty())
    Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other, MemOpChains);

  // Build a sequence of copy-to-reg nodes chained together with token chain
  // and flag operands which copy the outgoing args into the appropriate regs.
  SDValue InGlue;
  for (auto &Reg : RegsToPass) {
    Chain = DAG.getCopyToReg(Chain, DL, Reg.first, Reg.second, InGlue);
    InGlue = Chain.getValue(1);
  }

  // Handle the callee
  if (GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee)) {
    Callee = DAG.getTargetGlobalAddress(G->getGlobal(), DL, MVT::i32, 0);
  } else if (ExternalSymbolSDNode *E = dyn_cast<ExternalSymbolSDNode>(Callee)) {
    Callee = DAG.getTargetExternalSymbol(E->getSymbol(), MVT::i32);
  }

  // Build the operand list for the call node
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);

  // Add argument registers to the call node
  for (auto &Reg : RegsToPass)
    Ops.push_back(DAG.getRegister(Reg.first, MVT::i32));

  // Add a register mask operand representing the call-preserved registers
  const TargetRegisterInfo *TRI = MF.getSubtarget().getRegisterInfo();
  const uint32_t *Mask = TRI->getCallPreservedMask(MF, CallConv);
  assert(Mask && "Missing call preserved mask for calling convention");
  Ops.push_back(DAG.getRegisterMask(Mask));

  if (InGlue.getNode())
    Ops.push_back(InGlue);

  // Emit the call
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  Chain = DAG.getNode(CGP1ISD::Call, DL, NodeTys, Ops);
  InGlue = Chain.getValue(1);

  // Create CALLSEQ_END node
  Chain = DAG.getCALLSEQ_END(Chain, NumBytes, 0, InGlue, DL);
  InGlue = Chain.getValue(1);

  // Handle return value
  if (!Ins.empty()) {
    if (Ins.size() > 1) {
      report_fatal_error("Multiple return values not supported in calls", false);
    }

    if (Ins[0].VT == MVT::i32) {
    Chain = DAG.getCopyFromReg(Chain, DL, CGP1::R1, MVT::i32, InGlue).getValue(1);
    InVals.push_back(Chain.getValue(0));
    } else if (Ins[0].VT == MVT::f32) {
      Chain = DAG.getCopyFromReg(Chain, DL, CGP1::R1, MVT::f32, InGlue).getValue(1);
      InVals.push_back(Chain.getValue(0));
    } else {
      report_fatal_error("Only i32 or f32 return values are supported in calls", false);
    }
  }

  return Chain;
}

bool CGP1TargetLowering::CanLowerReturn(CallingConv::ID CallConv, MachineFunction &MF,
    bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs,
    LLVMContext &Context, const Type *RetTy) const{
      return true;
}

SDValue CGP1TargetLowering::LowerOperation(SDValue Op,
                                           SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::INTRINSIC_WO_CHAIN:
  case ISD::INTRINSIC_W_CHAIN:
    return lowerIntrinsic(Op, DAG);
  case ISD::INTRINSIC_VOID:
    return lowerVoidIntrinsic(Op, DAG);
  case ISD::GlobalAddress:
    return LowerGlobalAddress(Op, DAG);
  case ISD::ExternalSymbol:
    return LowerExternalSymbol(Op, DAG);
  case ISD::FrameIndex:
    return LowerFrameIndex(Op, DAG);
  case ISD::BR_CC:
    return LowerBR_CC(Op, DAG);
  case ISD::SETCC:
    return LowerSETCC(Op, DAG);
  case ISD::SELECT:
    return LowerSELECT(Op, DAG);
  case ISD::ConstantPool:
    return LowerConstantPool(Op, DAG);
  case ISD::ConstantFP:
    return LowerConstantFP(Op, DAG);
  case ISD::FP_TO_SINT:
    return LowerFP_TO_SINT(Op, DAG);
  case ISD::SINT_TO_FP:
    return LowerSINT_TO_FP(Op, DAG);
  case ISD::ROTL:
    return LowerROTL(Op, DAG);
  case ISD::ROTR:
    return LowerROTR(Op, DAG);
  case ISD::SIGN_EXTEND:
    return LowerSIGN_EXTEND(Op, DAG);
  case ISD::ZERO_EXTEND:
    return LowerZERO_EXTEND(Op, DAG);
  case ISD::ANY_EXTEND:
    return LowerANY_EXTEND(Op, DAG);
  case ISD::TRUNCATE:
    return LowerTRUNCATE(Op, DAG);
  case ISD::SIGN_EXTEND_INREG:
    return LowerSIGN_EXTEND_INREG(Op, DAG);
  case ISD::FMA:
    return LowerFMA(Op, DAG);
  case ISD::FCOPYSIGN:
    return LowerFCOPYSIGN(Op, DAG);
  case ISD::FRINT:
    return LowerFRINT(Op, DAG);
  case ISD::FNEARBYINT:
    return LowerFNEARBYINT(Op, DAG);
  default:
    return SDValue();
  }
}

SDValue CGP1TargetLowering::LowerFrameIndex(SDValue Op,
                                             SelectionDAG &DAG) const {
  SDLoc DL(Op);
  int FI = cast<FrameIndexSDNode>(Op)->getIndex();
  EVT VT = Op.getValueType();
  return DAG.getTargetFrameIndex(FI, VT);
}

SDValue CGP1TargetLowering::LowerBR_CC(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Chain = Op.getOperand(0);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(1))->get();
  SDValue LHS = Op.getOperand(2);
  SDValue RHS = Op.getOperand(3);
  SDValue Dest = Op.getOperand(4);
  EVT LHSVT = LHS.getValueType();
  
  // Check if we're in a kernel function (SIMT context)
  // For SIMT, we need to automatically manage EXEC mask on branches
  MachineFunction &MF = DAG.getMachineFunction();
  const Function &F = MF.getFunction();
  bool IsKernel = isKernelFunction(F);
  
  // Compute the branch condition as a mask (1 for threads that take branch, 0 otherwise)
  SDValue ConditionMask;
  
  // Handle floating-point comparisons
  if (LHSVT == MVT::f32) {
    // For FP comparisons, use SETCC to get comparison result (0 or 1)
    ConditionMask = DAG.getNode(ISD::SETCC, DL, MVT::i32, LHS, RHS,
                                DAG.getCondCode(CC));
  } else {
    // Integer comparisons: compute LHS - RHS to get comparison result
    SDValue Diff = DAG.getNode(ISD::SUB, DL, MVT::i32, LHS, RHS);
    SDValue Zero = DAG.getConstant(0, DL, MVT::i32);
    SDValue One = DAG.getConstant(1, DL, MVT::i32);
    
    // Convert comparison to mask (0 or 1)
    switch (CC) {
    case ISD::SETEQ:
      // Condition: Diff == 0 -> mask = (Diff == 0) ? 1 : 0
      ConditionMask = DAG.getNode(ISD::SELECT, DL, MVT::i32,
                                   DAG.getNode(ISD::SETCC, DL, MVT::i32, Diff, Zero,
                                               DAG.getCondCode(ISD::SETEQ)),
                                   One, Zero);
      break;
    case ISD::SETNE:
      // Condition: Diff != 0 -> mask = (Diff != 0) ? 1 : 0
      ConditionMask = DAG.getNode(ISD::SELECT, DL, MVT::i32,
                                   DAG.getNode(ISD::SETCC, DL, MVT::i32, Diff, Zero,
                                               DAG.getCondCode(ISD::SETNE)),
                                   One, Zero);
      break;
    case ISD::SETLT:
      // Condition: Diff < 0 -> mask = (Diff < 0) ? 1 : 0
      ConditionMask = DAG.getNode(ISD::SELECT, DL, MVT::i32,
                                   DAG.getNode(ISD::SETCC, DL, MVT::i32, Diff, Zero,
                                               DAG.getCondCode(ISD::SETLT)),
                                   One, Zero);
      break;
    case ISD::SETGT:
      // Condition: Diff > 0 -> mask = (Diff > 0) ? 1 : 0
      ConditionMask = DAG.getNode(ISD::SELECT, DL, MVT::i32,
                                   DAG.getNode(ISD::SETCC, DL, MVT::i32, Diff, Zero,
                                               DAG.getCondCode(ISD::SETGT)),
                                   One, Zero);
      break;
    case ISD::SETLE:
      // Condition: Diff <= 0 -> mask = (Diff <= 0) ? 1 : 0
      ConditionMask = DAG.getNode(ISD::SELECT, DL, MVT::i32,
                                   DAG.getNode(ISD::SETCC, DL, MVT::i32, Diff, Zero,
                                               DAG.getCondCode(ISD::SETLE)),
                                   One, Zero);
      break;
    case ISD::SETGE:
      // Condition: Diff >= 0 -> mask = (Diff >= 0) ? 1 : 0
      ConditionMask = DAG.getNode(ISD::SELECT, DL, MVT::i32,
                                   DAG.getNode(ISD::SETCC, DL, MVT::i32, Diff, Zero,
                                               DAG.getCondCode(ISD::SETGE)),
                                   One, Zero);
      break;
    default:
      // Fallback: use SETCC directly
      ConditionMask = DAG.getNode(ISD::SETCC, DL, MVT::i32, Diff, Zero,
                                   DAG.getCondCode(CC));
      break;
    }
  }
  
  // For SIMT (kernel functions), automatically manage EXEC mask
  if (IsKernel) {
    // Get current EXEC mask (R15)
    SDValue Exec = DAG.getCopyFromReg(Chain, DL, CGP1::R15, MVT::i32);
    Chain = Exec.getValue(1);
    
    // Push EXEC mask onto stack (for reconvergence)
    SDValue ExecPush = DAG.getNode(CGP1ISD::ExecPush, DL, MVT::Other, Chain, Exec);
    Chain = ExecPush;
    
    // Convert condition mask (0 or 1) to full mask (0xFFFFFFFF or 0x00000000)
    // Formula: full_mask = (condition - 1) ^ 0xFFFFFFFF
    // If condition=1: (1-1)^0xFFFFFFFF = 0^0xFFFFFFFF = 0xFFFFFFFF
    // If condition=0: (0-1)^0xFFFFFFFF = 0xFFFFFFFF^0xFFFFFFFF = 0
    SDValue One = DAG.getConstant(1, DL, MVT::i32);
    SDValue AllOnes = DAG.getConstant(0xFFFFFFFF, DL, MVT::i32);
    SDValue ConditionMinusOne = DAG.getNode(ISD::SUB, DL, MVT::i32, ConditionMask, One);
    SDValue FullConditionMask = DAG.getNode(ISD::XOR, DL, MVT::i32, ConditionMinusOne, AllOnes);
    
    // For SIMT, we need to handle both the taken path and fall-through path
    // Taken path: EXEC = old_EXEC AND condition_mask (threads where condition is true)
    // Fall-through path: EXEC = old_EXEC AND NOT condition_mask (threads where condition is false)
    
    // Compute EXEC mask for taken path
    SDValue TakenExec = DAG.getNode(ISD::AND, DL, MVT::i32, Exec, FullConditionMask);
    
    // Compute EXEC mask for fall-through path: old_EXEC AND NOT condition_mask
    SDValue NotConditionMask = DAG.getNode(ISD::XOR, DL, MVT::i32, FullConditionMask, AllOnes);
    SDValue FallThroughExec = DAG.getNode(ISD::AND, DL, MVT::i32, Exec, NotConditionMask);
    
    // For SIMT branches, we need to handle both taken and fall-through paths
    // Get the fall-through basic block (the block that follows the branch in the CFG)
    const BasicBlock *BB = DAG.getBasicBlock();
    SDValue FallThroughDest = Dest; // Default: use same as taken (will be fixed by pass)
    
    if (BB) {
      // Try to find the fall-through block
      // In LLVM IR, a conditional branch has two successors: taken and fall-through
      // The fall-through is typically the block that comes after the branch in the function
      const BasicBlock *NextBB = BB->getNextNode();
      if (NextBB) {
        // Check if NextBB is different from the taken destination
        const BasicBlock *TakenBB = cast<BasicBlockSDNode>(Dest)->getBasicBlock();
        if (NextBB != TakenBB) {
          // We have a valid fall-through path
          FallThroughDest = DAG.getBasicBlock(NextBB);
        }
      }
    }
    
    // Create SIMT branch node with both EXEC masks and both destinations
    // Operands: Chain, ConditionMask, TakenExec, FallThroughExec, TakenDest, FallThroughDest
    return DAG.getNode(CGP1ISD::SIMTBranch, DL, MVT::Other, Chain,
                       ConditionMask, TakenExec, FallThroughExec, Dest, FallThroughDest);
  }
  
  // For non-kernel functions, use original branch logic
  // Map condition code to our custom branch node
  unsigned BranchOpc;
  SDValue BranchCond;
  
  if (LHSVT == MVT::f32) {
    BranchOpc = CGP1ISD::BranchNotZero;
    BranchCond = ConditionMask;
  } else {
    SDValue Diff = DAG.getNode(ISD::SUB, DL, MVT::i32, LHS, RHS);
    switch (CC) {
    case ISD::SETEQ:
      BranchOpc = CGP1ISD::BranchZero;
      BranchCond = Diff;
      break;
    case ISD::SETNE:
      BranchOpc = CGP1ISD::BranchNotZero;
      BranchCond = Diff;
      break;
    case ISD::SETLT:
      BranchOpc = CGP1ISD::BranchLTZ;
      BranchCond = Diff;
      break;
    case ISD::SETGT:
      Diff = DAG.getNode(ISD::SUB, DL, MVT::i32, RHS, LHS);
      BranchOpc = CGP1ISD::BranchLTZ;
      BranchCond = Diff;
      break;
    case ISD::SETLE:
      Diff = DAG.getNode(ISD::SUB, DL, MVT::i32, RHS, LHS);
      BranchOpc = CGP1ISD::BranchGEZ;
      BranchCond = Diff;
      break;
    case ISD::SETGE:
      BranchOpc = CGP1ISD::BranchGEZ;
      BranchCond = Diff;
      break;
    default:
      BranchOpc = CGP1ISD::BranchNotZero;
      BranchCond = Diff;
      break;
    }
  }
  
  return DAG.getNode(BranchOpc, DL, MVT::Other, Chain, BranchCond, Dest);
}

SDValue CGP1TargetLowering::LowerSETCC(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue LHS = Op.getOperand(0);
  SDValue RHS = Op.getOperand(1);
  ISD::CondCode CC = cast<CondCodeSDNode>(Op.getOperand(2))->get();
  EVT VT = Op.getValueType();
  
  // Compute LHS - RHS
  SDValue Diff = DAG.getNode(ISD::SUB, DL, MVT::i32, LHS, RHS);
  SDValue Zero = DAG.getConstant(0, DL, MVT::i32);
  SDValue One = DAG.getConstant(1, DL, MVT::i32);
  
  // Use arithmetic shift to get sign bit, then mask for result
  // For now, implement using conditional moves pattern:
  // result = (cond) ? 1 : 0
  // We implement this by:
  // 1. Compute diff = LHS - RHS
  // 2. Based on condition, derive 0 or 1
  
  switch (CC) {
  case ISD::SETEQ:
    // (LHS == RHS) => (diff == 0)
    // Use: (diff | -diff) >> 31 gives 1 if diff != 0, so XOR with 1
    {
      SDValue NegDiff = DAG.getNode(ISD::SUB, DL, MVT::i32, Zero, Diff);
      SDValue OrVal = DAG.getNode(ISD::OR, DL, MVT::i32, Diff, NegDiff);
      SDValue Shift = DAG.getNode(ISD::SRL, DL, MVT::i32, OrVal, 
                                  DAG.getConstant(31, DL, MVT::i32));
      return DAG.getNode(ISD::XOR, DL, VT, Shift, One);
    }
  case ISD::SETNE:
    // (LHS != RHS) => (diff != 0)
    // Use: (diff | -diff) >> 31 gives 1 if diff != 0
    {
      SDValue NegDiff = DAG.getNode(ISD::SUB, DL, MVT::i32, Zero, Diff);
      SDValue OrVal = DAG.getNode(ISD::OR, DL, MVT::i32, Diff, NegDiff);
      return DAG.getNode(ISD::SRL, DL, VT, OrVal,
                         DAG.getConstant(31, DL, MVT::i32));
    }
  case ISD::SETLT:
    // (LHS < RHS) => (diff < 0)
    // Arithmetic shift right by 31 gives -1 if negative, 0 if positive
    // Then AND with 1
    return DAG.getNode(ISD::AND, DL, VT,
                       DAG.getNode(ISD::SRA, DL, MVT::i32, Diff,
                                   DAG.getConstant(31, DL, MVT::i32)),
                       One);
  case ISD::SETGT:
    // (LHS > RHS) => (RHS < LHS) => (RHS - LHS < 0)
    {
      SDValue RevDiff = DAG.getNode(ISD::SUB, DL, MVT::i32, RHS, LHS);
      return DAG.getNode(ISD::AND, DL, VT,
                         DAG.getNode(ISD::SRA, DL, MVT::i32, RevDiff,
                                     DAG.getConstant(31, DL, MVT::i32)),
                         One);
    }
  case ISD::SETLE:
    // (LHS <= RHS) => !(LHS > RHS) => !((RHS - LHS) < 0)
    {
      SDValue RevDiff = DAG.getNode(ISD::SUB, DL, MVT::i32, RHS, LHS);
      SDValue IsGT = DAG.getNode(ISD::AND, DL, MVT::i32,
                                 DAG.getNode(ISD::SRA, DL, MVT::i32, RevDiff,
                                             DAG.getConstant(31, DL, MVT::i32)),
                                 One);
      return DAG.getNode(ISD::XOR, DL, VT, IsGT, One);
    }
  case ISD::SETGE:
    // (LHS >= RHS) => !(LHS < RHS) => !((LHS - RHS) < 0)
    {
      SDValue IsLT = DAG.getNode(ISD::AND, DL, MVT::i32,
                                 DAG.getNode(ISD::SRA, DL, MVT::i32, Diff,
                                             DAG.getConstant(31, DL, MVT::i32)),
                                 One);
      return DAG.getNode(ISD::XOR, DL, VT, IsLT, One);
    }
  default:
    // For unsigned comparisons, use same logic (simplified)
    return DAG.getNode(ISD::AND, DL, VT,
                       DAG.getNode(ISD::SRA, DL, MVT::i32, Diff,
                                   DAG.getConstant(31, DL, MVT::i32)),
                       One);
  }
}

SDValue CGP1TargetLowering::LowerSELECT(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Cond = Op.getOperand(0);
  SDValue TrueVal = Op.getOperand(1);
  SDValue FalseVal = Op.getOperand(2);
  EVT VT = Op.getValueType();
  
  // SELECT is: (cond) ? TrueVal : FalseVal
  // Implement as: FalseVal + cond * (TrueVal - FalseVal)
  // If cond is 0: FalseVal + 0 = FalseVal
  // If cond is 1: FalseVal + (TrueVal - FalseVal) = TrueVal
  
  // First, ensure cond is 0 or 1 by ANDing with 1
  SDValue One = DAG.getConstant(1, DL, MVT::i32);
  SDValue CondMasked = DAG.getNode(ISD::AND, DL, MVT::i32, Cond, One);
  
  // Compute diff = TrueVal - FalseVal
  SDValue Diff = DAG.getNode(ISD::SUB, DL, VT, TrueVal, FalseVal);
  
  // Compute cond * diff (using MUL)
  SDValue Scaled = DAG.getNode(ISD::MUL, DL, VT, CondMasked, Diff);
  
  // Result = FalseVal + scaled
  return DAG.getNode(ISD::ADD, DL, VT, FalseVal, Scaled);
}

SDValue CGP1TargetLowering::LowerConstantPool(SDValue Op,
                                               SelectionDAG &DAG) const {
  SDLoc DL(Op);
  ConstantPoolSDNode *N = cast<ConstantPoolSDNode>(Op);
  
  SDValue CP;
  if (N->isMachineConstantPoolEntry())
    CP = DAG.getTargetConstantPool(N->getMachineCPVal(), MVT::i32,
                                   N->getAlign(), N->getOffset());
  else
    CP = DAG.getTargetConstantPool(N->getConstVal(), MVT::i32,
                                   N->getAlign(), N->getOffset());
  
  // Use LUI + ORI pattern to materialize constant pool address
  SDValue Hi = DAG.getNode(CGP1ISD::Hi, DL, MVT::i32, CP);
  SDValue Lo = DAG.getNode(CGP1ISD::Lo, DL, MVT::i32, CP);
  return DAG.getNode(ISD::ADD, DL, MVT::i32, Hi, Lo);
}

SDValue CGP1TargetLowering::LowerConstantFP(SDValue Op,
                                             SelectionDAG &DAG) const {
  SDLoc DL(Op);
  ConstantFPSDNode *CFP = cast<ConstantFPSDNode>(Op);
  const APFloat &FPVal = CFP->getValueAPF();
  
  // Convert float to its bit representation as integer
  APInt IntVal = FPVal.bitcastToAPInt();
  
  // Use a target-specific node to wrap the integer constant
  // This prevents the DAG combiner from folding it back to ConstantFP
  SDValue IntConst = DAG.getTargetConstant(IntVal, DL, MVT::i32);
  return DAG.getNode(CGP1ISD::FPImm, DL, MVT::f32, IntConst);
}

SDValue CGP1TargetLowering::LowerGlobalAddress(SDValue Op,
                                               SelectionDAG &DAG) const {
  SDLoc DL(Op);
  GlobalAddressSDNode *N = cast<GlobalAddressSDNode>(Op);
  const GlobalValue *GV = N->getGlobal();
  int64_t Offset = N->getOffset();

  // Create target global address
  SDValue GA = DAG.getTargetGlobalAddress(GV, DL, MVT::i32, Offset);

  // For CGP1, we use LUI + ORI pattern to materialize the full 32-bit address
  // LUI $reg, %hi(global)
  // ORI $reg, $reg, %lo(global)
  SDValue Hi = DAG.getNode(CGP1ISD::Hi, DL, MVT::i32, GA);
  SDValue Lo = DAG.getNode(CGP1ISD::Lo, DL, MVT::i32, GA);
  return DAG.getNode(ISD::ADD, DL, MVT::i32, Hi, Lo);
}

SDValue CGP1TargetLowering::LowerExternalSymbol(SDValue Op,
                                                SelectionDAG &DAG) const {
  SDLoc DL(Op);
  ExternalSymbolSDNode *N = cast<ExternalSymbolSDNode>(Op);
  const char *Sym = N->getSymbol();

  // Create target external symbol
  SDValue ES = DAG.getTargetExternalSymbol(Sym, MVT::i32);

  // Use LUI + ORI pattern
  SDValue Hi = DAG.getNode(CGP1ISD::Hi, DL, MVT::i32, ES);
  SDValue Lo = DAG.getNode(CGP1ISD::Lo, DL, MVT::i32, ES);
  return DAG.getNode(ISD::ADD, DL, MVT::i32, Hi, Lo);
}

// Framebuffer constants (must match emulator's memory.cpp)
static const uint32_t FRAMEBUFFER_BASE = 0x10000000;
static const uint32_t FRAMEBUFFER_WIDTH = 640;

SDValue CGP1TargetLowering::lowerVoidIntrinsic(SDValue Op,
                                               SelectionDAG &DAG) const {
  // INTRINSIC_VOID operands: [0] = chain, [1] = IID, [2...] = args
  unsigned IID = Op.getConstantOperandVal(1);
  SDLoc DL(Op);
  SDValue Chain = Op.getOperand(0);

  switch (IID) {
  //===--------------------------------------------------------------------===//
  // SIMT EXEC mask intrinsics
  //===--------------------------------------------------------------------===//
  case Intrinsic::cgp1_exec_set: {
    SDValue Mask = Op.getOperand(2);
    return DAG.getCopyToReg(Chain, DL, CGP1::R15, Mask);
  }
  case Intrinsic::cgp1_exec_and: {
    SDValue Mask = Op.getOperand(2);
    SDValue Exec = DAG.getCopyFromReg(Chain, DL, CGP1::R15, MVT::i32);
    SDValue NewExec = DAG.getNode(ISD::AND, DL, MVT::i32, Exec, Mask);
    return DAG.getCopyToReg(Exec.getValue(1), DL, CGP1::R15, NewExec);
  }
  case Intrinsic::cgp1_exec_push: {
    // push EXEC onto stack
    SDValue Exec = DAG.getCopyFromReg(Chain, DL, CGP1::R15, MVT::i32);
    return DAG.getNode(CGP1ISD::ExecPush, DL, MVT::Other, Exec.getValue(1),
                       Exec);
  }
  case Intrinsic::cgp1_exec_pop: {
    // pop EXEC from stack
    SDValue Pop = DAG.getNode(CGP1ISD::ExecPop, DL, DAG.getVTList(MVT::i32,
                                                                 MVT::Other),
                              Chain);
    return DAG.getCopyToReg(Pop.getValue(1), DL, CGP1::R15, Pop);
  }

  case Intrinsic::cgp1_barrier:
    // Emit BARRIER instruction for thread synchronization
    return DAG.getNode(CGP1ISD::Barrier, DL, MVT::Other, Chain);

  case Intrinsic::cgp1_write_pixel: {
    // write_pixel(x, y, color) -> store color to framebuffer[y * width + x]
    SDValue X = Op.getOperand(2);
    SDValue Y = Op.getOperand(3);
    SDValue Color = Op.getOperand(4);

    // Calculate pixel offset: (y * FRAMEBUFFER_WIDTH + x) * 4
    SDValue Width = DAG.getConstant(FRAMEBUFFER_WIDTH, DL, MVT::i32);
    SDValue YxWidth = DAG.getNode(ISD::MUL, DL, MVT::i32, Y, Width);
    SDValue Offset = DAG.getNode(ISD::ADD, DL, MVT::i32, YxWidth, X);
    SDValue ByteOffset = DAG.getNode(ISD::SHL, DL, MVT::i32, Offset,
                                     DAG.getConstant(2, DL, MVT::i32)); // * 4

    // Calculate absolute address: FRAMEBUFFER_BASE + byte_offset
    SDValue BaseAddr = DAG.getConstant(FRAMEBUFFER_BASE, DL, MVT::i32);
    SDValue Addr = DAG.getNode(ISD::ADD, DL, MVT::i32, BaseAddr, ByteOffset);

    // Store color to framebuffer
    return DAG.getStore(Chain, DL, Color, Addr, MachinePointerInfo());
  }

  case Intrinsic::cgp1_clear_screen: {
    // clear_screen(color) -> emit CLEAR_SCREEN instruction
    // Copy color to $a0
    SDValue Color = Op.getOperand(2);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R1, Color);
    // Emit CLEAR_SCREEN node (will be selected to CLEAR_SCREEN instruction)
    return DAG.getNode(CGP1ISD::ClearScreen, DL, MVT::Other, Chain);
  }

  case Intrinsic::cgp1_draw_line: {
    // draw_line(x0, y0, x1, y1, color) -> emit DRAW_LINE instruction
    SDValue X0 = Op.getOperand(2);
    SDValue Y0 = Op.getOperand(3);
    SDValue X1 = Op.getOperand(4);
    SDValue Y1 = Op.getOperand(5);
    SDValue Color = Op.getOperand(6);
    
    // Copy args to registers with glue to prevent reordering
    SDValue Glue;
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R1, X0, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R2, Y0, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R3, X1, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R4, Y1, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R5, Color, Glue);
    Glue = Chain.getValue(1);
    
    return DAG.getNode(CGP1ISD::DrawLine, DL, MVT::Other, Chain, Glue);
  }

  case Intrinsic::cgp1_draw_triangle: {
    // draw_triangle(x0, y0, x1, y1, x2, y2, color) -> emit DRAW_TRIANGLE instruction
    SDValue X0 = Op.getOperand(2);
    SDValue Y0 = Op.getOperand(3);
    SDValue X1 = Op.getOperand(4);
    SDValue Y1 = Op.getOperand(5);
    SDValue X2 = Op.getOperand(6);
    SDValue Y2 = Op.getOperand(7);
    SDValue Color = Op.getOperand(8);
    
    // Copy args to registers with glue to prevent reordering
    SDValue Glue;
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R1, X0, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R2, Y0, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R3, X1, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R4, Y1, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R5, X2, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R6, Y2, Glue);
    Glue = Chain.getValue(1);
    Chain = DAG.getCopyToReg(Chain, DL, CGP1::R7, Color, Glue);
    Glue = Chain.getValue(1);
    
    return DAG.getNode(CGP1ISD::DrawTriangle, DL, MVT::Other, Chain, Glue);
  }

  default:
    return SDValue();
  }
}

const char *CGP1TargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case CGP1ISD::Ret:
    return "CGP1ISD::Ret";
  case CGP1ISD::Call:
    return "CGP1ISD::Call";
  case CGP1ISD::Wrapper:
    return "CGP1ISD::Wrapper";
  case CGP1ISD::Hi:
    return "CGP1ISD::Hi";
  case CGP1ISD::Lo:
    return "CGP1ISD::Lo";
  case CGP1ISD::DrawTriangle:
    return "CGP1ISD::DrawTriangle";
  case CGP1ISD::DrawLine:
    return "CGP1ISD::DrawLine";
  case CGP1ISD::ClearScreen:
    return "CGP1ISD::ClearScreen";
  case CGP1ISD::FPImm:
    return "CGP1ISD::FPImm";
  case CGP1ISD::EndKernel:
    return "CGP1ISD::EndKernel";
  case CGP1ISD::Barrier:
    return "CGP1ISD::Barrier";
  case CGP1ISD::ExecPush:
    return "CGP1ISD::ExecPush";
  case CGP1ISD::ExecPop:
    return "CGP1ISD::ExecPop";
  case CGP1ISD::ExecReconverge:
    return "CGP1ISD::ExecReconverge";
  case CGP1ISD::ExecReconverge:
    return "CGP1ISD::ExecReconverge";
  case CGP1ISD::BranchZero:
    return "CGP1ISD::BranchZero";
  case CGP1ISD::BranchNotZero:
    return "CGP1ISD::BranchNotZero";
  case CGP1ISD::BranchLTZ:
    return "CGP1ISD::BranchLTZ";
  case CGP1ISD::BranchGEZ:
    return "CGP1ISD::BranchGEZ";
  case CGP1ISD::AtomicMax:
    return "CGP1ISD::AtomicMax";
  case CGP1ISD::WarpSize:
    return "CGP1ISD::WarpSize";
  case CGP1ISD::LaneId:
    return "CGP1ISD::LaneId";
  case CGP1ISD::WarpAll:
    return "CGP1ISD::WarpAll";
  case CGP1ISD::WarpAny:
    return "CGP1ISD::WarpAny";
  case CGP1ISD::WarpBallot:
    return "CGP1ISD::WarpBallot";
  case CGP1ISD::WarpShuffle:
    return "CGP1ISD::WarpShuffle";
  default:
    return "Unknown CGP1ISD::Node";
  }
}

bool CGP1TargetLowering::isKernelFunction(const Function &F) const {
  if (F.getCallingConv() == CallingConv::SPIR_KERNEL)
    return true;
  if (F.hasFnAttribute("cgp1_kernel"))
    return true;
  return false;
}

SDValue CGP1TargetLowering::lowerIntrinsic(SDValue Op,
                                           SelectionDAG &DAG) const {
  if (Op.getNumOperands() == 0)
    return SDValue();

  unsigned IID;
  SDValue Chain;
  if (Op.getOpcode() == ISD::INTRINSIC_WO_CHAIN) {
    IID = cast<ConstantSDNode>(Op.getOperand(0))->getZExtValue();
    Chain = DAG.getEntryNode();
  } else { // INTRINSIC_W_CHAIN
    IID = cast<ConstantSDNode>(Op.getOperand(1))->getZExtValue();
    Chain = Op.getOperand(0);
  }

  EVT VT = Op.getValueType();
  SDLoc DL(Op);
  
  switch (IID) {
  //===--------------------------------------------------------------------===//
  // SIMT EXEC mask intrinsics
  //===--------------------------------------------------------------------===//
  case Intrinsic::cgp1_exec_get: {
    // Model EXEC as reserved physical register R15.
    return DAG.getCopyFromReg(Chain, DL, CGP1::R15, VT);
  }

  //===--------------------------------------------------------------------===//
  // Thread ID intrinsics (thread index within a block)
  //===--------------------------------------------------------------------===//
  case Intrinsic::cgp1_thread_id_x: {
    // Read from R14 (LR) which holds thread ID in kernels
    MachineFunction &MF = DAG.getMachineFunction();
    MachineRegisterInfo &RegInfo = MF.getRegInfo();
    Register ThreadIDReg = 0;
    
    for (const auto &LI : RegInfo.liveins()) {
      if (LI.first == CGP1::R14) {
        ThreadIDReg = LI.second;
        break;
      }
    }
    
    if (ThreadIDReg)
      return DAG.getCopyFromReg(Chain, DL, ThreadIDReg, VT);
      
    // If not a kernel or R14 not found, return 0
    SDValue Res = DAG.getConstant(0, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }
  case Intrinsic::cgp1_thread_id_y: {
    SDValue Res = DAG.getConstant(0, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }
  case Intrinsic::cgp1_thread_id_z: {
    SDValue Res = DAG.getConstant(0, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }

  //===--------------------------------------------------------------------===//
  // Block ID intrinsics (block index within grid)
  //===--------------------------------------------------------------------===//
  case Intrinsic::cgp1_block_id_x: {
    // Read from R11 which holds block ID in kernels
    MachineFunction &MF = DAG.getMachineFunction();
    MachineRegisterInfo &RegInfo = MF.getRegInfo();
    Register BlockIDReg = 0;
    
    for (const auto &LI : RegInfo.liveins()) {
      if (LI.first == CGP1::R11) {
        BlockIDReg = LI.second;
        break;
      }
    }
    
    if (BlockIDReg)
      return DAG.getCopyFromReg(Chain, DL, BlockIDReg, VT);

    SDValue Res = DAG.getConstant(0, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }
  case Intrinsic::cgp1_block_id_y: {
    SDValue Res = DAG.getConstant(0, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }
  case Intrinsic::cgp1_block_id_z: {
    SDValue Res = DAG.getConstant(0, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }

  //===--------------------------------------------------------------------===//
  // Block dimension intrinsics (threads per block)
  //===--------------------------------------------------------------------===//
  case Intrinsic::cgp1_block_dim_x: {
    // Read from R10 which holds block dim in kernels
    MachineFunction &MF = DAG.getMachineFunction();
    MachineRegisterInfo &RegInfo = MF.getRegInfo();
    Register BlockDimReg = 0;
    
    for (const auto &LI : RegInfo.liveins()) {
      if (LI.first == CGP1::R10) {
        BlockDimReg = LI.second;
        break;
      }
    }
    
    if (BlockDimReg)
      return DAG.getCopyFromReg(Chain, DL, BlockDimReg, VT);

    // Default block size of 1 (single thread) for now
    SDValue Res = DAG.getConstant(1, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }
  case Intrinsic::cgp1_block_dim_y: {
    SDValue Res = DAG.getConstant(1, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }
  case Intrinsic::cgp1_block_dim_z: {
    SDValue Res = DAG.getConstant(1, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }

  //===--------------------------------------------------------------------===//
  // Grid dimension intrinsics (blocks per grid)
  //===--------------------------------------------------------------------===//
  case Intrinsic::cgp1_grid_dim_x: {
    // Read from R9 which holds grid dim in kernels
    MachineFunction &MF = DAG.getMachineFunction();
    MachineRegisterInfo &RegInfo = MF.getRegInfo();
    Register GridDimReg = 0;
    
    for (const auto &LI : RegInfo.liveins()) {
      if (LI.first == CGP1::R9) {
        GridDimReg = LI.second;
        break;
      }
    }
    
    if (GridDimReg)
      return DAG.getCopyFromReg(Chain, DL, GridDimReg, VT);

    // Default grid size of 1 (single block) for now
    SDValue Res = DAG.getConstant(1, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }
  case Intrinsic::cgp1_grid_dim_y: {
    SDValue Res = DAG.getConstant(1, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }
  case Intrinsic::cgp1_grid_dim_z: {
    SDValue Res = DAG.getConstant(1, DL, VT);
    if (Op.getOpcode() == ISD::INTRINSIC_W_CHAIN)
      return DAG.getMergeValues({Res, Chain}, DL);
    return Res;
  }

  //===--------------------------------------------------------------------===//
  // Shared Memory Intrinsics
  //===--------------------------------------------------------------------===//
  case Intrinsic::cgp1_shared_load: {
    // shared_load(addr) -> emit SHARED_LOAD instruction
    // Op: [0] = Chain, [1] = IID, [2] = Address
    SDValue Addr = Op.getOperand(2);
    // Lower to target-specific node
    return DAG.getNode(CGP1ISD::SharedLoad, DL, DAG.getVTList(VT, MVT::Other), Addr, Chain);
  }
  case Intrinsic::cgp1_shared_store: {
    SDValue Addr = Op.getOperand(2);
    SDValue Val = Op.getOperand(3);
    // Lower to target-specific node
    return DAG.getNode(CGP1ISD::SharedStore, DL, MVT::Other, Addr, Val, Chain);
  }

  //===--------------------------------------------------------------------===//
  // Atomic Intrinsics
  //===--------------------------------------------------------------------===//
  case Intrinsic::cgp1_atomic_add: {
    SDValue Addr = Op.getOperand(2);
    SDValue Val = Op.getOperand(3);
    // atomic_add $rd, $addr, $val
    return DAG.getNode(CGP1ISD::AtomicAdd, DL, DAG.getVTList(VT, MVT::Other), Addr, Val, Chain);
  }
  case Intrinsic::cgp1_atomic_cas: {
    SDValue Addr = Op.getOperand(2);
    SDValue Cmp = Op.getOperand(3);
    SDValue Val = Op.getOperand(4);
    // atomic_cas $rd, $addr, $cmp, $val
    return DAG.getNode(CGP1ISD::AtomicCas, DL, DAG.getVTList(VT, MVT::Other), Addr, Cmp, Val, Chain);
  }
  case Intrinsic::cgp1_atomic_max: {
    SDValue Addr = Op.getOperand(2);
    SDValue Val = Op.getOperand(3);
    // atomic_max $rd, $addr, $val
    return DAG.getNode(CGP1ISD::AtomicMax, DL, DAG.getVTList(VT, MVT::Other), Addr, Val, Chain);
  }
  case Intrinsic::cgp1_atomic_min: {
    SDValue Addr = Op.getOperand(2);
    SDValue Val = Op.getOperand(3);
    // atomic_min $rd, $addr, $val
    return DAG.getNode(CGP1ISD::AtomicMin, DL, DAG.getVTList(VT, MVT::Other), Addr, Val, Chain);
  }
  case Intrinsic::cgp1_atomic_exch: {
    SDValue Addr = Op.getOperand(2);
    SDValue Val = Op.getOperand(3);
    // atomic_exch $rd, $addr, $val
    return DAG.getNode(CGP1ISD::AtomicExch, DL, DAG.getVTList(VT, MVT::Other), Addr, Val, Chain);
  }

  //===--------------------------------------------------------------------===//
  // Warp-Level Intrinsics
  //===--------------------------------------------------------------------===//
  case Intrinsic::cgp1_warp_size:
    return DAG.getNode(CGP1ISD::WarpSize, DL, VT);
  case Intrinsic::cgp1_lane_id:
    return DAG.getNode(CGP1ISD::LaneId, DL, VT);
  case Intrinsic::cgp1_warp_all: {
    SDValue Pred = Op.getOperand(1);
    return DAG.getNode(CGP1ISD::WarpAll, DL, VT, Pred);
  }
  case Intrinsic::cgp1_warp_any: {
    SDValue Pred = Op.getOperand(1);
    return DAG.getNode(CGP1ISD::WarpAny, DL, VT, Pred);
  }
  case Intrinsic::cgp1_warp_ballot: {
    SDValue Pred = Op.getOperand(1);
    return DAG.getNode(CGP1ISD::WarpBallot, DL, VT, Pred);
  }
  case Intrinsic::cgp1_warp_shuffle: {
    SDValue Val = Op.getOperand(1);
    SDValue Lane = Op.getOperand(2);
    return DAG.getNode(CGP1ISD::WarpShuffle, DL, VT, Val, Lane);
  }

  default:
    return SDValue();
  }
}

SDValue CGP1TargetLowering::LowerFP_TO_SINT(SDValue Op,
                                            SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue FPVal = Op.getOperand(0);
  EVT VT = Op.getValueType();
  
  // For FP_TO_SINT, we need to handle signed conversion
  // Strategy: Convert to unsigned first, then adjust for sign
  // 1. Check if value is negative (compare with 0.0)
  // 2. If negative: convert abs(value) to uint, then negate
  // 3. If positive: convert directly to uint
  
  SDValue Zero = DAG.getConstantFP(0.0f, DL, MVT::f32);
  
  // Compare FP value with zero to check sign
  SDValue IsNegative = DAG.getNode(ISD::SETCC, DL, MVT::i32,
                                    FPVal, Zero, DAG.getCondCode(ISD::SETLT));
  
  // Get absolute value: if negative, negate; otherwise use as-is
  SDValue AbsVal = DAG.getNode(ISD::SELECT, DL, MVT::f32, IsNegative,
                               DAG.getNode(ISD::FNEG, DL, MVT::f32, FPVal),
                               FPVal);
  
  // Convert absolute value to unsigned integer
  SDValue UIntVal = DAG.getNode(ISD::FP_TO_UINT, DL, VT, AbsVal);
  
  // Negate if original value was negative
  SDValue NegUInt = DAG.getNode(ISD::SUB, DL, VT,
                                 DAG.getConstant(0, DL, VT), UIntVal);
  
  // Select: if negative, use negated value; otherwise use unsigned value
  return DAG.getNode(ISD::SELECT, DL, VT, IsNegative, NegUInt, UIntVal);
}

SDValue CGP1TargetLowering::LowerSINT_TO_FP(SDValue Op,
                                            SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue IntVal = Op.getOperand(0);
  EVT VT = Op.getValueType();
  
  // For SINT_TO_FP, we need to handle signed conversion
  // Strategy: Convert unsigned absolute value, then apply sign
  // 1. Check if integer is negative
  // 2. If negative: convert abs(value) to float, then negate
  // 3. If positive: convert directly to float
  
  SDValue Zero = DAG.getConstant(0, DL, MVT::i32);
  
  // Check if value is negative
  SDValue IsNegative = DAG.getNode(ISD::SETCC, DL, MVT::i32,
                                    IntVal, Zero, DAG.getCondCode(ISD::SETLT));
  
  // Get absolute value: if negative, negate; otherwise use as-is
  SDValue AbsVal = DAG.getNode(ISD::SELECT, DL, MVT::i32, IsNegative,
                               DAG.getNode(ISD::SUB, DL, MVT::i32, Zero, IntVal),
                               IntVal);
  
  // Convert absolute value to float (unsigned)
  SDValue UFloatVal = DAG.getNode(ISD::UINT_TO_FP, DL, VT, AbsVal);
  
  // Negate if original value was negative
  SDValue NegFloat = DAG.getNode(ISD::FNEG, DL, VT, UFloatVal);
  
  // Select: if negative, use negated value; otherwise use unsigned float value
  return DAG.getNode(ISD::SELECT, DL, VT, IsNegative, NegFloat, UFloatVal);
}

//===----------------------------------------------------------------------===//
// Rotate Operations
//===----------------------------------------------------------------------===//

SDValue CGP1TargetLowering::LowerROTL(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Val = Op.getOperand(0);
  SDValue Amt = Op.getOperand(1);
  EVT VT = Op.getValueType();
  
  // ROTL(x, a) = (x << a) | (x >> (32 - a))
  // For 32-bit: ROTL(x, a) = (x << a) | (x >> (32 - a))
  SDValue Bits = DAG.getConstant(32, DL, MVT::i32);
  SDValue SubAmt = DAG.getNode(ISD::SUB, DL, VT, Bits, Amt);
  SDValue Shl = DAG.getNode(ISD::SHL, DL, VT, Val, Amt);
  SDValue Shr = DAG.getNode(ISD::SRL, DL, VT, Val, SubAmt);
  return DAG.getNode(ISD::OR, DL, VT, Shl, Shr);
}

SDValue CGP1TargetLowering::LowerROTR(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Val = Op.getOperand(0);
  SDValue Amt = Op.getOperand(1);
  EVT VT = Op.getValueType();
  
  // ROTR(x, a) = (x >> a) | (x << (32 - a))
  SDValue Bits = DAG.getConstant(32, DL, MVT::i32);
  SDValue SubAmt = DAG.getNode(ISD::SUB, DL, VT, Bits, Amt);
  SDValue Shr = DAG.getNode(ISD::SRL, DL, VT, Val, Amt);
  SDValue Shl = DAG.getNode(ISD::SHL, DL, VT, Val, SubAmt);
  return DAG.getNode(ISD::OR, DL, VT, Shr, Shl);
}

//===----------------------------------------------------------------------===//
// Bit Counting Operations
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Multiply High Operations
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Type Conversion Operations
//===----------------------------------------------------------------------===//

SDValue CGP1TargetLowering::LowerSIGN_EXTEND(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Val = Op.getOperand(0);
  EVT VT = Op.getValueType();
  EVT SrcVT = Val.getValueType();
  
  // Sign extend from smaller type to i32
  if (SrcVT == MVT::i8) {
    // Sign extend i8 to i32: shift left 24, then arithmetic shift right 24
    SDValue Shl = DAG.getNode(ISD::SHL, DL, VT, Val, DAG.getConstant(24, DL, MVT::i32));
    return DAG.getNode(ISD::SRA, DL, VT, Shl, DAG.getConstant(24, DL, MVT::i32));
  } else if (SrcVT == MVT::i16) {
    // Sign extend i16 to i32: shift left 16, then arithmetic shift right 16
    SDValue Shl = DAG.getNode(ISD::SHL, DL, VT, Val, DAG.getConstant(16, DL, MVT::i32));
    return DAG.getNode(ISD::SRA, DL, VT, Shl, DAG.getConstant(16, DL, MVT::i32));
  }
  
  // For other cases, let LLVM expand
  return SDValue();
}

SDValue CGP1TargetLowering::LowerZERO_EXTEND(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Val = Op.getOperand(0);
  EVT VT = Op.getValueType();
  EVT SrcVT = Val.getValueType();
  
  // Zero extend from smaller type to i32
  if (SrcVT == MVT::i8) {
    // Zero extend i8 to i32: AND with 0xFF
    return DAG.getNode(ISD::AND, DL, VT, Val, DAG.getConstant(0xFF, DL, MVT::i32));
  } else if (SrcVT == MVT::i16) {
    // Zero extend i16 to i32: AND with 0xFFFF
    return DAG.getNode(ISD::AND, DL, VT, Val, DAG.getConstant(0xFFFF, DL, MVT::i32));
  }
  
  return SDValue();
}

SDValue CGP1TargetLowering::LowerANY_EXTEND(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Val = Op.getOperand(0);
  EVT VT = Op.getValueType();
  EVT SrcVT = Val.getValueType();
  
  // ANY_EXTEND is the same as ZERO_EXTEND for unsigned types
  // For signed types, it's the same as SIGN_EXTEND
  // We'll use ZERO_EXTEND logic (simpler)
  return LowerZERO_EXTEND(Op, DAG);
}

SDValue CGP1TargetLowering::LowerTRUNCATE(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Val = Op.getOperand(0);
  EVT VT = Op.getValueType();
  EVT SrcVT = Val.getValueType();
  
  // Truncate from larger type to smaller
  // For i32 to i8/i16, we can use AND mask to clear upper bits
  // The result will be i32 with masked bits, and LLVM's register allocator
  // will handle the actual type conversion
  if (SrcVT == MVT::i32) {
    if (VT == MVT::i8) {
      // Truncate i32 to i8: AND with 0xFF
      // Return as i32 (masked), register allocator handles i8
      return DAG.getNode(ISD::AND, DL, MVT::i32, Val, DAG.getConstant(0xFF, DL, MVT::i32));
    } else if (VT == MVT::i16) {
      // Truncate i32 to i16: AND with 0xFFFF
      return DAG.getNode(ISD::AND, DL, MVT::i32, Val, DAG.getConstant(0xFFFF, DL, MVT::i32));
    }
  }
  
  // For other cases (like i64->i32), let LLVM handle it
  return SDValue();
}

SDValue CGP1TargetLowering::LowerSIGN_EXTEND_INREG(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Val = Op.getOperand(0);
  EVT ExtVT = cast<VTSDNode>(Op.getOperand(1))->getVT();
  EVT VT = Op.getValueType();
  
  // Sign extend in register: extend Val from ExtVT bits to full width
  unsigned ExtBits = ExtVT.getSizeInBits();
  unsigned VTBits = VT.getSizeInBits();
  unsigned ShiftAmt = VTBits - ExtBits;
  
  // Shift left, then arithmetic shift right
  SDValue Shl = DAG.getNode(ISD::SHL, DL, VT, Val, DAG.getConstant(ShiftAmt, DL, MVT::i32));
  return DAG.getNode(ISD::SRA, DL, VT, Shl, DAG.getConstant(ShiftAmt, DL, MVT::i32));
}

//===----------------------------------------------------------------------===//
// Floating-Point Operations
//===----------------------------------------------------------------------===//

SDValue CGP1TargetLowering::LowerFMA(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue A = Op.getOperand(0);
  SDValue B = Op.getOperand(1);
  SDValue C = Op.getOperand(2);
  EVT VT = Op.getValueType();
  
  // FMA: Fused Multiply-Add: a * b + c
  // We don't have native FMA, so expand to FMUL + FADD
  SDValue Mul = DAG.getNode(ISD::FMUL, DL, VT, A, B);
  return DAG.getNode(ISD::FADD, DL, VT, Mul, C);
}

SDValue CGP1TargetLowering::LowerFCOPYSIGN(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Mag = Op.getOperand(0);  // Magnitude
  SDValue Sign = Op.getOperand(1); // Sign source
  EVT VT = Op.getValueType();
  
  // FCOPYSIGN: Copy sign bit from Sign to Mag
  // Extract sign bit from Sign (bit 31 for f32)
  // Clear sign bit in Mag, then OR with sign bit from Sign
  
  // For f32, we can use bit manipulation via integer operations
  // Since CGP1 has unified register file, we can treat floats as integers
  
  // Use target-specific node to convert float to int bits
  // We'll use a workaround: extract sign using FABS and comparison
  SDValue MagAbs = DAG.getNode(ISD::FABS, DL, VT, Mag);
  
  // Check if sign is negative
  SDValue Zero = DAG.getConstantFP(0.0f, DL, VT);
  SDValue IsSignNeg = DAG.getNode(ISD::SETCC, DL, MVT::i32, Sign, Zero,
                                   DAG.getCondCode(ISD::SETLT));
  
  // If sign is negative, negate magnitude; otherwise use as-is
  SDValue NegMag = DAG.getNode(ISD::FNEG, DL, VT, MagAbs);
  return DAG.getNode(ISD::SELECT, DL, VT, IsSignNeg, NegMag, MagAbs);
}

SDValue CGP1TargetLowering::LowerFRINT(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Val = Op.getOperand(0);
  EVT VT = Op.getValueType();
  
  // FRINT: Round to nearest integer (ties to even)
  // We can use FROUND which is natively supported
  return DAG.getNode(ISD::FROUND, DL, VT, Val);
}

SDValue CGP1TargetLowering::LowerFNEARBYINT(SDValue Op, SelectionDAG &DAG) const {
  SDLoc DL(Op);
  SDValue Val = Op.getOperand(0);
  EVT VT = Op.getValueType();
  
  // FNEARBYINT: Round to nearest integer (same as FRINT but may not raise exceptions)
  // For our purposes, FNEARBYINT is the same as FRINT
  // We can use FROUND which is natively supported
  return DAG.getNode(ISD::FROUND, DL, VT, Val);
}
//- cgp1-isel-lowering-cpp-2