enum NODETYPE
{
	NUMBER=0,
	VAR,
	BINARYOP,
	RIGHTOP
};
class Node{
public:
	void Clear();
	virtual void VirtualFun();
	void DeepTreval(void(*Fun)(Node*));
	NODETYPE NodeType;
	void DeepTreval(void* Fun);
	Node(NODETYPE nodetype);
};
class Number :public Node{
public:
	double Value;
	Number(double value);
	Number();
};
class Var :public Node{
public:
	char VarName;
	Var(char varName);
	Var();
};
class BinaryOperator :public Node{
public:
	Node* LeftOperand;
	Node* RightOperand;
	char Operate;
	BinaryOperator(Node* left, Node* right, char operate);
	BinaryOperator();
};
class RightOperator :public Node{
public:
	Node* Operand;
	char Operate;
	RightOperator();
	RightOperator(Node* par_Operator,char operate);
};
class Tree{
public:
	Node* root;
	Tree(Node* node);
	void Clear();
};