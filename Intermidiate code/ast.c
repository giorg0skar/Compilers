#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

static ast ast_make (kind k, char c, int n, ast l, ast r) {
  ast p;
  if ((p = malloc(sizeof(struct node))) == NULL)
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

int label = 0;

void ast_compile (ast t) {
  if (t == NULL) return;
  switch (t->k) {
  case PRINT:
    printf("  pushl %%edi\n");
    ast_compile(t->left);
    printf("  subl $8, %%esp\n");
    printf("  call _writeInteger\n");
    printf("  addl $12, %%esp\n");
    printf("  movl $NL, %%eax\n");
    printf("  pushl %%eax\n");
    printf("  subl $8, %%esp\n");
    printf("  call _writeString\n");
    printf("  addl $12, %%esp\n");
    printf("  popl %%edi\n");
    return;
  case LET:
    ast_compile(t->left);
    printf("  popl %%eax\n");
    printf("  movl %%eax, %d(%%edi)\n", 4*(t->id - 'a'));
    return;
  case FOR: {
    ast_compile(t->left);
    int l2 = ++label;
    printf("L%d:\n", l2);
    printf("  popl %%eax\n");
    printf("  cmpl $0, %%eax\n");
    int l1 = ++label;
    printf("  jle L%d\n", l1);
    printf("  decl %%eax\n");
    printf("  pushl %%eax\n");
    ast_compile(t->right);
    printf("  jmp L%d\n", l2);
    printf("L%d:\n", l1);
    return;
  }
  case IF: {
    ast_compile(t->left);
    printf("  popl %%eax\n");
    printf("  andl %%eax, %%eax\n");
    int l = ++label;
    printf("  jz L%d\n", l);
    ast_compile(t->right);
    printf("L%d:\n", l);
    return;
  }
  case SEQ:
    ast_compile(t->left);
    ast_compile(t->right);
    return;
  case ID:
    printf("  pushl %d(%%edi)\n", 4*(t->id - 'a'));
    return;
  case CONST:
    printf("  pushl $%d\n", t->num);
    return;
  case PLUS:
    ast_compile(t->left);
    ast_compile(t->right);
    printf("  popl %%ebx\n");
    printf("  popl %%eax\n");
    printf("  addl %%ebx, %%eax\n");
    printf("  pushl %%eax\n");
    return;
  case MINUS:
    ast_compile(t->left);
    ast_compile(t->right);
    printf("  popl %%ebx\n");
    printf("  popl %%eax\n");
    printf("  subl %%ebx, %%eax\n");
    printf("  pushl %%eax\n");
    return;
  case TIMES:
    ast_compile(t->left);
    ast_compile(t->right);
    printf("  popl %%ebx\n");
    printf("  popl %%eax\n");
    printf("  mull %%ebx\n");
    printf("  pushl %%eax\n");
    return;
  case DIV:
    ast_compile(t->left);
    ast_compile(t->right);
    printf("  popl %%ebx\n");
    printf("  popl %%eax\n");
    printf("  cdq\n");
    printf("  divl %%ebx\n");
    printf("  pushl %%eax\n");
    return;
  case MOD:
    ast_compile(t->left);
    ast_compile(t->right);
    printf("  popl %%ebx\n");
    printf("  popl %%eax\n");
    printf("  cdq\n");
    printf("  divl %%ebx\n");
    printf("  pushl %%edx\n");
    return;
  }
}

void prologue() {
  printf(".text\n"
         ".global _start\n"
         "\n"
         "_start:\n"
         "  movl $var, %%edi\n"
         "\n");
}

void epilogue() {
  printf("\n");
  printf("  movl $0, %%ebx\n");
  printf("  movl $1, %%eax\n");
  printf("  int $0x80\n");
  printf("\n");
  printf(".data\n");
  printf("var:\n");
  printf(".rept 26\n");
  printf(".long 0\n");
  printf(".endr\n");
  printf("NL:\n");
  printf(".asciz \"\\n\"\n");  
}
