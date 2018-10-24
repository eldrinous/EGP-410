#pragma once

#include <Trackable.h>

const int BAD_NODE_ID = -1;
#define NODE_ID int

class Node:public Trackable
{
public:
	Node( const NODE_ID& id );
	Node();
	~Node();

	const NODE_ID& getId() const {return mId;};

	Node* getBackPTR();
	float getCost();
	float getHeu();
	void setBackPTR(Node* newBacker);
	void setCost(float newCost);
	void setHeu(float newheu);
	
private:
	const NODE_ID mId;
	float costSoFar;
	float mHeuristic;
	Node* pBackPTR;
};