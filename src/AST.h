#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include "TNode.h"

using namespace std;

/*!  \class AST.
	 \brief A tree representation of the abstract syntatic structure of source code
*/

typedef string NODE_TYPE;
typedef string LINK_TYPE;
typedef int INDEX;
typedef int STMT;


class AST {

public:
	enum Link_Type{parent, children, left, right};
	//! Default constuctor
	AST();
	
	//! Constructor
	AST(TNode root);

	//! set root node of AST
	void setRootNode(INDEX rootNode);

	//! get root node of AST
	INDEX getRoot();

	//! Create AST node with specified note type, statement number and node value
	/*!
		\param NODE_TYPE AST node type
		\param STMT statement number
		\param CONSTANT value of AST node
		\return a reference to the AST node
	*/
	INDEX createTNode(NODE_TYPE nodeType, STMT stmtNumber, CONSTANT nodeValue);

	//! Form a link with specified node type between nodes N1 and N2
	/*!
		n1 is <LinkType> of n2
		\param Tnode AST node
		\param LINK_TYPE left, right, parent, child
	*/
	void linkNode(TNode n1, TNode n2, LINK_TYPE LinkType);

	//! Get the first child of a given AST node
	/*!
		\parm Tnode AST node
		\parm int the position of child you want, eg: first, second, with first child at index 0
		\return AST node
	*/
	TNode getChild(TNode p, int whichChild);

	//! Get the right Sibling of a given AST node
	/*!
		\parm Tnode AST node
		\return AST node
	*/
	TNode getRightSibling(TNode p);

	//! Get the up link of a given AST node
	/*!
		\parm Tnode AST node
		\return AST node
	*/
	TNode getUpLink(TNode p);

	//! Get the AST node with given index
	/*!
		\parm INDEX index of AST node
		\return AST node
	*/
	TNode getNode(INDEX index);

	//! Get the AST node type 
	/*!
		\parm STMT statement number
		\return node type 
	*/
	
//	string getNodeType(TNode n);

	// New method for CFG
	string getNodeType(STMT stmtNumber);

	//! Get the number of child of an AST node 
	/*!
		\parm Tnode AST node
		\return number of children AST nodes
	*/
	int getNoOfChild(TNode n);

	//! Get the node value of an AST node 
	/*!
		\parm Tnode AST node
		\return node value
	*/	
	CONSTANT getNodeValue(TNode n);

	//! Get all stmt number for a given node type
	/*!
		\parm NODE_TYPE AST node type
		\return a list of statement number for a AST node type
	*/
	vector<int> getAllStmtNumByType(NODE_TYPE nodeType);

	//! Get all constants for answering constant queries
	/*!
		\return a list of constants in the AST
	*/
	vector<CONSTANT> getAllConstant();

	//! Check if an AST node matches a node type
	/*!
		\parm Tnode AST node
		\parm NODE_TYPE AST node type
		\return TRUE if AST node n matches nodeType
	*/
	bool isMatch(TNode n, NODE_TYPE nodeType);

	//! Check if the AST contains the pattern
	/*!
		\parm STMT statement number
		\parm int 0 for exact match, 1 for partial match
		\parm vector<TNode> the partial AST of the pattern
		\return TRUE if AST has a matching pattern
	*/
	bool findMatchingPattern(STMT stmtNumber, int order, AST * patternAST);

	//! Check if the two AST nodes are siblings
	/*!
		\parm Tnode AST node
		\parm Tnode AST node
		\return TRUE if tn1 and tn2 are siblings
	*/
	bool isSibling(TNode tn1, TNode tn2);



	/************************************
	*       Methods for future use 
	************************************/

	// Method to get an array of index for the given node type in the whole AST
	vector<int> getAllByType(NODE_TYPE nodeType);

	TNode getAssignmentNode(STMT stmtNumber);
	TNode getControlStmt(STMT stmtNumber);
	TNode getStmtNode(STMT stmtNumber);

	// Method to get the whole AST data structure
	vector<TNode> getASTVector();
	
private:
	// data structure of AST
	vector<TNode> tree;
	INDEX rootNode;

	// private method to calculate the index of the TNode in the AST
	INDEX indexOf(TNode n);

	// private method to comapare whether 2 TNodes are the same
	bool isMatchingRootNode(TNode n1, TNode n2, vector<TNode> patternAST);

	// private method to convert the link type
	int getLinkType(string linkType);

	// private methods to convert nodetype to integer and vice versa
	int convertNodeTypeStringToInteger(NODE_TYPE NodeType);
	string convertNodeTypeIntegerToString(int nodeType);

	// private method to get the stmt number of a node
	STMT stmtOf(TNode n);
};