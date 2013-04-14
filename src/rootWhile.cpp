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
	if ( stmtNumber > this->rootVector->size() )
		return 0;
	else 
		return this->rootVector->at(stmtNumber-1);
}