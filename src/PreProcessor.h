#pragma once
#include <cstdio>
#include <boost/tokenizer.hpp>
#include "queryTree.h"
#include <ctype.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
using namespace boost;
typedef string token;

class PreProcessor {
public:
	/// Default constructor
	PreProcessor();
	/// Method to receive a query, parse it, validate it and return a query tree representation of the query
	queryTree* preProcess(string query);
	string getTokenType(string pendingToken); 

private:
	/* CS3202 METHODS */
	string getSynonymTypeFromString(string pendingSynonym);
	bool isValidTuple(string pendingTuple);
	void parseQuery(string queryInOneLine);
	token nextToken();
	token consumeToken();
	void groupQuery();
	bool areValidDeclarations();
	bool isValidSynonymSyntax(string pendingSynonym);	// Only checks the syntax
	bool areValidSuchThats();
	bool isValidRelation(string relation, string arg1, string arg2);
	bool areValidPatterns();
	bool isValidIfPattern(string pattern, string pattArg1, string pattArg2, string pattArg3);
	string getVarRefType(string pendingVarRef);
	string getExprSpecType(string pendingExprSpec);
	bool isValidExpression(string pendingExpression);
	bool areValidWiths();
	bool isValidWith(string withLHS, string withRHS);
	string getSynonymType(int index);
	vector<string> splitByDot(string toSplit);
	bool isValidInteger(string pendingInteger);
	string getAfterDot(string toSplit);
	string getBeforeDot(string toSplit);
	bool isValidAttrRef(string designEntity, string pendingAttrName);
	vector<string> getAttRefTypes(string pendingAttRef);
	string getQuotationRemoved(string inputString);
	bool areValidSelects();
	bool isValidSelect(string pendingSelect);
	vector<string> getRefTypes(string pendingRef);
	bool isValidTerm(string pendingTerm);
	bool isValidFactor(string pendingFactor);
	string getTrimmedExpression(string inputString);
	
	/* CS3201 Methods */
	bool isValidName(string pendingName);
	bool isValidIdent(string pendingIdent);
	string getEntRefType(string pendingEntRef);
	bool isValidDesignEntity(string pendingDesEnt);	
	bool isValidPattern(string pattern, string pattArg1, string pattArg2);
	// Returns false if one of the synonyms has already been declared
	bool saveSynonyms(string currentDesignEntity, vector<string> * currentSynonyms);
	int getSynonymIndex(string entValue);

	/* VARIABLES */
	// Query Tree
	queryTree * qt;
	// Tables
	vector<string> allowedDesignEntities;
	vector<vector<string>> allowedRelations;
	vector<vector<string>> declaredSynonyms;
	vector<vector<string>> allowedAttRefs;
	// Parsing
	char_separator<char> seperator;
	tokenizer<char_separator<char>> * tokens;
	tokenizer<char_separator<char>>::iterator tok_iter;
	vector<string> declarations, afterDeclarations, pendingSelects;
	vector<vector<string>> pendingPattern, pendingSuchThat, pendingWith;
};