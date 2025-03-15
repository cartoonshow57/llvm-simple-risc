#include "llvm/Support/raw_ostream.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;

int main() {
    Triple T("x86_64-unknown-linux-gnu");
    llvm::outs() << "Triple: " << T.str() << "\n";
    return 0;
}
