#include "rootWhile.h"

rootWhile::rootWhile()
{
	this->rootVector = new vector<int>();
}

rootWhile::~rootWhile()
{
	delete this->rootVector;
}

void rootWhile::setWhileRoot(int stmtNumberRootWhile)
{
	this->rootVector->push_back(stmtNumberRootWhile);
}

int rootWhile::getWhileRootOfStmt(int stmtNumber)
{
	return this->rootVector->at(stmtNumber-1);
}