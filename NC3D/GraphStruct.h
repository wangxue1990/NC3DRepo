#pragma once
#include <vector>
#include <string>
#include "ogre.h"  
using namespace std;

struct  VentNode
{
	Vector3 pos;
};

struct VentBranch 
{

};

struct GNodeData 
{
	Vector3 pos;
};

struct GEdgeData 
{
	float with;
	float height;

};
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
	//判断连通性
	//分割网络
	//
private:
	std::vector<GNode*> _nodes;
	std::vector<GEdge*> _edges;
};
/************************************************************************/
/* 只用点来表示的图                                                        */
/************************************************************************/
class NodeGraph
{
public:
	NodeGraph(){};
	~NodeGraph(){};
	
private:
	void *pContent;//可以是图论的边或者点，任意定义
	vector<NodeGraph*> pLinkGraphNodeS;
	vector<NodeGraph*>pInLinkNode;
	vector<NodeGraph*>pOutLinkNode;

};