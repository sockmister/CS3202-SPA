#include "Affects.h"

Affects::Affects(AST * as, Modifies * mod, Uses * use, VarTable * var, ProcTable * proc, rootWhile * rootW)
{
	ast = as;
	modifies = mod;
	uses = use;
	varTable = var;
	procTable = proc;
	rootwhile = rootW;
}

Affects::~Affects(void)
{
}

void Affects::initializeCache(void) {	
	/* Inefficient way to get total number of statements */
	/*STMT totalNoOfStatements = 0;																	
	PROCLIST allProcNames = procTable->getAllProcNames();
	for(size_t i = 0; i < allProcNames.size(); ++i) {
		PROCNAME procName = allProcNames.at(i);
		STMT firstSTMT = procTable->getFirstStmt(procName);
		STMT lastSTMT = procTable->getLastStmt(procName);
		if(totalNoOfStatements < lastSTMT)
			totalNoOfStatements = lastSTMT;
	}*/
	//int totalNoOfStatements = stmttable->getSize();

	// Affects Cache
	for(size_t i = 0; i < totalNoOfStatements; ++i) {
		vector<int> tempVector;
		for(size_t j = 0; j < totalNoOfStatements; ++j) {
			tempVector.push_back(-1);
		}
		affectsCache.push_back(tempVector);
	}
	// AffectsStar Cache
	for(size_t i = 0; i < totalNoOfStatements; ++i) {
		vector<int> tempVector;
		for(size_t j = 0; j < totalNoOfStatements; ++j) {
			tempVector.push_back(-1);
		}
		affectsStarCache.push_back(tempVector);
	}

	// getAffectsStarCache
	for(size_t i = 0; i < 2; ++i) {
		vector<vector<string>> tempVector;
		vector<string> notCheckedYet;
		for(size_t j = 0; j < totalNoOfStatements; ++j) {
			tempVector.push_back(notCheckedYet);
		}
		getAffectsStarCache.push_back(tempVector);
	}
}

void Affects::initializeStmtTable(StmtTable * st) {
	stmttable = st;
	// Set total number of stmts
	PROCLIST allProcNames = procTable->getAllProcNames();
	for(size_t i = 0; i < allProcNames.size(); ++i) {
		PROCNAME procName = allProcNames.at(i);
		STMT firstSTMT = procTable->getFirstStmt(procName);
		STMT lastSTMT = procTable->getLastStmt(procName);
		if(totalNoOfStatements < lastSTMT)
			totalNoOfStatements = lastSTMT;
	}
}

PROCNAME Affects::getProcedureName(STMT s) {
	//int totalNoOfStatements = stmttable->getSize();
	if(0 < s < totalNoOfStatements) {
		return stmttable->getCaller(s);
	} 
	else {
		return "-1";	
	}
	/* Unoptimised version */
	/*PROCLIST allProcNames = procTable->getAllProcNames();
	for(size_t i = 0; i < allProcNames.size(); ++i) {
		PROCNAME procName = allProcNames.at(i);
		STMT firstSTMT = procTable->getFirstStmt(procName);
		STMT lastSTMT = procTable->getLastStmt(procName);
		if( firstSTMT < s < lastSTMT )
			return procName;
	}*/
}

PROCNAME Affects::getSameProcedure(STMT a1, STMT a2) {
	PROCNAME a1ProcName = getProcedureName(a1);
	PROCNAME a2ProcName = getProcedureName(a2);
	// Either of them are out of bound
	if(a1ProcName == "-1" || a2ProcName == "-1") {
		return "-1";
	}
	// They are the same
	else if(a1ProcName == a2ProcName) {
		return a1ProcName;
	}
	// They are not the same
	else {
		return "-1";
	}
}

/* New method for use once faster access in PKB is working */
//PROCNAME Affects::getSameProcedure(STMT a1, STMT a2) {
//	PROCLIST allProcNames = procTable->getAllProcNames();
//	PROCNAME firstProcedure = procTable->getProcedure(a1);
//	PROCNAME secondProcedure = procTable->getProcedure(a2);
//	if(firstProcedure == secondProcedure)
//		return firstProcedure;
//	else
//		return "-1";
//}

// Can be optimised to O(log n) search instead of O(n)									<-------- Can optimise*/
bool contains(STMT s1, STMTLST path) {
	for(size_t i = 0; i < path.size(); ++i) {
		if(s1 == path[i])
			return true;
	}
	return false;
}

/* Used in unoptimised Affects Star */
// Does not re-enter a while loop
// Returns all traversed paths after and not including a1 up to and including a2
vector<STMTLST> Affects::findAllPaths(STMT a1, STMT a2, STMTLST path, bool firstPass) {
	if(!firstPass) {
		path.push_back(a1);
		if(a1 == a2) {
			vector<STMTLST> allPaths;
			allPaths.push_back(path);
			return allPaths;
		}
	}
	vector<STMTLST> allPaths;
	STMTLST nextStatements = cfg->nextStatement(1, a1);
	for(size_t i = 0; i < nextStatements.size(); ++i) {
		if(nextStatements[i] != -1 && !contains(nextStatements[i], path)) {
			vector<STMTLST> childPaths = findAllPaths(nextStatements[i], a2, path, false);
			for(size_t i = 0; i < childPaths.size(); ++i) {
				allPaths.push_back(childPaths[i]);
			}
		}
	}
	return allPaths;
}

// Re-enters a while loop but exits when hits a non-while stmt twice
// Returns all traversed paths after and not including a1 up to and including a2
//vector<STMTLST> Affects::findAllPaths(STMT a1, STMT a2, STMTLST path, bool firstPass) {
//	if(!firstPass) {
//		path.push_back(a1);
//		if(a1 == a2) {
//			vector<STMTLST> allPaths;
//			allPaths.push_back(path);
//			return allPaths;
//		}
//	}
//	vector<STMTLST> allPaths;
//	STMTLST nextStatements = cfg->nextStatement(1, a1);
//	for(size_t i = 0; i < nextStatements.size(); ++i) {
//		// It haven't reached the end && It is not a while statement that is repeated
//		if(nextStatements[i] != -1 && !(ast->getNodeType(nextStatements[i]) != "whileNode" && contains(nextStatements[i], path))) {
//		//if(nextStatements[i] != -1 && !(contains(nextStatements[i], path) && nextStatements[i] == a2)) {
//			vector<STMTLST> childPaths = findAllPaths(nextStatements[i], a2, path, false);
//			for(size_t i = 0; i < childPaths.size(); ++i) {
//				allPaths.push_back(childPaths[i]);
//			}
//		}
//	}
//	return allPaths;
//}

/* Used in unoptimised Affects */
/* Correct but not optimised solution, not used */
bool Affects::isNotModifiedInAControlFlow(STMT a1, STMT a2, VARNAME v) {
	STMTLST path;
	vector<STMTLST> allPaths = findAllPaths(a1, a2, path, true);
	//vector<STMTLST> allPaths = findAllPaths(a1, a2, path);
	for(size_t i = 0; i < allPaths.size(); ++i) {
		STMTLST currPath = allPaths[i];
		bool notModified = true, checkedPath = false;
		for(size_t j = 0; j < currPath.size(); ++j) {
			checkedPath = true;
			STMT currStatement = currPath[j];
			if(currStatement == a1 || currStatement == a2)
				continue;
			// if it is an assign statement
			else if(stmttable->getNodeType(currStatement) == 3) {
				if(modifies->isModifies(currStatement, v)) {
					notModified = false;
					break;
				}
			}
			// else if is a call statement
			else if(stmttable->getNodeType(currStatement) == 11) {
				if(modifies->isModifies(currStatement, v)) {
					notModified = false;
					break;
				}
			}
		}
		if(notModified && checkedPath)
			return true;
	}
	return false;
}

/* Used in optimised Affects */
/* Above 2 methods combined and optimised */
vector<STMTLST> Affects::getUnmodifiedPath(STMT a1, STMT a2, VARNAME v, STMTLST path, bool firstPass) {
	if(!firstPass) {
		path.push_back(a1);
		if(a1 == a2) {
			// Check that v is not modified in this path. Yes? Don't add it. No? add it.
			bool isModified = false;
			// Cases:
				// There is no path		-> it won't be here
				// The path is 1 STMT	-> That stmt is a2, no need check -> No need go in for loop
				// The path is > 1 STMT	-> go inside for loop, dun check last index (a2)
			for(size_t i = 0; i < path.size()-1; ++i) {
				STMT currStatement = path[i];
				int currStatementType =	stmttable->getNodeType(currStatement);
				if(currStatementType == 3) {    // Assign stmt
					if(modifies->isModifies(currStatement, v)) {
						isModified = true;
						break;
					}
				}
				else if(currStatementType == 11) { // Call stmt
					if(modifies->isModifies(currStatement, v)) {
						isModified = true;
						break;
					}
				}
			}
			vector<STMTLST> allPaths;
			if(!isModified) {
				allPaths.push_back(path);
				return allPaths;
			}
			else
				return allPaths;
		}
	}
	vector<STMTLST> allPaths;
	STMTLST nextStatements = cfg->nextStatement(1, a1);
	for(size_t i = 0; i < nextStatements.size(); ++i) {
		if(nextStatements[i] != -1 && !contains(nextStatements[i], path)) {
			vector<STMTLST> childPaths = getUnmodifiedPath(nextStatements[i], a2, v, path, false);
			for(size_t i = 0; i < childPaths.size(); ++i) {
				allPaths.push_back(childPaths[i]);
			}
		}
	}
	return allPaths;
}

/* Used in optimised Affects */
bool Affects::isNotModifiedInAControlFlowOptimised(STMT a1, STMT a2, VARNAME v) {
	STMTLST path;
	if(getUnmodifiedPath(a1, a2, v, path, true).empty())
		return false;
	else
		return true;
}

bool Affects::isAffectsCompute(STMT a1, STMT a2) {
	// Are a1 and a2 both assignment statements?
	if(stmttable->getNodeType(a1) != 3 || stmttable->getNodeType(a2) != 3)
		return false;

	// a1 modifies value of a variable v?
	INDEXLST variableIndex = modifies->getVariable(a1);		// Need check if > 1 variables are ever returned
	if(variableIndex.empty())								// No variable to modify
		return false;
	VARNAME v = varTable->getVarName(variableIndex.at(0));
	
	// a2 uses the value of variable v?
	if(!uses->isUses(a2, v))
		return false;

	// a1 and a2 in the same procedure?
	PROCNAME procedure = getSameProcedure(a1, a2);
	if(procedure == "-1")
		return false;

	// Control flow from a1 to a2?
	cfg = procTable->getCFG(a1);
	/*if(!cfg->isNextStar(a1, a2))		//					<-------- Use if O(1)
		return false;*/

	/*if(isNotModifiedInAControlFlow(a1, a2, v))			<-------- Unoptimised version
		return true;*/
	if(isNotModifiedInAControlFlowOptimised(a1, a2, v))
		return true;

	return false;
}

/* Used in unoptimised Affects Star */
// Is it Affects(a1, aX) & Affects*(aX, a2) ?				<-------- Unoptimised version, unused
bool Affects::isAffectsStarRecurse(STMT a1, STMT a2) {
	/*if(AbstractWrapper::GlobalStop) {    // Uncomment to exit if take too long
		return false;
	}*/
	if(isAffectsNoScan(a1, a2))
		return true;
	STMTLST path;
	vector<STMTLST> allPaths = findAllPaths(a1, a2, path, true);
	STMTLST currPath;

	for(size_t i = 0; i < allPaths.size(); ++i) {
		currPath = allPaths[i];
		for(size_t j = 0; j < currPath.size(); ++j) {
			if(isAffectsNoScan(a1, currPath[j]))
				if(isAffectsStarRecurse(currPath[j], a2))
					return true;
		}
	}
	return false;
}

/* Used in optimised Affects Star */
vector<STMTLST> Affects::getAffectsStarPath(STMT original_a1, STMT a1, STMT a2, STMTLST path, bool firstPass) {
	/*if(AbstractWrapper::GlobalStop) {    // Uncomment to exit if take too long
		return false;
	}*/
	if(!firstPass) {
		path.push_back(a1);
		if(a1 == a2) {
			if(isAffectsNoScan(original_a1, a2)) {
				vector<STMTLST> allPaths;
				allPaths.push_back(path);
				return allPaths;
			}	
			for(size_t j = 0; j < path.size(); ++j) {
				if(isAffectsNoScan(original_a1, path[j])) {    // a1 is different from the start
					STMTLST newPath;
					if(!(getAffectsStarPath(path[j], path[j], a2, newPath, true).empty())) {
						vector<STMTLST> allPaths;
						allPaths.push_back(path);
						return allPaths;
					}
				}
			}
		}
	}
	vector<STMTLST> allPaths;
	STMTLST nextStatements = cfg->nextStatement(1, a1);
	for(size_t i = 0; i < nextStatements.size(); ++i) {
		if(nextStatements[i] != -1 && !contains(nextStatements[i], path)) {
			vector<STMTLST> childPaths = getAffectsStarPath(original_a1, nextStatements[i], a2, path, false);
			for(size_t i = 0; i < childPaths.size(); ++i) {
				allPaths.push_back(childPaths[i]);
			}
		}
	}
	return allPaths;
}

/* Used in optimised Affects Star */
bool Affects::isAffectsStarRecurseOptimised(STMT a1, STMT a2) {
	STMTLST newPath;
	if(getAffectsStarPath(a1, a1, a2, newPath, true).empty())
		return false;
	else
		return true;
}

/* Used in redesigned Affects Star */
VARLIST Affects::getVariablesUsedinStmt(STMT s) {
	INDEXLIST varIndexList = uses->getVariable(s);
	VARLIST varNameList;
	for(size_t i = 0; i < varIndexList.size(); ++i) {
		varNameList.push_back(varTable->getVarName(varIndexList.at(i)));    // <-------- Can optimise
	}
	return varNameList;
}

/* Used in redesigned Affects Star */
STMTLST Affects::getStmtLstAffectingStmt(STMT s, STMTLST range) {
	VARNAME v;
	VARLIST variablesUsedInS = getVariablesUsedinStmt(s);
	STMTLST modifiesAssignList;
	for(size_t j = 0; j < variablesUsedInS.size(); ++j) {
		v = variablesUsedInS[j];
		STMTLST modifiesList = modifies->getStatements(v);
		for(size_t i = 0; i < modifiesList.size(); ++i) {
			STMT currStmt = modifiesList.at(i);
			if(currStmt >= range[0]) {
				if(stmttable->getNodeType(currStmt) == 3 && !contains(currStmt, modifiesAssignList)) {
					if(isAffectsNoScan(currStmt, s)) {
						modifiesAssignList.push_back(currStmt);
						/*if(isAffects(a1, currStmt))		<-------- Can optimise (If this is true then affectsStar is true already)
							return true;*/					
					}
				}
			}
			if(currStmt > range[1] )
				break;
		}
	}
	return modifiesAssignList;
}

/* Used in redesigned Affects Star */
vector<STMTLST> Affects::getAffectsStarRedesignedPath(STMT a1, STMT a2, STMTLST range, STMTLST path, bool firstPass) {
	/*if(AbstractWrapper::GlobalStop) {    // Uncomment to exit if take too long
		return false;
	}*/
	if(!firstPass) {
		path.push_back(a2);
		if(a1 == a2) {
			vector<STMTLST> allPaths;
			allPaths.push_back(path);
			return allPaths;
		}
	}
	vector<STMTLST> allPaths;
	STMTLST nextStatements = getStmtLstAffectingStmt(a2, range);
	for(size_t i = 0; i < nextStatements.size(); ++i) {
		if(nextStatements[i] != -1 && !contains(nextStatements[i], path)) {
			vector<STMTLST> childPaths = getAffectsStarRedesignedPath(a1, nextStatements[i], range, path, false);
			for(size_t i = 0; i < childPaths.size(); ++i) {
				allPaths.push_back(childPaths[i]);
			}
		}
	}
	return allPaths;
}

/* Used in redesigned version */
bool Affects::isAffectsStarRecurseRedesigned(STMT a1, STMT a2) {
	STMTLST newPath;
	STMTLST range = getRange(a1, a2);
	if(getAffectsStarRedesignedPath(a1, a2, range, newPath, true).empty())
		return false;
	else
		return true;
}

/* Used in redesigned Affects Star */
STMTLST Affects::getRange(STMT a1, STMT a2) {
	if(a1 > a2) {
		int temp = a2;
		a2 = a1;
		a1 = a2;
	}
	STMTLST range;
	STMT start = rootwhile->getWhileRootOfStmt(a1);
	if(start == 0) {
		start = a1;
	}
	range.push_back(start);

	STMT rootWhileA2 = rootwhile->getWhileRootOfStmt(a2);
	STMT end = rootWhileA2;
	if(end == 0) {
		end = a2;
	}
	else { 
		while(rootwhile->getWhileRootOfStmt(end) == rootWhileA2) {
			++end;
		}
		--end;
	}
	range.push_back(end);
	return range;
}


bool Affects::isAffectsStarCompute(STMT a1, STMT a2) {
	// Are a1 and a2 both assignment statements?
	if(stmttable->getNodeType(a1) != 3 || stmttable->getNodeType(a2) != 3)
		return false;

	// a1 and a2 in the same procedure?
	PROCNAME procedure = getSameProcedure(a1, a2);
	if(procedure == "-1")
		return false;

	// Control flow from a1 to a2?
	cfg = procTable->getCFG(a1);
	if(!cfg->isNextStar(a1, a2))		// Not working with new isNext* in new CFG.cpp
		return false;

	// Is it Affects(a1, aX) & Affects*(aX, a2)
	//if(isAffectsStarRecurse(a1, a2)) {		//		<-------- Unoptimised version
	//	return true;
	//}
	//if(isAffectsStarRecurseOptimised(a1, a2)) { //		<-------- Optimised version
	//	return true;
	//}
	if(isAffectsStarRecurseRedesigned(a1, a2)) { //   <-------- Redesigned version
		return true;
	}

	return false;
}

int Affects::getFromAffectsStarCache(STMT query_a1, STMT query_a2, bool scan) {
	bool algoAnswer;
	int cacheAnswer;
	INDEX a1 = query_a1 - 1; 
	INDEX a2 = query_a2 - 1;
	if(a1 < 0 || a2 < 0 || a1 >= affectsStarCache.size() || a2 >= affectsStarCache.size())
		return -1;
	cacheAnswer = affectsStarCache[a1][a2];
	
	/* Answer not in cache */
	if(cacheAnswer == -1 && scan) {
		for(size_t i = 0; i < affectsStarCache[a1].size(); ++i) {
			if(affectsStarCache[a1][i] == -1) {
				query_a2 = i + 1;
				algoAnswer = isAffectsStarCompute(query_a1, query_a2);
				if(algoAnswer == true)
					affectsStarCache[a1][i] = 1;
				else
					affectsStarCache[a1][i] = 0;
			}
		}
		return affectsStarCache[a1][a2];
	}

	/* Answer not in cache */
	else if(cacheAnswer == -1 && !scan) {
		algoAnswer = isAffectsStarCompute(query_a1, query_a2);
		if(algoAnswer == true)
			affectsStarCache[a1][a2] = 1;
		else
			affectsStarCache[a1][a2] = 0;
		return affectsStarCache[a1][a2];
	}

	/* Answer is in cache */
	else
		return cacheAnswer;
}

int Affects::getFromAffectsCache(STMT query_a1, STMT query_a2, bool scan) {
	bool algoAnswer;
	int cacheAnswer;
	INDEX a1 = query_a1 - 1;
	INDEX a2 = query_a2 - 1;
	if(a1 < 0 || a2 < 0 || a1 >= affectsCache.size() || a2 >= affectsCache.size())
		return -1;
	cacheAnswer = affectsCache[a1][a2];
	
	/* Answer not in cache */
	if(cacheAnswer == -1 && scan) {
		for(size_t i = 0; i < affectsCache[a1].size(); ++i) {
			if(affectsCache[a1][i] == -1) {
				query_a2 = i + 1;
				algoAnswer = isAffectsCompute(query_a1, query_a2);
				if(algoAnswer == true)
					affectsCache[a1][i] = 1;
				else
					affectsCache[a1][i] = 0;
			}
		}
		return affectsCache[a1][a2];
	}

	/* Answer not in cache */
	else if(cacheAnswer == -1 && !scan) {
		algoAnswer = isAffectsCompute(query_a1, query_a2);
		if(algoAnswer == true)
			affectsCache[a1][a2] = 1;
		else
			affectsCache[a1][a2] = 0;
		return affectsCache[a1][a2];
	}

	/* Answer is in cache */
	else
		return cacheAnswer;
}

bool Affects::isAffectsNoScan(STMT a1, STMT a2) {
	int answer = getFromAffectsCache(a1, a2, false);
	if(answer == 1)
		return true;
	else    // answer == 0
		return false;
}

//// Returns whether if affectsBip(a1, a2) is true
//bool isAffectsBip(STMT a1, STMT a2) {
//	return false;
//}
//
//// Returns whether if affectsBip*(a1, a2) is true
//bool isAffectsBipStar(STMT a1, STMT a2) {
//	return false;
//}

bool Affects::isAffectsStar(STMT a1, STMT a2) {
	int answer = getFromAffectsStarCache(a1, a2, false);
	if(answer == 1)
		return true;
	else    // answer == 0
		return false;	
}

bool Affects::isAffects(STMT a1, STMT a2) {
	int answer = getFromAffectsCache(a1, a2, true);
	if(answer == 1)
		return true;
	else    // answer == 0
		return false;
}

/* Used in redesigned Affects Star */
vector<STMTLST> Affects::getAffectsStarRedesignedPathBackwards(STMT a2, STMTLST range, STMTLST path, bool firstPass) {
	/*if(AbstractWrapper::GlobalStop) {    // Uncomment to exit if take too long
		return false;
	}*/
	if(!firstPass) {
		path.push_back(a2);
		/*if(range[0] == a2) {
			vector<STMTLST> allPaths;
			allPaths.push_back(path);
			return allPaths;
		}*/
	}
	vector<STMTLST> allPaths;
	STMTLST nextStatements = getStmtLstAffectingStmt(a2, range);
	for(size_t i = 0; i < nextStatements.size(); ++i) {
		if(nextStatements[i] != -1 && !contains(nextStatements[i], path)) {
			vector<STMTLST> childPaths = getAffectsStarRedesignedPathBackwards(nextStatements[i], range, path, false);
			for(size_t j = 0; j < childPaths.size(); ++j) {
				allPaths.push_back(childPaths[j]);
			}
		}
	}
	allPaths.push_back(path);
	return allPaths;
}

STMTLST Affects::getRangeFromProcStart(STMT a2) {
	PROCNAME procName = stmttable->getCaller(a2);
	STMT start = procTable->getFirstStmt(procName);
	STMTLST range;
	range.push_back(start);

	STMT rootWhileA2 = rootwhile->getWhileRootOfStmt(a2);
	STMT end = rootWhileA2;
	if(end == 0) {
		end = a2;
	}
	else { 
		while(rootwhile->getWhileRootOfStmt(end) == rootWhileA2) {
			++end;
		}
		--end;
	}
	range.push_back(end);
	return range;
}


STMTLST Affects::getRangeTillProcEnd(STMT a1) {
	/*if(a1 > a2) {
		int temp = a2;
		a2 = a1;
		a1 = a2;
	}*/
	STMTLST range;
	STMT start = rootwhile->getWhileRootOfStmt(a1);
	if(start == 0) {
		start = a1;
	}
	range.push_back(start);

	PROCNAME procName = stmttable->getCaller(a1);
	STMT end = procTable->getLastStmt(procName);
	range.push_back(end);
	return range;
}



/* Used in getAffectsStar */
STMTLST Affects::getStmtLstAffectedByStmt(STMT s, STMTLST range) {
	STMTLST affectedAssignList;
	INDEXLST variableIndex = modifies->getVariable(s);		// Need check if > 1 variables are ever returned
	if(variableIndex.empty())								// No variable to modify
		return affectedAssignList;
	VARNAME v = varTable->getVarName(variableIndex.at(0));
	STMTLST usesList = uses->getStatement(v);
	
	for(size_t i = 0; i < usesList.size(); ++i) {
		STMT currStmt = usesList.at(i);
		if(currStmt >= range[0]) {
			if(stmttable->getNodeType(currStmt) == 3 && !contains(currStmt, affectedAssignList)) {
				if(isAffectsNoScan(s, currStmt)) {
					affectedAssignList.push_back(currStmt);
					/*if(isAffects(a1, currStmt))		<-------- Can optimise (If this is true then affectsStar is true already)
						return true;*/					
				}
			}
		}
		if(currStmt > range[1] )
			break;
	}
	return affectedAssignList;
}


vector<STMTLST> Affects::getAffectsStarRedesignedPathForwards(STMT a1, STMTLST range, STMTLST path, bool firstPass) {
	/*if(AbstractWrapper::GlobalStop) {    // Uncomment to exit if take too long
		return false;
	}*/
	if(!firstPass) {
		path.push_back(a1);
		/*if(range[0] == a2) {
			vector<STMTLST> allPaths;
			allPaths.push_back(path);
			return allPaths;
		}*/
	}
	vector<STMTLST> allPaths;
	STMTLST nextStatements = getStmtLstAffectedByStmt(a1, range);    // <-------- Need modify
	for(size_t i = 0; i < nextStatements.size(); ++i) {
		if(nextStatements[i] != -1 && !contains(nextStatements[i], path)) {
			vector<STMTLST> childPaths = getAffectsStarRedesignedPathForwards(nextStatements[i], range, path, false);
			for(size_t j = 0; j < childPaths.size(); ++j) {
				allPaths.push_back(childPaths[j]);
			}
		}
	}
	allPaths.push_back(path);
	return allPaths;
}



/* Used in redesigned version */
vector<string> Affects::getAffectsStarRecurseRedesignedBackwards(STMT a2) {
	STMTLST newPath, results;
	vector<string> stringResults;
	STMTLST range = getRangeFromProcStart(a2);
	
	/* get all the unique results from here*/ 
	vector<STMTLST> allAffectsStarPaths = getAffectsStarRedesignedPathBackwards(a2, range, newPath, true);

	for(size_t i = 0; i < allAffectsStarPaths.size(); ++i) {
		for(size_t j = 0; j < allAffectsStarPaths[i].size(); ++j) {
			if(!contains(allAffectsStarPaths[i][j], results)) {
				results.push_back(allAffectsStarPaths[i][j]);
				stringResults.push_back(to_string(allAffectsStarPaths[i][j]));
			}
		}
	}
	
	return stringResults;
}

vector<string> Affects::getAffectsStarRecurseRedesignedForwards(STMT a1) {
	STMTLST newPath, results;
	vector<string> stringResults;
	STMTLST range = getRangeTillProcEnd(a1);
	
	/* get all the unique results from here*/ 
	vector<STMTLST> allAffectsStarPaths = getAffectsStarRedesignedPathForwards(a1, range, newPath, true);

	for(size_t i = 0; i < allAffectsStarPaths.size(); ++i) {
		for(size_t j = 0; j < allAffectsStarPaths[i].size(); ++j) {
			if(!contains(allAffectsStarPaths[i][j], results)) {
				results.push_back(allAffectsStarPaths[i][j]);
				stringResults.push_back(to_string(allAffectsStarPaths[i][j]));
			}
		}
	}
	
	return stringResults;
}

vector<string> Affects::getAffectsStarCompute(int order, STMT a) {
	vector<string> stringResults;
	// Is a2 a assignment statements?
	if(stmttable->getNodeType(a) != 3)
		return stringResults;

	// a1 and a2 in the same procedure?
	/*PROCNAME procedure = getSameProcedure(a1, a2);
	if(procedure == "-1")
		return false;*/

	// Control flow from a1 to a2?
	//cfg = procTable->getCFG(a1);
	//if(!cfg->isNextStar(a1, a2))		// Not working with new isNext* in new CFG.cpp
	//	return false;

	// Is it Affects(a1, aX) & Affects*(aX, a2)
	//if(isAffectsStarRecurse(a1, a2)) {		//		<-------- Unoptimised version
	//	return true;
	//}
	//if(isAffectsStarRecurseOptimised(a1, a2)) { //		<-------- Optimised version
	//	return true;
	//}
	if(order == 0) {
		return getAffectsStarRecurseRedesignedBackwards(a); //   <-------- Redesigned version
	}
	else if(order == 1)
		return getAffectsStarRecurseRedesignedForwards(a);
	else 
		return stringResults;
}


vector<string> Affects::getFromGetAffectsStarCache(int order, STMT query_a) {
	
	vector<string> cacheAnswer;
	vector<string> algoAnswer;

	INDEX a = query_a - 1;
	if(a < 0 || a >= affectsStarCache.size())
		return cacheAnswer;
	
	cacheAnswer = getAffectsStarCache[order][a];
	
	/* Answer not in cache */
	/*if(cacheAnswer == -1 && scan) {
		for(size_t i = 0; i < affectsStarCache[a1].size(); ++i) {
			if(affectsStarCache[a1][i] == -1) {
				query_a2 = i + 1;
				algoAnswer = isAffectsStarCompute(query_a1, query_a2);
				if(algoAnswer == true)
					affectsStarCache[a1][i] = 1;
				else
					affectsStarCache[a1][i] = 0;
			}
		}
		return affectsStarCache[a1][a2];
	}*/

	/* Answer not in cache */
	if(cacheAnswer.empty()) {
		algoAnswer = getAffectsStarCompute(order, query_a);
		if(!algoAnswer.empty()) {
			getAffectsStarCache[order][a] = algoAnswer;
		}
		else {
			algoAnswer.push_back("-1");
			getAffectsStarCache[order][a] = algoAnswer;
		}
		return getAffectsStarCache[order][a];
	}

	/* Answer is in cache */
	else
		return cacheAnswer;
}

vector<string> Affects::getAffectsStar(int order, STMT a) {
	vector<string> emptyResults;
	vector<string> results = getFromGetAffectsStarCache(order, a);

	if(results.empty())
		return emptyResults;
	else if(results[0] == "-1")
		return emptyResults;
	else
		return results;
	//if(answer == 1)
	//	return true;
	//else    // answer == 0
	//	return false;
		
		

	/*STMTLST affectsList = getAffectsStarCompute(order, a2);
	return affectsList;*/
	


	//int answer = getFromAffectsCache(a1, a2, true);
	//if(answer == 1)
	//	return true;
	//else    // answer == 0
	//	return false;
}

void Affects::testFindAllPaths(STMT a1, STMT a2) {
	STMTLST path;
	vector<STMTLST> allPaths;
	cfg = procTable->getCFG(a1);
	//if(cfg->isNextStar(a1, a2)) {
		allPaths = findAllPaths(a1, a2, path, true);
		cout << "\n";
	//}
}