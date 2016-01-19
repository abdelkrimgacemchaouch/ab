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
#include "cloneinst.hpp"
#include "binaryop.hpp"
#include "storeinst.hpp"
#include "icmpinst.hpp"
#include "callinst.hpp"
bool runOnFunction(Module &M, Function &F,Function &F1) 
{
vector<BasicBlock*> BBorg,BBclone;//  save the originals(BBorg) and copies(BBclone) of the blocks
vector<Instruction*> inst1,inst2; //  save the instructions
LLVMContext &context = M.getContext();
Type* type = Type::getFloatTy(context);
vector<Argument*> A1,A2; // save the arguments of each functions
takeA(F,A1); 
takeA(F1,A2);
for (Function::iterator blocdebase = F.begin(), e = F.end(); blocdebase != e; ++blocdebase)
  {
  BBorg.push_back(blocdebase); 
  BasicBlock *BB = BasicBlock::Create(context, "EntryBlock", &F1);
  for (BasicBlock::iterator instruction = blocdebase->begin(), ee = blocdebase->end(); instruction != ee; ++instruction)
    {
    switch (instruction->getValueID())
      {
      //************************************* for return instructions************************
      case 20:
        {
        Type* doublety= Type::getDoubleTy(context);
        Instruction *I=instruction->clone();
        int n=Searchinst(inst1,dyn_cast<Instruction>(instruction->getOperand(0)));
       if(n<inst1.size())
        {
        if(inst2[n]->getType()->isFloatTy())
          {
          FPExtInst *c= new FPExtInst  (inst2[n], doublety,"");
          inst2.push_back(c);
          BB->getInstList().push_back(c);
          inst1.push_back(c);
          I->setOperand(0,c);
          }
        }
       inst2.push_back(I);
       BB->getInstList().push_back(I);
       inst1.push_back(instruction);    
        }
        break;
        
        //****************************** for load instructions****************
      case 46:
        {
        LoadInst *load=dyn_cast<LoadInst>(instruction);
        int n=Searchinst(inst1,dyn_cast<Instruction>(instruction->getOperand(0)));
        LoadInst *new_load=new LoadInst(inst2[n]," ",false);    
        inst2.push_back(new_load);
        inst1.push_back(load);
        BB->getInstList().push_back(new_load);
        }
        break;
     
        //**************************** for binary instructions***************
      case 28:
      case 27:  
        binary(instruction,inst1,inst2,BB);          
        break;
        
     //******************************* for store instructions****************
      case 47:
        {
        store(instruction,inst1,inst2,A1,A2,BB);               
        }
        break;
        
      //******************************* for allocation ****************  
      case 45:
        { 
        AllocaInst *alloca= dyn_cast<AllocaInst>(instruction);
        if(alloca->getAllocatedType ()->isDoubleTy())
          {
          AllocaInst *new_alloca = new AllocaInst(type,0, 4, "a");
          inst2.push_back(new_alloca);
          BB->getInstList().push_back(new_alloca);
          }
        else
          {
          Instruction *I=instruction->clone();
          BB->getInstList().push_back(I);
          inst2.push_back(I);
          } 
        inst1.push_back(instruction);
        }
        break;
         
      case 65:
        {
        ICMP(instruction,inst1,inst2,BB);
        }
        break; 
      case 68:
        {
        call(instruction,inst1,inst2,BB);// for call instructions
        }
        //******************************* other instructions*********************************  
      default: 
        break;
      }
    }
   BBclone.push_back(BB);     
  }
  for(int i=0;i<BBorg.size();i++)
    {
    if (BranchInst *CI = dyn_cast<BranchInst>(BBorg[i]->getTerminator ()))
      {
      if(CI->isConditional())
       {
       int index= Search(BBorg,CI->getSuccessor(0));
       int  index1= Search(BBorg,CI->getSuccessor(1)); 
       int n=Searchinst(inst1,dyn_cast<Instruction>(CI->getCondition()));
       BranchInst::Create (BBclone[index],BBclone[index1] ,inst2[n],BBclone[i]);
       }
     else
       {
       int index= Search(BBorg,CI->getSuccessor(0));
       BranchInst::Create(BBclone[index],BBclone[i]);
       }
      }
    }
BBclone.clear();
BBorg.clear();  
return true;
}
//************************************ to take Argument of function*****************
void takeA(Function &F,vector<Argument*>&A1)
  {
  for (Function::arg_iterator I = F.arg_begin(), E = F.arg_end(); I != E; ++I)
      {
      A1.push_back(I);
      }
  }

