//===- CGP1.td - Describe the CGP1 Target Machine --------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "CGP1Schedule.td"
include "CGP1RegisterInfo.td"
include "CGP1CallingConv.td"
include "CGP1InstrInfo.td"

def CGP1InstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// CGP1 processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", CGP1SchedModel, []>;
def : ProcessorModel<"v11", CGP1SchedModel, []>;

def CGP1InstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def CGP1 : Target {
  // Pull in Instruction Info:
  let InstructionSet = CGP1InstrInfo;
  let AssemblyWriters = [CGP1InstPrinter];
}
