//Follows stores all the Follows relationship in the program

#pragma once


#include "Follows.h"


  // Default constructor
  Follows::Follows():FollowsDB(){
	  FollowsDB.push_back(STMTLST(2,-1)); //dummy first entry at index 0
  }

  // method to set a follows link between firstStmt and secondStmt
  // Note: secondStmt is the direct right sibling of firstStmt in AST
  void Follows::setFollows(STMT firstStmt, STMT secondStmt){

	  STMT stmt;
	  
	  if(firstStmt>secondStmt)
	  {
		  stmt = firstStmt;
	  }
	  else
	  {
		  stmt = secondStmt;
	  }

	  int size = FollowsDB.size();

	  if((size-1)<stmt)
	  {
		  for(int i=1; i<=(stmt-(size-1)); ++i)
		  {
			  FollowsDB.push_back(STMTLST(2,-1));
		  }
	  }

	  FollowsDB[firstStmt][1] = secondStmt;	
	  FollowsDB[secondStmt][0] = firstStmt;	
  }

  // If order = 0, return statement that is the direct left sibling of stmt
  // If order = 1, return statement that is the direct right sibling of stmt
  // Otherwise, return -1
  STMT Follows::getFollows(ORDER order, STMT stmt){
	  if(stmt >= FollowsDB.size()){
		  STMT temp;
		  return -1;
	  }
	  if(stmt < ((int)FollowsDB.size()) && ((order==1) || (order==0)))
	  {
		  return FollowsDB[stmt][order];
	  }
	  
	  return -1;
}


  // If order = 0, return a list of statements that are the left siblings of stmt
  // If order = 1, return a list of statements that are the right siblings of stmt
  // Otherwise, return a list which has -1 as the only element
  STMTLST Follows::getFollowsStar(ORDER order, STMT stmt){
	STMTLST temp;
	if(stmt >= FollowsDB.size())
		return temp;

	STMT s = FollowsDB[stmt][order];
	
	if(stmt < ((int)FollowsDB.size()))
	{
		if(s == -1)
		{
			temp.push_back(-1);
		}

		else{

			while(true)
			{
				if(s == -1)
				{
					break;
				}
				temp.push_back(s);
				s = FollowsDB[s][order];
			}
		}

	}

	else
	{
		temp.push_back(-1);
	}

	return temp;

}

  // If secondStmt directly follows firstStmt, return true
  // Otherwise, return false
  bool Follows::isFollows(STMT firstStmt, STMT secondStmt){
	if(firstStmt >= FollowsDB.size() || secondStmt >= FollowsDB.size())
		return false;

	if(firstStmt < ((int)FollowsDB.size()))
	{
		if(FollowsDB[firstStmt][1]==secondStmt) 
		{
			return true;
		}
	}

	return false;
  }

  // If secondStmt follows firstStmt, return true
  // Otherwise, return false
  bool Follows::isFollowsStar(STMT firstStmt, STMT secondStmt){
	if(firstStmt >= FollowsDB.size() || secondStmt >= FollowsDB.size())
		return false;

	STMT s = firstStmt;

	if(firstStmt < ((int)FollowsDB.size()))
	{
		while(s!=-1)
		{
			s = FollowsDB[s][1];
			if(s==secondStmt)
			{
				return true;
			}
		}
	}

	return false;
  }
