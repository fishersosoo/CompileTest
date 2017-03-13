%{
#include <stdlib.h>
#ifndef TREE_H
#include "Tree.h"
#endif
#define YYLEX_PARAM str
int yylex(char* str);
void yyerror(char *);
static Tree* TreePtr;
%}
%union {
    Node* node;
	Tree* tree;
	char Char;
	double Double;
}
%token <Double> INTEGER
%token <Char> VARA
%left '+' '-'
%left '*' '/'
%right UMINUS

%type <node> expr
%%
program : program expr '\n' { TreePtr=new Tree($2);return 99;}
        |
        ;
expr : INTEGER { $$ = new Number($1); }
     | expr '*' expr { $$ = new BinaryOperator($1,$3,'*');}
     | expr '/' expr { $$ = new BinaryOperator($1,$3,'/');}
     | expr '+' expr { $$ = new BinaryOperator($1,$3,'+');}
     | expr '-' expr { $$ = new BinaryOperator($1,$3,'-');}
     | '('expr')'{$$=$2;}
     | '-' expr %prec UMINUS{$$ = new RightOperator($2,'-');}
     | VARA { $$ = new Var($1);}
     ;
%%
void yyerror(char *s) 
{
  printf("%s\n", s);
}
Tree* LEGO_Parse(char* str) 
{
  if(TreePtr!=NULL){
	  TreePtr->Clear();
	  TreePtr=NULL;
  }
  yyparse(str);
  return TreePtr;
}