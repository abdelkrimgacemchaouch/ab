#ifndef callinst
#define callinst
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Instruction;
	class BasicBlock;
}
extern void call(Instruction* instruction,vector<Instruction*> &inst1,vector<Instruction*> &inst2,BasicBlock *BB);
#endif
