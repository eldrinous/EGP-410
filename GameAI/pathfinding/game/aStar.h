#pragma once

#include "GridPathfinder.h"
#include <vector>


class Path;
class Graph;
class GraphicsBuffer;
class Grid;

/*
This class will implement Astar,Dijkstra, and depth first search all based on a index that is passed in.
*/
class aStarPathfinder :public GridPathfinder
{
public:
	aStarPathfinder(Graph* pGraph);
	~aStarPathfinder();

	Path* findPath(Node* pFrom, Node* pTo, int index);//make sure to delete the path when you are done!
private:
};

