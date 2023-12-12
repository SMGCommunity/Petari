#pragma once

#include "Game/Util.h"

class TalkNode;
class TalkMessageInfo;

class MessageData {
public:
    MessageData(const char *);

    TalkNode* getNode(u32) const;

    TalkNode* getBranchNode(u32) const;

    bool getMessage(TalkMessageInfo *, u16, u16);

    bool isValidBranchNode(u32) const;

    JMapInfo* mIDTable;     // _0
    void* mInfoBlock;       // _4
    void* mDataBlock;       // _8
    u32 _C;
    void* mFlowBlock;       // _10
    u32 _14;
    u32 _18;
    void* mFLI1Block;       // _1C
};

class MessageHolder {
public:
    MessageHolder();

    void initSceneData();
    void destroySceneData();
    void initSystemData();
    void initGameData();

    MessageData* mSystemMessage;        // _0
    MessageData* mGameMessage;          // _4
    MessageData* mSceneData;            // _8
};

class MessageSystem {
public:

    class Node {

    };

    struct FlowNodeBranch {

    };

    struct FlowNodeEvent {
        u8 mFlowType;       // _0
        u8 mEventType;      // _1
        u16 mBranchID;      // _2
        u32 mArg;           // _4
    };

    static inline TalkNode* getCurrentBranchNodeFromSceneMsg(u32 idx) {
        return getSceneMessageData()->getBranchNode(idx);
    }

    static MessageData* getSceneMessageData();
};