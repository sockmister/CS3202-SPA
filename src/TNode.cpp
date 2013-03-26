#pragma once

#include "TNode.h"

  // Default constructor
  TNode::TNode(){
  }

  // Create a TNode with supplied parameters
  TNode::TNode(node_TYPE nodeType, STMT stmtNumber) {  
	  this->nodeType = nodeType;
	  this->stmtNumber = stmtNumber;
	  this->nodeValue = "";
	  this->rightSibling = -1;
	  this->leftSibling = -1;
	  this->parent = -1;
  }

  node_TYPE TNode::getNodeType() {
	  return nodeType;
  }

  int TNode::getIndex() {
	  return index;
  }

  int TNode::getStmtNumber() {
	  return stmtNumber;
  }

  CONSTANT TNode::getNodeValue() {
	  return nodeValue;
  }

  int TNode::getParent() {
	  return parent;
  }

  int TNode::getLeftSibling() {
	  return leftSibling;
  }

  int TNode::getRightSibling() {
	  return rightSibling;
  }
  
  std::vector<int> TNode::getChildren() {
	  return children;
  }

  void TNode::setIndex(int index){
	  this->index = index;
  }

  void TNode::setStmtNumber(int n) {
	  stmtNumber = n;
  }

  void TNode::setNodeValue(CONSTANT v) {
	  nodeValue = v;
  }

  void TNode::setParent(int p) {
	  parent = p;
  }

  void TNode::setLeftSibling(int l) {
	  leftSibling = l;
  }

  void TNode::setRightSibling(int r) {
	  rightSibling = r;
  }

  void TNode::addChild(int c) {
	  children.push_back(c);
  }








