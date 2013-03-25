#pragma once

#include <vector>
#include "VarTable.h"

using namespace std;

/*!  \class Uses.
	 \brief Uses for statements is used to store all the Uses for assignment statement or container statement relationship in the program.
*/

typedef int STMT; //statement number
typedef vector<STMT> STMTLST;
typedef vector<INDEX> INDEXLST;

using namespace std;

class Uses {
 
  public:
  //! Default constructor
  Uses(VarTable* theVarTable);

  //! set statementNumber to use varName in the Uses PKB
  /*!
    if the varName does not exist in VarTable, UsesDB is unchanged and -1 is returned
	\param stmt statement number
	\param v variable name
	\return index of the varName in the Vartable
  */
  INDEX setUses(STMT statementNumber, VARNAME varName);

  //! Gets the variables of the statement number given in stmt from Uses PKB
  /*!
	\parm stmt statement number
	\return empty list if there is no such variable
  */
  INDEXLST getVariable(STMT statementNumber);

  //! Gets a list of statements in which the variable is Used
  /*!
	\param v variable name
	\return empty list if there is no such statements 
  */
  STMTLST getStatement(VARNAME varName);

  //! If variable v is Used in statement number stmt, return TRUE
  /*!
	\param stmt statement number
	\param v variable name
	\return FALSE if it does not exist in the Uses PKB
  */
  bool isUses(STMT statementNumber, VARNAME varName);


 private:
	 vector<STMTLST> UsesDB; 
	 VarTable* varTable;
};
