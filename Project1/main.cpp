#include <Windows.h>
#include "CodeGenerateAPIImpl.h"
void main()
{
	MemoryInfo memory_info;
	std::map<char, std::string> VarNameToAddr;
	VarNameToAddr['a'] = "1";
	VarNameToAddr['b'] = "2";
	std::set<std::string> TempAddrs;
	TempAddrs.insert("x");
	TempAddrs.insert("y");
	TempAddrs.insert("z");
	memory_info.VarNameToAddr = VarNameToAddr;
	memory_info.TempAddrs = TempAddrs;
	CodeGenerateAPIImpl code_generate_api_impl(memory_info);
	for (int i = 0; i < 10; ++i)
	{
		std::cout << code_generate_api_impl.GenerateCodeByProxy("1+a", "CompileTestProxy.exe", "Temp")<< std::endl;
	}
	system("pause");
	return;
}