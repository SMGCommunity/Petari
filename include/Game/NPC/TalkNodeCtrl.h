#pragma once

#include "Game/NPC/TalkMessageInfo.h"

class TalkMessageHistory {
public:

    void entry(u16);
    u16 search(u16) const;

    u16 mHistory[0x10];     // _0
    s32 mCount;             // _20
};

class TalkNode {
public:

    u8 mNodeType;       // _0
    u8 mGroupID;        // _1
    s16 mIndex;         // _2
    s16 mNextIdx;       // _4
    u8 mNextGroup;      // _6
    u8 _7;
};

class TalkNodeCtrl {
public:

    TalkNodeCtrl();

    bool isExistNextNode() const;
    bool isNextNodeMessage() const;
    bool isCurrentNodeEvent() const;

    TalkNode* getNextNode() const;
    TalkNode* getNextNodeBranch() const;
    TalkNode* getCurrentNodeBranch() const;
    TalkNode* getCurrentNodeMessage() const;
    TalkNode* getCurrentNodeEvent() const;
    TalkNode* getNextNodeEvent() const;

    u32 _0;
    s32 _4;
    TalkMessageInfo mMessageInfo;   // _8
    TalkMessageHistory mHistory;    // _14
    TalkNode* _38;
    TalkNode* mCurrentNode;         // _3C
    TalkNode* mFlowNode;            // _40
    u16 mNodeData;                  // _44
};