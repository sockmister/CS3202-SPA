#pragma once

#include "AST.h"
#include <stack>

// Default constructor
AST::AST(){
}

AST::AST(TNode root) {
	tree.insert(tree.begin(), root);
}

void AST::setRootNode(INDEX root) {
	this->rootNode = root;
}

INDEX AST::getRoot() {
	return rootNode;
}

INDEX AST::createTNode(NODE_TYPE NodeType, STMT stmtNumber, CONSTANT nodeValue) {
	int nodeType = convertNodeTypeStringToInteger(NodeType);
	TNode newNode (nodeType, stmtNumber);
	newNode.setIndex(tree.size());

	if (nodeType == 5 || nodeType == 7 || nodeType == 11)
		newNode.setNodeValue(nodeValue);

	tree.push_back(newNode);

	return newNode.getIndex();
}

void AST::linkNode(TNode n1, TNode n2, string LinkType) {
	int n1Index = indexOf(n1);
	int n2Index = indexOf(n2);

	switch (getLinkType(LinkType)) {
	case AST::parent:
		tree.at(n1Index).addChild(n2Index);
		tree.at(n2Index).setParent(n1Index);
		break;
	case AST::children:
		tree.at(n2Index).addChild(n1Index);
		tree.at(n1Index).setParent(n2Index);
		break;
	case AST::left:
		tree.at(n1Index).setRightSibling(n2Index);
		tree.at(n2Index).setLeftSibling(n1Index);
		break;
	case AST::right:
		tree.at(n2Index).setRightSibling(n1Index);
		tree.at(n1Index).setLeftSibling(n2Index);
		break;
	}
}


TNode AST::getChild(TNode p, int index) {

	if ( p.getChildren().size()>0 )
		if (index < p.getChildren().size())
		return tree.at(p.getChildren().at(index));
	else {
		TNode newNode(-1 , -1);
		return newNode; 
	}
}

TNode AST::getRightSibling(TNode p) {
	return tree.at(p.getRightSibling());
}

TNode AST::getUpLink(TNode p) {
	return tree.at(p.getParent());
}

TNode AST::getNode(INDEX index) {
	return tree.at(index);
}

/*
string AST::getNodeType(TNode n) {
	return convertNodeTypeIntegerToString(n.getNodeType());
}
*/

// new Method for CFG
string AST::getNodeType(STMT stmtNumber) {
	vector<TNode>::iterator it;
	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getStmtNumber() == stmtNumber) {
			if ( it->getNodeType() == convertNodeTypeStringToInteger("whileNode") )
				return "whileNode";
			if ( it->getNodeType() == convertNodeTypeStringToInteger("ifNode") )
				return "ifNode";
			if (it->getNodeType() == convertNodeTypeStringToInteger("assignNode") )
				return "assignNode";
			if (it->getNodeType() == convertNodeTypeStringToInteger("callNode") )
				return "callNode";
		}
	}
	
	return "";
}

int AST::getNoOfChild(TNode n) {
	return n.getChildren().size();
}

CONSTANT AST::getNodeValue(TNode n) {
	return n.getNodeValue();
}


vector<int> AST::getAllStmtNumByType(NODE_TYPE nodeType) {
	vector<int> stmtNumTree;
	vector<TNode>::iterator it;
	int nodeTypeInteger = convertNodeTypeStringToInteger(nodeType);
	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getNodeType() == nodeTypeInteger)
			stmtNumTree.push_back(stmtOf(*it));
	}
	return stmtNumTree;
}

vector<CONSTANT> AST::getAllConstant() {
	vector<CONSTANT> constantList;
	vector<TNode>::iterator it;

	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getNodeType() == 7)
			constantList.push_back(getNodeValue(*it));
	}

	return constantList;
}

bool AST::isMatch(TNode n, NODE_TYPE nodeType) {
	if (n.getNodeType() == convertNodeTypeStringToInteger(nodeType))
		return true;
	else
		return false;
}

bool AST::findMatchingPattern(STMT stmtNumber, ORDER order, AST * paternAST) {
	bool match = true, child = true, right = false;
	vector<TNode> patternAST = paternAST->getASTVector();

	TNode assignNode = getAssignmentNode(stmtNumber);
	
	TNode leftVariable = getChild(assignNode,0);
	
	//int start = leftVariable.getRightSibling();
	int start = assignNode.getIndex() + 1;

	if (order == 0) {
		for (int i=0;i<patternAST.size();i++) {
			if (start<tree.size()) {
				int patternNodeType = patternAST[i].getNodeType();
				int ASTNodeType = tree[start].getNodeType();	
			
				if (patternNodeType != ASTNodeType) {
					match = false;
					break;
				}
				else if (patternNodeType == ASTNodeType) {
					if ( patternNodeType == 5 || patternNodeType == 7) {
						if (patternAST[i].getNodeValue() != tree[start].getNodeValue()) {
							match = false;
							break;
						}
					}
				}
				start++;
			}
		}

		if (match == false)
			return false;
		else 
			return true;
	}


	if (order == 1) {
		TNode startOfASTExpression = getChild(assignNode,1);
		TNode patternRoot = patternAST.at((paternAST->getRoot()));
		stack<TNode> ASTnodeStack;
		stack<TNode> patternASTnodeStack;
		
		bool found = false;

		if ( isMatchingRootNode(startOfASTExpression,patternRoot, patternAST) ==  false ) {

			while (found == false && (tree[start].getStmtNumber() == stmtNumber) ) {
				TNode ASTNode = tree[start];
				if (isMatchingRootNode(ASTNode, patternRoot,patternAST) == true) {
					found = true;
					break;
				}
				start++;
			}
		}
		else if (isMatchingRootNode(startOfASTExpression,patternRoot, patternAST) ==  true) {
			found = true;
		}

		if (found == false) 
			return false;
		
		startOfASTExpression = tree[start];

		ASTnodeStack.push(startOfASTExpression);
		patternASTnodeStack.push(patternRoot);

		// pre-order traversal
		while (patternASTnodeStack.empty() == false) {
			
			TNode patternNode = patternASTnodeStack.top();
			TNode ASTNode = ASTnodeStack.top();

			if ( isMatchingRootNode(ASTNode,patternNode, patternAST) ==  false ) 
				return false;

			patternASTnodeStack.pop();
			ASTnodeStack.pop();

			// push right and left children into stack
			if (patternNode.getChildren().size() == ASTNode.getChildren().size()) {
				 if (patternNode.getChildren().size() == 1) {
					patternASTnodeStack.push(getChild(patternNode,0));
					ASTnodeStack.push(getChild(ASTNode,0));
				 }
				 else if (patternNode.getChildren().size() == 2) {
					patternASTnodeStack.push(paternAST->getChild(patternNode,0));
					ASTnodeStack.push(getChild(ASTNode,0));
					patternASTnodeStack.push(paternAST->getChild(patternNode,1));
					ASTnodeStack.push(getChild(ASTNode,1));
				 }
			} 
			else 
				return false;
		}
		
		return true;

	
	}

	/*
	if (order == 1) {
		TNode patternRoot = patternAST[0];
		
		bool found = false;
		while (found == false && (tree[start].getStmtNumber() == stmtNumber) ) {
			TNode ASTNode = tree[start];
			if (isSameNode(ASTNode, patternRoot) == true) {
				found = true;
				break;
			}
			start++;
		}

		if (found == false) 
			return false;

		for (int i=0;i<patternAST.size();i++) {
			if (start<tree.size()) {
				int patternNodeType = patternAST[i].getNodeType();
				int ASTNodeType = tree[start].getNodeType();	
			
				if (patternNodeType != ASTNodeType) {
					match = false;
					break;
				}
				else if (patternNodeType == ASTNodeType) {
					if ( patternNodeType == 5 || patternNodeType == 7) {
						if (patternAST[i].getNodeValue() != tree[start].getNodeValue()) {
							match = false;
							break;
						}
					}
				}
				start++;
			}
		}

		if (match == false)
			return false;
		else 
			return true;
	}
	*/

	return false;

}

bool AST::isSibling(TNode tn1, TNode tn2) {
	if (tn1.getParent() == tn2.getParent() )
		return true;
	else
		return false;
}


bool AST::isContains(TNode tn1, TNode tn2) {
	vector<int> children = tn1.getChildren();
	int indexOftn2 = tn2.getIndex();

	for (int i=0;i<children.size();i++) {
		if ( children[i] == indexOftn2 )
			return true;
	}

	return false;
}


bool AST::isContainsStar(TNode tn1, TNode tn2) {
	int parent = tn2.getParent();

	do {
		if (parent == tn1.getIndex())
			return true;
		else if (parent != -1)
			parent = this->tree[parent].getParent();

	} while (parent != this->rootNode || parent !=-1);

	return false;
}


/************************************
*       Methods for future use 
************************************/

vector<int> AST::getAllByType(NODE_TYPE nodeType) {
	vector<int> indexInTree;
	vector<TNode>::iterator it;

	int nodeTypeInteger = convertNodeTypeStringToInteger(nodeType);

	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getNodeType() == nodeTypeInteger)
			indexInTree.push_back(indexOf(*it));
	}

	return indexInTree;
}


TNode AST::getAssignmentNode(STMT stmtNumber) {
	vector<TNode>::iterator it;

	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getStmtNumber() == stmtNumber && (it->getNodeType() == 3 ))
			return *it;
	}

	TNode blank;
	return blank;
}

TNode AST::getStmtNode(STMT stmtNumber) {
	vector<TNode>::iterator it;

	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getStmtNumber() == stmtNumber && (it->getNodeType() == 3 || it->getNodeType() == 4 || it->getNodeType()==10 || it->getNodeType()==11))
			return *it;
	}

	TNode blank;
	return blank;
}

TNode AST::getControlStmt(STMT stmtNumber) {
	vector<TNode>::iterator it;

	for (it = tree.begin() ; it!=tree.end() ; it++) {
		if (it->getStmtNumber() == stmtNumber &&  it->getNodeType() == 4 )
			return *it;
	}

	TNode blank;
	return blank;
}

vector<TNode> AST::getASTVector(){
	return this->tree;
}

/**********************************************
*			Private Methods
**********************************************/

// private method to calculate the index of the TNode in the tree
INDEX AST::indexOf(TNode n) {
	return n.getIndex();
}

bool AST::isMatchingRootNode(TNode n1, TNode n2, vector<TNode> patternAST) {
	if (n1.getNodeType() == n2.getNodeType() && n1.getNodeValue() == n2.getNodeValue() ) {
		if (n1.getChildren().size() == n2.getChildren().size() && n2.getChildren().size() > 0) {
			if (n1.getChildren().size() == 1) {
				if (tree.at(n1.getChildren().at(0)).getNodeValue() == patternAST.at(n2.getChildren().at(0)).getNodeValue() 
						&& tree.at(n1.getChildren().at(0)).getNodeType() == patternAST.at(n2.getChildren().at(0)).getNodeType() )
					return true;
			}
			else if (n1.getChildren().size() > 1) {	
				if (tree.at(n1.getChildren().at(0)).getNodeValue() == patternAST.at(n2.getChildren().at(0)).getNodeValue() 
						&& tree.at(n1.getChildren().at(0)).getNodeType() == patternAST.at(n2.getChildren().at(0)).getNodeType()
						&& tree.at(n1.getChildren().at(1)).getNodeValue() == patternAST.at(n2.getChildren().at(1)).getNodeValue()  
						&& tree.at(n1.getChildren().at(1)).getNodeType() == patternAST.at(n2.getChildren().at(1)).getNodeType() )
					
						return true;
			}
		} 
		else if (n1.getChildren().size() == n2.getChildren().size())
			return true;
		else 
			return false;
	}
	else
		return false;
}

// private method to convert the link type from string to integer
int AST::getLinkType(string linkTypestring) {
	if (linkTypestring == "parent")
		return AST::parent;

	if (linkTypestring == "children")
		return AST::children;

	if (linkTypestring == "left")
		return AST::left;

	if (linkTypestring == "right")
		return AST::right;

	return -1;
}


//private methods to convert nodetype to integer and vice versa
int AST::convertNodeTypeStringToInteger(NODE_TYPE NodeType) {
	const char *myNodeType = NodeType.c_str();
	int nodeType = -1;

	if ( strcmp(myNodeType, "programNode") == 0)
		nodeType = 0;

	if ( strcmp(myNodeType, "procedureNode") == 0)
		nodeType = 1;

	if ( strcmp(myNodeType, "stmtLstNode") == 0)
		nodeType = 2;

	if ( strcmp(myNodeType, "assignNode") == 0)
		nodeType = 3;

	if ( strcmp(myNodeType, "whileNode") == 0)
		nodeType = 4;

	if ( strcmp(myNodeType, "varNode") == 0)
		nodeType = 5;

	if ( strcmp(myNodeType, "plusNode") == 0)
		nodeType = 6;

	if ( strcmp(myNodeType, "constantNode") == 0)
		nodeType = 7;

	if ( strcmp(myNodeType, "minusNode") == 0)
		nodeType = 8;

	if ( strcmp(myNodeType, "timesNode") == 0)
		nodeType = 9;

	if ( strcmp(myNodeType, "ifNode") == 0)
		nodeType = 10;

	if ( strcmp(myNodeType, "callNode") == 0)
		nodeType = 11;

	return nodeType;
}

string AST::convertNodeTypeIntegerToString(int nodeType) {
	switch (nodeType) {
	case 0: 
		return "programNode";
		break;
	case 1:
		return "procedureNode";
		break;
	case 2:
		return "stmtLstNode";
		break;
	case 3: 
		return "assignNode";
		break;
	case 4:
		return "whileNode";
		break;
	case 5:
		return "varNode";
		break;
	case 6:
		return "plusNode";
		break;
	case 7: 
		return "constantNode";
		break;
	case 8:
		return "minusNode";
		break;
	case 9:
		return "timesNode";
		break;
	case 10:
		return "ifNode";
		break;
	case 11:
		return "callNode";
		break;
	}
}

STMT AST::stmtOf(TNode n) {
	return n.getStmtNumber();
}


