// BirjikGraph.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <deque>
using namespace std;

const long maxSize = 100001;

deque<bool> isBlack;
vector<long> weightCount;

class NodeEdge {
public:
	NodeEdge(long dest) : pointsTo(dest), nextPtr(nullptr) {}

	NodeEdge* nextPtr;
	long pointsTo;
};

struct Graph {
public:
	Graph(long size, bool isDirected = true) :myEdges(size, nullptr), vertexCount(0), directed(isDirected) {
	}

	void insertNode(long x, long y, bool isDirected) {


		NodeEdge* newEdge = new NodeEdge(y);

		// set to the front of the list for the same id.
		newEdge->nextPtr = myEdges[x];
		myEdges[x] = newEdge;

		// If not directed, add the oposite edge using recursion. 
		if (!isDirected)
		{
			insertNode(y, x, true);
		}
	}

	long getVertexCount()
	{
		return vertexCount;
	}

	// Consider using a deque of bools.
	vector<NodeEdge*> myEdges;
	bool directed;

private:

	long vertexCount;
};

namespace DFS
{
	vector<int> parent(maxSize);
	deque<bool> processed(maxSize);
	deque<bool> visited(maxSize);

	void(*preProcess)(long);
	void(*process)(long, long);
	void(*postProcess)(long);

	void InitSearch(long size)
	{
		parent.assign(size, 0);
		processed.assign(size, 0);
		visited.assign(size, 0);
	}

	long dfs(Graph* graph, long vertex)
	{
		long weight = 0;
		NodeEdge* edge = graph->myEdges[vertex];

		visited[vertex] = true;

		// pre process edge
		if (preProcess)
			preProcess(vertex);

		while (edge != nullptr) {
			long nextVertex = edge->pointsTo;
			if (visited[nextVertex] == false)
			{
				parent[nextVertex] = vertex;

				// process edge
				if (process)
					process(vertex, nextVertex);

				weight += dfs(graph, nextVertex);
			}
			else if ((!processed[nextVertex] && (parent[vertex] != nextVertex)) || graph->directed)
			{
				// process edge
			}

			edge = edge->nextPtr;
		}

		// post process
		if (postProcess)
			postProcess(vertex);

		processed[vertex] = true;

		if (isBlack[vertex])
			++weight;

		++(weightCount[weight]);

		return weight;
	}
}


void processNode(long parentId, long childId)
{
	cout << parentId << "->" << childId << endl;
}

void printWeights()
{
	vector<long>::iterator it = weightCount.begin();
	cout << *it++;

	for (; it != weightCount.end(); ++it)
	{
		cout << " " << *it;
	}

	cout << endl;
}

int main()
{
	// set size dynamically
	long dynamicSize = 5+1;

	isBlack.assign(dynamicSize, false);
	weightCount.assign(dynamicSize, 0);

	Graph *myGraph = new Graph(dynamicSize, false);
	myGraph->insertNode(1, 2, false);
	myGraph->insertNode(1, 3, false);
	myGraph->insertNode(1, 4, false);
	myGraph->insertNode(2, 5, false);
	//myGraph->insertNode(4, 7, false);
	//myGraph->insertNode(7, 0, false);

	isBlack[5] = true;
	isBlack[2] = true;
	isBlack[4] = true;

	//DFS::process = processNode;
	DFS::InitSearch(dynamicSize);
	DFS::dfs(myGraph, 1);

	/*
	long weight = 0;
	for (vector<long>::iterator it = weightCount.begin(); it != weightCount.end() && weight < 10; ++it)
	{
		cout << "w[" << weight << "]=" << *it << endl;
		++weight;


	}*/ 

		printWeights();

	int i;

	cin >> i;

	return 0;
}

