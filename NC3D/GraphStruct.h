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
	//�ж���ͨ��
	//�ָ�����
	//
private:
	std::vector<GNode*> _nodes;
	std::vector<GEdge*> _edges;
};
/************************************************************************/
/* ֻ�õ�����ʾ��ͼ                                                        */
/************************************************************************/
class NodeGraph
{
public:
	NodeGraph(){};
	~NodeGraph(){};
	
private:
	void *pContent;//������ͼ�۵ı߻��ߵ㣬���ⶨ��
	vector<NodeGraph*> pLinkGraphNodeS;
	vector<NodeGraph*>pInLinkNode;
	vector<NodeGraph*>pOutLinkNode;

};