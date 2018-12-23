#include <stdio.h>
#include <stdlib.h>
#include "ast.hpp"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Scalar.h>
#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR >= 4
#include <llvm/Transforms/Scalar/GVN.h>
#endif

using namespace llvm;

static ast ast_make (kind k, char c, int n, ast l, ast r) {
  ast p;
  if ((p = (ast) malloc(sizeof(struct node))) == NULL)
    exit(1);
  p->k = k;
  p->id = c;
  p->num = n;
  p->left = l;
  p->right = r;
  return p;
}

ast ast_id (char c) {
  return ast_make(ID, c, 0, NULL, NULL);
}

ast ast_const (int n) {
  return ast_make(CONST, '\0', n, NULL, NULL);
}

ast ast_op (ast l, kind op, ast r) {
  return ast_make(op, '\0', 0, l, r);
}

ast ast_print (ast l) {
  return ast_make(PRINT, '\0', 0, l, NULL);
}

ast ast_let (char c, ast l) {
  return ast_make(LET, c, 0, l, NULL);
}

ast ast_for (ast l, ast r) {
  return ast_make(FOR, '\0', 0, l, r);
}

ast ast_if (ast l, ast r) {
  return ast_make(IF, '\0', 0, l, r);
}

ast ast_seq (ast l, ast r) {
  if (r == NULL) return l;
  return ast_make(SEQ, '\0', 0, l, r);
}

// Global LLVM variables related to the LLVM suite.
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<legacy::FunctionPassManager> TheFPM;

// Global LLVM variables related to the generated code.
static GlobalVariable *TheVars;
static GlobalVariable *TheNL;
static Function *TheWriteInteger;
static Function *TheWriteString;

// Useful LLVM types.
static Type * i8 = IntegerType::get(TheContext, 8);
static Type * i32 = IntegerType::get(TheContext, 32);
static Type * i64 = IntegerType::get(TheContext, 64);

// Useful LLVM helper functions.
inline ConstantInt* c8(char c) {
  return ConstantInt::get(TheContext, APInt(8, c, true));
}
inline ConstantInt* c32(int n) {
  return ConstantInt::get(TheContext, APInt(32, n, true));
}

Value * ast_compile (ast t) {
  if (t == nullptr) return nullptr;
  switch (t->k) {
    case PRINT: {
      Value *n = ast_compile(t->left);
      Value *n64 = Builder.CreateZExt(n, i64, "ext");
      Builder.CreateCall(TheWriteInteger, std::vector<Value *>{ n64 });
      Value *idxList[] = { c32(0), c32(0) };
      Value *nl = Builder.CreateGEP(TheNL, idxList, "nl");
      Builder.CreateCall(TheWriteString, std::vector<Value *>{ nl });
      return nullptr;
    }
    case LET: {
      char name[] = { t->id, '_', 'p', 't', 'r', '\0' };
      Value *lhs = Builder.CreateGEP(
          TheVars, std::vector<Value *>{ c32(0), c32(t->id - 'a') }, name);
      Value *rhs = ast_compile(t->left);
      return Builder.CreateStore(rhs, lhs);
    }
    case FOR: {
      // Emit the number of iterations.
      Value *n = ast_compile(t->left);
      // Make the new basic block for the loop.
      Function *TheFunction = Builder.GetInsertBlock()->getParent();
      BasicBlock *PreheaderBB = Builder.GetInsertBlock();
      BasicBlock *LoopBB = BasicBlock::Create(TheContext, "loop", TheFunction);
      // Insert an explicit fall-through from the current block.
      Builder.CreateBr(LoopBB);
      // Start insertion in the loop.
      Builder.SetInsertPoint(LoopBB);
      // Create the phi node and start it with the number of iterations.
      PHINode *phi_iter = Builder.CreatePHI(i32, 2, "iter");
      phi_iter->addIncoming(n, PreheaderBB);
      // Create the end loop condition.
      Value *cond = Builder.CreateICmpSGT(phi_iter, c32(0), "loop_cond");
      // Create the "after loop" block.
      BasicBlock *InsideBB =
          BasicBlock::Create(TheContext, "inside", TheFunction);
      BasicBlock *AfterBB =
          BasicBlock::Create(TheContext, "after", TheFunction);
      Builder.CreateCondBr(cond, InsideBB, AfterBB);
      Builder.SetInsertPoint(InsideBB);
      // Decrease the number of iterations.
      Value *remaining = Builder.CreateSub(phi_iter, c32(1), "remaining");
      // Emit the body of the loop.
      ast_compile(t->right);
      // Loop back.
      phi_iter->addIncoming(remaining, Builder.GetInsertBlock());
      Builder.CreateBr(LoopBB);
      // End of loop.
      Builder.SetInsertPoint(AfterBB);
      return nullptr;
    }
    case IF: {
      Value *v = ast_compile(t->left);
      Value *cond = Builder.CreateICmpNE(v, c32(0), "if_cond");
      Function *TheFunction = Builder.GetInsertBlock()->getParent();
      BasicBlock *InsideBB =
          BasicBlock::Create(TheContext, "then", TheFunction);
      BasicBlock *AfterBB =
          BasicBlock::Create(TheContext, "endif", TheFunction);
      Builder.CreateCondBr(cond, InsideBB, AfterBB);
      Builder.SetInsertPoint(InsideBB);
      ast_compile(t->right);
      Builder.CreateBr(AfterBB);
      Builder.SetInsertPoint(AfterBB);
      return nullptr;
    }
    case SEQ: {
      ast_compile(t->left);
      ast_compile(t->right);
      return nullptr;
    }
    case ID: {
      char name[] = { t->id, '_', 'p', 't', 'r', '\0' };
      Value *v = Builder.CreateGEP(
          TheVars, std::vector<Value *>{ c32(0), c32(t->id - 'a') }, name);
      name[1] = '\0';
      return Builder.CreateLoad(v, name);
    }
    case CONST: {
      return c32(t->num);
    }
    case PLUS: {
      Value *l = ast_compile(t->left);
      Value *r = ast_compile(t->right);
      return Builder.CreateAdd(l, r, "addtmp");
    }
    case MINUS: {
      Value *l = ast_compile(t->left);
      Value *r = ast_compile(t->right);
      return Builder.CreateSub(l, r, "subtmp");
    }
    case TIMES: {
      Value *l = ast_compile(t->left);
      Value *r = ast_compile(t->right);
      return Builder.CreateMul(l, r, "multmp");
    }
    case DIV: {
      Value *l = ast_compile(t->left);
      Value *r = ast_compile(t->right);
      return Builder.CreateSDiv(l, r, "divtmp");
    }
    case MOD: {
      Value *l = ast_compile(t->left);
      Value *r = ast_compile(t->right);
      return Builder.CreateSRem(l, r, "modtmp");
    }
  }
  return nullptr;
}

void llvm_compile_and_dump (ast t) {
  // Initialize the module and the optimization passes.
  TheModule = make_unique<Module>("minibasic program", TheContext);
  TheFPM = make_unique<legacy::FunctionPassManager>(TheModule.get());
  TheFPM->add(createPromoteMemoryToRegisterPass());
  TheFPM->add(createInstructionCombiningPass());
  TheFPM->add(createReassociatePass());
  TheFPM->add(createGVNPass());
  TheFPM->add(createCFGSimplificationPass());
  TheFPM->doInitialization();
  // Define and initialize global symbols.
  // @vars = global [26 x i32] zeroinitializer, align 16
  ArrayType *vars_type = ArrayType::get(i32, 26);
  TheVars = new GlobalVariable(
      *TheModule, vars_type, false, GlobalValue::PrivateLinkage,
      ConstantAggregateZero::get(vars_type), "vars");
  TheVars->setAlignment(16);
  // @nl = private constant [2 x i8] c"\0A\00", align 1
  ArrayType *nl_type = ArrayType::get(i8, 2);
  TheNL = new GlobalVariable(
      *TheModule, nl_type, true, GlobalValue::PrivateLinkage,
      ConstantArray::get(nl_type,
                         std::vector<Constant *>{ c8('\n'), c8('\0') }),
      "nl");
  TheNL->setAlignment(1);
  // declare void @writeInteger(i64)
  FunctionType *writeInteger_type =
    FunctionType::get(Type::getVoidTy(TheContext),
                      std::vector<Type *>{ i64 }, false);
  TheWriteInteger =
    Function::Create(writeInteger_type, Function::ExternalLinkage,
                     "writeInteger", TheModule.get());
  // declare void @writeString(i8*)
  FunctionType *writeString_type =
    FunctionType::get(Type::getVoidTy(TheContext),
                      std::vector<Type *>{ PointerType::get(i8, 0) }, false);
  TheWriteString =
    Function::Create(writeString_type, Function::ExternalLinkage,
                     "writeString", TheModule.get());
  // Define and start the main function.
  Constant *c = TheModule->getOrInsertFunction("main", i32, NULL);
  Function* main = cast<Function>(c);
  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", main);
  Builder.SetInsertPoint(BB);
  // Emit the program code.
  ast_compile(t);
  Builder.CreateRet(c32(0));
  // Verify and optimize the main function.
  bool bad = verifyModule(*TheModule, &errs());
  if (bad) {
    fprintf(stderr, "The faulty IR is:\n");
    fprintf(stderr, "------------------------------------------------\n\n");
    TheModule->print(outs(), nullptr);
    return;
  }
  TheFPM->run(*main);
  // Print out the IR.
  TheModule->print(outs(), nullptr);
}
