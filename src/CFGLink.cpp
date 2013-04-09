#include "CFGLink.h"

CFGLink::CFGLink(int link, int edge){
	linkTo = link;
	edgeNumber = edge;
}

CFGLink::~CFGLink(){
}

void CFGLink::setLinkTo(int link){
	linkTo = link;
}

void CFGLink::setEdgeNumber(int edge){
	edgeNumber = edge;
}

int CFGLink::getLinkTo(){
	return linkTo;
}

int CFGLink::getEdgeNumber(){
	return edgeNumber;
}