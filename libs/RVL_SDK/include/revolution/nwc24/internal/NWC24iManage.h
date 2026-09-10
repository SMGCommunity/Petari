#ifndef RVL_SDK_NWC24_INTERNAL_MANAGE_H
#define RVL_SDK_NWC24_INTERNAL_MANAGE_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Mime.h>
#include <revolution/nwc24/NWC24Types.h>
#include <revolution/nwc24/internal/NWC24iConfig.h>
#include <revolution/nwc24/internal/NWC24iDownload.h>
#include <revolution/nwc24/internal/NWC24iFriendList.h>
#include <revolution/nwc24/internal/NWC24iMBoxCtrl.h>
#include <revolution/nwc24/internal/NWC24iSecretFList.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NWC24i_IO_BUFFER_SIZE 512
#define NWC24i_STRING_WORK_SIZE 1024
#define NWC24i_PATH_WORK_SIZE 256

#define NWC24i_MANAGE_ERROR_CODE_BASE 109000
#define NWC24i_MSG_ERROR_CODE_BASE 109300

#define NWC24i_APP_ID_IPL FOURCC('H', 'A', 'E', 'A')

typedef enum NWC24iMsgArrivedFlags {
    NWC24_MSG_ARRIVED = 1 << 0,
    NWC24_MSG_ARRIVED_FOR_MENU = 1 << 1,
} NWC24iMsgArrivedFlags;

#define WORK_SIZE(x) (ROUND_UP(sizeof(x), 0x100))
typedef struct NWC24iWork {
    char stringWork[NWC24i_STRING_WORK_SIZE];
    char WORK_0x400[0x800 - 0x400];
    char pathWork[NWC24i_PATH_WORK_SIZE];
    u8 readBuffer[NWC24i_IO_BUFFER_SIZE];
    u8 writeBuffer[2048];
    u8 config[WORK_SIZE(NWC24iConfig)];
    NWC24iMBCHeader sendCtrl;
    NWC24iMBCHeader recvCtrl;
    NWC24iMBCEntry mbcEntry;
    char WORK_0x1280[128];
    char base64Work[NWC24_BASE64_TABLE_SIZE];
    char WORK_0x1400[0x2400 - 0x1400];
    u8 flHeader[WORK_SIZE(NWC24iFLHeader)];
    u8 secretFlHeader[WORK_SIZE(NWC24iSecretFLHeader)];
    u8 dlHeader[WORK_SIZE(NWC24iDlHeader)];
    u8 dlTask[WORK_SIZE(NWC24iDlTask)];

} NWC24iWork;
#undef WORK_SIZE

extern NWC24iWork* NWC24WorkP;

void NWC24iRegister(void);
NWC24Err NWC24iSetNewMsgArrived(u32 flags);
void NWC24iSetErrorCode(u32 code);

#ifdef __cplusplus
}
#endif
#endif
