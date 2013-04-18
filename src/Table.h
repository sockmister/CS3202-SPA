#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <set>

#define MATCH_NONE 1
#define MATCH_ALL_SINGLE 2
#define MATCH_SOME_SINGLE 3
#define MATCH_ALL_MULTIPLE 4

#define MATCH_SOME_MULTIPLE 5

/*!  \class Table.
	 \brief Table to contain relational tuples. Used for evaluation of queries.
*/

using namespace std;

typedef string COLNAME;
typedef string VALUE;

class Table{
public:
	//Constructor, Destructor
	Table();
	~Table();

	//! check if a column exists in the Table
	/*!
		\param COLNAME col
		\return true if column exists, and false otherwise.
	*/
	bool hasColumns(COLNAME);

	//! get a column in the table
	/*!
		\param COLNAME col
		\return returns a vector<VALUE> containing the column values.
	*/
	vector<VALUE> getColumn(COLNAME);
	
	//! get a pair of related columns in the table
	/*!
		\param COLNAME col, COLNAME col
		\return returns a vector<vector<VALUE>> representing the pair tuples.
	*/
	vector<vector<VALUE>> getColumnPair(COLNAME, COLNAME);
	
	//! get a triple of related columns in the table
	/*!
		\param COLNAME col, COLNAME col, COLNAME col
		\return returns a vector<vector<VALUE>> representing the triple tuples.
	*/
	vector<vector<VALUE>> getColumnTriple(COLNAME, COLNAME, COLNAME);
	
	//! get a quadruple of related columns in the table
	/*!
		\param COLNAME col, COLNAME col, COLNAME col, COLNAME col
		\return returns a vector<vector<VALUE>> representing the quadruple tuples.
	*/
	vector<vector<VALUE>> getColumnQuad(COLNAME, COLNAME, COLNAME, COLNAME);

	//! inserts a quadrule of related columns in the table
	/*!
		\param COLNAME col, VALUE val, COLNAME col, VALUE val, COLNAME col, VALUE val, COLNAME col, VALUE val
		\return void
	*/
	void insert(COLNAME, VALUE, COLNAME, VALUE, COLNAME, VALUE, COLNAME, VALUE);
	
	//! inserts a triple of related columns in the table
	/*!
		\param COLNAME col, VALUE val, COLNAME col, VALUE val, COLNAME col, VALUE val
		\return void
	*/
	void insert(COLNAME, VALUE, COLNAME, VALUE, COLNAME, VALUE);
	
	//! inserts a double of related columns in the table
	/*!
		\param COLNAME col, VALUE val, COLNAME col, vector<VALUE> values
		\return void
	*/
	void insert(COLNAME, VALUE, COLNAME, vector<VALUE>);
	
	//! inserts a column in the table
	/*!
		\param COLNAME col, vector<VALUE> values
		\return void
	*/
	void insert(COLNAME, vector<VALUE>);

	//! shrink / merges the table based on the current intermediary values inserted
	/*!
		\return void
	*/
	void shrinkTable();
	
	//! gets a tuple as specified by the vector
	/*!
		\return a vector<string>, with each string representing a tuple ready for printing.
	*/
	vector<string> getTuple(vector<COLNAME>);
	void print();

private:
	void getCaseNo();
	//merge second into first
	void merge(list<vector<VALUE>>*, vector<COLNAME>*, list<vector<VALUE>>*, vector<COLNAME>*, int);
	vector<int> getAllIndexes();
	vector<pair<int,int>> getToHashIndexes(vector<COLNAME>, vector<COLNAME>);
	vector<int> getToAddIndexes(vector<COLNAME>, vector<COLNAME>);
	void updateColLookUp();
	bool checkForConstant(vector<COLNAME>, vector<VALUE>);
	void cart_product(vector<vector<VALUE>>, vector<VALUE>, int, vector<vector<VALUE>>*);
	void cart_product_joint(vector<vector<VALUE>>, vector<VALUE>, int, vector<string>*);
	int existInTable(vector<COLNAME>);

	//columns
	vector<COLNAME>* curr_columns;
	vector<int> mapped_tables;

	//table before shrinkTable
	list<vector<VALUE>> * intermediate_table;

	//collection of tables
	vector< list<vector<VALUE>> *> tables;

	//collection of invidual columns
	unordered_map<COLNAME, unordered_set<VALUE>> column_values;
	//unordered_map<COLNAME, set<VALUE>> column_values;

	//col-to-table lookup
	unordered_map<COLNAME, pair<int, int>> colLookUp;
	vector< vector<COLNAME>* > tableLookUp;

	int caseNo;
	bool newClause;
};
