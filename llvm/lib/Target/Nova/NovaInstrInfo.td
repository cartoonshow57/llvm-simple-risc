//@s nova-return-td-1 end
//===- Nova Instruction Definitions ----------------------------===//
include "NovaInstrFormats.td"

//==---------- All SD nodes for Nova ------------------===//
def NovaRetSDN : SDNode<"NovaISD::Ret",
                        SDTNone, // 0 results and 0 operands
                        [SDNPHasChain, SDNPVariadic, SDNPOptInGlue]>;
//==---------- End SD Node definitions ----------------===//
//- nova-return-td-1
//@s nova-pseudo-ret end
//==----------- Nova Instruction Definitions ----------===//
def PseudoRet : PseudoNovaInst<(outs), (ins), "ret"> {
  let isReturn = 1;
  let isTerminator = 1;
}
//==--------- End Nova Instruction Definitions --------===//
//- nova-pseudo-ret
//@s nova-pseudo-ret-pat end
//==---- All patterns to match SD nodes -----------==//
def : Pat<(NovaRetSDN), (PseudoRet)>;
//- nova-pseudo-ret-pat

//@s nova-adj-stack-td end
def callseq_end : SDNode<"ISD::CALLSEQ_END", SDTNone, [SDNPHasChain, SDNPOptInGlue]>;

def ADJCALLSTACKDOWN : Instruction {
  let OutOperandList = (outs);
  let Namespace = "Nova";
  let InOperandList = (ins);
  let AsmString = "ADJCALLSTACKDOWN";
  let Pattern = [(callseq_end)];
}

def ADJCALLSTACKUP : Instruction {
  let OutOperandList = (outs);
  let Namespace = "Nova";
  let InOperandList = (ins);
  let AsmString = "ADJCALLSTACKUP";
  let Pattern = [(callseq_end)];
}
//- nova-adj-stack-td


