#pragma once

#include "Map/Graph/RailGraphEdge.h"
#include "Map/Graph/RailGraphNode.h"

class RailGraph
{
public:
    RailGraph();

    s32 addNode(const JGeometry::TVec3<f32> &);
    void connectNodeTwoWay(s32, s32, const RailGraphEdge *);
    RailGraphNode* getNode(s32) const;
    RailGraphEdge* getEdge(s32) const;
    bool isValidEdge(s32) const;
    void connectEdgeToNode(s32, s32);

    RailGraphNode* mNodes; // _0
    u32 _4;
    u32 mNodeCount; // _8
    RailGraphEdge* mEdges; // _C
    u32 _10;
    u32 mEdgeCount; // _14
};