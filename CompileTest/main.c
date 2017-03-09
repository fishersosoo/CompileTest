#include"lexya_a.tab.h"
extern int LEGO_Parse(char* str);
int main(void)
{
	char* s = "1+2\n";
	LEGO_Parse(s);
	system("pause");
	return 0;
}