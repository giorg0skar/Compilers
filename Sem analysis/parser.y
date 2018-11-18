%{
#include <stdio.h>
#include <stdlib.h>
#include "MYast.h"
#include "symbol.h"
#include <string.h>
//#include <vector>
//#include <string>

void yyerror (const char *msg);

//vector<string> *ids; 
extern int linenumber;
ast t;
%}

//%start program
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
%token T_end  "end"
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
  func_def  { t = $$ = $1; }
;

func_def:
  "def" header local_def block  { $$ = ast_func_def($2,$3,$4); }
;

header:
  T_id                                          { $$ = ast_header($1,typeVoid,NULL,NULL); }
| T_id "is" data_type                           { $$ = ast_header($1,$3,NULL,NULL); }
| T_id ':' fpar_def header_part                 { $$ = ast_header($1,typeVoid,$3,$4); }
| T_id "is" data_type ':' fpar_def header_part  { $$ = ast_header($1,$3,$5,$6); }
;

header_part:
  %empty  { $$ = NULL; }
| ',' fpar_def header_part
;

fpar_def:
  id "as" fpar_type   { $$ = ast_fpar_def($1,$3); }
;

data_type:
  "int"   { $$ = typeInteger;}
| "byte"  { $$ = typeChar;}
;

type:
  data_type { $$ = $1; }
| type '[' T_intconst ']' { $$ = typeArray($3,$1); }
;

fpar_type:
  type
| "ref" data_type
| fpar_part
;

fpar_part:
  data_type '[' ']' { $$ = typeIArray($1); }
| fpar_part '[' T_intconst ']'  { $$ = typeArray($3,$1); }
;

local_def:
  %empty  { $$ = NULL; }
| func_def local_def    { $$ = ast_seq($1,$2); }
| func_decl local_def
| var_def local_def
;

func_decl:
  "decl" header
;

var_def:
  "var" id "is" type
;

id:
  T_id
| T_id id
;

stmt:
  "skip"
| l_value ":=" expr
| proc_call
| "exit"
| "return" ':' expr
| "if" cond ':' block
| "if" cond ':' block if_part "else" ':' block
| "loop" ':' block
| "loop" T_id ':' block
| "break"
| "break" ':' T_id
| "continue"
| "continue" ':' T_id
| stmt stmt   { $$ = ast_seq($1,$2); }
;

if_part:
  %empty
| "elif" cond ':' block if_part
;

block:
  "begin" stmt "end"
| stmt T_end
;

proc_call:
  T_id
| T_id ':' expr expr_part
;

expr_part:
  %empty
| ',' expr expr_part
;

func_call:
  T_id '(' ')'
| T_id '(' expr expr_part ')'
;

l_value:
  T_id
| T_string_literal
| l_value '[' expr ']'
;

expr:
  T_intconst
| T_char_const
| l_value
| '(' expr ')'
| func_call
| '+' expr
| '-' expr
| expr '+' expr
| expr '-' expr
| expr '*' expr
| expr '/' expr
| expr '%' expr
| "true"
| "false"
| '!' expr
| expr '&' expr
| expr '|' expr
;

cond:
  expr
| x_cond
;

x_cond:
  '(' x_cond ')'
| "not" cond
| cond "and" cond
| cond "or" cond
| expr '=' expr
| expr "<>" expr
| expr '<' expr
| expr '>' expr
| expr "<=" expr
| expr ">=" expr
;

%%

void yyerror (const char *msg) {
  fprintf(stderr, "Dana error: %s\n", msg);
  fprintf(stderr, "Aborting, I've had enough with line %d...\n",linenumber);
  //fprintf(stderr, "text is %s\n", yytext);
  exit(1);
}

int main() {
    if (yyparse()) return 1;
    printf("Parsing was succesful!\n");
    initSymbolTable(997);
    ast_sem(t);
    //ast_run(t);
    destroySymbolTable();
    return 0;
}
