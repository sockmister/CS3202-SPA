#include "Affects.h"


Affects::Affects(AST * ast, Modifies * mod, Uses * use, VarTable * var, ProcTable * proc)
{
	ast = ast;
	modifies = mod;
	uses = use;
	varTable = var;
	procTable = proc;
}

Affects::~Affects(void)
{
}

PROCNAME Affects::getProcedureName(STMT s) {
	// To do
	PROCLIST allProcNames = procTable->getAllProcNames();	// Need optimise
	for(size_t i = 0; i < allProcNames.size(); ++i) {
		PROCNAME procName = allProcNames.at(i);
		STMT firstSTMT = procTable->getFirstStmt(procName);
		STMT lastSTMT = procTable->getLastStmt(procName);
		if( firstSTMT < s < lastSTMT )
			return procName;
	}
}

bool Affects::isSameProcedure(STMT a1, STMT a2) {			// Need optimise
	PROCLIST allProcNames = procTable->getAllProcNames();
	for(size_t i = 0; i < allProcNames.size(); ++i) {
		PROCNAME procName = allProcNames.at(i);
		STMT firstSTMT = procTable->getFirstStmt(procName);
		STMT lastSTMT = procTable->getLastStmt(procName);
		if( firstSTMT < a1 < lastSTMT && firstSTMT < a2 < lastSTMT )
			return true;
	}
	return false;
}

STMTLST Affects::getSequentialControlFlow(STMT a1, STMT a2, STMTLST sortedControlFlow) {
	bool inMiddleOfLoop = false;
	if(!sortedControlFlow.empty())
		if(sortedControlFlow.front() < a1)
			bool inMiddleOfLoop = true;
		// if(==) then is a while statement, would've returned false by now
		// if(>) then not not in loop
	vector<STMT>::iterator it = sortedControlFlow.begin();
	while(*it < a1)
		++it;												// Get index of a1 in controlFlow
	vector<STMT>::iterator a1_index = it;
	++it;
	STMTLST sequentialControlFlow;
	bool hit_a2 = false;
	while(it != sortedControlFlow.end()) {
		if(*it == a2) {
			hit_a2 = true;
			break;
		}
		sequentialControlFlow.push_back(*it);
		++it;
	}
	if(!hit_a2) {
		while(*it != a2) {
			sequentialControlFlow.push_back(*it);
			++it;
		}
	}
	return sequentialControlFlow;
}

//bool Affects::isModifiedInCalls(STMT s) {
//	// To do
//	return false;
//}

// Can be optimised to O(log n) search instead of O(n)
bool contains(STMT s1, STMTLST path) {
	for(size_t i = 0; i < path.size(); ++i) {
		if(s1 == path[i])
			return true;
	}
	return false;
}

vector<STMTLST> Affects::findAllPaths(STMT a1, STMT a2, STMTLST path) {
	path.push_back(a1);
	if(a1 == a2) {
		vector<STMTLST> allPaths;
		allPaths.push_back(path);
		return allPaths;
	}
	vector<STMTLST> allPaths;
	STMTLST nextStatements = cfg->nextStatement(1, a1);
	if(!nextStatements.empty() && nextStatements[0] == -1) {
		return allPaths;
	}
	for(size_t i = 0; i < nextStatements.size(); ++i) {
		if(!contains(nextStatements[i], path)) {
			vector<STMTLST> childPaths = findAllPaths(nextStatements[i], a2, path);
			for(size_t i = 0; i < childPaths.size(); ++i) {
				allPaths.push_back(childPaths[i]);
			}
		}
	}
	return allPaths;
}

bool Affects::isNotModifiedInAControlFlow(STMT a1, STMT a2, VARNAME v) {
	STMTLST path;
	vector<STMTLST> allPaths = findAllPaths(a1, a2, path);
	for(size_t i = 0; i < allPaths.size(); ++i) {
		STMTLST currPath = allPaths[i];
		bool notModified = true;
		for(size_t j = 0; j < currPath.size(); ++j) {
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
			//else if is a call statement
			else if(ast->getNodeType(currStatement) == "callNode") {
				if(modifies->isModifies(currStatement, v)) {
					notModified = false;
					break;
				}
			}
		}
		if(notModified)
			return true;
	}
	return false;
}

STMTLST getTempControlFlow(string fileName, STMT s1, STMT s2) {
	STMTLST controlFlow;
	if(fileName == "s" && s1 == 1 && s2 == 5) {
		controlFlow.push_back(2);
		controlFlow.push_back(3);
		controlFlow.push_back(4);
	}
	if(fileName == "s" && s1 == 1 && s2 == 8) {
		controlFlow.push_back(2);
		controlFlow.push_back(3);
		controlFlow.push_back(4);
		controlFlow.push_back(5);
		controlFlow.push_back(6);
		controlFlow.push_back(7);
	}
	return controlFlow;
}

bool Affects::isAffects(STMT a1, STMT a2) {
	if(a1 == a2)											// Correct?
		return false;

	// a1 modifies value of a variable v?
	INDEXLST variableIndex = modifies->getVariable(a1);		// Why returns list? Need check if > 1 variables are returned ever
	if(variableIndex.empty())								// No variable to modify
		return false;
	VARNAME v = varTable->getVarName(variableIndex.at(0));
	
	// a2 uses the value of variable v?
	if(!uses->isUses(a2, v))
		return false;

	// a1 and a2 in the same procedure?
	if(!isSameProcedure(a1, a2))							// Can be optimised with stmtTable
		return false;

	// control flow from a1 to a2?
	if(!cfg->isNextStar(a1, a2))
		return false;
	
	// v is not modified in every assignment or procedure call statement in a control flow?
	//STMTLST sequentialControlFlow = getTempControlFlow("s", 1, 5);
	//STMTLST sequentialControlFlow = getTempControlFlow("s", 1, 8);
	/*STMTLST sortedControlFlow = cfg->nextStatementStar(0, a2);
	STMTLST sequentialControlFlow = getSequentialControlFlow(a1, a2, sortedControlFlow);*/
	//STMTLST sequentialControlFlow = cfg->affectsNext(a1, a2);

	if(isNotModifiedInAControlFlow(a1, a2, v))
		return true;
	
	//if(sequentialControlFlow.empty())
	//	return true;
	//vector<STMT>::iterator it = sequentialControlFlow.begin();
	//while(it != sequentialControlFlow.end()) {
	//	//if *it is an assign statement
	//	if(ast->getNodeType(*it) == "assignNode")
	//		if(modifies->isModifies(*it, v))
	//			return false;
	//	//else if *it is a call statement
	//	else if(ast->getNodeType(*it) == "callNode")
	//		if(modifies->isModifies(*it, v))
	//			return false;
	//		/*if(v is modified in any assignment statement in call)		// What if encounter another call statement in here? Recursion?
	//			return false;*/
	//	++it;
	//}
	return false;
}

bool Affects::isAffectsStar(STMT a1, STMT a2) {
	if(a1 == a2)											// Correct?
		return false;

	// a1 and a2 in the same procedure?
	if(!isSameProcedure(a1, a2))							// Can be optimised with stmtTable
		return false;

	// control flow from a1 to a2?
	if(!cfg->isNextStar(a1, a2))
		return false;

	if(isAffects(a1, a2))
		return true;

	/*--------------------------------------
			Need proper control flow
	--------------------------------------*/

	/*STMTLST sortedControlFlow = cfg->nextStatementStar(0, a1);
	STMTLST sequentialControlFlow = getSequentialControlFlow(a1, a2, sortedControlFlow);*/

	/*------------------------------------*/

	/*if(sequentialControlFlow.empty())
		return false;

	vector<STMT>::iterator it = sequentialControlFlow.begin();*/

	//while(it != sequentialControlFlow.end()) {
		/*if(isAffects(a1, *it))
			if(isAffectsStar(*it, a2))
				return true;
		++it;*/
	//}

	return false;
}