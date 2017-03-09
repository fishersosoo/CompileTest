# 编译框架 #
## lex 和 yacc设置 ##
lex设置
```lex
%{
  #include<stdlib.h>
  #define YY_NO_UNISTD_H 1
  void yyerror(char *);
  #include "lexya_a.tab.h"
  #define YY_DECL int yylex (char* str)
  #define YY_USER_INIT if ( ! YY_CURRENT_BUFFER ) {\
		yyensure_buffer_stack ();\
		YY_CURRENT_BUFFER_LVALUE = yy_scan_string(str);\
		}
%}
%%
([1-9][0-9]+)|([0-9])       { yylval = atoi(yytext); return INTEGER; }
[-+*/]       {return *yytext;}
[\t]         {}
[abcd]        {return yylval=yytext;return VAR;}
[(]           {return *yytext;}
[)]           {return *yytext;}
.            {yyerror("��Ч�ַ�");}
[\n]        {return *yytext;}
%%
int yywrap(void) 
{
  return 1;
}


```
yacc设置
```
%{
#include <stdlib.h>
#define YYLEX_PARAM str
int yylex(char* str);
void yyerror(char *);
%}
%token INTEGER
%token VAR
%left '+' '-'
%left '*' '/'
%right UMINUS
%%
program : program expr '\n' { printf("end");}
        |
        ;
expr : INTEGER { $$ = $1; }
     | expr '*' expr { $$ = $1 * $3;  printf("%d = %d * %d \n", $$,$1,$3);}
     | expr '/' expr { $$ = $1 / $3; printf("%d = %d / %d \n", $$,$1,$3);}
     | expr '+' expr { $$ = $1 + $3; printf("%d = %d + %d \n", $$,$1,$3);}
     | expr '-' expr { $$ = $1 - $3; printf("%d = %d - %d \n", $$,$1,$3);}
     | '('expr')'{$$=$2;printf("%d =  %d \n", $$,$2);}
     | '-' expr %prec UMINUS{$$=-$2;printf("%d = - %d \n", $$,$2);}
     | VAR {}
     ;
%%
void yyerror(char *s) 
{
  printf("%s\n", s);
}
int LEGO_Parse(char* str) 
{
  yyparse(str);
  return 0;
}
```
修改生成文件`lexya_a.tab.c`
修改`yyparse()`定义
查找
```c
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
```
将活动的预处理器模块中`yyparse()`定义修改为`int yyparse (char* str)`。

修改`yyparse()`的声明
搜索`| yyparse.  |`找到声明的活动预处理快
修改
```c
int
yyparse (char* str)
```
