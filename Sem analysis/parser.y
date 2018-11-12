%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol.h"
#include <vector>
#include <string>

void yyerror (const char *msg);

vector<string> *ids; 
extern int linenumber;
ast t;
%}

%token T_and "and"
%token T_as "as"
%token T_begin "begin"
%token T_break "break"
%token T_byte "byte"
%token T_continue "continue"
%token T_decl "decl"
%token T_def "def"
%token T_elif "elif"
%token T_else "else"
%token T_end 
%token T_exit "exit"
%token T_false "false"
%token T_if "if"
%token T_is "is"
%token T_int "int"
%token T_loop "loop"
%token T_not "not"
%token T_or "or"
%token T_ref "ref"
%token T_return "return"
%token T_skip "skip"
%token T_true "true"
%token T_var "var"
%token T_less '<'
%token T_greater '>'
%token T_lesseq "<="
%token T_greatereq ">="
%token T_noteq "<>"
%token T_assign ":="
%token T_name 
%token T_char 
%token T_intconst 
%token T_string 
%token T_id
%token T_char_const
%token T_string_literal
%token T_auto_end

%left '+' '-'
%left '*' '/' '%'
%left UMINUS UPLUS

%%

program:
  func_def {t = $$ = ast_program($1);}
;

func_def:
  "def" header local_def block { $$ = ast_func_def($2,$3,$4); }
;

header:
  T_id "is" data_type ':' fpar_def header_part { $$ = ast_header($1,$3,$5,$6);}
;

header_part:
  %empty { $$ = NULL; }
| ',' fpar_def header_part { $$ = ast_header_part($2,$3);}
;

fpar_def:
  id "as" fpar_type { $$ = ast_fpar_def($1,$3);}
;

data_type:
  "int"  { $$ = typeInteger;}
| "byte" { $$ = typeChar;}
;

/*type:
  data_type type_part
;*/

type:
  type '[' T_intconst ']' { $$ = typeArray($3,$1);}
| data_type { $$ = $1;}
;

fpar_type:
  type { $$ = $1;}
| "ref" data_type { $$ = $2; }
| fpar_part { $$ = $1; } //??
;

fpar_part:
  data_type '[' ']' { $$ = typeIArray($1); }
| fpar_part '[' T_intconst ']' { $$ = typeArray($3,$1); } //??
;

/*fpar_type:
  type { $$ = $1;}
| "ref" data_type { $$ = $2; }
| data_type '['']' type_part
;*/

/*type_part:
  %empty { $$ = NULL;}
| '[' T_intconst ']' type_part  //??
;*/

local_def:
  %empty { $$ = NULL;}
| func_def local_def { $$ = ast_seq($1,$2);}
| func_decl local_def { $$ = ast_seq($1,$2);}
| var_def local_def { $$ = ast_seq($1,$2);}
;

func_decl:
  "decl" header { $$ = ast_func_decl($2); }
;

var_def:
  "var" id "is" type { $$ = ast_decl($2,$4); }
;

id:
  T_id { $$ = ast_idlist($1,NULL); }
| T_id id { $$ = ast_idlist($1,$2); } //??
;
/*id:
  T_id { ids= new vector<string>(); ids->push_back($1); $$ = ids; }
| T_id id {ids->push_back($1);} //??
;*/

stmt:
  "skip" { $$ = ast_skip(); }
| l_value ":=" expr { $$ = ast_assign($1,$3); }
| proc_call { $$ = $1; }
| "exit"
| "return" ':' expr { $$ = ast_return($3); }
| "if" cond ':' block { $$ = ast_if($2,$4); }
| "if" cond ':' block if_part "else" ':' block { $$ = ast_if_else($2,$4,$5,$8); }
| "loop" T_id ':' block { $$ = ast_loop($2,$4); } // ??
| "loop" ':' block { $$ = ast_loop(NULL,$3); }
| "break" ':' T_id
| "break"
| "continue" ':' T_id
| "continue"
| stmt stmt { $$ = ast_seq($1,$2);}
;

if_part:
  %empty { $$ = NULL; }
| "elif" cond ':' block if_part { $$ = ast_if_else($2,$4,$5,NULL); }
;

block:
  "begin" stmt "end" { $$ = ast_block($2);  }   //??
|  stmt T_auto_end
;

proc_call: 
  T_id ':' expr expr_part { $$ = ast_proc_call($1,$3,$4)} //??
;

func_call:
  T_id '(' ')'
| T_id '(' expr expr_part ')'
;

expr_part:
  %empty { $$ = NULL;}
| ',' expr expr_part { $$ = ast_expr_part($2,$3);}
;

l_value:
  T_id { $$ = $1;}
| T_string_literal { $$ = $1;}
| l_value '[' expr ']'
;

expr:
  T_intconst { $$ = ast_int_const($1);}
| T_char_const { $$ = ast_char_const($1); }
| l_value { $$ = $1; } //??
| '(' expr ')' { $$ = $2; }
| func_call { $$ = ast_func_call($1); }
| '+' expr	{ $$ = ast_op(ast_int_const(0),PLUS,$2); } %prec UPLUS
| '-' expr	{ $$ = ast_op(ast_int_const(0),MINUS,$2); } %prec UMINUS
| expr '+' expr { $$ = ast_op($1,PLUS,$3); }
| expr '-' expr { $$ = ast_op($1,MINUS,$3); }
| expr '*' expr { $$ = ast_op($1,TIMES,$3); }
| expr '/' expr { $$ = ast_op($1,DIV,$3); }
| expr '%' expr { $$ = ast_op($1,MOD,$3); }
| "true"  { $$ = $1;}
| "false" { $$ = $1;}
| '!' expr { $$ = ast_not_expr($2);}
| expr '&' expr { $$ = ast_and_expr($1,$3);}
| expr '|' expr { $$ = ast_or_expr($1,$3);}
;

cond :
expr {$$ = $1;}
| x_cond {$$ = $1;}
;


x_cond:
  "(" x_cond ")" {$$ = $2;}
| "not" cond {$$ = ast_op(NULL, NOT, $2);}
| cond "and" cond {$$ = ast_op($1,AND, $3);}
| cond "or" cond {$$ = ast_op($1,OR, $3);}
| expr '=' expr  {$$ = ast_op($1,EQ, $3);}
| expr '<' expr  {$$ = ast_op($1,LT, $3);}
| expr '>' expr  {$$ = ast_op($1,GT, $3);}
| expr "<=" expr  {$$ = ast_op($1,LE, $3);}
| expr ">=" expr  {$$ = ast_op($1,GE, $3);}
| expr "<>" expr  {$$ = ast_op($1,NEQ, $3);}
;

%%

void yyerror (const char *msg) {
  fprintf(stderr, "Dana error: %s\n", msg);
  fprintf(stderr, "Aborting, I've had enough with line %d...\n",linenumber);
  exit(1);
}

int main() {
  if (yyparse()) return 1;
  printf("Compilation was successful.\n");
  initSymbolTable(997);
  ast_sem(t);
  //ast_run(t);
  destroySymbolTable();
  return 0;
}

