#ifndef OSCACHE_H
#define OSCACHE_H

#include <revolution/types.h>
#include <revolution/os/OSContext.h>
#include <revolution/os/OSError.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LC_MAX_DMA_BLOCKS   (128)
#define LC_MAX_DMA_BYTES    (LC_MAX_DMA_BLOCKS * 32)

void DCInvalidateRange(void *, u32);

void DCEnable(void);
void DCInvalidate(void *, u32);
void DCFlushRange(void *, u32);
void DCStoreRange(void *, u32);
void DCFlushRangeNoSync(void *, u32);
void DCStoreRangeNoSync(void *, u32);
void DCZeroRange(void *, u32);

void ICInvalidateRange(void *, u32);
void ICFlashInvalidate(void);
void ICEnable(void);

void LCEnable(void);
void LCDisable(void);
void LCStoreBlocks(void *, void *, u32);
u32 LCStoreData(void *, void *, u32);
void LCQueueWait(u32);

void DMAErrorHandler(OSError, OSContext *, ...);

void __OSCacheInit(void);

#ifdef __cplusplus
}
#endif

#endif // OSCACHE_H