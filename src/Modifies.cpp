#pragma once

#include "Modifies.h"

/*
  // Constructor
  Modifies::Modifies(VarTable* theVarTable):ModifiesDB(){
	  varTable = theVarTable;
  }

  // set Modifies(stmt, v) if that relationship hasn't been recorded 
  // return the index of the varName in the VarTable
  // if the varName does not exist in VarTable, ModifiesDB is unchanged and -1 is returned
  INDEX Modifies::setModifies(STMT stmt, VARNAME v){
	  
	  INDEX varIndx = varTable->getVarIndex(v);
	  
	  //check if the variable exists in VarTable
	  if(varIndx == -1)
	  {
		  return -1;
	  }
	  
	  // check if a Modifies vector which contains all the statements that modify v exists
	  if( ModifiesDB.size()-1 < varIndx ) {
		  for(int i=1; i<=(varIndx-(ModifiesDB.size()-1)); i++)
		  {
			  ModifiesDB.push_back(STMTLST());
		  }
		  ModifiesDB.back().push_back(stmt);
	  }
	  else {
		  //check if Modifies{stmt, v) already exists
		  if (find(ModifiesDB[varIndx].begin(), ModifiesDB[varIndx].end(), stmt) == ModifiesDB[varIndx].end()) {
			    ModifiesDB[varIndx].push_back(stmt);
		  }
	  }

	  return varIndx; 
  }

  // return the index of the variable modified in this statement, return -1 if there is no such variable
  INDEX Modifies::getVariable(STMT stmt){
	
	  for (vector<STMTLST>::size_type i = 0; i<ModifiesDB.size(); i++) { 		  
		  if(find(ModifiesDB[i].begin(), ModifiesDB[i].end(), stmt) != ModifiesDB[i].end()) {
			  return i;
		  }
	  }
	  
	  return -1;
  }

  // return a list of statements in which the variable is modified, return an empty list if there are no such statements 
  STMTLST Modifies::getStatements(VARNAME v){
	
	  INDEX varIndx = varTable->getVarIndex(v);

	  //check if the variable exists in VarTable
	  //and if a Modifies vector for that variable already exists
	  if(varIndx == -1 || varIndx>(ModifiesDB.size()-1))
	  {
		  STMTLST emptyList;
		  return emptyList;
	  }
  
	  return ModifiesDB[varIndx];
  }


   // return true only if Modifies(stmt, v) exists
  bool Modifies::isModifies(STMT stmt, VARNAME v){
	  
	  INDEX varIndx = varTable->getVarIndex(v);

	   //check if the variable exists in VarTable
	  //and if a Modifies vector for that variable already exists
	  if(varIndx == -1 || varIndx>(ModifiesDB.size()-1))
	  {
		  return false;
	  }
		  
	  if(find(ModifiesDB[varIndx].begin(), ModifiesDB[varIndx].end(), stmt) != ModifiesDB[varIndx].end()) {
			 return true;
	  }	  
	  
	  return false;
  }
  */
  // Default constructor
Modifies::Modifies(VarTable* theVarTable):ModifiesDB(){
	  ModifiesDB.push_back(STMTLST()); //dummy first entry at index 
	  varTable = theVarTable;
  }

  // method to set varName as Uses in statementNumber, returns index of varName in varTable
  INDEX Modifies::setModifies(STMT statementNumber, VARNAME varName){
	  int index = varTable->getVarIndex(varName);
	  // check if statementNumber is inserted before
	  if( ModifiesDB.size()-1 < statementNumber && index != -1) {
		  for (int i = ModifiesDB.size()-1; i < statementNumber; i++) {
			  ModifiesDB.push_back(STMTLST());
		  }
		  ModifiesDB[statementNumber].push_back(statementNumber);
	      ModifiesDB[statementNumber].push_back(index);
	  }
	  else if ( ModifiesDB[statementNumber].size() == 0 && index != -1) {
		  ModifiesDB[statementNumber].push_back(statementNumber);
	      ModifiesDB[statementNumber].push_back(index);
	  }
	  else 	{
		if ( ModifiesDB[statementNumber].size() > 0 && index != -1 ) {		 
			  if( ModifiesDB[statementNumber][0] == statementNumber ) {
				  for (STMTLST::size_type i = 1; i< ModifiesDB[statementNumber].size(); i++) { 
					  // check if varName is inserted before
					  if(ModifiesDB[statementNumber][i]	== index)
						  return index;
				  }
				  ModifiesDB.at(statementNumber).push_back(index);		
			  }
		}
	  }

	  return index; 
  }

  // returns a list of all variables in statementNumber, returns empty list if there are no such variables
  INDEXLST Modifies::getVariable(STMT statementNumber){
	  INDEXLST myVariable;
	  if(statementNumber >= ModifiesDB.size())
		  return myVariable;

	  if (ModifiesDB[statementNumber].size() > 0) {
		  for(STMTLST::size_type i = 1; i<ModifiesDB[statementNumber].size(); i++) {
			  myVariable.push_back(ModifiesDB[statementNumber][i]);			  
			  
		  }
		  return myVariable;
	  } else
		  return myVariable;
  }

  // returns a list of statement numbers which contains varName, returns empty list if there are no such statements 
  STMTLST Modifies::getStatements(VARNAME varName) {
	 int index = varTable->getVarIndex(varName);
	 STMTLST myStatement;
	  
	 if (index != -1) {
		 for(vector<STMTLST>::size_type i = 1; i<ModifiesDB.size(); i++)
		 {
			  for(STMTLST::size_type j = 1; j<ModifiesDB[i].size(); j++)
			  {
				  if(ModifiesDB[i][j] == index)
				  {
					  myStatement.push_back(ModifiesDB.at(i).front());
				  }
			 }
		 }
	 }
	 return myStatement;
  }


   // returns true if statementNumber contains varName
  bool Modifies::isModifies(STMT statementNumber, VARNAME varName) {
	  int index = varTable->getVarIndex(varName);
	  
	  if (index != -1 && statementNumber < ModifiesDB.size()) {
		  if (ModifiesDB[statementNumber].size() > 0) {
			  if( ModifiesDB[statementNumber][0] == statementNumber ) {
				  for(STMTLST::size_type i = 1; i<ModifiesDB[statementNumber].size(); i++) {
					  if (ModifiesDB[statementNumber][i] == index)
						  return true;  
				  }
				  return false;
			  } 
		  } 
	  }
	  
	  return false;
  }
