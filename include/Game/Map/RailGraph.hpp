#pragma once

#include <JSystem/JGeometry.hpp>
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

    /* 0x00 */ RailGraphNode* mNodes;
    /* 0x04 */ s32 mNodeCount;
    /* 0x08 */ u32 _8;
    /* 0x0C */ RailGraphEdge* mEdges;
    /* 0x10 */ s32 mEdgeCount;
    /* 0x14 */ s32 _14;
};
