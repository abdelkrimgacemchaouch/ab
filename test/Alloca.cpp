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
#include"Alloca.hpp"
#include <iostream>
#include <fstream>
using namespace llvm;
using namespace std;
void aloca(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
   //*******************************************************************************
   Instruction *I;
   LLVMContext &context = BB->getContext();
   Type* type = Type::getFloatTy(context);
  if(AllocaInst *alloca = dyn_cast<AllocaInst>(instruction) )
    {
    if(alloca->getAllocatedType()->isArrayTy())
      {
      if(alloca->getAllocatedType()->getArrayElementType ()->isDoubleTy())
        {
        T1.push_back(alloca);
        Type *ArrayTy = ArrayType::get(type, alloca->getAllocatedType()->getArrayNumElements ());              
        AllocaInst *new_alloca = new AllocaInst(ArrayTy,0,alloca->getAlignment(), "aray");
        BB->getInstList().push_back(new_alloca);
        T2.push_back(new_alloca);
        }
      else if(alloca->getAllocatedType()->getArrayElementType ()->isArrayTy())
        {
        T1.push_back(alloca);
        Type *MatTy = ArrayType::get(type,alloca->getAllocatedType()->getArrayElementType()->getArrayNumElements ());
        Type *MaTy = ArrayType::get(MatTy, alloca->getAllocatedType()->getArrayNumElements ());       
        AllocaInst *new_alloca = new AllocaInst(MaTy,0,alloca->getAlignment(), "mat");
        BB->getInstList().push_back(new_alloca);
        T2.push_back(new_alloca);
        }
      else
        {
        I=alloca->clone();
        BB->getInstList().push_back(I);
        T3.push_back(alloca);
        T4.push_back(I);                  
        }
      }
    else
      {  
      if(alloca->getAllocatedType ()->isDoubleTy())
        {
        T1.push_back(alloca);
        AllocaInst *new_alloca = new AllocaInst(type,0, 4, "a");
        T2.push_back(new_alloca);
        BB->getInstList().push_back(new_alloca);
        }
      else
        {
        I=alloca->clone();
        BB->getInstList().push_back(I);
        T3.push_back(alloca);
        T4.push_back(I);
        }
      }    
    }         
  }
