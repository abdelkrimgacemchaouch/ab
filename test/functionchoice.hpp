#ifndef functionchoice
#define functionchoice
using namespace std;
using namespace llvm;
namespace llvm 
  {
	class Value;
	class Instruction;
	class BasicBlock;
  }
extern void CreatChoice(vector<Function*> &T1,vector<Function*> &T2,vector<Function*> &T3,Value* &a,Value* &b,Module &M);
#endif
