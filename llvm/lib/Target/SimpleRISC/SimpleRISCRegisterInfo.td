


let Namespace = "SimpleRISC" in {

  class SimpleRISCReg<bits<4> Enc, string n, list<string> alt = []> : Register<n> {
    let HWEncoding{3-0} = Enc; // 4 bits for 16 registers
    let AltNames = alt;
  }

  // Optional: alternative name indexing
  let RegAltNameIndices = [ABIRegAltName] in {

    // Define all 16 registers

    let isConstant = true in
    def R0  : SimpleRISCReg<0,  "r0">;
    def R1  : SimpleRISCReg<1,  "r1">;
    def R2  : SimpleRISCReg<2,  "r2">;
    def R3  : SimpleRISCReg<3,  "r3">;
    def R4  : SimpleRISCReg<4,  "r4">;
    def R5  : SimpleRISCReg<5,  "r5">;
    def R6  : SimpleRISCReg<6,  "r6">;
    def R7  : SimpleRISCReg<7,  "r7">;
    def R8  : SimpleRISCReg<8,  "r8">;
    def R9  : SimpleRISCReg<9,  "r9">;
    def R10 : SimpleRISCReg<10, "r10">;
    def R11 : SimpleRISCReg<11, "r11">;
    def R12 : SimpleRISCReg<12, "r12">;
    def R13 : SimpleRISCReg<13, "r13">;
    def SP  : SimpleRISCReg<14, "sp", ["r14"]>;   // Stack pointer
    def RA  : SimpleRISCReg<15, "ra", ["r15"]>;   // Return address

  }

  // Register class for all general-purpose registers

  def GPR : RegisterClass<"SimpleRISC", [i32], 32,
    (add R0, R1, R2, R3, R4, R5, R6, R7,
     R8, R9, R10, R11, R12, R13, RA)>;

  //Register class for stack pointer

  def SPR : RegisterClass<"SimpleRISC", i[32], 32, (add SP) >;

}