#include "Node.h"

Node::Node()
:mId(BAD_NODE_ID)
{
}

Node::Node( const NODE_ID& id )
:mId(id)
{
}

Node::~Node()
{
}


Node* Node::getBackPTR()
{
	return pBackPTR;
}

float Node::getCost()
{
	return costSoFar; 
}

float Node::getHeu()
{
	return mHeuristic;
}

void  Node::setBackPTR(Node* newBacker)
{
	pBackPTR = newBacker;
}

void  Node::setCost(float newCost)
{
	costSoFar = newCost;
}

void Node::setHeu(float newheu)
{
	mHeuristic = newheu;
}