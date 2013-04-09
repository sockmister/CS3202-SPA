#include "CFGBip.h"
using namespace std;

CFGBip::CFGBip(){
}

CFGBip::~CFGBip(){
}

CFGBip::CFGBip(vector<vector<CFGLink>> CFGBip, ProcTable * procTable) {
	this->myCFGBip = CFGBip;
	
	//myCFGit = CFG->begin() + firstStmt;

	//this->firstStmt = firstStmt;
	int noOfProc = procTable->getNoOfProc();
	this->lastStmt = procTable->getLastStmt(procTable->getProcName(noOfProc-1));
	this->procTable = procTable;

	for (int i=0; i<CFGBip.size(); i++) 
		visited.push_back(false);
	
}

/*
STMTLST CFGBip::nextBipStatement(ORDER order, STMT programLine) {
	STMTLST nextBip;

	if (programLine < 0 || programLine > this->lastStmt ) 
		return nextBip;

	if (order == 1) {
		//vector<vector<CFGLink>>::iterator it = (this->myCFGBip->begin()) + programLine;
		//vector<std::pair<int,int>>::iterator it = this->myCFG->begin();
		vector<CFGLink> temp = this->myCFGBip->at(programLine);


		for (int i=0;i<temp.size();i++) {
			int link = temp[i].getLinkTo();
			if (link != 0) {
				if ( link <= this->lastStmt) {
					nextBip.push_back(link);
				}
				// if the next node is the dummy node
				else if ( link > this->lastStmt && link < this->myCFGBip->size() ) {
					vector<CFGLink> dummy = this->myCFGBip->at(link);

					for (int j=0;j<dummy.size();j++) {

					}
				}
			}
		}


		if ( it->second != 0)
			nextBip.push_back(it->second);
		
		return nextBip;
	}


	
	if(order==0) {
		vector<std::pair<int,int>>::iterator it = (this->myCFGBip->begin()) + this->firstStmt;
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

STMTLST CFGBip::nextBipStatementStar(ORDER order, STMT programLine) {
	STMTLST nextStar;
	fill(visited.begin(),visited.end(), false);
	STMTLST::iterator it;

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

*/


bool CFGBip::isNextBip(STMT n1, STMT n2) {
	
	if (n1 < 0 || n1 > this->lastStmt || n2 < 0 || n2 > this->lastStmt) 
		return false;

	else {
			vector<CFGLink> nextBip = this->myCFGBip.at(n1);
			
			for (int i=0;i<nextBip.size();i++) {
				int nextLink = nextBip[i].getLinkTo();
				
				// next node is not dummy node
				if ( nextLink <= this->lastStmt) {
					if (nextLink == n2) {
						return true;
					}
				}
				// the next node is a dummy node
				else if ( nextLink > this->lastStmt && nextLink < this->myCFGBip.size() ) {
					/*
					vector<CFGLink> dummy = this->myCFGBip->at(nextLink);

					for (int j=0;j<dummy.size();j++) {
						if (dummy[j].getLinkTo() == n2)
							return true;
					}
					*/

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
		
		int noOfProc = procTable->getNoOfProc();
		// search stmt number is in which procedure
		for (int n=0;n<noOfProc;n++) {
			if (n1 >= procTable->getFirstStmt(procTable->getProcName(n)) && n1 <= procTable->getLastStmt(procTable->getProcName(n)) ) {
				this->procedureFirstStmt = procTable->getFirstStmt(procTable->getProcName(n));
				this->procedureLastStmt = procTable->getLastStmt(procTable->getProcName(n));
			}	
		}

		skip = true;
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
	else if (skip == false)	
		visited[programLine] = true;

	vector<CFGLink> nextBip = this->myCFGBip.at(programLine);
	for (int i=0;i<nextBip.size();i++) {
		int nextLink = nextBip[i].getLinkTo();
		int edgeNumber = nextBip[i].getEdgeNumber();
		
		// insert edgeNumber into callStack (excludes dummy nodes)
		if (edgeNumber > 0 && programLine <= this->lastStmt)
			callStack.push_back(edgeNumber);

		// BranchOut case for dummy node
		if (programLine > this->lastStmt) {
			deque<STMT>::iterator it;
			it = find(callStack.begin(), callStack.end(), edgeNumber);
			
			// edgeNumber not found in callStack
			if (it == callStack.end())	
				continue;
			
			if (it != callStack.end())
				callStack.pop_back();
		}

		if ( nextLink==0 || programLine == this->procedureLastStmt)
			break;
		if (nextLink!=0 && visited[nextLink] == false)
			DFS(nextLink);
	}

	return visited;
}



