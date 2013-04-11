/*
The class Table stores a representation of query clauses.
It contains the values that satisfies the clauses.

Typically, a clause will check if the Table contains the columns using the hasColumns() method.
If it exist, it will get the Vector of possible values using the getColumn() method.
After evaluating and retrieving the values from the PKB, these values will be inserted using the insert() method.
After evaluating a clause, the shrinkTable() must be called.
The table will be returned to the main evaluator for merging, by calling the merge() method.
*/

#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#define NO_DS -1
#define MATCH_FIRST 0
#define MATCH_SECOND 1
#define DUAL_COL 2
#define CART_PRO 3
#define SINGLE_COL 4
#define APPEND 5
#define MATCH_ALL 6
#define MATCH_SOME 7
#define MATCH_NONE 8

using namespace std;

typedef string COLNAME;
typedef string VALUE;

class Table{
public:
	//Constructor, Destructor
	Table();
	~Table();

	Table(vector<vector<VALUE>>, unordered_map<COLNAME, int>);	//easy hack, remove when done

	bool hasColumns(COLNAME);
	vector<VALUE> getColumn(COLNAME);
	vector<vector<VALUE>> getColumnPair(COLNAME, COLNAME);
	vector<vector<VALUE>> getColumnTriple(COLNAME, COLNAME, COLNAME);
	vector<vector<VALUE>> getColumnQuad(COLNAME, COLNAME, COLNAME, COLNAME);

	void insert(COLNAME, VALUE, COLNAME, vector<VALUE>);
	void insert(COLNAME, vector<VALUE>);
	void insert(COLNAME, VALUE, COLNAME, VALUE, COLNAME, VALUE);
	void insert(COLNAME, VALUE, COLNAME, VALUE, COLNAME, VALUE, COLNAME, VALUE);

	void shrinkTable();
	vector<string> getTuple(vector<COLNAME>);
	void print();

private:
	bool checkForConstant(vector<COLNAME>, vector<VALUE>);
	void insertGeneral(vector<COLNAME>, vector<VALUE>);
	void assignCaseNumber(vector<COLNAME>);
	void mergeMatchFirst();
	void mergeMatchSecond();
	void mergeDualCol();
	void mergeCartPro();
	void mergeSingleCol();
	void mergeMatchAll();
	void mergeMatchSome();
	void mergeMatchNone();
	void append(COLNAME, VALUE, COLNAME, vector<VALUE>);
	void append(COLNAME, vector<VALUE>);
	void appendAll(vector<VALUE>);
	void cartProduct(vector<vector<VALUE>>*, vector<VALUE>*, int, vector<string>*);

	vector<vector<VALUE>> table;
	unordered_map<COLNAME, int> colIndex;

	COLNAME currFirst;
	COLNAME currSecond;
	int caseNo;
	//MATCH_FIRST
	unordered_map<VALUE, vector<VALUE>> match_first_DS;

	//MATCH_SECOND
	unordered_multimap<VALUE, VALUE> match_second_DS;

	//DUAL_COL
	unordered_set<string> dual_col_DS;

	//CART_PRO
	vector<vector<VALUE>> cart_pro_DS;

	//single column insert
	unordered_set<VALUE> single_col_DS;

	//MATCH_SOME
	vector<COLNAME> toMatch;
	vector<COLNAME> toInsert;
	unordered_multimap<VALUE, vector<VALUE>> match_some_DS;
};