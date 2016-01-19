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
#include "storeinst.hpp"
void store(Instruction* instruction,vector<Instruction*> &inst1,vector<Instruction*> &inst2,vector<Argument*> &A1,vector<Argument*> &A2,BasicBlock *BB)
  {
  LLVMContext &context = instruction->getContext();
  Type* type = Type::getFloatTy(context);
  StoreInst *new_store; 
  StoreInst *st=dyn_cast<StoreInst>(instruction);
  int n=Searchinst(inst1,dyn_cast<Instruction>(st->getOperand(0)));
  int n1=Searchinst(inst1,dyn_cast<Instruction>(st->getOperand(1)));
  //************** store instructions/operations  
  
  if(n<inst1.size())
    {
    if(inst2[n]->getType()->isDoubleTy())//inst2[n] is function call 
      {
      FPTruncInst *c= new FPTruncInst  (inst2[n], type, "");
      BB->getInstList().push_back(c); 
      new_store=new StoreInst(c,inst2[n1] , false,4); 
      }
     else
      { 
      new_store=new StoreInst(inst2[n],inst2[n1] , false,4);// other instructions 
      }
    }
    
  //***************if 1st operand is argument of function  *****************  
  else if(dyn_cast<Argument>(st->getOperand(0)))
    {
    int m=Searcharg(A1,dyn_cast<Argument>(st->getOperand(0)));
    new_store=new StoreInst(A2[m],inst2[n1] , false,4);
    }
    
    //*************************** store a constant value  *******************************
  else
    {
    if(ConstantFP *cc= dyn_cast<ConstantFP>(st->getOperand(0)))
      {
      double a= APFloat(cc->getValueAPF()).convertToDouble ();
      Value* x1 = ConstantFP::get(type,a);
      new_store = new StoreInst(x1,inst2[n1] , false,4);
      }
      else
      {
      new_store = new StoreInst(st->getOperand(0),inst2[n1] , false,4);
      }
    }
    
     inst1.push_back(instruction);
     inst2.push_back(new_store);
     BB->getInstList().push_back(new_store);
     }
