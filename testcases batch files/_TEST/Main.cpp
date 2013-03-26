#include "PQLController.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <list>
#include <vector>

void main () {
	vector <string> queryVector;
	vector <bool> boolVector;
	string query;
	
	bool correctSelect = true; bool correctSuchThat = true; bool correctPattern = true; bool correctWith = true; bool correctMixed = true;
	//bool correctSelect = false; bool correctSuchThat = false; bool correctPattern = false; bool correctWith = false; bool correctMixed = false;

	//bool wrongSelect = true; bool wrongSuchThat = true; bool wrongPattern = true; bool wrongWith = true; bool wrongMixed = true;
	bool wrongSelect = false; bool wrongSuchThat = false; bool wrongPattern = false; bool wrongWith = false; bool wrongMixed = false;
	
	bool extremeParsing = false;

	// Getting line numbers
	/*int n = __LINE__;
	cout << n << "\n";*/
	
	/* Master query	 */
	query = "procedure p1, p2; stmtLst sl1; stmt s1, s2; assign a1, a2, a3; call c1, c2; while w1; if f1; variable v1, v2; constant const1; prog_line pl1; plus plu1; minus m1; times t1; Select <a1, p1, v2> such that Parent(s1, s2) and Affects*(a2,a3) pattern a1 (v1, _) such that Follows(s2,a1) and pattern f1(\"anIdent\", _,_) and w1(v2, _) with p1.procName = \"someProcedureName\" and pl1=s2.stmt# and pl1 = const1.value and s2.stmt#= 3 with p2.procName = v2.varName with const1.value = 4;"; queryVector.push_back(query); boolVector.push_back(correctMixed);
	// Master query modified with assign patterns only
	query = "procedure p1, p2; stmtLst sl1; stmt s1, s2; assign a1, a2, a3; call c1, c2; while w1; if f1; variable v1, v2; constant const1; prog_line pl1; plus plu1; minus m1; times t1; Select <a1, p1, v2> such that Parent(s1, s2) and Affects*(a2,a3) and pattern a2(v2, _) such that Follows(s2,a1) pattern a1(v1,_) and a3  (   v1  ,    _\"v2\"_    ) with p1.procName = \"someProcedureName\" and pl1=s2.stmt# and pl1 = const1.value and s2.stmt#= 3 with p2.procName = v2.varName with const1.value = 4;"; queryVector.push_back(query); boolVector.push_back(correctMixed);

	/*----------------------------
	        Correct Select
	----------------------------*/
	query = "stmt s1; Select s1;"; queryVector.push_back(query); boolVector.push_back(correctSelect);
	query = "stmt s1; Select s1;"; queryVector.push_back(query); boolVector.push_back(correctSelect);
	query = "stmt s1, s2; Select <s1, s2>,;"; queryVector.push_back(query); boolVector.push_back(correctSelect);
	query = "Select BOOLEAN;"; queryVector.push_back(query); boolVector.push_back(correctSelect);
	query = "constant c; Select c"; queryVector.push_back(query); boolVector.push_back(correctSelect);
	query = "assign a; Select a"; queryVector.push_back(query); boolVector.push_back(correctSelect);

	/*--------------------------
	        Wrong Select
	--------------------------*/
	/* Wrong: s2 not declared */
		query = "stmt s1; Select <s1, s2>,;"; queryVector.push_back(query); boolVector.push_back(wrongSelect);
	query = "stmt #2; variable v; Select s such that Modifies(#2, v)"; queryVector.push_back(query); boolVector.push_back(wrongSelect);
	query = "stmt s1; variable v; Select s123 such that Modifies(s1, v)"; queryVector.push_back(query); boolVector.push_back(wrongSelect);
	query = "stmt s1; variable v; Select \"s123\" such that Modifies(s1, v)"; queryVector.push_back(query); boolVector.push_back(wrongSelect);
	query = "stmt s1; variable v; Select such that Modifies(s1, v)"; queryVector.push_back(query); boolVector.push_back(wrongSelect);

	/*--------------------------
	        Correct With
	--------------------------*/
	/* Testing prog_line = Integer in With query */
		query = "stmt s1; assign a1; prog_line pl1; Select pl1 with pl1 = s1.stmt#;"; queryVector.push_back(query); boolVector.push_back(correctWith);
		query = "stmt s1; assign a1; prog_line pl1; Select pl1 with pl1 = 5;"; queryVector.push_back(query); boolVector.push_back(correctWith);
	
	/*------------------------
	        Wrong With
	------------------------*/
	/* Wrong: Wrong const value */	
		query = "procedure p1, p2; stmt s2; variable v2; constant const1; prog_line pl1; Select p1 With p1.procName = \"someProcedureName\" and pl1=s2.stmt# and pl1 = const1.value and s2.stmt#= 3 with p2.procName = v2.varName with const1.value =  \"4\";"; queryVector.push_back(query); boolVector.push_back(wrongWith);
	/* Wrong: Differing types for LHS & RHS */ 
		query = "procedure p1, p2; stmt s1, s2; assign a1; variable v2; constant const1; prog_line pl1; Select <a1, p1, v2> with p1.procName = \"someProcedureName\" and pl1=s2.stmt# and pl1 = const1.value and s2.stmt#= 3 with p2.procName = s1.stmt# with const1.value = 4;"; queryVector.push_back(query); boolVector.push_back(wrongWith);
	/* Wrong: Synonym not of prog_line */ 
		query = "procedure p1, p2; stmtLst sl1; stmt s1, s2; assign a1, a2, a3; call c1, c2; while w1; if f1; variable v1, v2; constant const1; prog_line pl1; plus plu1; minus m1; times t1; Select <a1, p1, v2> with p1.procName = \"someProcedureName\" and s1=s2.stmt# and pl1 = const1.value and s2.stmt#= 3 with p2.procName = v2.varName with const1.value = 4;"; queryVector.push_back(query); boolVector.push_back(wrongWith);
	/* Wrong: prog_line != Integer */ 
		query = "procedure p1, p2; stmtLst sl1; stmt s1, s2; assign a1, a2, a3; call c1, c2; while w1; if f1; variable v1, v2; constant const1; prog_line pl1; plus plu1; minus m1; times t1; Select <a1, p1, v2> with p1.procName = \"someProcedureName\" and pl1=s2.stmt# and pl1 = p2.procName and s2.stmt#= 3 with p2.procName = s1.stmt# with const1.value = 4;"; queryVector.push_back(query); boolVector.push_back(wrongWith);

	/*-----------------------------
		    Correct Pattern
	-----------------------------*/
	query = "if f1; Select f1 pattern f1( \"Ident\", _, _)"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "if f1; Select f1 pattern f1(_, _, _)"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "if f1; variable v1; Select f1 pattern f1(v1, _, _)"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "assign a1; variable v1; Select a1 pattern a1(v1, \"x + y\")"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	//query = "assign a1; variable v1; Select a1 pattern a1(v1, \"Ident*4-3*(x + y-2*4)-(2*x)+z2x3 *  4\")"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "assign a1; variable v1; Select a1 pattern a1(v1, _\"x + y\"_)"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "assign a1; variable v1; Select a1 pattern a1(_, _\"x + y\"_)"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "assign a1; variable v1; Select a1 pattern a1(_, _)"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "while w1; variable v1; Select w1 pattern w1(_, _)"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(_, _)"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(\"x123\", _)"; queryVector.push_back(query); boolVector.push_back(correctPattern);
	query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(v, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(correctPattern);

	/*---------------------------
			Wrong Pattern
	---------------------------*/
	/* Wrong: Invalid var_name in expr*/
		query = "assign a1; variable v1; Select a1 pattern a1(v1, \"Ident*4-3*(x + (y-2)*4)-(2*x)+z2x3# *  4\")"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
	/* Wrong: Missing plus,minus and factors */
		query = "assign a1; variable v1; Select a1 pattern a1(v1, \"*4-3\")"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "assign a1; variable v1; Select a1 pattern a1(v1, \"Ident*4-3*(x + (y-)*4)-(2*x)+z2x3 *  4\")"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "assign a1; variable v1; Select a1 pattern a1(v1, \"Ident*4-3*(+ (y-2)*4)-(2*x)+z2x3 *  4\")"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
	/* Wrong: Missing bracket */
		query = "assign a1; variable v1; Select a1 pattern a1(v1, \"Ident*4-3*(1+ (y-2*4)-(2*x)+z2x3 *  4\")"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
	/* Wrong synonyms */	
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(v, _\"x#\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(a, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(w, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(c, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(s, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(pl, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(v, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select w pattern w(v, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select s pattern s(v, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select c pattern c(v, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select v pattern v(v, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select v1 pattern a(v, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
		query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select pl pattern pl(v, _\"x\"_)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);	
	query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(walk2132, _)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
	query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(\"#walk2132\", _)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);
	query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a("", _)"; queryVector.push_back(query); boolVector.push_back(wrongPattern);

	/*----------------------------- 
			Extreme Parsing
	-----------------------------*/
	//query = ""; queryVector.push_back(query); boolVector.push_back(extremeParsing);
	//query = "12o89n 79q34rfh3r qewy qe9v  9832y edf;sfjfsdfs] ;da;sd'asd"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
	//query = ",,),,"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
	//query = "if f1; Select f1 pattern with such that Follows(s1,s2)"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
	///* Missing bracket */ 
	//	query = "assign a1; variable v1; Select a1 pattern a1(v1, \"Ident*4-3*(1+ (y-2*4)-(2*x)+z2x3 *  4\")"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
	/* Lots of spaces */
		query = "    stmt    s   ;    assign    a    ;    while      w    ;    constant    c    ;    variable    v   ,    v1   ;    prog_line    pl    ;     Select    a    pattern    a   (    \"x\"   ,    _   )    "; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		query = "stmt     s;      assign       a         ;         while       w      ;       constant      c       ;       variable        v       ,       v1       ;       prog_line pl     ;      Select    a    such    that    Uses   (   w   ,    \"a12#\"   )"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		query = "stmt   s  ;   assign   a   ;   while   w   ;   constant   c  ;   variable   v  ,    v1   ;   prog_line pl   ;    Select   a    such   that    Follows   (   _   ,    _   )    pattern   a   (   v   ,   _\"v1\"_    )"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		query = "  stmt   s   ;    assign   a   ;    while   w  ;   constant   c   ;   variable    v  ,   v1  ;   prog_line   pl   ;    Select    a    pattern   a  (  v  ,  _\"v1\"_   )   such   that   Follows  (   _   ,   _    )    "; queryVector.push_back(query); boolVector.push_back(extremeParsing);
	/* Tightly packed */
		query = "stmt s;assign a;while w;constant c;variable v,v1;prog_line pl;Select a pattern a(\"x\",_)"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		query = "stmt s;assign a;while w;constant c;variable v,v1;prog_line pl;Select a such that Uses(w,\"a12#\")"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		query = "stmt s;assign a;while w;constant c;variable v,v1;prog_line pl;Select a such that Follows(_,_) pattern a(v,_\"v1\"_)"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		query = "stmt s;assign a;while w;constant c;variable v,v1;prog_line pl;Select a pattern a(v,_\"v1\"_) such that Follows(_,_)"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
	/* Mix of both */
		query = "stmt s  ; assign a; while w; constant c; variable v,v1; prog_line pl; Select   a pattern a(\"x\",_   )  "; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		query = "stmt s; assign a; while w; constant c; variable v,v1; prog_line pl      ;    Select a such     that Uses(   w,\"a12#\")"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		query = "stmt s; assign a; while w,w1,w2,     w3;constant c; variable v,    v1; prog_line pl; Select a such that Follows(_,_) pattern a(v,_\"v1\"_)"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		query = "stmt s,s1,s2,s3,s4,s5,s6,s6      ,s7; assign a; while w; constant c; variable v,v1; prog_line pl; Select a pattern a(v,_\"v1\"_) such that Follows(_,_)"; queryVector.push_back(query); boolVector.push_back(extremeParsing);
		
	/*----------------------------- 
			Wrong Such That 
	-----------------------------*/
	/* Wrong: Assign cannot be a parent */
		query = "assign a1, a2; Select a1 such that Parent(a1, a2)"; queryVector.push_back(query); boolVector.push_back(wrongSuchThat);
	query = "stmt s; Select s such that Modifies(5, z)"; queryVector.push_back(query); boolVector.push_back(wrongSuchThat);
	query = "assign 12; while #3; Select a such that Uses(12, #3)"; queryVector.push_back(query); boolVector.push_back(wrongSuchThat);

	/*-------------------------------
			Correct Such That
	-------------------------------*/
	query = "stmt s1, s2; Select s1 such that Follows(s1, s2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1, s2, s3, s4; Select s1 such that Follows(s1, 2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s2; Select s2 such that Follows(1, s2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1; Select s1 such that Follows(s1, 2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1, s2; Select s1 such that Follows(s1, s2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1, s2, s3, s4; Select s1 such that Parent(s1, 2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s2; Select s2 such that Parent(1, s2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1; Select s1 such that Parent(s1, 2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1, s2; Select s1 such that Parent(s1, s2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1, s2; Select s1 such that Parent(_, s2)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1, s2; Select s1 such that Parent(s2, _)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1; variable v; Select v such that Modifies(s1, v)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1; variable v; Select v such that Uses(s1, v)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1; variable v; Select v such that Modifies(s1, _)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s; variable v; Select v such that Uses(s, \"x#\")"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s; variable v; Select v such that Uses(s, \"x1\")"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s; variable v; Select v such that Uses(s, \"x\")"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1; variable v; Select v such that Modifies(s1, v)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1; variable v; Select v such that Modifies(s1, v)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1; variable v; Select v such that Modifies(s1, \"x\")"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "stmt s1; variable v; Select v such that Modifies(s1, v)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "prog_line pl; Select pl"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "assign a; prog_line pl; Select pl such that Follows(pl, a)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "assign a; prog_line pl; Select pl such that Follows(pl, a)"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "assign a; Select a such that Uses(a, \"cs3213isawesome\")"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);
	query = "assign a; Select a such that Uses(4, \"cs3213isawesome\")"; queryVector.push_back(query); boolVector.push_back(correctSuchThat);

	/*-------------------------
			Wrong Mixed
	-------------------------*/
	query = "stmt s1, s2; variable v, v1; Select v1 pattern s2(v1, _) such that Uses(s2, v)"; queryVector.push_back(query); boolVector.push_back(wrongMixed);




	/*---------------------No Boolean Vector implemented Yet for below---------------------*/

	/*---------------------------------
			Poh Chiat's Queries 
	---------------------------------*/
	//query = "stmt s, s1, s2; Select s such that Parent(s, s2)"; queryVector.push_back(query);
	//query = "stmt s; assign a; Select s such that Parent(s, a)"; queryVector.push_back(query);
	//query = "stmt s, s1, s2, s3; assign a; Select s such that Parent*(s, a)"; queryVector.push_back(query);
	//query = "stmt s; Select s such that Modifies(s, \"z\")"; queryVector.push_back(query);
	//query = "assign cs3213isawesome, a, a#a; Select a#a such that Follows(a, a) pattern cs3213isawesome(\"cs3213isawesome\", _)"; queryVector.push_back(query);
	///* Invalid */ query = "variable gg; assign gg; Select gg such that Uses(23, gg) pattern gg(gg, _)"; queryVector.push_back(query);
	///* Invalid */ query = "variable gg; assign gg; stmt s; Select s such that Uses(23, gg) pattern gg(gg, _)"; queryVector.push_back(query);
	//query = "stmt s; assign a; Select a such that Modifies(_, \"b\")"; queryVector.push_back(query);
	//query = "variable v; Select v such that Uses(_, v)"; queryVector.push_back(query);
	//query = "while w; Select w such that Follows(16, 19)"; queryVector.push_back(query);
	//query = "assign a; Select a such that Parent*(4, a) pattern a(_, _)"; queryVector.push_back(query);
	//query = "assign a; variable v; Select v such that Uses(a, v)"; queryVector.push_back(query);
	//query = "variable v; assign a; Select v such that Modifies(23, v) pattern a(\"z\", _)"; queryVector.push_back(query);
	//query = "assign a; Select a such that Uses(a, \"cs3213isawesome\")"; queryVector.push_back(query);
	//query = "assign a; Select a such that Modifies(a, \"x\") pattern a(_, _\"z+5\"_)"; queryVector.push_back(query);
	//query = "while w; assign a; stmt s; Select s such that Parent(w, s) pattern a(_, _\"b+c+d+e\"_)"; queryVector.push_back(query);
	//query = "while w; stmt s; Select w such that Follows*(s, w)"; queryVector.push_back(query);

	/* TESTING INPUT AS '0' */
	/*
	query = "stmt s; Select s such that Follows(0, _)"; queryVector.push_back(query);
	query = "stmt s; Select s such that Follows(s, 0)"; queryVector.push_back(query);
	query = "stmt s; Select s such that Follows(s, 0)"; queryVector.push_back(query);
	query = "stmt s0; Select s0 such that Follows(0, 0)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s pattern a(0, _)"; queryVector.push_back(query);
	*/

	/* Such That only */
	
	/* Modifies */
	///* TESTED */ query = "stmt s; Select s such that Modifies(s, \"z\")"; queryVector.push_back(query);
	/*query = "stmt s; Select s such that Modifies(_, \"z\")"; queryVector.push_back(query);
	query = "stmt s; Select s such that Modifies(4, \"z\")"; queryVector.push_back(query);
	//query = "stmt s; assign a, variable v; Select s such that Parent(s, a) and Parent*(s, a) and Modifies(a, \"Ident\")"; queryVector.push_back(query);
	//query = "stmt s; assign a, variable v; Select s such that Parent(s, a) and Parent*(s, a) and Modifies(a, \"Ident\")"; queryVector.push_back(query);	
	
	/* Parent */
	/* TESTED */ /*query = "stmt s; assign a; Select s such that Parent(s, a)"; queryVector.push_back(query);
	query = "while w; assign a; Select s such that Parent(w, a)"; queryVector.push_back(query);
	query = "assign a, a1; Select s such that Parent(a, a1)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent(_, a)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent(4, a)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent(s, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent(_, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent(4, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent(s, 4)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent(_, 4)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent(4, 4)"; queryVector.push_back(query);*/
	
	/* Parent* */
	/* TESTED */ /*query = "stmt s, s1, s2, s3; assign a; Select s such that Parent*(s, a)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent*(_, a)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent*(4, a)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent*(s, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent*(_, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent*(4, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent*(s, 4)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent*(_, 4)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Parent*(4, 4)"; queryVector.push_back(query);*/

	/* Follows */
	/*query = "stmt s; assign a; Select s such that Follows(s, a)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Follows(_, a)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Follows(16, a)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Follows(s, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Follows(_, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Follows(16, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Follows(s, 16)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Follows(_, 16)"; queryVector.push_back(query);
	query = "stmt s; assign a; Select s such that Follows(16, 16)"; queryVector.push_back(query);*/
	
	/* TESTED */ /*query = "while w; stmt s; Select w such that Follows*(s, w)"; queryVector.push_back(query);
	query = "while w; stmt s; Select w such that Follows*(_, w)"; queryVector.push_back(query);
	query = "while w; stmt s; Select w such that Follows*(4, w)"; queryVector.push_back(query);
	query = "while w; stmt s; Select w such that Follows*(s, _)"; queryVector.push_back(query);
	query = "while w; stmt s; Select w such that Follows*(_, _)"; queryVector.push_back(query);
	query = "while w; stmt s; Select w such that Follows*(4, _)"; queryVector.push_back(query);
	query = "while w; stmt s; Select w such that Follows*(s, 4)"; queryVector.push_back(query);
	query = "while w; stmt s; Select w such that Follows*(_, 4)"; queryVector.push_back(query);
	query = "while w; stmt s; Select w such that Follows*(4, 4)"; queryVector.push_back(query);*/

	// Pattern only
	/*query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(\"x\", _)"; queryVector.push_back(query);
	query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(_, _)"; queryVector.push_back(query);
	query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(\"x\", _)"; queryVector.push_back(query);
	query = "stmt s; assign a; while w; constant c; variable v, v1; prog_line pl; Select a pattern a(v, _\"x\"_)"; queryVector.push_back(query);*/
	
	// Such that and pattern
	//query = "variable v; assign a; Select v such that Modifies(23, v) pattern a(\"x\", _)"; queryVector.push_back(query);
	//query = "variable v; assign a; Select v pattern a(\"z\", _) such that Modifies(23, v)"; queryVector.push_back(query);

	/* CS3202 Driver */
	PreProcessor preProcessor;
	list <string> resultsList;
	for(int i=0; i<queryVector.size(); i++) {
		//cout << "Query Number " << i << ":\n";
		if (boolVector[i] == true) {
			queryTree * qt = preProcessor.preProcess(queryVector[i]);
			delete qt;
		}
		//cout << "\ n";
	}
	// Stop console from terminating
	std::getchar();
}