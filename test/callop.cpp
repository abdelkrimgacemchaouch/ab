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
#include "callop.hpp"
using namespace llvm;
using namespace std;
void call_2_load(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  LLVMContext &context = BB->getContext();
  Type* floaty = Type::getFloatTy(context);
  Type* doublety= Type::getDoubleTy(context);
  int indexinst,index;
  T3.push_back(instruction);
  CallInst *cal = dyn_cast<CallInst>(instruction);
  LoadInst *load = dyn_cast<LoadInst>(cal->getOperand(1));
  LoadInst *load1 = dyn_cast<LoadInst>(cal->getOperand(0));
  Instruction *newInst;
  if((load1->getType()->isDoubleTy()) && (load->getType()->isDoubleTy()))
    {
    indexinst =Searchinst(T1, dyn_cast<LoadInst>(cal->getOperand(0)));
    Instruction *newInst = cal->clone();
    FPExtInst *c= new FPExtInst  (T2[indexinst], doublety, "");
    BB->getInstList().push_back(c);
    newInst->setOperand(0,c);
    indexinst =Searchinst(T1, dyn_cast<LoadInst>(cal->getOperand(1)));
    c= new FPExtInst  (T2[indexinst], doublety, "");
    BB->getInstList().push_back(c);
    newInst->setOperand(1,c);
    BB->getInstList().push_back(newInst);
    T4.push_back(newInst);
    }
  }   
 void call_1_load(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  LLVMContext &context = BB->getContext();
  Type* floaty = Type::getFloatTy(context);
  Type* doublety= Type::getDoubleTy(context);
  int indexinst,index;
  CallInst *cal = dyn_cast<CallInst>(instruction);
  LoadInst *load = dyn_cast<LoadInst>(cal->getOperand(1));
  Instruction *newInst;
  if((load->getType()->isDoubleTy()))
    {
    indexinst =Searchinst(T1, dyn_cast<LoadInst>(cal->getOperand(1)));
    Instruction *newInst = cal->clone();
    FPExtInst *c= new FPExtInst  (T2[indexinst], doublety, "");
    BB->getInstList().push_back(c);
    newInst->setOperand(1,c);
    BB->getInstList().push_back(newInst); 
    }
  else
    {
    indexinst =Searchinst(T3, dyn_cast<LoadInst>(cal->getOperand(1)));
    Instruction *newInst = cal->clone();
    newInst->setOperand(1,T4[indexinst]);
    if(dyn_cast<LoadInst>(cal->getOperand(2)))
      {
      index =Searchinst(T3, dyn_cast<LoadInst>(cal->getOperand(2)));
      newInst->setOperand(2,T4[index]);
      }
    BB->getInstList().push_back(newInst); 
    }
  }
void call_aloca(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  int indexinst;
  CallInst *call = dyn_cast<CallInst>(instruction);
  AllocaInst *alloca = dyn_cast<AllocaInst>(call->getOperand(1));
  Instruction *newInst = call->clone();
  if (alloca->getAllocatedType()->isDoubleTy())
    {
    indexinst =Searchinst(T1, dyn_cast<AllocaInst>(call->getOperand(1)));  
    newInst->setOperand(1,T2[indexinst]);
    BB->getInstList().push_back(newInst);
    }
  else
    { 
    indexinst =Searchinst(T3, dyn_cast<AllocaInst>(call->getOperand(1)));
    newInst->setOperand(1,T4[indexinst]);
    BB->getInstList().push_back(newInst);                      
    }
  }
extern void call_GEP(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)    
  {
  CallInst *call = dyn_cast<CallInst>(instruction);
  Instruction *newInst = call->clone(); 
  int indexinst;
  GetElementPtrInst* gep = dyn_cast<GetElementPtrInst>(newInst->getOperand(1));              
  if((gep->getSourceElementType()->getArrayElementType ()->isDoubleTy())||(gep->getSourceElementType()->getArrayElementType ()->isArrayTy()))
    { 
    indexinst=Searchinst(T1, gep);
    newInst->setOperand(1,T2[indexinst]);
    }
  else 
    {
    indexinst=Searchinst(T3, gep);
    newInst->setOperand(1,T4[indexinst]);
    } 
  BB->getInstList().push_back(newInst);
  }
