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
	currStmtNumber(0),
	outputVector(new vector<string>()),
	operatorStack(new vector<string>())
{}

ASTBuilder::ASTBuilder(AST * astPointer, std::string expression):
	code(expression),
	sep(" \n\t", "+-*;()", boost::drop_empty_tokens),
	tokens(new tokenizer<char_separator<char>>(code,sep)),
	tok_iter(tokens->begin()),
	currStmtNumber(0),
	outputVector(new vector<string>()),
	operatorStack(new vector<string>()),
	ast(astPointer)
{}

ASTBuilder::~ASTBuilder(){
	delete tokens;
	delete ast;
}

bool ASTBuilder::convertToAST(){

	this->toPostFix();
	rootNode = getRootOfExpr();
	outputVector->clear();

	if (rootNode < 0)
		return false;
	else
	{
		ast->setRootNode(rootNode);
		return true;
	}
}

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

bool ASTBuilder::toPostFix() //CONSTANT or VAR_NAME or OPEN_BRACKET or CLOSE_BRACKET expected as current token upon call to expr()
{
	bool leftOp, rightOp;
	string Operator;
	leftOp = factor();

	if (!leftOp)
		return false;

	if (nextToken() != ";") //Parse only if end-of-statement is not reached 
	{
		if (nextToken() == "+" || nextToken() == "-")
		{
			Operator = nextToken();
			consumeToken();
		
			if ( !operatorStack->empty() )
			{
				if ( operatorStack->back() != "(" )
				{
					outputVector->push_back( operatorStack->back() );
					operatorStack->pop_back();
				}
			}

			operatorStack->push_back(Operator);

			rightOp = toPostFix();
			return rightOp;
		}
	}

	if (nextToken() == "")
	{
		while ( !operatorStack->empty() )
		{
			Operator = operatorStack->back();
			operatorStack->pop_back();
			outputVector->push_back(Operator);
		}
	}

	return leftOp;
}

bool ASTBuilder::factor()
{
	bool leftOp, rightOp;
	string Operator;
	leftOp = term();

	if (!leftOp)
		return false;

	if (nextToken() != ";") //Parse only if end-of-statement is not reached 
	{
		if (nextToken() == "*")
		{
			Operator = nextToken();
			consumeToken();

			if ( !operatorStack->empty() )
			{
				if ( operatorStack->back() == "*")
					outputVector->push_back(Operator);
				else //for all other operators at top of stack, * has higher precedence
					operatorStack->push_back(Operator);
			}
			else //stack is empty
			{
				operatorStack->push_back(Operator);
			}

			rightOp = factor();
			return rightOp;
		}
	}

	return leftOp;
}

bool ASTBuilder::term()
{
	bool node;
	string Operator;

	if ( isdigit(nextToken().at(0)) ) //CONSTANT expected
	{
		string nextConst = nextToken();
		if (match(CONSTANT) == false)
			return false;
		else //We would have matched constant
		{
			//node = ast->createTNode("constantNode", currStmtNumber, nextConst);
			node = true;
			outputVector->push_back(nextConst);
			return node;
		}
	}
	else 
	{	//Variable-name or open bracket "(" or close bracket ")" is expected
		string next = nextToken();

		if (isalpha(next.at(0))) //VAR_NAME expected
		{
			if (match(VAR_NAME) == false)
				return false;
		}
		else if (next != "(" && next != ")") //Only OPEN_BRACKET OR CLOSE_BRACKET expected
			return false;

		//Here, we have matched VAR_NAME or OPEN_BRACKET or CLOSE_BRACKET
		if (next == "(")
		{
			consumeToken();
			operatorStack->push_back(next);
			node = toPostFix();
			consumeToken(); //Consume CLOSE_BRACKET token
			
			//CLOSE_BRACKET token detected, push all operators in stack ( until "(" ) to output
			while ( operatorStack->back() != "(" )
			{
				Operator = operatorStack->back();
				operatorStack->pop_back();
				outputVector->push_back(Operator);
			}

			operatorStack->pop_back(); //pop OPEN_BRACKET from stack
		}

		else //next contains VAR_NAME
		{
			node = true;
			outputVector->push_back(next);
		}

		return node;
	}
}

INDEX ASTBuilder::getRootOfExpr()
{
	vector<INDEX> buildStack;
	INDEX leftChild, rightChild, root;
	string Operator;
	
	for (int i=0; i<outputVector->size(); ++i)
	{
		if ( isdigit(outputVector->at(i).at(0)) ) //if token is a CONSTANT
		{
			buildStack.push_back( ast->createTNode("constantNode", currStmtNumber, outputVector->at(i)) );
		}
		else if ( isalpha(outputVector->at(i).at(0)) ) //if token is a VARIABLE
		{
			buildStack.push_back( ast->createTNode("varNode", currStmtNumber, outputVector->at(i)) );
		}
		else //token is an OPERATOR
		{
			Operator = outputVector->at(i);
			rightChild = buildStack.back();
			buildStack.pop_back();
			leftChild = buildStack.back();
			buildStack.pop_back();

			if (Operator == "+")
			{
				root = ast->createTNode("plusNode", currStmtNumber, "+");
			}
			else if (Operator == "-")
			{
				root = ast->createTNode("minusNode", currStmtNumber, "-");
			}
			else if (Operator == "*")
			{
				root = ast->createTNode("timesNode", currStmtNumber, "*");
			}

			ast->linkNode(ast->getNode(leftChild), ast->getNode(root), "children");
			ast->linkNode(ast->getNode(rightChild), ast->getNode(root), "children");

			buildStack.push_back(root);
		}
	}

	return buildStack.back();
}