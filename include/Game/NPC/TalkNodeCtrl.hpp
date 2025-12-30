#pragma once

#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"

class TalkMessageHistory {
public:
    void entry(u16);
    bool search(u16) const;

    /* 0x0 */ u16 mHistory[0x10];
    /* 0x20 */ s32 mCount;
};

class TalkNode {
public:
    /* 0x0 */ u8 mNodeType;
    /* 0x1 */ u8 mGroupID;
    /* 0x2 */ u16 mIndex;
    union {
        /* 0x4 */ u32 mUnknown;
        struct {
            /* 0x4 */ u16 mNextIdx;
            /* 0x6 */ u16 mNextGroup;
        };
    };
};

class RecursiveHelper {
public:
    inline bool hasNode(const TalkNode*) const;
    TalkNode* mStack[64];
    s32 mIndex;
};

class MessageNode {
public:
    const char* mMessage;  // 0x0
};

class TalkNodeCtrl {
public:
    TalkNodeCtrl();

    void createFlowNodeDirect(TalkMessageCtrl*, const JMapInfoIter&, const char*, ActorCameraInfo**);
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
    void createFlowNode(TalkMessageCtrl*, const JMapInfoIter&, const char*, ActorCameraInfo**);
    void initNodeRecursive(TalkMessageCtrl*, const JMapInfoIter&, ActorCameraInfo*, RecursiveHelper*);

    char* _0;
    /* 0x4 */ s32 mCurrentNodeIdx;
    /* 0x8 */ TalkMessageInfo mMessageInfo;
    /* 0x14 */ TalkMessageHistory mHistory;
    TalkNode* _38;
    /* 0x3C */ TalkNode* mCurrentNode;
    /* 0x40 */ TalkNode* mFlowNode;
    /* 0x44 */ s16 mNodeData;
};
