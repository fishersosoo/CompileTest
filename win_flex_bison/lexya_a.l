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
([1-9][0-9]+)|([0-9])       { yylval.Double = atof(yytext); return INTEGER; }
[-+*/]       {return *yytext;}
[\t]         {}
[abcd]        {yylval.Char=*yytext;return VARA;}
[(]           {return *yytext;}
[)]           {return *yytext;}
.            {yyerror("��Ч�ַ�");}
[\n]        {return *yytext;}
%%
int yywrap(void) 
{
  return 1;
}
