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
	char* temp_file_path = argv[argc - 1];
	std::fstream temp_file(temp_file_path, std::ios::out| std::ios::trunc);
	temp_file << code_generate_api_impl.GenerateCode(exp);
	temp_file.close();
	return 1;
}
