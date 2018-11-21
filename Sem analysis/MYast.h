#ifndef __AST_H__
#define __AST_H__

#include "symbol.h"

typedef enum {
  PROGRAM, HEADER, HEADER_PART, FUNC_DEF, FPAR_DEF, FUNC_DECL,
  PROC_CALL, VAR, ID,
  SKIP, BREAK, CONT, LOOP, SEQ, IF, IF_ELSE, BLOCK,
  PLUS, MINUS, TIMES, DIV, MOD,
  NOT, AND, OR, EQ, LT, GT, LE, GE, NEQ,
  EXIT, ASSIGN, PROC_CALL, FUNC_CALL, RET, DECL,
  STRING_LIT, TID, ARR,
  INTCONST, CHARCONST,
  EXPR_PART, EXPR_NOT, EXPR_OR, EXPR_AND
} kind;

typedef struct node {
  kind k;
  char *id;
  int num;
  struct node *branch1, *branch2, *branch3, *branch4;
  Type type;
  int nesting_diff;  // ID and LET nodes
  int offset;        // ID and LET nodes
  int num_vars;      // BLOCK node
} *ast;

ast ast_func_def(ast header, ast local, ast block);
ast ast_header(char *l1, Type t, ast l2, ast l3);

ast ast_fpar_def(ast l1, Type t);
//ast ast_func_decl(ast l1);
ast ast_decl (ast l1);
ast ast_id (char *c, ast next);
ast ast_op (ast l, kind op, ast r);
ast ast_skip();

ast ast_arr(ast l1, ast l2);
ast ast_int_const (int n);
ast ast_loop(char *l, ast r);

void ast_sem (ast t);

int ast_run (ast t);

#endif
