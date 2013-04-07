#include "Affects.h"

Affects::Affects(AST * as, Modifies * mod, Uses * use, VarTable * var, ProcTable * proc)
{
	ast = as;
	modifies = mod;
	uses = use;
	varTable = var;
	procTable = proc;
}

Affects::~Affects(void)
{
}

void Affects::initializeCache(void) {
	/* Inefficient way to get total number of statements								<-------- Can optimise */
	STMT totalNoOfStatements = 0;																	
	PROCLIST allProcNames = procTable->getAllProcNames();
	for(size_t i = 0; i < allProcNames.size(); ++i) {
		PROCNAME procName = allProcNames.at(i);
		STMT firstSTMT = procTable->getFirstStmt(procName);
		STMT lastSTMT = procTable->getLastStmt(procName);
		if(totalNoOfStatements < lastSTMT)
			totalNoOfStatements = lastSTMT;
	}
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
}

/* Temp solution to getting procedure name of a stmt.
Change once faster access in PKB is working												<-------- Can optimise*/
PROCNAME Affects::getProcedureName(STMT s) {
	PROCLIST allProcNames = procTable->getAllProcNames();
	for(size_t i = 0; i < allProcNames.size(); ++i) {
		PROCNAME procName = allProcNames.at(i);
		STMT firstSTMT = procTable->getFirstStmt(procName);
		STMT lastSTMT = procTable->getLastStmt(procName);
		if( firstSTMT < s < lastSTMT )
			return procName;
	}
}

/* Temp solution to checking if 2 statments belong to same procedure
Change once faster access in PKB is working 											<-------- Can optimise*/
PROCNAME Affects::getSameProcedure(STMT a1, STMT a2) {				
	PROCLIST allProcNames = procTable->getAllProcNames();
	for(size_t i = 0; i < allProcNames.size(); ++i) {
		PROCNAME procName = allProcNames.at(i);
		STMT firstSTMT = procTable->getFirstStmt(procName);
		STMT lastSTMT = procTable->getLastStmt(procName);
		if( (firstSTMT <= a1  && a1 <= lastSTMT && firstSTMT <= a2 && a2 <= lastSTMT) )
			return procName;
	}
	return "-1";
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

/* Correct but not optimised solution */
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
			else if(ast->getNodeType(currStatement) == "assignNode") {
				if(modifies->isModifies(currStatement, v)) {
					notModified = false;
					break;
				}
			}
			// else if is a call statement
			else if(ast->getNodeType(currStatement) == "callNode") {
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
				string currStatementType = ast->getNodeType(currStatement);
				if(currStatementType == "assignNode") {
					if(modifies->isModifies(currStatement, v)) {
						isModified = true;
						break;
					}
				}
				else if(currStatementType == "callNode") {
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

bool Affects::isNotModifiedInAControlFlowOptimised(STMT a1, STMT a2, VARNAME v) {
	STMTLST path;
	if(getUnmodifiedPath(a1, a2, v, path, true).empty())
		return false;
	else
		return true;
}

bool Affects::isAffectsCompute(STMT a1, STMT a2) {
	// Are a1 and a2 both assignment statements?
	if(ast->getNodeType(a1) != "assignNode" || ast->getNodeType(a2) != "assignNode")
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
	/*if(!cfg->isNextStar(a1, a2))		// Not working with new isNext* in new CFG.cpp
		return false;*/

	/*if(isNotModifiedInAControlFlow(a1, a2, v))			<-------- Unoptimised version
		return true;*/
	if(isNotModifiedInAControlFlowOptimised(a1, a2, v))
		return true;

	return false;
}

// Is it Affects(a1, aX) & Affects*(aX, a2) ?				<-------- Unoptimised version
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

bool Affects::isAffectsStarRecurseOptimised(STMT a1, STMT a2) {
	STMTLST newPath;
	if(getAffectsStarPath(a1, a1, a2, newPath, true).empty())
		return false;
	else
		return true;
}

bool Affects::isAffectsStarCompute(STMT a1, STMT a2) {
	// Are a1 and a2 both assignment statements?
	if(ast->getNodeType(a1) != "assignNode" || ast->getNodeType(a2) != "assignNode")   //is getNodeType...O(1)?             <-------- Can optimise
		return false;

	// a1 and a2 in the same procedure?
	PROCNAME procedure = getSameProcedure(a1, a2);
	if(procedure == "-1")
		return false;

	// Control flow from a1 to a2?
	cfg = procTable->getCFG(a1);
	/*if(!cfg->isNextStar(a1, a2))		// Not working with new isNext* in new CFG.cpp
		return false;*/

	// Is it Affects(a1, aX) & Affects*(aX, a2)
	/*if(isAffectsStarRecurse(a1, a2)) {			<-------- Unoptimised version
		return true;
	}*/
	STMTLST path;
	if(isAffectsStarRecurseOptimised(a1, a2)) {
		return true;
	}
	return false;
}

int Affects::getFromAffectsStarCache(STMT query_a1, STMT query_a2, bool scan) {
	bool algoAnswer;
	int cacheAnswer;
	INDEX a1 = query_a1 - 1; 
	INDEX a2 = query_a2 - 1;
	if(a1 < 0 || a2 < 0 || a1 > affectsStarCache.size() || a2 > affectsStarCache.size())
		return false;
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
	if(a1 < 0 || a2 < 0 || a1 > affectsCache.size() || a2 > affectsCache.size())
		return false;
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
