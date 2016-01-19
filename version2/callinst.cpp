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
#include "callinst.hpp"
void call(Instruction* instruction,vector<Instruction*> &inst1,vector<Instruction*> &inst2,BasicBlock *BB)
  {
  LLVMContext &context = BB->getContext();
  Type* doublety= Type::getDoubleTy(context);
  Instruction *I =instruction->clone();
  int n;
  for(int i=0;i<instruction->getNumOperands ();i++)
    {
    n=Searchinst(inst1, dyn_cast<Instruction>(instruction->getOperand(i)));
    if(n<inst1.size())
      {
      if(inst1[n]->getType()->isDoubleTy())
        {
        FPExtInst *c= new FPExtInst  (inst2[n], doublety, "");
        inst1.push_back(c);   
        inst2.push_back(c);
        BB->getInstList().push_back(c); 
        I->setOperand(i,c);
        }
      else
        {  
        I->setOperand(i,inst2[n]);
        }
      }
    }
  inst1.push_back(instruction);   
  inst2.push_back(I);
  BB->getInstList().push_back(I); 
  }
