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
#include "Alloca.hpp"
#include "Load.hpp"
#include "BinairOperation.hpp"
using namespace llvm;
using namespace std;
BinaryOperator *binOpcopie;
//******************************************Operation is double*******************************************************
//two operand is two load
void binop_2load_doubles(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  int index, index2;
  BinaryOperator* binOp = dyn_cast<BinaryOperator>(instruction);
  T1.push_back(binOp);
  index =Searchinst(T2, dyn_cast<LoadInst>(binOp->getOperand(0)));
	index2=Searchinst(T2, dyn_cast<LoadInst>(binOp->getOperand(1)));
	binOpcopie = BinaryOperator::Create(binOp->getOpcode() ,T3[index],T3[index2] ,"add");
	BB->getInstList().push_back(binOpcopie);
	T4.push_back(binOpcopie);  
	}
//one operand is load the other is constant 
void binop_load_const_double(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  int index;
  LLVMContext &context = BB->getContext();
  Type* type = Type::getFloatTy(context);
  FPTruncInst *c;
  BinaryOperator* binOp = dyn_cast<BinaryOperator>(instruction);
  T1.push_back(binOp);
	if(dyn_cast<LoadInst>(binOp->getOperand(1)))
	  {
	  index =Searchinst(T2, dyn_cast<LoadInst>(binOp->getOperand(1)));
	  }
	else if(dyn_cast<LoadInst>(binOp->getOperand(0))) 
	  {
	  index =Searchinst(T2, dyn_cast<LoadInst>(binOp->getOperand(0)));
	  }
	if(dyn_cast<LoadInst>(binOp->getOperand(1)))
	  {
	  c= new FPTruncInst  (binOp->getOperand(0), type, "conv");
	  }
	else if(dyn_cast<LoadInst>(binOp->getOperand(0)))
	  {
	  c= new FPTruncInst  (binOp->getOperand(1), type, "conv");
	  }  
	c->insertAfter(instruction);
	 Value* x1;
	if(ConstantFP *cc= dyn_cast<ConstantFP>(c->getOperand(0)))
    {
    double a= APFloat(cc->getValueAPF()).convertToDouble ();
    x1 = ConstantFP::get(type,a);
    c->eraseFromParent();
    }
	if(dyn_cast<LoadInst>(binOp->getOperand(1)))
	  {
	  binOpcopie = BinaryOperator::Create(binOp->getOpcode(),x1,T3[index],"add");
	  }
	else if(dyn_cast<LoadInst>(binOp->getOperand(0)))
	  {
	  binOpcopie = BinaryOperator::Create(binOp->getOpcode(),T3[index],x1,"add");
	  }  
	BB->getInstList().push_back(binOpcopie);
	T4.push_back(binOpcopie);	
  }  
  //********************************************************************************************************
  //********************************** operation Not double*************************************************
void binop_2load_notdoubles(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  BinaryOperator* binOp = dyn_cast<BinaryOperator>(instruction);
  T1.push_back(binOp); 
  Instruction *I = binOp->clone(); 
	int indexinst =Searchinst(T2, dyn_cast<LoadInst>(binOp->getOperand(0)));
	int indexinst2=Searchinst(T2, dyn_cast<LoadInst>(binOp->getOperand(1)));
	I->setOperand(0,T3[indexinst]);
	I->setOperand(1,T3[indexinst2]);
	BinaryOperator *Add = dyn_cast<BinaryOperator>(I);
  BB->getInstList().push_back(Add);
  T4.push_back(Add);  
  }
void binop_load_const_notdouble(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4)
  {
  int indexinst;
  BinaryOperator* binOp = dyn_cast<BinaryOperator>(instruction);
  T1.push_back(binOp); 
  Instruction *I = binOp->clone();
  if(dyn_cast<LoadInst>(binOp->getOperand(0)))
    {
    indexinst =Searchinst(T2, dyn_cast<LoadInst>(I->getOperand(0)));
    I->setOperand(0,T3[indexinst]);
    }
  else if(dyn_cast<LoadInst>(binOp->getOperand(1)))
    {
    indexinst =Searchinst(T2, dyn_cast<LoadInst>(I->getOperand(1)));
    I->setOperand(1,T3[indexinst]);
    }  
  BinaryOperator *Add = dyn_cast<BinaryOperator>(I);
  BB->getInstList().push_back(Add);
  T4.push_back(Add);
  }
  //when one operand is load and other we don't know what  
void binop_1load_notdouble(Instruction *instruction,BasicBlock *BB,vector<Instruction*> &T1,vector<Instruction*> &T2,vector<Instruction*> &T3,vector<Instruction*> &T4,vector<Instruction*> &T5,vector<Instruction*> &T6)
  {
  int indexinst,indexinst2;
  BinaryOperator* binOp = dyn_cast<BinaryOperator>(instruction);
  T1.push_back(binOp); 
  Instruction *I = binOp->clone();
  if(LoadInst *load = dyn_cast<LoadInst>(binOp->getOperand(0)))
    {
    indexinst =Searchinst(T2, dyn_cast<LoadInst>(I->getOperand(0)));
    indexinst2=Searchinst(T3, dyn_cast<Instruction>(I->getOperand(1)));
    I->setOperand(0,T4[indexinst]);
    I->setOperand(1,T5[indexinst2]);
    }
  if(LoadInst *load = dyn_cast<LoadInst>(binOp->getOperand(1)))
    {
    indexinst =Searchinst(T2, dyn_cast<LoadInst>(I->getOperand(1)));
    indexinst2=Searchinst(T3, dyn_cast<Instruction>(I->getOperand(0)));
    I->setOperand(0,T5[indexinst2]);
    I->setOperand(1,T4[indexinst]);
    }    
  BinaryOperator *Add = dyn_cast<BinaryOperator>(I);
  BB->getInstList().push_back(Add);
  T6.push_back(Add);
  }   
