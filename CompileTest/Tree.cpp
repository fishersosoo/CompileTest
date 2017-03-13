#include "Tree.h"

void Node::Clear()
{
	switch (NodeType)
	{
	case NUMBER:
		delete (this);
		return;
		break;
	case RIGHTOP:
		dynamic_cast<RightOperator*>(this)->Operand->Clear();
		delete (this);
		return;
		break;
	case VAR:
		delete (this);
		return;
		break;
	case BINARYOP:
		dynamic_cast<BinaryOperator*>(this)->LeftOperand->Clear();
		dynamic_cast<BinaryOperator*>(this)->RightOperand->Clear();
		delete (this);
		return;
		break;
	default:
		delete (this);
		return;
	}
}

void Node::VirtualFun()
{
	
}


void Node::DeepTreval(void(* Fun)(Node*))
{
	switch (NodeType)
	{
	case NUMBER:
		(*Fun)(this);
		break;
	case RIGHTOP:
		dynamic_cast<RightOperator*>(this)->Operand->DeepTreval(Fun);
		break;
	case VAR:
		(*Fun)(this);
		break;
	case BINARYOP:
		dynamic_cast<BinaryOperator*>(this)->LeftOperand->DeepTreval(Fun);
		break;
	default:
		(*Fun)(this);
	}
	(*Fun)(this);
}

Node::Node(NODETYPE nodetype)
{
	NodeType = nodetype;
}

Number::Number() :Node(NUMBER)
{
}

Number::Number(double value) : Node(NUMBER), Value(value)
{
}

Var::Var(char varName) : Node(VAR), VarName(varName)
{
}

Var::Var() : Node(VAR)
{
}

BinaryOperator::BinaryOperator(Node* left, Node* right, char operate) :
Node(BINARYOP), LeftOperand(left), RightOperand(right), Operate(operate)
{
}

BinaryOperator::BinaryOperator() : Node(BINARYOP)
{
	
}

RightOperator::RightOperator() : Node(RIGHTOP)
{
	
}

RightOperator::RightOperator(Node* par_Operator, char operate) : Node(RIGHTOP), Operate(operate), Operand(par_Operator)
{
	
}

Tree::Tree(Node* node)
{
	root = node;
}

void Tree::Clear()
{
	root->Clear();
}




