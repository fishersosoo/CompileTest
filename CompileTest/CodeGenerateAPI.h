#pragma once
#include"Tree.h"
#include<string>
#include<vector>
#ifndef BYTE
#define BYTE unsigned char
#endif

class CodeGenerateAPI
{
public:
	Tree* SyntaxTree;
	CodeGenerateAPI(Tree* tree);
	CodeGenerateAPI();
	virtual std::string GenerateCode(std::string str) = 0;
	virtual std::string GetVarAddr(char var) = 0;
	virtual std::string GetTempAddr() = 0;
};

