#ifndef Alloca
#define Alloca
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Instruction;
	class BasicBlock;
}


extern void aloca(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4);
#endif
