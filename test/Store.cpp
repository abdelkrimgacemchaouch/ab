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
#include "Store.hpp"
using namespace llvm;
using namespace std;
//**********************store in array*****************************************************
void store_gep(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
	//************************************************
  StoreInst* new_store;
  StoreInst *store=dyn_cast<StoreInst>(instruction);
  int indexinst =Searchinst(T1, dyn_cast<Instruction>(store->getOperand(0)));
  int index2=Searchinst(T2, dyn_cast<GetElementPtrInst>(store->getOperand(1)));
  new_store = new StoreInst(T3[indexinst],T4[index2] , false,4);
  BB->getInstList().push_back(new_store);
  }
//*************************store in variable******************************************************************
//***************************double type**********************************************************************
void store_var_double(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  LLVMContext &context = BB->getContext();
  Type* type = Type::getFloatTy(context);
  StoreInst* new_store;
  StoreInst *store=dyn_cast<StoreInst>(instruction);
  int indexinst,index2;
  if(BinaryOperator* binOp = dyn_cast<BinaryOperator> (store->getOperand(0)))
    {
    indexinst =Searchinst(T1, dyn_cast<BinaryOperator> (store->getOperand(0)));
    index2=Searchinst(T2, dyn_cast<AllocaInst>(store->getOperand(1)));
    new_store = new StoreInst(T3[indexinst],T4[index2] , false,4);
    }
  else
    {
    FPTruncInst *c= new FPTruncInst  (store->getOperand(0), type, "");
    indexinst =Searchinst(T2, dyn_cast<AllocaInst>(store->getOperand(1)));
    BB->getInstList().push_back(c);
    if(ConstantFP *cc= dyn_cast<ConstantFP>(c->getOperand(0)))
      {
      double a= APFloat(cc->getValueAPF()).convertToDouble ();
      Value* x1 = ConstantFP::get(type,a);
      new_store = new StoreInst(x1,T4[indexinst] , false,4);
      c->eraseFromParent();
      }
    else
      {
      new_store = new StoreInst(c,T4[indexinst] , false,4);
      }                    
    }    
  BB->getInstList().push_back(new_store);
  }
//********************************************other type************************************************************
void store_var_other(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  int indexinst ,index2;
  StoreInst* new_store;
  StoreInst *store=dyn_cast<StoreInst>(instruction);
  if(BinaryOperator* binOp = dyn_cast<BinaryOperator> (store->getOperand(0)))
    {
    indexinst =Searchinst(T1, binOp);
    index2=Searchinst(T2, dyn_cast<AllocaInst>(store->getOperand(1)));
    new_store = new StoreInst(T3[indexinst],T4[index2] , false,4);
    }
  else
    {                  
    indexinst =Searchinst(T2, dyn_cast<AllocaInst>(store->getOperand(1)));
    new_store =new StoreInst(store->getOperand(0),T4[indexinst],false);
    }
  BB->getInstList().push_back(new_store);  
  }    
