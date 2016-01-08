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
#include "Alloca.hpp"
#include "Load.hpp"
using namespace llvm;
using namespace std;
 int indexinst;
 LoadInst *new_load;
void load_aloca_doubles(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  if (LoadInst *load = dyn_cast<LoadInst>(instruction))
    {
    T3.push_back(load);
    indexinst =Searchinst(T1, dyn_cast<AllocaInst>(load->getOperand(0)));
    new_load = new LoadInst(T2[indexinst]," ",false,4);
    T4.push_back(new_load); 
    BB->getInstList().push_back(new_load);
    }
  }
void load_aloca_other(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  if (LoadInst *load = dyn_cast<LoadInst>(instruction))
    {
    T3.push_back(load);
    indexinst =Searchinst(T1, dyn_cast<AllocaInst>(load->getOperand(0)));
    new_load=new LoadInst(T2[indexinst]," ",false);
    BB->getInstList().push_back(new_load);
    T4.push_back(new_load);
    }
  }
