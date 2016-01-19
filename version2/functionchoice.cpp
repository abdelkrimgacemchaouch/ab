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
void CreatChoice(vector<Function*> &T1,vector<Function*> &T2,vector<Function*> &T3,GlobalVariable* &a,GlobalVariable* &b,Module &M)
  {
  //**************************************create the function which make the choice*******************
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
    BasicBlock * BB  = BasicBlock::Create(context, "choice",func);// the block which makes the choice
    BasicBlock * BB1 = BasicBlock::Create(context, "or",func); // the block to call the original function
    BasicBlock * BB2 = BasicBlock::Create(context, "cop",func);// the block to call the copy function
    
    LoadInst  *aa = new LoadInst(a," ",false,BB); 
    LoadInst  *bb = new LoadInst(b," ",false,BB); //load the attributes of choice 
    Value *CondIn = new ICmpInst(*BB, ICmpInst::ICMP_SLE, aa, bb, "cond"); 
    BranchInst::Create(BB2,BB1,CondIn,BB);
    
    //create the instructions for block which call original function 
    for (Function::arg_iterator I = T1[i]->arg_begin(), E = T1[i]->arg_end(); I != E; ++I)
      {
      AllocaInst *new_alloca = new AllocaInst(I->getType(),0, "aorg",BB1);//create allocate which have same type like attribute   
      LoadInst  *new_load = new LoadInst(new_alloca," ",false,BB1);//load this allocate                                  
      l1.push_back(new_load); // save all the load
      }
    CallInst *O= CallInst::Create(T1[i], l1, "",BB1); //call the function 
    l1.clear(); 
    AllocaInst *new_alloca = new AllocaInst(T2[i]->getReturnType(),0, "corg",BB1);
    StoreInst  *new_store = new StoreInst(O,new_alloca , false,4,BB1);
    LoadInst  *new_load = new LoadInst(new_alloca,"Aorg",false,4,BB1);
    ReturnInst::Create(context, new_load, BB1);  
    
    //create the instructions for block which call copy function(same work) 
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
    
//*******************************************************for original function******************************************
  for (int i=0;i<T1.size();i++)
    {
    if(inst_begin(T1[i])!=inst_end(T1[i]))
      {
       for (Function::iterator blocdebase = T1[i]->begin(), e = T1[i]->end(); blocdebase != e; ++blocdebase)
        {      
        if(pred_begin(blocdebase)==pred_end(blocdebase))
          {
          blocdebase->setName(T1[i]->getName());
          X.push_back(blocdebase); //know the 1st block of function      
          }  
        }
       BasicBlock * BB = BasicBlock::Create(context, "blocdechoix",T1[i] ,T1[i]->begin());//create block which allows if we continue in this function or not
       BasicBlock * BB1 = BasicBlock::Create(context, "blocother",T1[i]);//create a block that makes the call to another function
       
       LoadInst  *aa = new LoadInst(a," ",false,4,BB);
       LoadInst  *bb = new LoadInst(b," ",false,4,BB);
       Value *CondInst = new ICmpInst(*BB, ICmpInst::ICMP_SLE, aa, bb, "cond");
       BranchInst::Create(BB1,X[0],CondInst,BB);   
       
       CallInst *O= CallInst::Create(T3[i], "",BB1);
       Type* ty = T1[i]->getReturnType();
       AllocaInst *new_alloca = new AllocaInst(ty,0, "a",BB1);
       StoreInst  *new_store = new StoreInst(O,new_alloca , false,4,BB1);
       LoadInst  *new_load = new LoadInst(new_alloca," ",false,4,BB1);
       ReturnInst::Create(context, new_load, BB1);
       
       //reset condition value for the following functions
       for (Function::iterator blocdebase = T1[i]->begin(), e = T1[i]->end(); blocdebase != e; ++blocdebase)
        {
        const TerminatorInst *TI = blocdebase->getTerminator();      
        if(TI->getNumSuccessors()==0)
          {
          ReturnInst *ci=dyn_cast<ReturnInst>(blocdebase->getTerminator());
          Instruction *cii=ci->clone();
          Value *t= ConstantInt::get(Type::getInt32Ty(context), 1);
          Value *e= ConstantInt::get(Type::getInt32Ty(context), 0);
          StoreInst *st= new StoreInst(e,a,false,blocdebase);
          StoreInst *st1 =new StoreInst(t,b, false,blocdebase);
          ci->eraseFromParent();
          cii->insertAfter((st1));     
          }       
        }
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
          X.push_back(blocdebase);  //know the last block of function    
          }       
        }
        
      BasicBlock * BB1 = BasicBlock::Create(context, "cont",T2[i]);//create block to continue in this function
      Instruction *v=X[0]->getTerminator()->clone(); // copy the returninst to new block 
      BB1->getInstList().push_back(v);  
      X[0]->getTerminator()->eraseFromParent();//delete the original returninst
      
      //create a block that makes the call to another function(like the original function)
      BasicBlock * BB2 = BasicBlock::Create(context, "call",T2[i]);
      CallInst *O= CallInst::Create(T3[i], "",BB2);
      Type* ty = T2[i]->getReturnType();
      AllocaInst *new_alloca = new AllocaInst(ty,0, "a",BB2);
      StoreInst  *new_store = new StoreInst(O,new_alloca , false,4,BB2);
      LoadInst  *new_load = new LoadInst(new_alloca," ",false,4,BB2);
      ReturnInst::Create(context, new_load, BB2);
      
      BasicBlock * BB3 = BasicBlock::Create(context, "over",T2[i]);////create a block to test overflow
     //change the initial value 
      Value *t= ConstantInt::get(Type::getInt32Ty(context), 0);
      Value *e= ConstantInt::get(Type::getInt32Ty(context), 1);
      new StoreInst(e,a,false,X[0]);
      new StoreInst(t,b, false,X[0]);
     
     //create a way to test underflow
      Type* Int1= Type::getInt8PtrTy(context);
      Type *typpe=Type::getInt32Ty(context);
      Type* voidd= Type::getVoidTy(context);
      Function *under_over = cast<Function>(M.getOrInsertFunction ("llvm.x86.sse.stmxcsr",voidd,Int1,nullptr));
      AllocaInst *und = new AllocaInst(typpe,0, "under",X[0]);
      AllocaInst *ove = new AllocaInst(typpe,0, "over",X[0]);
      BitCastInst *newb= new BitCastInst(und,Int1," ",X[0]);
      CallInst *Oo=CallInst::Create(under_over, newb, "",X[0]);
      LoadInst *Cund=new LoadInst(und,"",false,X[0]); 
      Value *one = ConstantInt::get(Type::getInt32Ty(context), 63);
      Value *two = ConstantInt::get(Type::getInt32Ty(context), 16);
      BinaryOperator *te1 = BinaryOperator::Create(Instruction::And,Cund,one,"",X[0]);
      BinaryOperator *te2 = BinaryOperator::Create(Instruction::And,te1,two,"",X[0]);
      Value *z = ConstantInt::get(Type::getInt32Ty(context), 0);
      Value *CondI = new ICmpInst(*X[0], ICmpInst::ICMP_SLE, te2,z , "cond");
      BranchInst::Create (BB3,BB2 ,CondI,X[0]);
      
      //create a block to test overflow
      BitCastInst *newb1= new BitCastInst(ove,Int1," ",BB3);
      CallInst *Oov=CallInst::Create(under_over, newb1, "",BB3);
      LoadInst *Cov=new LoadInst(und,"",false,BB3);
      Value *three = ConstantInt::get(Type::getInt32Ty(context), 8);
      BinaryOperator *te3 = BinaryOperator::Create(Instruction::And,Cov,one,"",BB3);
      BinaryOperator *te4 = BinaryOperator::Create(Instruction::And,te3,three,"",BB3);
      Value *CondIov = new ICmpInst(*BB3, ICmpInst::ICMP_SLE, te3,z , "condov");
      BranchInst::Create (BB1,BB2 ,CondI,BB3);
      X.clear();
      l.clear();
      }  
    }      
  }

