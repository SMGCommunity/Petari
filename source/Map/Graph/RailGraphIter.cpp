#include "Map/Graph/RailGraphIter.h"
#include "defines.h"

RailGraphIter::RailGraphIter(const RailGraph *pGraph)
{
    mGraph = pGraph;
    mCurrentNode = -1;
    mSelectedEdge = -1;
    mNextEdge = -1;
    mNextNode = -1;
}

void RailGraphIter::moveNodeNext()
{
    s32 selectedEdge = mSelectedEdge;
    mNextNode = mCurrentNode;
    RailGraphEdge *pEdge = mGraph->getEdge(selectedEdge);
    mCurrentNode = pEdge->getNextNode(mNextNode);
}

void RailGraphIter::setNode(s32 nextNode)
{
    mCurrentNode = nextNode;
    mNextNode = -1;
    mSelectedEdge = -1;
    mNextEdge = -1;
}

void RailGraphIter::watchStartEdge()
{
    const RailGraph *pGraph = mGraph;
    s32 curNode = mCurrentNode;
    RailGraphNode* node = pGraph->getNode(curNode);
    mNextEdge = node->_C;
}

void RailGraphIter::watchNextEdge()
{
    RailGraphEdge *pEdge = mGraph->getEdge(mNextEdge);
    mNextEdge = pEdge->getNextEdge(mCurrentNode);
}

bool RailGraphIter::isWatchEndEdge() const
{
    return (bool)(__cntlzw(mNextEdge + 1) >> 5);
}

void RailGraphIter::selectEdge()
{
   mSelectedEdge = mNextEdge; 
}

void RailGraphIter::selectEdge(s32 edge)
{
    mSelectedEdge = edge;
}

bool RailGraphIter::isWatchedPrevEdge() const
{
    RailGraphEdge *pEdge = mGraph->getEdge(mNextEdge);
    s32 nextNode = pEdge->getNextNode(mCurrentNode);
    return __cntlzw(nextNode - mNextNode) >> 5; 
}

bool RailGraphIter::isSelectedEdge() const
{
    return mGraph->isValidEdge(mSelectedEdge);
}

RailGraphNode* RailGraphIter::getCurrentNode() const
{
    return mGraph->getNode(mCurrentNode);
}

RailGraphNode* RailGraphIter::getNextNode() const
{
    RailGraphEdge *pEdge = mGraph->getEdge(mSelectedEdge);
    s32 nextNodeIdx = pEdge->getNextNode(mCurrentNode);
    return mGraph->getNode(nextNodeIdx);
}

RailGraphNode* RailGraphIter::getWatchNode() const
{
    RailGraphEdge *pEdge = mGraph->getEdge(mNextEdge);
    s32 nodeIdx = pEdge->getNextNode(mCurrentNode);
    return mGraph->getNode(nodeIdx);
}

RailGraphEdge* RailGraphIter::getCurrentEdge() const
{
    return mGraph->getEdge(mSelectedEdge);
}

RailGraphEdge* RailGraphIter::getWatchEdge() const
{
    return mGraph->getEdge(mNextEdge);
}