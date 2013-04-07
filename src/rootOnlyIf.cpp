#include "rootOnlyIf.h"

rootOnlyIf::rootOnlyIf(){
	this->rootVector = new vector< vector<int> >();
}

rootOnlyIf::~rootOnlyIf(){
	delete this->rootVector;
}

void rootOnlyIf::setOnlyIfRoot(vector<int> onlyIfList){
	this->rootVector->push_back(onlyIfList);
}

vector<int> rootOnlyIf::getOnlyIfRootOfStmt(int stmtNumber){
	return this->rootVector->at(stmtNumber-1);
}