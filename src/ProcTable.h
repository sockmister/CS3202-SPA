//ProcTable stores all the procedures in the program

#pragma once
#include "CFG.h"
#include "VarTable.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/*!  \class ProcTable.
	 \brief ProcTable is used to keep all the procedures in the program.
*/

typedef int INDEX;
typedef int PROCINDEX;
typedef vector<PROCINDEX> PROCINDEXLIST;
typedef vector<INDEX> INDEXLIST;
typedef string PROCNAME;
typedef vector<PROCNAME> PROCLIST;
typedef int STMT;
typedef vector<STMT> STMTLST;
typedef vector<CFG*> CFGLIST;
typedef vector<INDEXLIST> MODIFIES;
typedef vector<INDEXLIST> USES;


class ProcTable{
 
  public:
  //! Default constructor
  ProcTable();
  ProcTable(VarTable* theVarTable);

  //! insert procedure procName into ProcTable if it does not exist
  /*!
    the first procedure in the table has index 0
	\param PROCNAME procedure name
	\return index of the procName in the ProcTable (even if it already exists)
  */
  INDEX insertProc(PROCNAME procName);

  //! insert stmt number of the first statement of the procedure at index procIndex
  /*!
	\param PROCNAME procedure name
	\return -1 if the procedure index is not valid, otherwise return 0
  */
  INDEX insertFirstStmt(PROCNAME procName, STMT stmt);

  //! insert stmt number of the last statement of the procedure at index procIndex
  /*!
	\param PROCNAME procedure name
	\return -1 if the procedure index is not valid, otherwise return 0
  */
  INDEX insertLastStmt(PROCNAME procName, STMT stmt);

  //! insert stmt number of the last statement of the procedure at index procIndex
  /*!
	\param INDEX procedure index in ProcTable
	\return -1 if the procedure index is not valid, otherwise return 0
  */
  INDEX insertCFG(PROCNAME procName, CFG* cfg);

  //! insert the varName(as index in varTable) that procName Modifies
  /*!
	\param PROCNAME procedure name
	\param VARNAME variable name
	\return -1 if the procedure index is not valid, otherwise return 0
  */
  INDEX insertModifies(PROCNAME procName, VARNAME varName);

  //! insert the varName(as index in varTable) that procName Uses
  /*!
	\param PROCNAME procedure name
	\param VARNAME variable name
	\return -1 if the procedure index is not valid, otherwise return 0
  */
  INDEX insertUses(PROCNAME procName, VARNAME varName);

  //! get the pointer to vector extra (used by Design Extractor)
  PROCINDEXLIST* getExtra();

  //! Get the procedure name of a given index
  /*!
	return empty string if indx is not valid (i.e. indx<0 or indx>noOfProc-1)
	\parm INDEX index of ProcName
	\return procedure name of indx
  */
  PROCNAME getProcName(INDEX indx);

  //! Get the index of a given procedure name
  /*!
	return -1 if procedure does not exist
	\param PROCNAME procedure name
	\return index of procName
  */
  INDEX getProcIndex(PROCNAME procName);

 // //! Get the name of the procedure the statement is in
 // /*!
	//return empty string if the statement does not exist
	//\param STMT statement number
	//\return name of the procedure that contains statement stmt
 // */
 // PROCNAME getProcedure(STMT stmt);

  //! Get the statement number of the 1st statement in the procedure
  //! return -1 if procedure does not exist
  STMT getFirstStmt(PROCNAME procName);

  //! Get the statement number of the last statement in the procedure
  //! return -1 if procedure does not exist
  STMT getLastStmt(PROCNAME procName);

  //! Get the pointer to the CFG that contains stmt
  //! return NULL if stmt does not exist
  CFG* getCFG(STMT stmt);

  //! Get all the procedure names in the ProcTable
  PROCLIST getAllProcNames();

  //! Get the number of procedures in the table
  int getNoOfProc();

  //! Get the variables(as index in varTable) that procName Modifies
  INDEXLIST getModifies(PROCNAME procName);

  //! Get the variables(as index in varTable) that procName Uses
  INDEXLIST getUses(PROCNAME procName);

  //! Check if a procedure index is valid
  bool isProcIndex(INDEX indx);

  //! Check if a procedure name is already in ProcTable
  bool isProcName(PROCNAME p);

 private:
  PROCLIST ProcList;				// an array of procedure names
  STMTLST FirstStmt;				// stmt number of the first statement in each procedure
  STMTLST LastStmt;					// stmt number of the last statement in each procedure
  CFGLIST CFGList;
  MODIFIES ModifiesList;			
  USES UsesList;
  PROCINDEXLIST extra; // for retrieving index of the procedure that contains a statement

  VarTable* varTable;
  int noOfProc;						// number of procedures currently stored in this ProcTable
};
