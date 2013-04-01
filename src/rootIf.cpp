#include "rootIf.h"

rootIf::rootIf(){
	this->rootVector = new vector< vector<int> >();
}

rootIf::~rootIf(){
	delete this->rootVector;
}

void rootIf::setIfRoot(vector<int> ifList){
	this->rootVector->push_back(ifList);
}

vector<int> rootIf::getIfRootOfStmt(int stmtNumber){
	return this->rootVector->at(stmtNumber-1);
}