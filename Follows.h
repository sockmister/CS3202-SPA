//Follows stores all the Follows relationship in the program

#pragma once

#include <vector>
#include <algorithm>

using namespace std;

/*!  \class Follows.
	 \brief Follows and Follows* is used to keep all Follows relationships in the program.
*/

typedef int STMT; //statement number
typedef vector<STMT> STMTLST;
typedef short ORDER;

class Follows{
 
  public:
  //! Default constructor.
  Follows();

  //! Set a follows link between firstStmt and secondStmt
  /*!
	secondStmt is the direct right sibling of firstStmt in AST
	\param firstStmt first statement number
	\parm  secondStmt second statement number
	
  */
  void setFollows(STMT firstStmt, STMT secondStmt);

  //! If order = 0, return statement that is the direct left sibling of stmt, If order = 1, return statement that is the direct right sibling of stmt 
  /*!
	\param order 0 or 1
	\parm stmt statement number
	\return otherwise -1
  */
  STMT getFollows(ORDER order, STMT stmt);

  //! If order = 0, return statement that is the direct left siblings of stmt, If order = 1, return statement that is the direct right siblings of stmt 
  /*!
	\param order 0 or 1
	\parm stmt statement number
	\return otherwise return a list which has -1 as the only element
  */
  STMTLST getFollowsStar(ORDER order, STMT stmt);

  //! If secondStmt directly follows firstStmt, return TRUE
  /*!
	\param firstStmt first statement number
	\parm  secondStmt second statement number
	\return FALSE if secondStmt does not Follows firstStmt
  */
  bool isFollows(STMT firstStmt, STMT secondStmt);

  //! If secondStmt follows firstStmt, return TRUE
  /*!
	\param firstStmt first statement number
	\parm  secondStmt second statement number
	\return FALSE if secondStmt does not Follows firstStmt
  */
  bool isFollowsStar(STMT firstStmt, STMT secondStmt);


 private:
	 vector<STMTLST> FollowsDB; //Each element in FollowsDB is a pair, the first is the left sibling, the second is the right sibling
};
