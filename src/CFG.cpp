#include "CFG.h"
using namespace std;

CFG::CFG(){
}

CFG::~CFG(){
	Graph.erase(Graph.begin(),Graph.end());
	Block.erase(Block.begin(), Block.end());
}

CFG::CFG(vector<STMTLST> CFG, vector<STMTLST> Blocks){
	this->Graph = CFG;
	this->Block = Blocks;
	
	for (int i=0; i<Graph.size(); i++)
		visited.push_back(false);
}

/*
void CFG::buildCFG(STMT start, STMT end) {
	
	bool then = false;
	bool once = false;
	bool WHILE = false;
	int blockNumber = 0;
	STMT lastThenStatement = -1;
	STMT lastElseStatement = -1;
	STMTLST ifStatements;
	STMTLST whileStatements;
	
	string ifNode = "ifNode";
	string whileNode = "whileNode";
	
	for(int i=start; i<=end; i++ ) {
		STMTLST child = parent->getChild(i);	
		int prevFollows = follows->getFollows(0,i);
		int parentStmt = parent->getParent(i);

		if (child.size() == 0) {// no children, not a container statement
			
			int after = follows->getFollows(1,i);
			if ( after != -1 ) { // has a statement that follows it
				if (Block.size() <= blockNumber)
					Block.push_back(STMTLST());
				Block.at(blockNumber).push_back(i);
			}
			
			else if ( after == -1  ) { // does not have a statement that follows it

						
				
				if (Graph.size() <= blockNumber) {
					if (prevFollows != -1 && (find(ifStatements.begin(),ifStatements.end(), prevFollows)!=ifStatements.end())) { // follows after an "If" statement
							
							cout<<"Block Number= "<<blockNumber<<endl;
							if (lastThenStatement != -1) {
								if(Graph.size() <= blockNumber-1)
									Graph.push_back(STMTLST());
								Graph.at(getBlock(lastThenStatement)).push_back(blockNumber);
							}
						
							if (lastElseStatement != -1) {
								if(Graph.size() <= blockNumber-1)
									Graph.push_back(STMTLST());
								Graph.at(getBlock(lastElseStatement)).push_back(blockNumber);
							}

							lastThenStatement = lastElseStatement = -1;
					}

					if (prevFollows != -1 && (find(whileStatements.begin(),whileStatements.end(), prevFollows)!=whileStatements.end())) {  // follows after "while" statement
							
						Graph.at(getBlock(prevFollows)).push_back(getBlock(i)); 				
					}
				
				}
				if ( parentStmt != -1 && (find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) ) { // check if the parent of current stmt is "ifStmt"
					if (then) { // the "then" of "ifNode"

						if(Block.size() <= blockNumber)
							Block.push_back(STMTLST());
						Block.at(blockNumber).push_back(i);						
						
						blockNumber++;

						lastThenStatement = i;
						then = false;
					}
					else {
						if(Block.size() <= blockNumber)
							Block.push_back(STMTLST());
						Block.at(blockNumber).push_back(i);							

						Graph.at(getBlock(parentStmt)).push_back(getBlock(i));	// link the "if" Block to the "else" block					
						
						blockNumber++;
						
						lastElseStatement = i;
					}									   
					
				}
				
				if (prevFollows == -1 && parentStmt != -1) { // if the parent stmt does not follows any stmt, then get the parent of the parent stmt.
						// or the case when follows of i is (-1,-1) but is a child of a container statement
					if (find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) {// parent is "if" statement
						
						int parentSTMT = parentStmt;
						
						do {
							int parentStmt2 = parent->getParent(parentSTMT);
							if ( parentStmt2 != -1 && (find(whileStatements.begin(),whileStatements.end(), parentStmt2)!=whileStatements.end()) ) {
								if(Graph.size() <= blockNumber)
									Graph.push_back(STMTLST());
								Graph.at(getBlock(i)).push_back(getBlock(parentStmt2));		
								WHILE = true;
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

				if (parentStmt != -1 && (find(whileStatements.begin(),whileStatements.end(), parentStmt)!=whileStatements.end()) ) { // for last stmt in the while loop			

						if(Block.size() <= blockNumber)
								Block.push_back(STMTLST());
						if (getBlock(i) == -1)
							Block.at(blockNumber).push_back(i);
						
						blockNumber++;
			
						if(Graph.size() <= blockNumber-1)
								Graph.push_back(STMTLST());
						Graph.at(getBlock(i)).push_back(getBlock(parentStmt)); // link back to parent while stmt
				}
			
			}
		}
		
		
		
		else if ( child.size()>0 && (ast->getNodeType(i))==whileNode ) { // it is a while statement
			whileStatements.push_back(i);
			
			// while node is a block of itself
			if (Block.size()==0) {
				
				Block.push_back(STMTLST());
				Block.at(blockNumber).push_back(i);

				Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1);
				blockNumber++;
				once = true;
			}
			else if (Block.size() > 0) {				
				if (once) {
					blockNumber--;
					once = false;
				}
				
				Block.push_back(STMTLST());
				Block.at(blockNumber+1).push_back(i); 
				
				if(Graph.size() <= blockNumber)
					Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1);

			
				blockNumber++;

				Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1);

				blockNumber++;
				// check if this while statement has a parent that is also a while statement
				if ( parentStmt != -1  && (find(whileStatements.begin(),whileStatements.end(), parentStmt)!=whileStatements.end()) ) {
					Graph.at(getBlock(i)).push_back(getBlock(parentStmt));
				}

			}
			
		}

		
		
		else if ( child.size()>0 && (ast->getNodeType(i))==ifNode ) { // it is an if statement
			ifStatements.push_back(i);

			if (Block.size() == 0) {
				Block.push_back(STMTLST());
			}
			
			if (Block.size() > 0) {		
				if(Block.size() <= blockNumber)
					Block.push_back(STMTLST());				
			}	
				
				Block.at(blockNumber).push_back(i); // if statements will end the current block

				if (Graph.size() <= blockNumber)
					Graph.push_back(STMTLST());
				Graph.at(blockNumber).push_back(blockNumber+1); 
				
				
				if ( parentStmt != -1 && then == false && (find(ifStatements.begin(),ifStatements.end(), parentStmt)!=ifStatements.end()) ) {
					Graph.at(getBlock(parentStmt)).push_back(getBlock(i));
				}
				
				blockNumber++;
				then = true;
				
		}	

		if (prevFollows != -1 && (find(ifStatements.begin(),ifStatements.end(), prevFollows)!=ifStatements.end())) { // follows after an "If" statement
					
						if (lastThenStatement != -1) {
							if(Graph.size() <= blockNumber-1)
								Graph.push_back(STMTLST());
							Graph.at(getBlock(lastThenStatement)).push_back(getBlock(i));
						}
						
						if (lastElseStatement != -1) {
							if(Graph.size() <= blockNumber-1)
								Graph.push_back(STMTLST());
							Graph.at(getBlock(lastElseStatement)).push_back(getBlock(i));
						}

						lastThenStatement = lastElseStatement = -1;				
		}

		if (prevFollows != -1 && (find(whileStatements.begin(),whileStatements.end(), prevFollows)!=whileStatements.end())) {  // follows after "while" statement
					
					if(Block.size() <= blockNumber)
						Block.push_back(STMTLST());
					if (getBlock(i) == -1)
						Block.at(blockNumber).push_back(i);	
					
					
						Graph.at(getBlock(prevFollows)).push_back(getBlock(i)); 				
		}


		STMTLST nextBlocks = nextBlock(1,getBlock(i));

		if ( i ==  end && getBlock(i) == -1) {
			if(Block.size() <= blockNumber)
				Block.push_back(STMTLST());
			Block.at(blockNumber).push_back(i);	
		}

		if ( i == end && (nextBlocks.size() == 0)) { // ending node						
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
		else if ( i == end && (nextBlocks.size() > 0)) { // ending node is last statement in a while loop
				if (find(whileStatements.begin(),whileStatements.end(), Block.at(nextBlocks.at(0)).at(0) )!=whileStatements.end()) {
					
					if ( parentStmt != -1) { // check if it is the last while loop to exit or there is parent while loop
						int parentSTMT = parentStmt;
						
						do {
							int parentStmt2 = parent->getParent(parentSTMT);
							if ( parentStmt2 == -1 && (find(whileStatements.begin(),whileStatements.end(), parentSTMT)!=whileStatements.end()) ) {
								Graph.at(getBlock(parentSTMT)).push_back(-1);		
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
		
	for (int k=0; k<Graph.size(); k++)
		visited.push_back(false);

}
*/

STMTLST CFG::nextStatement(int order, STMT programLine) {
	STMTLST next;
	int block = getBlock(programLine);
	STMTLST::iterator it;

	
	if (block == -1)
		return next;
	
	if (block != -1 && (order ==1)) {
		STMTLST temp = Block.at(block);	
	
		if (temp.back() != programLine) {	// to deal with next statement in the same block
			it = find(temp.begin(),temp.end(), programLine);
			++it;
			next.push_back(*it);
			return next;
		}
		
		else if (temp.back() == programLine) { // to deal with next statement another block
			STMTLST after = nextBlock(1,block);
			
			if (after.front() == -1) {
				next.push_back(-1);
				return next;
			}
			
			if (after.size() == 1) {
				next.push_back(Block.at(after.front()).front());
				return next;
			}

			else if (after.size() > 1) {
				next.push_back(Block.at(after.front()).front());
				it = after.begin();
				++it;
				if ( *it == -1)
					next.push_back(-1);
				else
					next.push_back(Block.at(*it).front());
				return next;
			}
		}
		return next;
	}


	
	if(block != -1 && (order==0)) {
		
		STMTLST temp = Block.at(block);	
		if (temp.size()>1 && temp.front() != programLine) { // to deal with next statement in the same block
			it = find(temp.begin(),temp.end(), programLine);
			it--;
			next.push_back(*it);
			return next;
		}
		else if (temp.size()>0 && temp.front() ==  programLine) { // to deal with next statement another block
			if (block == 0)
				return next;
			
			if (block != 0) {
				STMTLST before = nextBlock(0,block);
				
				next.push_back(Block.at(before.front()).back());
				if (before.size()>1) {
					it = before.begin();
					++it;
					next.push_back(Block.at(*it).back());
				}
				return next;
			}		
		}
		return next;
	}
	return next;
}

STMTLST CFG::nextStatementStar(int order, STMT programLine) {
	STMTLST nextStar;
	fill(visited.begin(),visited.end(), false);
	STMTLST::iterator it;

	int block = getBlock(programLine);
	if(block == -1) {
		return nextStar;
	}

	// find program lines that appear after programLine 
	if(block != -1 && (order==1)) {

		int index = -1;
		STMTLST BLOCK = Block.at(block);
		for (size_t a=0;a<BLOCK.size();a++) {
			if (BLOCK.at(a) == programLine) {
				index = a;
				break;
			}
		}

		for (size_t i=index+1;i<BLOCK.size();i++) // skip the current programLine
			nextStar.push_back(BLOCK.at(i));

		skip = true;
		vector<bool> answer = DFS(block); // run DFS for reachability
		
		for (size_t j=0;j<answer.size();j++) {
			if (answer.at(j) ==  true)
				for (size_t k=0; k<Block.at(j).size(); k++) 
					nextStar.push_back(Block[j][k]);
		}

		
		return nextStar;
	}

	//find program lines that precedes programLine
	if(block != -1 && (order==0)) {
		bool add = false;
		int index = -1;

		STMTLST BLOCK = Block.at(block);
		for (size_t a=0;a<BLOCK.size();a++) {
			if (BLOCK.at(a) == programLine) {
				index = a;
				break;
			}
		}

		for (size_t i=0;i<Graph.size(); i++) { // DFS on every block 
			fill(visited.begin(),visited.end(), false);
			
			skip = true;
			vector<bool> answer = DFS(i);

			if ( i == block && answer.at(block) == false)
				add = true;

			if (answer.at(block) == true) {  // if it visits programLine (there is a control flow path),
				for(size_t j=0;j<Block.at(i).size(); j++)
					nextStar.push_back(Block[i][j]);		 // then add it to the nextStar array	
			}				
		}

		if (add) {
			for (size_t i=0;i<index;i++)		// in the same block, add what comes before programLine
				nextStar.push_back(BLOCK.at(i));
		}

		return nextStar;
	}

	return nextStar;

}



bool CFG::isNext(STMT n1, STMT n2) {
	int block1 = getBlock(n1);
	int block2 = getBlock(n2);
	
	if ( block1 == -1 || block2 == -1)
		return false;

	else {
		STMTLST next = nextStatement(1,n1);

		for (size_t i=0; i<next.size(); i++) {
			if ( next.at(i) == n2) 
				return true;
		}
	}
	
	return false;
}

bool CFG::isNextStar(STMT n1, STMT n2) {
	int block1 = getBlock(n1);
	int block2 = getBlock(n2);

	if ( block1 == -1 || block2 == -1)
		return false;

	else {

			STMTLST next = nextStatementStar(1,n1);

			for (size_t i=0; i<next.size(); i++) {
				if ( next[i] == n2 )
					return true;
			}
	}
	return false;
}

STMTLST CFG::affectsNext(STMT n1, STMT n2) {
	this->affectsNextDFS.clear();
	STMTLST affectsNextAnswer;
	fill(visited.begin(),visited.end(), false);

	int startBlock = getBlock(n1);
	int endBlock = getBlock(n2);
		
	if ( startBlock == -1 || endBlock == -1)
		return affectsNextAnswer;

		int index = -1;
		STMTLST BLOCK = Block.at(startBlock);
		for (size_t a=0;a<BLOCK.size();a++) {
			if (BLOCK.at(a) == n1) {
				index = a;
				break;
			}
		}
		
		for (size_t i=index+1;i<BLOCK.size();i++) // skip the current programLine
			affectsNextDFS.push_back(BLOCK.at(i));
		
		skip = true;
		visitStartBlock = false;
		visitEndBlock = false;
		originalStartBlock = startBlock;
		affectsDFS(startBlock,endBlock); // run DFS for reachability
		
		
		for (int j=0;j<affectsNextDFS.size();j++) {
			int index = affectsNextDFS[j];
			for (int k=0; k<Block[index].size(); k++) 
					affectsNextAnswer.push_back(Block[index][k]);
		}
		
		if (visitEndBlock == true) {
			for (int i=0;i<Block[endBlock].size();i++) {
				if (Block[endBlock][i] != n2)
					affectsNextAnswer.push_back(Block[endBlock][i]);
				else
					break;
			}
		}

		if (visitStartBlock == true) {
			for (int i=0;i<Block[startBlock].size();i++) {
				if (Block[startBlock][i] != n1)
					affectsNextAnswer.push_back(Block[startBlock][i]);
				else
					break;
			}
		}
		
		return affectsNextAnswer;

}

// DFS
vector<bool> CFG::DFS(STMT blockNumber) {
	
	if (skip) {
		visited[blockNumber] = false;
		skip = false;
	}
	else if (skip == false)	
		visited[blockNumber] = true;

	STMTLST next = nextBlock(1, blockNumber);
	for (size_t j=0;j<next.size();j++) {
		int n = next[j];
		if (n == -1)
			break;
		if (n!=-1 && visited[n] == false)
			DFS(n);
	}

	return visited;
}

// DFS for affects
void CFG::affectsDFS(int startBlock, int endBlock) {
	
	if (skip) {
		visited[startBlock] = false;
		skip = false;
	}
	else if (skip == false)	
		visited[startBlock] = true;

	STMTLST next = nextBlock(1, startBlock);
	for (size_t j=0;j<next.size();j++) {
		int n = next[j];

		if (n == originalStartBlock) {
			visited[startBlock] = true;
			visitStartBlock = true;
			return;
		}

		if (n == endBlock)
			visitEndBlock = true;
		

		if ( n==-1 )
			break;

		if (n!=endBlock  && n!=startBlock && visited[n] == false) {
			this->affectsNextDFS.push_back(n);
			affectsDFS(n,endBlock);
		}
	}
}

// For testing
vector<STMTLST> CFG::getCFG() {
	return Graph;
}

// For testing
vector<STMTLST> CFG::getBlock() {
	return Block;
}

// private method
STMT CFG::getBlock(STMT statementNumber) {
	for (size_t i=0; i<Block.size();i++) {
		for (size_t j=0;j<Block.at(i).size(); j++) {
			if (Block[i][j] == statementNumber)
				return i;
		}
	}
	
	return -1;
}

// private method
STMTLST CFG::nextBlock(int order, STMT blockNumber) {
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