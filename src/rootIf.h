#ifndef ROOTIF_H
#define ROOTIF_H

#include <vector>

using namespace std;

class rootIf{
private:
	vector< vector<int> > *rootVector;

public:
	rootIf();
	~rootIf();

	/*
	Inserts stmt number of root while at the current processing stmt. 
	rootList of 0 means current stmt is not contained directly or indirectly in any If root.
	*/
	void setIfRoot( vector<int> );
	
	vector<int> getIfRootOfStmt(int);
};

#endif