#ifndef ROOTONLYIF_H
#define ROOTONLYIF_H

#include <vector>

using namespace std;

class rootOnlyIf {
private:
	vector< vector<int> > *rootVector;

public:
	rootOnlyIf();
	~rootOnlyIf();

	/*
	Inserts stmt number of root if(only)  at the current processing stmt. 
	rootList of 0 means current stmt is not contained directly or indirectly in any If root.
	*/
	void setOnlyIfRoot( vector<int> );
	
	vector<int> getOnlyIfRootOfStmt(int);
};
#endif