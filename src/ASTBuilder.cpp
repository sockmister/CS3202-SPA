#include "ASTBuilder.h"
#include "AST.h"

#define VAR_NAME 1
#define CONSTANT 2
#define PLUS 3
#define MINUS 4
#define TIMES 5
#define OPEN_BRACKET 6
#define CLOSE_BRACKET 7

ASTBuilder::ASTBuilder():
	sep(" \n\t", "+-*;()", boost::drop_empty_tokens),
	tokens(new tokenizer<char_separator<char>>(code,sep)),
	tok_iter(tokens->begin()),
	currStmtNumber(0)
{}

ASTBuilder::ASTBuilder(AST * astPointer, std::string expression):
	code(expression),
	sep(" \n\t", "+-*;()", boost::drop_empty_tokens),
	tokens(new tokenizer<char_separator<char>>(code,sep)),
	tok_iter(tokens->begin()),
	currStmtNumber(0),
	ast(astPointer)
{}

ASTBuilder::~ASTBuilder(){
	delete tokens;
	delete ast;
}

/*
void ASTBuilder::setupASTBuilder(AST* astPointer, string expression){
	this->ast = astPointer;
	this->code= expression;
}
*/

bool ASTBuilder::convertToAST(){
	rootNode = expr();

	if (rootNode < 0)
		return false;
	else
	{
		ast->setRootNode(rootNode);
		return true;
	}
}

/*
INDEX ASTBuilder::getRoot(){
	return rootNode;
}
*/

token ASTBuilder::nextToken(){
	if(tok_iter != tokens->end())
		return *tok_iter;
	else
		return "";
}

//Precondition: tokens is not empty
token ASTBuilder::consumeToken(){
	cout <<*tok_iter << endl; 
	return *tok_iter++;
}

bool ASTBuilder::name(){
	string next = nextToken();
	if(isalpha(next[0]) == false){
		return false;
	}
	for(int i = 1; i < next.size(); i++)
		if(isalnum(next[i])== false){
			return false;
		}

		return true;
}

bool ASTBuilder::integer(){
	string next = nextToken();
	for(int i = 0; i < next.size(); i++)
		if(isdigit(next[i]) == false){
			return false;
		}

		return true;
}

bool ASTBuilder::checkOperator(int construct)
{
	string opr = nextToken();
	//cout << "in CheckOperator(PLUS): opr = "<< opr <<endl;

	switch (construct)
	{
	case PLUS:
		if (opr == "+")
			return true;
		else
			return false;
		break;
	case MINUS:
		if (opr == "-")
			return true;
		else
			return false;
		break;
	case TIMES:
		if (opr == "*")
			return true;
		else
			return false;
		break;
	case OPEN_BRACKET:
		if (opr == "(")
			return true;
		else
			return false;
		break;
	case CLOSE_BRACKET:
		if (opr == ")")
			return true;
		else
			return false;
		break;
	}
}

void ASTBuilder::error(token error){
	cout << "Error: " << error << endl;
}

bool ASTBuilder::match(int construct){
	if (nextToken() == "") //end of code reached unexpectedly
	{
		error("End of code reached unexpectedly");
		return false;
	}

	bool matched = false;
	switch(construct){
	case VAR_NAME:
		//cout << "check_VAR_NAME: " << nextToken() << endl;
		matched = name();
		break;
	case CONSTANT:
		//cout << "check_CONSTANT: " << nextToken() << endl;
		matched = integer();
		break;
	case PLUS:
		//cout << "check_PLUS: " << nextToken() <<endl;
		matched = checkOperator(PLUS);
		break;
	case MINUS:
		//cout << "check_MINUS: " << nextToken() <<endl;
		matched = checkOperator(MINUS);
		break;
	case TIMES:
		//cout << "check_TIMES: " << nextToken() <<endl;
		matched = checkOperator(TIMES);
		break;
	case OPEN_BRACKET:
		//cout << "check_OPEN_BRACKET: " << nextToken() <<endl;
		matched = checkOperator(OPEN_BRACKET);
		break;
	case CLOSE_BRACKET:
		//cout << "check_CLOSE_BRACKET: " << nextToken() <<endl;
		matched = checkOperator(CLOSE_BRACKET);
		break;
	}
	if(matched){
		consumeToken();
		return true;
	}
	else{
		error(nextToken());
		return false;
	}
}

int ASTBuilder::expr() //CONSTANT or VAR_NAME or OPEN_BRACKET or CLOSE_BRACKET expected as current token upon call to expr()
{
	INDEX node, leftChild, rightChild;
	leftChild = factor();

	if (leftChild < 0)
		return -1;

	if (nextToken() != ";")
	{
		if (nextToken() == "+")
		{
			consumeToken();
			node = ast->createTNode("plusNode", currStmtNumber, "");
			ast->linkNode(ast->getNode(leftChild), ast->getNode(node), "children");
			rightChild = expr();
			if (rightChild < 0)
				return -1;
			else
			{
				ast->linkNode(ast->getNode(rightChild), ast->getNode(node), "children");
				return node;
			}
		}
		else if (nextToken() == "-")
		{
			consumeToken();
			node = ast->createTNode("minusNode", currStmtNumber, "");
			ast->linkNode(ast->getNode(leftChild), ast->getNode(node), "children");
			rightChild = expr();
			if (rightChild < 0)
				return -1;
			else
			{
				ast->linkNode(ast->getNode(rightChild), ast->getNode(node), "children");
				return node;
			}
		}
	}

	return leftChild;
}

int ASTBuilder::factor()
{
	INDEX node, leftChild, rightChild;
	leftChild = term();

	if (leftChild < 0)
		return -1;

	if (nextToken() != ";") //Parse only if end-of-statement is not reached 
	{
		if (nextToken() == "*")
		{
			consumeToken();
			node = ast->createTNode("timesNode", currStmtNumber, "");
			ast->linkNode(ast->getNode(leftChild), ast->getNode(node), "children");
			rightChild = factor();
			if (rightChild < 0)
				return -1;
			else
			{
				ast->linkNode(ast->getNode(rightChild), ast->getNode(node), "children");
				return node;
			}
		}
	}

	return leftChild;
}

int ASTBuilder::term()
{
	INDEX node;

	if ( isdigit(nextToken().at(0)) ) //CONSTANT expected
	{
		string nextConst = nextToken();
		if (match(CONSTANT) == false)
			return -1;
		else //We would have matched constant
		{
			node = ast->createTNode("constantNode", currStmtNumber, nextConst);
			return node;
		}
	}
	else 
	{ //Variable-name or open bracket "(" or close bracket ")" is expected
		string next = nextToken();

		if (isalpha(next.at(0))) //VAR_NAME expected
		{
			if (match(VAR_NAME) == false)
				return -1;
		}
		else if (next != "(" && next != ")") //Only OPEN_BRACKET OR CLOSE_BRACKET expected
			return -1;

		//Here, we have matched VAR_NAME or OPEN_BRACKET or CLOSE_BRACKET
		if (next == "(")
		{
			consumeToken();
			node = expr();
			consumeToken(); //Consume CLOSE_BRACKET token
		}
		else //next contains VAR_NAME
		{
			//varTable->insertVar(next);
			node = ast->createTNode("varNode", currStmtNumber, next);
			STMT varStmtNum = ast->getNode(node).getStmtNumber();
		}

		return node;
	}
}