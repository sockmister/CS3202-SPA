#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
#include "spa source/SimpleParser.h"
#include "spa source/PKB.h"
#include "spa source/PQLController.h"

class TestWrapper : public AbstractWrapper {
 public:
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);

private:
	AST * ast;
	VarTable * varTable;
	Follows * follows;
	Parent * parent;
	Uses * uses;
	Modifies * modifies;
	PKB * pkb;
};

#endif
