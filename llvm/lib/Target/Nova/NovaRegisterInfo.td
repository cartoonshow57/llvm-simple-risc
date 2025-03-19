//@s nova-registers end
// Nova CPU Registers

class NovaReg<string n> : Register<n> {
    let Namespace = "Nova";
}

class NovaGPRReg<string n> : Register<n> {
    let Namespace = "Nova";
}

let Namespace = "Nova" in {
  def ZERO : NovaGPRReg<"zero"> {
    let isConstant = 1;
  }
  def AT   : NovaGPRReg<"at">;
  def V0   : NovaGPRReg<"2">;
  def V1   : NovaGPRReg<"3">;
  def A0   : NovaGPRReg<"4">;
  def A1   : NovaGPRReg<"5">;
  def A2   : NovaGPRReg<"6">;
  def A3   : NovaGPRReg<"7">;
  def T0   : NovaGPRReg<"8">;
  def T1   : NovaGPRReg<"9">;
  def T2   : NovaGPRReg<"10">;
  def T3   : NovaGPRReg<"11">;
  def T4   : NovaGPRReg<"12">;
  def T5   : NovaGPRReg<"13">;
  def T6   : NovaGPRReg<"14">;
  def T7   : NovaGPRReg<"15">;
  def S0   : NovaGPRReg<"16">;
  def S1   : NovaGPRReg<"17">;
  def S2   : NovaGPRReg<"18">;
  def S3   : NovaGPRReg<"19">;
  def S4   : NovaGPRReg<"20">;
  def S5   : NovaGPRReg<"21">;
  def S6   : NovaGPRReg<"22">;
  def S7   : NovaGPRReg<"23">;
  def T8   : NovaGPRReg<"24">;
  def T9   : NovaGPRReg<"25">;
  def K0   : NovaGPRReg<"26">;
  def K1   : NovaGPRReg<"27">;
  def GP   : NovaGPRReg<"gp">; // 28
  def SP   : NovaGPRReg<"sp">; // 29
  def FP   : NovaGPRReg<"fp">; // 30
  def RA   : NovaGPRReg<"ra">; // 31
  
  //== Special registers ==//
  //=======================//
  def PC   : NovaReg<"pc">;
} 
//- nova-registers


//@s nova-reg-class end
//=== -----------------
// Register Classes
//=== -----------------

def GPR32 : RegisterClass<"Nova",
//- nova-reg-class
//@s nova-reg-class1 end
  [i32],
//- nova-reg-class1
//@s nova-reg-class2 end
  32,
//- nova-reg-class2
//@s nova-reg-class3 end
  (add
   // Reserved
   ZERO, AT,
   // Return values and arguments
   V0, V1,
   A0, A1, A2, A3,
   // Caller-saved temporaries (not preserved across calls, clobbered by callee)
   T0, T1, T2, T3, T4, T5, T6, T7,
   // Callee-saved (preserved across calls)
   S0, S1, S2, S3, S4, S5, S6, S7,
   // More caller-saved temporaries
   T8, T9,
   // Kernel registers
   K0, K1,
   // Global pointer
   GP,
   // Stack pointer
   SP,
   // Frmame pointer
   FP,
   // Return address (mostly used by instruction "jal")
   RA)>;
//- nova-reg-class3

//@s nova-rc-zero end
def GPR32ZERO : RegisterClass<"Nova", [i32], 32, (add ZERO)>;
//- nova-rc-zero

//@s nova-rc-nonzero end
def GPR32NONZERO : RegisterClass<"Nova", [i32], 32, (sub GPR32, GPR32ZERO)>;
//- nova-rc-nonzero

//@s nova-rc-unalloc
let isAllocatable = false in {
  def CPURAReg : RegisterClass<"Nova", [i32], 32, (add RA)>;
  def CPUSPReg : RegisterClass<"Nova", [i32], 32, (add SP)>;

  def GP32 : RegisterClass<"Nova", [i32], 32, (add GP)>;
}
//- nova-rc-unalloc
