#include <fstream>
#include "CodeGenerateAPIImpl.h"

int main(int argc, char* argv[])
{
	std::string exp(argv[1]);
	if (exp[exp.size() - 1] != '\n')
	{
		exp += '\n';
	}

	MemoryInfo memory_info;
	memory_info.decode(argc, argv);
	CodeGenerateAPIImpl code_generate_api_impl(memory_info);
	std::stringstream ss;
	std::string temp_string = std::string(argv[argc - 1]);
	ss << temp_string;
	unsigned int temp_int;
	ss >> temp_int;
	code_generate_api_impl.ResultAddr = CodeGenerateAPIImpl::IntConvertByte(temp_int);
	char* temp_file_path = argv[argc - 2];
	std::fstream temp_file(temp_file_path, std::ios::out| std::ios::trunc);
	try{
	temp_file << code_generate_api_impl.GenerateCode(exp);
	}
	catch (...)
	{
		temp_file.close();
		return 1;
	}
	temp_file.close();
	return 1;
}
