#ifndef functionclone
#define functionclone
using namespace std;
using namespace llvm;
namespace llvm 
  {
	class Value;
	class Instruction;
	class BasicBlock;
  }
extern void clone(Function *F,vector<Function*>&T1,vector<Argument*>&A1,vector<Type*>&P); 
extern void clone2(vector<Function*>&T1,vector<Function*>&T2,vector<Argument*>&A1,vector<Type*>&P,Module &M);
#endif
