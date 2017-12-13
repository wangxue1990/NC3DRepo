#pragma once
#include <vector>
#include <string>
using namespace std;

struct GEdge;

struct GNode
{
	string name;
	std::vector<GEdge*> linkEdges;
	std::vector<GEdge*> inEdges;
	std::vector<GEdge*> outEdges;
	void *pData;
};
struct GEdge
{
	string name;
	GNode *startNode;
	GNode *endNode;
	void *pData;
};
class Graph
{
public:
	Graph(){};
	~Graph(){};
	bool AddNode(){	};
	bool DelNode(){};
	bool AddEdge(){	};
	bool DelEdge(){};
private:
	std::vector<GNode*> _nodes;
	std::vector<GEdge*> _edges;
};