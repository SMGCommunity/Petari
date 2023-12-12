#pragma once

#include "Game/System/NANDManagerThread.h"
#include <revolution.h>

class NANDRequestInfo;

typedef void (NANDReqFunc)(NANDRequestInfo *);

class NANDRequestInfo {
public:
    NANDRequestInfo();

    void init();
    bool isDone() const;
    const char* setMove(const char *, const char *);
    const char* setWriteSeq(const char *, const void *, u32, u8, u8);
    const char* setReadSeq(const char *, void *, u32, u32 *);
    const char* setCheck(u32, u32, u32 *);
    const char* setDelete(const char *);

    char mReqStr[0x40];         // _0
    u32 _40;
    s32 mRequestStatus;         // _44
    s32 mRequestResult;         // _48
    const void* _4C;
    void* _50;
    NANDReqFunc* _54;
    u8 mPermission;             // _58
    u8 mAttribute;              // _59
    u8 _5A;
    u8 _5B;
    u32 mFSBlock;               // _5C
    u32 mINode;                 // _60
};

class NANDManager {
public:
    NANDManager();

    bool addRequest(NANDRequestInfo *);

    OSMutex mMutex;                     // _0
    NANDManagerThread* mManager;        // _18
};

class NANDResultCode {
public:
    s32 getCode() const;
    bool isSuccess() const;
    bool isSaveDataCorrupted() const;
    bool isNANDCorrupted() const;
    bool isMaxBlocks() const;
    bool isMaxFiles() const;
    bool isNoExistFile() const;
    bool isBusyOrAllocFailed() const;
    bool isUnknown() const;

private:
    s32 mCode;      // _0
};

namespace MR {
    void addRequestToNANDManager(NANDRequestInfo *);
};