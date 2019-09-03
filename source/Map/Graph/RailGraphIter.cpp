#include "Map/Graph/RailGraphIter.h"
#include "defines.h"

RailGraphIter::RailGraphIter(const RailGraph *graph)
{
    this->mGraph = graph;
    this->mNextNode = -1;
    this->mSelectedEdge = -1;
    this->mNextEdge = -1;
    this->mCurrentNode = -1;
}

void RailGraphIter::moveNodeNext()
{
    s32 selectedEdge = this->mSelectedEdge;
    this->mCurrentNode = this->mNextNode;
    RailGraphEdge* edge = this->mGraph->getEdge(selectedEdge);
    this->mNextNode = edge->getNextNode(this->mCurrentNode);
}

void RailGraphIter::setNode(s32 nextNode)
{
    this->mNextNode = nextNode;
    this->mCurrentNode = -1;
    this->mSelectedEdge = -1;
    this->mNextEdge = -1;
}

void RailGraphIter::watchStartEdge()
{
    const RailGraph* graph = this->mGraph;
    s32 nextNode = this->mNextNode;
    RailGraphNode* node = graph->getNode(nextNode);
    this->mNextEdge = node->_C;
}

void RailGraphIter::watchNextEdge()
{
    RailGraphEdge* edge = this->mGraph->getEdge(this->mNextEdge);
    this->mNextEdge = edge->getNextEdge(this->mNextNode);
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