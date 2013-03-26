#pragma once

#include <vector>
#include <bitset>
#include <utility>
#include <queue>
#include "ProcTable.h"

using namespace std;

/*!  \class Calls.
	 \brief Calls is used to store all the Calls relationships for procedures in the program.
*/

typedef int PROCINDEX; //procedure index in ProcTable
typedef vector<PROCINDEX> PROCINDXLIST;
typedef string PROCNAME; //procedure name
typedef int STMT;

using namespace std;

class Calls {
 
  public:
  //! Default constructor
  Calls();
  //! Constructor
  Calls(ProcTable* theProcTable);

  //! set Calls relationship between two procedures (for Parser)
  /*!
	\param caller name of the procedure that calls another procedure
	\param callee name of the procedure that is called
	\param callStmt statement number of call statement (needed to compute Modifies and Uses for Call statements)
	\return true if the setting is successful
  */
  bool setCalls(PROCNAME caller, PROCNAME callee, STMT callStmt);

   //! set Calls relationship between two procedures (for Design Extractor)
  /*!
	\param caller index of the procedure that calls another procedure
	\param callee index of the procedure that is called
	\return true if the setting is successful
  */
  bool setCalls(PROCINDEX caller, PROCINDEX callee);

  //! pass temp to Design Extractor
  vector<pair<pair<PROCNAME, PROCNAME>, STMT>> getTemp();

  //! Get the indices of procedures that are directly called from procedure p
  /*!
	\return empty list if there is no such procedure; or return a list with only one element -1 if procedure p does not exist 
  */
  PROCINDXLIST getCallees(PROCNAME p);

  //! Get the indices of procedures that directly call procedure q
  /*!
	\return empty list if there is no such procedure; or return a list with only one element -1 if procedure q does not exist
  */
  PROCINDXLIST getCallers(PROCNAME q);

  //! Get the indices of procedures that are directly or indirectly called from procedure p 
  /*!
	\return empty list if there is no such procedure; or return a list with only one element -1 if procedure p does not exist 
  */
  PROCINDXLIST getCalleesStar(PROCNAME p);

  //! Get the indices of procedures that directly or indirectly call procedure q 
  /*!
	\return empty list if there is no such procedure; or return a list with only one element -1 if procedure q does not exist
  */
  PROCINDXLIST getCallersStar(PROCNAME q);

  //! If p directly calls q, return TRUE
  /*!
	\return FALSE otherwise
  */
  bool isCalls(PROCNAME p, PROCNAME q);

  //! If p directly or indirectly calls q, return TRUE
  /*!
	\return FALSE otherwise
  */
  bool isCallsStar(PROCNAME p, PROCNAME q);


 private:
	 vector<vector<bool>> CallsDB; 
	 vector<pair<pair<PROCNAME, PROCNAME>, STMT>> temp; //temporary storage for Calls during parsing (each element is of the form ((caller, callee), callStmt) )
	 ProcTable* procTable;
};
