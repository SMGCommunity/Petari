#pragma once

#include "Game/Map/RailGraph.hpp"
#include "Game/Map/RailGraphEdge.hpp"
#include "Game/Map/RailGraphNode.hpp"

class RailGraphIter {
public:
    RailGraphIter(const RailGraph*);

    RailGraphIter(const RailGraphIter& rIter) NO_INLINE {
        *this = rIter;
    }

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

    /* 0x00 */ const RailGraph* mGraph;
    /* 0x04 */ s32 _4;
    /* 0x08 */ s32 mSelectedEdge;
    /* 0x0C */ s32 mNextEdge;
    /* 0x10 */ s32 _10;
};
