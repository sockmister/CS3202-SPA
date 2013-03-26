#ifndef PQLController_h
#define PQLController_h

#include <string>
#include <iostream>
#include <vector>
#include <list>

#include "PreProcessor.h"
#include "queryTree.h"
#include "evaluator.h"
#include "PKB.h"
#include "DesignExtractor.h"

using namespace std;

class PQLController {
public:
	// Default constructor
	PQLController();

	// Constructor that passes a PKB
	PQLController(PKB * inputPKB);

	// Method called by testwrapper to pass in query
	void passInput(string input, list <string>  & resultsList);

private:
	PreProcessor preProcessor;
	PKB * pkb;
	evaluator eval;

	DesignExtractor* designExtractor;
};
#endif