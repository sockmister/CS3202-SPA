#ifndef ROOTWHILE_H
#define ROOTWHILE_H

#include <string>
#include <vector>

using namespace std;

class rootWhile{
private:
	vector<int> *rootVector;

public:
	rootWhile();
	~rootWhile();

	/*
	Inserts stmt number of root while at the current processing stmt. 
	ZERO means current stmt is not contained directly or indirectly in any while root.
	*/
	void setWhileRoot(int);
	
	int getWhileRootOfStmt(int);
};

#endif