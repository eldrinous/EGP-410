#include "aStar.h"
#include "Path.h"
#include "Connection.h"
#include "GridGraph.h"
#include "GameApp.h"
#include <PerformanceTracker.h>
#include <list>
#include <vector>
#include <algorithm>
#include "Grid.h"
#include "Vector2D.h"
using namespace std;

aStarPathfinder::aStarPathfinder(Graph* pGraph)
	:GridPathfinder(dynamic_cast<GridGraph*>(pGraph))
{
#ifdef VISUALIZE_PATH
	mpPath = NULL;
#endif

}

aStarPathfinder::~aStarPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpPath;
#endif
}

Path* aStarPathfinder::findPath(Node* pFrom, Node* pTo, int index)
{
	gpPerformanceTracker->clearTracker("path");
	gpPerformanceTracker->startTracking("path");
	//allocate nodes to visit list and place starting node in it
	vector<Node*> nodesToVisit;
	nodesToVisit.insert(nodesToVisit.begin(),pFrom);

	int isWhatPather = index; // 1: depth, 2: dijkstra 3:aStar

#ifdef VISUALIZE_PATH
	delete mpPath;
	mVisitedNodes.clear();
	pFrom->setCost(0);
	pFrom->setHeu(0);
	mVisitedNodes.push_back(pFrom);
#endif

	//create Path
	Path* pPath = new Path();

	Node* pCurrentNode = NULL;
	bool toNodeAdded = false;

	while (pCurrentNode != pTo && nodesToVisit.size() > 0)
	{
		//get current node from front of list
		pCurrentNode = nodesToVisit.front();

		//remove node from list
		nodesToVisit.erase(nodesToVisit.begin());

		//add Node to Path
		pPath->addNode(pCurrentNode);
		
		//get the Connections for the current node
		vector<Connection*> connections = mpGraph->getConnections(pCurrentNode->getId());

		//add all toNodes in the connections to the "toVisit" list, if they are not already in the list
		for (unsigned int i = 0; i < connections.size(); i++)
		{
			Connection* pConnection = connections[i];
			Node* pTempToNode = connections[i]->getToNode();
			if (!toNodeAdded &&
				!pPath->containsNode(pTempToNode) &&
				find(nodesToVisit.begin(), nodesToVisit.end(), pTempToNode) == nodesToVisit.end())
			{
				if (isWhatPather == 1)
				{
					nodesToVisit.insert(nodesToVisit.begin(), pTempToNode);
				}
				else
				{
					float distance = 0;
					Grid* pGrid = dynamic_cast<GameApp*>(gpGame)->getGrid();
					Vector2D connectionPos = pGrid->getULCornerOfSquare(pTempToNode->getId());
					if (isWhatPather == 3)
					{
						Vector2D targ = pGrid->getULCornerOfSquare(pTo->getId());
						distance = sqrt(pow(connectionPos.getX() - targ.getX(), 2) + pow(connectionPos.getY() - targ.getY(), 2));
					}
					else
					{
						distance = 0;
					}
					Vector2D curPos = pGrid->getULCornerOfSquare(pCurrentNode->getId());
					float costSoFar = sqrt(pow(connectionPos.getX() - curPos.getX(), 2) + pow(connectionPos.getY() - curPos.getY(), 2)) + pCurrentNode->getCost();

					pTempToNode->setCost(costSoFar);
					pTempToNode->setBackPTR(pCurrentNode);
					pTempToNode->setHeu(distance+costSoFar);
					if (nodesToVisit.size() == 0)
					{
						nodesToVisit.push_back(pTempToNode);
					}
					else
					{
						for (vector<int>::size_type i = 0; i != nodesToVisit.size(); ++i) {
							if (nodesToVisit[i]->getHeu() > pTempToNode->getHeu())
							{
								nodesToVisit.insert(nodesToVisit.begin() + i, pTempToNode);
								break;
							}
							else if (i == nodesToVisit.size() - 1)
							{
								nodesToVisit.push_back(pTempToNode);
								break;
							}
						}
					}
				}
				
				if (pTempToNode == pTo)
				{
					toNodeAdded = true;
				}

#ifdef VISUALIZE_PATH
				mVisitedNodes.push_back(pTempToNode);
#endif
			}
		}
	}
	
	
	//clear path
	if (isWhatPather != 1)
	{
		delete pPath;
		pPath = new Path();

		Node* tempNode = pTo;
		while (tempNode != pFrom)
		{
			pPath->addNode(tempNode);
			tempNode = tempNode->getBackPTR();
		}
		pPath->addNode(tempNode);
	}
	


	gpPerformanceTracker->stopTracking("path");
	mTimeElapsed = gpPerformanceTracker->getElapsedTime("path");

#ifdef VISUALIZE_PATH
	mpPath = pPath;
#endif
	return pPath;

}