#include "queryTree.h"
using namespace std;

queryTree::queryTree() {
}

void queryTree::insertPattern(string _synonym, string _synonym_type, string _pattern1, string _pattern1_type, string _pattern2,string _pattern2_type) {
	vector<string> tempVector;
	tempVector.push_back(_synonym);
	tempVector.push_back(_synonym_type);
	tempVector.push_back(_pattern1);
	tempVector.push_back(_pattern1_type);
	tempVector.push_back(_pattern2);
	tempVector.push_back(_pattern2_type);
	patternTree.push_back(tempVector);
}

void queryTree::insertIfPattern(string _synonym, string _synonym_type, string _pattern1, string _pattern1_type, string _pattern2,string _pattern2_type, string _pattern3, string _pattern3_type) {
	vector<string> tempVector;
	tempVector.push_back(_synonym);
	tempVector.push_back(_synonym_type);
	tempVector.push_back(_pattern1);
	tempVector.push_back(_pattern1_type);
	tempVector.push_back(_pattern2);
	tempVector.push_back(_pattern2_type);
	tempVector.push_back(_pattern3);
	tempVector.push_back(_pattern3_type);
	patternTree.push_back(tempVector);
}

void queryTree::insertWith(string _with1_type, string _with1_1, string _with1_2, string _with1_3, string _with2_type, string _with2_1, string _with2_2, string _with2_3) {
	vector<string> tempVector;
	tempVector.push_back(_with1_type);
	tempVector.push_back(_with1_1);
	tempVector.push_back(_with1_2);
	tempVector.push_back(_with1_3);
	tempVector.push_back(_with2_type);
	tempVector.push_back(_with2_1);
	tempVector.push_back(_with2_2);
	tempVector.push_back(_with2_3);
	withTree.push_back(tempVector);
}

/*
void queryTree::insertWith(string _with_type, string _with1, string _with1_type, string _with2, string _with2_type) {
	vector<string> tempVector;
	tempVector.push_back(_with_type);
	tempVector.push_back(_with1);
	tempVector.push_back(_with1_type);
	tempVector.push_back(_with2);
	tempVector.push_back(_with2_type);
	withTree.push_back(tempVector);
}
*/

void queryTree::insertSuchThat(string _relationship, string _value1, string _value1_type, string _value2, string _value2_type) {
	vector<string> tempVector;
	tempVector.push_back(_relationship);
	tempVector.push_back(_value1);
	tempVector.push_back(_value1_type);
	tempVector.push_back(_value2);
	tempVector.push_back(_value2_type);
	suchThatTree.push_back(tempVector);
}

void queryTree::insertSelect(string _select, string _select_type) {
	vector<string> tempVector;
	tempVector.push_back(_select);
	tempVector.push_back(_select_type);	
	selectTree.push_back(tempVector);
}

void queryTree::insertSuchThatWithPosition(int _index, vector<string> _query){
	suchThatTree.insert(suchThatTree.begin() + _index, _query );
}

void queryTree::insertPatterntWithPosition(int _index, vector<string> _query) {
	patternTree.insert(patternTree.begin() + _index, _query );
}

void queryTree::insertWithWithPosition(int _index, vector<string> _query) {
	withTree.insert(withTree.begin() + _index, _query );
}

void queryTree::insertSelectWithPosition(int _index, vector<string> _query) {
	selectTree.insert(selectTree.begin() + _index, _query );
}

int queryTree::getPatternQuerySize(){
	return patternTree.size();
}

int queryTree::getSuchThatQuerySize(){
	return suchThatTree.size();
}

int queryTree::getWithQuerySize(){
	return withTree.size();
}

int queryTree::getSelectSize(){
	return selectTree.size();
}

vector<string> queryTree::getSuchThatQuery(int _queryPosition){
	return suchThatTree[_queryPosition];
}

vector<string> queryTree::getPatternQuery(int _queryPosition){
	return patternTree[_queryPosition];
}

vector<string> queryTree::getWithQuery(int _queryPosition){
	return withTree[_queryPosition];
}

vector<string> queryTree::getSelect(int _queryPosition){
	return selectTree[_queryPosition];
}

void queryTree::deleteSuchThat(int _index){
	suchThatTree.erase(suchThatTree.begin() + _index);
}

void queryTree::deletePattern(int _index){
	patternTree.erase(patternTree.begin() + _index);
}

void queryTree::deleteWith(int _index){
	withTree.erase(withTree.begin() + _index);
}

void queryTree::deleteSelect(int _index){
	selectTree.erase(selectTree.begin() + _index);
}
