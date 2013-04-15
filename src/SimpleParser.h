#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <algorithm>
#include "AST.h"
#include "VarTable.h"
#include "Follows.h"
#include "Parent.h"
#include "Uses.h"
#include "Modifies.h"
#include "PKB.h"
#include "rootWhile.h"
#include "rootIf.h"
#include "StmtTable.h"

using namespace boost;

typedef std::string token;

class SimpleParser{
private:
	std::string code;
	char_separator<char> sep;
	tokenizer<char_separator<char>> * tokens;
	tokenizer<char_separator<char>>::iterator tok_iter;
	bool legal;
	int currStmtNumber;
	string proc;
	int whileLevel;
	int rootWhileStmtNum;
	vector<int> rootIfElseList;
	vector<int> rootOnlyIfList;
	int thenOrElse; //outside of IF completely = 0, in THEN or in ELSE currently then > 0. 
	vector<string> * outputVector;
	vector<string> * operatorStack;
	string RHSExpr;
	vector<string> procConst;
	vector<string> procVar;

	//PKB Components
	VarTable * varTable;
	AST * ast;
	Follows * follows;
	Parent * parent;
	Uses * uses;
	Modifies * modifies;
	ProcTable * procTable;
	Calls * calls;
	rootWhile * storeRootWhile;
	rootIf * storeRootIf;
	StmtTable * stmtTable;

	//Methods for concrete syntax grammar
	INDEX program();
	INDEX procedure();
	INDEX stmtLst();
	INDEX stmt();
	bool proc_name();
	bool var_name();
	bool constant();
	bool name();
	bool integer();
	bool checkOperator(int);
	void error(token);
	bool toPostFix();
	bool factor();
	bool term();
	INDEX getRootOfExpr();

	//Method to get token
	token nextToken();
	token consumeToken();

	//Method to match
	bool match(int);			
	bool match(token);

	string getCodeFromFile(ifstream *);

public:
	//Constructor, Destructor
	SimpleParser(ifstream *, PKB *);
	SimpleParser(std::string code, PKB *); //used for testing
	~SimpleParser();

	int getTotalStmtNumber();
	AST * getParserAST();
	
	//void displayAST();

	//PKB Methods
	void initAST(AST *);
	void initVarTable(VarTable *);
	void initFollows(Follows *);
	void initParent(Parent *);
	void initUses(Uses *);
	void initModifies(Modifies *);

	//Method to parse
	bool run();
};

#endif