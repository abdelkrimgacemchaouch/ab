#include "llvm/Pass.h"//
#include "llvm/IR/Function.h"//
#include "llvm/IR/Module.h"//
#include "llvm/Support/raw_ostream.h"//
#include "llvm/IR/Type.h"//
#include "llvm/IR/CallSite.h"//
#include "llvm/Analysis/CFG.h"
#include "llvm/IR/Instructions.h"//
#include "llvm/IR/InstIterator.h"//
#include "llvm/IR/Constants.h"//
#include"functionchoice.hpp"
#include <iostream>
#include <fstream>
using namespace llvm;
using namespace std;
void CreatChoice(vector<Function*> &T1,vector<Function*> &T2,vector<Function*> &T3,Value* &a,Value* &b,Module &M)
  {
  //*****************************************************creat function to do choice
  vector<Value*> l,l1;
  vector<BasicBlock*> X;
  FunctionType *FT;
  Function* func;
  BasicBlock * BB;
  LLVMContext &context = M.getContext();
  for (int i=0;i<T1.size();i++)
    {
    FT = FunctionType::get(T1[i]->getReturnType(),false);
    func = Function::Create(FT, Function::ExternalLinkage, "F", &M);
    BasicBlock * BB  = BasicBlock::Create(context, "choice",func);
    BasicBlock * BB1 = BasicBlock::Create(context, "or",func);
    BasicBlock * BB2 = BasicBlock::Create(context, "cop",func);
    Value *CondIn = new ICmpInst(*BB, ICmpInst::ICMP_SLE, a, b, "cond");
    BranchInst::Create(BB2,BB1,CondIn,BB);
    for (Function::arg_iterator I = T1[i]->arg_begin(), E = T1[i]->arg_end(); I != E; ++I)
      {
      AllocaInst *new_alloca = new AllocaInst(I->getType(),0, "aorg",BB1);
      LoadInst  *new_load = new LoadInst(new_alloca," ",false,BB1);      
      l1.push_back(new_load); 
      }
    CallInst *O= CallInst::Create(T1[i], l1, "",BB1);
    l1.clear();
    AllocaInst *new_alloca = new AllocaInst(T2[i]->getReturnType(),0, "corg",BB1);
    StoreInst  *new_store = new StoreInst(O,new_alloca , false,4,BB1);
    LoadInst  *new_load = new LoadInst(new_alloca,"Aorg",false,4,BB1);
    ReturnInst::Create(context, new_load, BB1);  
    for (Function::arg_iterator I = T2[i]->arg_begin(), E = T2[i]->arg_end(); I != E; ++I)
      {
      AllocaInst *new_alloca = new AllocaInst(I->getType(),0, "acop",BB2);
      LoadInst  *new_load = new LoadInst(new_alloca," ",false,BB2);      
      l1.push_back(new_load); 
      }
    CallInst *OO= CallInst::Create(T2[i], l1, "",BB2);
    l1.clear();
    AllocaInst *new_alloc = new AllocaInst(T2[i]->getReturnType(),0, "ccop",BB2);
    StoreInst  *new_stor = new StoreInst(OO,new_alloc , false,4,BB2);
    LoadInst  *new_loa = new LoadInst(new_alloc,"Aorg",false,4,BB2);
    ReturnInst::Create(context, new_loa, BB2);
    T3.push_back(func);
    }
//*******************************************************do the choice******************************************
  for (int i=0;i<T1.size();i++)
    {
    if(inst_begin(T1[i])!=inst_end(T1[i]))
      {
       for (Function::iterator blocdebase = T1[i]->begin(), e = T1[i]->end(); blocdebase != e; ++blocdebase)
        {      
        if(pred_begin(blocdebase)==pred_end(blocdebase))
          {
          blocdebase->setName(T1[i]->getName());
          X.push_back(blocdebase);     
          }  
        }
       BasicBlock * BB = BasicBlock::Create(context, "blocdechoix",T1[i] ,T1[i]->begin());
       BasicBlock * BB1 = BasicBlock::Create(context, "blocother",T1[i]);
       Value *CondInst = new ICmpInst(*BB, ICmpInst::ICMP_SLE, a, b, "cond");
       BranchInst::Create(BB1,X[0],CondInst,BB);   
       CallInst *O= CallInst::Create(T3[i], "",BB1);
       Type* ty = T1[i]->getReturnType();
       AllocaInst *new_alloca = new AllocaInst(ty,0, "a",BB1);
       StoreInst  *new_store = new StoreInst(O,new_alloca , false,4,BB1);
       LoadInst  *new_load = new LoadInst(new_alloca," ",false,4,BB1);
       ReturnInst::Create(context, new_load, BB1);
       X.clear();
       l.clear();
      } 
    }
//**************************************for function clone********************************************
  for (int i=0;i<T2.size();i++)
    {
    if(inst_begin(T2[i])!=inst_end(T2[i]))
      {
      for (Function::iterator blocdebase = T2[i]->begin(), e = T2[i]->end(); blocdebase != e; ++blocdebase)
        {
        const TerminatorInst *TI = blocdebase->getTerminator();      
        if(TI->getNumSuccessors()==0)
          {
          X.push_back(blocdebase);     
          }       
        }
      BasicBlock * BB1 = BasicBlock::Create(context, "cont",T2[i]);
      Instruction *v=X[0]->getTerminator()->clone();
      BB1->getInstList().push_back(v);
      X[0]->getTerminator()->eraseFromParent();
      BasicBlock * BB2 = BasicBlock::Create(context, "call",T2[i]);
      CallInst *O= CallInst::Create(T3[i], "",BB2);
      Type* ty = T2[i]->getReturnType();
      AllocaInst *new_alloca = new AllocaInst(ty,0, "a",BB2);
      StoreInst  *new_store = new StoreInst(O,new_alloca , false,4,BB2);
      LoadInst  *new_load = new LoadInst(new_alloca," ",false,4,BB2);
      ReturnInst::Create(context, new_load, BB2);
      a = ConstantInt::get(Type::getInt32Ty(context), 1);
      b = ConstantInt::get(Type::getInt32Ty(context), 0);
      Value *CondI = new ICmpInst(*X[0], ICmpInst::ICMP_SLE, a, b, "cond");
      BranchInst::Create (BB2,BB1 ,CondI,X[0]);
      X.clear();
      l.clear();
      }  
    }      
  }

