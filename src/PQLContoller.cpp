//#include "stdafx.h"
#include "PQLController.h"
#include "PreProcessor.h"
#include "queryTree.h"
#include "AST.h"
#include "evaluator.h"

PQLController::PQLController() {
}

PQLController::PQLController(PKB * inputPKB) {
	pkb = inputPKB;
}

// Maybe need accept 1 more argument
void PQLController::passInput(string queryInOneLine, list <string> &resultsList) {
	// While(still have queries)
	queryTree * tree = preProcessor.preProcess(queryInOneLine);	
	eval.insertAST(pkb->getAST());
	eval.insertFollows(pkb->getFollows());
	eval.insertParent(pkb->getParent());
	eval.insertModifies(pkb->getModifies());
	eval.insertUses(pkb->getUses());
	eval.insertVarTable(pkb->getVarTable());
	eval.insertCalls(pkb->getCalls());
	eval.insertProcTable(pkb->getProcTable());
	eval.insertAffects(pkb->getAffects());

	// Should work once Yong Wei modifies evaluator
	vector<string> results = eval.evaluateQuery(tree);
	delete tree;
	for(int i=0; i<results.size(); i++){
		//cout << results.at(i);
		resultsList.push_back(results.at(i));
	}
}