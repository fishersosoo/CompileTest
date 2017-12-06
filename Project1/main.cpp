#include <Windows.h>
#include "CodeGenerateAPIImpl.h"
void main()
{
	//����MemoryInfoʵ��
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
	//����CodeGenerateAPIImplʵ��
	CodeGenerateAPIImpl code_generate_api_impl(memory_info);
	code_generate_api_impl.ResultAddr = "r";

	const char* exp = "1+a";//���ʽ
	const char* exe_path = "CompileTestProxy.exe";//�ⲿִ���ļ�·��
	const char* temp_path = "Temp";//��ʱ�ļ�·��
	for (int i = 0; i < 10; ++i)
	{
		std::cout << code_generate_api_impl.GenerateCodeByProxy(exp, exe_path, temp_path)<< std::endl;
	}
	system("pause");
	return;
}