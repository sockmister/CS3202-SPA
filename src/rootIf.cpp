#include "rootIf.h"

rootIf::rootIf(){
	this->rootIfElseVector = new vector< vector<int> >();
	this->rootOnlyIfVector = new vector< vector<int> >();
}

rootIf::~rootIf(){
	delete this->rootIfElseVector;
	delete this->rootOnlyIfVector;
}

void rootIf::setIfRoot(vector<int> ifList){
	this->rootIfElseVector->push_back(ifList);
}

void rootIf::setOnlyIfRoot(vector<int> onlyIfList){
	this->rootOnlyIfVector->push_back(onlyIfList);
}

vector<int> rootIf::getIfRootOfStmt(int stmtNumber){
	return this->rootIfElseVector->at(stmtNumber-1);
}

vector<int> rootIf::getOnlyIfRootOfStmt(int stmtNumber){
	return this->rootOnlyIfVector->at(stmtNumber-1);
}