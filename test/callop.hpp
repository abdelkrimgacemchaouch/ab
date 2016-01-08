#ifndef callop
#define callop
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Type;
	class Instruction;
	class BasicBlock;
}
extern void call_2_load(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4);
extern void call_aloca(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4);
extern void call_GEP(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4);
extern void call_1_load(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4);
#endif
