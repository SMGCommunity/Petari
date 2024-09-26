#ifndef RVL_FACE_LIBRARY_INTERNAL_CONTROLLER_H
#define RVL_FACE_LIBRARY_INTERNAL_CONTROLLER_H
#include <RFL_MiddleDatabase.h>
#include <RFLi_Types.h>
#include <revolution/card.h>
#include <revolution/mem.h>
#include <revolution/wpad.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define RFLi_CTRL_REPLACE_BUF_NUM 2

typedef struct RFLiCtrlBuf {
    u32 identifier;                       // at 0x0
    u16 secretFlag;                       // at 0x4
    u16 padding;                          // at 0x6
    RFLiCharData data[RFL_CTRL_CHAR_MAX]; // at 0x8
    u16 deleted;                          // at 0x2EC
    u16 checksum;                         // at 0x2EE
} RFLiCtrlBuf;

typedef struct RFLiCtrlWriteDeleteList {
    u8 deleted[100];
} RFLiCtrlWriteDeleteList;

typedef struct RFLiCtrlBufManager {
    RFLiCtrlBuf* buffer[4];          // at 0x0
    BOOL loaded[4];                  // at 0x10
    RFLiCtrlBuf* replaceBuf[RFLi_CTRL_REPLACE_BUF_NUM]; // at 0x20
    BOOL read;                                          // at 0x28
    CARDFileInfo file;                                  // at 0x2C
    void* tempBuffer;                                   // at 0x40
    void* readBuffer;                                   // at 0x44
    void* writeBuffer;                                  // at 0x48
    u16 writeSize;                                      // at 0x4C
    u16 writeOffset;                                    // at 0x4E
    u16 deleted;                                        // at 0x50
    s32 writeChan;                                      // at 0x54
    const RFLiCtrlWriteDeleteList* writeDeleteList;     // at 0x58
    u16 readSize;                                       // at 0x5C
    u16 currPos;                                        // at 0x5E
    BOOL readIsChMode;                                  // at 0x60
    u8 writeCount;                                      // at 0x64
    u16 rwLen;                                          // at 0x66
    u8 retry;                                           // at 0x68
    u8* verifyBuffer;                                   // at 0x6C
    RFLMiddleDB hiddenMDB;                              // at 0x0
} RFLiCtrlBufManager;

void RFLiInitCtrlBuf(MEMiHeapHead* heap);
BOOL RFLiCheckCtrlBufferCore(const RFLiCtrlBuf* buf, u8 index,
                             RFLiHiddenType type);
RFLErrcode RFLiLoadControllerAsync(s32 chan, BOOL ch);
BOOL RFLiGetControllerData(RFLiCharInfo* info, s32 chan, u16 index,
                           BOOL allowHidden);

#ifdef __cplusplus
}
#endif
#endif
