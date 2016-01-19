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
#include"functionclone.hpp"
#include "functionchoice.hpp"
#include "cloneinst.hpp"
#include <iostream>
#include <fstream>
using namespace llvm;
using namespace std;
namespace {
  struct test : public ModulePass {
    static char ID;
    test() : ModulePass(ID) {}
   virtual  bool runOnModule(Module &M) {
    LLVMContext &context = M.getContext();
    std::vector<Function*> Funorg,Funclone,Fu;
    //funorg:save the olds functions, funclone:save the news functions, fu: save functions which takes decision 
    clone2(Funorg,Funclone,M); //create the new function 
    for (int i=0;i<Funorg.size();i++)
     {
     runOnFunction(M,*Funorg[i],*Funclone[i]); // filling the new functions
     }
  //*****************************************creat the way to choice ********************************************************       
      //create initials values  
      Constant *da= ConstantInt::get(Type::getInt32Ty(context), 0);
      Constant *da1= ConstantInt::get(Type::getInt32Ty(context), 1);
      GlobalVariable *val1=new GlobalVariable(M,Type::getInt32Ty(context),false,GlobalValue::ExternalLinkage,nullptr);
      val1->setName("a");
      val1->setInitializer(da);
      GlobalVariable *val2=new GlobalVariable(M,Type::getInt32Ty(context),false,GlobalValue::ExternalLinkage,nullptr);
      val2->setName("b");
      val2->setInitializer(da1);
      
      CreatChoice(Funorg,Funclone,Fu,val1,val2, M); 
return true;
}
  };
}
char test::ID = 0;
static RegisterPass<test> X("ttt", "Hello World Pass", false, false);

