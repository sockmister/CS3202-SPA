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


STMTLST CFGBip::nextBipStatements(STMT n) {
	STMTLST nextBip;
	if (n < 0 || n > this->lastStmt ) 
		return nextBip;

	else {
		this->n1 = n;
		PROCNAME procName = stmtTable->getCaller(n);

		this->procedureFirstStmt = procTable->getFirstStmt(procName);
		this->procedureLastStmt = procTable->getLastStmt(procName);

		skip = true;
		fill(visited.begin(),visited.end(), false);
		callStack.clear();
		vector<bool> answer = DFS(n); // run DFS for reachability
		
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
		fill(visited.begin(),visited.end(), false);
		callStack.clear();
		vector<bool> answer = DFS(n1); // run DFS for reachability
		
		for (int i=0;i<answer.size();i++) {
			if ( i==n2 && answer[i] ==  true )
				return true;
		}

	}
	return false;
}


// DFS
vector<bool> CFGBip::DFS(STMT programLine) {

	if (skip) {
		visited[programLine] = false;
		skip = false;
	}
	else if (skip == false)	{
		// dummy node can be visited more than once
		if ( programLine <= this->lastStmt )
			visited[programLine] = true;
	}

	vector<CFGLink> nextBip = this->myCFGBip->at(programLine);
	for (int i=0;i<nextBip.size();i++) {
		int nextLink = nextBip[i].getLinkTo();
		int edgeNumber = nextBip[i].getEdgeNumber();
		
		// insert edgeNumber into callStack (excludes dummy nodes)
		if (edgeNumber > 0 && programLine < this->lastStmt)
			callStack.push_back(edgeNumber);

		
		// BranchOut case for dummy node
		if (programLine > this->lastStmt) {
			deque<STMT>::iterator it;
			it = find(callStack.begin(), callStack.end(), edgeNumber);
			
			// edgeNumber not found in callStack or larger than the original n1
			if (!callStack.empty() && it == callStack.end() && edgeNumber > this->n1)
				continue;
			
			// if edgeNumber is found
			if (it != callStack.end() && edgeNumber != callStack.front())
				callStack.pop_back();
		}
		

		if ( nextLink==0 )
			break;
		// visit nodes that are not yet visited
		if (nextLink!=0 && visited[nextLink] == false)
			DFS(nextLink);
	}

	return visited;
}



