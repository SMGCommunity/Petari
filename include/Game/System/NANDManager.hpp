#pragma once

#include "Game/System/NANDManagerThread.hpp"
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

    char mReqStr[0x40];         // 0x0
    u32 _40;
    s32 mRequestStatus;         // 0x44
    s32 mRequestResult;         // 0x48
    const void* _4C;
    void* _50;
    NANDReqFunc* _54;
    u8 mPermission;             // 0x58
    u8 mAttribute;              // 0x59
    u8 _5A;
    u8 _5B;
    u32 mFSBlock;               // 0x5C
    u32 mINode;                 // 0x60
};

class NANDManager {
public:
    NANDManager();

    bool addRequest(NANDRequestInfo *);

    OSMutex mMutex;                     // 0x0
    NANDManagerThread* mManager;        // 0x18
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
    s32 mCode;      // 0x0
};

namespace MR {
    void addRequestToNANDManager(NANDRequestInfo *);
};