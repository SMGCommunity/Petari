#include "revolution/nwc24/NWC24Manage.h"
#include "revolution/nwc24/NWC24MBoxCtrl.h"
#include "revolution/nwc24/NWC24Mime.h"
#include "revolution/nwc24/NWC24Schedule.h"
#include <revolution/nwc24.h>
#include <revolution/os.h>
#include <revolution/vf.h>
#include <stdint.h>

#define MANAGE_ERROR_CODE_BASE 109000

typedef enum { NWC24_LIB_CLOSED, NWC24_LIB_OPENED, NWC24_LIB_OPENED_BY_TOOL, NWC24_LIB_BLOCKED } NWC24LibState;

typedef enum { NWC24_FAIL_SFL = (1 << 0), NWC24_FAIL_DL_TASK = (1 << 1), NWC24_FAIL_FATAL = (1 << 2) } NWC24FailFlag;

static NWC24LibState Opened = NWC24_LIB_CLOSED;
static u32 YouGotMail = 0;
static u32 GlobalErrorCode = 0;
static BOOL Registered = FALSE;

const char* __NWC24Version = "<< RVL_SDK - NWC24 \trelease build: Dec 10 2007 10:02:25 (0x4199_60831) >>";

void NWC24iRegister(void) {
    if (Registered == 0) {
        OSRegisterVersion(__NWC24Version);
        Registered = 1;
    }
}

int NWC24IsMsgLibOpened(void) {
    return Opened == 1;
}

static NWC24Err NWC24OpenLibInternal(NWC24Work* work, NWC24LibState state);

NWC24Err NWC24OpenLib(NWC24Work* work) {
    if (Opened == 2) {
        return -0x1a;
    }
    return NWC24OpenLibInternal(work, 1);
}

static NWC24Err NWC24OpenLibInternal(NWC24Work* work, NWC24LibState state) {
    NWC24Err result;
    NWC24Err failErr;
    u32 failFlag;

    NWC24iSetErrorCode(NWC24_OK);

    if (!VFIsAvailable()) {
        return NWC24_ERR_FATAL;
    }

    if (NWC24IsMsgLibOpened()) {
        return NWC24_ERR_LIB_OPENED;
    }

    if (NWC24IsMsgLibOpenBlocking()) {
        return NWC24_ERR_BUSY;
    }

    if (work == NULL) {
        return NWC24_ERR_NULL;
    }

    if ((uintptr_t)work % 32 != 0) {
        return NWC24_ERR_ALIGNMENT;
    }

    result = NWC24iTrySuspendForOpenLib();
    if (result == NWC24_OK) {
        NWC24iRegister();

        YouGotMail &= ~(1 << NWC24_MSGTYPE_RVL_MENU_SHARED);
        NWC24WorkP = work;

        NWC24InitBase64Table(NWC24WorkP->base64Work);

        failFlag = 0;
        failErr = NWC24_OK;

        result = NWC24iConfigOpen();
        if (result != NWC24_OK) {
            failErr = result;
            failFlag |= NWC24_FAIL_FATAL;
        }

        result = NWC24iOpenMBox();
        if (result != NWC24_OK) {
            failErr = result;
            failFlag |= NWC24_FAIL_FATAL;
        }

        result = NWC24iOpenFriendList();
        if (result != NWC24_OK) {
            failErr = result;
            failFlag |= NWC24_FAIL_FATAL;
        }

        result = NWC24iOpenSecretFriendList();
        if (result != NWC24_OK) {
            failErr = result;

            if (result == NWC24_ERR_FILE_NOEXISTS) {
                failFlag |= NWC24_FAIL_FATAL;
            } else {
                failFlag |= NWC24_FAIL_SFL;
            }
        }

        result = NWC24iOpenDlTaskList();
        if (result < 0) {
            failErr = result;

            if (result == NWC24_ERR_FILE_NOEXISTS) {
                failFlag |= NWC24_FAIL_FATAL;
            } else {
                failFlag |= NWC24_FAIL_DL_TASK;
            }
        }

        if (failFlag == (NWC24_FAIL_SFL | NWC24_FAIL_DL_TASK)) {
            failErr = NWC24_ERR_OLD_SYSTEM;
        }

        if (failFlag != 0) {
            NWC24WorkP = NULL;
            NWC24iResumeForCloseLib();
            result = failErr;
        } else {
            Opened = state;
            return NWC24_OK;
        }
    }

    switch (result) {
    case NWC24_ERR_OLD_SYSTEM:
    case NWC24_ERR_FILE_BROKEN:
    case NWC24_ERR_INTERNAL_VF:
    case NWC24_ERR_INTERNAL_IPC:
    case NWC24_ERR_NAND_CORRUPT:
    case NWC24_ERR_INPROGRESS:
    case NWC24_ERR_BUSY:
    case NWC24_ERR_MUTEX:
    case NWC24_ERR_FILE_OTHER:
    case NWC24_ERR_FILE_NOEXISTS:
    case NWC24_ERR_FILE_WRITE:
    case NWC24_ERR_FILE_READ:
    case NWC24_ERR_FILE_CLOSE:
    case NWC24_ERR_FILE_OPEN:
    case NWC24_ERR_BROKEN:
    case NWC24_ERR_FATAL: {
        NWC24iSetErrorCode(result - MANAGE_ERROR_CODE_BASE);
        break;
    }

    default: {
        break;
    }
    }

    return result;
}

NWC24Err NWC24CloseLib(void) {
    s32 result;

    if (Opened != NWC24_LIB_OPENED) {
        return NWC24_ERR_LIB_NOT_OPENED;
    }

    result = NWC24iConfigFlush();
    if (result != NWC24_OK) {
        return result;
    }

    result = NWC24iCloseDlTaskList();
    if (result < 0) {
        return result;
    }

    result = NWC24iResumeForCloseLib();
    if (result != NWC24_OK) {
        return result;
    }

    NWC24WorkP = NULL;
    Opened = NWC24_LIB_CLOSED;
    return result;
}

int NWC24IsMsgLibOpenedByTool(void) {
    return Opened == 2;
}

int NWC24IsMsgLibOpenBlocking(void) {
    return Opened == 3;
}

NWC24Err NWC24BlockOpenMsgLib(BOOL block) {
    s32 result;
    BOOL enabled;

    result = NWC24_OK;
    enabled = OSDisableInterrupts();

    if (block) {
        if (Opened == NWC24_LIB_CLOSED) {
            Opened = NWC24_LIB_BLOCKED;
        } else if (Opened == NWC24_LIB_OPENED) {
            result = NWC24_ERR_LIB_OPENED;
        } else {
            result = NWC24_ERR_BUSY;
        }
    } else {
        if (Opened == NWC24_LIB_BLOCKED) {
            Opened = NWC24_LIB_CLOSED;
        } else {
            result = NWC24_ERR_LIB_NOT_OPENED;
        }
    }

    OSRestoreInterrupts(enabled);
    return result;
}

NWC24Err NWC24iSetNewMsgArrived(u32 flags) {
    YouGotMail |= flags;
    return NWC24_OK;
}

u32 NWC24GetErrorCode() {
    return GlobalErrorCode;
}

void NWC24iSetErrorCode(u32 code) {
    GlobalErrorCode = code;
}
