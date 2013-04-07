#include "evaluator.h"
#include <iostream>
#include "AbstractWrapper.h"
using namespace std;

evaluator::evaluator(void)
{
	

}
evaluator::~evaluator(void){

}
void evaluator::insertAST(AST* _ast){
	ast = _ast;
}
void evaluator::insertFollows(Follows* _follows){
	follows = _follows;
}
void evaluator::insertParent(Parent* _parent){
	parent = _parent;
}
void evaluator::insertUses(Uses* _uses){
	uses = _uses;
}
void evaluator::insertModifies(Modifies* _modifies){
	modifies = _modifies;
}
void evaluator::insertVarTable(VarTable* _varTable){
	varTable = _varTable;
}
void evaluator::insertCalls(Calls* _calls){
	calls = _calls;
}
void evaluator::insertProcTable(ProcTable* _procTable){
	procTable = _procTable;
}
void evaluator::insertCFG(CFG* _cfg){
	cfg = _cfg;
}
void evaluator::insertAffects(Affects* _affects){
	affects = _affects;
}
void evaluator::insertOptimisedCaller(OptimisedCaller* _optimisedCaller){
	optimisedCaller = _optimisedCaller;
}

vector<string> evaluator::evaluateQuery(queryTree* _queryTree){
	vector<string> select1=_queryTree->getSelect(0);
	string select = select1.at(0);
	string selectType =  select1.at(1);
	//string select2 = _queryTree->getSelect(1).at(0);
	//string selectType2 =  _queryTree->getSelect(1).at(1);
	vector<string> results;
	//queryHasTrue=false;
	//patternHasTrue=false;
	//withHasTrue=false;
	queryHasTrue=true;
	patternHasTrue=true;
	withHasTrue=true;
	if(select!=""){
		

		/*if(_queryTree->getWithQuerySize()>0){
			evaluateWith(select, selectType, _queryTree);
		}else{	withHasTrue=true;
		}
		if(_queryTree->getSuchThatQuerySize()>0){
				evaluateSuchThat(select ,selectType,_queryTree);
		}else{	queryHasTrue=true;
		}
		if(_queryTree->getPatternQuerySize()>0){
			evaluatePattern(_queryTree);
		}else{	patternHasTrue=true;
		}
		*/

		vector<vector<string>> optimisedQuery = optimiseQuery(_queryTree);
		for(int i=0; i<optimisedQuery.size(); i++){
			if(queryHasTrue && patternHasTrue && withHasTrue){
				vector<string> singleQuery = optimisedQuery.at(i);
				string queryType = singleQuery.at(singleQuery.size()-1);
				if(queryType=="suchThat"){
					evaluateSuchThat(select ,selectType,singleQuery);
				}else if(queryType=="with"){
					evaluateWith(select, selectType, singleQuery);
				}if(queryType=="pattern"){
					evaluatePattern(singleQuery);
				}
			}
			else break;
		}

		if(select == "BOOLEAN"){
			if(queryHasTrue && patternHasTrue && withHasTrue) results.push_back("true");
			else results.push_back("false");
		}else{
			if(queryHasTrue && patternHasTrue && withHasTrue){
				results= getResults(_queryTree);
			}	
		}
	}
	return results;
}

void evaluator::evaluateWith(string _select, string _selectType, vector<string> withTree){
//void evaluator::evaluateWith(string _select, string _selectType, queryTree* _queryTree){
	//for(int i=0; i<_queryTree->getWithQuerySize(); i++){
		withHasTrue = false;
		//WITHBRANCH withTree = _queryTree->getWithQuery(i);
		string value0 = withTree.at(0);
		string value1 = withTree.at(1);
		string value2 = withTree.at(2);
		string value3 = withTree.at(3);
		string value4 = withTree.at(4);
		string value5 = withTree.at(5);
		string value6 = withTree.at(6);
		string value7 = withTree.at(7);
		vector<string> allPossibleStmt;
		
		if(value0=="attRef"){
			if(value3=="stmt#"){
				if(table.hasColumns(value1)){
					allPossibleStmt = table.getColumn(value1);
				}else{
					allPossibleStmt = getAllPossibleByType(value2);
				}	
				if(value4=="Ident"){
					withHasTrue = false;
				}else if(value4=="Integer"){
					vector<string> correctEntry;
					for(int i=0;i<allPossibleStmt.size();i++){
						if(allPossibleStmt.at(i)==value5){
							correctEntry.push_back(value5);
							table.insert(value1, correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}else if(value4=="prog_line"){
					vector<string> allPossibleStmt2; 
					if(table.hasColumns(value5)){
						allPossibleStmt2 = table.getColumn(value5);
					}else{
						allPossibleStmt2 = getAllPossibleByType("prog_line");
					}
					for(int i=0;i<allPossibleStmt.size();i++){
						vector<string> correctEntry;
						string entryLeft = allPossibleStmt.at(i);
						for(int j=0;j<allPossibleStmt2.size(); j++){
							string entryRight = allPossibleStmt2.at(j);
							if(entryLeft==entryRight){
								correctEntry.push_back(entryRight);
								table.insert(value1, entryLeft, value5, correctEntry);
								withHasTrue=true;
								break;
							}
						}
					}
				}else if(value4=="attRef"){
					//to implement if value3==value7
					if(value3==value7 || value7=="value"){
						vector<string> allPossibleStmt2; 
						if(table.hasColumns(value5)){
							allPossibleStmt2 = table.getColumn(value5);
						}else{
							allPossibleStmt2 = getAllPossibleByType(value6);
						}						
						for(int i=0;i<allPossibleStmt.size();i++){
							vector<string> correctEntry;
							string entryLeft = allPossibleStmt.at(i);
							for(int j=0;j<allPossibleStmt2.size(); j++){
								string entryRight = allPossibleStmt2.at(j);
								if(entryLeft==entryRight){
									correctEntry.push_back(entryRight);
									table.insert(value1, entryLeft, value5, correctEntry);
									withHasTrue=true;
									break;
								}
							}
						}
					}
				}
			}else if(value3=="varName"){
				if(table.hasColumns(value1)){
					allPossibleStmt = table.getColumn(value1);
				}else{
					allPossibleStmt = getAllPossibleByType(value2);
				}
				if(value4=="Ident"){
					vector<string> correctEntry;
					for(int i=0;i<allPossibleStmt.size();i++){
						if(allPossibleStmt.at(i)==value5){
							correctEntry.push_back(value5);
						}
					}
					if(correctEntry.size()>0){
						table.insert(value1, correctEntry);
						withHasTrue=true;
					}
				}else if(value4=="Integer"){
					withHasTrue = false;
				}else if(value4=="prog_line"){
					withHasTrue = false;
				}else if(value4=="attRef"){
					if(value3==value7 || value7=="procName"){
						vector<string> allPossibleStmt2;
						if(table.hasColumns(value5)){
							allPossibleStmt2 = table.getColumn(value5);
						}else{
							allPossibleStmt2 = getAllPossibleByType(value6);
						}
						for(int i=0;i<allPossibleStmt.size();i++){
							vector<string> correctEntry;
							string entryLeft = getProcName(value2, allPossibleStmt.at(i));
							for(int j=0;j<allPossibleStmt2.size(); j++){
								string entryRight = getProcName(value6, allPossibleStmt2.at(j));
								if(entryLeft==entryRight){
									correctEntry.push_back(entryRight);
									table.insert(value1, entryLeft, value5, correctEntry);
									withHasTrue=true;
									break;
								}
							}
						}
					}
				}
			}else if(value3=="procName"){
				vector<string> correctEntry;
				if(table.hasColumns(value1)){
					allPossibleStmt = table.getColumn(value1);
				}else{
					allPossibleStmt = getAllPossibleByType(value2);
				}
				if(value4=="Ident"){
					for(int i=0;i<allPossibleStmt.size();i++){
						string callsProcName = getProcName(value2, allPossibleStmt.at(i));
						if(callsProcName==value5){
							correctEntry.push_back(allPossibleStmt.at(i));
							table.insert(value1, correctEntry);
							withHasTrue=true;
						}
					}
				}else if(value4=="Integer"){
					//withHasTrue==false;
				}else if(value4=="prog_line"){
					//withHasTrue==false;
				}else if(value4=="attRef"){
					if(value7=="stmt#"){
						//withHasTrue==false;
					}else if(value7=="varName" || value7=="procName"){
						vector<string> allPossibleStmt2;
						if(table.hasColumns(value5)){
							allPossibleStmt2 = table.getColumn(value5);
						}else{
							allPossibleStmt2 = getAllPossibleByType(value6);
						}
						for(int i=0;i<allPossibleStmt.size();i++){
							vector<string> correctEntry;
							string entryLeft = getProcName(value2, allPossibleStmt.at(i));
							for(int j=0;j<allPossibleStmt2.size(); j++){
								string entryRight = getProcName(value6, allPossibleStmt2.at(j));
								if(entryLeft==entryRight){
									correctEntry.push_back(entryRight);
									table.insert(value1, entryLeft, value5, correctEntry);
									withHasTrue=true;
									break;
								}
							}
						}
						
					}else if(value7=="value"){
						//withHasTrue==false;
					}
				}
			}else if(value3=="value"){
				if(table.hasColumns(value1)){
					allPossibleStmt = table.getColumn(value1);
				}else{
					allPossibleStmt = getAllPossibleByType(value2);
				}
				vector<string> correctEntry;
				if(value4=="Ident"){
				}else if(value4=="Integer"){
					for(int i=0;i<allPossibleStmt.size();i++){
						if(allPossibleStmt.at(i)==value5){
							correctEntry.push_back(value5);
							table.insert(value1, correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}else if(value4=="prog_line"){
					vector<string> allPossibleStmt2;
					if(table.hasColumns(value5)){
						allPossibleStmt2 = table.getColumn(value5);
					}else{
						allPossibleStmt2 = getAllPossibleByType("prog_line");
					}
					for(int i=0;i<allPossibleStmt.size();i++){
						vector<string> correctEntry;
						string entryLeft = allPossibleStmt.at(i);
						for(int j=0;j<allPossibleStmt2.size(); j++){
							string entryRight = allPossibleStmt2.at(j);
							if(entryLeft==entryRight){
								correctEntry.push_back(entryRight);
								table.insert(value1, entryLeft, value5, correctEntry);
								withHasTrue=true;
								break;
							}
						}
					}

				}else if(value4=="attRef"){
					if(value3==value7 || value7=="stmt#"){
						vector<string> allPossibleStmt2; 
						if(table.hasColumns(value5)){
							allPossibleStmt2 = table.getColumn(value5);
						}else{
							allPossibleStmt2 = getAllPossibleByType(value6);
						}						
						for(int i=0;i<allPossibleStmt.size();i++){
							vector<string> correctEntry;
							string entryLeft = allPossibleStmt.at(i);
							for(int j=0;j<allPossibleStmt2.size(); j++){
								string entryRight = allPossibleStmt2.at(j);
								if(entryLeft==entryRight){
									correctEntry.push_back(entryRight);
									table.insert(value1, entryLeft, value5, correctEntry);
									withHasTrue=true;
									break;
								}
							}
						}
					}
				}
			}
		}
		else if(value0=="Integer"){
			if(value4=="Ident"){
			}else if(value4=="Integer"){
				if(value1==value5){
					withHasTrue=true;
				}
			}else if(value4=="prog_line"){
				vector<string> correctEntry;
				vector<string> allPossibleStmt2;
				if(table.hasColumns(value5)){
					allPossibleStmt2 = table.getColumn(value5);
				}else{
					allPossibleStmt2 = getAllPossibleByType("prog_line");
				}					
				for(int i=0;i<allPossibleStmt2.size(); i++){
					if(value1==allPossibleStmt2.at(i)){
						correctEntry.push_back(value1);
						table.insert(value5, correctEntry);
						withHasTrue=true;
						break;
					}
				}
			}else if(value4=="attRef"){
				if(value7=="stmt#" || value7=="value"){
					vector<string> correctEntry;
					vector<string> allPossibleStmt2;
					if(table.hasColumns(value5)){
						allPossibleStmt2 = table.getColumn(value5);
					}else{
						allPossibleStmt2 = getAllPossibleByType(value6);
					}					
					for(int i=0;i<allPossibleStmt2.size(); i++){
						if(value1==allPossibleStmt2.at(i)){
							correctEntry.push_back(value1);
							table.insert(value5, correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}else if(value7=="varName"){
					//withHasTrue=false;
				}else if(value7=="procName"){
					//withHasTrue=false;
				}
			}
		}else if(value0=="Ident"){
			if(value4=="Ident"){
				if(value1==value5){
					withHasTrue=true;
				}
			}else if(value4=="Integer"){
				//withHasTrue=false;
			}else if(value4=="prog_line"){
				//withHasTrue=false;
			}else if(value4=="attRef"){
				if(value7=="stmt#"){
					//withHasTrue=false;
				}else if(value7=="varName" || value7=="procName"){
					vector<string> correctEntry;
					vector<string> allPossibleStmt2;
					if(table.hasColumns(value5)){
						allPossibleStmt2 = table.getColumn(value5);
					}else{
						allPossibleStmt2 = getAllPossibleByType(value6);
					}					
					for(int i=0;i<allPossibleStmt2.size(); i++){
						string entryRight = getProcName(value6, allPossibleStmt2.at(i));
						if(value1==entryRight){
							correctEntry.push_back(value1);
							table.insert(value5, correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}else if(value7=="value"){
				}
			}
		}else if(value0=="prog_line"){
			//get all possible prog_line first 
			if(table.hasColumns(value1)){
				allPossibleStmt = table.getColumn(value1);
			}else{
				allPossibleStmt = getAllPossibleByType("prog_line");
			}
			if(value4=="Ident"){
				//withHasTrue=false;
			}else if(value4=="Integer"){
				vector<string> correctEntry;
				for(int i=0; i<allPossibleStmt.size(); i++){
					if(value5==allPossibleStmt.at(i)){
						correctEntry.push_back(value5);
						table.insert(value1, correctEntry);
						withHasTrue=true;
						break;
					}
				}
			}else if(value4=="prog_line"){
				vector<string> allPossibleStmt2;
				if(table.hasColumns(value5)){
					allPossibleStmt2 = table.getColumn(value5);
				}else{
					allPossibleStmt2 = getAllPossibleByType("prog_line");
				}
				for(int i=0; i<allPossibleStmt.size(); i++){
					vector<string> correctEntry;
					for(int j=0; j<allPossibleStmt2.size(); j++){
						if(allPossibleStmt.at(i)==allPossibleStmt2.at(j)){
							correctEntry.push_back(allPossibleStmt.at(i));
							table.insert(value1, correctEntry);
							withHasTrue=true;
							break;
						}
					}
				}
			}else if(value4=="attRef"){
				if(value7=="stmt#" || value7=="value"){
					vector<string> allPossibleStmt2;
					if(table.hasColumns(value5)){
						allPossibleStmt2 = table.getColumn(value5);
					}else{
						allPossibleStmt2 = getAllPossibleByType(value6);
					}
					for(int i=0; i<allPossibleStmt.size(); i++){
						vector<string> correctEntry;
						for(int j=0; j<allPossibleStmt2.size(); j++){
							if(allPossibleStmt.at(i)==allPossibleStmt2.at(j)){
								correctEntry.push_back(allPossibleStmt.at(i));
								table.insert(value1, allPossibleStmt.at(i), value5, correctEntry);
								withHasTrue=true;
								break;
							}

						}
					}
				}else if(value7=="varName"){
					//withHasTrue=false;
				}else if(value7=="procName"){
					//withHasTrue=false;
				}
			}
		}
		
		table.shrinkTable();
		//if(!withHasTrue) break;
	//}
}

void evaluator::evaluatePattern( vector<string> patternTree){
//void evaluator::evaluatePattern( queryTree* _queryTree){
	//for(int i=0; i<_queryTree->getPatternQuerySize(); i++){
		patternHasTrue = false;
		//PATTERNBRANCH patternTree = _queryTree->getPatternQuery(i);
		string patternStmt = patternTree.at(0);
		string patternTypeStmt = patternTree.at(1);
		string patternValueLeft = patternTree.at(2);
		string patternValueRight = patternTree.at(5);
		string patternTypeLeft = patternTree.at(3);
		string patternTypeRight = patternTree.at(4);
		vector<string>	allStmtEntry;
		vector<string>	allLeftEntry;
		vector<string>	allRightEntry;
		if(table.hasColumns(patternStmt)){
			allStmtEntry = table.getColumn(patternStmt);
		}else{
			allStmtEntry=getAllPossibleByType(patternTypeStmt);
		}
		if(patternTypeLeft== "Ident"){
			allLeftEntry.push_back(patternValueLeft);
		}else if(patternTypeLeft=="variable"){
			if(table.hasColumns(patternValueLeft)){
				allLeftEntry = table.getColumn(patternValueLeft);
			}else{
				allLeftEntry = getAllPossibleByType(patternTypeLeft); 
			}
		}else if(patternTypeLeft=="_"){
			allLeftEntry = getAllPossibleByType("variable"); 
		}
		if(patternTypeRight=="Var_name"){
			allRightEntry.push_back(patternValueRight);
		}else if(patternTypeRight=="_"){
			allRightEntry = getAllPossibleByType("variable"); 
		}else if(patternTypeRight=="expr" || patternTypeRight=="_expr_"){
			allRightEntry.push_back(patternValueRight);
		}

		if(patternTypeStmt=="if"){
			evaluateIfPatternBranch(patternStmt,patternTypeStmt,patternTypeLeft,patternValueLeft,patternTypeRight,patternValueRight, allStmtEntry, allLeftEntry,allRightEntry);
		}else if(patternTypeStmt=="while"){
			evaluateWhilePatternBranch(patternStmt,patternTypeStmt,patternTypeLeft,patternValueLeft,patternTypeRight,patternValueRight, allStmtEntry, allLeftEntry,allRightEntry);
		}else{
			evaluatePatternBranch(patternStmt,patternTypeStmt,patternTypeLeft,patternValueLeft,patternTypeRight,patternValueRight, allStmtEntry, allLeftEntry,allRightEntry);
		}
		table.shrinkTable();
		//if(!patternHasTrue) break;
	//}
}

void evaluator::evaluatePatternBranch(string _patternStmt, string _typeStmt, string _typeLeft, string _valueLeft, string _typeRight,
		string _valueRight, vector<VALUE> _allStmtEntry, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	bool patternExist;
	vector<string> correctEntry;
	//for(int i=0; i<_allStmtEntry.size(); i++){
	
	int allLeftSize = _allLeftEntry.size(); 
	int allRightSize = _allRightEntry.size(); 
		
	if(_typeLeft=="variable"){
		for(int i=0; i<_allStmtEntry.size(); i++){
			patternExist = false;
			string entryStmt = _allStmtEntry.at(i);
			int intStmt = atoi( entryStmt.c_str() );
			for(int j=0; j<allLeftSize; j++){
				vector<string> correctEntry;
				string entryLeft = _allLeftEntry.at(j); 
				if(modifies->isModifies(intStmt,entryLeft)){
					if(_typeRight=="_"){
						correctEntry.push_back(entryLeft);
						table.insert(_patternStmt, entryStmt, _valueLeft, correctEntry);
						patternHasTrue = true;
						break;
					}else if(_typeRight=="_expr_"){
						AST * queryAst = new AST();
						ASTBuilder * astBuilder = new ASTBuilder(queryAst, _valueRight);
						astBuilder->convertToAST();
						if(ast->findMatchingPattern(intStmt, 1, queryAst)){
							correctEntry.push_back(entryLeft);
							table.insert(_patternStmt, entryStmt, _valueLeft, correctEntry);
							patternHasTrue = true;
							break;
						}

					}else if(_typeRight=="expr"){
						AST * queryAst = new AST();
						ASTBuilder * astBuilder = new ASTBuilder(queryAst, _valueRight);
						astBuilder->convertToAST();
						if(ast->findMatchingPattern(intStmt, 0, queryAst)){
							correctEntry.push_back(entryLeft);
							table.insert(_patternStmt, entryStmt, _valueLeft, correctEntry);
							patternHasTrue = true;
							break;
						}
					}

				}

			}
		}
	}else{
		for(int i=0; i<_allStmtEntry.size(); i++){
			patternExist = false;
			string entryStmt = _allStmtEntry.at(i);
			int intStmt = atoi( entryStmt.c_str() );
			if(_typeRight=="_" && _typeLeft=="_" && allLeftSize>0 && allRightSize>0){
				patternExist = true;
			}else{
				if(_valueLeft=="_" || modifies->isModifies(intStmt, _valueLeft)){
					if(_typeRight=="_"){
						patternExist = true;
					}else if(_typeRight=="_expr_"){
						AST * queryAst = new AST();
						ASTBuilder * astBuilder = new ASTBuilder(queryAst, _valueRight);
						astBuilder->convertToAST();
						if(uses->isUses(intStmt, _valueRight) || ast->findMatchingPattern(intStmt, 1, queryAst)){
							patternExist = true;
						}
					}
					else if(_typeRight=="expr"){
						AST * queryAst = new AST();
						ASTBuilder * astBuilder = new ASTBuilder(queryAst, _valueRight);
						astBuilder->convertToAST();
						if(uses->isUses(intStmt, _valueRight) || ast->findMatchingPattern(intStmt, 0, queryAst)){
							patternExist = true;
						}
					}
				}
								
			}
			if(patternExist){
				correctEntry.push_back(entryStmt);
				patternHasTrue = true;
				
			}
		}
		table.insert(_patternStmt, correctEntry);
	}
}


void evaluator::evaluateIfPatternBranch(string _patternStmt, string _typeStmt, string _typeLeft, string _valueLeft, string _typeRight,
		string _valueRight, vector<VALUE> _allStmtEntry, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	vector<string> correctEntry;
	for(int i=0; i<_allStmtEntry.size(); i++){
		string entryStmt = _allStmtEntry.at(i);
		int intStmt = atoi( entryStmt.c_str() );
		TNode ifNode = ast->getStmtNode(intStmt);
		TNode ifVarNode = ast->getChild(ifNode, 0);
		string ifVarValue = ifVarNode.getNodeValue();
		int allRightSize = _allRightEntry.size(); 
		if(_typeLeft=="_" && _allLeftEntry.size()>0){
			patternHasTrue=true;
			break;
		}
		else{
			for(int j=0; j<_allLeftEntry.size(); j++){
				if(ifVarValue==_allLeftEntry.at(j)){
					correctEntry.push_back(ifVarValue);
					table.insert(_patternStmt, entryStmt, _valueLeft, correctEntry);
					patternHasTrue = true;
					break;
				}
			}
		}
	}
}

void evaluator::evaluateWhilePatternBranch(string _patternStmt, string _typeStmt, string _typeLeft, string _valueLeft, string _typeRight,
		string _valueRight, vector<VALUE> _allStmtEntry, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	vector<string> correctEntry;
	for(int i=0; i<_allStmtEntry.size(); i++){
		string entryStmt = _allStmtEntry.at(i);
		int intStmt = atoi( entryStmt.c_str() );
		TNode whileNode = ast->getStmtNode(intStmt);
		TNode whileVarNode = ast->getChild(whileNode, 0);
		string whileVarValue = whileVarNode.getNodeValue();
		int allRightSize = _allRightEntry.size(); 
		if(_typeLeft=="_" && _allLeftEntry.size()>0){
			patternHasTrue=true;
			break;
		}
		else{
			for(int j=0; j<_allLeftEntry.size(); j++){
				if(whileVarValue==_allLeftEntry.at(j)){
					correctEntry.push_back(whileVarValue);
					table.insert(_patternStmt, entryStmt, _valueLeft, correctEntry);
					patternHasTrue = true;
					break;
				}
			}
		}
	}
}

void evaluator::evaluateSuchThat(string _select, string _selectType, vector<string> suchThatQuery){
/*void evaluator::evaluateSuchThat(string _select, string _selectType, queryTree* _queryTree){
	if (_queryTree->getSuchThatQuerySize() == 0){
		//return convertSolution(getAllPossibleByType(_selectType));
	}else{
		for(int i=0; i<_queryTree->getSuchThatQuerySize(); i++){*/
		queryHasTrue = false;
		vector<VALUE> allLeftEntry;
		vector<VALUE> allRightEntry;
		//QUERYBRANCH suchThatQuery = _queryTree->getSuchThatQuery(i);
		string typeLeft = suchThatQuery.at(2);
		string typeRight = suchThatQuery.at(4);
		string valueLeft = suchThatQuery.at(1);
		string valueRight = suchThatQuery.at(3);
		string queryType = suchThatQuery.at(0);
		string tableValueLeft;
		string tableValueRight;
		//checks left
		if(typeLeft=="Integer" || typeLeft=="Ident"){
			allLeftEntry.push_back(valueLeft);
			tableValueLeft="_";
		}else if(typeLeft=="_"){
			if(queryType=="Follows" || queryType=="Follows*" || 
				queryType=="Parent" || queryType=="Parent*" ||
				queryType=="Uses" || queryType=="Modifies*" ||
				queryType=="Next" || queryType=="Next*" ||
				queryType=="Affects" || queryType=="Affects*"){
				allLeftEntry = getAllPossibleByType("stmt");
			}else if(queryType=="Calls" || queryType=="Calls*"){
				allLeftEntry = getAllPossibleByType("procedure");
			}
			tableValueLeft="_";
		}//valueLeft is a declaration
		else if(table.hasColumns(valueLeft)){
			allLeftEntry = table.getColumn(valueLeft);
			tableValueLeft=valueLeft;
		}else{
			allLeftEntry = getAllPossibleByType(typeLeft);
			tableValueLeft=valueLeft;
		}
		//checks right
		if(typeRight=="Integer"){
			allRightEntry.push_back(valueRight);
			tableValueRight="_";
		}else if(typeRight=="Ident"){
			allRightEntry.push_back(valueRight);
			tableValueRight="_";
		}else if(typeRight=="_"){
			if(queryType=="Follows" || queryType=="Follows*" || 
				queryType=="Parent" || queryType=="Parent*" ||
				queryType=="Next" || queryType=="Next*" ||
				queryType=="Affects" || queryType=="Affects*"){
				allRightEntry = getAllPossibleByType("stmt");
			}else if(queryType=="Uses" || queryType=="Modifies"){
				allRightEntry = getAllPossibleByType("variable");
			}else if(queryType=="Calls" || queryType=="Calls*"){
				allRightEntry = getAllPossibleByType("procedure");
			}
			tableValueRight="_";
		}//valueRight is a declaration
		else if(table.hasColumns(valueRight)){
			allRightEntry = table.getColumn(valueRight);
			tableValueRight=valueRight;
		}else{
			allRightEntry = getAllPossibleByType(typeRight);
			tableValueRight=valueRight;
		}
		evaluateBranch(queryType,tableValueLeft, tableValueRight, allLeftEntry, allRightEntry,typeLeft, typeRight);
		table.shrinkTable();
			//if(!queryHasTrue) break;
		//}
	//}
}

void evaluator::evaluateBranch(string _queryType, string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, string _typeLeft, string _typeRight){
	if(_queryType=="Follows"){
		evaluateFollowsBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Follows*"){
		evaluateFollowsStarBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Parent"){
		evaluateParentBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Parent*"){
		evaluateParentStarBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Uses"){
		evaluateUsesBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry, _typeLeft, _typeRight);
	}
	else if(_queryType=="Modifies"){
		evaluateModifiesBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry, _typeLeft, _typeRight);
	}
	else if(_queryType=="Calls"){
		evaluateCallsBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Calls*"){
		evaluateCallsStarBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Next"){
		evaluateNextBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Next*"){
		evaluateNextStarBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Affects"){
		evaluateAffectsBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Affects*"){
		evaluateAffectsStarBranch(_valueLeft, _valueRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Contains"){
		evaluateContainsBranch(_valueLeft, _valueRight, _typeLeft, _typeRight, _allLeftEntry, _allRightEntry);
	}
	else if(_queryType=="Contains*"){
		evaluateContainsStarBranch(_valueLeft, _valueRight, _typeLeft, _typeRight, _allLeftEntry, _allRightEntry);
	}
}

void evaluator::evaluateFollowsBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(follows->isFollows(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
				queryHasTrue = true;
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				int intRight = atoi( entryRight.c_str() );
				if(follows->isFollows(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateFollowsStarBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(follows->isFollows(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
			}
			if(correctEntry.size()>0){
				queryHasTrue = true;
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				int intRight = atoi( entryRight.c_str() );
				if(follows->isFollowsStar(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateParentBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(parent->isParent(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
			}
			if(correctEntry.size()>0){
				queryHasTrue = true;
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				int intRight = atoi( entryRight.c_str() );
				if(parent->isParent(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateParentStarBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(parent->isParentStar(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
			}
			if(correctEntry.size()>0){
				queryHasTrue = true;
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				int intRight = atoi( entryRight.c_str() );
				if(parent->isParentStar(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateUsesBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, string _typeLeft, string _typeRight){
	if(_typeLeft=="procedure"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			vector<INDEX> procedureUses= procTable->getUses(_allLeftEntry.at(i));
			for(int j=0; j<procedureUses.size(); j++){
				INDEX varIndex = procedureUses.at(j);
				string usesVar = varTable->getVarName(varIndex);
				for(int k=0; k<_allRightEntry.size(); k++){
					if(usesVar == _allRightEntry.at(k)){
						correctEntry.push_back(usesVar);
						queryHasTrue = true;
						break;
					}
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}

	}else if(_allLeftEntry.size()==1 && procTable->isProcName(_allLeftEntry.at(0))){
		vector<VALUE> correctEntry; 
			vector<INDEX> procedureUses= procTable->getUses(_allLeftEntry.at(0));
			for(int j=0; j<procedureUses.size(); j++){
				INDEX varIndex = procedureUses.at(j);
				string usesVar = varTable->getVarName(varIndex);
				for(int k=0; k<_allRightEntry.size(); k++){
					if(usesVar == _allRightEntry.at(k)){
						correctEntry.push_back(usesVar);
						queryHasTrue = true;
						break;
					}
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueRight, correctEntry);
			}
	
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				if(uses->isUses(intLeft, entryRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateModifiesBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry,string _typeLeft, string _typeRight){
	if(_typeLeft=="procedure"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			vector<INDEX> procedureModifies= procTable->getModifies(_allLeftEntry.at(i));
			for(int j=0; j<procedureModifies.size(); j++){
				INDEX varIndex = procedureModifies.at(j);
				string modifiesVar = varTable->getVarName(varIndex);
				for(int k=0; k<_allRightEntry.size(); k++){
					if(modifiesVar == _allRightEntry.at(k)){
						correctEntry.push_back(modifiesVar);
						queryHasTrue = true;
						break;
					}
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}

	}else if(_allLeftEntry.size()==1 && procTable->isProcName(_allLeftEntry.at(0))){
		vector<VALUE> correctEntry; 
			vector<INDEX> procedureModifies= procTable->getModifies(_allLeftEntry.at(0));
			for(int j=0; j<procedureModifies.size(); j++){
				INDEX varIndex = procedureModifies.at(j);
				string modifiesVar = varTable->getVarName(varIndex);
				for(int k=0; k<_allRightEntry.size(); k++){
					if(modifiesVar == _allRightEntry.at(k)){
						correctEntry.push_back(modifiesVar);
						queryHasTrue = true;
						break;
					}
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueRight, correctEntry);
			}
	
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intLeft = atoi( entryLeft.c_str() );
				if(modifies->isModifies(intLeft, entryRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateCallsBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			if(calls->isCalls(entryLeft, entryLeft)){
				correctEntry.push_back(entryLeft);
				queryHasTrue = true;
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{	
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				if(calls->isCalls(entryLeft, entryRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateCallsStarBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			if(calls->isCallsStar(entryLeft, entryLeft)){
				correctEntry.push_back(entryLeft);
				queryHasTrue = true;
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				if(calls->isCallsStar(entryLeft, entryRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateNextBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			// get CFG
			//string procName = procTable->getProcedure(intLeft);
			cfg =  procTable->getCFG(intLeft);	
			if(cfg->isNext(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
				queryHasTrue = true;
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			int intLeft = atoi( entryLeft.c_str() );
			vector<VALUE> correctEntry; 
			// get CFG
			//string procName = procTable->getProcedure(intLeft);
			cfg =  procTable->getCFG(intLeft);	
			//to cut
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intRight = atoi( entryRight.c_str() );
				if(cfg->isNext(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateNextStarBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			// get CFG
			//string procName = procTable->getProcedure(intLeft);
			cfg =  procTable->getCFG(intLeft);
			if(cfg->isNextStar(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
				queryHasTrue = true;
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			int intLeft = atoi( entryLeft.c_str() );
			vector<VALUE> correctEntry; 
			// get CFG
			//string procName = procTable->getProcedure(intLeft);
			cfg =  procTable->getCFG(intLeft);
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intRight = atoi( entryRight.c_str() );
				if(cfg->isNextStar(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}		
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateAffectsBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(affects->isAffects(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
				queryHasTrue = true;
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{	
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			int intLeft = atoi( entryLeft.c_str() );
			vector<VALUE> correctEntry; 
			//to cut
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intRight = atoi( entryRight.c_str() );
				if(affects->isAffects(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateAffectsStarBranch(string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_valueLeft==_valueRight && _valueLeft!="_"){
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			vector<VALUE> correctEntry; 
			int intLeft = atoi( entryLeft.c_str() );
			if(affects->isAffectsStar(intLeft, intLeft)){
				correctEntry.push_back(entryLeft);
				queryHasTrue = true;
			}
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}		
	}else{	
		for(int i=0; i<_allLeftEntry.size(); i++){
			string entryLeft = _allLeftEntry.at(i);
			int intLeft = atoi( entryLeft.c_str() );
			vector<VALUE> correctEntry; 
			for(int j=0; j<_allRightEntry.size(); j++){
				string entryRight = _allRightEntry.at(j); 
				int intRight = atoi( entryRight.c_str() );
				if(affects->isAffectsStar(intLeft, intRight)){
					correctEntry.push_back(entryRight);
					queryHasTrue = true;
				}
			}		
			if(correctEntry.size()>0){
				table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
			}
		}
	}
}

void evaluator::evaluateContainsBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	if(_typeLeft=="if" && _typeRight=="variable"){

	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}else if(_typeLeft==" " && _typeRight==" "){
	}


}

void evaluator::evaluateContainsStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry){
	for(int i=0; i<_allLeftEntry.size(); i++){
		string entryLeft = _allLeftEntry.at(i);
		int intLeft = atoi( entryLeft.c_str() );
		vector<VALUE> correctEntry; 
		for(int j=0; j<_allRightEntry.size(); j++){
			string entryRight = _allRightEntry.at(j); 
			int intRight = atoi( entryRight.c_str() );
			if(affects->isAffectsStar(intLeft, intRight)){
				correctEntry.push_back(entryRight);
				queryHasTrue = true;
			}
		}		
		if(correctEntry.size()>0){
			table.insert(_valueLeft, entryLeft, _valueRight, correctEntry);
		}
	}
}

vector<vector<string>> convertToStringVectorResults(vector<int> _intResults, string _type){
	vector<vector<string>> result;
	for(int i=0; i<_intResults.size(); i++){	
		string stringResult = std::to_string(static_cast<long long>( _intResults.at(i)));
		vector<string> tempStringResult;
		tempStringResult.push_back(stringResult);
		tempStringResult.push_back(_type);
		result.push_back(tempStringResult);
	}
	return result;
}

vector<string> convertToStringResults(vector<int> _intResults, string _type){
	vector<string> result;
	for(int i=0; i<_intResults.size(); i++){	
		string stringResult = std::to_string(static_cast<long long>( _intResults.at(i)));
		result.push_back(stringResult);
	}
	return result;
}

vector<string> evaluator::getAllPossibleByType(string _selectType){
	string nodeType;	
	if(_selectType == "stmt" || _selectType == "prog_line"){
		return optimisedCaller->getAllStmt();
	}else if(_selectType == "assign"){
		return optimisedCaller->getAllAssignStmt();
	}else if(_selectType == "while"){
		return optimisedCaller->getAllWhileStmt();
	}else if(_selectType == "variable"){
		nodeType = "varNode";
		vector<vector<string>> varResult;
		return varTable->getAllVar();
	}else if(_selectType == "constant"){
		return optimisedCaller->getAllConstant();
	}else if(_selectType == "procedure"){
		vector<string> results;
		results=procTable->getAllProcNames();
		return results; 
	}else if(_selectType == "if"){
		return optimisedCaller->getAllIfStmt();
	}else if(_selectType == "call"){
		return optimisedCaller->getAllCallsStmt();
	}else if(_selectType == "stmtLst"){		 
		return optimisedCaller->getAllStmtLst();
	}
}

vector<vector<string>> evaluator::convertSolution(vector<string> _solution){
	vector<vector<string>> returnsol;
	for(int i=0; i<_solution.size(); i++){
		vector<string> temp;
		temp.push_back(_solution.at(i));
		returnsol.push_back(temp);
	}
	return returnsol;
}

string evaluator::getProcName(string _type, string _valueOrName){
	if(_type=="call"){
		return ast->getStmtNode(atoi( _valueOrName.c_str())).getNodeValue();
	}
	return _valueOrName;
}

vector<string> evaluator::getResults(queryTree* _queryTree){
	vector<string> results;
	int selectSize = _queryTree->getSelectSize();
	if(selectSize == 0){
		return results;
	}else if(selectSize == 1){
		string select = _queryTree->getSelect(0).at(0);
		string selectType =  _queryTree->getSelect(0).at(1);
		if(table.hasColumns(select)){
			return table.getColumn(select);
		}else{
			return getAllPossibleByType(selectType);
		}
	}else{
		vector<string> selectResults;
		for(int i=0; i< selectSize; i++){
			string select = _queryTree->getSelect(i).at(0);
			string selectType =  _queryTree->getSelect(i).at(1);
			//vector<string> tempResults;
			if(!table.hasColumns(select)){
				table.insert(select, getAllPossibleByType(selectType));
			}
			selectResults.push_back(select);
			
			/*if(table.hasColumns(select)){
				selectResults = table.getColumn(select);
			}else{
				selectResults = getAllPossibleByType(selectType);
			}
			
			if(results.size()==0){
				results=selectResults;
			}else{
				//optimisation R+R*s, R<s
				if(results.size()<=selectResults.size()){
					for(int j=0; j<results.size(); j++){
						string singleResultClause = results.at(j);
						for(int k=0; k<selectResults.size(); k++){
							tempResults.push_back(singleResultClause+" "+selectResults.at(k));
						}
					}
				}else{
					for(int j=0; j<selectResults.size(); j++){
						string singleSelectClause = selectResults.at(j);
						for(int k=0; k<results.size(); k++){
							tempResults.push_back(results.at(k)+" "+singleSelectClause);
						}
					}
				}
				results=tempResults;
			}*/
		}
		//return results;
		return table.getTuple(selectResults);
	}
}

vector<vector<string>>  evaluator::optimiseQuery(queryTree* _queryTree){
	vector<vector<string>> tier1;
	vector<vector<string>> tier2;
	vector<vector<string>> tier3;
	
	//eval with tree
	for(int i=0; i<_queryTree->getWithQuerySize(); i++){
		WITHBRANCH withTree = _queryTree->getWithQuery(i);
		string typeLeft = withTree.at(0);
		string typeRight = withTree.at(4);
		if( (typeLeft=="Integer" || typeLeft=="Ident") && (typeRight=="Integer" || typeRight=="Ident") ){
			withTree.push_back("with");
			tier1.push_back(withTree);
		}else if( (typeLeft=="Integer" || typeLeft=="Ident") || (typeRight=="Integer" || typeRight=="Ident") ){
			withTree.push_back("with");
			tier2.push_back(withTree);
		}else{
			withTree.push_back("with");
			tier3.push_back(withTree);
		}
	}
	//eval suchthat tree
	vector<vector<string>> tier1NextStar;
	vector<vector<string>> tier2NextStar;
	vector<vector<string>> tier3NextStar;
	vector<vector<string>> tier1Affects;
	vector<vector<string>> tier2Affects;
	vector<vector<string>> tier3Affects;
	vector<vector<string>> tier1AffectsStar;
	vector<vector<string>> tier2AffectsStar;
	vector<vector<string>> tier3AffectsStar;
	for(int i=0; i<_queryTree->getSuchThatQuerySize(); i++){
		QUERYBRANCH suchThatQuery = _queryTree->getSuchThatQuery(i);
		string typeLeft = suchThatQuery.at(2);
		string typeRight = suchThatQuery.at(4);
		string queryType = suchThatQuery.at(0);
		if( (typeLeft=="Integer" || typeLeft=="Ident") && (typeRight=="Integer" || typeRight=="Ident") ){
			suchThatQuery.push_back("suchThat");
			if(queryType=="Next*"){
				tier1NextStar.push_back(suchThatQuery);
			}else if(queryType=="Affects"){
				tier1Affects.push_back(suchThatQuery);
			}else if(queryType=="AFfects*"){
				tier1AffectsStar.push_back(suchThatQuery);
			}else{
				tier1.push_back(suchThatQuery);
			}
		}else if( (typeLeft=="Integer" || typeLeft=="Ident") || (typeRight=="Integer" || typeRight=="Ident") ){
			suchThatQuery.push_back("suchThat");
			if(queryType=="Next*"){
				tier2NextStar.push_back(suchThatQuery);
			}else if(queryType=="Affects"){
				tier2Affects.push_back(suchThatQuery);
			}else if(queryType=="AFfects*"){
				tier2AffectsStar.push_back(suchThatQuery);
			}else{
				tier2.push_back(suchThatQuery);
			}
		}else{
			suchThatQuery.push_back("suchThat");
			if(queryType=="Next*"){
				tier3NextStar.push_back(suchThatQuery);
			}else if(queryType=="Affects"){
				tier3Affects.push_back(suchThatQuery);
			}else if(queryType=="AFfects*"){
				tier3AffectsStar.push_back(suchThatQuery);
			}else{
				tier3.push_back(suchThatQuery);
			}
		}
	}
	tier1.insert(tier1.end(), tier1NextStar.begin(), tier1NextStar.end());
	tier1.insert(tier1.end(), tier1Affects.begin(), tier1Affects.end());
	tier1.insert(tier1.end(), tier1AffectsStar.begin(), tier1AffectsStar.end());
	tier2.insert(tier2.end(), tier2NextStar.begin(), tier2NextStar.end());
	

	//evaluate pattern 
	for(int i=0; i<_queryTree->getPatternQuerySize(); i++){
		PATTERNBRANCH patternTree = _queryTree->getPatternQuery(i);
		string typeLeft = patternTree.at(3);
		string typeRight = patternTree.at(4);
		string patternValueRight = patternTree.at(5);
		bool rightIsSingleExpression = (typeRight=="Var_name" || typeRight=="expr" || typeRight=="_expr_");
		if( typeLeft=="Ident" && rightIsSingleExpression ){
			patternTree.push_back("pattern");
			tier1.push_back(patternTree);
		}else if( typeLeft=="Ident" || rightIsSingleExpression ){
			patternTree.push_back("pattern");
			tier2.push_back(patternTree);
		}else{
			patternTree.push_back("pattern");
			tier3.push_back(patternTree);
		}
	}
	//combine all
	tier2.insert(tier2.end(), tier2Affects.begin(), tier2Affects.end());
	tier2.insert(tier2.end(), tier2AffectsStar.begin(), tier2AffectsStar.end());
	tier3.insert(tier3.end(), tier3NextStar.begin(), tier3NextStar.end());
	tier3.insert(tier3.end(), tier3Affects.begin(), tier3Affects.end());
	tier3.insert(tier3.end(), tier3AffectsStar.begin(), tier3AffectsStar.end());

	tier1.insert(tier1.end(), tier2.begin(), tier2.end());
	tier1.insert(tier1.end(), tier3.begin(), tier3.end());
	return tier1;
}
