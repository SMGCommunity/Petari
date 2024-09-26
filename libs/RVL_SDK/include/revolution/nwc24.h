#ifndef NWC24_H
#define NWC24_H

#include <revolution/types.h>
#include <revolution.h>

typedef enum NWC24Err
{
    NWC24_OK                    = 0,
    NWC24_ERR_FATAL             = -1,
    NWC24_ERR_FAILED            = -2,
    NWC24_ERR_INVALID_VALUE     = -3,
    NWC24_ERR_NOT_SUPPORTED     = -4,
    NWC24_ERR_NULL              = -5,
    NWC24_ERR_FULL              = -6,
    NWC24_ERR_PROTECTED         = -7,
    NWC24_ERR_OVERFLOW          = -8,
    NWC24_ERR_LIB_NOT_OPENED    = -9,
    NWC24_ERR_LIB_OPENED        = -10,
    NWC24_ERR_NOMEM             = -11,
    NWC24_ERR_CONFIG            = -12,
    NWC24_ERR_NOT_FOUND         = -13,
    NWC24_ERR_BROKEN            = -14,
    NWC24_ERR_DONE              = -15,
    NWC24_ERR_FILE_OPEN         = -16,
    NWC24_ERR_FILE_CLOSE        = -17,
    NWC24_ERR_FILE_READ         = -18,
    NWC24_ERR_FILE_WRITE        = -19,
    NWC24_ERR_FILE_NOEXISTS     = -20,
    NWC24_ERR_FILE_OTHER        = -21,
    NWC24_ERR_MUTEX             = -22,
    NWC24_ERR_ALIGNMENT         = -23,
    NWC24_ERR_FORMAT            = -24,
    NWC24_ERR_STRING_END        = -25,
    NWC24_ERR_BUSY              = -26,
    NWC24_ERR_VER_MISMATCH      = -27,
    NWC24_ERR_HIDDEN            = -28,
    NWC24_ERR_INPROGRESS        = -29,
    NWC24_ERR_NOT_READY         = -30,
    NWC24_ERR_NETWORK           = -31,
    NWC24_ERR_SERVER            = -32,
    NWC24_ERR_CONFIG_NETWORK    = -33,
    NWC24_ERR_ID_NOEXISTS       = -34,
    NWC24_ERR_ID_GENERATED      = -35,
    NWC24_ERR_ID_REGISTERED     = -36,
    NWC24_ERR_ID_CRC            = -37,
    NWC24_ERR_NAND_CORRUPT      = -38,
    NWC24_ERR_DISABLED          = -39,
    NWC24_ERR_INVALID_OPERATION = -40,
    NWC24_ERR_FILE_EXISTS       = -41,
    NWC24_ERR_INTERNAL_IPC      = -42,
    NWC24_ERR_INTERNAL_VF       = -43,
    NWC24_ERR_ID_NOT_REGISTERED = -44,
    NWC24_ERR_VERIFY_SIGNATURE  = -45,
    NWC24_ERR_FILE_BROKEN       = -46,
    NWC24_ERR_INVALID_CHAR      = -47,
    NWC24_ERR_CANCELLED         = -48,
    NWC24_ERR_OLD_SYSTEM        = -49,
    NWC24_ERR_SCRIPT_VERSION    = -50,
    NWC24_ERR_GIVE_UP           = -51,
    NWC24_ERR_OTHER_REGION      = -52
    
} NWC24Err;

void NWC24iRegister(void);
BOOL NWC24iIsAsyncRequestPending(void);

int NWC24IsMsgLibOpened(void);
int NWC24IsMsgLibOpenedByTool(void);
int NWC24IsMsgLibOpenBlocking(void);

NWC24Err NWC24iIoctlResourceManager(const char *, IOSFd, s32, void *, u32, void *, u32);
NWC24Err NWC24iIoctlResourceManagerAsync(const char *, IOSFd, s32, void *, u32, void *, u32, IOSError *);

NWC24Err NWC24iOpenResourceManager(const char *, const char *, IOSFd *, u32);
NWC24Err NWC24iCloseResourceManager(const char *, IOSFd);

NWC24Err NWC24iSetScriptMode(s32);

#include <revolution/nwc24/NWC24MsgObj.h>

#endif // NWC24_H