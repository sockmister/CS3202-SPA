#pragma once

class CFGLink{

private:
	int linkTo;
	int edgeNumber;

public:
	//Constructor, Destructor
	CFGLink(int, int);
	~CFGLink();
	
	void setLinkTo(int);
	void setEdgeNumber(int);

	int getLinkTo();
	int getEdgeNumber();
};