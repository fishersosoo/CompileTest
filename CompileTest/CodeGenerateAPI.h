#pragma once
#include"Tree.h"
#include<string>
class CodeGenerateAPI
{
public:
	Tree* SyntaxTree;
	CodeGenerateAPI(Tree* tree);
	virtual std::string GenerateCode()=0;
	virtual std::string GetVarAddr(char var)=0;
	virtual std::string GetTempAddr(int Id)=0;
	CodeGenerateAPI();
	~CodeGenerateAPI();
};

