#include "Map/Graph/RailGraphIter.h"
#include "defines.h"

RailGraphIter::RailGraphIter(const RailGraph *graph)
{
    this->mGraph = graph;
    this->mCurrentNode = -1;
    this->mSelectedEdge = -1;
    this->mNextEdge = -1;
    this->mNextNode = -1;
}

void RailGraphIter::moveNodeNext()
{
    s32 selectedEdge = this->mSelectedEdge;
    this->mNextNode = this->mCurrentNode;
    RailGraphEdge* edge = this->mGraph->getEdge(selectedEdge);
    this->mCurrentNode = edge->getNextNode(this->mNextNode);
}

void RailGraphIter::setNode(s32 nextNode)
{
    this->mCurrentNode = nextNode;
    this->mNextNode = -1;
    this->mSelectedEdge = -1;
    this->mNextEdge = -1;
}

void RailGraphIter::watchStartEdge()
{
    const RailGraph* graph = this->mGraph;
    s32 nextNode = this->mCurrentNode;
    RailGraphNode* node = graph->getNode(nextNode);
    this->mNextEdge = node->_C;
}

void RailGraphIter::watchNextEdge()
{
    RailGraphEdge* edge = this->mGraph->getEdge(this->mNextEdge);
    this->mNextEdge = edge->getNextEdge(this->mCurrentNode);
}

bool RailGraphIter::isWatchEndEdge() const
{
    return (bool)(__cntlzw(this->mNextEdge + 1) >> 5);
}

void RailGraphIter::selectEdge()
{
   this->mSelectedEdge = this->mNextEdge; 
}

void RailGraphIter::selectEdge(s32 edge)
{
    this->mSelectedEdge = edge;
}

bool RailGraphIter::isWatchedPrevEdge() const
{
    RailGraphEdge* edge = this->mGraph->getEdge(this->mNextEdge);
    s32 nextNode = edge->getNextNode(this->mCurrentNode);
    return __cntlzw(nextNode - this->mNextNode) >> 5; 
}

bool RailGraphIter::isSelectedEdge() const
{
    return this->mGraph->isValidEdge(this->mSelectedEdge);
}

RailGraphNode* RailGraphIter::getCurrentNode() const
{
    return this->mGraph->getNode(this->mCurrentNode);
}

RailGraphNode* RailGraphIter::getNextNode() const
{
    RailGraphEdge* edge = this->mGraph->getEdge(this->mSelectedEdge);
    s32 nextNodeIdx = edge->getNextNode(this->mCurrentNode);
    return this->mGraph->getNode(nextNodeIdx);
}

RailGraphNode* RailGraphIter::getWatchNode() const
{
    RailGraphEdge* edge = this->mGraph->getEdge(this->mNextEdge);
    s32 nodeIdx = edge->getNextNode(this->mCurrentNode);
    return this->mGraph->getNode(nodeIdx);
}

RailGraphEdge* RailGraphIter::getCurrentEdge() const
{
    return this->mGraph->getEdge(this->mSelectedEdge);
}

RailGraphEdge* RailGraphIter::getWatchEdge() const
{
    return this->mGraph->getEdge(this->mNextEdge);
}