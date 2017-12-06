#include "CodeGenerateAPIImpl.h"


std::string MemoryInfo::encode()
{
	std::stringstream ss;
	std::string str = "";
	int var_num = this->VarNameToAddr.size();
	ss << var_num;
	std::string var_num_c;
	ss >> var_num_c;
	ss.clear();
	str += var_num_c;
	str += " ";
	for (int i = 0; i < var_num; ++i)
	{
		char var_name = 'a' + i;
		str += this->VarNameToAddr[var_name];
		str += " ";
	}
	ss << this->TempAddrs.size();
	std::string temp_num_c;
	ss >> temp_num_c;
	str += temp_num_c;
	str += " ";
	for (auto i = this->TempAddrs.cbegin(); i != this->TempAddrs.cend();)
	{
		str += (*i);
		++i;
		if (i != this->TempAddrs.cend())
		{
			str += " ";
		}
	}
	return str;
}

void MemoryInfo::decode(int argc, char* argv[])
{
	int var_num = atoi(argv[2]);
	std::cout << var_num<< std::endl;
	for (int i = 0; i < var_num; ++i)
	{
		char var_name = 'a' + i;
		this->VarNameToAddr[var_name] = std::string(argv[i + 3]);
	}
	int temp_num = atoi(argv[3 + var_num]);
	std::cout << temp_num<<std::endl;
	for (int i = 0; i < temp_num; ++i)
	{
		this->TempAddrs.insert(std::string(argv[i + 3 + var_num]));
	}
}

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
		//常数节点，叶节点
		//申请临时空间并将数值赋值到此空间
		return_str += AssignFloat;
		TempAddr = GetTempAddr();
		return_str += TempAddr;
		node->node_info_.TempAddr.push_back(TempAddr);
		node->node_info_.NodeAddr = TempAddr;
		return_str += Float_convertbyte(dynamic_cast<Number*>(node)->Value);
		return return_str;
	case RIGHTOP:
		//子树深度优先
		return_str += GenerateCodeHelper(dynamic_cast<RightOperator*>(node)->Operand);
		return_str += MathCode;
		return_str += RightOpCMD.at(dynamic_cast<RightOperator*>(node)->Operate);
		return_str += dynamic_cast<RightOperator*>(node)->Operand->node_info_.NodeAddr;
		return_str += dynamic_cast<RightOperator*>(node)->Operand->node_info_.NodeAddr;
		TempAddr = dynamic_cast<RightOperator*>(node)->node_info_.NodeAddr;
		node->node_info_.TempAddr.push_back(TempAddr);
		node->node_info_.NodeAddr = TempAddr;
		return return_str;
	case NODETYPE::VAR:
		//变量节点，叶节点
		//申请临时空间并将变量值复制到此空间
		return_str += CopyFloat;
		TempAddr = GetTempAddr();
		return_str += TempAddr;
		node->node_info_.TempAddr.push_back(TempAddr);
		node->node_info_.NodeAddr = TempAddr;
		return_str += GetVarAddr(dynamic_cast<Var*>(node)->VarName);
		return return_str;
	case BINARYOP:
		//左、右子树深度优先，将结果保存在左操作数地址，右操作数地址释放
		return_str += GenerateCodeHelper(dynamic_cast<BinaryOperator*>(node)->LeftOperand);
		return_str += GenerateCodeHelper(dynamic_cast<BinaryOperator*>(node)->RightOperand);
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

CodeGenerateAPIImpl::
CodeGenerateAPIImpl(MemoryInfo memory_info) :
	memory_info_(memory_info)
{
	initMap();
}

std::string CodeGenerateAPIImpl::GetVarAddr(char var)
{
	return memory_info_.VarNameToAddr.at(var);
}

std::string CodeGenerateAPIImpl::GetTempAddr()
{
	if (memory_info_.TempAddrs.empty())
	{
		p();
		return "";
	}
	std::string addr = *(memory_info_.TempAddrs.begin());
	memory_info_.TempAddrs.erase(memory_info_.TempAddrs.begin());
	return addr;
}

void CodeGenerateAPIImpl::FreeTempAddr(std::string addr)
{
	memory_info_.TempAddrs.insert(addr);
}

std::string CodeGenerateAPIImpl::GenerateCode(std::string str)
{
	//构建语法树
	SyntaxTree = static_cast<Tree*>(LEGOP(const_cast<char*>(str.c_str())));
	if (SyntaxTree == nullptr)
	{
		p();
		return "";
	}
	std::string return_str = "";
	//深度遍历语法树
	return_str += GenerateCodeHelper(SyntaxTree->root);
	//获取根节点地址，复制结果
	return_str += CopyFloat;
	return_str += ResultAddr;
	return_str += SyntaxTree->root->node_info_.NodeAddr;
	return return_str;
}

std::string CodeGenerateAPIImpl::GenerateCodeByProxy(std::string str, const char* file_path, const char* temp_file_path)
{
	std::string str_param = str + " " + memory_info_.encode() + " " + temp_file_path;
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = file_path;
	ShExecInfo.lpParameters = str_param.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	std::ifstream temp_file(temp_file_path);
	std::string out_str((std::istreambuf_iterator<char>(temp_file)),
	                    std::istreambuf_iterator<char>());
	return out_str;
}
