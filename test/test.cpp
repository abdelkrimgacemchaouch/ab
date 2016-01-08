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
#include"functionclone.hpp"
#include"Alloca.hpp"
#include"Load.hpp"
#include "BinairOperation.hpp"
#include "callop.hpp"
#include "Icmp.hpp"
#include "Store.hpp"
#include "search.hpp"
#include "functionchoice.hpp"
#include <iostream>
#include <fstream>
using namespace llvm;
using namespace std;
namespace {
  struct test : public ModulePass {
    static char ID;
    test() : ModulePass(ID) {}
    vector<Instruction*> orginal_alloca,copie_alloca,orginal_other_alloca,copie_other_alloca;//vector for alloca inst 
    vector<Instruction*> orginal_load,copie_load,orginal_other_load,copie_other_load; //vector for load inst
    vector<Instruction*> orginal_bin,copie_bin,orginal_other_bin,copie_other_bin;
    vector<Instruction*> orginal_icmp,copie_icmp;
    vector<Instruction*> orginal_other,copie_other;
    std::vector<Type*> P;
    std::vector<Function*> Funorg,Funclone,Fu;
    vector<Argument*> Aorg,Aclone;
    vector<BasicBlock*> BBorg,BBclone;
    vector<Instruction*> IBorg,IBclone;
    virtual  bool runOnModule(Module &M) 
      {
      LLVMContext &context = M.getContext();
    //***********************************creat new function*************************************   
      for(Module::iterator f = M.begin(), fe = M.end(); f != fe; f++) 
        { 
        clone(f,Funorg,Aorg,P); // clone each functions 
        }
      clone2(Funorg,Funclone,Aclone,P,M);//creat new function
//**********************************copy the instruction to the clone functuion****************************************
      filling();//copie the instruction in the clone function           
      for(int i=0;i<Funclone.size();i++) 
        {
        for (Function::iterator blocdebase = Funclone[i]->begin(), e = Funclone[i]->end(); blocdebase != e; ++blocdebase)
          {    
          for (BasicBlock::iterator instruction = blocdebase->begin(), ee = blocdebase->end(); instruction != ee; ++instruction)
            {
            int n=Searchinst(orginal_other,dyn_cast<Instruction>(instruction->getOperand(0)));
            if(n<orginal_other.size())
              {
              instruction->setOperand(0,copie_other[n]);
              }
            }
          }        
        }
//*****************************************creat the way to choice ********************************************************       
      Value *One = ConstantInt::get(Type::getInt32Ty(context), 0);
      Value *Two = ConstantInt::get(Type::getInt32Ty(context), 1);
      CreatChoice(Funorg,Funclone,Fu,One,Two, M);
      return true;//false si on veut pas de changement
      }
//****************************************creat the new instruction and filling the function copie***********************************      
    void filling()
      {
      int n2;
      for (int i=0;i<Funorg.size();i++)
        {
        LLVMContext &context = Funorg[i]->getContext();
        for (Function::iterator blocdebase = Funorg[i]->begin(), e = Funorg[i]->end(); blocdebase != e; ++blocdebase)
          {
          BBorg.push_back(blocdebase);
          BasicBlock *BB = BasicBlock::Create(context, "EntryBlock", Funclone[i]);
          for (BasicBlock::iterator instruction = blocdebase->begin(), ee = blocdebase->end(); instruction != ee; ++instruction)
            {
            //*************************allocainst*********************************************************
            if(dyn_cast<AllocaInst>(instruction))
              {
              aloca(instruction,BB,orginal_alloca,copie_alloca,orginal_other_alloca,copie_other_alloca);
              }
              //***********************************storeinst********************************************
            else if (StoreInst *store = dyn_cast<StoreInst>(instruction))
              {
              if(dyn_cast<Argument>(store->getOperand(0)))
                {
                int n = Searcharg(Aorg,dyn_cast<Argument>(instruction->getOperand(0)));
                n2= Searchinst(orginal_alloca,dyn_cast<Instruction>(instruction->getOperand(1)));
                if(n2<orginal_alloca.size())
                  {
                  StoreInst* new_store = new StoreInst(Aclone[n],copie_alloca[n2] , false,4);
                  BB->getInstList().push_back(new_store);
                  }
                }
              else 
                {  
                if(dyn_cast<GetElementPtrInst>(store->getOperand(1)))
                  {
                // store_gep(instruction,BB,orginal_other,orginal_GEP,copie_other,copie_GEP);
                  }
                else
                  {
                  if (store->getOperand(0)->getType()->isDoubleTy())
                    {
                    store_var_double(instruction,BB,orginal_bin,orginal_alloca,copie_bin,copie_alloca);                  
                    }
                  else
                    {
                    store_var_other(instruction,BB,orginal_other_bin,orginal_other_alloca,copie_other_bin,copie_other_alloca);
                    }
                  }     
                }
              }
              //*************************************load inst******************************************
            else if (LoadInst *load = dyn_cast<LoadInst>(instruction))
              {
              if (load->getType()->isDoubleTy())
                {
                load_aloca_doubles(instruction,BB,orginal_alloca,copie_alloca,orginal_load,copie_load);
                }
              else
                {
                load_aloca_other(instruction,BB,orginal_other_alloca,copie_other_alloca,orginal_other_load,copie_other_load);
                }      
              }
              //******************************binary inst *******************************************
            else if (BinaryOperator* binOp = dyn_cast<BinaryOperator>(instruction))
              {
              if (binOp->getType()->isDoubleTy())
                {
                if(dyn_cast<LoadInst>(binOp->getOperand(0))&&dyn_cast<LoadInst>(binOp->getOperand(1)))
		              {
		              binop_2load_doubles(instruction,BB,orginal_bin,orginal_load,copie_load,copie_bin);
		              } 
		            else
		             {
		             if((dyn_cast<LoadInst>(binOp->getOperand(1))&&dyn_cast<ConstantFP>(binOp->getOperand(0))) || (dyn_cast<LoadInst>(binOp->getOperand(0))&&dyn_cast<ConstantFP>(binOp->getOperand(1))) )
		              {
		              binop_load_const_double(instruction,BB,orginal_bin,orginal_load,copie_load,copie_bin);
		              }    
		             } 
		            }//if doublty 
		          else    //not Double binary operation
                {
                if(dyn_cast<LoadInst>(binOp->getOperand(0))&&dyn_cast<LoadInst>(binOp->getOperand(1)))
		              {
		              binop_2load_notdoubles(instruction,BB,orginal_other_bin,orginal_other_load,copie_other_load,copie_other_bin);
		              }
                else if((dyn_cast<LoadInst>(binOp->getOperand(0)) &&dyn_cast<ConstantInt>(binOp->getOperand(1)))||(dyn_cast<LoadInst>(binOp->getOperand(1)) &&dyn_cast<ConstantInt>(binOp->getOperand(0))))
                  {
                  binop_load_const_notdouble(instruction,BB,orginal_other_bin,orginal_other_load,copie_other_load,copie_other_bin);
                  } 
                else
                  {
                  if((dyn_cast<LoadInst>(binOp->getOperand(0))) || (dyn_cast<LoadInst>(binOp->getOperand(1))))
                    {
                   // binop_1load_notdouble(instruction,BB,orginal_other_bin,orginal_other_load,orginal_other,copie_other_load,copie_other,copie_other_bin);
                    }                     
                  } 
                }	       
              }
             //*********************************************callinst*********************************
            else if(CallInst *call=dyn_cast<CallInst>(instruction))
              {
              if(dyn_cast<LoadInst>(call->getOperand(0))&&dyn_cast<LoadInst>(call->getOperand(1)))
                {
                call_2_load(instruction,BB,orginal_load,copie_load,orginal_other,copie_other);  
                }
              else if (dyn_cast<LoadInst>(call->getOperand(1)))  
                {
                call_1_load(instruction,BB,orginal_load,copie_load,orginal_other_load,copie_other_load);  
                }
              else if(dyn_cast<AllocaInst>(call->getOperand(1)) )
                {
                call_aloca(instruction,BB,orginal_alloca,copie_alloca,orginal_other_alloca,copie_other_alloca); 
                }
              else if(dyn_cast<GetElementPtrInst>(call->getOperand(1)))
                {
               // call_GEP(instruction,BB,orginal_GEP,copie_GEP,orginal_other_GEP,copie_other_GEP);
                }      
              }
              //********************************************icmpinst*********************************
            else if(ICmpInst *icmp=dyn_cast<ICmpInst>(instruction))
              {            
              if(dyn_cast<LoadInst>(icmp->getOperand(0))&&dyn_cast<LoadInst>(icmp->getOperand(1)))
                {
                icmp_2load(instruction,BB,orginal_icmp,orginal_other_load,copie_other_load,copie_icmp);
                }
              else
                {
                icmp_1load(instruction,BB,orginal_icmp,orginal_other_load,copie_other_load,copie_icmp);      
                }                  
              }
              //**************************************otherinst**************************************
            else 
              {
              Instruction *I= instruction->clone();
              BB->getInstList().push_back(I);
              IBorg.push_back(instruction);
              IBclone.push_back(I);
              }  
            }
          BBclone.push_back(BB);
          }    
        for(int i=0;i<(BBorg.size());i++)
          {
          if (BranchInst *CI = dyn_cast<BranchInst>(BBclone[i]->getTerminator ()))
            {
            if(CI->isConditional())
              {
              int index= Search(BBorg,CI->getSuccessor(0));
              int  index1= Search(BBorg,CI->getSuccessor(1)); 
              int n=Searchinst(orginal_icmp,dyn_cast<Instruction>(CI->getCondition()));
              BranchInst::Create (BBclone[index],BBclone[index1] ,copie_icmp[n],BBclone[i]);
              }
            else
              {
              int index= Search(BBorg,CI->getSuccessor(0));
              BranchInst::Create(BBclone[index],BBclone[i]);
              }
            CI->eraseFromParent();    
            }  
          }
        if (ReturnInst *ret=dyn_cast<ReturnInst>(BBclone[i]->getTerminator ()))
          {
          Type* doublety= Type::getDoubleTy(context);
          if (dyn_cast<BinaryOperator>(ret->getOperand(0)))
            {
            int n=Searchinst(orginal_bin,dyn_cast<Instruction>(ret->getOperand(0)));
            FPExtInst *c= new FPExtInst  (copie_bin[n], doublety, "");
            c->insertAfter(copie_bin[n]);
            ret->setOperand(0,c);
            }
          }    
        BBclone.clear();
        BBorg.clear();
        }
      }
  };
}
char test::ID = 0;
static RegisterPass<test> X("T", "Hello World Pass", false, false);

