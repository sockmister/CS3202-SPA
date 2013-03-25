#pragma once

#include "Calls.h"


  // Default constructor
 Calls::Calls(ProcTable* theProcTable):CallsDB(), temp(){
	  procTable = theProcTable;
  }

  // method to set Calls relationship (for Parser)
  bool Calls::setCalls(PROCNAME caller, PROCNAME callee, STMT callStmt){
	  
	  temp.push_back(make_pair(make_pair(caller, callee), callStmt));
	  return true;	
  }

  // method to set Calls relationship (for Design Extractor)
  bool Calls::setCalls(PROCINDEX caller, PROCINDEX callee){

			// check if caller is inserted before
			for (int i = CallsDB.size()-1; i < caller; i++) {
					CallsDB.push_back(vector<bool>());
			}

			for (int i = CallsDB[caller].size()-1; i < callee; i++) {
					CallsDB[caller].push_back(false);
			}
			
			CallsDB[caller][callee] = true;

			return true;
  }

  // method to pass temp to Design Extractor
  vector<pair<pair<PROCNAME, PROCNAME>, STMT>> Calls::getTemp(){

	  vector<pair<pair<PROCNAME, PROCNAME>, STMT>> newVec;

	  //this also clears the content of temp
	  newVec.swap(temp);

	  return newVec;
  }


  // method to get the indices of procedures that are directly called from procedure p
  // return empty list if there is no such procedure; or return a list with only one element -1 if procedure p does not exist
  PROCINDXLIST Calls::getCallees(PROCNAME p){
	  
	  PROCINDXLIST myCallees;
	  
	  PROCINDEX pIndx = procTable->getProcIndex(p);

	  if(pIndx != -1){
		  if(pIndx < CallsDB.size()){
					for (vector<bool>::size_type i = 0; i< CallsDB[pIndx].size(); i++) {
						if(CallsDB[pIndx][i] == true){
							myCallees.push_back(i);
						}
					}
					
			}	
		  return myCallees;
	  }
	 
	  myCallees.push_back(-1);
	  return myCallees;
  }

  // method to get the indices of procedures that call procedure q
  // return empty list if there is no such procedure; or return a list with only one element -1 if procedure q does not exist
  PROCINDXLIST Calls::getCallers(PROCNAME q){
	  
	  PROCINDXLIST myCallers;
	  
	  PROCINDEX qIndx = procTable->getProcIndex(q);

	  if(qIndx != -1){
		 for(vector<vector<bool>>::size_type i = 0; i<CallsDB.size(); i++)
		 {
			 if(CallsDB[i].size()>qIndx){
				 if(CallsDB[i][qIndx] == true)
				  {
					  myCallers.push_back(i);
				  }
			 }
		 }
		 return myCallers;
	  }
	 
	  myCallers.push_back(-1);
	  return myCallers;
  }

  // method to get the indices of procedures that are called from procedure p indirectly
  // return empty list if there is no such procedure; or return a list with only one element -1 if procedure p does not exist
  PROCINDXLIST Calls::getCalleesStar(PROCNAME p){
	  
	  int numOfProc = procTable->getNoOfProc();
	  PROCINDXLIST myCalleesStar;
	  queue<PROCINDEX> toBeVisited;
	  vector<bool> visited(numOfProc, false); //any visited procedure is a callee
	  
	  PROCINDEX pIndx = procTable->getProcIndex(p);
	  PROCINDEX indx;

	  if(pIndx != -1){
		  if(pIndx < CallsDB.size()){
					toBeVisited.push(pIndx);

					while(!toBeVisited.empty()){
						indx = toBeVisited.front();
						visited[indx] = true;

						for (vector<bool>::size_type i = 0; i< CallsDB[indx].size(); i++) {
							if(CallsDB[indx][i] == true && visited[i] == false){
								toBeVisited.push(i);
								myCalleesStar.push_back(i);
							}
						}
						
						toBeVisited.pop();
					
					}	
		  
			}
			return myCalleesStar;
	  }
	 
	  myCalleesStar.push_back(-1);
	  return myCalleesStar;
  }

  // method to get the indices of procedures that call procedure q indirectly
  // return empty list if there is no such procedure; or return a list with only one element -1 if procedure q does not exist
  PROCINDXLIST Calls::getCallersStar(PROCNAME q){
	  
	  int numOfProc = procTable->getNoOfProc();
	  PROCINDXLIST myCallersStar;
	  queue<PROCINDEX> toBeVisited;
	  vector<bool> visited(numOfProc, false); //any visited procedure is a caller
	  
	  PROCINDEX qIndx = procTable->getProcIndex(q);
	  PROCINDEX indx;

	  if(qIndx != -1){
		  toBeVisited.push(qIndx);

		  while(!toBeVisited.empty()){
			  indx = toBeVisited.front();
			  visited[indx] = true;

			  for(vector<vector<bool>>::size_type i = 0; i<CallsDB.size(); i++)
			  {
					if(CallsDB[i].size()>indx){
						if(CallsDB[i][indx] == true && visited[i] == false)
						{
							toBeVisited.push(i); 
							myCallersStar.push_back(i);
						}
					}
			  }

			  toBeVisited.pop();

		 }
		 
		  return myCallersStar;
	  }
	 
	  myCallersStar.push_back(-1);
	  return myCallersStar;
  }


   // returns true if the Calls relationship exists
  bool Calls::isCalls(PROCNAME p, PROCNAME q) {
	  
	  PROCINDEX caller = procTable->getProcIndex(p);
	  PROCINDEX callee = procTable->getProcIndex(q);
	  
	  if (caller != -1 && callee != -1 && caller < CallsDB.size()) {	
			if(CallsDB[caller].size()>callee){
				if(CallsDB[caller][callee] == true)
				{
					return true;
				}
			}
	  } 
	  return false;
  }

  // returns true if the Calls* relationship exists
  bool Calls::isCallsStar(PROCNAME p, PROCNAME q) {
	  
	  PROCINDEX callee = procTable->getProcIndex(q);
	  
	  if(callee != -1){
		PROCINDXLIST myCalleesStar = getCalleesStar(p);

		PROCINDXLIST::iterator it = find(myCalleesStar.begin(), myCalleesStar.end(), callee);
		if(it != myCalleesStar.end()){
			return true;
		}
	  }

	  return false;
  }