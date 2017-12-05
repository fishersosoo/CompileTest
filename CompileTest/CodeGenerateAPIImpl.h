#pragma once
#ifndef CODEGENERATEAPI_H
#include "CodeGenerateAPI.h"
#endif
#ifndef LEGOP_H
#include "LEGOP.h"
#endif
#ifndef _MAP_
#include <map>
#endif
#ifndef _SET_
#include <set>
#endif
/************************************************************************/
/* 这个接口实现对照的是20160521的汇编指令集                             */
/************************************************************************/
struct MemoryInfo
{
	std::map<char, std::string> VarNameToAddr;
	std::set<std::string> TempAddrs;
};
class CodeGenerateAPIImpl :
	public CodeGenerateAPI
{
	std::string Float_convertbyte(float FloatNum);
	std::string GenerateCodeHelper(Node* node);
public:
	void SetErrorCallback(FPtr p);
	const BYTE AssignFloat = 0x02;
	const BYTE CopyFloat = 0x04;
	const BYTE MathCode = 0x0D;
	std::map<char, BYTE> BinaryOpCMD;
	std::map<char, BYTE>RightOpCMD;
	std::string ResultAddr;
	void initMap();
	MemoryInfo memory_info_;
	CodeGenerateAPIImpl();
	CodeGenerateAPIImpl(MemoryInfo memory_info);
	std::string GetVarAddr(char var) override;
	std::string GetTempAddr() override;
	void FreeTempAddr(std::string addr);
	std::string GenerateCode(std::string str) override;
};

