#include "OptimisedCaller.h"


OptimisedCaller::OptimisedCaller(AST * _ast, ProcTable * _procTable,  Calls * _calls, Follows * _follows, Modifies * _modifies, Parent * _parent, Uses * _uses ) {
	ast = _ast;
	procTable = _procTable;
	calls = _calls;
	follows = _follows;
	modifies = _modifies;
	parent = _parent; 
	uses = _uses;
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

vector<string> OptimisedCaller::getStmtLstStmt(int stmtLstNum){
	return stmtLstTable.at(stmtLstNum);
}

vector<string> OptimisedCaller::getIfStmtLstStmt(int stmtLstNum){
	return ifStmtLstTable.at(stmtLstNum);
}


vector<string> OptimisedCaller::getFollows(int order, int stmtNum, string type){
	if(type=="stmt" || type=="prog_line"){
		if(order==0) return follows_Right_Stmt.at(stmtNum);
		if(order==1) return follows_Left_Stmt.at(stmtNum);
	}else if(type=="assign"){
		if(order==0) return follows_Right_Assign.at(stmtNum);
		if(order==1) return follows_Left_Assign.at(stmtNum);
	}else if(type=="while"){
		if(order==0) return follows_Right_While.at(stmtNum);
		if(order==1) return follows_Left_While.at(stmtNum);
	}else if(type=="if"){
		if(order==0) return follows_Right_If.at(stmtNum);
		if(order==1) return follows_Left_If.at(stmtNum);
	}else if(type=="calls"){
		if(order==0) return follows_Right_Calls.at(stmtNum);
		if(order==1) return follows_Left_Calls.at(stmtNum);
	}
}

vector<string> OptimisedCaller::getFollowsStar(int order, int stmtNum, string type){
	if(type=="stmt" || type=="prog_line"){
		if(order==0) return followsStar_Right_Stmt.at(stmtNum);
		if(order==1) return followsStar_Left_Stmt.at(stmtNum);
	}else if(type=="assign"){
		if(order==0) return followsStar_Right_Assign.at(stmtNum);
		if(order==1) return followsStar_Left_Assign.at(stmtNum);
	}else if(type=="while"){
		if(order==0) return followsStar_Right_While.at(stmtNum);
		if(order==1) return followsStar_Left_While.at(stmtNum);
	}else if(type=="if"){
		if(order==0) return followsStar_Right_If.at(stmtNum);
		if(order==1) return followsStar_Left_If.at(stmtNum);
	}else if(type=="calls"){
		if(order==0) return followsStar_Right_Calls.at(stmtNum);
		if(order==1) return followsStar_Left_Calls.at(stmtNum);
	}
}

vector<string> OptimisedCaller::getNextStar(int order, int stmtNum, string type){
	if(type=="stmt" || type=="prog_line"){
		if(order==0) return nextStar_Right_Stmt.at(stmtNum);
		if(order==1) return nextStar_Left_Stmt.at(stmtNum);
	}else if(type=="assign"){
		if(order==0) return nextStar_Right_Assign.at(stmtNum);
		if(order==1) return nextStar_Left_Assign.at(stmtNum);
	}else if(type=="while"){
		if(order==0) return nextStar_Right_While.at(stmtNum);
		if(order==1) return nextStar_Left_While.at(stmtNum);
	}else if(type=="if"){
		if(order==0) return nextStar_Right_If.at(stmtNum);
		if(order==1) return nextStar_Left_If.at(stmtNum);
	}else if(type=="calls"){
		if(order==0) return nextStar_Right_Calls.at(stmtNum);
		if(order==1) return nextStar_Left_Calls.at(stmtNum);
	}
}

void OptimisedCaller::initialiseAll(){
	vector<string> empty;
	for(int i=0; i<=stmtNumberVector.size(); i++){
		//follows
		follows_Left_Assign.push_back(empty);
		follows_Left_Calls.push_back(empty);
		follows_Left_While.push_back(empty);
		follows_Left_If.push_back(empty);
		follows_Left_Stmt.push_back(empty);

		follows_Right_Assign.push_back(empty);
		follows_Right_Calls.push_back(empty);
		follows_Right_While.push_back(empty);
		follows_Right_If.push_back(empty);
		follows_Right_Stmt.push_back(empty);

		//followsStar
		followsStar_Left_Assign.push_back(empty);
		followsStar_Left_Calls.push_back(empty);
		followsStar_Left_While.push_back(empty);
		followsStar_Left_If.push_back(empty);
		followsStar_Left_Stmt.push_back(empty);

		followsStar_Right_Assign.push_back(empty);
		followsStar_Right_Calls.push_back(empty);
		followsStar_Right_While.push_back(empty);
		followsStar_Right_If.push_back(empty);
		followsStar_Right_Stmt.push_back(empty);

		//nextStar
		nextStar_Left_Assign.push_back(empty);
		nextStar_Left_Calls.push_back(empty);
		nextStar_Left_While.push_back(empty);
		nextStar_Left_If.push_back(empty);
		nextStar_Left_Stmt.push_back(empty);

		nextStar_Right_Assign.push_back(empty);
		nextStar_Right_Calls.push_back(empty);
		nextStar_Right_While.push_back(empty);
		nextStar_Right_If.push_back(empty);
		nextStar_Right_Stmt.push_back(empty);
	}

}

void OptimisedCaller::generateOptimised(){
	
	generateStmtLst();
	generateStmtNum();
	generateAssignStmtNum();
	generateWhileStmtNum();
	generateIfStmtNum();
	generateCallsStmtNum();
	generateConstant();
	generateStmtLstTable();
	generateIfStmtLstTable();
//	initialiseAll();
//	genereateFollows();
//	genereateFollowsStar();
//	genereateNextStar();

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
	set<string> s;
	for(vector<string>::iterator it = constantVector.begin(); it != constantVector.end(); ++it){
		s.insert(*it);
	}

	constantVector.assign(s.begin(), s.end());
}


void OptimisedCaller::genereateFollows(){
	for(int i=1; i<=stmtNumberVector.size(); i++){
		string leftString = stmtNumberVector.at(i-1);
		int leftNum = atoi( leftString.c_str() );
		vector<string> isFollowsVector;
		for(int j=0; j<assignStmtNumberVector.size(); j++){
			string rightString = assignStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector.push_back(rightString);
			}
		}
		follows_Left_Assign.at(leftNum) = isFollowsVector;
		vector<string> isFollowsVector1;
		for(int j=0; j<whileStmtNumberVector.size(); j++){
			string rightString = whileStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector1.push_back(rightString);
			}
		}
		follows_Left_While.at(leftNum) = isFollowsVector1;
		vector<string> isFollowsVector2;
		for(int j=0; j<callStmtNumberVector.size(); j++){
			string rightString = callStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector2.push_back(rightString);
			}
		}
		follows_Left_Calls.at(leftNum) = isFollowsVector2;
		vector<string> isFollowsVector3;
		for(int j=0; j<ifStmtNumberVector.size(); j++){
			string rightString = ifStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector3.push_back(rightString);
			}
		}
		follows_Left_If.at(leftNum) = isFollowsVector3;
		vector<string> isFollowsVector4;
		for(int j=0; j<stmtNumberVector.size(); j++){
			string rightString = stmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector4.push_back(rightString);
			}
		}
		follows_Left_Stmt.at(leftNum) = isFollowsVector4;

		//right
		string rightString = stmtNumberVector.at(i-1);
		int rightNum = atoi( rightString.c_str() );
		vector<string> isFollowsVector5;
		for(int j=0; j<assignStmtNumberVector.size(); j++){
			string leftString = assignStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector5.push_back(leftString);
			}
		}
		follows_Right_Assign.at(rightNum) = isFollowsVector5;
		vector<string> isFollowsVector6;
		for(int j=0; j<whileStmtNumberVector.size(); j++){
			string leftString = whileStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector6.push_back(leftString);
			}
		}
		follows_Right_While.at(rightNum) = isFollowsVector6;
		vector<string> isFollowsVector7;
		for(int j=0; j<callStmtNumberVector.size(); j++){
			string leftString = callStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector7.push_back(leftString);
			}
		}
		follows_Right_Calls.at(rightNum) = isFollowsVector7;
		vector<string> isFollowsVector8;
		for(int j=0; j<ifStmtNumberVector.size(); j++){
			string leftString = ifStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector8.push_back(leftString);
			}
		}
		follows_Right_If.at(rightNum) = isFollowsVector8;
		vector<string> isFollowsVector9;
		for(int j=0; j<stmtNumberVector.size(); j++){
			string leftString = stmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollows(leftNum, rightNum)){
				isFollowsVector9.push_back(leftString);
			}
		}
		follows_Right_Stmt.at(rightNum) = isFollowsVector9;

	}
}

void OptimisedCaller::genereateFollowsStar(){
	for(int i=1; i<=stmtNumberVector.size(); i++){
		string leftString = stmtNumberVector.at(i-1);
		int leftNum = atoi( leftString.c_str() );
		vector<string> isFollowsStarVector;
		for(int j=0; j<assignStmtNumberVector.size(); j++){
			string rightString = assignStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector.push_back(rightString);
			}
		}
		followsStar_Left_Assign.at(leftNum) = isFollowsStarVector;
		vector<string> isFollowsStarVector1;
		for(int j=0; j<whileStmtNumberVector.size(); j++){
			string rightString = whileStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector1.push_back(rightString);
			}
		}
		followsStar_Left_While.at(leftNum) = isFollowsStarVector1;
		vector<string> isFollowsStarVector2;
		for(int j=0; j<callStmtNumberVector.size(); j++){
			string rightString = callStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector2.push_back(rightString);
			}
		}
		followsStar_Left_Calls.at(leftNum) = isFollowsStarVector2;
		vector<string> isFollowsStarVector3;
		for(int j=0; j<ifStmtNumberVector.size(); j++){
			string rightString = ifStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector3.push_back(rightString);
			}
		}
		followsStar_Left_If.at(leftNum) = isFollowsStarVector3;
		vector<string> isFollowsStarVector4;
		for(int j=0; j<stmtNumberVector.size(); j++){
			string rightString = stmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector4.push_back(rightString);
			}
		}
		followsStar_Left_Stmt.at(leftNum) = isFollowsStarVector4;

		//right
		string rightString = stmtNumberVector.at(i-1);
		int rightNum = atoi( rightString.c_str() );
		vector<string> isFollowsStarVector5;
		for(int j=0; j<assignStmtNumberVector.size(); j++){
			string leftString = assignStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector5.push_back(leftString);
			}
		}
		followsStar_Right_Assign.at(rightNum) = isFollowsStarVector5;
		vector<string> isFollowsStarVector6;
		for(int j=0; j<whileStmtNumberVector.size(); j++){
			string leftString = whileStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector6.push_back(leftString);
			}
		}
		followsStar_Right_While.at(rightNum) = isFollowsStarVector6;
		vector<string> isFollowsStarVector7;
		for(int j=0; j<callStmtNumberVector.size(); j++){
			string leftString = callStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector7.push_back(leftString);
			}
		}
		followsStar_Right_Calls.at(rightNum) = isFollowsStarVector7;
		vector<string> isFollowsStarVector8;
		for(int j=0; j<ifStmtNumberVector.size(); j++){
			string leftString = ifStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector8.push_back(leftString);
			}
		}
		followsStar_Right_If.at(rightNum) = isFollowsStarVector8;
		vector<string> isFollowsStarVector9;
		for(int j=0; j<stmtNumberVector.size(); j++){
			string leftString = stmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(follows->isFollowsStar(leftNum, rightNum)){
				isFollowsStarVector9.push_back(leftString);
			}
		}
		followsStar_Right_Stmt.at(rightNum) = isFollowsStarVector9;

	}
}

void OptimisedCaller::genereateNextStar(){
	for(int i=1; i<=stmtNumberVector.size(); i++){
		string leftString = stmtNumberVector.at(i-1);
		int leftNum = atoi( leftString.c_str() );
		CFG * cfg = procTable->getCFG(leftNum);
		vector<string> isNextStarVector;
		for(int j=0; j<assignStmtNumberVector.size(); j++){
			string rightString = assignStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector.push_back(rightString);
			}
		}
		nextStar_Left_Assign.at(leftNum) = isNextStarVector;
		vector<string> isNextStarVector1;
		for(int j=0; j<whileStmtNumberVector.size(); j++){
			string rightString = whileStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector1.push_back(rightString);
			}
		}
		nextStar_Left_While.at(leftNum) = isNextStarVector1;
		vector<string> isNextStarVector2;
		for(int j=0; j<callStmtNumberVector.size(); j++){
			string rightString = callStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector2.push_back(rightString);
			}
		}
		nextStar_Left_Calls.at(leftNum) = isNextStarVector2;
		vector<string> isNextStarVector3;
		for(int j=0; j<ifStmtNumberVector.size(); j++){
			string rightString = ifStmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector3.push_back(rightString);
			}
		}
		nextStar_Left_If.at(leftNum) = isNextStarVector3;
		vector<string> isNextStarVector4;
		for(int j=0; j<stmtNumberVector.size(); j++){
			string rightString = stmtNumberVector.at(j);
			int rightNum = atoi( rightString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector4.push_back(rightString);
			}
		}
		nextStar_Left_Stmt.at(leftNum) = isNextStarVector4;

		//right
		string rightString = stmtNumberVector.at(i-1);
		int rightNum = atoi( rightString.c_str() );
		vector<string> isNextStarVector5;
		for(int j=0; j<assignStmtNumberVector.size(); j++){
			string leftString = assignStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector5.push_back(leftString);
			}
		}
		nextStar_Right_Assign.at(rightNum) = isNextStarVector5;
		vector<string> isNextStarVector6;
		for(int j=0; j<whileStmtNumberVector.size(); j++){
			string leftString = whileStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector6.push_back(leftString);
			}
		}
		nextStar_Right_While.at(rightNum) = isNextStarVector6;
		vector<string> isNextStarVector7;
		for(int j=0; j<callStmtNumberVector.size(); j++){
			string leftString = callStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector7.push_back(leftString);
			}
		}
		nextStar_Right_Calls.at(rightNum) = isNextStarVector7;
		vector<string> isNextStarVector8;
		for(int j=0; j<ifStmtNumberVector.size(); j++){
			string leftString = ifStmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector8.push_back(leftString);
			}
		}
		nextStar_Right_If.at(rightNum) = isNextStarVector8;
		vector<string> isNextStarVector9;
		for(int j=0; j<stmtNumberVector.size(); j++){
			string leftString = stmtNumberVector.at(j);
			int leftNum = atoi( leftString.c_str() );
			if(cfg->isNextStar(leftNum, rightNum)){
				isNextStarVector9.push_back(leftString);
			}
		}
		nextStar_Right_Stmt.at(rightNum) = isNextStarVector9;

	}
}

void OptimisedCaller::generateStmtLstTable(){
	vector<string> empty;
	for(int i=0; i<=stmtNumberVector.size(); i++){
		stmtLstTable.push_back(empty);
	}

	CFG * cfg = procTable->getCFG(1);

	for(int i=1; i<=stmtNumberVector.size(); i++){
		rootIf * storeRootIf = cfg->getStoreRootIf();
		int level = storeRootIf->getIfRootOfStmt(i).size()-1;
		int stmtLstNum= storeRootIf->getIfRootOfStmt(i).at(level);
		if(stmtLstNum!=0)
			stmtLstTable.at(stmtLstNum).push_back(std::to_string(static_cast<long long>(i)));
	
	}

}

void OptimisedCaller::generateIfStmtLstTable(){
	vector<string> empty;
	for(int i=0; i<=stmtNumberVector.size(); i++){
		ifStmtLstTable.push_back(empty);
	}

	CFG * cfg = procTable->getCFG(1);

	for(int i=1; i<=stmtNumberVector.size(); i++){
		rootIf * storeRootIf = cfg->getStoreRootIf();
		int level = storeRootIf->getOnlyIfRootOfStmt(i).size()-1;
		int stmtLstNum= storeRootIf->getOnlyIfRootOfStmt(i).at(level);
		if(stmtLstNum!=0)
			ifStmtLstTable.at(stmtLstNum).push_back(std::to_string(static_cast<long long>(i)));
	
	}

}

vector<string> OptimisedCaller::convertToStringResults(vector<int> _intResults){
	vector<string> result;
	for(int i=0; i<_intResults.size(); i++){	
		string stringResult = std::to_string(static_cast<long long>( _intResults.at(i)));
		result.push_back(stringResult);
	}
	return result;
}

