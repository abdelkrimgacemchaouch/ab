#ifndef cloneinst
#define cloneinst
using namespace std;
using namespace llvm;
namespace llvm {
	class Value;
	class Type;
	class Instruction;
	class BasicBlock;
}
 extern  bool runOnFunction(Module &M, Function &F,Function &F1);
extern void takeA(Function &F,vector<Argument*>&A1);
#endif
