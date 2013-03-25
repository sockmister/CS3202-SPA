#pragma once

#include <vector>
#include <algorithm>
#include "VarTable.h"

using namespace std;

/*!  \class Modifies.
	 \brief Modifies for statements is used to store all the Modifies relationship for statements in the program.
*/

typedef int STMT; //statement number
typedef vector<STMT> STMTLST;
typedef vector<INDEX> INDEXLST;

class Modifies {
 
  public:
  //! Default constructor.
  Modifies(VarTable* theVarTable);

  //! set Modifies(stmt, v) if it is not in Modifies PKB
  /*!
    if the varName does not exist in VarTable, ModifiesDB is unchanged and -1 is returned
	\param stmt statement number
	\param v variable name
	\return index of the varName in the Vartable
  */
  INDEX setModifies(STMT stmt, VARNAME v);

  //! Get the variables of the statement number given in stmt from Modifies PKB
  /*!
	\parm stmt statement number
	\return empty list if there is no such variable
  */
  INDEXLST getVariable(STMT stmt);

  //! Gets a list of statements in which the variable is modified
  /*!
	\param v variable name
	\return empty list if there is no such statements 
  */
  STMTLST getStatements(VARNAME v);

  //! If variable v is modified in statement number stmt, return TRUE
  /*!
	\param stmt statement number
	\param v variable name
	\return FALSE if it does not exist in the Modifies PKB
  */
  bool isModifies(STMT stmt, VARNAME v);


 private:
	 vector<STMTLST> ModifiesDB; //ModifiesDB stores Modifies vectors for all the variables in VarTable. A Modifies vector of a variable contains statements in which that variable is modified.  
	 VarTable* varTable;
};
