#include "Game/Map/RailGraph.hpp"
#include "Game/Map/RailGraphEdge.hpp"
#include "Game/Map/RailGraphIter.hpp"
#include "Game/Map/RailGraphNode.hpp"

RailGraph::RailGraph() {
    mNodes = 0;
    mNodeCount = 0;
    _8 = 0;
    mEdges = 0;
    mEdgeCount = 0;
    _14 = 0;

    mNodes = new RailGraphNode[0x100];
    mNodeCount = 0x100;
    mEdges = new RailGraphEdge[0x100];
    mEdgeCount = 0x200;
}

s32 RailGraph::addNode(const TVec3f &rVec) {
    RailGraphNode node;
    node._0.x = rVec.x;
    node._0.y = rVec.y;
    node._0.z = rVec.z;
    s32 cnt = _8;
    _8 = cnt + 1;

    RailGraphNode* nodes = &mNodes[cnt];
    nodes->_0.setPS(node._0);
    nodes->_C = node._C;
    nodes->_10 = node._10;
    return _8 - 1;
} 

void RailGraph::connectNodeTwoWay(s32 a1, s32 a2, const RailGraphEdge *pEdge) {
    RailGraphEdge edge;

    if (pEdge) {
        edge = *pEdge;
    }

    edge.clearConnectInfo();
    edge._4 = a1;
    edge._8 = a2;
    edge.mDistance = PSVECDistance((const Vec*)&mNodes[a1]._0, (const Vec*)&mNodes[a2]._0);
    s32 cnt = _14;
    _14 = cnt + 1;
    mEdges[cnt] = edge;
    s32 edgeIdx = _14 - 1;
    connectEdgeToNode(a1, edgeIdx);
    connectEdgeToNode(a2, edgeIdx);
}

RailGraphNode* RailGraph::getNode(s32 idx) const {
    return &mNodes[idx];
}

RailGraphEdge* RailGraph::getEdge(s32 idx) const {
    return &mEdges[idx];
}

bool RailGraph::isValidEdge(s32 edgeIdx) const {
    bool ret = false;

    if (edgeIdx >= 0 && edgeIdx < _14) {
        ret = true;
    }

    return ret;
}

void RailGraph::connectEdgeToNode(s32 a1, s32 a2) {
    RailGraphNode* node = &mNodes[a1];
    s32 val = node->_C;

    if (val == -1) {
        node->_C = a2;
    }
    else {
        s32 next_edge = mEdges[val].getNextEdge(a1);

        while (next_edge != -1) {
            val = next_edge;
            next_edge = mEdges[next_edge].getNextEdge(a1);
        }

        mEdges[val].setNextEdge(a2, a1);
    }

    mNodes[a1]._10++;
}

RailGraphIter RailGraph::getIterator() const {
    return RailGraphIter(this);
}
