#include "SimpleParser.h"
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <boost/tokenizer.hpp>

#define PROC_NAME 0
#define VAR_NAME 1
#define CONSTANT 2
#define PLUS 3
#define MINUS 4
#define TIMES 5
#define OPEN_BRACKET 6
#define CLOSE_BRACKET 7

using namespace boost;
using namespace std;

SimpleParser::SimpleParser(ifstream * filestream, PKB * pkb){
	code = getCodeFromFile(filestream);
	sep = char_separator<char> (" \n\t", "=+-*;(){}", boost::drop_empty_tokens);
	tokens = new tokenizer<char_separator<char>> (code, sep);
	tok_iter = tokens->begin();
	legal = true;
	currStmtNumber = 0;
	whileLevel = 0;
	rootWhileStmtNum = 0;
	storeRootWhile = pkb->getRootWhile();
	storeRootIf = pkb->getRootIf();
	thenOrElse = 0;
	ast = pkb->getAST();
	modifies = pkb->getModifies();
	follows = pkb->getFollows();
	parent = pkb->getParent();
	uses = pkb->getUses();
	varTable = pkb->getVarTable();
	procTable = pkb->getProcTable();
	calls = pkb->getCalls();
}


SimpleParser::SimpleParser(string inCode, PKB * pkb):
	code(inCode),
	sep(" \n\t", "=+-*;(){}", boost::drop_empty_tokens),
	tokens(new tokenizer<char_separator<char>>(code,sep)),
	tok_iter(tokens->begin()),
	legal(true),
	currStmtNumber(0),
	whileLevel(0),
	rootWhileStmtNum(0),
	storeRootWhile(pkb->getRootWhile()),
	storeRootIf(pkb->getRootIf()),
	thenOrElse(0),
	ast(pkb->getAST()),
	modifies(pkb->getModifies()),
	follows(pkb->getFollows()),
	parent(pkb->getParent()),
	uses(pkb->getUses()),
	varTable(pkb->getVarTable()),
	procTable(pkb->getProcTable()),
	calls(pkb->getCalls())
{}


SimpleParser::~SimpleParser(){
	delete tokens;
}

string SimpleParser::getCodeFromFile(ifstream * file){
	string line, code = "";
	if(file->is_open()){
		while(file->good()){
			getline(*file,line);
			code.append(line + "\n");
		}
		file->close();
	}

	return code;
}

//return AST
bool SimpleParser::run(){
	INDEX progNode = program();

	if (progNode < 0)
	{
		legal = false;
	}
	return legal;
}

token SimpleParser::nextToken(){
	if(tok_iter != tokens->end())
		return *tok_iter;
	else
		return "";
}

//Precondition: tokens is not empty
token SimpleParser::consumeToken(){
	cout <<*tok_iter << endl; 
	return *tok_iter++;
}

bool SimpleParser::match(token tok){
	if(nextToken() == tok){
		consumeToken();
		return true;
	}
	else{
		error(nextToken());
		return false;
	}
}

bool SimpleParser::match(int construct){
	if (nextToken() == "") //end of code reached unexpectedly
	{
		error("End of code reached unexpectedly");
		return false;
	}

	bool matched = false;
	switch(construct){
	case PROC_NAME:
		//cout << "check_PROC_NAME: " << nextToken() << endl;
		matched = name();
		break;
	case VAR_NAME:
		//cout << "check_VAR_NAME: " << nextToken() << endl;
		if(matched = name())
			varTable->insertVar(nextToken());
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
		//legal = true;
		return true;
	}
	else{
		error(nextToken());
		return false;
	}
}

INDEX SimpleParser::program(){ //Supports multiple procedures
	INDEX progNode = ast->createTNode("programNode", -1, "");	//there shouldn't be stmtNumber for programNode

	while (nextToken() != "")
	{
		INDEX procNode = procedure();
		if (procNode == -1)
			return -1;

		ast->linkNode(ast->getNode(progNode), ast->getNode(procNode), "parent");
	}

	ast->setRootNode(progNode);
	return progNode;
}

INDEX SimpleParser::procedure(){
	if(match("procedure") == false)
		return -1;

	proc = nextToken(); //proc stores the name of the current procedure under parsing

	if(match(PROC_NAME) == false)
		return -1;

	//Store procedure name into ProcTable
	procTable->insertProc(proc);
	cout << "Inserting proc: " << proc << " into ProcTable." <<endl;

	if(match("{") == false)
		return -1;

	INDEX firstChildStmtNode = stmtLst();
	if (firstChildStmtNode == -1)
		return -1;
	//we create a stmtLst node, which is the parent of all stmts.
	INDEX stmtLstNode = ast->createTNode("stmtLstNode", -1, "");	//there shouldn't be a stmtNumber for stmtLstNode (should change for future implementations)

	//Insert first stmt of procedure into ProcTable
	procTable->insertFirstStmt(proc, ast->getNode(firstChildStmtNode).getStmtNumber());
	procTable->insertLastStmt(proc, currStmtNumber);

	//here we link all the stmt nodes to stmtLst node as children.
	ast->linkNode(ast->getNode(stmtLstNode), ast->getNode(firstChildStmtNode), "parent");
	INDEX nextChildStmtNode = ast->getNode(firstChildStmtNode).getRightSibling();

	while(nextChildStmtNode > 0)
	{
		ast->linkNode(ast->getNode(stmtLstNode), ast->getNode(nextChildStmtNode), "parent");	
		nextChildStmtNode = ast->getNode(nextChildStmtNode).getRightSibling();
	}

	if (match("}") == false)
		return -1;

	//we create proc node and link to stmtLst node and return
	INDEX procNode = ast->createTNode("procedureNode", -1, "");	//shouldn't be stmtNumber for proc
	ast->linkNode(ast->getNode(procNode), ast->getNode(stmtLstNode), "parent");
	return procNode;
}

INDEX SimpleParser::stmtLst(){
	INDEX currStmtNode = stmt();
	if (legal == false) //check if stmt() has altered legal to false/parsing stopped prematurely
		return -1;

	if(nextToken() == "}")
	{
		return currStmtNode;
	}
	else{ //more stmts expected in this procedure; not end-of-procedure yet
		//link node of this stmt to next stmt
		INDEX nextStmtNode = stmtLst();

		//check for validity of nextStmtNode
		if (nextStmtNode != -1) 
		{
			ast->linkNode(ast->getNode(currStmtNode), ast->getNode(nextStmtNode), "left");
			follows->setFollows(ast->getNode(currStmtNode).getStmtNumber(), ast->getNode(nextStmtNode).getStmtNumber());
			//cout <<"SetFollows: " <<currStmtNumber <<" " <<currStmtNumber+1;
			return currStmtNode;
		}
		else
			return -1;
	}
}

INDEX SimpleParser::stmt(){

	if (nextToken() == "while")
	{
		/****WHILE STATEMENT****/
		consumeToken();
		currStmtNumber++;

		//set level of while node (value of 1 means root while node)
		whileLevel++;
		if (whileLevel == 1)
			rootWhileStmtNum = currStmtNumber;

		storeRootWhile->setWhileRoot(rootWhileStmtNum);
		
		//set rootIfThenElseList for this stmt
		if (thenOrElse == 0)
		{
			vector<int> zero;
			zero.push_back(0);
			storeRootIf->setIfRoot(zero);
		}
		else
		{
			storeRootIf->setIfRoot(rootIfElseList);
		}

		INDEX whileNode = ast->createTNode("whileNode", currStmtNumber, ""); //create while node
		int whileStmtNum = ast->getNode(whileNode).getStmtNumber();

		VARNAME controlVar = nextToken();
		if (match(VAR_NAME) == false) //check for control variable of while stmt
			return -1;
		//INDEX controlIndex = varTable->insertVar(controlVar);
		uses->setUses(whileStmtNum, controlVar);
		procTable->insertUses(proc, controlVar);
		INDEX controlVariableNode = ast->createTNode("varNode", currStmtNumber, controlVar); //create control variable node

		if (match("{") == false)
			return -1; 

		//check for empty while statement (not allowed)
		if (nextToken() == "")
			return -1;

		//we create a stmtLst node, which is the parent of all stmts.
		INDEX stmtLstNode = ast->createTNode("stmtLstNode", -1, ""); //there shouldn't be a stmtNumber for stmtLstNode (should change for future implementations)

		INDEX firstChildStmtNode = stmtLst();
		if (firstChildStmtNode == -1)
			return -1;

		//here we link all the stmt nodes to stmtLst node as children
		int currNodeStmtNum = ast->getNode(firstChildStmtNode).getStmtNumber();
		parent->setParent(whileStmtNum, currNodeStmtNum);
		cout << "Parent " << whileStmtNum << " " << currNodeStmtNum <<endl;

		INDEXLST currNodeUses = uses->getVariable(currNodeStmtNum);
		INDEX varIndex;
		VARNAME varName;

		//at the same time we set uses
		for(int i = 0; i < currNodeUses.size(); i++){
			varIndex = currNodeUses.at(i);
			varName = varTable->getVarName(varIndex);
			uses->setUses(whileStmtNum, varName);
			procTable->insertUses(proc, varName);
			cout << "Uses " << whileStmtNum << " " << varName << endl;
		}

		//at the same time we set modifies
		INDEXLST currNodeModifies = modifies->getVariable(currNodeStmtNum);
		for(int i = 0; i < currNodeModifies.size(); i++){
			varIndex = currNodeModifies.at(i);
			varName = varTable->getVarName(varIndex);
			modifies->setModifies(whileStmtNum, varName);
			procTable->insertModifies(proc, varName);
			cout << "Modifies " << whileStmtNum << " " << varName << endl;
		}

		ast->linkNode(ast->getNode(stmtLstNode), ast->getNode(firstChildStmtNode), "parent");
		INDEX nextChildStmtNode = ast->getNode(firstChildStmtNode).getRightSibling();

		while(nextChildStmtNode > 0){
			//parent link
			currNodeStmtNum = ast->getNode(nextChildStmtNode).getStmtNumber();
			parent->setParent(whileStmtNum, currNodeStmtNum);
			cout << "Parent " << whileStmtNum << " " << currNodeStmtNum <<endl;

			//at the same time we set uses
			currNodeUses = uses->getVariable(currNodeStmtNum);
			for(int i = 0; i < currNodeUses.size(); i++){
				varIndex = currNodeUses.at(i);
				varName = varTable->getVarName(varIndex);
				uses->setUses(whileStmtNum, varName);
				procTable->insertUses(proc, varName);
				cout << "Uses " << whileStmtNum << " " << varName << endl;
			}

			//at the same time we set modifies
			currNodeModifies = modifies->getVariable(currNodeStmtNum);
			for(int i = 0; i < currNodeModifies.size(); i++){
				varIndex = currNodeModifies.at(i);
				varName = varTable->getVarName(varIndex);
				modifies->setModifies(whileStmtNum, varName);
				procTable->insertModifies(proc, varName);
				cout << "Modifies " << whileStmtNum << " " << varName << endl;
			}

			//ast linking
			ast->linkNode(ast->getNode(stmtLstNode), ast->getNode(nextChildStmtNode), "parent");
			nextChildStmtNode = ast->getNode(nextChildStmtNode).getRightSibling();
		}

		cout << "Matching }" << endl;
		if (match("}") == false)
			return -1;

		//Link up nodes
		cout << "Linking while node to control variable and stmtLst" << endl;
		ast->linkNode(ast->getNode(whileNode), ast->getNode(controlVariableNode), "parent");
		ast->linkNode(ast->getNode(whileNode), ast->getNode(stmtLstNode), "parent");		

		//Decrement while level upon finishing to maintain value integrity
		whileLevel--;

		return whileNode;
		/******************/
	}

	else if (nextToken() == "if")
	{
		/****(IF-THEN)-ELSE STATEMENT****/
		consumeToken();
		currStmtNumber++;

		//Set root while for this stmt, if any
		if (whileLevel != 0)
			storeRootWhile->setWhileRoot(rootWhileStmtNum);
		else
			storeRootWhile->setWhileRoot(0);
		
		//set rootIfThenElseList for this stmt
		if (thenOrElse == 0)
		{
			vector<int> zero;
			zero.push_back(0);
			storeRootIf->setIfRoot(zero);
		}
		else
		{
			storeRootIf->setIfRoot(rootIfElseList);
		}

		//Set If stmt number into ifList
		rootIfElseList.push_back(currStmtNumber+1);
		thenOrElse = 1; //in THEN or ELSE mode

		INDEX ifNode = ast->createTNode("ifNode", currStmtNumber, ""); //Create IF node
		int ifStmtNum = ast->getNode(ifNode).getStmtNumber();

		VARNAME controlVar = nextToken();
		if (match(VAR_NAME) == false) //Check for control variable of IF stmt
			return -1;
		//INDEX controlIndex = varTable->insertVar(controlVar);
		uses->setUses(ifStmtNum, controlVar);
		procTable->insertUses(proc, controlVar);
		INDEX controlVariableNode = ast->createTNode("varNode", currStmtNumber, controlVar); //Create control variable node

		if (match("then") == false)
			return -1;

		if (match("{") == false)
			return -1; 

		//Create a stmtLst node for "THEN"
		INDEX thenStmtLstNode = ast->createTNode("stmtLstNode", -1, ""); //there shouldn't be a stmtNumber for stmtLstNode (should change for future implementations)
		//Identification for THEN or ELSE stmtLst???

		INDEX firstChildStmtNode = stmtLst();
		if (firstChildStmtNode == -1)
			return -1;

		//Set Parents relationship for IF
		int currNodeStmtNum = ast->getNode(firstChildStmtNode).getStmtNumber();
		parent->setParent(ifStmtNum, currNodeStmtNum);

		//Set Uses relationship for IF
		INDEXLST currNodeUses = uses->getVariable(currNodeStmtNum);
		INDEX varIndex;
		VARNAME varName;

		for(int i = 0; i < currNodeUses.size(); i++){
			varIndex = currNodeUses.at(i);
			varName = varTable->getVarName(varIndex);
			uses->setUses(ifStmtNum, varName);
			procTable->insertUses(proc, varName);
		}

		//Set Modifies relationship for IF
		INDEXLST currNodeModifies = modifies->getVariable(currNodeStmtNum);
		for(int i = 0; i < currNodeModifies.size(); i++){
			varIndex = currNodeModifies.at(i);
			varName = varTable->getVarName(varIndex);
			modifies->setModifies(ifStmtNum, varName);
			procTable->insertModifies(proc, varName);
		}

		//Link all the stmt nodes to "THEN" stmtLst node as children
		ast->linkNode(ast->getNode(thenStmtLstNode), ast->getNode(firstChildStmtNode), "parent"); //for firstChildStmtNode
		INDEX nextChildStmtNode = ast->getNode(firstChildStmtNode).getRightSibling();

		while(nextChildStmtNode > 0){ //for all the siblings of firstChildStmtNode
			//Set parent link
			currNodeStmtNum = ast->getNode(nextChildStmtNode).getStmtNumber();
			parent->setParent(ifStmtNum, currNodeStmtNum);
			cout << "Parent " << ifStmtNum << " " << currNodeStmtNum;

			//Set Uses
			currNodeUses = uses->getVariable(currNodeStmtNum);
			for(int i = 0; i < currNodeUses.size(); i++){
				varIndex = currNodeUses.at(i);
				varName = varTable->getVarName(varIndex);
				uses->setUses(ifStmtNum, varName);
				procTable->insertUses(proc, varName);
				cout << "Uses " << ifStmtNum << " " << varName << endl;
			}

			//Set Modifies
			currNodeModifies = modifies->getVariable(currNodeStmtNum);
			for(int i = 0; i < currNodeModifies.size(); i++){
				varIndex = currNodeModifies.at(i);
				varName = varTable->getVarName(varIndex);
				modifies->setModifies(ifStmtNum, varName);
				procTable->insertModifies(proc, varName);
				cout << "Modifies " << ifStmtNum << " " << varName << endl;
			}

			//Link nodes to "THEN" stmtLst node in AST
			ast->linkNode(ast->getNode(thenStmtLstNode), ast->getNode(nextChildStmtNode), "parent");
			nextChildStmtNode = ast->getNode(nextChildStmtNode).getRightSibling();
		}

		//Check for closing brace of IF(THEN stmtLst)
		cout << "Matching }" << endl;
		if (match("}") == false)
			return -1;

		//Remove last added if stmt number from list
		if (!rootIfElseList.empty())
			rootIfElseList.pop_back();

		//Link up nodes
		ast->linkNode(ast->getNode(ifNode), ast->getNode(controlVariableNode), "parent");
		cout << "Linking \"IF\" node to \"THEN\" stmtLst" << endl;
		ast->linkNode(ast->getNode(ifNode), ast->getNode(thenStmtLstNode), "parent");		

		//****ELSE stmtLst node****\\/
		if (match("else") == false)
			return -1;

		if (match("{") == false)
			return -1;

		//NOTE: There is no need to increment currStmtNumber because stmt numbering skips line of "else" by definition
		
		//Set If stmt number into ifList
		rootIfElseList.push_back(currStmtNumber+1);

		//we create a stmtLst node for "ELSE"
		INDEX elseStmtLstNode = ast->createTNode("stmtLstNode", -1, ""); //there shouldn't be a stmtNumber for stmtLstNode (should change for future implementations)
		//Identification for THEN or ELSE stmtLst???

		firstChildStmtNode = stmtLst();
		if (firstChildStmtNode == -1)
			return -1;

		//here we set Parents relationship for IF
		currNodeStmtNum = ast->getNode(firstChildStmtNode).getStmtNumber();
		parent->setParent(ifStmtNum, currNodeStmtNum);

		//at the same time we set Uses relationship for IF
		currNodeUses = uses->getVariable(currNodeStmtNum);

		for(int i = 0; i < currNodeUses.size(); i++){
			varIndex = currNodeUses.at(i);
			varName = varTable->getVarName(varIndex);
			uses->setUses(ifStmtNum, varName);
			procTable->insertUses(proc, varName);
		}

		//at the same time we set Modifies relationship for IF
		currNodeModifies = modifies->getVariable(currNodeStmtNum);
		for(int i = 0; i < currNodeModifies.size(); i++){
			varIndex = currNodeModifies.at(i);
			varName = varTable->getVarName(varIndex);
			modifies->setModifies(ifStmtNum, varName);
			procTable->insertModifies(proc, varName);
		}

		//here we link all the stmt nodes to "ELSE" stmtLst node as children
		ast->linkNode(ast->getNode(elseStmtLstNode), ast->getNode(firstChildStmtNode), "parent");
		nextChildStmtNode = ast->getNode(firstChildStmtNode).getRightSibling();

		while(nextChildStmtNode > 0){
			//parent link
			currNodeStmtNum = ast->getNode(nextChildStmtNode).getStmtNumber();
			parent->setParent(ifStmtNum, currNodeStmtNum);
			cout << "Parent " << ifStmtNum << " " << currNodeStmtNum;

			//at the same time we set uses
			currNodeUses = uses->getVariable(currNodeStmtNum);
			for(int i = 0; i < currNodeUses.size(); i++){
				varIndex = currNodeUses.at(i);
				varName = varTable->getVarName(varIndex);
				uses->setUses(ifStmtNum, varName);
				procTable->insertUses(proc, varName);
				cout << "Uses " << ifStmtNum << " " << varName << endl;
			}

			//at the same time we set modifies
			currNodeModifies = modifies->getVariable(currNodeStmtNum);
			for(int i = 0; i < currNodeModifies.size(); i++){
				varIndex = currNodeModifies.at(i);
				varName = varTable->getVarName(varIndex);
				modifies->setModifies(ifStmtNum, varName);
				procTable->insertModifies(proc, varName);
				cout << "Modifies " << ifStmtNum << " " << varName << endl;
			}

			//ast linking
			ast->linkNode(ast->getNode(elseStmtLstNode), ast->getNode(nextChildStmtNode), "parent");
			nextChildStmtNode = ast->getNode(nextChildStmtNode).getRightSibling();
		}

		//Check for closing brace of ELSE stmtLst
		cout << "Matching }" << endl;
		if (match("}") == false)
			return -1;

		//Remove last added if stmt number from list
		if (!rootIfElseList.empty())
			rootIfElseList.pop_back();

		thenOrElse = 0;

		//Link up nodes
		cout << "Linking if node to \"ELSE\" stmtLst" << endl;
		ast->linkNode(ast->getNode(ifNode), ast->getNode(elseStmtLstNode), "parent");	

		return ifNode;
		/******************/
	}

	else if (nextToken() == "call")
	{
		/****CALL STATEMENT****/
		consumeToken();
		currStmtNumber++;

		//Set root while for this stmt, if any
		if (whileLevel != 0)
			storeRootWhile->setWhileRoot(rootWhileStmtNum);
		else
			storeRootWhile->setWhileRoot(0);

		//set rootIfThenElseList for this stmt
		if (thenOrElse == 0)
		{
			vector<int> zero;
			zero.push_back(0);
			storeRootIf->setIfRoot(zero);
		}
		else
		{
			storeRootIf->setIfRoot(rootIfElseList);
		}

		string callee = nextToken();

		if (match(PROC_NAME) == false) //check procTable???
			return -1;

		if (match(";") == false)
			return -1;

		INDEX callNode = ast->createTNode("callNode", currStmtNumber, callee);
		calls->setCalls(proc, callee, currStmtNumber);
		cout << "Set Calls: " << proc << " calls " << callee << endl;

		return callNode;
	}

	else 
	{
		/****ASSIGNMENT STATEMENT****/
		currStmtNumber++;
		VARNAME next = nextToken();
		if (match(VAR_NAME) == false) 
			return -1;

		//Set root while for this stmt, if any
		if (whileLevel != 0)
			storeRootWhile->setWhileRoot(rootWhileStmtNum);
		else
			storeRootWhile->setWhileRoot(0);

		//set rootIfThenElseList for this stmt
		if (thenOrElse == 0)
		{
			vector<int> zero;
			zero.push_back(0);
			storeRootIf->setIfRoot(zero);
		}
		else
		{
			storeRootIf->setIfRoot(rootIfElseList);
		}

		//by now we have matched varname
		INDEX leftNode = ast->createTNode("varNode", currStmtNumber, next);

		//set modifies relationship
		modifies->setModifies(ast->getNode(leftNode).getStmtNumber(), next);
		procTable->insertModifies(proc, next);

		if (match("=") == false)
			return -1;

		//by now we have matched assign
		INDEX assignNode = ast->createTNode("assignNode", currStmtNumber, "");

		INDEX rightNode = expr();
		if (rightNode == -1) //if error occured in parsing RHS expression of assignment statement
			return -1;

		if(match(";") == false)
			return -1;

		//link the nodes up
		ast->linkNode(ast->getNode(leftNode), ast->getNode(assignNode), "children");
		ast->linkNode(ast->getNode(rightNode), ast->getNode(assignNode), "children");

		//return index to node
		return assignNode;
		/******************/
	}
}

bool SimpleParser::name(){
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

bool SimpleParser::integer(){
	string next = nextToken();
	for(int i = 0; i < next.size(); i++)
		if(isdigit(next[i]) == false){
			return false;
		}

		return true;
}

bool SimpleParser::checkOperator(int construct)
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

void SimpleParser::error(token error){
	cout << "Error: " << error << endl;
	legal = false;
}

int SimpleParser::getTotalStmtNumber(){
	return currStmtNumber;
}

int SimpleParser::expr() //CONSTANT or VAR_NAME or OPEN_BRACKET or CLOSE_BRACKET expected as current token upon call to expr()
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

int SimpleParser::factor()
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

int SimpleParser::term()
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
			uses->setUses(varStmtNum, next);
			procTable->insertUses(proc, next);
		}

		return node;

	}
}


/*
* PKB Methods
*/
void SimpleParser::initAST(AST * ast){
	this->ast = ast;
}

void SimpleParser::initVarTable(VarTable * varTable){
	this->varTable = varTable;
}

void SimpleParser::initFollows(Follows * follows){
	this->follows = follows;
}

void SimpleParser::initParent(Parent * parent){
	this->parent = parent;
}

void SimpleParser::initUses(Uses * uses){
	this->uses = uses;
}

void SimpleParser::initModifies(Modifies * modifies){
	this->modifies = modifies;
}

AST * SimpleParser::getParserAST()
{
	return this->ast;
}