#pragma once
#ifndef TREE_H
#include"Tree.h"
#endif

#include<string>
#include<vector>
#ifndef BYTE
#define BYTE unsigned char
#endif
typedef void(*FPtr)(void);

#ifndef CODEGENERATEAPI_H
#define CODEGENERATEAPI_H
#endif

class CodeGenerateAPI
{
public:
	virtual void SetErrorCallback(FPtr p) = 0;
	FPtr p;
	Tree* SyntaxTree;
	CodeGenerateAPI(Tree* tree);
	CodeGenerateAPI();
	virtual std::string GenerateCode(std::string str) = 0;
	virtual std::string GetVarAddr(char var) = 0;
	virtual std::string GetTempAddr() = 0;
};

