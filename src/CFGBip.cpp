#include "CFGBip.h"
using namespace std;

CFGBip::CFGBip(){
}

CFGBip::~CFGBip(){
	visited.clear();
	delete this->myCFGBip;
	delete this->procTable;
	delete this->stmtTable;
}

CFGBip::CFGBip(vector<vector<CFGLink>> * CFGBip, StmtTable * stmtTable, ProcTable * procTable) {
	this->myCFGBip = CFGBip;
	this->stmtTable = stmtTable;
	this->lastStmt = stmtTable->getSize()-1;
	this->procTable = procTable;
	
	for (int i=0; i<CFGBip->size(); i++) 
		visited.push_back(false);
	
}


STMTLST CFGBip::nextBipStatements(STMT n, STMT branchFrom) {
	STMTLST nextBip;
	if (n < 0 || n > this->lastStmt ) 
		return nextBip;

	else {
		this->n1 = n;
		this->branchFrom = branchFrom;
		PROCNAME procName = stmtTable->getCaller(n);

		this->procedureFirstStmt = procTable->getFirstStmt(procName);
		this->procedureLastStmt = procTable->getLastStmt(procName);

		skip = true;
		branch = false;
		call = true;
		first = false;
		exit = false;
		fill(visited.begin(),visited.end(), false);
		callStack.clear();

		vector<bool> answer;
		if (branchFrom > 0 )
			answer = AffectsBipDFS(n,0); // run AffectsBipDFS for reachability
		else if (branchFrom == 0)
			answer = DFS(n,0); // run DFS for reachability
		
		for (int i=0;i<answer.size();i++) {
			if ( answer[i] ==  true )
				nextBip.push_back(i);
		}

	}

	return nextBip;
}


bool CFGBip::isNextBip(STMT n1, STMT n2) {
	
	if (n1 < 0 || n1 > this->lastStmt || n2 < 0 || n2 > this->lastStmt) 
		return false;

	else {
			vector<CFGLink> nextBip = this->myCFGBip->at(n1);
			
			for (int i=0;i<nextBip.size();i++) {
				int nextLink = nextBip[i].getLinkTo();
				
				// next node is not dummy node
				if ( nextLink <= this->lastStmt) {
					if (nextLink == n2) {
						return true;
					}
				}
				// the next node is a dummy node
				else if ( nextLink > this->lastStmt && nextLink < this->myCFGBip->size() ) {
					
					vector<CFGLink> dummy = this->myCFGBip->at(nextLink);

					for (int j=0;j<dummy.size();j++) {
						if (dummy[j].getLinkTo() == n2)
							return true;
						
						if (dummy[j].getLinkTo() > this->lastStmt) {
							vector<CFGLink> dummy2 = this->myCFGBip->at(dummy[j].getLinkTo());
							for (int k=0;k<dummy2.size();k++) {
									if (dummy2[k].getLinkTo() == n2)
										return true;
							}
						}

					}
					
					return false;
				}
			}
			
	}
	
	return false;
}

bool CFGBip::isNextBipStar(STMT n1, STMT n2) {
	
	if (n1 < 0 || n1 > this->lastStmt || n2 < 0 || n2 > this->lastStmt) 
		return false;

	else {
		this->n1 = n1;
		PROCNAME procName = stmtTable->getCaller(n1);

		this->procedureFirstStmt = procTable->getFirstStmt(procName);
		this->procedureLastStmt = procTable->getLastStmt(procName);

		skip = true;
		branch = false;
		call = true;
		first = false;
		exit = false;
		fill(visited.begin(),visited.end(), false);
		callStack.clear();
		vector<bool> answer = DFS(n1,0); // run DFS for reachability
		
		for (int i=0;i<answer.size();i++) {
			if ( i==n2 && answer[i] ==  true )
				return true;
		}

	}
	return false;
}


// DFS
vector<bool> CFGBip::DFS(STMT programLine, int edgeNumber) {

	if (skip) {
		visited[programLine] = false;
		skip = false;
	}
	else if (skip == false)	{
		// dummy node can be visited more than once
		if ( programLine <= this->lastStmt) 
			visited[programLine] = true;
		
		if (programLine > this->lastStmt && first == false) { 
			first = true;	
			if (callStack.empty()) 
				call = false;
		}
	}

	// remove edge from stack after branching in
	if (branch == true) {
		callStack.pop_back();
		branch = false;
	}

	
	vector<CFGLink> nextBip = this->myCFGBip->at(programLine);
	
	// ending node
	if (nextBip.size() == 0 && call == true) {
		exit = true;
		return visited;
	}

	for (int i=0;i<nextBip.size();i++) {

		int nextLink = nextBip[i].getLinkTo();
		edgeNumber = nextBip[i].getEdgeNumber();
		
		if (exit && programLine >this->lastStmt) 
			break;

		// insert edgeNumber into callStack (excludes dummy nodes)
		if (edgeNumber > 0 && programLine <=this->lastStmt && call == true) {
			callStack.push_back(edgeNumber);
		}

		
		// BranchOut case for dummy node
		if (programLine > this->lastStmt) {	

			if (!callStack.empty()) {
				deque<STMT>::iterator it;
				it = find(callStack.begin(), callStack.end(), edgeNumber);
				
				// if edgeNumber is found
				if (it != callStack.end()) {
					branch = true;
				}
				else {
				// edgeNumber not found in callStack, do not branch in
					if (it == callStack.end()) {
						continue;
						/*
						// not called from original procedure
						if (this->stmtTable->getCaller(edgeNumber) == this->stmtTable->getCaller(this->n1))
							continue;	
						*/
					}
					
				}
				
			}
			// if call stack is empty, cases where the callers are unknown
		}

		if ( nextLink==0)
			break;
		
		// visit nodes that are not yet visited
		if (nextLink!=0 && visited[nextLink] == false)
			DFS(nextLink,edgeNumber);
	}

	return visited;
}


// DFS for AffectsBip*
vector<bool> CFGBip::AffectsBipDFS(STMT programLine, int edgeNumber) {
		if (skip) {
		visited[programLine] = false;
		skip = false;
	}
	else if (skip == false)	{
		// dummy node can be visited more than once
		if ( programLine <= this->lastStmt) 
			visited[programLine] = true;
		
		if (programLine > this->lastStmt && first == false) { 
			first = true;	
			if (callStack.empty()) 
				call = false;
		}
	}

	// remove edge from stack after branching in
	if (branch == true) {
		callStack.pop_back();
		branch = false;
	}

	
	vector<CFGLink> nextBip = this->myCFGBip->at(programLine);
	
	// ending node
	if (nextBip.size() == 0 && call == true) {
		exit = true;
		return visited;
	}

	for (int i=0;i<nextBip.size();i++) {

		int nextLink = nextBip[i].getLinkTo();
		edgeNumber = nextBip[i].getEdgeNumber();
		
		if (exit && programLine >this->lastStmt) 
			break;

		// insert edgeNumber into callStack (excludes dummy nodes)
		if (edgeNumber > 0 && programLine <=this->lastStmt && call == true) {
			callStack.push_back(edgeNumber);
		}

		
		// BranchOut case for dummy node
		if (programLine > this->lastStmt) {	

			if (!callStack.empty()) {
				deque<STMT>::iterator it;
				it = find(callStack.begin(), callStack.end(), edgeNumber);
				
				// if edgeNumber is found
				if (it != callStack.end()) {
					branch = true;
				}
				else {
				// edgeNumber not found in callStack, do not branch in
					if (it == callStack.end()) 
						continue;				
				}
				
			}
			// if call stack is empty, branch out to the given caller
			else {
				if (edgeNumber  != this->branchFrom )
					continue;
			}
		}

		if ( nextLink==0)
			break;
		
		// visit nodes that are not yet visited
		if (nextLink!=0 && visited[nextLink] == false)
			AffectsBipDFS(nextLink,edgeNumber);
	}

	return visited;

}