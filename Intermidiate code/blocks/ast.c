#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "error.h"
#include "symbol.h"

static ast ast_make (kind k, char c, int n, ast l, ast r, Type t) {
  ast p;
  if ((p = malloc(sizeof(struct node))) == NULL)
    exit(1);
  p->k = k;
  p->id = c;
  p->num = n;
  p->left = l;
  p->right = r;
  p->type = t;
  return p;
}

ast ast_id (char c) {
  return ast_make(ID, c, 0, NULL, NULL, NULL);
}

ast ast_const (int n) {
  return ast_make(CONST, '\0', n, NULL, NULL, NULL);
}

ast ast_op (ast l, kind op, ast r) {
  return ast_make(op, '\0', 0, l, r, NULL);
}

ast ast_print (ast l) {
  return ast_make(PRINT, '\0', 0, l, NULL, NULL);
}

ast ast_let (char c, ast l) {
  return ast_make(LET, c, 0, l, NULL, NULL);
}

ast ast_for (ast l, ast r) {
  return ast_make(FOR, '\0', 0, l, r, NULL);
}

ast ast_if (ast l, ast r) {
  return ast_make(IF, '\0', 0, l, r, NULL);
}

ast ast_seq (ast l, ast r) {
  if (r == NULL) return l;
  return ast_make(SEQ, '\0', 0, l, r, NULL);
}

ast ast_decl (char c, Type t) {
  return ast_make(DECL, c, 0, NULL, NULL, t);
}

ast ast_block (ast l, ast r) {
  if (r == NULL) return l;
  return ast_make(BLOCK, '\0', 0, l, r, NULL);
}

#define NOTHING 0

struct activation_record_tag {
  struct activation_record_tag * previous;
  int data[0];
};

typedef struct activation_record_tag * activation_record;

activation_record current_AR = NULL;

int ast_run (ast t) {
  if (t == NULL) return NOTHING;
  switch (t->k) {
  case PRINT:
    printf("%d\n", ast_run(t->left));
    return NOTHING;
  case LET: {
    activation_record ar = current_AR;
    for (int i = 0; i < t->nesting_diff; ++i) ar = ar->previous;
    ar->data[t->offset] = ast_run(t->left);
    return NOTHING;
  }
  case FOR:
    for (int i = 0, times = ast_run(t->left); i < times; ++i)
      ast_run(t->right);
    return NOTHING;
  case IF:
    if (ast_run(t->left) != 0) ast_run(t->right);
    return NOTHING;
  case SEQ:
    ast_run(t->left);
    ast_run(t->right);
    return NOTHING;
  case DECL:
    return NOTHING;
  case BLOCK: {
    activation_record new_AR =
        (activation_record) malloc(
            sizeof(struct activation_record_tag) +
            t->num_vars * sizeof(int));
    new_AR->previous = current_AR;
    current_AR = new_AR;
    for (int i = 0; i < t->num_vars; ++i) new_AR->data[i] = 0;
    ast_run(t->left);
    ast_run(t->right);
    current_AR = current_AR->previous;
    free(new_AR);
    return NOTHING;
  }
  case ID: {
    activation_record ar = current_AR;
    for (int i = 0; i < t->nesting_diff; ++i) ar = ar->previous;
    return ar->data[t->offset];
  }
  case CONST:
    return t->num;
  case PLUS:
    return ast_run(t->left) + ast_run(t->right);
  case MINUS:
    return ast_run(t->left) - ast_run(t->right);
  case TIMES:
    return ast_run(t->left) * ast_run(t->right);
  case DIV:
    return ast_run(t->left) / ast_run(t->right);
  case MOD:
    return ast_run(t->left) % ast_run(t->right);
  case LT:
    return ast_run(t->left) < ast_run(t->right);
  case GT:
    return ast_run(t->left) > ast_run(t->right);
  case LE:
    return ast_run(t->left) <= ast_run(t->right);
  case GE:
    return ast_run(t->left) >= ast_run(t->right);
  case EQ:
    return ast_run(t->left) == ast_run(t->right);
  case NE:
    return ast_run(t->left) != ast_run(t->right);
  case AND:
    return ast_run(t->left) && ast_run(t->right);
  case OR:
    return ast_run(t->left) || ast_run(t->right);
  case NOT:
    return !ast_run(t->left);
  }
}

SymbolEntry * lookup(char c) {
  char name[] = " ";
  name[0] = c;
  return lookupEntry(name, LOOKUP_ALL_SCOPES, true);
}

SymbolEntry * insert(char c, Type t) {
  char name[] = " ";
  name[0] = c;
  return newVariable(name, t);
}

void ast_sem (ast t) {
  if (t == NULL) return;
  switch (t->k) {
  case PRINT:
    ast_sem(t->left);
    if (!equalType(t->left->type, typeInteger))
      error("you cannot print something that's not an integer");
    return;
  case LET: {
    SymbolEntry * e = lookup(t->id);
    ast_sem(t->left);
    if (!equalType(e->u.eVariable.type, t->left->type))
      error("type mismatch in assignment");
    t->nesting_diff = currentScope->nestingLevel - e->nestingLevel;
    t->offset = e->u.eVariable.offset;
    return;
  }
  case FOR:
    ast_sem(t->left);
    if (!equalType(t->left->type, typeInteger))
      error("for loop expects an integer number");
    ast_sem(t->right);
    return;
  case IF:
    ast_sem(t->left);
    if (!equalType(t->left->type, typeBoolean))
      error("if expects a boolean condition");
    ast_sem(t->right);
    return;
  case SEQ:
    ast_sem(t->left);
    ast_sem(t->right);
    return;
  case ID: {
    SymbolEntry *e = lookup(t->id);
    t->type = e->u.eVariable.type;
    t->nesting_diff = currentScope->nestingLevel - e->nestingLevel;
    t->offset = e->u.eVariable.offset;
    return;
  }
  case CONST:
    t->type = typeInteger;
    return;
  case PLUS:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in + operator");
    t->type = typeInteger;
    return;
  case MINUS:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("\rtype mismatch in - operator");
    t->type = typeInteger;
    return;
  case TIMES:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in * operator");
    t->type = typeInteger;
    return;
  case DIV:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in / operator");
    t->type = typeInteger;
    return;
  case MOD:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in % operator");
    t->type = typeInteger;
    return;
  case EQ:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in = operator");
    t->type = typeBoolean;
    return;
  case NE:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in <> operator");
    t->type = typeBoolean;
    return;
  case LT:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in < operator");
    t->type = typeBoolean;
    return;
  case LE:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in <= operator");
    t->type = typeBoolean;
    return;
  case GT:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in > operator");
    t->type = typeBoolean;
    return;
  case GE:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeInteger) ||
        !equalType(t->right->type, typeInteger))
      error("type mismatch in >= operator");
    t->type = typeBoolean;
    return;
  case AND:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeBoolean) ||
        !equalType(t->right->type, typeBoolean))
      error("type mismatch in and operator");
    t->type = typeBoolean;
    return;
  case OR:
    ast_sem(t->left);
    ast_sem(t->right);
    if (!equalType(t->left->type, typeBoolean) ||
        !equalType(t->right->type, typeBoolean))
      error("type mismatch in or operator");
    t->type = typeBoolean;
    return;
  case NOT:
    ast_sem(t->left);
    if (!equalType(t->left->type, typeBoolean))
      error("type mismatch in not operator");
    t->type = typeBoolean;
    return;
  case DECL:
    insert(t->id, t->type);
    return;
  case BLOCK:
    openScope();
    ast_sem(t->left);
    t->num_vars = currentScope->negOffset;
    ast_sem(t->right);
    closeScope();
    return;
  }
}

int label = 0;

void ast_compile (ast t) {
  if (t == NULL) return;
  switch (t->k) {
  case PRINT:
    ast_compile(t->left);
    printf("  subl $8, %%esp\n");
    printf("  call _writeInteger\n");
    printf("  addl $12, %%esp\n");
    printf("  movl $NL, %%eax\n");
    printf("  pushl %%eax\n");
    printf("  subl $8, %%esp\n");
    printf("  call _writeString\n");
    printf("  addl $12, %%esp\n");
    return;
  case LET:
    ast_compile(t->left);
    printf("  popl %%eax\n");
    if (t->nesting_diff == 0)
      printf("  movl %%eax, %d(%%ebp)\n", -4 * (t->offset + 1));
    else {
      printf("  movl 0(%%ebp), %%edi\n");
      for (int i = 0; i < t->nesting_diff - 1; ++i)
        printf("  movl 0(%%edi), %%edi\n");
      printf("  movl %%eax, %d(%%edi)\n", -4 * (t->offset + 1));
    }
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
  case DECL:
    return;
  case BLOCK:
    printf("  pushl %%ebp\n");
    printf("  movl %%esp, %%ebp\n");
    for (int i = 0; i < t->num_vars; ++i)
      printf("  pushl $0\n");
    ast_compile(t->left);
    ast_compile(t->right);
    printf("  addl $%d, %%esp\n", 4*t->num_vars);
    printf("  popl %%ebp\n");
    return;
  case ID:
    if (t->nesting_diff == 0)
      printf("  pushl %d(%%ebp)\n", -4 * (t->offset + 1));
    else {
      printf("  movl 0(%%ebp), %%edi\n");
      for (int i = 0; i < t->nesting_diff - 1; ++i)
        printf("  movl 0(%%edi), %%edi\n");
      printf("  pushl %d(%%edi)\n", -4 * (t->offset + 1));
    }
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
