#pragma once

#include "Uses.h"


  // Default constructor
  Uses::Uses(VarTable* theVarTable):UsesDB(){
	  UsesDB.push_back(STMTLST()); //dummy first entry at index 
	  varTable = theVarTable;
  }

  // method to set varName as Uses in statementNumber, returns index of varName in varTable
  INDEX Uses::setUses(STMT statementNumber, VARNAME varName){
	  int index = varTable->getVarIndex(varName);
	  // check if statementNumber is inserted before
	  if( UsesDB.size()-1 < statementNumber && index != -1 ) {
		  for (int i = UsesDB.size()-1; i < statementNumber; i++) {
			  UsesDB.push_back(STMTLST());
		  }
		  UsesDB[statementNumber].push_back(statementNumber);
	      UsesDB[statementNumber].push_back(index);
	  }
	  else if ( UsesDB[statementNumber].size() == 0 && index != -1) {
		  UsesDB[statementNumber].push_back(statementNumber);
	      UsesDB[statementNumber].push_back(index);
	  }
	  else 	{
		if ( UsesDB[statementNumber].size() > 0 && index != -1) {		 
			  if( UsesDB[statementNumber][0] == statementNumber ) {
				  for (STMTLST::size_type i = 1; i< UsesDB[statementNumber].size(); i++) { 
					  // check if varName is inserted before
					  if(UsesDB[statementNumber][i]	== index)
						  return index;
				  }
				  UsesDB.at(statementNumber).push_back(index);		
			  }
		}
	  }

	  return index; 
  }

  // returns a list of all variables in statementNumber, returns empty list if there are no such variables
  INDEXLST Uses::getVariable(STMT statementNumber){
	  INDEXLST myVariable;
	  if(statementNumber >= UsesDB.size())
		  return myVariable;

	  if (UsesDB[statementNumber].size() > 0) {
		  for(STMTLST::size_type i = 1; i<UsesDB[statementNumber].size(); i++) {
			  myVariable.push_back(UsesDB[statementNumber][i]);			  
		  }
		  return myVariable;
	  } else
		  return myVariable;
  }

  // returns a list of statement numbers which contains varName, returns empty list if there are no such statements 
  STMTLST Uses::getStatement(VARNAME varName) {
	 int index = varTable->getVarIndex(varName);
	 STMTLST myStatement;
	 
	 if (index != -1) {
		 for(vector<STMTLST>::size_type i = 1; i<UsesDB.size(); i++)
		 {
			  for(STMTLST::size_type j = 1; j<UsesDB[i].size(); j++)
			  {
				  if(UsesDB[i][j] == index)
				  {
					  myStatement.push_back(UsesDB.at(i).front());
				  }
			 }
		 }
	 }
	 return myStatement;
  }


   // returns true if statementNumber contains varName
  bool Uses::isUses(STMT statementNumber, VARNAME varName) {
	  int index = varTable->getVarIndex(varName);
	  
	  if (index !=-1 && statementNumber < UsesDB.size()) {	
		  if (UsesDB[statementNumber].size() > 0) {
			  if( UsesDB[statementNumber][0] == statementNumber ) {
				  for(STMTLST::size_type i = 1; i<UsesDB[statementNumber].size(); i++) {
					  if (UsesDB[statementNumber][i] == index)
						  return true;  
				  }
				  return false;
			  } 
		  } 
	  } 

	  return false;
  }
