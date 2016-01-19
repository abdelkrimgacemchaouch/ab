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
extern void clone(vector<Function*>&T1,vector<Type*>&P,Module &M); 
extern void clone2(vector<Function*>&T1,vector<Function*>&T2,Module &M);
#endif
