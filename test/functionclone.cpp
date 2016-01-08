#include "llvm/Pass.h"//
#include "llvm/IR/Function.h"//
#include "llvm/IR/Module.h"//
#include "llvm/Support/raw_ostream.h"//
#include "llvm/IR/Type.h"//
#include "llvm/IR/CallSite.h"//
#include "llvm/IR/Instructions.h"//
#include "llvm/IR/InstIterator.h"//
#include "llvm/IR/Constants.h"//
#include"functionclone.hpp"
#include <iostream>
#include <fstream>
using namespace llvm;
using namespace std;
//*****************take the Attribut and save the original function
void clone(Function *F,vector<Function*>&T1,vector<Argument*>&A1,vector<Type*>&P)
  {
  LLVMContext &context = F->getContext();
  if(inst_begin(F)!=inst_end(F))
    {
    T1.push_back(F);
    for (Function::arg_iterator I = F->arg_begin(), E = F->arg_end(); I != E; ++I)
      {
      if(I->getType()->isDoubleTy())
        {
        P.push_back(Type::getFloatTy(context));
        }
      else
        {
        P.push_back(I->getType());//take the type of the attribute of function orginal
        }
      A1.push_back(I);
      }
    }
  }
//*********************************************************************************************************************
//*************************creat new function*********************************************************** 
void clone2(vector<Function*>&T1,vector<Function*>&T2,vector<Argument*>&A1,vector<Type*>&P,Module &M)
  {
  FunctionType *FT;
  std::vector<Type*> m;
  int a =0;
  for (int i=0;i<T1.size();i++)
    { 
    for (Function::const_arg_iterator I = T1[i]->arg_begin(), E = T1[i]->arg_end(); I != E; ++I)
      {
      m.push_back(P[a]); //take type of attribut of specific function
      a++;
      }
    FT = FunctionType::get(T1[i]->getFunctionType()->getReturnType(),m,false);
    Function* func = Function::Create(FT, Function::ExternalLinkage, "Foo", &M);
    T2.push_back(func); 
    m.clear();
    }
  for (int i=0;i<T2.size();i++)
    {
    for (Function::arg_iterator I = T2[i]->arg_begin(), E = T2[i]->arg_end(); I != E; ++I)
      {
      A1.push_back(I);
      }
    }  
  }
