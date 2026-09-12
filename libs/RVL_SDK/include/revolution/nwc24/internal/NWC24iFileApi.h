#ifndef RVL_SDK_NWC24_INTERNAL_FILE_API_H
#define RVL_SDK_NWC24_INTERNAL_FILE_API_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

#define NWC24i_VF_DRIVE "@24"

NWC24Err NWC24iFOpenNand(NWC24File* pFile, const char* pPath, u32 mode);
NWC24Err NWC24iFOpenVF(NWC24File* pFile, const char* pPath, u32 mode);

NWC24Err NWC24iFCloseNand(NWC24File* pFile) NO_INLINE;
NWC24Err NWC24iFCloseVF(NWC24File* pFile);

#ifdef __cplusplus
}
#endif
#endif
