#pragma once

#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"

class RecursiveHelper;

class TalkMessageHistory {
public:

    void entry(u16);
    bool search(u16) const;

    u16 mHistory[0x10];     // 0x0
    s32 mCount;             // 0x20
};

class TalkNode {
public:

    u8 mNodeType;       // 0x0
    u8 mGroupID;        // 0x1
    u16 mIndex;         // 0x2
    u16 mNextIdx;       // 0x4
    u16 mNextGroup;      // 0x6
};

class MessageNode {
public:
    const char* mMessage;   // 0x0
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
    s32 mCurrentNodeIdx;            // 0x4
    TalkMessageInfo mMessageInfo;   // 0x8
    TalkMessageHistory mHistory;    // 0x14
    TalkNode* _38;
    TalkNode* mCurrentNode;         // 0x3C
    TalkNode* mFlowNode;            // 0x40
    s16 mNodeData;                  // 0x44
};