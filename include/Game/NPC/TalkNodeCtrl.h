#pragma once

#include "Game/NPC/TalkMessageInfo.h"
#include "Game/NPC/TalkMessageCtrl.h"

class RecursiveHelper;

class TalkMessageHistory {
public:

    void entry(u16);
    bool search(u16) const;

    u16 mHistory[0x10];     // _0
    s32 mCount;             // _20
};

class TalkNode {
public:

    u8 mNodeType;       // _0
    u8 mGroupID;        // _1
    u16 mIndex;         // _2
    u16 mNextIdx;       // _4
    u16 mNextGroup;      // _6
};

class MessageNode {
public:
    const char* mMessage;   // _0
};

class TalkNodeCtrl {
public:
    TalkNodeCtrl();

    void createFlowNodeDirect(TalkMessageCtrl *, const JMapInfoIter &, const char *, ActorCameraInfo **);
    void resetFlowNode();
    void resetTempFlowNode();
    void recordTempFlowNode();
    void forwardFlowNode();
    bool isExistNextNode() const;
    bool isNextNodeMessage() const;
    bool isCurrentNodeEvent() const;
    TalkNode* getNextNode() const;
    TalkNode* getNextNodeBranch() const;
    TalkNode* getCurrentNodeBranch() const;
    TalkNode* getCurrentNodeMessage() const;
    TalkNode* getCurrentNodeEvent() const;
    TalkNode* getNextNodeEvent() const;
    void updateMessage();
    void readMessage();
    const wchar_t* getSubMessage() const;
    void forwardCurrentBranchNode(bool);
    void createFlowNode(TalkMessageCtrl *, const JMapInfoIter &, const char *, ActorCameraInfo **);
    void initNodeRecursive(TalkMessageCtrl *, const JMapInfoIter &, ActorCameraInfo *, RecursiveHelper *);

    u32 _0;
    s32 mCurrentNodeIdx;            // _4
    TalkMessageInfo mMessageInfo;   // _8
    TalkMessageHistory mHistory;    // _14
    TalkNode* _38;
    TalkNode* mCurrentNode;         // _3C
    TalkNode* mFlowNode;            // _40
    s16 mNodeData;                  // _44
};