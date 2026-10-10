#pragma once

#include <revolution/types.h>

class JMapInfo;
class TalkMessageInfo;
class TalkNode;

struct MessageInfoBlock {
    /* 0x0 */ u32 mMagic;
    /* 0x4 */ u32 mBlockSize;
    /* 0x8 */ u16 mItemCount;
    /* 0xA */ u16 mItemSize;
    /* 0xC */ u32 _C;
};

struct MessageDataBlock {
    /* 0x0 */ u32 mMagic;
    /* 0x4 */ u32 mBlockSize;
};

struct MessageFlowBlock {
    /* 0x0 */ u32 mMagic;
    /* 0x4 */ u32 mBlockSize;
    /* 0x8 */ u16 mNodeCount;
    /* 0xA */ u16 _A;
    /* 0xC */ u32 _C;
};

struct MessageFLI1Block {
    /* 0x0 */ u32 mMagic;
    /* 0x4 */ u32 mBlockSize;
};

class MessageData {
public:
    MessageData(const char*);

    bool getMessageDirect(TalkMessageInfo*, const char*) const;
    bool getMessage(TalkMessageInfo*, u16, u16) const;
    TalkNode* findNode(const char*) const;
    TalkNode* getNode(u32) const;
    TalkNode* getBranchNode(u32) const;
    bool isValidBranchNode(u32) const;
    u8* getMessageInfoTool(int) const;
    s32 findMessageIndex(const char*) const;

    /* 0x00 */ JMapInfo* mIDTable;
    /* 0x04 */ MessageInfoBlock* mInfoBlock;
    /* 0x08 */ MessageDataBlock* mDataBlock;
    /* 0x0C */ u32 _C;
    /* 0x10 */ MessageFlowBlock* mFlowBlock;
    /* 0x14 */ u16* _14;
    /* 0x18 */ u8* _18;
    /* 0x1C */ MessageFLI1Block* mFLI1Block;
};

class MessageHolder {
public:
    MessageHolder();

    void initSceneData();
    void destroySceneData();
    void initSystemData();
    void initGameData();

    /* 0x00 */ MessageData* mSystemMessageData;
    /* 0x04 */ MessageData* mGameMessageData;
    /* 0x08 */ MessageData* mSceneMessageData;
};

class MessageSystem {
public:
    class Node {};

    struct FlowNodeBranch {};

    struct FlowNodeEvent {
        /* 0x00 */ u8 mFlowType;
        /* 0x01 */ u8 mEventType;
        /* 0x02 */ u16 mBranchID;
        /* 0x04 */ u32 mArg;
    };

    static bool getSystemMessageDirect(TalkMessageInfo*, const char*);
    static bool getGameMessageDirect(TalkMessageInfo*, const char*);
    static bool getLayoutMessageDirect(TalkMessageInfo*, const char*);
    static MessageData* getSceneMessageData();
};
