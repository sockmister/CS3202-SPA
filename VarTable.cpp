//VarTable stores all the variables that appeared in the program
#pragma once

#include "VarTable.h"


  // Default constructor
  VarTable::VarTable():VarList(), noOfVar(0){
	  noOfVar = 0;
  }

  // method to insert a variable to VarTable
  // return the index of the variable (even if it already exists)
  // the first variable in the table has index 0
  INDEX VarTable::insertVar(VARNAME varName){
	  
	  VARLIST::iterator it = find(VarList.begin(), VarList.end(), varName);
	  if(it != VarList.end())
	  {
		  return it - VarList.begin();
	  }
	
		  VarList.push_back(varName); //add new variable to the end of the list
		  noOfVar = VarList.size();
		  return noOfVar - 1;
	
  }

  // method to get the name of a variable
  // if indx is not valid (i.e. indx<0 or indx>noOVar-1), return an empty string
  VARNAME VarTable::getVarName(INDEX indx){
	  
	  if((indx > (noOfVar-1)) || (indx<0))
	  {
			return "";
	  }

	  return VarList[indx];
  }

  // method to get the index of a variable
  // if the variable does not exist, return -1
  INDEX VarTable::getVarIndex(VARNAME varName){
	  
	   VARLIST::iterator it = find(VarList.begin(), VarList.end(), varName);

	   if(it != VarList.end())
	   {
			return it - VarList.begin();
	   }
	   else
	   {
			return -1;
	   }
  }

  //method to return all the variables in the VarTable
  VARLIST VarTable::getAllVar(){
	  return VarList;
  }

  //method to return the number of variables in the table
  int VarTable::getNoOfVar(){  
	  return noOfVar;
  }