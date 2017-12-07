#pragma once

#ifndef LEGOP_H
#include "LEGOP.h"
#endif
#ifndef _MAP_
#include <map>
#endif
#ifndef _SET_
#include <set>
#endif
#ifndef _IOSTREAM_
#include <iostream>
#endif
#ifndef _SSTREAM_
#include <sstream>
#endif
#ifndef _WINDOWS_
#include <Windows.h>
#endif
#ifndef _FSTREAM_
#include <fstream>
#endif
#ifndef CODEGENERATEAPI_H
#include "CodeGenerateAPI.h"
#endif
/************************************************************************/
/* 这个接口实现对照的是20160521的汇编指令集                             */
/************************************************************************/
struct MemoryInfo
{
	std::string encode();
	void decode(int argc, char* argv[]);
	std::map<char, std::string> VarNameToAddr;
	std::set<std::string> TempAddrs;
};

class CodeGenerateAPIImpl :
	public CodeGenerateAPI
{

	std::string Float_convertbyte(float FloatNum);
	std::string GenerateCodeHelper(Node* node);
public:
	static std::string IntConvertByte(unsigned int IntNum);
	void SetErrorCallback(FPtr p);
	const BYTE AssignFloat = 0x02;
	const BYTE CopyFloat = 0x04;
	const BYTE MathCode = 0x0D;
	std::map<char, BYTE> BinaryOpCMD;
	std::map<char, BYTE> RightOpCMD;
	std::string ResultAddr;
	void initMap();
	MemoryInfo memory_info_;
	CodeGenerateAPIImpl();
	CodeGenerateAPIImpl(MemoryInfo memory_info);
	std::string GetVarAddr(char var) override;
	std::string GetTempAddr() override;
	void FreeTempAddr(std::string addr);
	std::string GenerateCode(std::string str) override;
	std::string GenerateCodeByProxy(std::string str, const char* file_path, const char* temp_file_path);
};
