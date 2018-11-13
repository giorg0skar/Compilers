%{
#include "tryparser.h"
#define T_eof 0

int choice=0;              //if choice==1 then we have read a keyword that begins a block
int blockstart=0;
int i;
int commentcounter;
int linenumber = 1;
int spacecounter=0;
int blockcounter=0;
int newline_block = 0;
int colon_block = 0;
int mystack[100];
int sp = -1;                //sp points to the latest element pushed into the stack

void push(int element);
int pop();
void yyerror (const char *msg);
%}

Id [[:alpha:]][[:alnum:]]*
Intconst [0-9]+
W [ \t\r]
asciizero \0
backslash \\
singlequote \'
doublequote \"
asciichar \x[0-9a-fA-F][0-9a-fA-F]
Char {singlequote}{1}.{singlequote}{1}

%x SLCOMMENT
%x MLCOMMENT
%x STRING
%x FINAL
%s BLOCK
%s SIMPLE
%x NEWLINE

%%
	BEGIN(BLOCK);

 /*--------------------------------------layout-------------------------------------- */

<BLOCK><<EOF>>		{ if( blockcounter > 0) BEGIN(FINAL); }
<BLOCK>" "		{ spacecounter++; }
<BLOCK>\t 		{ spacecounter+=4; }
<BLOCK>\n		{ spacecounter=0; linenumber++;
					if (newline_block == 1) {    
						newline_block = 0;
						return T_begin;		
					} 
					else {
						BEGIN(NEWLINE);
					}
				}
<BLOCK>:		{ if (colon_block == 1) {
				if (choice==0) {
					choice=1;
					char *yycopy = strdup( yytext );
					for ( i = yyleng - 1; i >= 0; --i ) unput( yycopy[i] );
					free(yycopy);
					return yytext[0];
			  	}
			  	else {
					choice=0;
					colon_block = 0;
					return T_begin;
				}   		
			  }
			  else
			  { return yytext[0]; }
			}		  
<BLOCK>"def" 		{ blockstart=1; newline_block = 1; push(spacecounter); blockcounter++; return T_def; }
<BLOCK>"if"  		{ blockstart=1; colon_block = 1; push(spacecounter); blockcounter++; return T_if; }
<BLOCK>"elif" 		{blockstart=1; colon_block = 1; push(spacecounter); blockcounter++; return T_elif; }
<BLOCK>"else"		{ blockstart=1; colon_block = 1; push(spacecounter); blockcounter++; return T_else; }
<BLOCK>"loop" 		{ blockstart=1; colon_block = 1; push(spacecounter); blockcounter++; return T_loop; }

<NEWLINE>" "		{ spacecounter++ ; }
<NEWLINE>\t 		{ spacecounter+=4; }
<NEWLINE>\n			{ spacecounter=0; linenumber++;}
<NEWLINE>[^ \t\n]	{ while(mystack[sp] >= spacecounter && sp > -1) { 
						sp--; blockcounter--;
						char *yycopy = strdup( yytext );
						for ( i = yyleng - 1; i >= 0; --i ) unput( yycopy[i] );
						free(yycopy);
						return T_end;
					 }
					char *yycopy = strdup( yytext );
					for ( i = yyleng - 1; i >= 0; --i ) unput( yycopy[i] );
					free(yycopy);
					BEGIN(BLOCK);
					}
<NEWLINE><<EOF>>		{ if( blockcounter > 0) BEGIN(FINAL); }

<FINAL><<EOF>>		{ while (blockcounter!=0) printf("token=%d\n", CloseBlocks()); return T_eof;}

 /*--------------------------------------end of layout-------------------------------------- */


"and"			{ return T_and; }
"as"			{ return T_as; }
"begin"			{ return T_begin; }
"break"			{ return T_break; }
"byte"			{ return T_byte; }
"continue"		{ return T_continue; }
"decl"			{ return T_decl; }
"def"			{ return T_def; }					
"elif"			{ return T_elif; }
"else"			{ return T_else; }
"end"			{ return T_end; }
"exit"			{ return T_exit; }
"false"			{ return T_false; }
"if"			{ return T_if; }
"is"			{ return T_is; }
"int"			{ return T_int; }
"loop"			{ return T_loop; }
"not"			{ return T_not; }
"or"			{ return T_or; }
"ref"			{ return T_ref; }
"return"		{ return T_return;}
"skip"			{ return T_skip; }
"true"			{ return T_true; }
"var"			{ return T_var; }

	/*comment handling*/
"#"					{ BEGIN(SLCOMMENT); }
<SLCOMMENT>[^\n]*	{ /* nothing */ }
<SLCOMMENT>\n		{ linenumber++; BEGIN(BLOCK); }

"(*"				{ commentcounter = 1; BEGIN(MLCOMMENT); }
<MLCOMMENT>"(*"		{ commentcounter++; }
<MLCOMMENT>"*)"		{ if(--commentcounter==0) BEGIN(BLOCK); }
<MLCOMMENT>\n		{ linenumber++; }
<MLCOMMENT>"*"		{ /* nothing */ }
<MLCOMMENT>[^*\n(]*	{ /* nothing */ }

"\""				{ BEGIN(STRING); }
<STRING>[^"]*		{ return T_string; }
<STRING>\"			{ BEGIN(BLOCK); }

"<"				{ return T_less; }
">"				{ return T_greater; }
"<="			{ return T_lesseq; }
">="			{ return T_greatereq; }
"<>"			{ return T_noteq; }
":="			{ return T_assign; }

	/*extra regular expression-whenever you find the following return the ascii character*/

[(),:=+\-*/%!&|]	{ return yytext[0]; }

"["			{ return yytext[0]; }
"]"			{ return yytext[0]; }
	

	/*identifiers*/

{Intconst}		{ return T_intconst; }
{Char}			{ return T_char; }
{Id}			{ return T_id; }
{W}	   			{ /* nothing */}
<SIMPLE>{W}+		{ /* nothing */}
\n			{ linenumber++; }

.			{ yyerror("illegal character"); }

%%

void push(int element) {
	sp++;
	mystack[sp]=element;
	return;
}

int pop(void) {
	int n= mystack[sp];
	sp--;
	return n;
}

int CloseBlocks(void) {
	blockcounter--;
	return T_end;
}
