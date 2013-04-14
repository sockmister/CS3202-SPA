//ProcTable stores all the procedures in the program
#pragma once

#include "ProcTable.h"


  // Default constructor
ProcTable::ProcTable(VarTable* theVarTable):ProcList(),FirstStmt(),LastStmt(),noOfProc(0),ModifiesList(),UsesList(),extra(),ProcConstList(),ProcVarList(){
	  varTable = theVarTable;
	  noOfProc = 0;
  }

  // method to insert a procedure to ProcTable
  // return the index of the procedure (even if it already exists)
  // the first procedure in the table has index 0
  INDEX ProcTable::insertProc(PROCNAME procName){
	  
	  PROCLIST::iterator it = find(ProcList.begin(), ProcList.end(), procName);
	  if(it != ProcList.end())
	  {
		  return it - ProcList.begin();
	  }
	
		  ProcList.push_back(procName); //add new procedure to the end of the list
		  FirstStmt.push_back(-1);
		  LastStmt.push_back(-1);
		  CFGList.push_back(new CFG());
		  ModifiesList.push_back(INDEXLIST());
		  UsesList.push_back(INDEXLIST());
		  noOfProc = ProcList.size();
		  ProcConstList.push_back(vector<string>());
		  ProcVarList.push_back(vector<string>());
		  return noOfProc - 1;
	
  }

  // method to insert stmt number of the first statement of the procedure at index procIndex
  // return -1 if the procedure index is not valid, otherwise return 0
  
  INDEX ProcTable::insertFirstStmt(PROCNAME procName, STMT stmt){

	  int index = getProcIndex(procName);
	  if(index == -1)
	  {
		  return -1;
	  }

	  FirstStmt[index] = stmt;

	  return 0;
  }


  // method to insert stmt number of the last statement of the procedure at index procIndex
  // return -1 if the procedure index is not valid, otherwise return 0
  
  INDEX ProcTable::insertLastStmt(PROCNAME procName, STMT stmt){

	  int index = getProcIndex(procName);
	  if(index == -1)
	  {
		  return -1;
	  }

	  LastStmt[index] = stmt;

	  return 0;
  }

  // method to insert CFG
  // return -1 if the procedure index is not valid, otherwise return 0
  
  INDEX ProcTable::insertCFG(PROCNAME procName, CFG* cfg){

	  int procIndex = getProcIndex(procName);
	  if(procIndex<0 || procIndex>(noOfProc-1))
	  {
		  return -1;
	  }

	  CFGList[procIndex] = cfg;

	  return 0;
  }

  // method to insert the varName(as index in varTable) that procName Modifies
  // return -1 if the procedure index is not valid, otherwise return 0
  
  INDEX ProcTable::insertModifies(PROCNAME procName, VARNAME varName){

	  int procIndex = getProcIndex(procName);
	  int varIndex = varTable->getVarIndex(varName);
	  if(procIndex == -1)
	  {
		  return -1;
	  }
		
	  //check if the variable has already been inserted
	  INDEXLIST::iterator it = find(ModifiesList[procIndex].begin(), ModifiesList[procIndex].end(), varIndex);
	  
	  if(procIndex != -1 && varIndex != -1 && it == ModifiesList[procIndex].end()) {
		  ModifiesList[procIndex].push_back(varIndex);
	  }

	  return 0;
  }

  // method to insert the varName(as index in varTable) that procName Uses
  // return -1 if the procedure index is not valid, otherwise return 0
  
  INDEX ProcTable::insertUses(PROCNAME procName, VARNAME varName){

	  int procIndex = getProcIndex(procName);
	  int varIndex = varTable->getVarIndex(varName);
	  if(procIndex == -1)
	  {
		  return -1;
	  }
	
	  //check if the variable has already been inserted
	  INDEXLIST::iterator it = find(UsesList[procIndex].begin(), UsesList[procIndex].end(), varIndex);

	  if (procIndex != -1 && varIndex != -1 && it == UsesList[procIndex].end()) {
		  UsesList[procIndex].push_back(varIndex);
	  }

	  return 0;
  }

  // method to insert vector of constants that appear in procedure
  // return -1 if the procedure index is not valid, otherwise return 0
  
  INDEX ProcTable::insertProcConst(PROCNAME procName, vector<string> procConstList){

	  int index = getProcIndex(procName);
	  if(index == -1)
	  {
		  return -1;
	  }

	  ProcConstList[index] = procConstList;

	  return 0;
  }

  // method to insert vector of variables that appear in procedure
  // return -1 if the procedure index is not valid, otherwise return 0
  
  INDEX ProcTable::insertProcVar(PROCNAME procName, vector<string> procVarList){

	  int index = getProcIndex(procName);
	  if(index == -1)
	  {
		  return -1;
	  }

	  ProcVarList[index] = procVarList;

	  return 0;
  }

  // method used by Design Extractor to get a pointer to vector extra

  PROCINDEXLIST* ProcTable::getExtra(){
	
	  return &extra;
  }

  // method to get the name of a procedure
  // if indx is not valid (i.e. indx<0 or indx>noOVar-1), return an empty string
  
  PROCNAME ProcTable::getProcName(INDEX indx){
	  
	  if((indx > (noOfProc-1)) || (indx<0))
	  {
			return "";
	  }

	  return ProcList[indx];
  }

  // method to get the index of a procedure
  // if the procedure does not exist, return -1
  INDEX ProcTable::getProcIndex(PROCNAME procName){
	  
	   PROCLIST::iterator it = find(ProcList.begin(), ProcList.end(), procName);

	   if(it != ProcList.end())
	   {
			return it - ProcList.begin();
	   }
	   else
	   {
			return -1;
	   }
  }


  //// method to get the name of the procedure that contains statement stmt
  //// if the statement does not exist, return an empty string
  //PROCNAME ProcTable::getProcedure(STMT stmt){
	
	 // if(stmt>0 && stmt<extra.size()){
		//  return extra[stmt];
	 // }

	 // return "";
  //}

  // method to get the stmt number of the first stmt
  // if the procedure does not exist, return -1
  INDEX ProcTable::getFirstStmt(PROCNAME procName){
	  
	  int index = getProcIndex(procName);
	  if(index == -1){
		  
		   return -1;
	  }

	  return FirstStmt[index];
  }

  // method to get the stmt number of the last stmt
  // if the procedure does not exist, return -1
  INDEX ProcTable::getLastStmt(PROCNAME procName){
	  
	  int index = getProcIndex(procName);
	  if(index == -1){
		  
		   return -1;
	  }

	  return LastStmt[index];
	 
  }

  // method to get the CFG that contains the stmt
  // if the procedure does not exist, return NULL
  CFG* ProcTable::getCFG(STMT stmt){
	
	  if(stmt>0 && stmt<extra.size()){
		  PROCINDEX procIndx = extra[stmt];
		  if(procIndx>-1 && procIndx<noOfProc){
			  return CFGList[procIndx];
		  }
	  }
	  return NULL;
  }

  //method to return all the procedure names in the ProcTable
  PROCLIST ProcTable::getAllProcNames(){
	  return ProcList;
  }

  //method to return the number of procedures in the table
  int ProcTable::getNoOfProc(){  
	  return noOfProc;
  }


  //method to return the variables(as index in varTable) that procName Modifies
  INDEXLIST ProcTable::getModifies(PROCNAME procName) {
	int procIndex = getProcIndex(procName);

	if(procIndex == -1)
	  {
		  return INDEXLIST();
	  }

	return ModifiesList.at(procIndex);
  }

  //method to return the variables(as index in varTable) that procName Uses
  INDEXLIST ProcTable::getUses(PROCNAME procName) {
	int procIndex = getProcIndex(procName);

	if(procIndex == -1)
	  {
		  return INDEXLIST();
	  }

	return UsesList.at(procIndex);
  }

  //method to get the constants that appear in procName
  vector<string> ProcTable::getProcConstList(PROCNAME procName){
	  
	  int index = getProcIndex(procName);
	  if(index == -1){
		  
		   return vector<string>();
	  }

	  return ProcConstList[index];
  }

  //method to get the constants that appear in procName
  vector<string> ProcTable::getProcVarList(PROCNAME procName){
	  
	  int index = getProcIndex(procName);
	  if(index == -1){
		  
		   return vector<string>();
	  }

	  return ProcVarList[index];
  }

  //method to check if a procedure index is valid
  bool ProcTable::isProcIndex(INDEX indx){
	  
	  if((indx > (noOfProc-1)) || (indx<0))
	  {
			return false;
	  }

	  return true;
  }

  //method to check if a procedure name is already in ProcTable
  bool ProcTable::isProcName(PROCNAME p){
	  
	   PROCLIST::iterator it = find(ProcList.begin(), ProcList.end(), p);

	   if(it != ProcList.end())
	   {
			return true;
	   }
	   else
	   {
			return false;
	   }
  }