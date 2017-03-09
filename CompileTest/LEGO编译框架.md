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
```yacc
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

>   `Lex` 生成的代码可能在 vs 下编译可能会出现 `fileno` 报错，只要改成 `_fileno`就可以了

# yacc里面的一些设置

### 设置符号值的类型

将数据结构定义到外部的头文件中，在 yacc 中 include 进去

在 yacc 中，所有的符号都会有一个对应的值与之相关联。这个值默认是`int`。

通过将类型声明添加到 `union` 中，我们可以添加其他类型进去。

数据结构需要在 action 中使用就必须在定义符号之前包含在 `union` 体中

```
%union {
    Node *node;
    NBlock *block;
    NExpression *expr;
    NStatement *stmt;
    NIdentifier *ident;
    NVariableDeclaration *var_decl;
    std::vector *varvec;
    std::vector *exprvec;
    std::string *string;
    int token;
}
```

即使是使用的类类型要明确写进去，如果用子类就写子类，用父类就写父类。

>   `union` 联合体很像 `struct` ，其作用就是让这个联合体类型的变量拥有足够空间去容纳联合体内的任一类型。

在定义符号的时候，就要告诉yacc，这个符号是什么类型的。对于终结符使用 `%token <类型> 符号` 来定义，对于非终结符使用 `type <类型> 符号` 。要注意的是，类型填的是 `union` 中的变量名，不是变量类型。（也就是填 node，不要填 Node*）。

如果不需要使用某个符号的值的时候，大可以不用定义类型，不过定义符号还是要的。当 yacc 中存在 union ，所有所有使用值的符号都需要类型定义（就不会默认是 int 了）。

### action 部分

`$$` 表示产生式左边符号的值，`$x` 表示产生式右边第x个符号（可以是终结符也可以是非终结符）。所以打印结果的加法式子 `E->a+b` action应该这样写`{printf(%d =%d + %d);}`。