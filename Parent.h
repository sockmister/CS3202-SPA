#pragma once
#include <vector>

using namespace std;

/*!  \class Parent.
	 \brief Parent and Parent* is used to keep all the Parent relationships in the program.
*/

/*! \var typedef int STMT
	\brief A type definition for statement number.

*/

typedef int STMT; //statement number
typedef vector<STMT> STMTLST;

class Parent {
 
  public:
  //! Default constructor.
  Parent();

  //! Set parentStmt as Parent of childStmt
  /*!
	\param parentStmt Parent statement number
	\param childStmt Child statement number
  */
  void setParent(STMT parentStmt, STMT childStmt);

  //! return the statement number of the direct parent statement of childStmt
  /*!
	\param childStmt Child statement number
	\return -1 if childStmt has no parent
  */
  STMT getParent(STMT childStmt);

  //! return statement numbers of all direct and indirect parent statement of childStmt
  /*!
	\param childStmt Child statement number
	\return empty array if childStmt has no parent star
  */
  STMTLST getParentStar(STMT childStmt);

  //! return an array of statement numbers that contains all direct child statements of parentStmt if they exist
  /*!
	\param parentStmt Parent statement number
	\return empty array if parentStmt has no children statements
  */
  STMTLST getChild(STMT parentStmt);

  //! return an array of statement numbers that contains all child statements of parentStmt if they exist
  /*!
	\param parentStmt Parent statement number
	\return empty array if parentStmt has no children statements
  */
  STMTLST getChildStar(STMT parentStmt);

  //! if firstStmt is a parent of secondStmt, return TRUE
  /*!
	\param firstStmt Parent statement number
	\parm  secondStmt Child statement number
	\return FALSE if firstStmt is not a parent of secondStmt
  */
  bool isParent(STMT firstStmt, STMT secondStmt);

  //! if firstStmt is a parent star of secondStmt, return TRUE
  /*!
	\param firstStmt Parent statement number
	\parm  secondStmt Child statement number
	\return FALSE if firstStmt is not a parent star of secondStmt
  */
  bool isParentStar(STMT firstStmt, STMT secondStmt);

 private:
	 vector<STMTLST> ParentDB; 
};
