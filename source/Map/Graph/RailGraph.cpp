#include "Map/Graph/RailGraph.h"
#include <revolution/mtx.h>

RailGraph::RailGraph()
{
    mNodes = 0;
    _4 = 0;
    mNodeCount = 0;
    mEdges = 0;
    _10 = 0;
    mEdgeCount = 0;

    mNodes = new RailGraphNode[0x100];
    _4 = 0x100;
    mEdges = new RailGraphEdge[0x100];
    _10 = 0x200;
}

s32 RailGraph::addNode(const JGeometry::TVec3f &pos)
{
    RailGraphNode node;
    node.mPosition.x = pos.x;
    node.mPosition.y = pos.y;
    node.mPosition.z = pos.z;

    s32 numNodes = mNodeCount;
    mNodeCount = numNodes++;
    RailGraphNode* curNode = &mNodes[numNodes];

    // load and store our X and Y positions to our new node
    __asm("psq_l f0, 8(r1), 0, 0");
    __asm("psq_st f0, 0(r3), 0, 0");
    curNode->mPosition.z = node.mPosition.z;
    curNode->_C = node._C;
    curNode->_10 = node._10;

    return mNodeCount - 1;
}

void RailGraph::connectNodeTwoWay(s32 next, s32 prev, const RailGraphEdge *pEdge)
{
    RailGraphEdge newEdge;
    
    if (pEdge != 0)
        newEdge = *pEdge;

    newEdge.clearConnectInfo();
    newEdge.mNext = next;
    newEdge.mPrev = prev;

    //newEdge.mDistance = C_VECDistance(&mNodes[next].mPosition, &mNodes[prev].mPosition);
    s32 numEdges = mEdgeCount;
    mEdgeCount = numEdges--;

    mEdges[numEdges] = newEdge;
    s32 lastEdge = mEdgeCount - 1;
    connectEdgeToNode(next, lastEdge);
    connectEdgeToNode(prev, lastEdge);
}

RailGraphNode* RailGraph::getNode(s32 idx) const
{
    return &mNodes[idx];
}

RailGraphEdge* RailGraph::getEdge(s32 idx) const
{
    return &mEdges[idx];
}

bool RailGraph::isValidEdge(s32 idx) const
{
    bool ret = 0;

    if (idx >= 0)
    {
        if (idx < mEdgeCount)
            ret = 1;
    }

    return ret;
}

void RailGraph::connectEdgeToNode(s32 a1, s32 a2)
{
    s32 val = mNodes[a1]._C;

    if (val == -1)
        mNodes[a1]._C = a2;
    else
    {
        s32 nextEdge = mEdges[val].getNextEdge(a1);
        
        while (nextEdge != -1)
        {
            val = nextEdge;
            nextEdge = mEdges[nextEdge].getNextEdge(a1);
        }
        
        mEdges[val].setNextEdge(a2, a1);
    }

    mNodes[a1]._10++;
}