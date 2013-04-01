//#ifndef PKB_H
//#define PKB_H

#pragma once
#include "AST.h"
#include "VarTable.h"
#include "Follows.h"
#include "Parent.h"
#include "Modifies.h"
#include "Uses.h"
#include "Calls.h"
#include "ProcTable.h"
#include "Affects.h"
#include "OptimisedCaller.h"

class PKB{
public:
	//constructors
	PKB();
	PKB(AST *, VarTable *, Follows *, Parent *, Modifies *, Uses *, ProcTable *, Calls *, Affects *, OptimisedCaller *);

	//destructors
	~PKB();

	void setAST(AST *);
	void setVarTable(VarTable *);
	void setFollows(Follows *);
	void setParent(Parent *);
	void setModifies(Modifies *);
	void setUses(Uses *);
	void setCalls(Calls *);
	void setProcTable(ProcTable *);
	void setAffects(Affects *);
	void setOptimisedCaller(OptimisedCaller *);

	AST* getAST();
	VarTable* getVarTable();
	Follows* getFollows();
	Parent * getParent();
	Uses * getUses();
	Modifies * getModifies();
	Calls * getCalls();
	ProcTable * getProcTable();
	Affects * getAffects();
	OptimisedCaller * getOptimisedCaller();

private:
	AST * ast;
	VarTable * varTable;
	Follows * follows;
	Parent * parent;
	Modifies * modifies;
	Uses * uses;
	Calls * calls;
	ProcTable * procTable;
	Affects * affects;
	OptimisedCaller * optimisedCaller;
};

//#endif