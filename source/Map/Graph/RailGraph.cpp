#include "Map/Graph/RailGraph.h"
#include "OS/vec.h"

RailGraph::RailGraph()
{
    this->mNodes = 0;
    this->_4 = 0;
    this->mNodeCount = 0;
    this->mEdges = 0;
    this->_10 = 0;
    this->mEdgeCount = 0;

    this->mNodes = new RailGraphNode[0x100];
    this->_4 = 0x100;
    this->mEdges = new RailGraphEdge[0x100];
    this->_10 = 0x200;
}

s32 RailGraph::addNode(const JGeometry::TVec3<f32> &pos)
{
    RailGraphNode node;
    node.mPosition.x = pos.x;
    node.mPosition.y = pos.y;
    node.mPosition.z = pos.z;

    s32 numNodes = this->mNodeCount;
    this->mNodeCount = numNodes++;
    RailGraphNode* curNode = &this->mNodes[numNodes];

    // load and store our X and Y positions to our new node
    __asm("psq_l f0, 8(r1), 0, 0");
    __asm("psq_st f0, 0(r3), 0, 0");
    curNode->mPosition.z = node.mPosition.z;
    curNode->_C = node._C;
    curNode->_10 = node._10;

    return this->mNodeCount - 1;
}

void RailGraph::connectNodeTwoWay(s32 next, s32 prev, const RailGraphEdge *edge)
{
    RailGraphEdge newEdge;
    
    if (edge != 0)
        newEdge = *edge;

    newEdge.clearConnectInfo();
    newEdge.mNext = next;
    newEdge.mPrev = prev;

    newEdge.mDistance = PSVECDistance(&this->mNodes[next].mPosition, &this->mNodes[prev].mPosition);
    s32 numEdges = this->mEdgeCount;
    this->mEdgeCount = numEdges--;

    this->mEdges[numEdges] = newEdge;
    s32 lastEdge = this->mEdgeCount - 1;
    this->connectEdgeToNode(next, lastEdge);
    this->connectEdgeToNode(prev, lastEdge);
}

RailGraphNode* RailGraph::getNode(s32 idx) const
{
    return &this->mNodes[idx];
}

RailGraphEdge* RailGraph::getEdge(s32 idx) const
{
    return &this->mEdges[idx];
}

bool RailGraph::isValidEdge(s32 idx) const
{
    bool ret = 0;

    if (idx >= 0)
    {
        if (idx < this->mEdgeCount)
            ret = 1;
    }

    return ret;
}

void RailGraph::connectEdgeToNode(s32 a1, s32 a2)
{
    s32 val = this->mNodes[a1]._C;

    if (val == -1)
        this->mNodes[a1]._C = a2;
    else
    {
        s32 nextEdge = this->mEdges[val].getNextEdge(a1);
        
        while (nextEdge != -1)
        {
            val = nextEdge;
            nextEdge = this->mEdges[nextEdge].getNextEdge(a1);
        }
        
        this->mEdges[val].setNextEdge(a2, a1);
    }

    this->mNodes[a1]._10++;
}