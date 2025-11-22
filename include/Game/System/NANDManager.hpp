#pragma once

#include <revolution.h>

class NANDManagerThread;
class NANDRequestInfo;

typedef void(NANDReqFunc)(NANDRequestInfo*);

class NANDRequestInfo {
public:
    NANDRequestInfo();

    void        init();
    bool        isDone() const;
    const char* setMove(const char*, const char*);
    const char* setWriteSeq(const char*, const void*, u32, u8, u8);
    const char* setReadSeq(const char*, void*, u32, u32*);
    const char* setCheck(u32, u32, u32*);
    const char* setDelete(const char*);

    /* 0x00 */ char mPath[NAND_MAX_PATH];
    /* 0x40 */ u32  _40;
    /* 0x44 */ s32  mType;
    /* 0x48 */ s32  mResult;
    /* 0x4C */ union {
        void*       mReadBuf;
        const void* mWriteBuf;
    };
    /* 0x50 */ union {
        const char* mMoveDestDir;
        u32*        mReadLength;
        u32*        mCheckAnswer;
    };
    /* 0x54 */ NANDReqFunc* _54;
    /* 0x58 */ u8           mPermission;
    /* 0x59 */ u8           mAttribute;
    /* 0x5C */ u32          mFsBlock;
    /* 0x60 */ u32          mInode;
    /* 0x64 */ u8           _64[0x1C];
};

class NANDManager {
public:
    NANDManager();

    bool addRequest(NANDRequestInfo*);

private:
    /* 0x00 */ OSMutex            mMutex;
    /* 0x18 */ NANDManagerThread* mManagerThread;
};

class NANDResultCode {
public:
    NANDResultCode(s32 code)
        : mCode(code) {
    }

    s32  getCode() const;
    bool isSuccess() const;
    bool isSaveDataCorrupted() const;
    bool isNANDCorrupted() const;
    bool isMaxBlocks() const;
    bool isMaxFiles() const;
    bool isNoExistFile() const;
    bool isBusyOrAllocFailed() const;
    bool isUnknown() const;

private:
    /* 0x0 */ s32 mCode;
};

namespace MR {
    void addRequestToNANDManager(NANDRequestInfo*);
};
