#pragma once

#include "Game/Map/RailGraph.h"
#include "Game/Map/RailGraphEdge.h"
#include "Game/Map/RailGraphNode.h"

class RailGraphIter {
public:
    RailGraphIter(const RailGraph *);

    void moveNodeNext();
    void setNode(s32);
    void watchStartEdge();
    void watchNextEdge();
    bool isWatchEndEdge() const;
    void selectEdge();
    void selectEdge(s32);
    bool isWatchedPrevEdge() const;
    bool isSelectedEdge() const;
    RailGraphNode* getCurrentNode() const;
    RailGraphNode* getNextNode() const;
    RailGraphNode* getWatchNode() const;
    RailGraphEdge* getCurrentEdge() const;
    RailGraphEdge* getWatchEdge() const;

    const RailGraph* mGraph;    // _0
    s32 _4;
    s32 mSelectedEdge;          // _8
    s32 mNextEdge;              // _C
    s32 _10;
};