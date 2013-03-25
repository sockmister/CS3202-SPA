#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include <boost/tokenizer.hpp>
#include "AST.h"

using namespace std;
using namespace boost;

typedef string token;

class ASTBuilder{
private:
	string code;
	char_separator<char> sep;
	tokenizer<char_separator<char>> * tokens;
	tokenizer<char_separator<char>>::iterator tok_iter;
	int currStmtNumber; //dummy variable for AST::createTNode()
	
	//AST Component
	AST *ast;
	INDEX rootNode;

	//Methods for concrete syntax grammar
	bool name();
	bool integer();
	bool checkOperator(int);
	void error(token);
	int expr();
	int factor();
	int term();

	//Method to get token
	token nextToken();
	token consumeToken();

	//Method to match
	bool match(int);			
	bool match(token);

public:
	//Constuctor and Destructor
	ASTBuilder();
	ASTBuilder(AST *, string);
	~ASTBuilder();

	//void setupASTBuilder(AST *, string);
	bool convertToAST();
	//INDEX getRoot();
};
#endif