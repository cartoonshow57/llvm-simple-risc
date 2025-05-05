//@s nova-return-td-1 end
//===- Nova Instruction Definitions ----------------------------===//
include "NovaInstrFormats.td"

//==---------- All SD nodes for Nova ------------------===//
def NovaRetSDN : SDNode<"NovaISD::Ret",
                        SDTNone, // 0 results and 0 operands
                        [SDNPHasChain, SDNPIsRet, SDNPVariadic,
                         SDNPOptInGlue]>;
//==---------- End SD Node definitions ----------------===//
//- nova-return-td-1
//@s nova-pseudo-ret end
//==----------- Nova Instruction Definitions ----------===//
def PseudoRet : PseudoNovaInst<(outs), (ins), "jr $ra"> {
  let isReturn = 1;
  let isTerminator = 1;
}
//==--------- End Nova Instruction Definitions --------===//
//- nova-pseudo-ret

