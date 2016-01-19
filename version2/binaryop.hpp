#ifndef binaryop
#define binaryop
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Type;
	class Instruction;
	class BasicBlock;
}

extern void  binary(Instruction* instruction,vector<Instruction*> &inst1,vector<Instruction*> &inst2,BasicBlock *BB);
#endif
