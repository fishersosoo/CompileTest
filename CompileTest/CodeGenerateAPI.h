#pragma once
#include"Tree.h"
#include<string>
class CodeGenerateAPI
{
public:
	Tree* SyntaxTree;
	CodeGenerateAPI(Tree* tree);
	virtual std::string GenerateCode();
	virtual std::string GetVarAddr(char var);
	virtual std::string GetTempAddr(int Id);
	CodeGenerateAPI();
	~CodeGenerateAPI();
};

