/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Target Register Enum Values                                                *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_REGINFO_ENUM
#undef GET_REGINFO_ENUM

namespace llvm {

class MCRegisterClass;
extern const MCRegisterClass CGP1MCRegisterClasses[];

//@s reg.h-regs
namespace CGP1 {
enum : unsigned {
  NoRegister,
  AT = 1,
  FP = 2,
  GP = 3,
  PC = 4,
  RA = 5,
  SP = 6,
  ZERO = 7,
  A0 = 8,
//- reg.h-regs
  A1 = 9,
  A2 = 10,
  A3 = 11,
  K0 = 12,
  K1 = 13,
  S0 = 14,
  S1 = 15,
  S2 = 16,
  S3 = 17,
  S4 = 18,
  S5 = 19,
  S6 = 20,
  S7 = 21,
  T0 = 22,
  T1 = 23,
  T2 = 24,
  T3 = 25,
  T4 = 26,
  T5 = 27,
  T6 = 28,
  T7 = 29,
  T8 = 30,
  T9 = 31,
  V0 = 32,
  V1 = 33,
  NUM_TARGET_REGS // 34
};
} // end namespace CGP1
 
//@s reg.h-rc
// Register classes

namespace CGP1 {
enum {
  GPR32RegClassID = 0,
  GPR32NONZERORegClassID = 1,
  CPURARegRegClassID = 2,
  CPUSPRegRegClassID = 3,
  GP32RegClassID = 4,
  GPR32ZERORegClassID = 5,

};
} // end namespace CGP1
//- reg.h-rc

// Register pressure sets enum.
namespace CGP1 {
enum RegisterPressureSets {
  GPR32ZERO = 0,
  GPR32 = 1,
};
} // end namespace CGP1

} // end namespace llvm

#endif // GET_REGINFO_ENUM

/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* MC Register Information                                                    *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_REGINFO_MC_DESC
#undef GET_REGINFO_MC_DESC

namespace llvm {

extern const int16_t CGP1RegDiffLists[] = {
  /* 0 */ 0,
};

extern const LaneBitmask CGP1LaneMaskLists[] = {
  /* 0 */ LaneBitmask(0xFFFFFFFFFFFFFFFF), 
};

extern const uint16_t CGP1SubRegIdxLists[] = {
  /* 0 */ 
  /* dummy */ 0
};


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverlength-strings"
#endif
extern const char CGP1RegStrings[] = {
  /* 0 */ "A0\000"
  /* 3 */ "K0\000"
  /* 6 */ "S0\000"
  /* 9 */ "T0\000"
  /* 12 */ "V0\000"
  /* 15 */ "A1\000"
  /* 18 */ "K1\000"
  /* 21 */ "S1\000"
  /* 24 */ "T1\000"
  /* 27 */ "V1\000"
  /* 30 */ "A2\000"
  /* 33 */ "S2\000"
  /* 36 */ "T2\000"
  /* 39 */ "A3\000"
  /* 42 */ "S3\000"
  /* 45 */ "T3\000"
  /* 48 */ "S4\000"
  /* 51 */ "T4\000"
  /* 54 */ "S5\000"
  /* 57 */ "T5\000"
  /* 60 */ "S6\000"
  /* 63 */ "T6\000"
  /* 66 */ "S7\000"
  /* 69 */ "T7\000"
  /* 72 */ "T8\000"
  /* 75 */ "T9\000"
  /* 78 */ "RA\000"
  /* 81 */ "PC\000"
  /* 84 */ "ZERO\000"
  /* 89 */ "FP\000"
  /* 92 */ "GP\000"
  /* 95 */ "SP\000"
  /* 98 */ "AT\000"
};
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

extern const MCRegisterDesc CGP1RegDesc[] = { // Descriptors
  { 2, 0, 0, 0, 0, 0, 0, 0 },
  { 98, 0, 0, 0, 0, 0, 0, 0 },
  { 89, 0, 0, 0, 1, 0, 0, 0 },
  { 92, 0, 0, 0, 2, 0, 0, 0 },
  { 81, 0, 0, 0, 3, 0, 0, 0 },
  { 78, 0, 0, 0, 4, 0, 0, 0 },
  { 95, 0, 0, 0, 5, 0, 0, 0 },
  { 84, 0, 0, 0, 6, 0, 1, 0 },
  { 0, 0, 0, 0, 7, 0, 0, 0 },
  { 15, 0, 0, 0, 8, 0, 0, 0 },
  { 30, 0, 0, 0, 9, 0, 0, 0 },
  { 39, 0, 0, 0, 10, 0, 0, 0 },
  { 3, 0, 0, 0, 11, 0, 0, 0 },
  { 18, 0, 0, 0, 12, 0, 0, 0 },
  { 6, 0, 0, 0, 13, 0, 0, 0 },
  { 21, 0, 0, 0, 14, 0, 0, 0 },
  { 33, 0, 0, 0, 15, 0, 0, 0 },
  { 42, 0, 0, 0, 16, 0, 0, 0 },
  { 48, 0, 0, 0, 17, 0, 0, 0 },
  { 54, 0, 0, 0, 18, 0, 0, 0 },
  { 60, 0, 0, 0, 19, 0, 0, 0 },
  { 66, 0, 0, 0, 20, 0, 0, 0 },
  { 9, 0, 0, 0, 21, 0, 0, 0 },
  { 24, 0, 0, 0, 22, 0, 0, 0 },
  { 36, 0, 0, 0, 23, 0, 0, 0 },
  { 45, 0, 0, 0, 24, 0, 0, 0 },
  { 51, 0, 0, 0, 25, 0, 0, 0 },
  { 57, 0, 0, 0, 26, 0, 0, 0 },
  { 63, 0, 0, 0, 27, 0, 0, 0 },
  { 69, 0, 0, 0, 28, 0, 0, 0 },
  { 72, 0, 0, 0, 29, 0, 0, 0 },
  { 75, 0, 0, 0, 30, 0, 0, 0 },
  { 12, 0, 0, 0, 31, 0, 0, 0 },
  { 27, 0, 0, 0, 32, 0, 0, 0 },
};

extern const MCPhysReg CGP1RegUnitRoots[][2] = {
  { CGP1::AT },
  { CGP1::FP },
  { CGP1::GP },
  { CGP1::PC },
  { CGP1::RA },
  { CGP1::SP },
  { CGP1::ZERO },
  { CGP1::A0 },
  { CGP1::A1 },
  { CGP1::A2 },
  { CGP1::A3 },
  { CGP1::K0 },
  { CGP1::K1 },
  { CGP1::S0 },
  { CGP1::S1 },
  { CGP1::S2 },
  { CGP1::S3 },
  { CGP1::S4 },
  { CGP1::S5 },
  { CGP1::S6 },
  { CGP1::S7 },
  { CGP1::T0 },
  { CGP1::T1 },
  { CGP1::T2 },
  { CGP1::T3 },
  { CGP1::T4 },
  { CGP1::T5 },
  { CGP1::T6 },
  { CGP1::T7 },
  { CGP1::T8 },
  { CGP1::T9 },
  { CGP1::V0 },
  { CGP1::V1 },
};

namespace {     // Register classes...
  // GPR32 Register Class...
  const MCPhysReg GPR32[] = {
    CGP1::ZERO, CGP1::AT, CGP1::V0, CGP1::V1, CGP1::A0, CGP1::A1, CGP1::A2, CGP1::A3, CGP1::T0, CGP1::T1, CGP1::T2, CGP1::T3, CGP1::T4, CGP1::T5, CGP1::T6, CGP1::T7, CGP1::S0, CGP1::S1, CGP1::S2, CGP1::S3, CGP1::S4, CGP1::S5, CGP1::S6, CGP1::S7, CGP1::T8, CGP1::T9, CGP1::K0, CGP1::K1, CGP1::GP, CGP1::SP, CGP1::FP, CGP1::RA, 
  };

  // GPR32 Bit set.
  const uint8_t GPR32Bits[] = {
    0xee, 0xff, 0xff, 0xff, 0x03, 
  };

  // GPR32NONZERO Register Class...
  const MCPhysReg GPR32NONZERO[] = {
    CGP1::AT, CGP1::V0, CGP1::V1, CGP1::A0, CGP1::A1, CGP1::A2, CGP1::A3, CGP1::T0, CGP1::T1, CGP1::T2, CGP1::T3, CGP1::T4, CGP1::T5, CGP1::T6, CGP1::T7, CGP1::S0, CGP1::S1, CGP1::S2, CGP1::S3, CGP1::S4, CGP1::S5, CGP1::S6, CGP1::S7, CGP1::T8, CGP1::T9, CGP1::K0, CGP1::K1, CGP1::GP, CGP1::SP, CGP1::FP, CGP1::RA, 
  };

  // GPR32NONZERO Bit set.
  const uint8_t GPR32NONZEROBits[] = {
    0x6e, 0xff, 0xff, 0xff, 0x03, 
  };

  // CPURAReg Register Class...
  const MCPhysReg CPURAReg[] = {
    CGP1::RA, 
  };

  // CPURAReg Bit set.
  const uint8_t CPURARegBits[] = {
    0x20, 
  };

  // CPUSPReg Register Class...
  const MCPhysReg CPUSPReg[] = {
    CGP1::SP, 
  };

  // CPUSPReg Bit set.
  const uint8_t CPUSPRegBits[] = {
    0x40, 
  };

  // GP32 Register Class...
  const MCPhysReg GP32[] = {
    CGP1::GP, 
  };

  // GP32 Bit set.
  const uint8_t GP32Bits[] = {
    0x08, 
  };

  // GPR32ZERO Register Class...
  const MCPhysReg GPR32ZERO[] = {
    CGP1::ZERO, 
  };

  // GPR32ZERO Bit set.
  const uint8_t GPR32ZEROBits[] = {
    0x80, 
  };

} // end anonymous namespace


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverlength-strings"
#endif
extern const char CGP1RegClassStrings[] = {
  /* 0 */ "GP32\000"
  /* 5 */ "GPR32\000"
  /* 11 */ "GPR32ZERO\000"
  /* 21 */ "GPR32NONZERO\000"
  /* 34 */ "CPURAReg\000"
  /* 43 */ "CPUSPReg\000"
};
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

extern const MCRegisterClass CGP1MCRegisterClasses[] = {
  { GPR32, GPR32Bits, 5, 32, sizeof(GPR32Bits), CGP1::GPR32RegClassID, 32, 1, true, false },
  { GPR32NONZERO, GPR32NONZEROBits, 21, 31, sizeof(GPR32NONZEROBits), CGP1::GPR32NONZERORegClassID, 32, 1, true, false },
  { CPURAReg, CPURARegBits, 34, 1, sizeof(CPURARegBits), CGP1::CPURARegRegClassID, 32, 1, false, false },
  { CPUSPReg, CPUSPRegBits, 43, 1, sizeof(CPUSPRegBits), CGP1::CPUSPRegRegClassID, 32, 1, false, false },
  { GP32, GP32Bits, 0, 1, sizeof(GP32Bits), CGP1::GP32RegClassID, 32, 1, false, false },
  { GPR32ZERO, GPR32ZEROBits, 11, 1, sizeof(GPR32ZEROBits), CGP1::GPR32ZERORegClassID, 32, 1, true, false },
};

extern const uint16_t CGP1RegEncodingTable[] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
};
static inline void InitCGP1MCRegisterInfo(MCRegisterInfo *RI, unsigned RA, unsigned DwarfFlavour = 0, unsigned EHFlavour = 0, unsigned PC = 0) {
  RI->InitMCRegisterInfo(CGP1RegDesc, 34, RA, PC, CGP1MCRegisterClasses, 6, CGP1RegUnitRoots, 33, CGP1RegDiffLists, CGP1LaneMaskLists, CGP1RegStrings, CGP1RegClassStrings, CGP1SubRegIdxLists, 1,
CGP1RegEncodingTable);

}

} // end namespace llvm

#endif // GET_REGINFO_MC_DESC

/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Register Information Header Fragment                                       *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_REGINFO_HEADER
#undef GET_REGINFO_HEADER

#include "llvm/CodeGen/TargetRegisterInfo.h"

namespace llvm {

class CGP1FrameLowering;

struct CGP1GenRegisterInfo : public TargetRegisterInfo {
  explicit CGP1GenRegisterInfo(unsigned RA, unsigned D = 0, unsigned E = 0,
      unsigned PC = 0, unsigned HwMode = 0);
  const RegClassWeight &getRegClassWeight(const TargetRegisterClass *RC) const override;
  unsigned getRegUnitWeight(unsigned RegUnit) const override;
  unsigned getNumRegPressureSets() const override;
  const char *getRegPressureSetName(unsigned Idx) const override;
  unsigned getRegPressureSetLimit(const MachineFunction &MF, unsigned Idx) const override;
  const int *getRegClassPressureSets(const TargetRegisterClass *RC) const override;
  const int *getRegUnitPressureSets(unsigned RegUnit) const override;
  ArrayRef<const char *> getRegMaskNames() const override;
  ArrayRef<const uint32_t *> getRegMasks() const override;
  bool isGeneralPurposeRegister(const MachineFunction &, MCRegister) const override;
  bool isGeneralPurposeRegisterClass(const TargetRegisterClass *RC) const override;
  bool isFixedRegister(const MachineFunction &, MCRegister) const override;
  bool isArgumentRegister(const MachineFunction &, MCRegister) const override;
  bool isConstantPhysReg(MCRegister PhysReg) const override final;
  /// Devirtualized TargetFrameLowering.
  static const CGP1FrameLowering *getFrameLowering(
      const MachineFunction &MF);
};

namespace CGP1 { // Register classes
  extern const TargetRegisterClass GPR32RegClass;
  extern const TargetRegisterClass GPR32NONZERORegClass;
  extern const TargetRegisterClass CPURARegRegClass;
  extern const TargetRegisterClass CPUSPRegRegClass;
  extern const TargetRegisterClass GP32RegClass;
  extern const TargetRegisterClass GPR32ZERORegClass;
} // end namespace CGP1

} // end namespace llvm

#endif // GET_REGINFO_HEADER

/*===- TableGen'erated file -------------------------------------*- C++ -*-===*\
|*                                                                            *|
|* Target Register and Register Classes Information                           *|
|*                                                                            *|
|* Automatically generated file, do not edit!                                 *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/


#ifdef GET_REGINFO_TARGET_DESC
#undef GET_REGINFO_TARGET_DESC

namespace llvm {

extern const MCRegisterClass CGP1MCRegisterClasses[];

static const MVT::SimpleValueType VTLists[] = {
  /* 0 */ MVT::i32, MVT::Other,
};

static const char *SubRegIndexNameTable[] = { "" };

static const TargetRegisterInfo::SubRegCoveredBits SubRegIdxRangeTable[] = {
  { 65535, 65535 },
};


static const LaneBitmask SubRegIndexLaneMaskTable[] = {
  LaneBitmask::getAll(),
 };



static const TargetRegisterInfo::RegClassInfo RegClassInfos[] = {
  // Mode = 0 (Default)
  { 32, 32, 32, /*VTLists+*/0 },    // GPR32
  { 32, 32, 32, /*VTLists+*/0 },    // GPR32NONZERO
  { 32, 32, 32, /*VTLists+*/0 },    // CPURAReg
  { 32, 32, 32, /*VTLists+*/0 },    // CPUSPReg
  { 32, 32, 32, /*VTLists+*/0 },    // GP32
  { 32, 32, 32, /*VTLists+*/0 },    // GPR32ZERO
};
static const uint32_t GPR32SubClassMask[] = {
  0x0000003f, 
};

static const uint32_t GPR32NONZEROSubClassMask[] = {
  0x0000001e, 
};

static const uint32_t CPURARegSubClassMask[] = {
  0x00000004, 
};

static const uint32_t CPUSPRegSubClassMask[] = {
  0x00000008, 
};

static const uint32_t GP32SubClassMask[] = {
  0x00000010, 
};

static const uint32_t GPR32ZEROSubClassMask[] = {
  0x00000020, 
};

static const uint16_t SuperRegIdxSeqs[] = {
  /* 0 */ 0,
};

static unsigned const GPR32NONZEROSuperclasses[] = {
  CGP1::GPR32RegClassID,
};

static unsigned const CPURARegSuperclasses[] = {
  CGP1::GPR32RegClassID,
  CGP1::GPR32NONZERORegClassID,
};

static unsigned const CPUSPRegSuperclasses[] = {
  CGP1::GPR32RegClassID,
  CGP1::GPR32NONZERORegClassID,
};

static unsigned const GP32Superclasses[] = {
  CGP1::GPR32RegClassID,
  CGP1::GPR32NONZERORegClassID,
};

static unsigned const GPR32ZEROSuperclasses[] = {
  CGP1::GPR32RegClassID,
};


namespace CGP1 {   // Register class instances
  extern const TargetRegisterClass GPR32RegClass = {
    &CGP1MCRegisterClasses[GPR32RegClassID],
    GPR32SubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    nullptr, 0,
    nullptr
  };

  extern const TargetRegisterClass GPR32NONZERORegClass = {
    &CGP1MCRegisterClasses[GPR32NONZERORegClassID],
    GPR32NONZEROSubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    GPR32NONZEROSuperclasses,  1,
    nullptr
  };

  extern const TargetRegisterClass CPURARegRegClass = {
    &CGP1MCRegisterClasses[CPURARegRegClassID],
    CPURARegSubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    CPURARegSuperclasses,  2,
    nullptr
  };

  extern const TargetRegisterClass CPUSPRegRegClass = {
    &CGP1MCRegisterClasses[CPUSPRegRegClassID],
    CPUSPRegSubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    CPUSPRegSuperclasses,  2,
    nullptr
  };

  extern const TargetRegisterClass GP32RegClass = {
    &CGP1MCRegisterClasses[GP32RegClassID],
    GP32SubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    GP32Superclasses,  2,
    nullptr
  };

  extern const TargetRegisterClass GPR32ZERORegClass = {
    &CGP1MCRegisterClasses[GPR32ZERORegClassID],
    GPR32ZEROSubClassMask,
    SuperRegIdxSeqs + 0,
    LaneBitmask(0x0000000000000001),
    0,
    false,
    0x00, /* TSFlags */
    false, /* HasDisjunctSubRegs */
    false, /* CoveredBySubRegs */
    GPR32ZEROSuperclasses,  1,
    nullptr
  };

} // end namespace CGP1

namespace {
  const TargetRegisterClass *const RegisterClasses[] = {
    &CGP1::GPR32RegClass,
    &CGP1::GPR32NONZERORegClass,
    &CGP1::CPURARegRegClass,
    &CGP1::CPUSPRegRegClass,
    &CGP1::GP32RegClass,
    &CGP1::GPR32ZERORegClass,
  };
} // end anonymous namespace

static const uint8_t CostPerUseTable[] = { 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };


static const bool InAllocatableClassTable[] = { 
false, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, };


static const TargetRegisterInfoDesc CGP1RegInfoDesc = { // Extra Descriptors
CostPerUseTable, 1, InAllocatableClassTable};

/// Get the weight in units of pressure for this register class.
const RegClassWeight &CGP1GenRegisterInfo::
getRegClassWeight(const TargetRegisterClass *RC) const {
  static const RegClassWeight RCWeightTable[] = {
    {1, 32},  	// GPR32
    {1, 31},  	// GPR32NONZERO
    {1, 1},  	// CPURAReg
    {1, 1},  	// CPUSPReg
    {1, 1},  	// GP32
    {1, 1},  	// GPR32ZERO
  };
  return RCWeightTable[RC->getID()];
}

/// Get the weight in units of pressure for this register unit.
unsigned CGP1GenRegisterInfo::
getRegUnitWeight(unsigned RegUnit) const {
  assert(RegUnit < 33 && "invalid register unit");
  // All register units have unit weight.
  return 1;
}


// Get the number of dimensions of register pressure.
unsigned CGP1GenRegisterInfo::getNumRegPressureSets() const {
  return 2;
}

// Get the name of this register unit pressure set.
const char *CGP1GenRegisterInfo::
getRegPressureSetName(unsigned Idx) const {
  static const char *PressureNameTable[] = {
    "GPR32ZERO",
    "GPR32",
  };
  return PressureNameTable[Idx];
}

// Get the register unit pressure limit for this dimension.
// This limit must be adjusted dynamically for reserved registers.
unsigned CGP1GenRegisterInfo::
getRegPressureSetLimit(const MachineFunction &MF, unsigned Idx) const {
  static const uint8_t PressureLimitTable[] = {
    1,  	// 0: GPR32ZERO
    32,  	// 1: GPR32
  };
  return PressureLimitTable[Idx];
}

/// Table of pressure sets per register class or unit.
static const int RCSetsTable[] = {
  /* 0 */ 0, 1, -1,
};

/// Get the dimensions of register pressure impacted by this register class.
/// Returns a -1 terminated array of pressure set IDs
const int *CGP1GenRegisterInfo::
getRegClassPressureSets(const TargetRegisterClass *RC) const {
  static const uint8_t RCSetStartTable[] = {
    1,1,2,2,2,0,};
  return &RCSetsTable[RCSetStartTable[RC->getID()]];
}

/// Get the dimensions of register pressure impacted by this register unit.
/// Returns a -1 terminated array of pressure set IDs
const int *CGP1GenRegisterInfo::
getRegUnitPressureSets(unsigned RegUnit) const {
  assert(RegUnit < 33 && "invalid register unit");
  static const uint8_t RUSetStartTable[] = {
    1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,};
  return &RCSetsTable[RUSetStartTable[RegUnit]];
}

extern const MCRegisterDesc CGP1RegDesc[];
extern const int16_t CGP1RegDiffLists[];
extern const LaneBitmask CGP1LaneMaskLists[];
extern const char CGP1RegStrings[];
extern const char CGP1RegClassStrings[];
extern const MCPhysReg CGP1RegUnitRoots[][2];
extern const uint16_t CGP1SubRegIdxLists[];
extern const uint16_t CGP1RegEncodingTable[];
CGP1GenRegisterInfo::
CGP1GenRegisterInfo(unsigned RA, unsigned DwarfFlavour, unsigned EHFlavour,
      unsigned PC, unsigned HwMode)
  : TargetRegisterInfo(&CGP1RegInfoDesc, RegisterClasses, RegisterClasses+6,
             SubRegIndexNameTable, SubRegIdxRangeTable, SubRegIndexLaneMaskTable,
             LaneBitmask(0xFFFFFFFFFFFFFFFF), RegClassInfos, VTLists, HwMode) {
  InitMCRegisterInfo(CGP1RegDesc, 34, RA, PC,
                     CGP1MCRegisterClasses, 6,
                     CGP1RegUnitRoots,
                     33,
                     CGP1RegDiffLists,
                     CGP1LaneMaskLists,
                     CGP1RegStrings,
                     CGP1RegClassStrings,
                     CGP1SubRegIdxLists,
                     1,
                     CGP1RegEncodingTable);

}



ArrayRef<const uint32_t *> CGP1GenRegisterInfo::getRegMasks() const {
  return {};
}

bool CGP1GenRegisterInfo::
isGeneralPurposeRegister(const MachineFunction &MF, MCRegister PhysReg) const {
  return
      false;
}

bool CGP1GenRegisterInfo::
isGeneralPurposeRegisterClass(const TargetRegisterClass *RC) const {
  return
      false;
}

bool CGP1GenRegisterInfo::
isFixedRegister(const MachineFunction &MF, MCRegister PhysReg) const {
  return
      false;
}

bool CGP1GenRegisterInfo::
isArgumentRegister(const MachineFunction &MF, MCRegister PhysReg) const {
  return
      false;
}

bool CGP1GenRegisterInfo::
isConstantPhysReg(MCRegister PhysReg) const {
  return
      PhysReg == CGP1::ZERO ||
      false;
}

ArrayRef<const char *> CGP1GenRegisterInfo::getRegMaskNames() const {
  return {};
}

const CGP1FrameLowering *
CGP1GenRegisterInfo::getFrameLowering(const MachineFunction &MF) {
  return static_cast<const CGP1FrameLowering *>(
      MF.getSubtarget().getFrameLowering());
}

} // end namespace llvm

#endif // GET_REGINFO_TARGET_DESC

