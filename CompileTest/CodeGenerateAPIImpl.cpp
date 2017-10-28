#include "CodeGenerateAPIImpl.h"



std::string CodeGenerateAPIImpl::Float_convertbyte(float FloatNum)
{
	std::vector<BYTE> Byte;
	typedef union
	{
		float Num;
		BYTE data_b[4];
	} FloatYByte;
	FloatYByte temp;
	temp.Num = FloatNum;
	for (int i = 0; i < 4; ++i)
	{
		Byte.push_back(temp.data_b[i]);
	}
	std::string out_string;
	for (int i = 0; i < Byte.size(); ++i)
	{
		out_string += Byte[i];
	}
	return out_string;
}

std::string CodeGenerateAPIImpl::GenerateCodeHelper(Node* node)
{
	std::string return_str = "";
	std::string TempAddr;
	switch (node->NodeType)
	{
	case NUMBER:
		return_str += AssignFloat;
		TempAddr = GetTempAddr();
		return_str += TempAddr;
		node->node_info_.TempAddr.push_back(TempAddr);
		node->node_info_.NodeAddr = TempAddr;
		return_str += Float_convertbyte(dynamic_cast<Number*>(node)->Value);
		return return_str;
	case RIGHTOP:
		return_str += MathCode;
		return_str += RightOpCMD.at(dynamic_cast<RightOperator*>(node)->Operate);
		return_str += dynamic_cast<RightOperator*>(node)->Operand->node_info_.NodeAddr;
		return_str += dynamic_cast<RightOperator*>(node)->Operand->node_info_.NodeAddr;
		TempAddr = dynamic_cast<RightOperator*>(node)->node_info_.NodeAddr;
		node->node_info_.TempAddr.push_back(TempAddr);
		node->node_info_.NodeAddr = TempAddr;
		return return_str;
	case NODETYPE::VAR:
		return_str += CopyFloat;
		TempAddr = GetTempAddr();
		return_str += TempAddr;
		node->node_info_.TempAddr.push_back(TempAddr);
		node->node_info_.NodeAddr = TempAddr;
		return_str += GetVarAddr(dynamic_cast<Var*>(node)->VarName);
		return return_str;
	case BINARYOP:
		return_str += BinaryOpCMD.at(dynamic_cast<BinaryOperator*>(node)->Operate);
		return_str += dynamic_cast<BinaryOperator*>(node)->LeftOperand->node_info_.NodeAddr;
		return_str += dynamic_cast<BinaryOperator*>(node)->LeftOperand->node_info_.NodeAddr;
		return_str += dynamic_cast<BinaryOperator*>(node)->RightOperand->node_info_.NodeAddr;
		node->node_info_.TempAddr.push_back(dynamic_cast<BinaryOperator*>(node)->LeftOperand->node_info_.NodeAddr);
		node->node_info_.NodeAddr = dynamic_cast<BinaryOperator*>(node)->LeftOperand->node_info_.NodeAddr;
		FreeTempAddr(dynamic_cast<BinaryOperator*>(node)->RightOperand->node_info_.NodeAddr);
		return return_str;
	default:
		return "";
	}
}

void CodeGenerateAPIImpl::SetErrorCallback(FPtr p)
{
	this->p = p;
}

void CodeGenerateAPIImpl::initMap()
{
	BinaryOpCMD['+'] = 0x09;
	BinaryOpCMD['-'] = 0x0A;
	BinaryOpCMD['*'] = 0x0B;
	BinaryOpCMD['/'] = 0x0C;
	RightOpCMD['-'] = 0x07;
}

CodeGenerateAPIImpl::CodeGenerateAPIImpl()
{
	initMap();
}

CodeGenerateAPIImpl::CodeGenerateAPIImpl(MemoryInfo memory_info) :memory_info_(memory_info)
{
	initMap();
}

std::string CodeGenerateAPIImpl::GetVarAddr(char var)
{
	return memory_info_.VarNameToAddr.at(var);
}

std::string CodeGenerateAPIImpl::GetTempAddr()
{
	if(memory_info_.TempAddrs.empty())
	{
		p();
		return "";
	}
	std::string addr=*(memory_info_.TempAddrs.begin());
	memory_info_.TempAddrs.erase(memory_info_.TempAddrs.begin());
	return addr;
}

void CodeGenerateAPIImpl::FreeTempAddr(std::string addr)
{
	memory_info_.TempAddrs.insert(addr);
}

std::string CodeGenerateAPIImpl::GenerateCode(std::string str)
{
	SyntaxTree =static_cast<Tree*> (LEGOP(const_cast<char*>(str.c_str())));
	if (SyntaxTree==nullptr)
	{
		p();
		return "";
	}
	return GenerateCodeHelper(SyntaxTree->root);
}

