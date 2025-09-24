//===-- CGP1MCCodeEmitter.cpp - Minimal CGP1 MCCodeEmitter --------------===//
//
// Minimal, build-friendly MCCodeEmitter for CGP1.
// Emits a fixed 4-byte word for every instruction.  This is intentionally
// tiny — it avoids TableGen dependencies and complex fixup handling so you
// can focus on codegen while keeping the MC layer buildable.
//
//===----------------------------------------------------------------------===//

#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/EndianStream.h"

using namespace llvm;

namespace {
class CGP1MCCodeEmitter : public MCCodeEmitter {
public:
  CGP1MCCodeEmitter(const MCInstrInfo & /*MCII*/, MCContext & /*Ctx*/) {}
  ~CGP1MCCodeEmitter() override = default;

  // Very small encoder: write 4 zero bytes (big-endian) for every MCInst.
  void encodeInstruction(const MCInst & /*Inst*/, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> & /*Fixups*/,
                         const MCSubtargetInfo & /*STI*/) const override {
    support::endian::write<uint32_t>(CB, 0u, llvm::endianness::big);
  }

  // Helper: naive operand encoder. Returns an integer for regs/imms,
  // and records a trivial fixup for expressions (so downstream code doesn't
  // crash if a fixup is expected). This is optional for the minimal emitter
  // but handy to keep around.
  unsigned getMachineOpValue(const MCInst & /*Inst*/, const MCOperand &Op,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo & /*STI*/) const {
    if (Op.isReg())
      return 0;
    if (Op.isImm())
      return static_cast<unsigned>(Op.getImm());
    if (Op.isExpr()) {
      // Record a generic fixup (MCFixupKind 0). This is a placeholder.
      Fixups.push_back(MCFixup::create(0, Op.getExpr(), MCFixupKind(0)));
      return 0;
    }
    return 0;
  }
};
} // end anonymous namespace

namespace llvm {
MCCodeEmitter *createCGP1MCCodeEmitter(const MCInstrInfo &II, MCContext &Ctx) {
  return new CGP1MCCodeEmitter(II, Ctx);
}
} // end namespace llvm
