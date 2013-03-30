#include "TestWrapper.h"


// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
	VarTable * varTable = new VarTable();
	ProcTable * procTable = new ProcTable(varTable);
	AST * ast = new AST();
	Uses * uses = new Uses(varTable);
	Modifies * mod = new Modifies(varTable);
	pkb = new PKB(ast, varTable, new Follows(), new Parent(), mod, uses, procTable, new Calls(procTable), new Affects(ast, mod, uses, varTable, procTable));
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
  // ...rest of your code...
	ifstream * file = new ifstream(filename);
	SimpleParser * parser = new SimpleParser(file, pkb);

	parser->run();

	DesignExtractor * designExtractor = new DesignExtractor(pkb);
	designExtractor->computeCalls();
	designExtractor->computeModifiesAndUses();
	designExtractor->computeCFG();
	designExtractor->fillExtra();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
  // ...code to evaluate query...
	PQLController pqlC(pkb);
	for(std::list<std::string>::iterator it = results.begin(); it != results.end(); it++){
		//cout << *it <<endl;
	}
	
	pqlC.passInput(query, results);

  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
