#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <iostream>
//#include <string>
//#include <cstdio>
//#include <cstdlib>
#include "MYast.h"
#include "error.h"
#include "symbol.h"

static ast ast_make (kind k, char *c, int n, ast l1, ast l2, ast l3, ast l4, Type t) {
  ast p;
  if ((p = malloc(sizeof(struct node))) == NULL) exit(1);
  p->k = k;
  p->id = (char *) malloc(sizeof(strlen(c + 1)));
  //p->id = c;
  strcpy(p->id, c);
  p->num = n;
  p->branch1 = l1;
  p->branch2 = l2;
  p->branch3 = l3;
  p->branch4 = l4;    //each node in the ast has <=4 children
  p->type = t;
  return p;
}

ast ast_program(ast l1) {
  return ast_make(PROGRAM, "\0", 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_func_def(ast header, ast local, ast block) {
  return ast_make(FUNC_DEF,"\0", 0, header, local, block, NULL, NULL);
}

ast ast_header(char *l1, Type t, ast l2, ast l3) {
  return ast_make(HEADER, l1, 0, l2, l3, NULL, NULL, t);
}

ast ast_header_part(ast l1, ast l2) {
  return ast_make(HEADER_PART, "\0", 0, l1, l2, NULL, NULL, NULL);
}

ast ast_fpar_def(ast l1, Type t) {
  return ast_make(FPAR_DEF, "\0", 0, l1, NULL, NULL, NULL, t);
}

ast ast_func_decl(ast l1) {
  return ast_make(FUNC_DECL, "\0", 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_decl (ast idlist, Type t) {
  return ast_make(DECL, "\0", 0, idlist, NULL, NULL, NULL, t);
}

ast ast_var(ast idlist, Type t) {
  return ast_make(VAR, "\0", 0, idlist, NULL, NULL, NULL, t);
}

ast ast_id(char *s, ast next) {
  return ast_make(ID, s, 0, next, NULL, NULL, NULL, NULL);
}

ast ast_op (ast l, kind op, ast r) {
  return ast_make(op, "\0", 0, l, r, NULL, NULL, NULL);
}

ast ast_skip() {
  return ast_make(SKIP, "\0", 0, NULL, NULL, NULL, NULL, NULL);
}

//assign a value to a variable
ast ast_assign(char* lvalue, ast r) {
  return ast_make(ASSIGN, lvalue, 0, r, NULL, NULL, NULL, NULL);
}

ast ast_if (ast l, ast r) {
  return ast_make(IF, "\0", 0, l, r, NULL, NULL, NULL);
}

ast ast_if_else (ast cond, ast blck, ast ifp, ast elblck) {
  return ast_make(IF_ELSE, "\0", 0, cond, blck, ifp, elblck, NULL);
}

ast ast_loop(char *s, ast l1) {
  return ast_make(LOOP, s, 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_seq(ast l1, ast l2) {
  if (l2 == NULL) return l1;
  return ast_make(SEQ, "\0", 0, l1, l2, NULL, NULL, NULL);
}

//not sure
ast ast_proc_call(char *c, ast l1, ast l2) {
  return ast_make(PROC_CALL, c, 0, l1, l2, NULL, NULL, NULL);
}

ast ast_func_call(ast l1) {
  return ast_make(CALL, "\0", 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_expr_part(ast l1, ast l2) {
  return ast_make(EXPR_PART, "\0", 0, l1, l2, NULL, NULL, NULL);
}

ast ast_not_expr(ast l1) {
  return ast_make(EXPR_NOT, "\0", 0, l1, NULL, NULL, NULL, NULL);
}

ast ast_and_expr(ast l1, ast l2) {
  return ast_make(EXPR_AND, "\0", 0, l1, l2, NULL, NULL, NULL);
}

ast ast_or_expr(ast l1, ast l2) {
  return ast_make(EXPR_OR, "\0", 0, l1, l2, NULL, NULL, NULL);
}

#define NOTHING 0

struct activation_record_tag {
  struct activation_record_tag * previous;
  int data[0];
};

typedef struct activation_record_tag * activation_record;

activation_record current_AR = NULL;

/*int ast_run (ast t) {
  if (t == NULL) return NOTHING;
  switch (t->k) {
    case SKIP:
      return NOTHING;
    case PROGRAM:
      ast_run(t->branch1); //??
      return NOTHING;
    case PRINT:
      printf("%d\n", ast_run(t->branch1));
      return NOTHING;
    case IF:
      if (ast_run(t->branch1) != 0) ast_run(t->branch2);
      return NOTHING;
    case IF_ELSE:
      if (ast_run(t->branch1) != 0) ast_run(t->branch2);
      //else if (ast_run(t->branch3->branch1) != 0) ast_run(t->branch3);
      ast_run(t->branch3);
      //  NOTE: needs work for else_if
      else ast_run(t->branch4);     //branch3,4 are extra ast parameters to each block specifically for cases like if...elif...else
      return NOTHING;
    case LOOP:
      for(;;) {
        ast_run(t->branch2);  // NOTE:unfinished
      }
      return NOTHING;
    case ID: {
      activation_record ar = current_AR;
      for (int i = 0; i < t->nesting_diff; ++i) ar = ar->previous;
      return ar->data[t->offset];
    }
    case PLUS:
      return ast_run(t->branch1) + ast_run(t->branch2);
    case MINUS:
      return ast_run(t->branch1) - ast_run(t->branch2);
    case TIMES:
      return ast_run(t->branch1) * ast_run(t->branch2);
    case DIV:
      return ast_run(t->branch1) / ast_run(t->branch2);
    case MOD:
      return ast_run(t->branch1) % ast_run(t->branch2);
    case LT:
      return ast_run(t->branch1) < ast_run(t->branch2);
    case GT:
      return ast_run(t->branch1) > ast_run(t->branch2);
    case LE:
      return ast_run(t->branch1) <= ast_run(t->branch2);
    case GE:
      return ast_run(t->branch1) >= ast_run(t->branch2);
    case EQ:
      return ast_run(t->branch1) == ast_run(t->branch2);
    case NE:
      return ast_run(t->branch1) != ast_run(t->branch2);
    case AND:
      return ast_run(t->branch1) && ast_run(t->branch2);
    case OR:
      return ast_run(t->branch1) || ast_run(t->branch2);
    case NOT:
      return !ast_run(t->branch2);
  }
}*/

SymbolEntry * lookup(char *c) {
  char *name;
  name = (char *) malloc(strlen(c)+1);
  //name[0] = c;
  strcpy((char *) name, c);
  return lookupEntry(name, LOOKUP_ALL_SCOPES, true);
}

SymbolEntry * insert(char *c, Type t) {
  char *name;
  name = (char *) malloc(strlen(c)+1);
  //name[0] = c;
  strcpy(name, c);
  return newVariable(name, t);
}

void ast_sem (ast t) {
  if (t==NULL) return;
  switch (t->k) {
    case FUNC_DEF:
      openScope();
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      t->num_vars = currentScope->negOffset;
      ast_sem(t->branch3);
      //?? stuff
      closeScope();
      return;
    case HEADER:
      SymbolEntry *f = newFunction(t->id);
      f->u.eFunction.resultType = t->type;
      //branch1: fpar_def , branch2: header_part i.e multiple fpar_defs
      //ast_sem(t->branch1);
      ast temp;
      Type parType;
      PassMode mode;
      if (t->branch1 != NULL) {
        temp = t->branch1;    //temp now shows to fpar_def node. branch1 of that node is the beginning of the list of id nodes
        parType = temp->type;
        if (temp->type->kind == TYPE_POINTER ) mode = PASS_BY_REFERENCE;
        else mode = PASS_BY_VALUE;
        for(temp=temp->branch1; temp!=NULL; temp=temp->branch1) {
          //we evaluate the semantics of each id node before we create the new entry. this is to avoid same name conflicts in the parameter definition
          ast_sem(temp);
          temp->type = parType;
          SymbolEntry *par = newParameter(temp->id, parType, mode, f);
        }
      }

      //ast_sem(t->branch2);

      //if t->branch2 is NULL the following loop exits immediatly so i don't need an if-check
      temp = t->branch2;    //temp now points to the header_part node. branch1 is a fpar_node and branch2 is another header_part node
      ast headerpart;
      for(headerpart=t->branch2; headerpart!=NULL; headerpart=headerpart->branch2) {
        //for each headerpart we evaluate a fpar_def node like before
        temp = headerpart->branch1;   //in every iteration temp points to the fpar_def node of the header_part node
        parType = temp->type;
        if (temp->type->kind == TYPE_POINTER ) mode = PASS_BY_REFERENCE;
        else mode = PASS_BY_VALUE;
        ast idnode;
        for(idnode=temp->branch1; idnode!=NULL; idnode=idnode->branch1) {
          ast_sem(idnode);
          idnode->type = parType;
          SymbolEntry *par = newParameter(idnode->id, parType, mode, f);
        }
      }

      endFunctionHeader(f, t->type);
      return;
    case FPAR_DEF:
      //this case may not need to get accessed. --> most likely.

      //here one or more parameters are defined. all parameters have the same type
      ast temp;
      for(temp=t->branch1; temp!=NULL; temp=temp->branch1) {
        ast_sem(temp);
        //we set the parameter type as the given one
        temp->type = t->type;
      }
      return;
    case DECL: //?? may be wrong
      // ast temp= t->branch1;
      // while(temp->branch1 != NULL) {
      //   insert(temp->id, t->type);
      //   temp=temp->branch1;
      // }
      return;
    case VAR:
      //var definitions
      ast temp = t->branch1;
      for(temp=t->branch1; temp!=NULL; temp=temp->branch1) {
        ast_sem(temp);
        temp->type = t->type;
        SymbolEntry *v = newVariable(temp->id, t->type);
      }
      return;
    case ID:
      //check if there's already a variable with the same name in current scope
      char c = t->id[0];
      if (!isalpha(c)) {
        error("variable names have to start with a letter. variable is: %s\n", t->id);
      }
      SymbolEntry *e = lookupEntry(t->id,LOOKUP_CURRENT_SCOPE,false);
      if (e!=NULL) {
        error("there is already a variable with the name: $s\n", t->id);
      }
      return;
    case SKIP:
      return;
    case IF:
      ast_sem(t->branch1);
      if (!equalType(t->branch1->type, typeBoolean))
        error("if expects a boolean condition");
      ast_sem(t->branch2);
      return;
    case IF_ELSE:
      ast_sem(t->branch1);
      if (!equalType(t->branch1->type, typeBoolean))
        error("if expects a boolean condition");
      ast_sem(t->branch2);
      ast_sem(t->branch3);
      //??
      return;
    case LOOP:
      // ??
      return;
    case SEQ:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      return;
      
    case PLUS:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeInteger;
        else error("type mismatch in + operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeChar;
        else error("type mismatch in + operator");
      }
      return;
    case MINUS:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeInteger;
        else error("type mismatch in - operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeChar;
        else error("type mismatch in - operator");
      }
      return;
    case TIMES:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeInteger;
        else error("type mismatch in * operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeChar;
        else error("type mismatch in * operator");
      }
      return;
    case DIV:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeInteger;
        else error("type mismatch in / operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeChar;
        else error("type mismatch in / operator");
      }
      return;
    case MOD:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeInteger;
        else error("type mismatch in mod operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeChar;
        else error("type mismatch in mod operator");
      }
      return;
    case AND:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if ( (!equalType(t->branch1->type, typeInteger)) || (!equalType(t->branch2->type, typeBoolean)))
        error("type mismatch in and operator");
      t->type = typeBoolean;
      return;
    case OR:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if ( (!equalType(t->branch1->type, typeInteger)) || (!equalType(t->branch2->type, typeBoolean)))
        error("type mismatch in or operator");
      t->type = typeBoolean;
      return;
    case EQ:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeBoolean;
        else error("type mismatch in = operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeBoolean;
        else error("type mismatch in = operator");
      }
      return;
    case LT:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeBoolean;
        else error("type mismatch in < operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeBoolean;
        else error("type mismatch in < operator");
      }
      return;
    case GT:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeBoolean;
        else error("type mismatch in > operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeBoolean;
        else error("type mismatch in > operator");
      }
      return;
    case LE:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeBoolean;
        else error("type mismatch in <= operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeBoolean;
        else error("type mismatch in <= operator");
      }
      return;
    case GE:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeBoolean;
        else error("type mismatch in >= operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeBoolean;
        else error("type mismatch in >= operator");
      }
      return;
    case NEQ:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if (equalType(t->branch1->type, typeInteger)) {
        if (equalType(t->branch2->type, typeInteger)) t->type = typeBoolean;
        else error("type mismatch in <> operator");
      }
      else if (equalType(t->branch1->type, typeChar)) {
        if (equalType(t->branch2->type, typeChar)) t->type = typeBoolean;
        else error("type mismatch in <> operator");
      }
      return;
    case EXPR_AND:
      ast_sem(t->branch1);
      ast_sem(t->branch2);
      if ((!equalType(t->branch1->type, typeChar)) || (!equalType(t->branch2->type, typeChar)))
        error("type mismatch in expression and operation");
      t->type = typeChar;
      return;
  }
}