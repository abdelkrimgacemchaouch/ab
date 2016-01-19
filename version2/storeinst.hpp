#ifndef storeinst
#define storeinst
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Type;
	class Instruction;
	class BasicBlock;
}

extern void store(Instruction* instruction,vector<Instruction*> &inst1,vector<Instruction*> &inst2,vector<Argument*> &A1,vector<Argument*> &A2,BasicBlock *BB);
#endif
