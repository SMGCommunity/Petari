#include "Game/Map/RailGraphIter.hpp"

RailGraphIter::RailGraphIter(const RailGraph *pGraph) {
    mGraph = pGraph;
    _4 = -1;
    mSelectedEdge = -1;
    mNextEdge = -1;
    _10 = -1;
}

void RailGraphIter::moveNodeNext() {
    s32 selected_edge = mSelectedEdge;
    _10 = _4;
    RailGraphEdge* edge = mGraph->getEdge(selected_edge);
    s32 next_node = edge->getNextNode(_10);
    _4 = next_node;
}

void RailGraphIter::setNode(s32 a1) {
    _4 = a1;
    _10 = -1;
    mSelectedEdge = -1;
    mNextEdge = -1;
}

void RailGraphIter::watchStartEdge() {
    RailGraphNode* node = mGraph->getNode(_4);
    mNextEdge = node->_C;
}

void RailGraphIter::watchNextEdge() {
    RailGraphEdge* edge = mGraph->getEdge(mNextEdge);
    s32 next_edge = edge->getNextEdge(_4);
    mNextEdge = next_edge;
}

bool RailGraphIter::isWatchEndEdge() const {
    return mNextEdge == -1;
}

void RailGraphIter::selectEdge() {
    mSelectedEdge = mNextEdge;
}

void RailGraphIter::selectEdge(s32 edge) {
    mSelectedEdge = edge;
}

bool RailGraphIter::isWatchedPrevEdge() const {
    RailGraphEdge* edge = mGraph->getEdge(mNextEdge);
    return edge->getNextNode(_4) == _10;   
}

bool RailGraphIter::isSelectedEdge() const {
    return mGraph->isValidEdge(mSelectedEdge);
}

RailGraphNode* RailGraphIter::getCurrentNode() const {
    return mGraph->getNode(_4);
}

RailGraphNode* RailGraphIter::getNextNode() const {
    RailGraphEdge* edge = mGraph->getEdge(mSelectedEdge);
    s32 next = edge->getNextNode(_4);
    return mGraph->getNode(next);
}

RailGraphNode* RailGraphIter::getWatchNode() const {
    RailGraphEdge* edge = mGraph->getEdge(mNextEdge);
    s32 next_node = edge->getNextNode(_4);
    return mGraph->getNode(next_node);
}

RailGraphEdge* RailGraphIter::getCurrentEdge() const {
    return mGraph->getEdge(mSelectedEdge);
}

RailGraphEdge* RailGraphIter::getWatchEdge() const {
    return mGraph->getEdge(mNextEdge);
}