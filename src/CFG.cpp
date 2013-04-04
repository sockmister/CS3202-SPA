#include "CFG.h"
using namespace std;

CFG::CFG(){

}

CFG::CFG(rootWhile * storeRootWhile,	rootIf * storeRootIf){
	this->storeRootWhile = storeRootWhile;
	this->storeRootIf = storeRootIf;
}

CFG::~CFG(){
}

CFG::CFG(vector<std::pair<int,int>>* CFG, STMT firstStmt, STMT lastStmt, rootWhile * storeRootWhile,	rootIf * storeRootIf) {
	this->myCFG = CFG;
	
	//myCFGit = CFG->begin() + firstStmt;

	this->firstStmt = firstStmt;
	this->lastStmt = lastStmt;
	this->storeRootWhile = storeRootWhile;
	this->storeRootIf = storeRootIf;

	for (int i=0; i<CFG->size(); i++) 
		visited.push_back(false);
	
}


STMTLST CFG::nextStatement(ORDER order, STMT programLine) {
	STMTLST next;
	if(this == NULL){
		return next;
	}


	if (programLine < this->firstStmt || programLine > this->lastStmt) 
		return next;

	if (order == 1) {
		vector<std::pair<int,int>>::iterator it = (this->myCFG->begin()) + programLine;
		//vector<std::pair<int,int>>::iterator it = this->myCFG->begin();

		if ( it->first != 0 )
			next.push_back(it->first);

		if ( it->second != 0)
			next.push_back(it->second);
		
		return next;
	}


	
	if(order==0) {
		vector<std::pair<int,int>>::iterator it = (this->myCFG->begin()) + this->firstStmt;
		//vector<std::pair<int,int>>::iterator it = this->myCFG->begin();

		for (int i=this->firstStmt; i<this->lastStmt;i++) {
			if (it->first == programLine) {
				next.push_back(i);
			}
			if (it->second == programLine) {
				next.push_back(i);
			}
			it++;
		}

		return next;
	}
	return next;
}

STMTLST CFG::nextStatementStar(ORDER order, STMT programLine) {
	STMTLST nextStar;
	fill(visited.begin(),visited.end(), false);
	STMTLST::iterator it;

	if(this == NULL){
		return nextStar;
	}

	if (programLine < this->firstStmt || programLine > this->lastStmt) 
		return nextStar;

	// find program lines that appear after programLine 
	if( order == 1 ) {

		skip = true;
		vector<bool> answer = DFS(programLine); // run DFS for reachability
		
		for (int j=0;j<answer.size();j++) {
			if (answer.at(j) ==  true)
				nextStar.push_back(j);
		}

		return nextStar;
	}

	//find program lines that precedes programLine
	if( order==0 ) {

		for (int i=this->firstStmt; i<this->lastStmt ; i++) { // DFS on every node 
			fill(visited.begin(),visited.end(), false);

			skip = true;
			vector<bool> answer = DFS(i);

			if (answer[programLine] ==  true)  // if it visits programLine (there is a control flow path),
				nextStar.push_back(i);		   // then add it to the nextStar array	

		}

		return nextStar;
	}

	return nextStar;

}



bool CFG::isNext(STMT n1, STMT n2) {
	if(this == NULL)
		return false;

	if (n1 < this->firstStmt || n1 > this->lastStmt || n2 < this->firstStmt || n2 > this->lastStmt) 
		return false;

	else {
		STMTLST next = nextStatement(1,n1);

		for (size_t i=0; i<next.size(); i++) {
			if ( next.at(i) == n2) 
				return true;
		}
	}
	
	return false;
}

bool CFG::isNextStar(STMT n1, STMT n2) {
	/*if(this == NULL)
		return false;
	
	if (n1 < this->firstStmt || n1 > this->lastStmt || n2 < this->firstStmt || n2 > this->lastStmt) 
		return false;

	else {
		STMTLST nextStar = nextStatementStar(1,n1);

		for (int i=0; i<nextStar.size(); i++) {
			if ( nextStar[i] == n2 )
				return true;
		}
	}
	return false;
	*/
	if(n1<firstStmt || n1>lastStmt || n2<firstStmt || n2>lastStmt){
		return false;
	}
	if(storeRootWhile->getWhileRootOfStmt(n1) == storeRootWhile->getWhileRootOfStmt(n2)
		&& storeRootWhile->getWhileRootOfStmt(n1)!=0 ){
		return true;
	}
	if(n1==n2){
		return false;
	}
	if(storeRootIf->getIfRootOfStmt(n1).at(0)!=0 && storeRootIf->getIfRootOfStmt(n2).at(0)!=0){
		int level;
		if(storeRootIf->getIfRootOfStmt(n1).size()>storeRootIf->getIfRootOfStmt(n2).size()){
			level = storeRootIf->getIfRootOfStmt(n2).size() -1;
		}else{
			level = storeRootIf->getIfRootOfStmt(n1).size() -1;
		}
		if(storeRootIf->getIfRootOfStmt(n1).at(level)!=storeRootIf->getIfRootOfStmt(n2).at(level)){
			return false;
		}
	}
	return (n2>n1);
}


// DFS
vector<bool> CFG::DFS(STMT programLine) {

	if (skip) {
		visited[programLine] = false;
		skip = false;
	}
	else if (skip == false)	
		visited[programLine] = true;

	STMTLST next = nextStatement(1,programLine);
	for (int j=0;j<next.size();j++) {
		int n = next[j];
		if (n ==-1 || n==0 )
			break;
		if (n!=-1 && visited[n] == false)
			DFS(n);
	}

	return visited;
}

// For testing
vector<std::pair<int,int>> * CFG::getCFG() {
	return this->myCFG;
}

