enum NODETYPE
{
	NUMBER=0,
	VAR,
	BINARYOP,
	RIGHTOP
};
class Node{
public:
	NODETYPE NodeType;

};
class Number :Node{
public:
	double Value;
	Number(double value);
	Number();
};
class Var :Node{
public:
	char VarName;
	Var(char varName);
};
class BinaryOperator{
public:
	Node* LeftOperand;
	Node* RightOperand;
	char Operate;
	BinaryOperator(Node* left, Node* right, char operate);
};
class RightOperator :Node{
public:
	Node* Operater;
	char Operate;
};
class Tree{
private:
	Node* root;
public:
	Tree(Node* node);
	void Clear();
};