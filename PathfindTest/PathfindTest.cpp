// PathfindTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

struct Node
{
    Node(const int index, const int cameFromIndex, const int x, const int y, const int globalGoal, const int distance ) :
        index(index),
        cameFromIndex(cameFromIndex),
        x(x), y(y),
        globalGoal(globalGoal),
        localGoal(distance)
    {}

    int index;
    int cameFromIndex;
    int x;
    int y;
    int globalGoal;  // cost value to Target(heuristic)
    int localGoal;   // cost from start

    //compare for globalGoal
    bool operator > (const Node& n) const {
        return (globalGoal > n.globalGoal);
    };
};
//find index from x and y coordinate
int getNodeIndex(const int x, const int y, const int mapWidth)
{
    return x + (y * mapWidth);
}
  //Manhattan distance on a square grid
int heuristic(const int nStartX, const int nStartY, const int nTargetX, const int nTargetY)
{
    return std::abs(nStartX - nTargetX) + std::abs(nStartY - nTargetY);
}

bool isNodeVisited(const int index, std::unordered_set<int>& visitIndex)
{
    return visitIndex.find(index) != visitIndex.end();
}

bool isPathPassable(const unsigned char value)
{
    return value == 1;
}

int FindPath(const int nStartX, const int nStartY,
    const int nTargetX, const int nTargetY,
    const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
    int* pOutBuffer, const int nOutBufferSize)
{
    
    int startIndex = getNodeIndex(nStartX, nStartY, nMapWidth);
    int targetIndex = getNodeIndex(nTargetX, nTargetY, nMapWidth);

    //If Start and Target are the same position the function should return 0
    if(startIndex == targetIndex)
    {
        return 0;
    }

    //if target index is not valid, return -1
    if (targetIndex > (nMapHeight * nMapWidth) || targetIndex < 0)
    {
        return -1;
    }

    //The PriorityQueue is created to get the node with the lowest value first.
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> frontier;
    //for visited indices
    std::unordered_set<int> visitedIndices;
    //to keep the where we came from
    std::unordered_map<int, int> cameFrom;

    //start node
    Node startNode = Node(getNodeIndex(nStartX, nStartY, nMapWidth), 0, nStartX, nStartY, heuristic(nStartX, nStartY, nTargetX, nTargetY), 0);
    frontier.push(startNode);

    while(!frontier.empty())
    {
        Node currentNode = frontier.top();
        frontier.pop();
        int currentIndex = currentNode.index;

        //if index is already visited skip it.
        if (visitedIndices.find(currentIndex) != visitedIndices.end())
        {
            continue;
        }

        cameFrom.emplace(currentIndex, currentNode.cameFromIndex);

        //found
        if(currentIndex == targetIndex)
        {
           if (currentNode.localGoal <= nOutBufferSize)
           {
                int parentIndex = currentIndex;

                for (int i = currentNode.localGoal - 1 ; i >= 0; i--)
                {
                    pOutBuffer[i] = parentIndex;
                    parentIndex = cameFrom.at(parentIndex);
                }
           }
           return currentNode.localGoal;
        }

        //check neighbors

        int neighborIndex = 0;
        int neighborX = 0;
        int neighborY = 0;
        bool isIndexValid = false;

        for (int i = 0; i < 4; i++)
        {
            switch (i)
            {
            case 0: // Up neighbor
                neighborIndex = currentNode.index - nMapWidth;
                neighborX = currentNode.x;
                neighborY = currentNode.y - 1;
                isIndexValid = neighborIndex >= 0;
                break;

            case 1: //Left neighbor
                neighborIndex = currentNode.index - 1;
                neighborX = currentNode.x - 1;
                neighborY = currentNode.y;
                isIndexValid = neighborX >= 0;
                break;

            case 2: //Down neighbor
                neighborIndex = currentNode.index + nMapWidth;
                neighborX = currentNode.x;
                neighborY = currentNode.y + 1;
                isIndexValid = neighborIndex < (nMapHeight * nMapWidth);
                break;

            case 3: //Right neighbor
                neighborIndex = currentNode.index + 1;
                neighborX = currentNode.x + 1;
                neighborY = currentNode.y;
                isIndexValid = neighborX < nMapWidth;
                break;
            }

            //validation for neighbor
            if (!isNodeVisited(neighborIndex, visitedIndices) && isPathPassable(pMap[neighborIndex]) && isIndexValid)
            {
                int distance = currentNode.localGoal + 1;

                Node neighborNode = Node(neighborIndex, currentIndex, neighborX, neighborY, (distance + heuristic(neighborX, neighborY, nTargetX, nTargetY)), distance);

                frontier.push(neighborNode);
            }
        }

        visitedIndices.insert(currentIndex);

    }
    return -1;
}

int main()
{

    unsigned char pMap[] = { 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1 };
    int pOutBuffer[12];

    int steps = FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12);

    //unsigned char pMap[] = { 0, 0, 1, 0, 1, 1, 1, 0, 1 };
    //int pOutBuffer[7];
    //int result = FindPath(2, 0, 0, 2, pMap, 3, 3, pOutBuffer, 7);

    for (int i = 0; i < steps; i++)
    {
        std::cout << pOutBuffer[i] << ",";
    }

}