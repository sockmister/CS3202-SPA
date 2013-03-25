#pragma once
//#include "PKB.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>


using namespace std;
typedef int STMT;
typedef vector<STMT> STMTLST;

class CFG
{
public:
	//! Default constuctor
	CFG();
	//! Destructor
	~CFG();
	//! Constructor
	CFG(vector<STMTLST>, vector<STMTLST>);

	//void buildCFG(STMT start, STMT end);

   //! If order = 0, return statements that directly precedes programLine ,  If order = 1, return statements that appears directly after programLine
   /*!
	 \param order 0 or 1
	 \parm STMT program line number
	 \return otherwise empty STMTLST
   */
	STMTLST nextStatement (int order, STMT programLine);

   //! If order = 0, return statements that precedes programLine ,  If order = 1, return statements that appears after programLine
   /*!
	 \param order 0 or 1
	 \parm STMT program line number
	 \return otherwise empty STMTLST
   */
	STMTLST nextStatementStar (int order, STMT programLine);


   //! Check if n2 appears directly after n1
   /*!
	 \parm STMT program line number 1
	 \parm STMT program line number 2
	 \return TRUE if n2 appears directly after n1, otherwise return FALSE
   */
	bool isNext(STMT n1, STMT n2);

	//! Check if there is a control flow path from n1 to n2
   /*!
	 \parm STMT program line number 1
	 \parm STMT program line number 2
	 \return TRUE if there is a control flow path from n1 to n2, otherwise return FALSE
   */
	bool isNextStar(STMT n1, STMT n2);

	STMTLST affectsNext(STMT n3, STMT n4);

	// For testing
	vector<STMTLST> getCFG();
	vector<STMTLST> getBlock();
private:
	vector<STMTLST> Graph;
	vector<STMTLST> Block;
	vector<bool> visited;
	bool skip, visitStartBlock, visitEndBlock;
	int originalStartBlock;
	STMTLST affectsNextDFS;

	//AST* ast;
	//Follows* follows;
	//Parent* parent;

	vector<bool> DFS(STMT programLine); // Depth first search
	void affectsDFS(int,int); // modified depth first search for affects
	STMT getBlock(STMT statementNumber);
	STMTLST nextBlock(int order, STMT blockNumber);
};
