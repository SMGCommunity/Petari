#ifndef RVL_SDK_NWC24_MIME_H
#define RVL_SDK_NWC24_MIME_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NWC24_BASE64_TABLE_SIZE 256

void NWC24InitBase64Table(char* pTable);

NWC24Err NWC24Base64Encode(const u8* pData, s32 dataSize, char* pText, s32 textSize, u32* pTextWritten);

NWC24Err NWC24EncodeQuotedPrintable(char* pText, u32 textSize, u32* pTextWritten, const u8* pData, u32 dataSize, u32* pDataRead);

#ifdef __cplusplus
}
#endif
#endif
