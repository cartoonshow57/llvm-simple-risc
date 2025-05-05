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
extern const MCRegisterClass NovaMCRegisterClasses[];

namespace Nova {
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
} // end namespace Nova

// Register classes

namespace Nova {
enum {
  GPR32RegClassID = 0,
  GPR32NONZERORegClassID = 1,
  CPURARegRegClassID = 2,
  CPUSPRegRegClassID = 3,
  GP32RegClassID = 4,
  GPR32ZERORegClassID = 5,

};
} // end namespace Nova

// Register pressure sets enum.
namespace Nova {
enum RegisterPressureSets {
  GPR32ZERO = 0,
  GPR32 = 1,
};
} // end namespace Nova

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

extern const int16_t NovaRegDiffLists[] = {
  /* 0 */ 0,
};

extern const LaneBitmask NovaLaneMaskLists[] = {
  /* 0 */ LaneBitmask(0xFFFFFFFFFFFFFFFF), 
};

extern const uint16_t NovaSubRegIdxLists[] = {
  /* 0 */ 
  /* dummy */ 0
};


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Woverlength-strings"
#endif
extern const char NovaRegStrings[] = {
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

extern const MCRegisterDesc NovaRegDesc[] = { // Descriptors
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

extern const MCPhysReg NovaRegUnitRoots[][2] = {
  { Nova::AT },
  { Nova::FP },
  { Nova::GP },
  { Nova::PC },
  { Nova::RA },
  { Nova::SP },
  { Nova::ZERO },
  { Nova::A0 },
  { Nova::A1 },
  { Nova::A2 },
  { Nova::A3 },
  { Nova::K0 },
  { Nova::K1 },
  { Nova::S0 },
  { Nova::S1 },
  { Nova::S2 },
  { Nova::S3 },
  { Nova::S4 },
  { Nova::S5 },
  { Nova::S6 },
  { Nova::S7 },
  { Nova::T0 },
  { Nova::T1 },
  { Nova::T2 },
  { Nova::T3 },
  { Nova::T4 },
  { Nova::T5 },
  { Nova::T6 },
  { Nova::T7 },
  { Nova::T8 },
  { Nova::T9 },
  { Nova::V0 },
  { Nova::V1 },
};

namespace {     // Register classes...
  // GPR32 Register Class...
  const MCPhysReg GPR32[] = {
    Nova::ZERO, Nova::AT, Nova::V0, Nova::V1, Nova::A0, Nova::A1, Nova::A2, Nova::A3, Nova::T0, Nova::T1, Nova::T2, Nova::T3, Nova::T4, Nova::T5, Nova::T6, Nova::T7, Nova::S0, Nova::S1, Nova::S2, Nova::S3, Nova::S4, Nova::S5, Nova::S6, Nova::S7, Nova::T8, Nova::T9, Nova::K0, Nova::K1, Nova::GP, Nova::SP, Nova::FP, Nova::RA, 
  };

  // GPR32 Bit set.
  const uint8_t GPR32Bits[] = {
    0xee, 0xff, 0xff, 0xff, 0x03, 
  };

  // GPR32NONZERO Register Class...
  const MCPhysReg GPR32NONZERO[] = {
    Nova::AT, Nova::V0, Nova::V1, Nova::A0, Nova::A1, Nova::A2, Nova::A3, Nova::T0, Nova::T1, Nova::T2, Nova::T3, Nova::T4, Nova::T5, Nova::T6, Nova::T7, Nova::S0, Nova::S1, Nova::S2, Nova::S3, Nova::S4, Nova::S5, Nova::S6, Nova::S7, Nova::T8, Nova::T9, Nova::K0, Nova::K1, Nova::GP, Nova::SP, Nova::FP, Nova::RA, 
  };

  // GPR32NONZERO Bit set.
  const uint8_t GPR32NONZEROBits[] = {
    0x6e, 0xff, 0xff, 0xff, 0x03, 
  };

  // CPURAReg Register Class...
  const MCPhysReg CPURAReg[] = {
    Nova::RA, 
  };

  // CPURAReg Bit set.
  const uint8_t CPURARegBits[] = {
    0x20, 
  };

  // CPUSPReg Register Class...
  const MCPhysReg CPUSPReg[] = {
    Nova::SP, 
  };

  // CPUSPReg Bit set.
  const uint8_t CPUSPRegBits[] = {
    0x40, 
  };

  // GP32 Register Class...
  const MCPhysReg GP32[] = {
    Nova::GP, 
  };

  // GP32 Bit set.
  const uint8_t GP32Bits[] = {
    0x08, 
  };

  // GPR32ZERO Register Class...
  const MCPhysReg GPR32ZERO[] = {
    Nova::ZERO, 
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
extern const char NovaRegClassStrings[] = {
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

extern const MCRegisterClass NovaMCRegisterClasses[] = {
  { GPR32, GPR32Bits, 5, 32, sizeof(GPR32Bits), Nova::GPR32RegClassID, 32, 1, true, false },
  { GPR32NONZERO, GPR32NONZEROBits, 21, 31, sizeof(GPR32NONZEROBits), Nova::GPR32NONZERORegClassID, 32, 1, true, false },
  { CPURAReg, CPURARegBits, 34, 1, sizeof(CPURARegBits), Nova::CPURARegRegClassID, 32, 1, false, false },
  { CPUSPReg, CPUSPRegBits, 43, 1, sizeof(CPUSPRegBits), Nova::CPUSPRegRegClassID, 32, 1, false, false },
  { GP32, GP32Bits, 0, 1, sizeof(GP32Bits), Nova::GP32RegClassID, 32, 1, false, false },
  { GPR32ZERO, GPR32ZEROBits, 11, 1, sizeof(GPR32ZEROBits), Nova::GPR32ZERORegClassID, 32, 1, true, false },
};

extern const uint16_t NovaRegEncodingTable[] = {
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
static inline void InitNovaMCRegisterInfo(MCRegisterInfo *RI, unsigned RA, unsigned DwarfFlavour = 0, unsigned EHFlavour = 0, unsigned PC = 0) {
  RI->InitMCRegisterInfo(NovaRegDesc, 34, RA, PC, NovaMCRegisterClasses, 6, NovaRegUnitRoots, 33, NovaRegDiffLists, NovaLaneMaskLists, NovaRegStrings, NovaRegClassStrings, NovaSubRegIdxLists, 1,
NovaRegEncodingTable);

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

class NovaFrameLowering;

struct NovaGenRegisterInfo : public TargetRegisterInfo {
  explicit NovaGenRegisterInfo(unsigned RA, unsigned D = 0, unsigned E = 0,
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
  static const NovaFrameLowering *getFrameLowering(
      const MachineFunction &MF);
};

namespace Nova { // Register classes
  extern const TargetRegisterClass GPR32RegClass;
  extern const TargetRegisterClass GPR32NONZERORegClass;
  extern const TargetRegisterClass CPURARegRegClass;
  extern const TargetRegisterClass CPUSPRegRegClass;
  extern const TargetRegisterClass GP32RegClass;
  extern const TargetRegisterClass GPR32ZERORegClass;
} // end namespace Nova

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

extern const MCRegisterClass NovaMCRegisterClasses[];

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
  Nova::GPR32RegClassID,
};

static unsigned const CPURARegSuperclasses[] = {
  Nova::GPR32RegClassID,
  Nova::GPR32NONZERORegClassID,
};

static unsigned const CPUSPRegSuperclasses[] = {
  Nova::GPR32RegClassID,
  Nova::GPR32NONZERORegClassID,
};

static unsigned const GP32Superclasses[] = {
  Nova::GPR32RegClassID,
  Nova::GPR32NONZERORegClassID,
};

static unsigned const GPR32ZEROSuperclasses[] = {
  Nova::GPR32RegClassID,
};


namespace Nova {   // Register class instances
  extern const TargetRegisterClass GPR32RegClass = {
    &NovaMCRegisterClasses[GPR32RegClassID],
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
    &NovaMCRegisterClasses[GPR32NONZERORegClassID],
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
    &NovaMCRegisterClasses[CPURARegRegClassID],
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
    &NovaMCRegisterClasses[CPUSPRegRegClassID],
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
    &NovaMCRegisterClasses[GP32RegClassID],
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
    &NovaMCRegisterClasses[GPR32ZERORegClassID],
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

} // end namespace Nova

namespace {
  const TargetRegisterClass *const RegisterClasses[] = {
    &Nova::GPR32RegClass,
    &Nova::GPR32NONZERORegClass,
    &Nova::CPURARegRegClass,
    &Nova::CPUSPRegRegClass,
    &Nova::GP32RegClass,
    &Nova::GPR32ZERORegClass,
  };
} // end anonymous namespace

static const uint8_t CostPerUseTable[] = { 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };


static const bool InAllocatableClassTable[] = { 
false, true, true, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, };


static const TargetRegisterInfoDesc NovaRegInfoDesc = { // Extra Descriptors
CostPerUseTable, 1, InAllocatableClassTable};

/// Get the weight in units of pressure for this register class.
const RegClassWeight &NovaGenRegisterInfo::
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
unsigned NovaGenRegisterInfo::
getRegUnitWeight(unsigned RegUnit) const {
  assert(RegUnit < 33 && "invalid register unit");
  // All register units have unit weight.
  return 1;
}


// Get the number of dimensions of register pressure.
unsigned NovaGenRegisterInfo::getNumRegPressureSets() const {
  return 2;
}

// Get the name of this register unit pressure set.
const char *NovaGenRegisterInfo::
getRegPressureSetName(unsigned Idx) const {
  static const char *PressureNameTable[] = {
    "GPR32ZERO",
    "GPR32",
  };
  return PressureNameTable[Idx];
}

// Get the register unit pressure limit for this dimension.
// This limit must be adjusted dynamically for reserved registers.
unsigned NovaGenRegisterInfo::
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
const int *NovaGenRegisterInfo::
getRegClassPressureSets(const TargetRegisterClass *RC) const {
  static const uint8_t RCSetStartTable[] = {
    1,1,2,2,2,0,};
  return &RCSetsTable[RCSetStartTable[RC->getID()]];
}

/// Get the dimensions of register pressure impacted by this register unit.
/// Returns a -1 terminated array of pressure set IDs
const int *NovaGenRegisterInfo::
getRegUnitPressureSets(unsigned RegUnit) const {
  assert(RegUnit < 33 && "invalid register unit");
  static const uint8_t RUSetStartTable[] = {
    1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,};
  return &RCSetsTable[RUSetStartTable[RegUnit]];
}

extern const MCRegisterDesc NovaRegDesc[];
extern const int16_t NovaRegDiffLists[];
extern const LaneBitmask NovaLaneMaskLists[];
extern const char NovaRegStrings[];
extern const char NovaRegClassStrings[];
extern const MCPhysReg NovaRegUnitRoots[][2];
extern const uint16_t NovaSubRegIdxLists[];
extern const uint16_t NovaRegEncodingTable[];
NovaGenRegisterInfo::
NovaGenRegisterInfo(unsigned RA, unsigned DwarfFlavour, unsigned EHFlavour,
      unsigned PC, unsigned HwMode)
  : TargetRegisterInfo(&NovaRegInfoDesc, RegisterClasses, RegisterClasses+6,
             SubRegIndexNameTable, SubRegIdxRangeTable, SubRegIndexLaneMaskTable,
             LaneBitmask(0xFFFFFFFFFFFFFFFF), RegClassInfos, VTLists, HwMode) {
  InitMCRegisterInfo(NovaRegDesc, 34, RA, PC,
                     NovaMCRegisterClasses, 6,
                     NovaRegUnitRoots,
                     33,
                     NovaRegDiffLists,
                     NovaLaneMaskLists,
                     NovaRegStrings,
                     NovaRegClassStrings,
                     NovaSubRegIdxLists,
                     1,
                     NovaRegEncodingTable);

}



ArrayRef<const uint32_t *> NovaGenRegisterInfo::getRegMasks() const {
  return {};
}

bool NovaGenRegisterInfo::
isGeneralPurposeRegister(const MachineFunction &MF, MCRegister PhysReg) const {
  return
      false;
}

bool NovaGenRegisterInfo::
isGeneralPurposeRegisterClass(const TargetRegisterClass *RC) const {
  return
      false;
}

bool NovaGenRegisterInfo::
isFixedRegister(const MachineFunction &MF, MCRegister PhysReg) const {
  return
      false;
}

bool NovaGenRegisterInfo::
isArgumentRegister(const MachineFunction &MF, MCRegister PhysReg) const {
  return
      false;
}

bool NovaGenRegisterInfo::
isConstantPhysReg(MCRegister PhysReg) const {
  return
      PhysReg == Nova::ZERO ||
      false;
}

ArrayRef<const char *> NovaGenRegisterInfo::getRegMaskNames() const {
  return {};
}

const NovaFrameLowering *
NovaGenRegisterInfo::getFrameLowering(const MachineFunction &MF) {
  return static_cast<const NovaFrameLowering *>(
      MF.getSubtarget().getFrameLowering());
}

} // end namespace llvm

#endif // GET_REGINFO_TARGET_DESC

