#ifndef __AST_H__
#define __AST_H__

#include "symbol.h"

typedef enum {
  PROGRAM, HEADER, HEADER_PART, FUNC_DEF, FPAR_DEF, FUNC_DECL,
  SKIP, BREAK, CONT, LOOP, SEQ, IF, IF_ELSE,
  PLUS, MINUS, TIMES, DIV, MOD,
  NOT, AND, OR, EQ, LT, GT, LE, GE, NEQ,
  EXIT, ASSIGN, CALL, RET, DECL, INTCONST, CHARCONST,
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

ast ast_id (char *c);
ast ast_int_const (int n);
ast ast_op (ast l, kind op, ast r);
ast ast_loop(ast l, ast r);

void ast_sem (ast t);

int ast_run (ast t);

#endif
