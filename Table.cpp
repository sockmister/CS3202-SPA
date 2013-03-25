#include "Table.h"
#include <iostream>
#include <set>

using namespace std;


Table::Table(){
	caseNo = APPEND;
}

Table::~Table(){
}

Table::Table(vector<vector<VALUE>> tabIn, unordered_map<COLNAME, int> colIn){
	caseNo = APPEND;
	table = tabIn;
	colIndex = colIn;
}

//public methods
bool Table::hasColumns(COLNAME col){
	return (colIndex.count(col))== 0 ? false : true;
}

vector<VALUE> Table::getColumn(COLNAME col){
	vector<VALUE> column;
	if(colIndex.begin() == colIndex.end())
		return column;

	//get index
	int index = (colIndex.find(col))->second;

	//get 1 by 1
	for(vector<vector<VALUE>>::iterator it = table.begin(); it != table.end(); ++it){
		vector<VALUE>::iterator tuple_it = it->begin();
		column.push_back(*(tuple_it+index));
	}
	//return

	//sort and remove duplicates
	set<VALUE> s;
	unsigned size = column.size();
	for(unsigned i = 0; i < size; ++i) s.insert(column[i]);
	column.assign(s.begin(), s.end());

	return column;
}

void Table::insert(COLNAME firstCol, VALUE first, COLNAME secondCol, vector<VALUE> secondValues){
	//special case of "-" insert
	if(firstCol == "_" && secondCol == "_")
		return;
	else if(firstCol == "_"){
		insert(secondCol, secondValues);
		return;
	}
	else if(secondCol == "_"){
		vector<VALUE> value;
		value.push_back(first);
		insert(firstCol, value);
		return;
	}

	if(caseNo == APPEND){
		colIndex.insert(std::pair<COLNAME, int>(firstCol, colIndex.size()));
		colIndex.insert(std::pair<COLNAME, int>(secondCol, colIndex.size()));
		append(firstCol, first, secondCol, secondValues);
		return;
	}
	if(caseNo == NO_DS){
		//assign case number
		bool haveFirst = hasColumns(firstCol);
		bool haveSec = hasColumns(secondCol);

		if(haveFirst && haveSec){
			caseNo = DUAL_COL;
		}
		else if(haveFirst){
			caseNo = MATCH_FIRST;
			colIndex.insert(std::pair<COLNAME, int>(secondCol, colIndex.size()));
		}
		else if(haveSec){
			caseNo = MATCH_SECOND;
			colIndex.insert(std::pair<COLNAME, int>(firstCol, colIndex.size()));
		}
		else{
			caseNo = CART_PRO;
			colIndex.insert(std::pair<COLNAME, int>(firstCol, colIndex.size()));
			colIndex.insert(std::pair<COLNAME, int>(secondCol, colIndex.size()));
		}	
		currFirst = firstCol;
		currSecond = secondCol;
	}

	switch(caseNo){
	case MATCH_FIRST:
		match_first_DS.insert(unordered_map<VALUE, vector<VALUE>>::value_type(first, secondValues));
		break;
	case MATCH_SECOND:
		for(vector<VALUE>::iterator it = secondValues.begin(); it != secondValues.end(); ++it){
			match_second_DS.insert(pair<VALUE, VALUE>(*it, first));
		}
		break;
	case DUAL_COL:
		for(vector<VALUE>::iterator it = secondValues.begin(); it != secondValues.end(); ++it){
			string toInsert = first + *it;
			dual_col_DS.insert(toInsert);
		}
		break;
	case CART_PRO:
		for(vector<VALUE>::iterator it = secondValues.begin(); it != secondValues.end(); ++it){
			vector<VALUE> pair;
			pair.push_back(first);
			pair.push_back(*it);
			cart_pro_DS.push_back(pair);
		}
		break;
	}
}

void Table::insert(COLNAME col, vector<VALUE> values){
	//special case of "-"
	if(col == "_")
		return;

	if(values.size() == 0)
		return;
	//what about existing column vs no existing column
	if(caseNo == APPEND){
		colIndex.insert(std::pair<COLNAME, int>(col, 0));
		append(col, values);
		return;
	}
	if(caseNo == NO_DS){
		caseNo = SINGLE_COL;
		//colIndex.insert(std::pair<COLNAME, int>(col, colIndex.size()));
		currFirst = col;
	}
	for(vector<VALUE>::iterator it = values.begin(); it != values.end(); ++it){
		single_col_DS.insert(*it);
	}
}

void Table::shrinkTable(){
	//actual merging
	switch(caseNo){
	case MATCH_FIRST:
		mergeMatchFirst();
		match_first_DS.clear();
		break;
	case MATCH_SECOND:
		mergeMatchSecond();
		match_second_DS.clear();
		break;
	case DUAL_COL:
		mergeDualCol();
		dual_col_DS.clear();
		break;
	case CART_PRO:
		mergeCartPro();
		cart_pro_DS.clear();
		break;
	case SINGLE_COL:
		mergeSingleCol();
		single_col_DS.clear();
		break;
	case APPEND:
		//this check if append has been used before by the clause. If no, we should still keep append for next clause.
		if(table.size() == 0) return;
		break;
	}

	//finally change case number back to -1
	caseNo = NO_DS;
}

void Table::print(){
	//print column headings
	for(unordered_map<VALUE, int>::iterator col_it = colIndex.begin(); col_it != colIndex.end(); ++col_it){
		cout << col_it->first << "\t";
	}

	cout << endl;

	for(vector<vector<VALUE>>::iterator t_it = table.begin(); t_it != table.end(); ++t_it){
		for(vector<VALUE>::iterator v_it = t_it->begin(); v_it != t_it->end(); ++v_it){
			cout << *v_it << "\t";
		}
		cout << endl;
	}

	cout << "-----END OF TABLE------\n" << endl;
}

//precondition: all columns exist in table
vector<string> Table::getTuple(vector<COLNAME> columns){
	vector<string> results;
	//get columns
	vector<vector<VALUE>> tupleTable;
	for(int i = 0; i < columns.size(); ++i){
		int index = (colIndex.find(columns.at(i)))->second;
		vector<VALUE> currCol;

		//get 1 by 1
		for(vector<vector<VALUE>>::iterator it = table.begin(); it != table.end(); ++it){
			vector<VALUE>::iterator tuple_it = it->begin();
			currCol.push_back(*(tuple_it+index));
		}
		tupleTable.push_back(currCol);
	}

	//generate results
	for(int currRow = 0; currRow < tupleTable.at(0).size(); ++currRow){
		string tuple = tupleTable.at(0).at(currRow);
		for(int currCol = 1; currCol < tupleTable.size(); ++currCol){
			tuple = tuple + " " + tupleTable.at(currCol).at(currRow);
		}
		results.push_back(tuple);
	}

	set<string> s;
	for(int i = 0; i < results.size(); ++i){
		s.insert(results.at(i));
	}

	results.assign(s.begin(), s.end());

	return results;
	/*
	vector<vector<VALUE>> * sets = new vector<vector<VALUE>>();
	for(int i = 0; i < columns.size(); ++i){
		vector<VALUE> col = getColumn(columns.at(i));
		sets->push_back(col);
	}

	vector<VALUE> * curr = new vector<VALUE>(sets->size());
	vector<string> * results = new vector<string>();
	cartProduct(sets, curr, 0, results);
	
	delete sets;

	return *results;
	*/
}

//helper for cartesian product
void Table::cartProduct(vector<vector<VALUE>>* sets, vector<VALUE>* curr, int k, vector<string> * results){
	if(k == sets->size()){
		string tuple = "";
		if(k > 0)
			tuple = curr->at(0);
		for(int i = 1; i < k; ++i){
			tuple = tuple + " " + curr->at(i);
		}
		results->push_back(tuple);
	}
	else{
		for(int j = 0; j < sets->at(k).size(); j++){
			vector<VALUE>::iterator it = curr->begin();
			*(it+k) = sets->at(k)[j];
			cartProduct(sets, curr, k+1, results);
		}
	}
}

//helper methods
void Table::mergeMatchFirst(){
	int matchIndex = colIndex.find(currFirst)->second;
	vector<vector<VALUE>> newTuples;
	//for each tuple
	for(vector<vector<VALUE>>::iterator tuple = table.begin(); tuple != table.end(); ++tuple){
		VALUE toMatch = *((tuple->begin()) + matchIndex);
		unordered_map<VALUE, vector<VALUE>>::iterator currIt = match_first_DS.find(toMatch);

		if(currIt != match_first_DS.end()){    //we found a match
			vector<VALUE> toInsert = currIt->second;
			//start inserting
			for(vector<VALUE>::iterator value_it = toInsert.begin(); value_it != toInsert.end(); ++value_it){
				vector<VALUE> newTuple(*tuple);
				newTuple.push_back(*value_it);
				newTuples.push_back(newTuple);
			}
		}
	}

	table = newTuples;
}

void Table::mergeMatchSecond(){
	int matchIndex = colIndex.find(currSecond)->second;
	vector<vector<VALUE>> newTuples;
	//for each current tuple
	for(vector<vector<VALUE>>::iterator tuple = table.begin(); tuple != table.end(); ++tuple){
		VALUE toMatch = *((tuple->begin()) + matchIndex);
		pair<unordered_multimap<VALUE, VALUE>::iterator, 
			unordered_multimap<VALUE, VALUE>::iterator> range = match_second_DS.equal_range(toMatch);

		for(unordered_multimap<VALUE, VALUE>::iterator map_it = range.first; map_it != range.second; ++map_it){
			vector<VALUE> newTuple(*tuple);
			newTuple.push_back(map_it->second);
			newTuples.push_back(newTuple);
		}
	}

	table = newTuples;
}

void Table::mergeDualCol(){
	//go thru each tuple in existing table
	int matchIndexFirst = colIndex.find(currFirst)->second;
	int matchIndexSecond = colIndex.find(currSecond)->second;
	vector<vector<VALUE>> newTuples;

	for(vector<vector<VALUE>>::iterator tuple = table.begin(); tuple != table.end(); ++tuple){
		VALUE toMatchFirst = *((tuple->begin()) + matchIndexFirst);
		VALUE toMatchSecond = *((tuple->begin()) + matchIndexSecond);
		string finalToMatch = toMatchFirst + toMatchSecond;

		if(dual_col_DS.count(finalToMatch) > 0){    //there is  match
			vector<VALUE> newTuple(*tuple);
			newTuples.push_back(newTuple);
		}
	}

	table = newTuples;
}

void Table::mergeCartPro(){
	vector<vector<VALUE>> newTuples;
	
	for(vector<vector<VALUE>>::iterator tuple = table.begin(); tuple != table.end(); ++tuple){
		for(vector<vector<VALUE>>::iterator curr_it = cart_pro_DS.begin(); curr_it != cart_pro_DS.end(); ++curr_it){
			vector<VALUE> newTuple(*tuple);
			vector<VALUE> pair = *curr_it;
			newTuple.push_back(pair[0]);
			newTuple.push_back(pair[1]);
			newTuples.push_back(newTuple);
		}
	}

	table = newTuples;
}

void Table::mergeSingleCol(){
	//check if column exist
	bool colExist = hasColumns(currFirst);

	vector<vector<VALUE>> newTuples;
	if(colExist){
		int matchIndex = colIndex.find(currFirst)->second;
		for(vector<vector<VALUE>>::iterator tuple = table.begin(); tuple != table.end(); ++tuple){
			VALUE toMatch = *((tuple->begin()) + matchIndex);

			if(single_col_DS.count(toMatch) != 0){   //we have a match
				vector<VALUE> newTuple(*tuple);
				newTuples.push_back(newTuple);
			}
		}

	}
	else{
		//it doesn't exist so we insert
		colIndex.insert(std::pair<COLNAME, int>(currFirst, colIndex.size()));
		//cartesian product
		for(vector<vector<VALUE>>::iterator tuple = table.begin(); tuple != table.end(); ++tuple){
			for(unordered_set<VALUE>::iterator it = single_col_DS.begin(); it != single_col_DS.end(); ++it){
				vector<VALUE> newTuple(*tuple);
				newTuple.push_back(*it);
				newTuples.push_back(newTuple);
			}
		}
	}

	table = newTuples;
}

void Table::append(COLNAME firstCol, VALUE first, COLNAME secondCol, vector<VALUE> secondValues){
	for(vector<VALUE>::iterator it = secondValues.begin(); it != secondValues.end(); ++it){
		vector<VALUE> tuple;
		tuple.push_back(first);
		tuple.push_back(*it);
		table.push_back(tuple);
	}
}

void Table::append(COLNAME col, vector<VALUE> values){
	for(vector<VALUE>::iterator it = values.begin(); it != values.end(); ++it){
		vector<VALUE> tuple;
		tuple.push_back(*it);
		table.push_back(tuple);
	}
}