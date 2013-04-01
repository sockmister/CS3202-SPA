#include "OptimisedCaller.h"


OptimisedCaller::OptimisedCaller(AST * _ast, ProcTable * _procTable) {
	ast = _ast;
	procTable = _procTable;
}

vector<string> OptimisedCaller::getAllStmtLst(){
	return stmtLstVector;
}

vector<string> OptimisedCaller::getAllStmt(){
	return stmtNumberVector;
}

vector<string> OptimisedCaller::getAllAssignStmt(){
	return assignStmtNumberVector;
}

vector<string> OptimisedCaller::getAllWhileStmt(){
	return whileStmtNumberVector;
}

vector<string> OptimisedCaller::getAllIfStmt(){
	return ifStmtNumberVector;
}

vector<string> OptimisedCaller::getAllCallsStmt(){
	return callStmtNumberVector;
}

vector<string> OptimisedCaller::getAllConstant(){
	return constantVector;
}

void OptimisedCaller::generateOptimised(){
	generateStmtLst();
	generateStmtNum();
	generateAssignStmtNum();
	generateWhileStmtNum();
	generateIfStmtNum();
	generateCallsStmtNum();
	generateConstant();
}

void OptimisedCaller::generateStmtLst(){
	vector<int> getAll;
	vector<int> getAll1 = ast->getAllStmtNumByType("whileNode");
	vector<int> getAll2 = ast->getAllStmtNumByType("ifNode");
	getAll.insert(getAll.end(), getAll1.begin(), getAll1.end());
	getAll.insert(getAll.end(), getAll2.begin(), getAll2.end());
	for(int i=0; i<getAll.size(); i++){
		stmtLstVector.push_back( std::to_string(static_cast<long long>( getAll.at(i)+1)));
	}
	vector<string> allProcName =procTable->getAllProcNames();
	for(int i=0; i<getAll2.size(); i++){
		TNode stmtNode= ast->getStmtNode(getAll2.at(i));
		TNode childNode= ast->getChild(ast->getChild(stmtNode, 2), 0);
		int value= childNode.getStmtNumber();

		//int value = ast->getChild(ast->getStmtNode(getAll2.at(i)) ,2).getStmtNumber();
		stmtLstVector.push_back( std::to_string(static_cast<long long>(value)) );
	}
	for(int i=0; i< allProcName.size(); i++){
		stmtLstVector.push_back( std::to_string(static_cast<long long>( procTable->getFirstStmt(allProcName.at(i)))));
	}
}

void OptimisedCaller::generateStmtNum(){
	vector<int> getAll = ast->getAllStmtNumByType("assignNode");
	vector<int> getAll1 = ast->getAllStmtNumByType("whileNode");
	vector<int> getAll2 = ast->getAllStmtNumByType("ifNode");
	vector<int> getAll3 = ast->getAllStmtNumByType("callNode");
	getAll.insert(getAll.end(), getAll1.begin(), getAll1.end());
	getAll.insert(getAll.end(), getAll2.begin(), getAll2.end());
	getAll.insert(getAll.end(), getAll3.begin(), getAll3.end());
	stmtNumberVector = convertToStringResults(getAll);
}

void OptimisedCaller::generateAssignStmtNum(){
	vector<int> getAll = ast->getAllStmtNumByType("assignNode");
	assignStmtNumberVector = convertToStringResults(getAll);
}

void OptimisedCaller::generateWhileStmtNum(){
	vector<int> getAll = ast->getAllStmtNumByType("whileNode");
	whileStmtNumberVector = convertToStringResults(getAll);
}

void OptimisedCaller::generateIfStmtNum(){
	vector<int> getAll = ast->getAllStmtNumByType("ifNode");
	ifStmtNumberVector = convertToStringResults(getAll);
}

void OptimisedCaller::generateCallsStmtNum(){
	vector<int> getAll = ast->getAllStmtNumByType("callNode");
	callStmtNumberVector = convertToStringResults(getAll);
}

void OptimisedCaller::generateConstant(){
	constantVector = ast->getAllConstant();
}

vector<string> OptimisedCaller::convertToStringResults(vector<int> _intResults){
	vector<string> result;
	for(int i=0; i<_intResults.size(); i++){	
		string stringResult = std::to_string(static_cast<long long>( _intResults.at(i)));
		result.push_back(stringResult);
	}
	return result;
}
