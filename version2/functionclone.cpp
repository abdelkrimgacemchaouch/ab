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
void clone(vector<Function*>&T1,vector<Type*>&P,Module &M)
 {
 for(Module::iterator f = M.begin(), fe = M.end(); f != fe; f++) 
  {
  if(inst_begin(f)!=inst_end(f))
    {
    T1.push_back(f); //take all the functions      
    }
  } 
//******************* create the type of arguments for new functions******************   
 for (int i=0;i<T1.size();i++)
  {  
  LLVMContext &context = T1[i]->getContext();
  for (Function::arg_iterator I = T1[i]->arg_begin(), E = T1[i]->arg_end(); I != E; ++I)
    {
    if(I->getType()->isDoubleTy())
      {
      P.push_back(Type::getFloatTy(context));
      }
    else
      {
      P.push_back(I->getType());
      }
    }
  }
 }

//*************************create new function*********************************************************** 
void clone2(vector<Function*>&T1,vector<Function*>&T2,Module &M)
  {
  FunctionType *FT;
  std::vector<Type*> m,S;
  clone(T1,S,M);
  int a =0;
  for (int i=0;i<T1.size();i++)
    { 
    for (Function::const_arg_iterator I = T1[i]->arg_begin(), E = T1[i]->arg_end(); I != E; ++I)
      {
      m.push_back(S[a]); //take the type argument of the function
      a++;
      }
    FT = FunctionType::get(T1[i]->getFunctionType()->getReturnType(),m,false); //return type of the new function
    Function* func = Function::Create(FT, Function::ExternalLinkage, "Foo", &M);// new function 
    T2.push_back(func); 
    m.clear();
    }  
  }
