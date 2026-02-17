//@s cgp1-subtarget-cpp
#include "CGP1.h"
#include "CGP1Subtarget.h"
#include "CGP1RegisterInfo.h"
#include "CGP1TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "cgp1-subtarget"

#define GET_SUBTARGETINFO_CTOR
#define GET_SUBTARGETINFO_TARGET_DESC
#include "CGP1GenSubtargetInfo.inc"

//- cgp1-subtarget-cpp