#pragma once

#include "Game/Util.hpp"

#define INF1_MAGIC 0x494e4631
#define DAT1_MAGIC 0x44415431
#define FLW1_MAGIC 0x464c5731
#define FLI1_MAGIC 0x464c4931

class TalkNode;
class TalkMessageInfo;

class MessageData {
public:
    MessageData(const char *);

    TalkNode* getNode(u32) const;

    TalkNode* getBranchNode(u32) const;

    bool getMessage(TalkMessageInfo *, u16, u16);

    bool isValidBranchNode(u32) const;

    JMapInfo* mIDTable;     // 0x0
    u8* mInfoBlock;       // 0x4
    u8* mDataBlock;       // 0x8
    u32 _C;
    u8* mFlowBlock;       // 0x10
    u8* _14;
    u8* _18;
    u8* mFLI1Block;       // 0x1C
};

class MessageHolder {
public:
    MessageHolder();

    void initSceneData();
    void destroySceneData();
    void initSystemData();
    void initGameData();

    MessageData* mSystemMessage;        // 0x0
    MessageData* mGameMessage;          // 0x4
    MessageData* mSceneData;            // 0x8
};

class MessageSystem {
public:

    class Node {

    };

    struct FlowNodeBranch {

    };

    struct FlowNodeEvent {
        u8 mFlowType;       // 0x0
        u8 mEventType;      // 0x1
        u16 mBranchID;      // 0x2
        u32 mArg;           // 0x4
    };

    static inline TalkNode* getCurrentBranchNodeFromSceneMsg(u32 idx) {
        return getSceneMessageData()->getBranchNode(idx);
    }

    static MessageData* getSceneMessageData();
};
