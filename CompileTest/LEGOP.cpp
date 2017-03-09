#include "LEGOP.h"
#include<Windows.h>
void LEGOP(char* str){
	LEGO_Parse(str);
}
void main(){
	char* s = "1+2\n";
	LEGOP(s);
	system("pause");
}