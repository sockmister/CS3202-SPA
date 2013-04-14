#pragma once
#include "PreProcessor.h"
//#include "AbstractWrapper.h"

// Default constructor
PreProcessor::PreProcessor() {
	/* Program Model Tables */

	// Scope of design entities
	//string tempDesignEntities[] = {"assign", "while", "stmt", "constant", "variable", "prog_line"};
	//allowedDesignEntities.assign(tempRow, tempRow + sizeof(tempRow) / sizeof(tempRow[0]));
	allowedDesignEntities.push_back("assign");
	allowedDesignEntities.push_back("while");
	allowedDesignEntities.push_back("stmt");
	allowedDesignEntities.push_back("constant");
	allowedDesignEntities.push_back("variable");
	allowedDesignEntities.push_back("prog_line");
	allowedDesignEntities.push_back("procedure");
	allowedDesignEntities.push_back("stmtLst");
	allowedDesignEntities.push_back("call");
	allowedDesignEntities.push_back("if");
	allowedDesignEntities.push_back("plus");
	allowedDesignEntities.push_back("minus");
	allowedDesignEntities.push_back("times");

	// Scope of relations and their allowed argument types.
	//string tempRow[] = {"Follows", "2", "assign", "while", "stmt", "prog_line", "Integer", ";", "assign", "while", "stmt", "prog_line", "Integer", "_", ";"};
	//allowedRelations.assign(tempRow, tempRow + sizeof(tempRow) / sizeof(tempRow[0]));
	vector<string> tempRow;
	tempRow.push_back("Follows");
	tempRow.push_back("2");
	tempRow.push_back("assign");
	tempRow.push_back("while");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("if");
	tempRow.push_back("call");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	tempRow.push_back("assign");
	tempRow.push_back("while");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("if");
	tempRow.push_back("call");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Follows*");
	tempRow.push_back("2");
	tempRow.push_back("assign");
	tempRow.push_back("while");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("if");
	tempRow.push_back("call");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	tempRow.push_back("assign");
	tempRow.push_back("while");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("if");
	tempRow.push_back("call");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");		
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	// Can stmtLst be arg1 of Parent?
	tempRow.push_back("Parent");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("while");
	tempRow.push_back("if");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("while");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("call");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");		
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	// Can stmtLst be arg1 of Parent*?
	tempRow.push_back("Parent*");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("while");
	tempRow.push_back("if");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("while");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("call");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Modifies");
	tempRow.push_back("2");
	tempRow.push_back("assign");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("while");
	tempRow.push_back("if");
	tempRow.push_back("procedure");
	tempRow.push_back("call");
	tempRow.push_back("Integer");
	tempRow.push_back("Ident");
	tempRow.push_back(";");
	tempRow.push_back("variable");
	tempRow.push_back("Ident");
	tempRow.push_back("_");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Uses");
	tempRow.push_back("2");
	tempRow.push_back("assign");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("while");
	tempRow.push_back("if");
	tempRow.push_back("call");
	tempRow.push_back("procedure");
	tempRow.push_back("Integer");
	tempRow.push_back("Ident");
	tempRow.push_back(";");
	tempRow.push_back("variable");
	tempRow.push_back("Ident");
	tempRow.push_back("_");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	// Can take Integer? If can need include all the stmt types?
	tempRow.push_back("Calls");
	tempRow.push_back("2");
	tempRow.push_back("procedure");
	tempRow.push_back("_");
	tempRow.push_back("Ident");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	tempRow.push_back("procedure");
	tempRow.push_back("_");
	tempRow.push_back("Ident");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	// Can take Integer? If can need include all the stmt types?
	tempRow.push_back("Calls*");
	tempRow.push_back("2");
	tempRow.push_back("procedure");
	tempRow.push_back("_");
	tempRow.push_back("Ident");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	tempRow.push_back("procedure");
	tempRow.push_back("_");
	tempRow.push_back("Ident");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Next");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("while");
	tempRow.push_back("call");
	tempRow.push_back("prog_line");
	tempRow.push_back("_");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("while");
	tempRow.push_back("call");
	tempRow.push_back("prog_line");
	tempRow.push_back("_");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Next*");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("while");
	tempRow.push_back("call");
	tempRow.push_back("prog_line");
	tempRow.push_back("_");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("while");
	tempRow.push_back("call");
	tempRow.push_back("prog_line");
	tempRow.push_back("_");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Affects");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("assign");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("assign");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Affects*");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("prog_line");	
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("prog_line");	
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Contains");
	tempRow.push_back("2");
	tempRow.push_back("procedure");	
	tempRow.push_back("stmtLst");	
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("call");
	tempRow.push_back("while");
	tempRow.push_back("if");	
	tempRow.push_back("plus");	
	tempRow.push_back("minus");
	tempRow.push_back("times");	
	tempRow.push_back("variable");	
	tempRow.push_back("constant");	
	tempRow.push_back("Integer");
	tempRow.push_back("prog_line");
	tempRow.push_back("Ident");
	tempRow.push_back(";");
	tempRow.push_back("procedure");	
	tempRow.push_back("stmtLst");	
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("call");
	tempRow.push_back("while");
	tempRow.push_back("if");	
	tempRow.push_back("plus");	
	tempRow.push_back("minus");
	tempRow.push_back("times");	
	tempRow.push_back("variable");	
	tempRow.push_back("constant");	
	tempRow.push_back("Integer");
	tempRow.push_back("prog_line");
	tempRow.push_back("Ident");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Contains*");
	tempRow.push_back("2");
	tempRow.push_back("procedure");	
	tempRow.push_back("stmtLst");	
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("call");
	tempRow.push_back("while");
	tempRow.push_back("if");	
	tempRow.push_back("plus");	
	tempRow.push_back("minus");
	tempRow.push_back("times");	
	tempRow.push_back("variable");	
	tempRow.push_back("constant");	
	tempRow.push_back("Integer");
	tempRow.push_back("prog_line");
	tempRow.push_back("Ident");
	tempRow.push_back(";");
	tempRow.push_back("procedure");	
	tempRow.push_back("stmtLst");	
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("call");
	tempRow.push_back("while");
	tempRow.push_back("if");	
	tempRow.push_back("plus");	
	tempRow.push_back("minus");
	tempRow.push_back("times");	
	tempRow.push_back("variable");	
	tempRow.push_back("constant");	
	tempRow.push_back("Integer");
	tempRow.push_back("prog_line");
	tempRow.push_back("Ident");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("Sibling");
	tempRow.push_back("2");
	tempRow.push_back("procedure");	
	tempRow.push_back("stmtLst");	
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("call");
	tempRow.push_back("while");
	tempRow.push_back("if");	
	tempRow.push_back("plus");	
	tempRow.push_back("minus");
	tempRow.push_back("times");	
	tempRow.push_back("variable");	
	tempRow.push_back("constant");	
	tempRow.push_back("Integer");
	tempRow.push_back("prog_line");
	tempRow.push_back("Ident");
	tempRow.push_back(";");
	tempRow.push_back("procedure");	
	tempRow.push_back("stmtLst");	
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("call");
	tempRow.push_back("while");
	tempRow.push_back("if");	
	tempRow.push_back("plus");	
	tempRow.push_back("minus");
	tempRow.push_back("times");	
	tempRow.push_back("variable");	
	tempRow.push_back("constant");	
	tempRow.push_back("Integer");
	tempRow.push_back("prog_line");
	tempRow.push_back("Ident");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("NextBip");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("while");
	tempRow.push_back("call");
	tempRow.push_back("prog_line");
	tempRow.push_back("_");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("while");
	tempRow.push_back("call");
	tempRow.push_back("prog_line");
	tempRow.push_back("_");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("NextBip*");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("while");
	tempRow.push_back("call");
	tempRow.push_back("prog_line");
	tempRow.push_back("_");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("if");
	tempRow.push_back("while");
	tempRow.push_back("call");
	tempRow.push_back("prog_line");
	tempRow.push_back("_");
	tempRow.push_back("Integer");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("AffectsBip");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("assign");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("prog_line");
	tempRow.push_back("assign");
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("AffectsBip*");
	tempRow.push_back("2");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("prog_line");	
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	tempRow.push_back("stmt");
	tempRow.push_back("assign");
	tempRow.push_back("prog_line");	
	tempRow.push_back("Integer");
	tempRow.push_back("_");
	tempRow.push_back(";");
	allowedRelations.push_back(tempRow);
	tempRow.clear();

	// Allowed pairings of Design Entities and attNames
	tempRow.push_back("procedure");
	tempRow.push_back("procName");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("stmtLst");
	tempRow.push_back("stmt#");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("stmt");
	tempRow.push_back("stmt#");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("assign");
	tempRow.push_back("stmt#");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("call");
	tempRow.push_back("procName");
	tempRow.push_back("stmt#");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("while");
	tempRow.push_back("stmt#");
	//tempRow.push_back("varName");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("if");
	tempRow.push_back("stmt#");
	//tempRow.push_back("varName");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("variable");
	tempRow.push_back("varName");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();
	
	tempRow.push_back("constant");
	tempRow.push_back("value");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();

	tempRow.push_back("prog_line");
	tempRow.push_back("stmt#");
	allowedAttRefs.push_back(tempRow);
	tempRow.clear();
};

// Controller of this class
queryTree* PreProcessor::preProcess(string queryInOneLine) {
	qt = new queryTree();
	bool validQuery = true;
	
	if(queryInOneLine == "") {
		validQuery = false;
		cout << "\tError: Query is empty!\n";
		cout << "Invalid query\n\n";
	}
	parseQuery(queryInOneLine);
	if(validQuery) {
		validQuery = areValidDeclarations();
		if(!validQuery) {
			cout << "Invalid Declarations for the query:\n";
			cout << queryInOneLine << "\n\n";
		}
	}
	if(validQuery) {
		validQuery = areValidSelects();
		if(!validQuery) {
			cout << "Invalid Select for the query:\n";
			cout << queryInOneLine << "\n\n";
		}
	}
	if(validQuery) {
		validQuery = areValidSuchThats();
		if(!validQuery) {
			cout << "Invalid Such That for the query:\n";
			cout << queryInOneLine << "\n\n";
		}
	}
	if(validQuery) {
		validQuery = areValidPatterns();
		if(!validQuery) {
			cout << "Invalid Pattern for the query:\n";
			cout << queryInOneLine << "\n\n";
		}
	}
	if(validQuery) {
		validQuery = areValidWiths();
		if(!validQuery) {
			cout << "Invalid With for the query:\n";
			cout << queryInOneLine << "\n\n";
		}
	}
	// Tables
	//allowedDesignEntities.clear();
	//allowedRelations.clear();
	
	// Clear variables whose data changes every query
	declaredSynonyms.clear();
	// Parsing
	//seperator.reset();
	//tok_iter.dereference;
	declarations.clear();
	afterDeclarations.clear();
	pendingSelects.clear();
	pendingPattern.clear();
	pendingSuchThat.clear();
	pendingWith.clear();
	if(validQuery) {
		cout << "---Success--- for the query:\n";
		cout << queryInOneLine << "\n";
		//cout << "-----" << "\n";
		return qt;
	}
	else {
		// Must insert select and selectType == "" else Evaluator will break
		if(qt->getSelectSize() == 1 && qt->getSelect(0)[1] == "BOOLEAN") {
			delete qt;
			qt = new queryTree();
			qt->insertSelect("BOOLEAN", "BOOLEAN");
			return qt;
		}

		delete qt;
		qt = new queryTree();
		qt->insertSelect("", "");
		return qt;
	}
}

token PreProcessor::nextToken(){
	if(tok_iter != tokens->end())
		return *tok_iter;
	else
		return "";
}

token PreProcessor::consumeToken(){
	return *tok_iter++;
}

void PreProcessor::parseQuery(string queryInOneLine) {
	seperator = char_separator<char> (" \n\t,", ";()<>=", boost::drop_empty_tokens);
	tokens = new tokenizer<char_separator<char>> (queryInOneLine, seperator);
	tok_iter = tokens->begin();
	//cout << "Parsing...\n";
	while(tok_iter != tokens->end()) {
		if(nextToken() == "Select")
			break;
		declarations.push_back(nextToken());		
		consumeToken();
	}
	while(tok_iter != tokens->end()) {
		afterDeclarations.push_back(nextToken());
		consumeToken();
	}
	groupQuery();
}

void PreProcessor::groupQuery() {
	vector<string>::iterator it = afterDeclarations.begin();
	if(it != afterDeclarations.end()) ++it;
	if(it != afterDeclarations.end() && *it == "<") {
		++it;
		while(it != afterDeclarations.end() && *it != ">") {
			pendingSelects.push_back(*it);
			if(it != afterDeclarations.end()) ++it;
		}
		if(it != afterDeclarations.end()) ++it;		// '>'
	}
	else if(it != afterDeclarations.end()){
		pendingSelects.push_back(*it);
		if(it != afterDeclarations.end()) ++it;
	}
	bool pushSuchThat = false, pushPattern = false, pushWith = false;
	while(it != afterDeclarations.end()) {
		if(*it == "and") {
			if(it != afterDeclarations.end()) ++it;
			if(it != afterDeclarations.end() && (*it == "such" || *it == "with" || *it == "pattern")) {
				pushSuchThat = false;
				pushWith = false;
				pushPattern = false;
				--it;
			}
		}
		else {
			pushSuchThat = false;
			pushPattern = false;
			pushWith = false;
			if(*it == "such") {
				pushSuchThat = true;				
				if(it != afterDeclarations.end()) ++it;
				if(it != afterDeclarations.end()) ++it;			// "that"
			}
			else if(*it == "pattern") {
				pushPattern = true;				
				if(it != afterDeclarations.end()) ++it;
			}
			else if(*it == "with") {
				pushWith = true;				
				if(it != afterDeclarations.end()) ++it;
			}		
		}
		
		if(pushSuchThat) {
			vector<string> tempRow;
			if(it != afterDeclarations.end()) tempRow.push_back(*it);	
			if(it != afterDeclarations.end()) ++it;		// Relation
			if(it != afterDeclarations.end()) ++it;		// '('
			if(it != afterDeclarations.end()) tempRow.push_back(*it); 
			if(it != afterDeclarations.end()) ++it;		// arg 1
			if(it != afterDeclarations.end()) tempRow.push_back(*it); 
			if(it != afterDeclarations.end()) ++it;		// arg 2
			pendingSuchThat.push_back(tempRow);
		}
		else if(pushPattern) {
			vector<string> tempRow;
			if(it != afterDeclarations.end()) tempRow.push_back(*it);
			if(it != afterDeclarations.end()) ++it;		// "("
			if(it != afterDeclarations.end()) ++it;								
			bool firstPass = true;
			int bracketsPassed = 1;
			while(it != afterDeclarations.end() && (bracketsPassed > 0 || *it != ")")) {
				if(firstPass) {
					--bracketsPassed;
					firstPass = false;
				}
			//while(*it != ")") {
				if(*it == "(")
					bracketsPassed++;
				if(*it == ")")
					bracketsPassed--;
				tempRow.push_back(*it); ++it;
			}
			pendingPattern.push_back(tempRow);
		}
		else if(pushWith) {
			vector<string> tempRow;
			if(it != afterDeclarations.end()) tempRow.push_back(*it);
			if(it != afterDeclarations.end()) ++it;
			if(it != afterDeclarations.end()) ++it;			// '='
			if(it != afterDeclarations.end()) tempRow.push_back(*it);
			pendingWith.push_back(tempRow);				
		}
		if(it != afterDeclarations.end()) ++it;
	}
}

bool PreProcessor::areValidDeclarations() {
	vector<string>::iterator it = declarations.begin();
	string currentDesignEntity;
	vector<string> * currentSynonyms;
	bool inValidDeclarations = false;
	//if(afterDeclarations.empty()) {
	//	inValidDeclarations = true;
	//	cout << "\tError: There are no declarations\n";
	//}
	for(it; it != declarations.end() && !inValidDeclarations; ++it) {
		currentDesignEntity = *it;
		if(!isValidDesignEntity(currentDesignEntity)) {			
			inValidDeclarations = true;
			break;
		}
		++it;
		currentSynonyms = new vector<string>();
		while(*it != ";") {
			if(!isValidSynonymSyntax(*it)) {		
				cout << "\tError: Synonym "<< *it << " in declarations not an Ident\n";
				inValidDeclarations = true;
				break;
			}
			currentSynonyms->push_back(*it);
			++it;
		}
		if(!inValidDeclarations) {
			bool noRedeclaration = saveSynonyms(currentDesignEntity, currentSynonyms);
			if(!noRedeclaration) {
				cout << "\tError: Redeclared variables\n";
				inValidDeclarations = true;
				break;
			}
		}	
	}
	if(inValidDeclarations)
		return false;
	else
		return true;
}

bool PreProcessor::areValidSelects() {
	vector<string>::iterator it = pendingSelects.begin();
	if(pendingSelects.size() == 1) {
		if(*it == "BOOLEAN")
			qt->insertSelect("BOOLEAN", "BOOLEAN");
		else if(!isValidSelect(*it)) {
			cout << "\tError: " << *it << " is not a valid Select\n";
			return false;
		}
	}
	else {
		for(it; it != pendingSelects.end(); ++it) {
			if(!isValidSelect(*it)) {
				cout << "\tError: " << *it << " is not a valid Select\n";
				return false;
			}
		}
	}
	return true;
}

bool PreProcessor::isValidSelect(string pendingSelect) {
	// Is it a valid Synonym?
	string pendingSelectType;

	vector<string> attRefTypes = getAttRefTypes(pendingSelect);
	if(!attRefTypes.empty()) {
		if(attRefTypes[2] != "call") {
			qt->insertSelect(attRefTypes[1], attRefTypes[2]);
			return true;
		}
		else {
			qt->insertSelect(attRefTypes[1], attRefTypes[3]);
			return true;
		}
	}
	else {	
		int pendingSelectTypeIndex = getSynonymIndex(pendingSelect);
		if(pendingSelectTypeIndex != -1)
			pendingSelectType = getSynonymType(pendingSelectTypeIndex);
		else
			return false;
		qt->insertSelect(pendingSelect, pendingSelectType);
		return true;
	}
}

bool PreProcessor::areValidSuchThats() {
	vector<vector<string>>::iterator it = pendingSuchThat.begin();
	for(it; it != pendingSuchThat.end(); ++it) {
		if(!isValidRelation(it->at(0), it->at(1), it->at(2)))
			return false;		
	}
	return true;
}

// Validates all patterns in query then adds them to queryTree. Returns true is all are valid and false upon the first invalid pattern encountered
bool PreProcessor::areValidPatterns() {
	vector<vector<string>>::iterator it = pendingPattern.begin();
	for(it; it != pendingPattern.end(); ++it) {
		string patternSynonymType;
		int patternSynonymIndex = getSynonymIndex(it->at(0));		
		if(patternSynonymIndex != -1) {
			patternSynonymType =  getSynonymType(getSynonymIndex(it->at(0)));
		}
		else {
			cout << "\tError: " << it->at(0) << " used as the (assign/while/if) synonym of pattern has not been declared\n";
			return false;
		}
		if(patternSynonymType == "if") {
			if(!isValidIfPattern(it->at(0), it->at(1), it->at(2), it->at(3)))
				return false;
		}
		else {
			string patternArg2;
			for(size_t i=2; i < it->size(); i++) {
				patternArg2 += it->at(i);
				/*if(i != it->size()-1)
					patternArg2 += " ";*/
			}
			if(!isValidPattern(it->at(0), it->at(1), patternArg2))
				return false;
		}
	}
	return true;
}

bool PreProcessor::areValidWiths() {
	vector<vector<string>>::iterator it = pendingWith.begin();
	for(it; it != pendingWith.end(); ++it) {
		if(!isValidWith(it->at(0), it->at(1)))
			return false;
	}
	return true;
}

bool PreProcessor::isValidWith(string withLHS, string withRHS) {
	vector<string> LHSRefTypes, RHSRefTypes;
	LHSRefTypes = getRefTypes(withLHS);
	if(LHSRefTypes.empty()) {
		cout << "\tError: " << withLHS << " which is on the LHS of " << "=" << withRHS << " is not a valid refType\n";
		return false;
	}
	RHSRefTypes = getRefTypes(withRHS);
	if(RHSRefTypes.empty()) {
		cout << "\tError: " << withRHS << " which is on the RHS of " << withLHS << "=" << " is not a valid refType\n";
		return false;
	}
	if(LHSRefTypes[4] != RHSRefTypes[4]) {
		cout << "\tError: " << withLHS << " and " << withRHS << " are not of a matching type (String VS Integer)\n";
		return false;
	}
	qt->insertWith(LHSRefTypes[0], LHSRefTypes[1], LHSRefTypes[2], LHSRefTypes[3], RHSRefTypes[0], RHSRefTypes[1], RHSRefTypes[2], RHSRefTypes[3]);
	return true;
}

vector<string> PreProcessor::getRefTypes(string pendingRef) {
	vector<string> tempRow, attRefTypes;
	
	// Is it an attRef?
	attRefTypes = getAttRefTypes(pendingRef);
	if(!attRefTypes.empty())
		return attRefTypes;

	// Is it an Integer?
	if(isValidInteger(pendingRef)) {
		tempRow.push_back("Integer");
		tempRow.push_back(pendingRef);
		tempRow.push_back("");
		tempRow.push_back("");
		tempRow.push_back("Integer");
		return tempRow;
	}
	
	// Is it a prog_line?
	int pendingRefSynonymIndex = getSynonymIndex(pendingRef);
	string pendingRefSynonymType;
	if(pendingRefSynonymIndex != -1)
		pendingRefSynonymType = getSynonymType(pendingRefSynonymIndex);
	if(pendingRefSynonymType == "prog_line") {
		tempRow.push_back("prog_line");
		tempRow.push_back(pendingRef);
		tempRow.push_back("");
		tempRow.push_back("");
		tempRow.push_back("Integer");
		return tempRow;
	}
	
	// Is it an Ident?
	string pendingIdent = getQuotationRemoved(pendingRef);
	if(isValidIdent(pendingIdent)) {
		tempRow.push_back("Ident");
		tempRow.push_back(pendingIdent);
		tempRow.push_back("");
		tempRow.push_back("");
		tempRow.push_back("String");
		return tempRow;
	}
	return tempRow;
}

vector<string> PreProcessor::getAttRefTypes(string pendingAttRef) {
	vector<string> tempRow;
	if(pendingAttRef.find(".") == -1)
		return tempRow;
	string pendingSynonym = getBeforeDot(pendingAttRef);
	string pendingAttName = getAfterDot(pendingAttRef);
	int synonymIndex = getSynonymIndex(pendingSynonym);
	if(synonymIndex == -1) {
		//cout << "\tError: Synonym used in attRef: " << pendingAttRef << " has not been delcared\n";
		return tempRow;
	}
	else {
		string synonymType = getSynonymType(synonymIndex);
		if(!isValidAttrRef(synonymType, pendingAttName)) {
			//cout << "\tError: attrName used in attRef is not compatible with " << synonymType << "\n";
			return tempRow;
		}
		else {
			// Can adjust later for evaluator
			//withArg1Type = synonymType + "." + pendingAttrName;
			tempRow.push_back("attRef");
			tempRow.push_back(pendingSynonym);
			tempRow.push_back(synonymType);
			tempRow.push_back(pendingAttName);
			if(pendingAttName == "varName" || pendingAttName == "procName")
				tempRow.push_back("String");
			else
				tempRow.push_back("Integer");
			return tempRow;
		}
	}
}

// New CS3202 Method. Check allowedAttrNames table
bool PreProcessor::isValidAttrRef(string designEntity, string pendingAttrName) {
	for(size_t i=0; i < allowedAttRefs.size(); i++) {
		if(allowedAttRefs[i][0] == designEntity) {
			for(size_t j=1; j < allowedAttRefs[i].size(); j++) {
				if(allowedAttRefs[i][j] == pendingAttrName)
					return true;
			}
		}
	}
	return false;
}

string PreProcessor::getQuotationRemoved(string inputString) {
	string noQuotation;
	if(inputString[0] != '"' || inputString[inputString.size()-1] != '"')
		return noQuotation;
	for(int i=1; i<inputString.length()-1; i++)
		noQuotation += inputString[i];
	return noQuotation;
}

bool PreProcessor::isValidInteger(string pendingInteger) {
	if(pendingInteger == "0") {
		return true;
	}
	else {
		int intValue = atoi(pendingInteger.c_str());
		if(intValue != 0)
			return true;
		}
	return false;
}

string PreProcessor::getBeforeDot(string toSplit) {
	string beforeDot = "";
	for(int i=0; toSplit[i] != '.'; i++) {
		beforeDot += toSplit[i];
	}
	return beforeDot;
}

string PreProcessor::getAfterDot(string toSplit) {
	int dotPosition = toSplit.find(".");
	string afterDot = toSplit.substr(dotPosition+1);
	return afterDot;	
}


vector<string> PreProcessor::splitByDot(string toSplit) {
	int i = 0;
	string beforeDot = "", afterDot = "";
	while(toSplit[i] != '.') {
		beforeDot += toSplit[i];
		i++;
	}
	i++;								// Skip past '.'
	while(i < toSplit.size()) {
		afterDot += toSplit[i];
		i++;
	}
	vector<string> splitted;
	splitted.push_back(beforeDot);
	splitted.push_back(afterDot);
	return splitted;
}

bool PreProcessor::isValidSynonymSyntax(string pendingSynonym) {
	if(isValidIdent(pendingSynonym))
		return true;
	else
		return false;
}

string PreProcessor::getSynonymTypeFromString(string pendingSynonym) {
	int index = getSynonymIndex(pendingSynonym);
	if(index == -1)
		return "-1";
	return getSynonymType(index);
}

// New method for CS3202
// Method to validate a pattern and it's arguments in the query
bool PreProcessor::isValidPattern(string pattern, string pattArg1, string pattArg2) {
	// Validate pattern
	string patternType;
	int pattTypeIndex = getSynonymIndex(pattern);
	if(pattTypeIndex!= -1) {
		patternType = declaredSynonyms[pattTypeIndex][0];
		if(patternType != "assign" && patternType != "while") {
			cout << "\tError: " << patternType << " is neither assign nor while\n";
			return false;
		}
	}
	else
		return false;
	// Validate pattArg1
	string pattArg1Type = getVarRefType(pattArg1);
	if(pattArg1Type == "-1") {
		cout << "\tError: " << pattArg1 << " is not a valid 1st argument for pattern\n";
		return false;
	}
	// Validate pattArg2
	string pattArg2Type;

	if(patternType == "while") {
		if(pattArg2 == "_")
			pattArg2Type = "_";
		else if(getSynonymTypeFromString(pattArg2) == "stmtLst") {
			pattArg2Type = "stmtLst";
		}
		else {
			cout << "\tError: " << pattArg2 << " is not a valid 2nd argument for while pattern\n";
			return false;
		}
	}

	else if(patternType == "assign") {
		if(pattArg2 == "_")
			pattArg2Type = "_";
		else {
			pattArg2Type = getExprSpecType(pattArg2);
			if(pattArg2Type == "-1") {
				cout << "\tError: " << pattArg2 << " is not a valid 2nd argument for assign pattern\n";
				return false;
			}
		}
	}

	/*if(pattArg2 != "_") {
		if(patternType == "while" && getSynonymType(getSynonymIndex(pattArg2)) != "stmtLst") {
			cout << "\tError: " << pattArg2 << " is not a valid 2nd argument for while pattern\n";
			return false;
		}
		else if(patternType == "while" && getSynonymType(getSynonymIndex(pattArg2)) == "stmtLst") {
			cout << "\tError: " << pattArg2 << " is not a valid 2nd argument for while pattern\n";
			return false;
		}
		else if(patternType == "assign") {
			pattArg2Type = getExprSpecType(pattArg2);
			if(pattArg2Type == "-1") {
				cout << "\tError: " << pattArg2 << " is not a valid 2nd argument for assign pattern\n";
				return false;
			}
		}
	}
	else
		pattArg2Type = "_";	*/
	
	// Passed all conditions

	// Correcting Format
	if(pattArg1Type == "Ident")
		pattArg1 = getQuotationRemoved(pattArg1);
	if(pattArg2Type == "Ident")
		pattArg2 = getQuotationRemoved(pattArg2);
	if(pattArg2Type == "_expr_")
		pattArg2 = getTrimmedExpression(pattArg2);
	if(pattArg2Type == "expr")
		pattArg2 = getQuotationRemoved(pattArg2);

	if(pattArg2Type == "Ident")
	cout << "\n" << pattArg2 << "\n";
	qt->insertPattern(pattern, patternType, pattArg1, pattArg1Type, pattArg2, pattArg2Type);
	return true;
}

string PreProcessor::getTrimmedExpression(string inputString) {
	string trimmed;
	/*if(inputString[0] != '_' || inputString[inputString.size()-1] != '_')
		return trimmed;*/
	for(int i=2; i<inputString.length()-2; i++)
		trimmed += inputString[i];
	return trimmed;
}

// Need change method
bool PreProcessor::isValidIfPattern(string pattern, string pattArg1, string pattArg2, string pattArg3) {
	// Is it an 'if' pattern?
	string patternType;
	int patternTypeIndex = getSynonymIndex(pattern);
	if(patternTypeIndex != -1) {
		patternType = declaredSynonyms[patternTypeIndex][0];
		if(patternType != "if") {
			cout << "\tError: " << pattern << " not an if pattern\n";
			return false;
		}
	}
	else 
		return false;
	// Validate pattArg1
	string pattArg1Type = getVarRefType(pattArg1);
	if(pattArg1Type == "-1") {
		cout << "\tError: " << pattArg1 << " in if pattern's arg1 is not a valid varRef\n";
		return false;
	}
	// Validate pattArg2
	string pattArg2Type;
	if(pattArg2 == "_")
		pattArg2Type = "_";
	else if(getSynonymTypeFromString(pattArg2) == "stmtLst") {
		pattArg2Type = "stmtLst";
	}
	else {
		cout << "\tError: " << pattArg2 << " in if pattern's arg2 is not '_' or stmtLst";
		return false;
	}
	// Validate pattArg3
	string pattArg3Type;
	if(pattArg3 == "_")
		pattArg3Type = "_";
	else if(getSynonymTypeFromString(pattArg2) == "stmtLst") {
		pattArg3Type = "stmtLst";
	}
	else {
		cout << "\tError: " << pattArg3 << " in if pattern's arg3 is not '_' or stmtLst";
		return false;
	}
	// Passed all conditions
	if(pattArg1Type == "Ident")
		pattArg1 = getQuotationRemoved(pattArg1);
	qt->insertIfPattern(pattern, patternType, pattArg1, pattArg1Type, pattArg2, pattArg2Type, pattArg3, pattArg3Type);
	return true;
}

string PreProcessor::getVarRefType(string pendingVarRef) {
	if(pendingVarRef == "_")
		return "_";
	if(getSynonymIndex(pendingVarRef) != -1) {
		int pendingVarRefIndex = getSynonymIndex(pendingVarRef);
		string pendingVarRefType = declaredSynonyms[pendingVarRefIndex][0];
		// Can it be other design entities besides "variable"?
		if(pendingVarRefType == "variable")
			return "variable";
	}				
	if(isValidIdent(getQuotationRemoved(pendingVarRef)))
		return "Ident";
	return "-1";
}

// Returns true if inputIdent is valid NAME: LETTER(LETTER|DIGIT)*
bool PreProcessor::isValidName(string inputName) {
	if(inputName.size()>=1 && isalpha(inputName[0])) {
		for(int i = 1; i< inputName.length(); i++)
			if(!isalnum(inputName[i])) {
				//cout << "\tError: " << inputName << " is not a valid Name\n";
				return false;
			}
	}
	else {
		//cout << "\tError: " << inputName << " is not a valid Name\n";
		return false;
	}
	//passes all cond
	return true;
}

// Returns true if inputIdent is valid IDENT: LETTER(LETTER|DIGIT|'#')*
bool PreProcessor::isValidIdent(string inputIdent) {
	bool validity = true;
	if(inputIdent.size()>=1 && isalpha(inputIdent[0]))
		for(int i = 1; i< inputIdent.length(); i++) {
			if(!isalnum(inputIdent[i]) && inputIdent[i] != '#')
				validity = false;
		}
	else 
		validity = false;
	return validity;
}

// New CS3202 method
// Returns true if inputIdent is valid Expression-Spec: "_" ""expr"" "_" | "expr"
string PreProcessor::getExprSpecType(string pendingExprSpec) {
	// is it a '_' ""expr"" '_' ?
	string expr = "";
	if(pendingExprSpec.size()>4 && pendingExprSpec[0] == '_' && pendingExprSpec[pendingExprSpec.length()-1] == '_' && pendingExprSpec[1] == '"' && pendingExprSpec[pendingExprSpec.length()-2] == '"') {
		for(int i=2; i< pendingExprSpec.length()-2; i++) {
			expr += pendingExprSpec[i];
		}
		if(isValidExpression(expr))
			return "_expr_";
	}
	// is it a ""expr"" ?
	if(pendingExprSpec.size()>2 && pendingExprSpec[0] == '"' && pendingExprSpec[pendingExprSpec.length()-1] == '"') {
		for(int i=1; i< pendingExprSpec.length()-1; i++) {
			expr += pendingExprSpec[i];
		}
		if(isValidExpression(expr))
			return "expr";
	}
	// Need check if an expression can be simply "-1"
	return "-1";	
}

bool PreProcessor::isValidExpression(string pendingExpression) {
	// Tokenize
	//vector<string> splitExpr;
	int bracketCount = 0;
	//bool withinBracket = false;
	string currToken, prevToken, prevTokenType, currTokenType;
	seperator = char_separator<char> (" ", "+-*()", boost::drop_empty_tokens);
	tokens = new tokenizer<char_separator<char>> (pendingExpression, seperator);
	tok_iter = tokens->begin();
	//cout << *(tokens->end)--;
	// First token check
	currToken = nextToken();
	if(currToken != "(" && !isValidFactor(currToken))
		return false;
	else if(currToken == "(")
		++bracketCount;
	prevToken = currToken;
	consumeToken();
	// Checking from 2nd token onwards till the end
	while(tok_iter != tokens->end()) {
		currToken = nextToken();
		currTokenType = getTokenType(currToken);
		prevTokenType = getTokenType(prevToken);
		
		if(prevTokenType == "-1")
			return false;
		else if(prevTokenType == "factor" && bracketCount == 0) {
			if(currTokenType != "operator")
				return false;
		}
		else if(prevTokenType == "factor" && bracketCount > 0) {
			if(currTokenType != "operator" && currToken != ")")
				return false;
		}
		else if(prevTokenType == "operator") {
			if(currTokenType != "factor" && currTokenType != "(")
				return false;
		}
		else if(prevTokenType == "(") {
			if(currTokenType != "factor" && currTokenType != "(")
				return false;
		}
		else if(prevTokenType == ")") {
			if(currTokenType != "operator" && currToken != ")")
				return false;
		}
		if(currToken == "(")
			++bracketCount;
		else if(currToken == ")")
			--bracketCount;
		
		prevToken = currToken;
		//cout << currToken << "\n";
		consumeToken();
	}
	if(bracketCount > 0)
		return false;
	if(currToken != ")" && !isValidFactor(currToken))
		return false;	// Last token is invalid
	return true;		// Passes all conditions
}

string PreProcessor::getTokenType(string pendingToken) {
	if(isValidFactor(pendingToken))
		return "factor";
	if(pendingToken == "+" || pendingToken == "-" || pendingToken == "*")
		return "operator";
	if(pendingToken == "(")
		return "(";
	if(pendingToken == ")")
		return ")";
	return "-1";
}

bool PreProcessor::isValidFactor(string pendingFactor) {
	/*if(pendingFactor.length() > 2 && pendingFactor[0] == '(' && pendingFactor[pendingFactor.length()-1] == ')') {
		string newExpression = pendingFactor.substr(1, pendingFactor.length()-2);
		return isValidExpression(newExpression);
	}*/
	if(isValidInteger(pendingFactor))
		return true;
	if(isValidName(pendingFactor))
		return true;
	return false;
}

//bool PreProcessor::isValidExpression(string pendingExpression) {
//	if(pendingExpression.length() >= 2 && pendingExpression[0] == '(' && pendingExpression[pendingExpression.length()-1] == ')')
//		return isValidFactor(pendingExpression);
//	unsigned lastPlusMinus = pendingExpression.find_last_of("+-");
//	if(lastPlusMinus == -1)
//		return isValidTerm(pendingExpression);
//	else {
//		string newExpression = pendingExpression.substr(0, lastPlusMinus);
//		string newTerm = pendingExpression.substr(lastPlusMinus+1);
//		return isValidTerm(newTerm) && isValidExpression(newExpression);
//	}
//}
//
//bool PreProcessor::isValidTerm(string pendingTerm) {
//	if(pendingTerm.length() >= 2 && pendingTerm[0] == '(' && pendingTerm[pendingTerm.length()-1] == ')')
//		return isValidFactor(pendingTerm);
//	unsigned lastTimes = pendingTerm.find_last_of("*");
//	if(lastTimes == -1)
//		return isValidFactor(pendingTerm);
//	else {
//		string newTerm = pendingTerm.substr(0, lastTimes);
//		string newFactor = pendingTerm.substr(lastTimes+1);
//		return isValidFactor(newFactor) && isValidTerm(newTerm);
//	}
//}
//
//bool PreProcessor::isValidFactor(string pendingFactor) {
//	if(pendingFactor.length() > 2 && pendingFactor[0] == '(' && pendingFactor[pendingFactor.length()-1] == ')') {
//		string newExpression = pendingFactor.substr(1, pendingFactor.length()-2);
//		return isValidExpression(newExpression);
//	}
//	if(isValidInteger(pendingFactor))
//		return true;
//	if(isValidName(pendingFactor))
//		return true;
//	return false;
//}

// Method to save your declared synonyms' variable names in a table
bool PreProcessor::saveSynonyms(std::string desEnt, vector<string> * entVarVect) {
	bool added = false;
	for(int i=0; i<declaredSynonyms.size(); i++) {
		if(declaredSynonyms[i][0] == desEnt) {
			for(int j=0; j<entVarVect->size(); j++) {
				if(getSynonymIndex(entVarVect->at(j)) != -1)
					return false;
				declaredSynonyms[i].push_back(entVarVect->at(j));
			}
			added = true;
		}
	}
	if(!added) {
		std::vector <std::string> tempRow;
		tempRow.push_back(desEnt);
		for(int i=0; i<entVarVect->size(); i++) {
			if(getSynonymIndex(entVarVect->at(i)) != -1) {
				return false;
			}
			tempRow.push_back(entVarVect->at(i));		
		}
		declaredSynonyms.push_back(tempRow);
	}
	delete entVarVect;
	return true;
}

// is the Design Entity used in the declarations among the list of allowed Design Entities ?
bool PreProcessor::isValidDesignEntity(std::string desEnt) {
	for(int i=0; i<allowedDesignEntities.size(); i++) {
		if(allowedDesignEntities[i] == desEnt)
			return true;
	}
	//cout << "\tError: Invalid Design Entity\n";
	return false;
}

string PreProcessor::getSynonymType(int index) {
	return declaredSynonyms[index][0];
}

// has the synonym that appears in a query been declared initially ? Returns index that refers to synonym's design entity
int PreProcessor::getSynonymIndex(std::string entVar) {
	for(int i=0; i<declaredSynonyms.size(); i++ ) {
		for(int j=1; j<declaredSynonyms[i].size(); j++ )
			if(declaredSynonyms[i][j] == entVar)
				return i;
	}
	return -1;
}

// Method to validate a relation and it's arguments in the query
bool PreProcessor::isValidRelation(std::string relation, std::string arg1, std::string arg2) {
	bool arg1Success = false;
	int arg1Index, arg2Index, j, arg1Value, arg2Value;
	std::string arg1Type, arg2Type;

	for(int i=0; i<allowedRelations.size(); i++) {
		//validate the relation itself
		if(allowedRelations[i][0] == relation) {
			//check if the first argument is a integer
			if(arg1 == "0")
				arg1Type = "Integer";
			else {
				arg1Value = atoi(arg1.c_str());
				if(arg1Value!=0)
					arg1Type = "Integer";
			}
			if(arg1Type != "Integer") {
				//check if the first argument a '_'
				if(arg1 == "_")
						arg1Type = "_";
				//check if first argument is a synonym
				else {
					arg1Index = getSynonymIndex(arg1);
					if(arg1Index != -1)
						arg1Type = declaredSynonyms[arg1Index][0];
					else {
						string inputIdent;
						for(int i=1; i<arg1.length()-1; i++)
							inputIdent += arg1[i];
						if(isValidIdent(inputIdent)) {
							arg1Type = "Ident";
							arg1 = inputIdent;
						}
					}
				}
			}
			for(j=2; allowedRelations[i][j] != ";"; j++)
				//validate if the type of the first argument corresponds with the allowed type in relation table
				if(arg1Type == allowedRelations[i][j])
					arg1Success = true;
			//no need to validate the second argument if the first one is false
			if(arg1Success) {
				//check if the second argument is a '_'
				if(arg2 == "_")
					arg2Type = "_";				
				//check if the second argument is a synonym				
				else if(getSynonymIndex(arg2) != -1) {
					arg2Index = getSynonymIndex(arg2);
					arg2Type = declaredSynonyms[arg2Index][0];
				}				
				else {
					//check if the second argument is an integer
					if(isValidInteger(arg2)) {
						arg2Type = "Integer";
					}
					//check if the second argument is an Ident
					else {
						string inputIdent;
						for(int i=1; i<arg2.length()-1; i++)
							inputIdent += arg2[i];
						if(isValidIdent(inputIdent)) {
							arg2Type = "Ident";
							arg2 = inputIdent;
						}
					}
				}
				//skip past the ";"
				j++;
				for(j; allowedRelations[i][j] != ";"; j++) {
					//validate the second argument
					if(arg2Type == allowedRelations[i][j]) {
						if(relation != "Next" && relation!= "Next*" && relation!= "NextBip" && relation!= "NextBip*") {
							if(arg1Type == "prog_line")
								arg1Type = "stmt";
							if(arg2Type == "prog_line")
								arg2Type = "stmt";
						}
						qt->insertSuchThat(relation, arg1, arg1Type, arg2, arg2Type);
						return true;
					}
				}
			}
		}
	}
	cout << "\tError: " << relation << "(" << arg1 << ", " << arg2 << ") is not a valid relation\n";
	return false;
}