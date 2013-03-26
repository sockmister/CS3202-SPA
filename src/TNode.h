#pragma once

#include <vector>
#include <string>

using namespace std;

typedef int node_TYPE;
typedef int STMT;
typedef string CONSTANT;

class TNode {

public:
	//enum for nodeType
//	static enum node_TYPE{programNode, procedureNode, stmtLstNode, assignNode, whileNode, varNode, plusNode, constNode, end};
	
	static TNode* ASTroot;

	// default constructor
	TNode();

	// constructor
	TNode(node_TYPE nodeType, STMT); 

	node_TYPE getNodeType();
	int getIndex();
	int getStmtNumber();
	CONSTANT getNodeValue();
	int getParent();
	int getLeftSibling();
	int getRightSibling();
	vector<int> getChildren();
	
	void setIndex(int);
	void setStmtNumber(int n);
	void setNodeValue(string v);
	void setParent(int p);
	void setLeftSibling(int l);
	void setRightSibling(int r);
	void addChild(int c);


private:
	node_TYPE nodeType; 
	int index;     // index in the AST
	int stmtNumber;
	CONSTANT nodeValue; // If node is a variable, nodeValue will be the index of the variable in varTable
				   // If node is a constant, nodeValue will be the constant value
	               // Else nodeValue will be -1
	// Links of a TNode
	int parent;
	int leftSibling;
	int rightSibling;
	vector<int> children;

};