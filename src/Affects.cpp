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

/* Temp solution to getting procedure name of a stmt.
Change once faster access in PKB is working */
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
Change once faster access in PKB is working */
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
//	/*PROCLIST allProcNames = procTable->getAllProcNames();
//	PROCNAME firstProcedure = procTable->getProcedure(a1);
//	PROCNAME secondProcedure = procTable->getProcedure(a2);
//	if(firstProcedure == secondProcedure)
//		return firstProcedure;
//	else
//		return "-1";
//}

// Can be optimised to O(log n) search instead of O(n)
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

/* Method above without taking care a1 == a2 */
//vector<STMTLST> Affects::findAllPaths(STMT a1, STMT a2, STMTLST path) {
//	path.push_back(a1);
//	if(a1 == a2) {
//		vector<STMTLST> allPaths;
//		allPaths.push_back(path);
//		return allPaths;
//	}
//	vector<STMTLST> allPaths;
//	STMTLST nextStatements = cfg->nextStatement(1, a1);
//	if(!nextStatements.empty() && nextStatements[0] == -1) {
//		return allPaths;
//	}
//	for(size_t i = 0; i < nextStatements.size(); ++i) {
//		if(!contains(nextStatements[i], path)) {
//			vector<STMTLST> childPaths = findAllPaths(nextStatements[i], a2, path);
//			for(size_t i = 0; i < childPaths.size(); ++i) {
//				allPaths.push_back(childPaths[i]);
//			}
//		}
//	}
//	return allPaths;
//}

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

bool Affects::isAffects(STMT a1, STMT a2) {
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
	cfg = procTable->getCFG(procedure);
	/*if(!cfg->isNextStar(a1, a2))		// Not working with new isNext* in new CFG.cpp
		return false;*/

	if(isNotModifiedInAControlFlow(a1, a2, v))
		return true;
	
	return false;
}

// Is it Affects(a1, aX) & Affects*(aX, a2) ?
bool Affects::isAffectsStarRecurse(STMT a1, STMT a2) {
	if(isAffects(a1, a2))
		return true;
	STMTLST path;
	vector<STMTLST> allPaths = findAllPaths(a1, a2, path, true);
	STMTLST currPath;

	for(size_t i = 0; i < allPaths.size(); ++i) {
		currPath = allPaths[i];
		for(size_t j = 0; j < currPath.size(); ++j) {
			if(isAffects(a1, currPath[j]))
				if(isAffectsStarRecurse(currPath[j], a2))
					return true;
		}
	}
	return false;
}

bool Affects::isAffectsStar(STMT a1, STMT a2) {
	// Are a1 and a2 both assignment statements?
	if(ast->getNodeType(a1) != "assignNode" || ast->getNodeType(a2) != "assignNode")
		return false;

	// a1 and a2 in the same procedure?
	PROCNAME procedure = getSameProcedure(a1, a2);
	if(procedure == "-1")
		return false;

	// Control flow from a1 to a2?
	cfg = procTable->getCFG(procedure);
	/*if(!cfg->isNextStar(a1, a2))		// Not working with new isNext* in new CFG.cpp
		return false;*/

	// Is it Affects(a1, aX) & Affects*(aX, a2)
	if(isAffectsStarRecurse(a1, a2)) {
		return true;
	}
	return false;
}
