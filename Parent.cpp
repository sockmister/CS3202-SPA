#pragma once

#include "Parent.h"


  // Default constructor
  Parent::Parent():ParentDB(){
	  ParentDB.push_back(STMTLST()); //dummy first entry at index 0
  }

  // method to set parentStmt as parent of childStmt
  void Parent::setParent(STMT parentStmt, STMT childStmt){
	  // check if parentStmt has been inserted before
	  if( ParentDB.size()-1 < parentStmt ) {
		  for (int i = ParentDB.size()-1; i < parentStmt; i++) {
			  ParentDB.push_back(STMTLST());
		  }
		  //ParentDB[parentStmt].push_back(parentStmt);
		  ParentDB[parentStmt].push_back(childStmt);
	  }
	  else if ( ParentDB[parentStmt].size() == 0 ) 	{	 
			  	//ParentDB[parentStmt].push_back(parentStmt);
		        ParentDB[parentStmt].push_back(childStmt);
	  } else {
			if( ParentDB[parentStmt].size() > 0 ) {
				//if( ParentDB[parentStmt][0] == parentStmt ) {
					for (STMTLST::size_type i = 0; i< ParentDB[parentStmt].size(); i++) { 
						// check if childStmt is inserted before
						if(ParentDB[parentStmt][i] == childStmt)
							return;
					}
						ParentDB[parentStmt].push_back(childStmt);					  	
				}
			
	  }
	 
  }

  // returns -1 if childStmt has no parent
  STMT Parent::getParent(STMT childStmt){
	  for(vector<STMTLST>::size_type i = 1; i<ParentDB.size(); i++)
	  {
		  for(STMTLST::size_type j = 0; j<ParentDB[i].size(); j++)
		  {
			  if(ParentDB[i][j] == childStmt)
			  {
				  return i;
				  //return ParentDB.at(i).front();
			  }
		  }
	  }
	  return -1;
  }

  // returns -1 if childStmt has no parent star
  STMTLST Parent::getParentStar(STMT childStmt){
	 STMTLST parentStar;

	 STMT temp = getParent(childStmt);
	 while (temp != -1) {
		 parentStar.push_back(temp);
		 temp = getParent(temp);
	 }

	 return parentStar;
  }

  // returns null array if there are no children statements
  STMTLST Parent::getChild(STMT parentStmt){
	  STMTLST childStmt;
	  if(parentStmt >= ParentDB.size())
		  return childStmt;

	  if ( ParentDB[parentStmt].size() > 0 ) {
		  childStmt = ParentDB.at(parentStmt);
		  //childStmt.erase(childStmt.begin());
		  return childStmt;
	  }

	  return childStmt;
  }

  STMTLST Parent::getChildStar(STMT parentStmt){
	  STMTLST childrenStar;
	  
	  STMTLST temp = getChild(parentStmt);
	  if (temp.size() > 0 ) {
		  for (vector<STMT>::size_type i = 0 ; i<temp.size() ; i++) {
			  childrenStar.push_back(temp.at(i));
			  STMTLST temp2 = getChild(temp.at(i));	  
			  if (temp2.size() > 0) {
				  for (STMTLST::size_type j = 0 ; j<temp2.size() ; j++) {
					  childrenStar.push_back(temp2.at(j));
				  }
			  }
		  }  
	  }

	  return childrenStar;
  }

  bool Parent::isParent(STMT firstStmt, STMT secondStmt) {
	  bool cont = false;
	  if ( firstStmt > ParentDB.size()-1 )
		  return false;
	  else {
		  for(vector<STMTLST>::size_type i = 1; i<ParentDB.size(); i++)
		  {
			  for(STMTLST::size_type j = 0; j<ParentDB[i].size(); j++)
			  {
				  if(ParentDB[i][j] == secondStmt)
				  {
					  cont = true;
					  break;
				  }
			  }
		}
	  }
		  
	  if ( cont ) {
			int result = getParent(secondStmt);
	  
			if ( result == -1 || result != firstStmt )
				return false;
			else 
				return true;
	  }
	  else 
		  return false;
  }

  bool Parent::isParentStar(STMT firstStmt, STMT secondStmt) {

	  STMTLST parentStar = getParentStar(secondStmt);
	  
	  if (parentStar.size() > 0 && firstStmt < ParentDB.size()) {
		  for ( STMTLST::size_type i = 0 ; i < parentStar.size() ; i++ ) {
			  if ( parentStar.at(i) == firstStmt)
				  return true;
		  }
	  }
      return false;
  }