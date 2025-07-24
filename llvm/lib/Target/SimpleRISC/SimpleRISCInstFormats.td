

// The following opcode names match those given in Table 3.10 in the
// Basic Comp Arch Sarangi
//21 -instructions 5 - bit



// Format specifies the encoding used by the instruction.This is used by
// RISCVMCCodeEmitter to determine which form of fixup to use. These
// definitions must be kept in-sync with RISCVBaseInfo.h.


// for mat sourced from sarangi COmp arch [Table 3.11]

class SimpleRISCInstFormat<bits<3> val> {
  bits<3> Value = val;
}

def InstFormatPseudo        : SimpleRISCInstFormat<0>;
def InstFormatB             : SimpleRISCInstFormat<1>;
def InstFormatR            : SimpleRISCInstFormat<2>;
def InstFormatI             : SimpleRISCInstFormat<3>;

class SimpleRISCOpcode<string name, bits<5> val> {
  string Name = name;
  bits<5> Value = val;
}

def SimpleRISCOpcodesList : GenericTable {
  let FilterClass = "SimpleRISCOpcode";
  let Fields = [
    "Name", "Value"
  ];
  let PrimaryKey = [ "Value" ];
  let PrimaryKeyName = "lookupSimpleRISCOpcodeByValue";
}

def lookupSimpleRISCOpcodeByName : SearchIndex {
  let Table = SimpleRISCOpcodesList;
  let Key = [ "Name" ];
}

def OPC_ADD      : SimpleRISCOpcode<"ADD",      0b00000>;
def OPC_SUB      : SimpleRISCOpcode<"SUB",      0b00001>;
def OPC_MUL      : SimpleRISCOpcode<"MUL",      0b00010>;
def OPC_DIV      : SimpleRISCOpcode<"DIV",      0b00011>;
def OPC_MOD      : SimpleRISCOpcode<"MOD",      0b00100>;
def OPC_CMP      : SimpleRISCOpcode<"CMP",      0b00101>;
def OPC_AND      : SimpleRISCOpcode<"AND",      0b00110>;
def OPC_OR       : SimpleRISCOpcode<"OR",       0b00111>;
def OPC_NOT      : SimpleRISCOpcode<"NOT",      0b01000>;
def OPC_MOV      : SimpleRISCOpcode<"MOV",      0b01001>;
def OPC_LSL      : SimpleRISCOpcode<"LSL",      0b01010>;
def OPC_LSR      : SimpleRISCOpcode<"LSR",      0b01011>;
def OPC_ASR      : SimpleRISCOpcode<"ASR",      0b01100>;
def OPC_NOP      : SimpleRISCOpcode<"NOP",      0b01101>;
def OPC_LD       : SimpleRISCOpcode<"LD",       0b01110>;
def OPC_ST       : SimpleRISCOpcode<"ST",       0b01111>;
def OPC_BEQ      : SimpleRISCOpcode<"BEQ",      0b10000>;
def OPC_BGT      : SimpleRISCOpcode<"BGT",      0b10001>;
def OPC_B        : SimpleRISCOpcode<"B",        0b10010>;
def OPC_CALL     : SimpleRISCOpcode<"CALL",     0b10011>;
def OPC_RET      : SimpleRISCOpcode<"RET",      0b10100>;