#include <string>
#include <vector>
#include <iostream>

using namespace std;
#pragma once

class queryTree
{
private:
	vector<vector<string>> suchThatTree;
	vector<vector<string>> patternTree;
	vector<vector<string>> withTree;
	vector<vector<string>> selectTree;

public:
	queryTree();
	//~queryTree();

	void insertSuchThat(string _relationship, string _value1, string _value1_type, string _value2, string _value2_type);	
	void insertPattern(string _synonym, string _synonym_type, string _pattern1, string _pattern1_type, string _pattern2, string _pattern2_type);
	void insertIfPattern(string _synonym, string _synonym_type, string _pattern1, string _pattern1_type, string _pattern2, string _pattern2_type, string _pattern3, string _pattern3_type);
	//void insertWith(string _with_type, string _with1, string _with1_type, string _with2, string _with2_type);
	// Need confirm the format
	void insertWith(string _with1_type, string _with1_1, string _with1_2, string _with1_3, string _with2_type, string _with2_1, string _with2_2, string _with2_3);
	void insertSelect(string _select, string _select_type);

	int getSuchThatQuerySize();
	int getPatternQuerySize();
	int getWithQuerySize();
	int getSelectSize();

	vector<string> getSuchThatQuery(int _queryPosition);
	vector<string> getPatternQuery(int _queryPosition);
	vector<string> getWithQuery(int _queryPosition);
	vector<string> getSelect(int _queryPosition);

	void deleteSuchThat(int _index);
	void deletePattern(int _index);
	void deleteWith(int _index);
	void deleteSelect(int _index);

	void insertSuchThatWithPosition(int _index, vector<string> _query);	
	void insertPatterntWithPosition(int _index, vector<string> _query);	
	void insertWithWithPosition(int _index, vector<string> _query);	
	void insertSelectWithPosition(int _index, vector<string> _query);	
};