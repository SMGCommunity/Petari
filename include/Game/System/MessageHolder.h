#pragma once

#include "Game/Util.h"

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
};

class MessageData {
public:
    MessageData(const char *);

    JMapInfo* mIDTable;     // _0
    void* mInfoBlock;       // _4
    void* mDataBlock;       // _8

    void* mFlowBlock;       // _10

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