#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class RailGraphIter;
class RailGraphNode;
class RailGraphEdge;

class RailGraph {
public:
    RailGraph();

    s32 addNode(const TVec3f&);
    void connectNodeTwoWay(s32, s32, const RailGraphEdge*);
    RailGraphNode* getNode(s32) const;
    RailGraphEdge* getEdge(s32) const;
    bool isValidEdge(s32) const;
    void connectEdgeToNode(s32, s32);
    RailGraphIter getIterator() const;

    RailGraphNode* mNodes;  // 0x0
    s32 mNodeCount;         // 0x4
    u32 _8;
    RailGraphEdge* mEdges;  // 0xC
    s32 mEdgeCount;         // 0x10
    s32 _14;
};