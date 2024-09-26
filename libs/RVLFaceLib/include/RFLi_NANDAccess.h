#ifndef RVL_FACE_LIBRARY_INTERNAL_NAND_ACCESS_H
#define RVL_FACE_LIBRARY_INTERNAL_NAND_ACCESS_H
#include <RFLi_Types.h>
#include <revolution/FS.h>
#include <revolution/MEM.h>
#include <revolution/NAND.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLiCallbackTag {
    RFLiAsyncTag tag;  // at 0x0
    RFLiFileType type; // at 0x4
} RFLiCallbackTag;

typedef struct RFLiAccessInfo {
    RFLiCallback callback; // at 0x0
    union {
        struct {
            char path[64 + 1]; // at 0x4
            u8 openMode;                // at 0x46
            u8 perm;                    // at 0x47
            u8 attr;                    // at 0x48
        } openInfo;

        struct {
            void* dst;  // at 0x4
            u32 size;   // at 0x8
            s32 offset; // at 0xC
        } readInfo;

        struct {
            const void* src; // at 0x4
            u32 size;        // at 0x8
            s32 offset;      // at 0xC
        } writeInfo;

        struct {
            u8 count; // at 0x4
        } closeInfo;

        struct {
            u32* dst; // at 0x4
        } getLengthInfo;
    };
    NANDFileInfo file;            // at 0x48
    NANDCommandBlock block;       // at 0xD4
    RFLiCallbackTag tag;          // at 0x190
    OSAlarm alarm;                // at 0x198
    RFLiFileType alarmData;       // at 0x1C8
    RFLiExCallback retryCallback; // at 0x1CC
    u8 retryCount;                // at 0x1D0
    void* safeBuffer;             // at 0x1D4
    u8 opened;                    // at 0x1D8
} RFLiAccessInfo;

void RFLiInitAccessInfo(MEMiHeapHead* heap);
void RFLiExitAccessInfo(void);
BOOL RFLiIsWorking(void);
void RFLiStartWorking(void);
void RFLiEndWorkingReason(RFLErrcode err, s32 reason);
void RFLiEndWorking(RFLErrcode err);
NANDCommandBlock* RFLiSetCommandBlock(RFLiFileType type, RFLiAsyncTag tag);
RFLiFileType RFLiGetType(NANDCommandBlock* block);
NANDFileInfo* RFLiGetWorkingFile(RFLiFileType type);
RFLErrcode RFLiOpenAsync(RFLiFileType type, u8 openMode, RFLiCallback callback);
RFLErrcode RFLiReadAsync(RFLiFileType type, void* dst, u32 size,
                         RFLiCallback callback, s32 offset);
RFLErrcode RFLiWriteAsync(RFLiFileType type, const void* src, u32 size,
                          RFLiCallback callback, s32 offset);
RFLErrcode RFLiCloseAsync(RFLiFileType type, RFLiCallback callback);
RFLErrcode RFLiGetLengthAsync(RFLiFileType type, u32* out,
                              RFLiCallback callback);
RFLErrcode RFLiDeleteAsync(RFLiFileType type, RFLiCallback callback);
RFLErrcode RFLiCreateSaveDirAsync(RFLiCallback callback);

#ifdef __cplusplus
}
#endif
#endif
