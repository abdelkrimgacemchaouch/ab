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
#include "binaryop.hpp"
void binary(Instruction* instruction,vector<Instruction*> &inst1,vector<Instruction*> &inst2,BasicBlock *BB)
  {
  LLVMContext &context = instruction->getContext();
  Type* type = Type::getFloatTy(context);
  BinaryOperator* binOp = dyn_cast<BinaryOperator>(instruction);
  BinaryOperator* bi;
  int n=Searchinst(inst1,dyn_cast<Instruction>(binOp->getOperand(0)));
  int n1=Searchinst(inst1,dyn_cast<Instruction>(binOp->getOperand(1)));
  if(dyn_cast<LoadInst>(binOp->getOperand(0))&&dyn_cast<LoadInst>(binOp->getOperand(1)))
    {
    bi=BinaryOperator::Create(binOp->getOpcode(),inst2[n],inst2[n1],"add");//if both operands are  instructions
    }
    
    //*****************************if one of operands is constant ***********************
  else if (dyn_cast<Constant>(binOp->getOperand(0))||dyn_cast<Constant>(binOp->getOperand(1)))
    {
    if (n>inst1.size()) //1st operand is constant 
      {
      //******************** if the constant is float point ********   
      if (ConstantFP *cc=dyn_cast<ConstantFP>(binOp->getOperand(0)))  
        {
        double a= APFloat(cc->getValueAPF()).convertToDouble ();
        Value*x1 = ConstantFP::get(type,a);
        bi=BinaryOperator::Create(binOp->getOpcode(),x1,inst2[n1],"add");
        }
      else 
        {
        bi=BinaryOperator::Create(binOp->getOpcode(),binOp->getOperand(0),inst2[n1],"add");//if constant is another type
        }  
      }
    else if (n1>inst1.size()) //2nd operand is constant 
     {
      if (ConstantFP *cc=dyn_cast<ConstantFP>(binOp->getOperand(1)))
        {
        double a= APFloat(cc->getValueAPF()).convertToDouble ();
        Value* x1 = ConstantFP::get(type,a);
        bi=BinaryOperator::Create(binOp->getOpcode(),inst2[n],x1,"add");
        }
      else 
        {
        bi=BinaryOperator::Create(binOp->getOpcode(),inst2[n],binOp->getOperand(1),"add");
        }  
      }
    }
  inst1.push_back(binOp);   
  inst2.push_back(bi);
  BB->getInstList().push_back(bi);
  }
