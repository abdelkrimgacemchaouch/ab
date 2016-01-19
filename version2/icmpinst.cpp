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
#include "icmpinst.hpp"
void ICMP(Instruction* instruction,vector<Instruction*> &inst1,vector<Instruction*> &inst2,BasicBlock *BB)
  {
  Instruction *I;
  int indexinst,index2;
  I=instruction->clone();
  indexinst =Searchinst(inst1, dyn_cast<Instruction>(instruction->getOperand(0)));
  index2= Searchinst(inst1, dyn_cast<Instruction>(instruction->getOperand(1)));
  if(dyn_cast<LoadInst>(instruction->getOperand(0))&&dyn_cast<LoadInst>(instruction->getOperand(1)))
    {
    I->setOperand(0,inst2[indexinst]);
    I->setOperand(1,inst2[index2]);
    }
  else
    {
    if(indexinst>inst1.size())
      {
      I->setOperand(1,inst2[index2]);
      }
    else 
      {
      I->setOperand(0,inst2[indexinst]);
      }  
    }  
  inst1.push_back(instruction);   
  inst2.push_back(I);
  BB->getInstList().push_back(I);
  }
