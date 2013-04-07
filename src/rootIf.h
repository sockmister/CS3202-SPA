#ifndef ROOTIF_H
#define ROOTIF_H

#include <vector>

using namespace std;

class rootIf{
private:
	vector< vector<int> > *rootIfElseVector;
	vector< vector<int> > *rootOnlyIfVector;

public:
	rootIf();
	~rootIf();

	/*
	Inserts stmt number of root if at the current processing stmt. 
	rootList of 0 means current stmt is not contained directly or indirectly in any If root.
	*/
	void setIfRoot( vector<int> );
	void setOnlyIfRoot( vector<int> );

	vector<int> getIfRootOfStmt(int);
	vector<int> getOnlyIfRootOfStmt(int);
};

#endif