//Design Extractor traverses AST and compute all the remaining design abstractions: Uses, Modifies for procedure and call statement; Next; Affects
#pragma once
#include "DesignExtractor.h"
#define PROGRAM 0
#define PROCEDURE 1
#define STMT_LST 2
#define ASSIGN 3
#define WHILE 4
#define VAR 5
#define PLUS 6
#define CONSTANT 7
#define MINUS 8
#define TIMES 9
#define IF 10
#define CALL 11
//Default Constructor
DesignExtractor::DesignExtractor(){}
//Constructor
DesignExtractor::DesignExtractor(PKB* pkb):temp(){
	this->pkb = pkb;	
	ast = pkb->getAST();
	follows = pkb->getFollows();
	parent = pkb->getParent();
	varTable = pkb->getVarTable();
	procTable = pkb->getProcTable();
	modifies = pkb->getModifies();
	uses = pkb->getUses();
	calls = pkb->getCalls();
	affects = pkb->getAffects();
	optimisedCaller = pkb->getOptimisedCaller();
	storeRootWhile = pkb->getRootWhile();
	storeRootIf = pkb->getRootIf();
	flag = false;
}
void DesignExtractor::buildCFG(TNode currNode, vector<std::pair<int,int>> * graph){
	int nodeType = currNode.getNodeType();
	vector<int> children = currNode.getChildren();	
	vector<std::pair<int,int>>::iterator it = graph->begin();
	int currStmtNum = currNode.getStmtNumber();
	switch(nodeType){
		case PROCEDURE:
		{
			//go into statement list
			buildCFG(ast->getChild(currNode, 0), graph);
			//set end nodes
			setEndNode(graph);
			break;
		}
		case STMT_LST:
		{
			//for each node of stmtLst visit it
			for(int i = 0; i < children.size(); ++i){
				buildCFG(ast->getChild(currNode, i), graph);
			}
			break;
		}
		case CALL:
		case ASSIGN:
		{
			//first we check if we can find follows, otherwise find a parent's follows.
			STMT followStmtNum = follows->getFollows(1,currStmtNum);
			if(followStmtNum == -1) {
				graph->at(currStmtNum).first = findLoopback(currStmtNum);
				if(graph->at(currStmtNum).first == 0)
					graph->at(currStmtNum).first = findFollows(currStmtNum);
			}
			else{
				graph->at(currStmtNum).first = followStmtNum;
			}
			return;
			break;
		}
		case WHILE:
		{
			//connect to first stmt of my stmtLst
			STMT firstStmtNum = ast->getChild(ast->getChild(currNode, 1), 0).getStmtNumber();
			graph->at(currStmtNum).first = firstStmtNum;
			//first we check if we can find follows, otherwise find a parent's follows.
			STMT followStmtNum = follows->getFollows(1,currStmtNum);
			if(followStmtNum == -1) {
				graph->at(currStmtNum).second = findLoopback(currStmtNum);
				if(graph->at(currStmtNum).second == 0)
					graph->at(currStmtNum).second = findFollows(currStmtNum);
			}
			else{
				graph->at(currStmtNum).second = followStmtNum;
			}
			// call buildCFG again on the stmtLst
			buildCFG(ast->getChild(currNode, 1), graph);
			break;
		}
		case IF:
		{
			//connect to first stmt of then stmtLst
			TNode thenNode = ast->getChild(currNode, 1);
			STMT thenStmtNum = ast->getChild(thenNode, 0).getStmtNumber();
			graph->at(currStmtNum).first = thenStmtNum;
			//connect to first stmt of else stmtLst
			TNode elseNode = ast->getChild(currNode, 2);
			STMT elseStmtNum = ast->getChild(elseNode, 0).getStmtNumber();
			graph->at(currStmtNum).second = elseStmtNum;
			//call buildCFG of then
			buildCFG(thenNode, graph);
			//call buildCFG of else
			buildCFG(elseNode, graph);
			break;
		}
		default:
			break;
	}
}
int DesignExtractor::findFollows(int currStmt){
	//we try to find someone for this guy to point to
	//by searching the parent, if not parent of parent, if not ...
	STMT followStmtNum = 0, followsOfParent = 0;
	STMT parentStmtNum = parent->getParent(currStmt);
	do{
		followsOfParent = follows->getFollows(1, parentStmtNum);
		if(followsOfParent == -1)
			parentStmtNum = parent->getParent(parentStmtNum);
		else{
			followStmtNum = followsOfParent;
			break;
		}
	} while (parentStmtNum != -1);
	return followStmtNum;
}
int DesignExtractor::findLoopback(int currStmt){
	
	if(follows->getFollows(1,currStmt) == -1){
		//first we check for the case where it's the last stmt of a while stmtLst
		if(ast->getNodeType(parent->getParent(currStmt)) == "whileNode") {
			return parent->getParent(currStmt);
		}
		//otherwise, we look for a higher while
		else{
			STMT parentStmtNum = parent->getParent(currStmt);
			bool loopback = true;
			while(loopback == true){
				if(parentStmtNum == -1)
					loopback = false;
				if(ast->getNodeType(parentStmtNum) == "whileNode" && loopback){
					return parentStmtNum;
					break;
				}
				else{	//if node
					if(follows->getFollows(1, parentStmtNum) != -1)
						return 0;
				}
				parentStmtNum = parent->getParent(parentStmtNum);
			}
		}
	}
	return 0;
}
void DesignExtractor::setEndNode(vector<std::pair<int,int>> * graph){
	for(int i = 1; i < graph->size(); ++i){
		string nodeType = ast->getNodeType(i);
		if(nodeType == "ifNode")
			continue;
		if(nodeType == "whileNode" && graph->at(i).second == 0){
			graph->at(i).second = -1;
		}
		else{
			if(graph->at(i).first == 0 && graph->at(i).second == 0){
				graph->at(i).first = -1;
			}
		}
	}
}
	
//Build CFG for each procedure
void DesignExtractor::computeCFG(){
/*
	int numOfProc = procTable->getNoOfProc();
	//CFG cfg = CFG(ast, follows, parent);
	for(int i=0; i<numOfProc; i++){
		//buildCFG(procTable->getFirstStmt(procTable->getProcName(i)), procTable->getLastStmt(procTable->getProcName(i)));
		CFG * cfg = new CFG(Graph,Block);
		procTable->insertCFG(procTable->getProcName(i), cfg);
	}
*/
	//starting with the program node, we look at each child of program node
	INDEX root = ast->getRoot();
	int numOfProc = ast->getNoOfChild(ast->getNode(root));
	CFG * cfg = new CFG(storeRootWhile, storeRootIf);
	vector<std::pair<int,int>> * graph = new vector<std::pair<int,int>>();
	PROCLIST procList = procTable->getAllProcNames();
	
	//init graph
	std::pair<int,int> a_pair (0,0);
	for(int i = 0; i <= procTable->getLastStmt(*(procList.end()-1)); ++i){
		graph->push_back(a_pair);
	}
	for(int i = 0; i < numOfProc; ++i){
		TNode currProc = ast->getChild(ast->getNode(root), i);
		buildCFG(currProc, graph);
		
		
	}
	//TODO create cfg and insert into proc table
	int numOfProcs = procTable->getNoOfProc();
	for(int i=0; i<numOfProcs; i++){		
		CFG * cfg = new CFG(graph, procTable->getFirstStmt(procTable->getProcName(i)), procTable->getLastStmt(procTable->getProcName(i)), storeRootWhile, storeRootIf);
		procTable->insertCFG(procTable->getProcName(i), cfg);
	}
	
	//print out graph
	for(int i = 0; i < graph->size(); ++i){
		cout << i << "->" << graph->at(i).first << ", " << graph->at(i).second << endl;
	}
	
	//system("PAUSE");
}

void DesignExtractor::computeCFGBip(){
	//take exsiting graph and insert into new graph
	vector<vector<CFGLink>> graphBip;
	int numOfProc = procTable->getNoOfProc();
	PROCLIST procList = procTable->getAllProcNames();
	vector<pair<int,int>> * graphCFG = procTable->getCFG(1)->getCFG();
	int numOfStmt = procTable->getLastStmt(*(procList.end()-1));

	//translate old graph into new graph
	for(int i = 0; i < graphCFG->size(); i++){
		int firstLink = graphCFG->at(i).first;
		int secondLink = graphCFG->at(i).second;
		
		vector<CFGLink> currStmt;

		if(firstLink != 0){
			currStmt.push_back(CFGLink(firstLink, 0));
		}

		if(secondLink != 0){
			currStmt.push_back(CFGLink(secondLink, 0));
		}

		graphBip.push_back(currStmt);
	}

	//set dummy nodes
	vector<CFGLink> aLink;
	for(int i = 0; i < numOfProc; i++){
		graphBip.push_back(aLink);
	}

	//procedure's corresponding dummy node is
	//lastStmtNum + procIndex + 1
	for(int i = 0; i < graphCFG->size(); i++){
		int firstLink = graphCFG->at(i).first;
		int secondLink = graphCFG->at(i).second;

		if(firstLink == -1 || secondLink == -1){
			//get procedure's corresponding dummy node
			PROCINDEX procIndex = procTable->getProcIndex(getProcedure(i));

			//point curr stmt to dummy node instead
			if(firstLink == -1){
				graphBip.at(i).at(0).setLinkTo(numOfStmt + procIndex + 1);
			}
			else{
				graphBip.at(i).at(1).setLinkTo(numOfStmt + procIndex + 1);
			}
		}
	}

	//by now, we have a CFG Bip graph that is equivalent of CFG graph, but with links that are (stmt,0), and dummy nodes 
	//now we go thru 1 pass to link call nodes and dummy nodes
	for(int i = 1; i <= numOfStmt; i++){
		//if it's a call node
		if(ast->getNodeType(i) == "callNode"){
			//we store the curr next link
			int currNextLink = graphBip.at(i).at(0).getLinkTo();

			//we connect curr link to the corresponding procedure node
			TNode currNode = ast->getStmtNode(i);
			PROCNAME calledProc = currNode.getNodeValue();
			int firstStmt = procTable->getFirstStmt(calledProc);
			graphBip.at(i).at(0).setLinkTo(firstStmt);
			graphBip.at(i).at(0).setEdgeNumber(i);

			//we check if procedure has a dummy, if so we connect using the dummy
			if(hasDummy(calledProc, numOfStmt, graphCFG)){
				PROCINDEX procIndex = procTable->getProcIndex(calledProc);
				graphBip.at(numOfStmt+procIndex+1).push_back(CFGLink(currNextLink, i));
			}
			else{
			//otherwise we use the current procedure's last statement
				int procLastStmt = procTable->getLastStmt(calledProc);
				graphBip.at(procLastStmt).push_back(CFGLink(currNextLink, i));
			}
		}
	}

	//this->cfgBip = graphBip;

	pkb->setCFGBip(graphBip);

	//printing for debugging
	//print graphCFG
	for(int i = 0; i < graphCFG->size(); i++){
		cout << i << " -> " << graphCFG->at(i).first << " " << graphCFG->at(i).second << endl;
	}

	//print graphBip
	for(int i = 0; i < graphBip.size(); i++){
		cout << i << " -> |";
		for(int j = 0; j < graphBip.at(i).size(); j++){
			cout << graphBip.at(i).at(j).getLinkTo() << "," << graphBip.at(i).at(j).getEdgeNumber() << "|";
		}
		cout << endl;
	}
}

bool DesignExtractor::hasDummy(PROCNAME proc, int totalStmt, vector<pair<int,int>> * graph){
	//we check if anywhere in the procedure have we pointed it to dummy
	PROCINDEX procIndex = procTable->getProcIndex(proc);
	int procStart = procTable->getFirstStmt(proc);
	int procEnd = procTable->getLastStmt(proc);

	for(int i = procStart; i <= procEnd; i++){
		if(graph->at(i).first == -1 || graph->at(i).second == -1)
			return true;
	}

	return false;
}

PROCNAME DesignExtractor::getProcedure(int stmtNum){
	PROCLIST procList = procTable->getAllProcNames();
	for(int i = 0; i < procList.size(); i++){
		int start = procTable->getFirstStmt(procList.at(i));
		int end = procTable->getLastStmt(procList.at(i));

		if(start <= stmtNum && stmtNum <= end){
			return procList.at(i);
		}
	}
}

string DesignExtractor::computeCalls(){
	string error = "";
	if(flag == false){
		temp = calls->getTemp();		
		for(vector<pair<pair<PROCNAME, PROCNAME>, STMT>>::size_type i = 0; i<temp.size(); i++)
		{
			PROCINDEX caller = procTable->getProcIndex(temp[i].first.first);
			PROCINDEX callee = procTable->getProcIndex(temp[i].first.second);
			if(caller == -1)
			{
				error += "\rProcedure "+ temp[i].first.first+" does not exist. Setting \""+temp[i].first.first+" calls "+temp[i].first.second+"\" fails.\n";
			}
			else if(callee == -1)
			{
				error += "\rProcedure "+ temp[i].first.second+" does not exist. Setting \""+temp[i].first.first+" calls "+temp[i].first.second+"\" fails.\n";
			}
			else
			{
				calls->setCalls(caller, callee);
				
			}
		}
		flag = true;
	}
	return error;
}
//compute Modifies and Uses for procedures and call statements
void DesignExtractor::computeModifiesAndUses(){
	if(flag == false){
		computeCalls();
	}
	//Compute Modifies and Uses for procedures
	int numOfProc = procTable->getNoOfProc();
	PROCLIST procNames = procTable->getAllProcNames();
	for(int i = 0; i<numOfProc; i++){
		PROCINDXLIST myCalleesStar = calls->getCalleesStar(procNames[i]);
		INDEXLIST modifiedByI = procTable->getModifies(procNames[i]);
		INDEXLIST usedByI = procTable->getUses(procNames[i]);
		for(int j = 0; j<myCalleesStar.size(); j++){
				
			INDEXLIST modifiedByJ = procTable->getModifies(procNames[myCalleesStar[j]]);
			for(int k = 0; k<modifiedByJ.size(); k++)
			{
				INDEXLIST::iterator it = find(modifiedByI.begin(), modifiedByI.end(), modifiedByJ[k]);
				if(it == modifiedByI.end()){
					procTable->insertModifies(procNames[i], varTable->getVarName(modifiedByJ[k]));
				}
			}
			INDEXLIST usedByJ = procTable->getUses(procNames[myCalleesStar[j]]);
			for(int k = 0; k<usedByJ.size(); k++)
			{
				INDEXLIST::iterator it = find(usedByI.begin(), usedByI.end(), usedByJ[k]);
				if(it == usedByI.end()){
					procTable->insertUses(procNames[i], varTable->getVarName(usedByJ[k]));
				}
			}
		}
	}
	//Compute Modifies and Uses for call statements and Update Modifies and Uses for their parent statements
	for(vector<pair<pair<PROCNAME, PROCNAME>, STMT>>::size_type i = 0; i<temp.size(); i++){
		
		INDEXLIST varModified = procTable->getModifies(temp[i].first.second);
		INDEXLIST varUsed = procTable->getUses(temp[i].first.second);
		PROCINDEX callee = procTable->getProcIndex(temp[i].first.second);
		STMTLST parentStmt = parent->getParentStar(temp[i].second);
		for(int j=0; j<varModified.size(); j++){
			modifies->setModifies(temp[i].second, varTable->getVarName(varModified[j]));
			for(int k=0; k<parentStmt.size(); k++)
			{
				modifies->setModifies(parentStmt[k], varTable->getVarName(varModified[j]));
			}
		}
		for(int j=0; j<varUsed.size(); j++){
			uses->setUses(temp[i].second, varTable->getVarName(varUsed[j]));
			for(int k=0; k<parentStmt.size(); k++)
			{
				uses->setUses(parentStmt[k], varTable->getVarName(varUsed[j]));
			}
		}
	}
}
// fill vector extra in ProcTable
void DesignExtractor::fillExtra(){
	PROCINDEXLIST* p = procTable->getExtra();
	p->push_back(-1); //dummy first entry (no stmt number 0)
	for(int i = 0; i<procTable->getNoOfProc(); i++)
	{
		PROCNAME proc = procTable->getProcName(i);
		for(int j = procTable->getFirstStmt(proc); j <= procTable->getLastStmt(proc); j++)
		{
			p->push_back(i);
		}
	}
}

void DesignExtractor::computeOptimisedCaller(){
	optimisedCaller->generateOptimised();
}


void DesignExtractor::initializeAffectsCache() {
	affects->initializeCache();
}
