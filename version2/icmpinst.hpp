#ifndef icmpinst
#define icmpinst
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Type;
	class Instruction;
	class BasicBlock;
}

extern void ICMP(Instruction* instruction,vector<Instruction*> &inst1,vector<Instruction*> &inst2,BasicBlock *BB);
#endif
