//Design Extractor traverses AST and compute all the remaining design abstractions: Uses, Modifies for procedure and call statement; Next; Affects
#pragma once

#include "DesignExtractor.h"


	//Default Constructor
	DesignExtractor::DesignExtractor(){}

	//Constructor
	DesignExtractor::DesignExtractor(PKB* pkb):temp(){
		
		ast = pkb->getAST();
		follows = pkb->getFollows();
		parent = pkb->getParent();
		varTable = pkb->getVarTable();
		procTable = pkb->getProcTable();
		modifies = pkb->getModifies();
		uses = pkb->getUses();
		calls = pkb->getCalls();

		flag = false;
	}

	void DesignExtractor::buildCFG(STMT start, STMT end) {
	Graph.erase(Graph.begin(),Graph.end());
	Block.erase(Block.begin(),Block.end());
	bool then = false;
	bool once1 = false, once2 = true;
	bool WHILE = false;
	int blockNumber = 0;
	STMT lastThenStatement = -1;
	STMT lastElseStatement = -1;
	STMTLST ifStatements;
	STMTLST whileStatements;
	stack<int> thenStack;
	stack<int> elseStack;
	
	string ifNode = "ifNode"; 
	string whileNode = "whileNode";
	
	for(int i=start; i<=end; i++ ) {
		once2 = true;
		STMTLST child = parent->getChild(i);	
		int prevFollows = follows->getFollows(0,i);
		int parentStmt = parent->getParent(i);

		// no children, not a container statement
		if (child.size() == 0) {
			
			int after = follows->getFollows(1,i);

			// an assignment/call stmt that has a statement that follows it
			if ( after != -1 ) { 
				if (Block.size() <= blockNumber)
					Block.push_back(STMTLST());
				Block.at(blockNumber).push_back(i);
			}

			// an assignment/call stmt that does not have a statement that follows it
			else if ( after == -1  ) { 

				// check if the parent of current stmt is "ifStmt"
				if ( parentStmt != -1 && (find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) ) { 
					if (then) { // the "then" of "ifNode"

						if(Block.size() <= blockNumber)
							Block.push_back(STMTLST());
						Block.at(blockNumber).push_back(i);						
						
						blockNumber++;
						
						thenStack.push(i);
						
						then = false;
					}
					else {
						if(Block.size() <= blockNumber)
							Block.push_back(STMTLST());
						Block.at(blockNumber).push_back(i);							


						// link the "if" Block to the "else" block	
						Graph.at(getBlock(parentStmt)).push_back(getBlock(i));					
						
						blockNumber++;
						
						elseStack.push(i);
						
					}									   
					
				}
				
				/*
				*	Here, we backtrack to find where this statement connects to ( it is a child of a while or if stmt )
				*	
				*/
				if (prevFollows == -1 && parentStmt != -1) { 
						int parentSTMT = parentStmt;				
					if (find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) {// parent is "if" statement
						int parentFollows = follows->getFollows(1,parentStmt);
						
						// if the parent stmt does not follows any stmt, then get the parent of the parent stmt.
						// or the case when follows of i is (-1,-1) but is a child of a container statement
						if ( parentFollows == -1) {
							do {
								int parentStmt2 = parent->getParent(parentSTMT);
								if (find(whileStatements.begin(),whileStatements.end(), parentStmt2)!=whileStatements.end())
									break;
								if (parentStmt2 != -1 )
									parentFollows = follows->getFollows(1,parentStmt2);
								parentSTMT = parentStmt2;
							} while (parentFollows ==-1 && parentSTMT!=-1 );
							 
							if (parentFollows == -1) { // after the check, if parentFollows is still -1, then find a loop back to while
								
								parentSTMT = parentStmt;	
								do {
									int parentStmt2 = parent->getParent(parentSTMT);
									if ( parentStmt2 != -1 && (find(whileStatements.begin(),whileStatements.end(), parentStmt2)!=whileStatements.end()) ) {
										if(Graph.size() <= blockNumber)
											Graph.push_back(STMTLST());
										Graph.at(getBlock(i)).push_back(getBlock(parentStmt2));		
										WHILE = true;
										break;
									}

									parentSTMT = parentStmt2;
								} while (parentSTMT != -1);		

								parentSTMT = parentStmt;
								if (!WHILE) {
									do {
										int parentStmt2 = parent->getParent(parentSTMT);
										if ( parentStmt2 != -1 && (find(ifStatements.begin(),ifStatements.end(), parentStmt2)!=ifStatements.end()) ){
											if (Graph.size() <= blockNumber)
												Graph.push_back(STMTLST());
											Graph.at(getBlock(i)).push_back(-1);
										}

										parentSTMT = parentStmt2;
									} while (parentSTMT != -1);		
								}			
														
							}
						}
						
					}
				}

				// for last stmt in the while loop	
				if (parentStmt != -1 && (find(whileStatements.begin(),whileStatements.end(), parentStmt)!=whileStatements.end()) ) { 		

						if(Block.size() <= blockNumber)
								Block.push_back(STMTLST());
						if (getBlock(i) == -1)
							Block.at(blockNumber).push_back(i);
						
			
						if(Graph.size() <= blockNumber)
							for (int i=Graph.size();i<=blockNumber;i++)
								Graph.push_back(STMTLST());
						// link back to parent while stmt
						Graph.at(getBlock(i)).push_back(getBlock(parentStmt)); // link back to parent while stmt

						blockNumber++; 
				}
			
			}
		}
		
		/**
		* it is a while statement
		*/
		else if ( child.size()>0 && (ast->getNodeType(i))==whileNode ) { 
			whileStatements.push_back(i);

			// while node is a block of itself
			if (Block.size()==0) {
				
				Block.push_back(STMTLST());
				Block.at(blockNumber).push_back(i);

				Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1);
				blockNumber++;
				once1 = true;
			}
			else if (Block.size() > 0) {		
				
				if ((find(whileStatements.begin(),whileStatements.end(), i-1)!=whileStatements.end()) //while stmt just after a while stmt
							|| (find(whileStatements.begin(),whileStatements.end(), prevFollows)!=whileStatements.end())  // prevFollows is a while stmt
							|| (find(ifStatements.begin(),ifStatements.end(), i-1)!=ifStatements.end()) ) { // prev stmt is an if stmt
					Block.push_back(STMTLST());
					Block.at(blockNumber).push_back(i);

					if (Graph.size() <= blockNumber)
						for (int i=Graph.size();i<=blockNumber;i++)
							Graph.push_back(STMTLST());
					Graph.at(blockNumber).push_back(blockNumber+1);

					blockNumber++;
				} else if (find(ifStatements.begin(),ifStatements.end(), prevFollows)!=ifStatements.end()) { // prevFollows is an if stmt
						Block.push_back(STMTLST());
						Block.at(blockNumber).push_back(i);
					
						//check for last stmt in the 'then'

						if (thenStack.size()>0) {
								if (Graph.size() <= blockNumber)
									for (int i=Graph.size();i<=blockNumber;i++)
										Graph.push_back(STMTLST());
								int test = thenStack.top();
								
								STMTLST nextBlocks = nextBlock(1,getBlock(test));
								for (int i=0;i<nextBlocks.size();i++) {
									if (find(whileStatements.begin(),whileStatements.end(), Block[nextBlocks[i]].front())!=whileStatements.end())
										thenStack.pop();
								}
								Graph.at(getBlock(thenStack.top())).push_back(blockNumber); 
								thenStack.pop();
						}
					
						//check for last stmt in the 'else'

						if (elseStack.size()>0) {
								if (Graph.size() <= blockNumber)
									for (int i=Graph.size();i<=blockNumber;i++)
										Graph.push_back(STMTLST());
								int test = elseStack.top();
								
								STMTLST nextBlocks = nextBlock(1,getBlock(test));
								for (int i=0;i<nextBlocks.size();i++) {
									if (find(whileStatements.begin(),whileStatements.end(), Block[nextBlocks[i]].front())!=whileStatements.end())
										elseStack.pop();
								}
								Graph.at(getBlock(elseStack.top())).push_back(blockNumber);
								elseStack.pop();
						}

						if (Graph.size() <= blockNumber)
							for (int i=Graph.size();i<=blockNumber;i++)
								Graph.push_back(STMTLST());
						Graph.at(blockNumber).push_back(blockNumber+1);

						blockNumber++;
							
						once2 = false;
				}
				// while follows an assignment stmt
				else { 
					Block.push_back(STMTLST());
					Block.at(blockNumber+1).push_back(i); 
				
					if (Graph.size() <= blockNumber)
						for (int i=Graph.size();i<=blockNumber;i++)
							Graph.push_back(STMTLST());
					Graph.at(blockNumber).push_back(blockNumber+1);

			
					blockNumber++;

					Graph.push_back(STMTLST());
					Graph.at(blockNumber).push_back(blockNumber+1);

					blockNumber++;
				}

				// check if this while statement has a parent that is also a while statement
				int after = follows->getFollows(1,i);
				if ( after == -1 && parentStmt != -1  && (find(whileStatements.begin(),whileStatements.end(), parentStmt)!=whileStatements.end()) ) {
					Graph.at(getBlock(i)).push_back(getBlock(parentStmt));
				}

			}
			
		}

		/**
		* it is an if statement
		*/
		else if ( child.size()>0 && (ast->getNodeType(i))==ifNode ) { 
			ifStatements.push_back(i);

			if (Block.size() == 0) {
				Block.push_back(STMTLST());
			}
			
			if (Block.size() > 0) {		
				if(Block.size() <= blockNumber)
					Block.push_back(STMTLST());				
			}	
				
			// if statements will end the current block
				Block.at(blockNumber).push_back(i); 

				if (Graph.size() <= blockNumber)
					for (int i=Graph.size();i<=blockNumber;i++)
						Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1); 
				
				
				if ( parentStmt != -1 && then == false && (find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) ) {
					Graph.at(getBlock(parentStmt)).push_back(getBlock(i));
				}
				
				blockNumber++;
				then = true;
				
		}	

		/*
		* Here we link back stmt that follows after an 'if' stmt or 'while' stmt back to the 'if' or 'while' stmt
		*/

		//follows after an "If" statement
		if (prevFollows != -1 && (find(ifStatements.begin(),ifStatements.end(), prevFollows)!=ifStatements.end())) { 
			int blockNo = blockNumber;	
			if ( find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end() )
					blockNo-= 1;

				// link the 'then' node to the next stmt

						while (thenStack.size()>0) {
								if(Graph.size() <= blockNumber-1)
									Graph.push_back(STMTLST());
								int test = thenStack.top();
								
								STMTLST nextBlocks = nextBlock(1,getBlock(test));
								for (int i=0;i<nextBlocks.size();i++) {
									if (find(whileStatements.begin(),whileStatements.end(), Block[nextBlocks[i]].front())!=whileStatements.end())
										thenStack.pop();
								}
								if (thenStack.size()>0) {
									Graph.at(getBlock(thenStack.top())).push_back(blockNo);
									thenStack.pop();
								}
							}
					
			   // link the 'else' node to the next stmt
						while (elseStack.size()>0) {
								if(Graph.size() <= blockNumber-1)
									Graph.push_back(STMTLST());
								int test = elseStack.top();
								
								STMTLST nextBlocks = nextBlock(1,getBlock(test));
								for (int i=0;i<nextBlocks.size();i++) {
									if (find(whileStatements.begin(),whileStatements.end(), Block[nextBlocks[i]].front())!=whileStatements.end())
										elseStack.pop();
								}
								if (elseStack.size()>0) {
									Graph.at(getBlock(elseStack.top())).push_back(blockNo);
									elseStack.pop();
								}
							}
	
		}

		// follows after "while" statement
		if (prevFollows != -1 && (find(whileStatements.begin(),whileStatements.end(), prevFollows)!=whileStatements.end())) {  
				if (once2) {	
					if(Block.size() <= blockNumber)
						Block.push_back(STMTLST());
					if (getBlock(i) == -1)
						Block.at(blockNumber).push_back(i);	
					
						Graph.at(getBlock(prevFollows)).push_back(getBlock(i)); 
						once2 = false;
				}				
		}

		// while stmt that is in the 'else' 
		if (find(whileStatements.begin(),whileStatements.end(), i)!=whileStatements.end()) { 
				
				// last statement in an if statement
				if ((find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) && (follows->getFollows(1,i) == -1) ) { 
						if (then)
							thenStack.push(i);
						else
							elseStack.push(i);
				}
		}

		STMTLST nextBlocks = nextBlock(1,getBlock(i));

		
		if ( i ==  end && getBlock(i) == -1) {
			if(Block.size() <= blockNumber)
				Block.push_back(STMTLST());
			Block.at(blockNumber).push_back(i);	
		}


		// ending node	
		if ( i == end && (nextBlocks.size() == 0)) { 					
			if (find(ifStatements.begin(),ifStatements.end(), prevFollows)!=ifStatements.end()) {
				for (int i=0; i<Graph.size();i++) {
					for (int j=0;j<Graph.at(i).size(); j++) {
						if (Graph[i][j] == -1) {
							Graph.at(i).erase(Graph.at(i).begin(), Graph.at(i).end());
							Graph.at(i).push_back(blockNumber);
						}
					}
				}
			}
					Graph.push_back(STMTLST());
					Graph.at(getBlock(i)).push_back(-1);
					break;
			
		}

		// ending node is last statement in a while loop
		else if ( i == end && (nextBlocks.size() > 0)) { 
				if (find(whileStatements.begin(),whileStatements.end(), Block.at(nextBlocks.at(0)).at(0) )!=whileStatements.end()) {
					
					// check if it is the last while loop to exit or there is parent while loop
					if ( parentStmt != -1) { 

						int parentSTMT = parentStmt;
						
						do {
							int parentStmt2 = parent->getParent(parentSTMT);
							if ( parentStmt2 == -1 && (find(whileStatements.begin(),whileStatements.end(), parentSTMT)!=whileStatements.end()) ) {
								Graph.at(getBlock(parentSTMT)).push_back(-1);	
								if (elseStack.size()>0) {
									if (elseStack.top() == Block.at(nextBlocks.at(0)).at(0))
										Graph.at(nextBlocks.at(0)).push_back(getBlock(parentSTMT));
								}
								break;
							}

							parentSTMT = parentStmt2;
						} while (parentSTMT != -1);
						break;
					}
					else {
						Graph.at(nextBlocks.at(0)).push_back(-1);
						break;
					}
				}
		}
		
	}
		

}


	//Build CFG for each procedure
	void DesignExtractor::computeCFG(){
	
		int numOfProc = procTable->getNoOfProc();
		//CFG cfg = CFG(ast, follows, parent);

		for(int i=0; i<numOfProc; i++){
			buildCFG(procTable->getFirstStmt(procTable->getProcName(i)), procTable->getLastStmt(procTable->getProcName(i)));
			CFG * cfg = new CFG(Graph,Block);
			procTable->insertCFG(procTable->getProcName(i), cfg);
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

		PROCLIST* p = procTable->getExtra();

		p->push_back("Dummy"); //dummy first entry (no stmt number 0)

		for(int i = 0; i<procTable->getNoOfProc(); i++)
		{
			PROCNAME proc = procTable->getProcName(i);
			for(int j = procTable->getFirstStmt(proc); j <= procTable->getLastStmt(proc); j++)
			{
				p->push_back(proc);
			}
		}
	}

	// private method
STMT DesignExtractor::getBlock(STMT statementNumber) {
	for (size_t i=0; i<Block.size();i++) {
		for (size_t j=0;j<Block.at(i).size(); j++) {
			if (Block[i][j] == statementNumber)
				return i;
		}
	}
	
	return -1;
}

// private method
STMTLST DesignExtractor::nextBlock(ORDER order, STMT blockNumber) {
	STMTLST nextBlock;
	
	if (blockNumber >= Block.size()) {
		return nextBlock;
	}

	
	if ( (blockNumber < Block.size()) && (order==1) && (Graph.size()>blockNumber) ) {
		nextBlock = Graph.at(blockNumber);
		return nextBlock;
	}

	if((blockNumber < Block.size()) && (order==0)) {
		for (size_t i=0; i<Graph.size();i++) {
			for (size_t j=0;j<Graph.at(i).size(); j++) {
				if (Graph[i][j] == blockNumber)
					nextBlock.push_back(i);
			}
		}
		return nextBlock;
	}

	return nextBlock;
}

