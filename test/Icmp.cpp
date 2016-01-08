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
#include "Icmp.hpp"
using namespace llvm;
using namespace std;
void icmp_2load(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  Instruction *icmp;
  int indexinst,index2;
  if (dyn_cast<ICmpInst>(instruction))
    { 
    icmp =instruction;  
    }
  if (dyn_cast <CmpInst>(instruction))
    {
    icmp =instruction;
    }  
  Instruction *I;
  T1.push_back(icmp);
  I=icmp->clone();
  indexinst =Searchinst(T2, dyn_cast<LoadInst>(icmp->getOperand(0)));
  index2=Searchinst(T2, dyn_cast<LoadInst>(icmp->getOperand(1)));
  I->setOperand(0,T3[indexinst]);
  I->setOperand(1,T3[index2]);
  BB->getInstList().push_back(I);
  T4.push_back(I);
  }
void icmp_1load(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  int indexinst,index2;
  ICmpInst *icmp=dyn_cast<ICmpInst>(instruction);
  Instruction *I;
  T1.push_back(icmp);
  I=icmp->clone();            
  if(dyn_cast<LoadInst>(icmp->getOperand(0)))
    {
    indexinst =Searchinst(T2, dyn_cast<LoadInst>(icmp->getOperand(0)));
    I->setOperand(0,T3[indexinst]);
    }
  else if(dyn_cast<LoadInst>(icmp->getOperand(1)))
    {
    indexinst =Searchinst(T2, dyn_cast<LoadInst>(icmp->getOperand(1)));
    I->setOperand(1,T3[indexinst]);
    }    
  BB->getInstList().push_back(I);
  T4.push_back(I);  
  }  
