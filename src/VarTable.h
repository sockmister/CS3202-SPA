//VarTable stores all the variables that appeared in the program

#pragma once
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*!  \class VarTable.
	 \brief VarTable is used to keep all the variable that appeared in the program.
*/

typedef int INDEX;
typedef string VARNAME;
typedef vector<VARNAME> VARLIST;

class VarTable{
 
  public:
  //! Default constructor
  VarTable();

  //! insert variable varName into VarTable if it does not exist
  /*!
    the first variable in the table has index 0
	\param VARNAME variable name
	\return index of the varName in the Vartable (even if it already exists)
  */
  INDEX insertVar(VARNAME varName);

  //! Get the variable name of a given index
  /*!
	return empty string if indx is not valid (i.e. indx<0 or indx>noOVar-1)
	\parm INDEX index of VarName
	\return variable name of indx
  */
  VARNAME getVarName(INDEX indx);

  //! Get the index of a given variable name
  /*!
	return -1 if variable does not exist
	\param VARNAME variable name
	\return index of varName
  */

  // method to get the index of a variable
  // if the variable does not exist, return -1
  INDEX getVarIndex(VARNAME varName);

  //! Get all the variables in the VarTable
  VARLIST getAllVar();

  //! Get the number of variables in the table
  int getNoOfVar();

 private:
  VARLIST VarList;				// an array of variable names
  int noOfVar;              // number of variables currently stored in this VarTable
};
