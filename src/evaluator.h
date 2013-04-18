#pragma once
#include "queryTree.h"
#include "PKB.h"
#include "AST.h"
#include "TNode.h"
#include <string>
#include <vector>
#include "Follows.h"
#include "Parent.h"
#include "Uses.h"
#include "Modifies.h"
#include "VarTable.h"
#include "Table.h"
#include "Calls.h"
#include "ProcTable.h"
#include "CFG.h"
#include "TNode.h"
#include "ASTBuilder.h"
#include "Affects.h"
#include "CFGBip.h"

using namespace std;
typedef vector<string> QUERYBRANCH;
typedef vector<string> PATTERNBRANCH;
typedef vector<string> WITHBRANCH;

class evaluator
{
public:
	evaluator(void);
	~evaluator(void);
	evaluator(queryTree _queryTree);
	AST* ast;
	Follows* follows;
	Parent* parent;
	Uses* uses;
	Modifies* modifies;
	VarTable* varTable;
	Calls* calls;
	ProcTable* procTable;
	CFG* cfg;
	Affects* affects;
	OptimisedCaller* optimisedCaller;
	CFGBip * cfgBip;
	StmtTable * stmtTable;

	Table table;
	//ASTBuilder astBuilder;

	bool queryHasTrue;
	bool patternHasTrue;
	bool withHasTrue;

	//insertPKB
	void insertAST(AST* _ast);
	void insertFollows(Follows* _follows);
	void insertParent(Parent* _parent);
	void insertModifies(Modifies* _modifies);
	void insertUses(Uses* _uses);
	void insertVarTable(VarTable* _varTable);
	void insertCalls(Calls* _calls);
	void insertProcTable(ProcTable* _procTable);
	void insertCFG(CFG* _cfg);
	void insertAffects(Affects* affects);
	void insertOptimisedCaller(OptimisedCaller* OptimisedCaller);
	void insertCFGBip(CFGBip * cfgBip);
	void insertStmtTable(StmtTable * stmtTable);

	vector<string> evaluateQuery(queryTree* _queryTree);


private:
	vector<vector<string>> getAllByType(string _select, string _selectType, queryTree* _queryTree);
	
	////////cs3202////////

	//suchthat
	void evaluateSuchThat(string _select, string _selectType, vector<string> suchThatQuery);
	//void evaluateSuchThat(string _select, string _selectType, queryTree* _queryTree);
	void evaluateBranch(string _queryType, string _valueLeft, string _valueRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, string _typeLeft, string _typeRight, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateFollowsBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateFollowsStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateParentBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateParentStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateUsesBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateModifiesBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateCallsBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateCallsStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateNextBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateNextStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateAffectsBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateAffectsStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluatePreContainsBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluatePreContainsStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateContainsBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateContainsStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateNextBipBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateNextBipStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateAffectsBipBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateAffectsBipStarBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);
	void evaluateSiblingBranch(string _valueLeft, string _valueRight, string _typeLeft, string _typeRight, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<vector<string>> columnPair, bool tupleActivated);

	//pattern
	void evaluatePattern(vector<string> patternTree);
	//void evaluatePattern(queryTree* _queryTree);
	void evaluatePatternBranch(string _patternStmt, string _typeStmt, string _typeLeft, string _valueLeft, string _typeRight, 
		string _valueRight, vector<VALUE> _allStmtEntry, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);
	void evaluateIfPatternBranch(string _patternStmt, string _typeStmt, string _typeLeft, string _valueLeft, string _typeRight, 
		string _valueRight, string _typeRight2, string _valueRight2, vector<VALUE> _allStmtEntry, 
		vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry, vector<VALUE> _allRight2Entry);
	void evaluateWhilePatternBranch(string _patternStmt, string _typeStmt, string _typeLeft, string _valueLeft, string _typeRight, 
		string _valueRight, vector<VALUE> _allStmtEntry, vector<VALUE> _allLeftEntry, vector<VALUE> _allRightEntry);

	//with
	//void evaluateWith(string _select, string _selectType, queryTree* _queryTree);
	void evaluateWith(string _select, string _selectType, vector<string> withTree);

	//helper
	vector<string> getAllPossibleByType(string _selectType);
	vector<vector<string>> convertSolution(vector<string> _solution);
	string getProcName(string _typr, string _valueOrName);
	vector<string> getResults(queryTree* _queryTree);
	bool isElseStmtLst(int intStmt, int stmtLstNum);
	bool isChildVariable(int intStmt, string controlVariable);
	bool isChildVariableStar(int intStmt, string variable);
	bool isChildConstant(int intStmt, string controlVariable);
	bool isChildConstantStar(int intStmt, string variable);
	bool isChildMathStar(int intStmt, string variable);
	bool isInStmtList(int, int);
	bool varInProcList(int, string);
	bool varInProcListStar(int, string);
	bool isSiblingByIndexes(vector<int> nodeIndexLeft, vector<int> nodeIndexRight);
	vector<int> toNode(string, string);
	//vector<string> getAllStmtLst();
	vector<vector<string>> optimiseQuery(queryTree* _queryTree);
	vector<vector<string>> optimiseQuery2(queryTree* _queryTree);
	vector<vector<string>> optimiseQueryPartial(vector<vector<string>>);
	////////cs3202////////


};

