#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
//#include <cstdio>
//#include <cstdlib>
#include "MYast.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR >= 4
#include <llvm/Transforms/Scalar/GVN.h>
#endif
extern "C"
{
#include "error.h"
#include "symbol.h"
}

using namespace llvm;
Value* ast_compile(ast t);

static ast ast_make(kind k, const char *c, int n, ast l1, ast l2, ast l3, ast l4, Type_h t)
{
    //printf("another node in ast\n");
    ast p;
    if ((p = (ast) malloc(sizeof(struct node))) == NULL)
        exit(1);
    p->k = k;
    p->id = (char *) malloc(sizeof(char)*(strlen(c) + 1));
    //p->id = c;
    strcpy(p->id, c);
    p->num = n;
    p->branch1 = l1;
    p->branch2 = l2;
    p->branch3 = l3;
    p->branch4 = l4; //each node in the ast has <=4 children
    p->type = t;
    //printf("node created\n");
    return p;
}

// ast ast_program(ast l1) {
//   return ast_make(PROGRAM, "\0", 0, l1, NULL, NULL, NULL, NULL);
// }

ast ast_func_def(ast header, ast local, ast block)
{
    return ast_make(FUNC_DEF, "\0", 0, header, local, block, NULL, NULL);
}

ast ast_header(const char *l1, Type_h t, ast l2, ast l3)
{
    return ast_make(HEADER, l1, 0, l2, l3, NULL, NULL, t);
}

ast ast_header_part(ast l1, ast l2)
{
    return ast_make(HEADER_PART, "\0", 0, l1, l2, NULL, NULL, NULL);
}

ast ast_fpar_def(ast l1, Type_h t)
{
    return ast_make(FPAR_DEF, "\0", 0, l1, NULL, NULL, NULL, t);
}

// ast ast_func_decl(ast l1) {
//   return ast_make(FUNC_DECL, "\0", 0, l1, NULL, NULL, NULL, NULL);
// }

ast ast_decl(ast l1)
{
    return ast_make(DECL, "\0", 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_var(ast idlist, Type_h t)
{
    return ast_make(VAR, "\0", 0, idlist, NULL, NULL, NULL, t);
}

ast ast_id(const char *s, ast next)
{
    return ast_make(ID, s, 0, next, NULL, NULL, NULL, NULL);
}

ast ast_op(ast l, kind op, ast r)
{
    return ast_make(op, "\0", 0, l, r, NULL, NULL, NULL);
}

ast ast_skip()
{
    return ast_make(SKIP, "\0", 0, NULL, NULL, NULL, NULL, NULL);
}

//assign a value to a variable
ast ast_assign(ast l, ast r)
{
    return ast_make(ASSIGN, "\0", 0, l, r, NULL, NULL, NULL);
}

ast ast_exit()
{
    return ast_make(EXIT, "\0", 0, NULL, NULL, NULL, NULL, NULL);
}

ast ast_return(ast l1)
{
    return ast_make(RET, "\0", 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_if(ast l1, ast l2, ast l3)
{
    return ast_make(IF, "\0", 0, l1, l2, l3, NULL, NULL);
}

ast ast_if_else(ast cond, ast blck, ast ifp, ast elblck)
{
    return ast_make(IF_ELSE, "\0", 0, cond, blck, ifp, elblck, NULL);
}

ast ast_loop(const char *s, ast l1)
{
    return ast_make(LOOP, s, 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_break(const char *s)
{
    if (s == NULL)
        return ast_make(BREAK, "\0", 0, NULL, NULL, NULL, NULL, NULL);
    return ast_make(BREAK, s, 0, NULL, NULL, NULL, NULL, NULL);
}

ast ast_continue(const char *s)
{
    if (s == NULL)
        return ast_make(CONT, "\0", 0, NULL, NULL, NULL, NULL, NULL);
    return ast_make(CONT, s, 0, NULL, NULL, NULL, NULL, NULL);
}

ast ast_seq(ast l1, ast l2)
{
    //if (l2 == NULL) return l1;
    return ast_make(SEQ, "\0", 0, l1, l2, NULL, NULL, NULL);
}

ast ast_block(ast l1)
{
    return ast_make(BLOCK, "\0", 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_tid(const char *s)
{
    return ast_make(TID, s, 0, NULL, NULL, NULL, NULL, NULL);
}

ast ast_string_lit(const char *s)
{
    return ast_make(STRING_LIT, s, 0, NULL, NULL, NULL, NULL, NULL);
}

ast ast_arr(ast l1, ast l2)
{
    return ast_make(ARR, "\0", 0, l1, l2, NULL, NULL, NULL);
}

ast ast_int_const(int num)
{
    return ast_make(INTCONST, "\0", num, NULL, NULL, NULL, NULL, NULL);
}

ast ast_char_const(char c)
{
    char name[2];
    name[0] = c;
    return ast_make(CHARCONST, name, 0, NULL, NULL, NULL, NULL, NULL);
}

ast ast_proc_call(const char *s, ast l1, ast l2)
{
    return ast_make(PROC_CALL, s, 0, l1, l2, NULL, NULL, NULL);
}

ast ast_func_call(const char *s, ast l1, ast l2)
{
    return ast_make(FUNC_CALL, s, 0, l1, l2, NULL, NULL, NULL);
}

ast ast_expr_part(ast l1, ast l2)
{
    return ast_make(EXPR_PART, "\0", 0, l1, l2, NULL, NULL, NULL);
}

ast ast_not_expr(ast l1)
{
    return ast_make(EXPR_NOT, "\0", 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_and_expr(ast l1, ast l2)
{
    return ast_make(EXPR_AND, "\0", 0, l1, l2, NULL, NULL, NULL);
}

ast ast_or_expr(ast l1, ast l2)
{
    return ast_make(EXPR_OR, "\0", 0, l1, l2, NULL, NULL, NULL);
}

// Global LLVM variables related to the LLVM suite.
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::unique_ptr<legacy::FunctionPassManager> TheFPM;

// Global LLVM variables related to the generated code.
static GlobalVariable *TheNL;
static Function *TheWriteInteger;
static Function *TheWriteString;
static Function *TheWriteChar;
static Function *TheWriteByte;
static Function *TheReadInteger;
static Function *TheReadString;
static Function *TheReadChar;
static Function *TheReadByte;
static Function *extend;
static Function *shrink;
static Function *TheStrlen;
static Function *TheStrcmp;
static Function *TheStrcpy;
static Function *TheStrcat;
StructType *current_AR = NULL;
AllocaInst *currentAlloca = NULL;
AllocaInst *retAlloca;
BasicBlock *returnBlock;
std::map<char *, BasicBlock *> loopMap;

// Useful LLVM types.
static Type *i8 = IntegerType::get(TheContext, 8);
static Type *i32 = IntegerType::get(TheContext, 32);
static Type *i64 = IntegerType::get(TheContext, 64);

// Useful LLVM helper functions.
inline ConstantInt *c8(char c)
{
    return ConstantInt::get(TheContext, APInt(8, c, true));
}
inline ConstantInt *c32(int n)
{
    return ConstantInt::get(TheContext, APInt(32, n, true));
}

Type *translateType(Type_h type)
{
    if (equalType(type, typeInteger))
        return i32;
    if (equalType(type, typeChar))
        return i8;
    if (equalType(type, typeVoid))
        return Type::getVoidTy(TheContext);
    if (isArray(type))
    {
        Type_h ty = type->refType;
        std::vector<int> array_sizes;
        while (isArray(ty->refType))
        {
            array_sizes.push_back(ty->size);
            ty = ty->refType;
        }
        //the size of the last dimension
        array_sizes.push_back(ty->size);

        auto base = translateType(ty->refType);
        for (auto size : array_sizes)
        {
            base = ArrayType::get(base, size);
        }
        //std::cout << base << std::endl;
        //printType(type);
        return base;
    }
    if (isIArray(type)) 
    {
        Type_h ty = type->refType;
        std::vector<int> array_sizes;
        ty = ty->refType;
        while (isArray(ty->refType))
        {
            array_sizes.push_back(ty->size);
            ty = ty->refType;
        }
        //the size of the last dimension
        array_sizes.push_back(ty->size);

        auto base = translateType(ty->refType);
        for (auto size : array_sizes)
        {
            base = ArrayType::get(base, size);
        }
        //std::cout << base << std::endl;
        //printType(type);
        return base;
    }
    if (isPointer(type))
    {
        Type_h ty = type->refType;
        auto ref = translateType(ty);
        return PointerType::getUnqual(ref);
    }
    return nullptr;
}


//the following variables specify when we need to exit a block and what command caused the exit
enum
{
    EXITING,
    BREAKING,
    RETURNING
} leave_code;

int time_to_leave = 0;

//if we're in a condition we convert \x01 and \0 chars to true and false respectively
void checkForBool(ast tr)
{
    if (equalType(tr->type, typeChar))
        if (strcmp(tr->id, "\0") == 0 || strcmp(tr->id, "\x01") == 0)
            tr->type = typeBoolean;
    return;
}

void ast_sem(ast t)
{
    if (t == NULL)
        return;
    switch (t->k)	{
    case FUNC_DEF:
    {
        //printf("entered func def\n");
        //openScope();
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        t->num_vars = currentScope->negOffset;
        ast_sem(t->branch3);

        closeScope();
        return;
    }
    case HEADER:
    {
        ;
        //printf("entered header\n");
        SymbolEntry *f = newFunction(t->id);
        openScope();

        //branch1: fpar_def , branch2: header_part i.e multiple fpar_defs
        //ast_sem(t->branch1);
        ast temp;
        Type_h parType;
        PassMode mode;
        if (t->branch1 != NULL)
        {
            temp = t->branch1; //temp now shows to fpar_def node. branch1 of that node is the beginning of the list of id nodes
            parType = temp->type;
            if (isPointer(temp->type) || isArray(temp->type) || isIArray(temp->type))
                mode = PASS_BY_REFERENCE;
            else
                mode = PASS_BY_VALUE;
            for (temp = temp->branch1; temp != NULL; temp = temp->branch1)
            {
                //we evaluate the semantics of each id node before we create the new entry. this is to avoid same name conflicts in the parameter definition
                ast_sem(temp);
                temp->type = parType;
                SymbolEntry *par = newParameter(temp->id, parType, mode, f);
            }
        }

        //ast_sem(t->branch2);

        //if t->branch2 is NULL the following loop exits immediatly so i don't need an if-check
        temp = t->branch2; //temp now points to the header_part node. branch1 is a fpar_node and branch2 is another header_part node
        ast headerpart;
        for (headerpart = t->branch2; headerpart != NULL; headerpart = headerpart->branch2)
        {
            //for each headerpart we evaluate a fpar_def node like before
            temp = headerpart->branch1; //in every iteration temp points to the fpar_def node of the header_part node
            parType = temp->type;
            if (isPointer(temp->type) || isArray(temp->type) || isIArray(temp->type))
                mode = PASS_BY_REFERENCE;
            else
                mode = PASS_BY_VALUE;
            ast idnode;
            for (idnode = temp->branch1; idnode != NULL; idnode = idnode->branch1)
            {
                ast_sem(idnode);
                idnode->type = parType;
                SymbolEntry *par = newParameter(idnode->id, parType, mode, f);
            }
        }

        endFunctionHeader(f, t->type);
        return;
    }
    case DECL:
    {
        //printf("entered decl %s\n", t->branch1->id);
        //DECL is the same as HEADER + we declare the function as forward 
        //branch1 points to header node
        //parameter declarations have their own scope
        //we assume that there are not any functions with the same name and different headers in the same scope

        //t_header points to the header of the function we wish to declare
        ast t_header = t->branch1;
        SymbolEntry *f = newFunction(t_header->id);
        forwardFunction(f);

        openScope();

        //branch1: fpar_def , branch2: header_part i.e multiple fpar_defs
        //ast_sem(t->branch1);
        ast temp;
        Type_h parType;
        PassMode mode;
        if (t_header->branch1 != NULL) {
            temp = t_header->branch1;    //temp now shows to fpar_def node. branch1 of that node is the beginning of the list of id nodes
            parType = temp->type;
            if (isPointer(temp->type) || isArray(temp->type) || isIArray(temp->type) ) mode = PASS_BY_REFERENCE;
            else mode = PASS_BY_VALUE;
            for(temp=temp->branch1; temp!=NULL; temp=temp->branch1) {
                //we evaluate the semantics of each id node before we create the new entry. this is to avoid same name conflicts in the parameter definition
                ast_sem(temp);
                temp->type = parType;
                SymbolEntry *par = newParameter(temp->id, parType, mode, f);
            }
        }

        //if t_header->branch2 is NULL the following loop exits immediatly so i don't need an if-check
        temp = t_header->branch2;    //temp now points to the header_part node. branch1 is a fpar_node and branch2 is another header_part node
        ast headerpart;
        for(headerpart = t_header->branch2; headerpart!=NULL; headerpart = headerpart->branch2) {
            //for each headerpart we evaluate a fpar_def node like before
            temp = headerpart->branch1;   //in every iteration temp points to the fpar_def node of the header_part node
            parType = temp->type;
            if (isPointer(temp->type) || isArray(temp->type) || isIArray(temp->type) ) mode = PASS_BY_REFERENCE;
            else mode = PASS_BY_VALUE;
            ast idnode;
            for(idnode=temp->branch1; idnode!=NULL; idnode=idnode->branch1) {
                ast_sem(idnode);
                idnode->type = parType;
                SymbolEntry *par = newParameter(idnode->id, parType, mode, f);
            }
        }

        endFunctionHeader(f, t_header->type);
        closeScope();

        return;
    }
    case VAR:
    {
        //var definitions
        ;
        ast temp;
        for (temp = t->branch1; temp != NULL; temp = temp->branch1)
        {
            ast_sem(temp);
            temp->type = t->type;
            SymbolEntry *v = newVariable(temp->id, t->type);
        }
        return;
    }
    case ID:
    {
        ;
        //printf("entered id -> %s\n", t->id);
        //check if there's already a variable with the same name in current scope
        char c = t->id[0];
        if (!isalpha(c))
        {
            strcat(t->id, "\\0");
            error("variable names have to start with a letter. variable is: %s\n", t->id);
        }
        SymbolEntry *e = lookupEntry(t->id, LOOKUP_CURRENT_SCOPE, false);
        if (e != NULL)
        {
            strcat(t->id, "\\0");
            error("there is already a variable with the name: $s\n", t->id);
        }
        return;
    }
    case SKIP:
        return;
    case ASSIGN:
    {
        //WARNING: still need to check the case where k = STRING_LIT.
        //assign a value to a variable
        ast_sem(t->branch1);
        if (isArray(t->branch1->type) || isIArray(t->branch1->type))
            error("cannot assign a value to an array variable");
        kind k = t->branch1->k;
        ast_sem(t->branch2);
        //check if types are the same
        if (!equalType(t->branch1->type, t->branch2->type))
        {
            error("type mismatch in assigning value to variable");
        }
        if (k == TID)
        {
            //we first check if a variable with that name exists
            SymbolEntry *v = lookupEntry(t->branch1->id, LOOKUP_ALL_SCOPES, true);
            if (v == NULL)
            {
                error("no variable with name %s\n", t->branch1->id);
            }
            if (!equalType(v->u.eVariable.type, t->branch1->type))
                error("type mismatch in assignment");
            t->nesting_diff = currentScope->nestingLevel - v->nestingLevel;
            t->offset = v->u.eVariable.offset;
        }
        else if (k == ARR)
        {
            //l_value is t[n]
            ast temp = t->branch1;
            while (temp->k != ID && temp->k != TID)
                temp = temp->branch1;
            SymbolEntry *v = lookupEntry(temp->id, LOOKUP_ALL_SCOPES, true);
            t->nesting_diff = currentScope->nestingLevel - v->nestingLevel;
            t->offset = v->u.eVariable.offset;
        }
        return;
    }
    case EXIT:
    {
        //we exit a block. it must not have a return type
        //we must make sure exit is inside a function with return type: void
        time_to_leave = 1;
        leave_code = EXITING;
        //closeScope();
        return;
    }
    case RET:
    {
        //printf("entered return\n");
        //we return an expr and leave the function
        //we must make sure return is inside a function with the same type as the return value
        ast_sem(t->branch1);
        //SymbolEntry *e;
        Scope *loop_scope;
        int found = 0;
        int foundAFunction = 0;
        for (loop_scope = currentScope; loop_scope != NULL; loop_scope = loop_scope->parent)
        {
            SymbolEntry *e;
            for (e = loop_scope->entries; e != NULL; e = e->nextInScope)
            {
                if (e->entryType == ENTRY_FUNCTION)
                {
                    foundAFunction = 1;
                    if (equalType(e->u.eFunction.resultType, t->branch1->type))
                    {
                        //we found the function
                        found = 1;
                        break;
                    }
                }
            }
            if (found)
                break;
        }
        //foundAFunction exists solely to print the correct error statement
        if (found == 0)
        {
            if (foundAFunction)
                error("function doesn't have the same type as the return value");
            else
                error("return command used but no function found");
        }
        time_to_leave = 1;
        leave_code = RETURNING;
        //printf("finished return\n");
        return;
    }
    case IF:
    {
        //printf("entered if\n");
        ast_sem(t->branch1);
        checkForBool(t->branch1);
        if (!equalType(t->branch1->type, typeBoolean))
            error("if expects a boolean condition");
        ast_sem(t->branch2);

        ast_sem(t->branch3);
        return;
    }
    case IF_ELSE:
    {
        //printf("entered if else\n");
        ast_sem(t->branch1);
        checkForBool(t->branch1);
        if (!equalType(t->branch1->type, typeBoolean))
            error("if expects a boolean condition");
        //openScope();
        ast_sem(t->branch2);
        //closeScope();
        ast_sem(t->branch3);
        //openScope();
        ast_sem(t->branch4);
        //closeScope();
        return;
    }
    case LOOP:
    {
        //we need to remember the loop's name if it has one
        if (strcmp(t->id, "\0") != 0)
        {
            Type_h ctype = typeArray(strlen(t->id) + 1, typeChar);
            SymbolEntry *e = newConstant(t->id, ctype, t->id);
        }

        ast_sem(t->branch1);
        return;
    }
    case BREAK:
    {
        if (strcmp(t->id, "\0") != 0)
        {
            //the break stops a specific loop
            SymbolEntry *l = lookupEntry(t->id, LOOKUP_ALL_SCOPES, true);
            if (l->entryType != ENTRY_CONSTANT)
                error("break given string that's not a loop name");
            if (l->nestingLevel > currentScope->nestingLevel)
                error("break isn't located inside the %s loop", t->id);

            t->nesting_diff = currentScope->nestingLevel - l->nestingLevel;
        }
        time_to_leave = 1;
        leave_code = BREAKING;
        return;
    }
    case CONT:
    {
        if (strcmp(t->id, "\0") != 0)
        {
            //continue begins the next iteration of a specific loop
            SymbolEntry *l = lookupEntry(t->id, LOOKUP_ALL_SCOPES, true);
            if (l->entryType != ENTRY_CONSTANT)
                error("break given string that's not a loop name");
            if (currentScope->nestingLevel < l->nestingLevel)
                error("continue isn't located inside the %s loop", t->id);

            t->nesting_diff = currentScope->nestingLevel - l->nestingLevel;
        }
        return;
    }
    case SEQ:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        return;
    }
    case BLOCK:
    {
        //we begin a new block.
        t->num_vars = currentScope->negOffset;
        //printf("block begins\n");
        ast_sem(t->branch1);
        //printf("block ends\n");
        return;
    }
    case PROC_CALL: {
		;
		//calling a previously defined function (with no return value)
		//branch1-> expr , branch2-> expr_part (more expressions)
		//we check if an entry with the given name exists, if it's a function with void return type
		//printf("calling procedure %s\n", t->id);
		SymbolEntry *f = lookupEntry(t->id, LOOKUP_ALL_SCOPES, true);
		if (f->entryType != ENTRY_FUNCTION) error("name given is not a function");
		if (!equalType(f->u.eFunction.resultType, typeVoid)) error("type mismatch, called function is not void");
		t->type = typeVoid;
		ast_sem(t->branch1);
		if (t->branch1 == NULL) {
            if (f->u.eFunction.firstArgument != NULL) error("function has parameters and none were given");
            else {
                //function has no parameters and we called it using none, so everything's wrapped up real nice
                return;
            }
		}
		if (t->branch1 != NULL && f->u.eFunction.firstArgument == NULL) error("function has no parameters, however some were given");
		
		SymbolEntry *params = f->u.eFunction.firstArgument;
		if (!equalType(t->branch1->type, params->u.eParameter.type)) {
            //if the type of the typical parameter is an IArray then it can be matched with an Arraytype of any length
            //if (isIArray(t->branch1->type) && isArray(params->u.eParameter.type)) ;
            if (isArray(t->branch1->type) && isIArray(params->u.eParameter.type)) {
                //we need to check if the referenced types of the arrays match
                if (!equalType(t->branch1->type->refType, params->u.eParameter.type->refType))
                error("parameter type mismatch");
            }
            else error("parameter type mismatch");
		}
		if ((params->u.eParameter.mode == PASS_BY_REFERENCE) && (t->branch1->k != TID) 
			&& (t->branch1->k != ARR) && (t->branch1->k != STRING_LIT))
		error("parameter passing mode mismatch");

		ast temp = t->branch2;
		params = params->u.eParameter.next;
		//we check each real parameter to see if they match with the function's typical parameters
		while(temp != NULL && params != NULL) {
			ast_sem(temp->branch1);

			if (!equalType(temp->branch1->type, params->u.eParameter.type)) {
				//if one of the types is an IArray then it can be matched with an Arraytype of any length
				if (isArray(temp->branch1->type) && isIArray(params->u.eParameter.type)) {
				//we need to check if the referenced types of the arrays match
				if (!equalType(temp->branch1->type->refType, params->u.eParameter.type->refType))
					error("parameter type mismatch");
				}
				else error("parameter type mismatch");
			}
			if ((params->u.eParameter.mode == PASS_BY_REFERENCE) && (temp->branch1->k != TID) 
				&& (temp->branch1->k != ARR ) && (temp->branch1->k != STRING_LIT))
				error("parameter passing mode mismatch");
				params = params->u.eParameter.next;
				temp = temp->branch2;
		}

		if (temp!=NULL && params==NULL) error("proc call was given too many parameters");
		if (temp==NULL && params!=NULL) error("proc call was given too few parameters");

        t->nesting_diff = currentScope->nestingLevel - f->nestingLevel;
		//printf("leaving procedure %s\n", t->id);
		return;
    }
	case FUNC_CALL: {
		;
		//printf("calling function %s\n", t->id);
		//calling a previously defined function (with return value)
		//branch1-> expr , branch2-> expr_part (more expressions)
		//we check if an entry with the given name exists, if it's a function with non-void return type
		SymbolEntry *f = lookupEntry(t->id, LOOKUP_ALL_SCOPES, true);
		if (f->entryType != ENTRY_FUNCTION) error("name given is not a function");
		if (equalType(f->u.eFunction.resultType, typeVoid)) error("type mismatch, called function is void");

		//we make the type of ast node the same as the function's. this is needed in order to use the called function in math calculations
		t->type = f->u.eFunction.resultType;
		ast_sem(t->branch1);
		if (t->branch1 == NULL) {
			if (f->u.eFunction.firstArgument != NULL) error("function has parameters and none were given");
			else {
			    //function has no parameters and we called it using none, so everything's wrapped up real nice
			    return;
			}
		}
		if (t->branch1 != NULL && f->u.eFunction.firstArgument == NULL) error("function has no parameters, however some were given");

		SymbolEntry *params = f->u.eFunction.firstArgument;
		if (!equalType(t->branch1->type, params->u.eParameter.type)) {
			//if one of the types is an IArray then it can be matched with an Arraytype of any length
			if (isArray(t->branch1->type) && isIArray(params->u.eParameter.type)) {
                //we need to check if the referenced types of the arrays match
                if (!equalType(t->branch1->type->refType, params->u.eParameter.type->refType))
                    error("parameter type mismatch");
			}
			else error("parameter type mismatch");
		}
		if ((params->u.eParameter.mode == PASS_BY_REFERENCE) && (t->branch1->k != TID) 
			&& (t->branch1->k != ARR) && (t->branch1->k != STRING_LIT))
			error("parameter passing mode mismatch");

		ast temp = t->branch2;
		params = params->u.eParameter.next;
		//we check each real parameter to see if they match with the function's typical parameters
		while(temp!=NULL && params!=NULL) {
			ast_sem(temp->branch1);
			if (!equalType(temp->branch1->type, params->u.eParameter.type)) {
			//if one of the types is an IArray then it can be matched with an Arraytype of any length
			if (isArray(temp->branch1->type) && isIArray(params->u.eParameter.type)) {
				//we need to check if the referenced types of the arrays match
				if (!equalType(temp->branch1->type->refType, params->u.eParameter.type->refType))
				error("parameter type mismatch");
			}
			else error("parameter type mismatch");
			}
			if ((params->u.eParameter.mode == PASS_BY_REFERENCE) && (temp->branch1->k != TID) 
				&& (temp->branch1->k != ARR ) && (temp->branch1->k != STRING_LIT))
			error("parameter passing mode mismatch");
			params = params->u.eParameter.next;
			temp = temp->branch2;
		}

		if (temp!=NULL && params==NULL) error("func call was given too many parameters");
		if (temp==NULL && params!=NULL) error("func call was given too few parameters");

        t->nesting_diff = currentScope->nestingLevel - f->nestingLevel;
		//printf("finished function call\n");
		return;
	}
    case TID:
    {
        ;
        //printf("accesing variable %s\n", t->id);
        char c1 = t->id[0];
        if (!isalpha(c1))
        {
            error("variable names have to start with a letter");
        }
        SymbolEntry *e = lookupEntry(t->id, LOOKUP_ALL_SCOPES, true);
        if (e->entryType == ENTRY_PARAMETER)
        {
            t->type = e->u.eParameter.type;
            t->nesting_diff = currentScope->nestingLevel - e->nestingLevel;
            t->offset = e->u.eParameter.offset;
            //printf("parameter %s nesting diff is %d\n", t->id, t->nesting_diff);
            //printf("parameter %s offset is: %d\n", t->id, t->offset);
        }
        else if (e->entryType == ENTRY_VARIABLE)
        {
            t->type = e->u.eVariable.type;
            t->nesting_diff = currentScope->nestingLevel - e->nestingLevel;
            t->offset = e->u.eVariable.offset;
            //printf("variable %s nesting diff is %d\n", t->id, t->nesting_diff);
            //printf("variable %s offset is: %d\n", t->id, t->offset);
        }
        return;
    }
    case STRING_LIT:
    {
        ;
        //string constant
        int len = strlen(t->id);
        // char *strconst;
        // strconst = (char *) malloc(sizeof(char)*(len + 1);
        // strcpy(strconst, t->id);
        t->type = typeArray(len + 1, typeChar);
        return;
    }
    case ARR:
    {
        //branch1-> l_value, branch2-> expr
        //case is an access to a[i] (element i of array a)
        //we check if 'a' is an array, if it exists, if i is int and if 0 <= i < N , N being the size of the array
        ast_sem(t->branch1);
        ast temp = t->branch1;
        while ((temp->k != TID) && (temp->k != STRING_LIT))
            temp = temp->branch1;
        SymbolEntry *e2 = lookupEntry(temp->id, LOOKUP_ALL_SCOPES, true);
        if (!isArray(t->branch1->type) && !isIArray(t->branch1->type))
            error("l_value is not an array");
        ast_sem(t->branch2);
        if (!equalType(t->branch2->type, typeInteger))
            error("tried to access an array with index not being integer");
        if (t->branch2->num < 0)
            error("index below 0");
        if (isArray(t->branch1->type))
        {
            if (t->branch2->num >= t->branch1->type->size)
                error("index exceeds array size");
        }

        t->type = t->branch1->type->refType;
        return;
    }
    case INTCONST:
    {
        //printf("number %d\n", t->num);
        t->type = typeInteger;
        return;
    }
    case CHARCONST:
    {
        //characters \x01 and \0 represent the true and false keywords. but we don't know if they will be used as bool or char
        //solution: we consider them chars for now unless we notice that they are used in a condition

        // if (strcmp(t->id, "\0")==0) t->type = typeBoolean;
        // else if(strcmp(t->id, "\x01")==0) t->type = typeBoolean;
        // else t->type = typeChar;
        t->type = typeChar;
        return;
    }
    case PLUS:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeInteger;
            else
                error("type mismatch in + operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeChar;
            else
                error("type mismatch in + operator");
        }
        return;
    }
    case MINUS:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeInteger;
            else
                error("type mismatch in - operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeChar;
            else
                error("type mismatch in - operator");
        }
        return;
    }
    case TIMES:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeInteger;
            else
                error("type mismatch in * operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeChar;
            else
                error("type mismatch in * operator");
        }
        else
            error("unknown type for * operation");
        return;
    }
    case DIV:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeInteger;
            else
                error("type mismatch in / operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeChar;
            else
                error("type mismatch in / operator");
        }
        return;
    }
    case MOD:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeInteger;
            else
                error("type mismatch in mod operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeChar;
            else
                error("type mismatch in mod operator");
        }
        return;
    }
    case NOT:
    {
        ast_sem(t->branch2);
        checkForBool(t->branch2);
        if (!equalType(t->branch2->type, typeBoolean))
            error("type mismatch in not operation");
        t->type = typeBoolean;
        return;
    }
    case AND:
    {
        ast_sem(t->branch1);
        checkForBool(t->branch1);
        ast_sem(t->branch2);
        checkForBool(t->branch2);
        if ((!equalType(t->branch1->type, typeBoolean)) || (!equalType(t->branch2->type, typeBoolean)))
            error("type mismatch in and operator");
        t->type = typeBoolean;
        return;
    }
    case OR:
    {
        ast_sem(t->branch1);
        checkForBool(t->branch1);
        ast_sem(t->branch2);
        checkForBool(t->branch2);
        if ((!equalType(t->branch1->type, typeBoolean)) || (!equalType(t->branch2->type, typeBoolean)))
            error("type mismatch in or operator");
        t->type = typeBoolean;
        return;
    }
    case EQ:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeBoolean;
            else
                error("type mismatch in = operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeBoolean;
            else
                error("type mismatch in = operator");
        }
        return;
    }
    case LT:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeBoolean;
            else
                error("type mismatch in < operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeBoolean;
            else
                error("type mismatch in < operator");
        }
        return;
    }
    case GT:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeBoolean;
            else
                error("type mismatch in > operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeBoolean;
            else
                error("type mismatch in > operator");
        }
        return;
    }
    case LE:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeBoolean;
            else
                error("type mismatch in <= operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeBoolean;
            else
                error("type mismatch in <= operator");
        }
        return;
    }
    case GE:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeBoolean;
            else
                error("type mismatch in >= operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeBoolean;
            else
                error("type mismatch in >= operator");
        }
        return;
    }
    case NEQ:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if (equalType(t->branch1->type, typeInteger))
        {
            if (equalType(t->branch2->type, typeInteger))
                t->type = typeBoolean;
            else
                error("type mismatch in <> operator");
        }
        else if (equalType(t->branch1->type, typeChar))
        {
            if (equalType(t->branch2->type, typeChar))
                t->type = typeBoolean;
            else
                error("type mismatch in <> operator");
        }
        return;
    }
    case EXPR_NOT:
    {
        //not operation for expressions
        ast_sem(t->branch1);
        if (!equalType(t->branch1->type, typeChar))
            error("type mismatch in ! operation");
        t->type = typeChar;
        return;
    }
    case EXPR_AND:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if ((!equalType(t->branch1->type, typeChar)) || (!equalType(t->branch2->type, typeChar)))
            error("type mismatch in & operation");
        t->type = typeChar;
        return;
    }
    case EXPR_OR:
    {
        ast_sem(t->branch1);
        ast_sem(t->branch2);
        if ((!equalType(t->branch1->type, typeChar)) || (!equalType(t->branch2->type, typeChar)))
            error("type mismatch in | operation");
        t->type = typeChar;
        return;
    }
    }
}

void set_lib_functions() {
  //initialize the following dana functions in the symbol table

  ast fdecl, fpardef;
  //decl writeInteger: n as int
  fpardef = ast_fpar_def(ast_id("n", NULL), typeInteger);
  fdecl = ast_header("writeInteger", typeVoid, fpardef, NULL);
  ast_sem(ast_decl(fdecl));
  
  //decl writeByte: b as byte
  fpardef = ast_fpar_def(ast_id("b", NULL), typeChar);
  fdecl = ast_header("writeByte", typeVoid, fpardef, NULL);
  ast_sem(ast_decl(fdecl));

  //decl writeChar: b as byte
  fpardef = ast_fpar_def(ast_id("b", NULL), typeChar);
  fdecl = ast_header("writeChar", typeVoid, fpardef, NULL);
  ast_sem(ast_decl(fdecl));

  //decl writeString: s as byte []
  fpardef = ast_fpar_def(ast_id("s", NULL), typeIArray(typeChar));
  fdecl = ast_header("writeString", typeVoid, fpardef, NULL);
  ast_sem(ast_decl(fdecl));

  //decl readInteger is int
  fdecl = ast_header("readInteger", typeInteger, NULL, NULL);
  ast_sem(ast_decl(fdecl));

  //decl readByte is byte
  fdecl = ast_header("readByte", typeChar, NULL, NULL);
  ast_sem(ast_decl(fdecl));

  //decl readChar is byte
  fdecl = ast_header("readChar", typeChar, NULL, NULL);
  ast_sem(ast_decl(fdecl));

  //decl readString: n as int, s as byte []
  fpardef = ast_fpar_def(ast_id("s", NULL), typeIArray(typeChar));
  ast fpardef2 = ast_header_part(fpardef, NULL);
  fpardef = ast_fpar_def(ast_id("n", NULL), typeInteger);
  fdecl = ast_header("readString", typeVoid, fpardef, fpardef2);
  ast_sem(ast_decl(fdecl));

  //decl extend is int: b as byte
  fpardef = ast_fpar_def(ast_id("b", NULL), typeChar);
  fdecl = ast_header("extend", typeInteger, fpardef, NULL);
  ast_sem(ast_decl(fdecl));

  //decl shrink is byte: n as int
  fpardef = ast_fpar_def(ast_id("n", NULL), typeInteger);
  fdecl = ast_header("shrink", typeChar, fpardef, NULL);
  ast_sem(ast_decl(fdecl));

  //decl strlen is int: s as byte []
  fpardef = ast_fpar_def(ast_id("s", NULL), typeIArray(typeChar));
  fdecl = ast_header("strlen", typeInteger, fpardef, NULL);
  ast_sem(ast_decl(fdecl));

  //decl strcmp is int: s1 s2 as byte []
  fpardef = ast_fpar_def(ast_id("s1", ast_id("s2", NULL)), typeIArray(typeChar));
  fdecl = ast_header("strcmp", typeInteger, fpardef, NULL);
  ast_sem(ast_decl(fdecl));

  //decl strcpy: trg src as byte []
  fpardef = ast_fpar_def(ast_id("trg", ast_id("src", NULL)), typeIArray(typeChar));
  fdecl = ast_header("strcpy", typeVoid, fpardef, NULL);
  ast_sem(ast_decl(fdecl));

  //decl strcat: trg src as byte []
  fpardef = ast_fpar_def(ast_id("trg", ast_id("src", NULL)), typeIArray(typeChar));
  fdecl = ast_header("strcat", typeVoid, fpardef, NULL);
  ast_sem(ast_decl(fdecl));
}

//---------------------end of sem analysis----------------------------

int isLibFunction(char *name) {
    if (strcmp(name,"writeInteger") == 0) return 1;
    if (strcmp(name,"writeByte") == 0) return 1;
    if (strcmp(name,"writeChar") == 0) return 1;
    if (strcmp(name,"writeString") == 0) return 1;

    if (strcmp(name,"readInteger") == 0) return 1;
    if (strcmp(name,"readByte") == 0) return 1;
    if (strcmp(name,"readChar") == 0) return 1;
    if (strcmp(name,"readString") == 0) return 1;

    if (strcmp(name,"extend") == 0) return 1;
    if (strcmp(name,"shrink") == 0) return 1;

    if (strcmp(name,"strlen") == 0) return 1;
    if (strcmp(name,"strcmp") == 0) return 1;
    if (strcmp(name,"strcpy") == 0) return 1;
    if (strcmp(name,"strcat") == 0) return 1;

    return 0;
}

Value *compile_function(ast f)
{
    //case of FUNC_DEF
    if (f == NULL) return nullptr;

    StructType *old = current_AR;
    BasicBlock *OldRetBlock;
    StructType *new_frame = TheModule->getTypeByName(f->branch1->id);
    if (new_frame == NULL) {
        char *structname = (char *) malloc(sizeof(char)*(strlen(f->branch1->id) + 8) );
        strcpy(structname, "struct_");
        strcat(structname,f->branch1->id);
        new_frame = StructType::create(TheContext, structname );
    }
    std::vector<Type *> parameters;
    std::vector<Type *> frame_fields;

    if(!isLibFunction(f->branch1->id)) {
        parameters.push_back(PointerType::get(current_AR, 0));
    }
    frame_fields.push_back(PointerType::get(current_AR, 0));
    current_AR = new_frame;

    //iterate through the parameters and push the types in the vector
    ast params= f->branch1->branch1;
    Type *par_type;
    if (params) {
        par_type = translateType(params->type);
        for(ast temp=params->branch1; temp!=NULL; temp=temp->branch1) {
            parameters.push_back(par_type);
            frame_fields.push_back(par_type);
        }
    }
    for(ast defs = f->branch1->branch2; defs!=NULL; defs=defs->branch2) {
        params = defs->branch1;
        par_type = translateType(params->type);
        for(ast temp = params->branch1; temp!=NULL; temp=temp->branch1) {
            parameters.push_back(par_type);
            frame_fields.push_back(par_type);
        }
    }

    //local variable definitions
    for(ast local_defs = f->branch2; local_defs != NULL; local_defs=local_defs->branch2) {
        if (local_defs->branch1->k == VAR) {
            //we found a var def
            ast var_def = local_defs->branch1;
            Type *var_type = translateType(var_def->type);
            for(ast temp = var_def->branch1; temp != NULL; temp = temp->branch1) {
                frame_fields.push_back(var_type);
                // printf("variable added to frame\n");
            }
        }
    }

    if (new_frame->isOpaque()) {
        //this if is used only in the case of forwarding functions
        new_frame->setBody(frame_fields, false);
    }
    //printf("point 2\n");
    //if the function doesn't already exist we create it
    FunctionType *ftype = FunctionType::get(translateType(f->branch1->type), parameters, false);
    Function *NewFunction = TheModule->getFunction(f->branch1->id);
    if (NewFunction == NULL) {
        NewFunction = Function::Create(ftype, GlobalValue::ExternalLinkage, f->branch1->id, TheModule.get());
    }
    //printf("point 3\n");
    //we change the names of the parameters in the Function to their original ones
    Function::arg_iterator iter = NewFunction->arg_begin();
    iter->setName("previous");
    iter++;

    params= f->branch1->branch1;
    if (params) {
        for(ast temp=params->branch1; temp!=NULL; temp=temp->branch1, iter++) {
            iter->setName(temp->id);
        }
        for(ast defs = f->branch1->branch2; defs!=NULL; defs=defs->branch2) {
            params = defs->branch1;
            for(ast temp = params->branch1; temp!=NULL; temp=temp->branch1, iter++) {
                iter->setName(temp->id);
            }
        }
    }

    //printf("point 4\n");
    //we execute the local defs and decls
    ast_compile(f->branch2);
    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", NewFunction);
    Builder.SetInsertPoint(BB);

    AllocaInst *oldAlloca = currentAlloca;
    currentAlloca = Builder.CreateAlloca(new_frame, 0, "new_frame");
    if (!equalType(f->branch1->type, typeVoid)) {
        //we allocate memory for the return value
        retAlloca = Builder.CreateAlloca(translateType(f->branch1->type), 0, "");
        OldRetBlock = returnBlock;
        returnBlock = BasicBlock::Create(TheContext, "return", NewFunction);
    }
    //store Function parameters in the current frame
    iter = NewFunction->arg_begin();
    //std::vector<Type *> idxlist;
    Value *gep = Builder.CreateGEP(currentAlloca, std::vector<Value *>{c32(0), c32(0)}, "");
    Value *element = & (*iter);
    Builder.CreateStore(element, gep, false);
    iter++;
    for(int i=1; i < parameters.size(); i++, iter++) {
        gep = Builder.CreateGEP(currentAlloca, std::vector<Value *>{c32(0), c32(i)}, "");
        element = & (*iter);
        Builder.CreateStore(element, gep, false);
    }
    //we start executing the block of the function
    ast_compile(f->branch3);

    //if we encountered no return instruction, the function has void return type
    if (equalType(f->branch1->type, typeVoid)) {
        BasicBlock *RetBlock = BasicBlock::Create(TheContext, "return", NewFunction);
        Builder.CreateBr(RetBlock);
        Builder.SetInsertPoint(RetBlock);
        Builder.CreateRetVoid();
    }
    else {
        // Function *endFunction = Builder.GetInsertBlock()->getParent();
        Builder.CreateBr(returnBlock);
        Builder.SetInsertPoint(returnBlock);

        Value *returnValue = Builder.CreateLoad(retAlloca, "");
        Builder.CreateRet(returnValue);
        returnBlock = OldRetBlock;
    }

    //function finished. we return to the previous AR
    current_AR = old;
    currentAlloca = oldAlloca;
    // returnBlock = OldRetBlock;
    return nullptr;
}

Value *ast_compile(ast t)
{
    if (t == nullptr) return nullptr;
    switch (t->k) {
    case FUNC_DEF:
    {
        compile_function(t);
        //execute function header actions, create a new activation record
        //ast_compile(t->branch1);

        //execute local def instructions
        //ast_compile(t->branch2);

        //execute function block
        //ast_compile(t->branch3);
        return nullptr;
    }
    case HEADER:
    {
        //CASE ALREADY COVERED
        return nullptr;
    }
    case DECL:
    {
        StructType *new_frame = TheModule->getTypeByName(t->branch1->id);
        if (new_frame == NULL) {
            char *structname = (char *) malloc(sizeof(char)*(strlen(t->branch1->id) + 8) );
            strcpy(structname, "struct_");
            strcat(structname,t->branch1->id);
            new_frame = StructType::create(TheContext, structname );
        }
        std::vector<Type *> parameters;
        std::vector<Type *> frame_fields;

        if(!isLibFunction(t->branch1->id)) {
            parameters.push_back(PointerType::get(current_AR, 0));
        }
        frame_fields.push_back(PointerType::get(current_AR, 0));

        //parameter definition
        ast params= t->branch1->branch1;
        Type *par_type;
        if (params) {
            par_type = translateType(params->type);
            for(ast temp=params->branch1; temp!=NULL; temp=temp->branch1) {
                parameters.push_back(par_type);
                frame_fields.push_back(par_type);
            }
        }
        for(ast defs = t->branch1->branch2; defs!=NULL; defs=defs->branch2) {
            params = defs->branch1;
            par_type = translateType(params->type);
            for(ast temp = params->branch1; temp!=NULL; temp=temp->branch1) {
                parameters.push_back(par_type);
                frame_fields.push_back(par_type);
            }
        }

        //if the function doesn't already exist we create it
        FunctionType *ftype = FunctionType::get(translateType(t->branch1->type), parameters, false);
        Function *NewFunction = TheModule->getFunction(t->branch1->id);
        if (NewFunction == NULL) {
            NewFunction = Function::Create(ftype, GlobalValue::ExternalLinkage, t->branch1->id, TheModule.get());
        }
        return nullptr;
    }
    case VAR:
    {
        //CASE ALREADY COVERED
        return nullptr;
    }
    case ID:
    {
        return nullptr;
    }
    case SKIP:
    {
        return nullptr;
    }
    case ASSIGN:
    {
        Value *val = ast_compile(t->branch2);
        int indx = t->branch1->offset;
        //printf("%s var ofset is %d\n", t->branch1->id, t->branch1->offset);
        // if (t->branch1->k) {
        //     index = t->branch1->offset;
        // }
        int frame_diff = t->branch1->nesting_diff;
        Value *record = currentAlloca;
        Value *gep;
        for(int i=0; i < frame_diff; i++) {
            gep = Builder.CreateGEP(record, std::vector<Value *>{c32(0), c32(0)}, "");
            record = Builder.CreateLoad(gep, "previous");
        }

        gep =  Builder.CreateGEP(record, std::vector<Value *>{c32(0), c32(indx)}, "");
        Builder.CreateStore(val, gep, false);
        //printf("finished assign\n");
        return nullptr;
    }
    case EXIT:
    {
        return nullptr;
    }
    case RET:
    {
        Value *retval = ast_compile(t->branch1);
        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        BasicBlock *AfterBB = BasicBlock::Create(TheContext, "after_return", TheFunction);

        //Value *gep = Builder.CreateGEP(retAlloca, 0, "");
        Builder.CreateStore(retval, retAlloca, false);
        Builder.CreateBr(returnBlock);
        
        Builder.SetInsertPoint(AfterBB);
        return nullptr;
    }
    case IF:
    {
        Value *cond;
        Value *v = ast_compile(t->branch1);

        //if Value v is a condition we don't need to do one more comparison. if it's a numerical value the comparison is needed
        if (equalType(t->branch1->type, typeBoolean)) {
            cond = v;
        }
        else {
            cond = Builder.CreateICmpNE(v, c32(0), "if_cond");
        }

        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        BasicBlock *InsideBB = BasicBlock::Create(TheContext, "then", TheFunction);
        BasicBlock *NextBB;
        if (t->branch3 != NULL) NextBB = BasicBlock::Create(TheContext, "elif", TheFunction);
        else NextBB = BasicBlock::Create(TheContext, "endif", TheFunction);
        Builder.CreateCondBr(cond, InsideBB, NextBB);
        Builder.SetInsertPoint(InsideBB);
        //we execute the block inside if
        ast_compile(t->branch2);

        Builder.CreateBr(NextBB);
        Builder.SetInsertPoint(NextBB);
        //we execute all elif nodes
        //ast_compile(t->branch3);

        for(ast elif_node=t->branch3; elif_node!=NULL; elif_node=elif_node->branch3) {
            Value *val = ast_compile(elif_node->branch1);
            Value *cond1 = Builder.CreateICmpNE(val, c32(0), "elif_cond");
            Function *TheFunction = Builder.GetInsertBlock()->getParent();
            BasicBlock *InElifBB = BasicBlock::Create(TheContext, "in_elif", TheFunction);
            BasicBlock *NextElifBB;
            if (elif_node->branch3 != NULL) NextElifBB = BasicBlock::Create(TheContext, "next_elif", TheFunction);
            else                            NextElifBB = BasicBlock::Create(TheContext, "endif", TheFunction);
            Builder.CreateCondBr(cond1, InElifBB, NextElifBB);
            Builder.SetInsertPoint(InElifBB);
            ast_compile(elif_node->branch2);

            Builder.CreateBr(NextElifBB);
            Builder.SetInsertPoint(NextElifBB);
        }

        return nullptr;
    }
    case IF_ELSE:
    {
        Value *cond;
        Value *v = ast_compile(t->branch1);

        //if Value v is a condition we don't need to do one more comparison. if it's a numerical value the comparison is needed
        if (equalType(t->branch1->type, typeBoolean)) {
            cond = v;
        }
        else {
            cond = Builder.CreateICmpNE(v, c32(0), "if_cond");
        }
        
        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        BasicBlock *InsideBB = BasicBlock::Create(TheContext, "then", TheFunction);
        BasicBlock *NextBB;
        if (t->branch3 != NULL) NextBB = BasicBlock::Create(TheContext, "elif", TheFunction);
        else NextBB = BasicBlock::Create(TheContext, "else", TheFunction);

        Builder.CreateCondBr(cond, InsideBB, NextBB);
        Builder.SetInsertPoint(InsideBB);
        //we execute the block inside 'if'
        ast_compile(t->branch2);

        //BasicBlock *BB = Builder.GetInsertBlock();
        
        Builder.CreateBr(NextBB);
        Builder.SetInsertPoint(NextBB);
        //we execute all elif nodes
        //ast_compile(t->branch3);
        ast elif_node;
        for(elif_node=t->branch3; elif_node!=NULL; elif_node=elif_node->branch3) {
            Value *val = ast_compile(elif_node->branch1);
            Value *cond1 = Builder.CreateICmpNE(val, c32(0), "elif_cond");
            Function *TheFunction1 = Builder.GetInsertBlock()->getParent();
            BasicBlock *InElifBB = BasicBlock::Create(TheContext, "in_elif", TheFunction1);
            BasicBlock *NextElifBB;
            if (elif_node->branch3 != NULL) NextElifBB = BasicBlock::Create(TheContext, "next_elif", TheFunction1);
            else                            NextElifBB = BasicBlock::Create(TheContext, "else", TheFunction1);
            Builder.CreateCondBr(cond1, InElifBB, NextElifBB);
            Builder.SetInsertPoint(InElifBB);
            ast_compile(elif_node->branch2);

            Builder.CreateBr(NextElifBB);
            Builder.SetInsertPoint(NextElifBB);
        }

        //we execute the 'else' block
        ast_compile(t->branch4);
        TheFunction = Builder.GetInsertBlock()->getParent();
        BasicBlock *AfterBB = BasicBlock::Create(TheContext, "endif", TheFunction);

        Builder.CreateBr(AfterBB);
        Builder.SetInsertPoint(AfterBB);
        return nullptr;
    }
    case LOOP:
    {
        // Make the new basic block for the loop.
        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        BasicBlock *PreheaderBB = Builder.GetInsertBlock();
        BasicBlock *LoopBB = BasicBlock::Create(TheContext, "loop", TheFunction);
        BasicBlock *AfterBB = BasicBlock::Create(TheContext, "after_loop", TheFunction);

        //if the loop has a name we store it
        char *name;
        if (!strcmp(t->id, "\0")) {
            name = (char *) malloc((strlen(t->id)+1)*sizeof(char));
            loopMap.insert(std::pair<char *, BasicBlock*>(name, AfterBB));
        }
        else {
            name = NULL;
        }

        // Insert an explicit fall-through from the current block.
        Builder.CreateBr(LoopBB);
        // Start insertion in the loop.
        Builder.SetInsertPoint(LoopBB);
        //we execute the body of the loop
        ast_compile(t->branch1);
        //loop ends only when instructions break, exit are encountered inside

        Builder.CreateBr(LoopBB);
        //end of loop
        Builder.SetInsertPoint(AfterBB);
        return nullptr;
    }
    case BREAK:
    {
        char *name;
        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        BasicBlock *AfterBlock;
        BasicBlock *BreakBlock = BasicBlock::Create(TheContext, "break", TheFunction);

        if (!strcmp(t->id, "\0")) {
            //if break is given a loop name we break out of that specific loop.
            name = (char *) malloc((strlen(t->id)+1)*sizeof(char));
            AfterBlock = (loopMap.find(name))->second;
            Builder.CreateBr(AfterBlock);
            Builder.SetInsertPoint(BreakBlock);
        }
        else {
            //otherwise we break the closest loop
            name = NULL;
            Builder.CreateBr(BreakBlock);
            Builder.SetInsertPoint(BreakBlock);
        }
        return nullptr;
    }
    case CONT:
    {
        char *name;
        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        BasicBlock *ContBlock = BasicBlock::Create(TheContext, "cont", TheFunction);
        if (!strcmp(t->id,"\0")) {
            //continue is given a specific name
            name = (char *) malloc((strlen(t->id)+1)*sizeof(char));
        }
        else {
            name = NULL;
        }
        return nullptr;
    }
    case SEQ:
    {
        ast_compile(t->branch1);
        ast_compile(t->branch2);
        return nullptr;
    }
    case BLOCK:
    {
        //UNFINISHED
        ast_compile(t->branch1);
        return nullptr;
    }
    case PROC_CALL:
    {
        //Function *TheFunction = Builder.GetInsertBlock()->getParent();
        Function *Callee = TheModule->getFunction(t->id);
        std::vector<Value *> args;
        Value *record = currentAlloca;

        int isLib = isLibFunction(t->id);
        
        if (!isLib) {
            for(int i=0; i < t->nesting_diff; i++) {
                Value *gep = Builder.CreateGEP(record, std::vector<Value *>{c32(0), c32(0)}, "");
                record = Builder.CreateLoad(gep, "previous");
            }
            args.push_back(record);
        }

        if (t->branch1) {
            args.push_back(ast_compile(t->branch1));
            for(ast temp=t->branch2; temp != NULL; temp=temp->branch2) {
                Value *v = ast_compile(temp->branch1);
                args.push_back(v);
            }
        }
        if (!isLib) Builder.CreateCall(Callee, args, t->id);
        else Builder.CreateCall(Callee, args, "");
        return nullptr;
    }
    case FUNC_CALL:
    {
        Function *Callee = TheModule->getFunction(t->id);
        std::vector<Value *> args;
        Value *record = currentAlloca;

        int isLib = isLibFunction(t->id);

        if (!isLib) {
            for(int i=0; i < t->nesting_diff; i++) {
                Value *gep = Builder.CreateGEP(record, std::vector<Value *>{c32(0), c32(0)}, "");
                record = Builder.CreateLoad(gep, "previous");
            }
            args.push_back(record);
        }
        //if func call has arguments we push them in the args vector
        if (t->branch1) {
            args.push_back(ast_compile(t->branch1));
            for(ast temp=t->branch2; temp != NULL; temp=temp->branch2) {
                Value *v = ast_compile(temp->branch1);
                args.push_back(v);
            }
        }
        if (!isLib) return Builder.CreateCall(Callee, args, t->id);
        else return Builder.CreateCall(Callee, args, "");
    }
    case TID:
    {
        //we search the AR where the variable/param was defined and load it's value
        //printf("accessing variable %s\n", t->id);
        int frame_diff = t->nesting_diff;
        Value *record = currentAlloca;
        Value *gep;
        for(int i=0; i < frame_diff; i++) {
            gep = Builder.CreateGEP(record, std::vector<Value *>{c32(0), c32(0)}, "");
            record = Builder.CreateLoad(gep, "previous");
        }
        int index = t->offset;
        gep = Builder.CreateGEP(record, std::vector<Value *>{c32(0), c32(index)}, "");
        // printf("finished variable %s\n", t->id);
        return Builder.CreateLoad(gep, t->id);
    }
    case ARR:
    {
        return nullptr;
    }
    case STRING_LIT:
    {
        return nullptr;
    }
    case INTCONST:
    {
        return c32(t->num);
    }
    case CHARCONST:
    {
        char c = (t->id)[0];
        return c8(c);
    }
    case PLUS:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateAdd(l, r, "addtmp");
    }
    case MINUS:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateSub(l, r, "subtmp");
    }
    case TIMES:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateMul(l, r, "multmp");
    }
    case DIV:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateSDiv(l, r, "divtmp");
    }
    case MOD:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateSRem(l, r, "modtmp");
    }
    case NOT:
    {
        //Boolean not, and, or cases (!, &&, ||)
        Value *l = ast_compile(t->branch1);
        return Builder.CreateICmpEQ(l, c32(0), "cond_nottmp");
    }
    case AND:
    {
        return nullptr;
    }
    case OR:
    {
        return nullptr;
    }
    case EQ:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateICmpEQ(l, r, "equaltmp");
    }
    case LT:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateICmpSLT(l, r, "lowertmp");
    }
    case GT:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateICmpSGT(l, r, "greatertmp");
    }
    case LE:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateICmpSLE(l, r, "lowerorequaltmp");
    }
    case GE:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateICmpSGE(l, r, "greaterorequaltmp");
    }
    case NEQ:
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateICmpNE(l, r, "nequaltmp");
    }
    case EXPR_NOT: 
    {
        Value *l = ast_compile(t->branch1);
        return Builder.CreateNot(l, "nottmp");
    }
    case EXPR_AND: 
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateAnd(l, r, "andtmp");
    }
    case EXPR_OR: 
    {
        Value *l = ast_compile(t->branch1);
        Value *r = ast_compile(t->branch2);
        return Builder.CreateOr(l, r, "ortmp");
    }
    }
    return nullptr;
}

void declare_library_functions() {
    // declare void @writeInteger(i32)
    FunctionType *writeInteger_type =
        FunctionType::get(Type::getVoidTy(TheContext),
                          std::vector<Type *>{i32}, false);
    TheWriteInteger =
        Function::Create(writeInteger_type, Function::ExternalLinkage,
                         "writeInteger", TheModule.get());
    // declare void @writeChar(i8)
    FunctionType *writeChar_type = FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *>{i8}, false);
    TheWriteChar = Function::Create(writeChar_type, Function::ExternalLinkage, "writeChar", TheModule.get());
    //declare void @writyByte(i8)
    FunctionType *writeByte_type = FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *>{i8}, false);
    TheWriteByte = Function::Create(writeByte_type, Function::ExternalLinkage, "writeByte", TheModule.get());
    // declare void @writeString(i8*)
    FunctionType *writeString_type =
        FunctionType::get(Type::getVoidTy(TheContext),
                          std::vector<Type *>{PointerType::get(i8, 0)}, false);
    TheWriteString =
        Function::Create(writeString_type, Function::ExternalLinkage,
                         "writeString", TheModule.get());

    //declare int @readInteger()
    FunctionType *readInteger_type = FunctionType::get(Type::getInt32Ty(TheContext), NULL, false);
    TheReadInteger = Function::Create(readInteger_type, Function::ExternalLinkage, "readInteger", TheModule.get());
    //declare byte @readChar()
    FunctionType *readChar_type = FunctionType::get(Type::getInt8Ty(TheContext), NULL, false);
    TheReadChar = Function::Create(readChar_type, Function::ExternalLinkage, "readChar", TheModule.get());
    //declare byte @readByte()
    FunctionType *readByte_type = FunctionType::get(Type::getInt8Ty(TheContext), NULL, false);
    TheReadByte = Function::Create(readByte_type, Function::ExternalLinkage, "readByte", TheModule.get());
    //declare void @readInteger(i32, i8)
    FunctionType *readString_type = 
        FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *>{i32, PointerType::get(i8, 0)}, false);
    TheReadString = Function::Create(readString_type, Function::ExternalLinkage, "readString", TheModule.get());

    //declare int @extend(i8)
    FunctionType *extend_type = FunctionType::get(Type::getInt32Ty(TheContext), std::vector<Type *>{i8}, false);
    extend = Function::Create(extend_type, Function::ExternalLinkage, "extend", TheModule.get());
    //declare byte @shrink(i32)
    FunctionType *shrink_type = FunctionType::get(Type::getInt8Ty(TheContext), std::vector<Type *>{i32}, false);
    shrink = Function::Create(shrink_type, Function::ExternalLinkage, "shrink", TheModule.get());

    //declare int @strlen(*i8)
    FunctionType *strlen_type = FunctionType::get(Type::getInt32Ty(TheContext), std::vector<Type *>{PointerType::get(i8, 0)}, false);
    TheStrlen = Function::Create(strlen_type, Function::ExternalLinkage, "strlen", TheModule.get());
    //declare int @strcmp(*i8,*i8)
    FunctionType *strcmp_type = 
        FunctionType::get(Type::getInt32Ty(TheContext), std::vector<Type *>{PointerType::get(i8, 0), PointerType::get(i8, 0)}, false);
    TheStrcmp = Function::Create(strcmp_type, Function::ExternalLinkage, "strcmp", TheModule.get());
    //declare void @strcpy(*i8,*i8)
    FunctionType *strcpy_type = 
        FunctionType::get(Type::getVoidTy(TheContext), std::vector<Type *>{PointerType::get(i8, 0), PointerType::get(i8, 0)}, false);
    TheStrcpy = Function::Create(strcpy_type, Function::ExternalLinkage, "strcpy", TheModule.get());
    //declare void @strcat(*i8,*i8)
    TheStrcat = Function::Create(strcpy_type, Function::ExternalLinkage, "strcat", TheModule.get());
    return;
}

void llvm_compile_and_dump(ast t)
{
    // Initialize the module and the optimization passes.
    TheModule = make_unique<Module>("dana program", TheContext);
    TheFPM = make_unique<legacy::FunctionPassManager>(TheModule.get());
    // TheFPM->add(createPromoteMemoryToRegisterPass());
    // TheFPM->add(createInstructionCombiningPass());
    // TheFPM->add(createReassociatePass());
    // TheFPM->add(createGVNPass());
    // TheFPM->add(createCFGSimplificationPass());
    TheFPM->doInitialization();

    declare_library_functions();
                         
    // Define and start the main function.
    // Constant *c = TheModule->getOrInsertFunction("main", i32, NULL);
    // Function *main = cast<Function>(c);
    // BasicBlock *BB = BasicBlock::Create(TheContext, "entry", main);
    // Builder.SetInsertPoint(BB);

    //we initialize the global current_AR
    current_AR = StructType::create(TheContext, "dummy");
    //currentAlloca = Builder.CreateAlloca(current_AR, 0, "");
    // Emit the program code.
    ast_compile(t);

    Function *main = TheModule->getFunction("main");

    // Verify and optimize the main function.
    bool bad = verifyModule(*TheModule, &errs());
    if (bad)
    {
        fprintf(stderr, "The faulty IR is:\n");
        fprintf(stderr, "------------------------------------------------\n\n");
        TheModule->print(outs(), nullptr);
        return;
    }
    TheFPM->run(*main);
    // Print out the IR.
    TheModule->print(outs(), nullptr);
}