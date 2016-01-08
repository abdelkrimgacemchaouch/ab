#include "llvm/Pass.h"//
#include "llvm/IR/Function.h"//
#include "llvm/IR/Module.h"//
#include "llvm/Support/raw_ostream.h"//
#include "llvm/IR/Type.h"//
#include "llvm/IR/CallSite.h"//
#include "llvm/IR/Instructions.h"//
#include "llvm/IR/InstIterator.h"//
#include "llvm/IR/Constants.h"//
#include "search.hpp"
#include "Other.hpp"
using namespace llvm;
using namespace std;
void other(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4,vector<Instruction*> &T5,vector<Instruction*> &T6)
  {
  int indexinst;
  T1.push_back(instruction);
  if(instruction->getType()->isDoubleTy())
    {
    if (!instruction->getOperand(0)->getType()->isDoubleTy())
      {
      if(BinaryOperator* binOp = dyn_cast<BinaryOperator>(instruction->getOperand(0)))
        {
        indexinst=Searchinst(T2,binOp);
        Instruction *I1=T3[indexinst];
        T4.push_back(I1);
        }
      }
    }
  else 
    {  
    Instruction *I=instruction->clone();
    if(LoadInst *load = dyn_cast<LoadInst>(I->getOperand(0)))
      {
      indexinst =Searchinst(T5,load);
      I->setOperand(0,T6[indexinst]);
      }
    else if(BinaryOperator* binOp = dyn_cast<BinaryOperator>(I->getOperand(0)))
      {
      indexinst =Searchinst(T2,binOp);
      I->setOperand(0,T3[indexinst]);
      }  
    BB->getInstList().push_back(I);
    T4.push_back(I); 
    }
  }
