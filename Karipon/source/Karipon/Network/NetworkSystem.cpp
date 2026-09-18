#include "Karipon/Network/NetworkSystem.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/SystemUtil.hpp"
#include "private/iostypes.h"
#include "revolution/ipc/ipcclt.h"
#include "revolution/os.h"
#include "revolution/os/OSInterrupt.h"
#include <cstring>
#include <mem.h>

/*

Based on:
- https://github.com/dolphin-emu/dolphin/
- https://github.com/devkitPro/libogc
- https://github.com/jmlee337/Nintendont/

*/

#define RETRY_TIMES 5
#define RETRY_TICKS OSSecondsToTicks(1)

#define NCD_IOS_PATH "/dev/net/ncd/manage"
#define NWC24_IOS_PATH "/dev/net/kd/request"
#define SO_IOS_PATH "/dev/net/ip/top"

enum NCDIoctl {
    IOCTLV_NCD_LOCK_WIRELESS_DRIVER = 0x01,
    IOCTLV_NCD_UNLOCK_WIRELESS_DRIVER = 0x02,
    IOCTLV_NCD_GET_CONFIG = 0x03,
    IOCTLV_NCD_SET_CONFIG = 0x04,
    IOCTLV_NCD_READ_CONFIG = 0x05,
    IOCTLV_NCD_WRITE_CONFIG = 0x06,
    IOCTLV_NCD_GET_LINK_STATUS = 0x07,
    IOCTLV_NCD_GET_WIRELESS_MAC_ADDRESS = 0x08,
};

enum NWC24Ioctl {
    IOCTL_NWC24_SUSPEND_SCHEDULER = 0x01,
    IOCTL_NWC24_EXEC_TRY_SUSPEND_SCHEDULER = 0x02,
    IOCTL_NWC24_EXEC_RESUME_SCHEDULER = 0x03,
    IOCTL_NWC24_KD_GET_TIME_TRIGGERS = 0x04,
    IOCTL_NWC24_SET_SCHEDULE_SPAN = 0x05,
    IOCTL_NWC24_STARTUP_SOCKET = 0x06,
    IOCTL_NWC24_CLEANUP_SOCKET = 0x07,
    IOCTL_NWC24_LOCK_SOCKET = 0x08,
    IOCTL_NWC24_UNLOCK_SOCKET = 0x09,
    IOCTL_NWC24_CHECK_MAIL_NOW = 0x0A,
    IOCTL_NWC24_SEND_MAIL_NOW = 0x0B,
    IOCTL_NWC24_RECEIVE_MAIL_NOW = 0x0C,
    IOCTL_NWC24_SAVE_MAIL_NOW = 0x0D,
    IOCTL_NWC24_DOWNLOAD_NOW_EX = 0x0E,
    IOCTL_NWC24_REQUEST_GENERATED_USER_ID = 0x0F,
    IOCTL_NWC24_REQUEST_REGISTER_USER_ID = 0x10,
    IOCTL_NWC24_GET_SCHEDULER_STAT = 0x1E,
    IOCTL_NWC24_SET_FILTER_MODE = 0x1F,
    IOCTL_NWC24_SET_DEBUG_MODE = 0x20,
    IOCTL_NWC24_KD_SET_NEXT_WAKEUP = 0x21,
    IOCTL_NWC24_SET_SCRIPT_MODE = 0x22,
    IOCTL_NWC24_REQUEST_SHUTDOWN = 0x28,
};

enum SOIoctl {
    IOCTL_SO_ACCEPT = 0x01,
    IOCTL_SO_BIND = 0x02,
    IOCTL_SO_CLOSE = 0x03,
    IOCTL_SO_CONNECT = 0x04,
    IOCTL_SO_FCNTL = 0x05,
    IOCTL_SO_GET_PEER_NAME = 0x06,
    IOCTL_SO_GET_SOCK_NAME = 0x07,
    IOCTL_SO_GET_SOCK_OPT = 0x08,
    IOCTL_SO_SET_SOCK_OPT = 0x09,
    IOCTL_SO_LISTEN = 0x0A,
    IOCTL_SO_POLL = 0x0B,
    IOCTLV_SO_RECV_FROM = 0x0C,
    IOCTLV_SO_SEND_TO = 0x0D,
    IOCTL_SO_SHUTDOWN = 0x0E,
    IOCTL_SO_SOCKET = 0x0F,
    IOCTL_SO_GET_HOST_ID = 0x10,
    IOCTL_SO_GET_HOST_BY_NAME = 0x11,
    IOCTL_SO_GET_HOST_BY_ADDR = 0x12,
    IOCTLV_SO_GET_NAME_INFO = 0x13,
    IOCTL_SO_GET_LAST_ERROR = 0x14,
    IOCTL_SO_INETATON = 0x15,
    IOCTL_SO_INETPTON = 0x16,
    IOCTL_SO_INETNTOP = 0x17,
    IOCTLV_SO_GET_ADDR_INFO = 0x18,
    IOCTL_SO_SOCK_AT_MARK = 0x19,
    IOCTLV_SO_STARTUP = 0x1A,
    IOCTLV_SO_CLEANUP = 0x1B,
    IOCTLV_SO_GET_INTERFACE_OPT = 0x1C,
    IOCTLV_SO_SET_INTERFACE_OPT = 0x1D,
    IOCTL_SO_SET_INTERFACE = 0x1E,
    IOCTL_SO_INIT_INTERFACE = 0x1F,

    IOCTL_SO_ICMP_SOCKET = 0x30,
    IOCTLV_SO_ICMP_PING = 0x31,
    IOCTL_SO_ICMP_CANCEL = 0x32,
    IOCTL_SO_ICMP_CLOS = 0x33,
};

NetworkSystem::NetworkSystem() {
    resetInternalState(STATE_INACTIVE);
}

NetworkSystem::~NetworkSystem() {
    closeSystem();
}

void NetworkSystem::initSystem(bool wait) {
    if (mState == STATE_INACTIVE) {
        MR::startFunctionAsyncExecute(MR::Functor(this, &NetworkSystem::initAsync), 10, "NetworkSystem::initAsync");

        if (wait) {
            MR::waitForEndFunctionAsyncExecute("NetworkSystem::initAsync");
        }
    }
}

void NetworkSystem::closeSystem() {
    if (mFd >= 0) {
        IOS_Close(mFd);
    }

    resetInternalState(STATE_INACTIVE);
}

IOSFd NetworkSystem::socket(u32 domain, u32 type, u32 protocol) {
    if (mFd < 0) {
        return NW::SO_ENXIO;
    }

    struct {
        u32 mDomain;
        u32 mType;
        u32 mProtocol;
    } params ATTRIBUTE_ALIGN(32);

    params.mDomain = domain;
    params.mType = type;
    params.mProtocol = protocol;

    return IOS_Ioctl(mFd, IOCTL_SO_SOCKET, &params, sizeof(params), nullptr, 0);
}

IOSError NetworkSystem::close(IOSFd fd) {
    if (mFd < 0 || fd < 0) {
        return NW::SO_ENXIO;
    }

    struct {
        IOSFd mFd;
    } params ATTRIBUTE_ALIGN(32);

    params.mFd = fd;

    return IOS_Ioctl(mFd, IOCTL_SO_CLOSE, &params, sizeof(params), nullptr, 0);
}

IOSError NetworkSystem::bind(IOSFd fd, const NW::SockAddress& rAddress) {
    if (mFd < 0 || fd < 0) {
        return NW::SO_ENXIO;
    }

    struct {
        IOSFd mFd;
        BOOL mHasAddress;
        NW::SockAddress mAddress;
    } params ATTRIBUTE_ALIGN(32);

    params.mFd = fd;
    params.mHasAddress = TRUE;
    memcpy(&params.mAddress, &rAddress, sizeof(params.mAddress));

    return IOS_Ioctl(mFd, IOCTL_SO_BIND, &params, sizeof(params), nullptr, 0);
}

IOSError NetworkSystem::connect(IOSFd fd, const NW::SockAddress& rAddress) {
    if (mFd < 0 || fd < 0) {
        return NW::SO_ENXIO;
    }

    struct {
        IOSFd mFd;
        BOOL mHasAddress;
        NW::SockAddress mAddress;
    } params ATTRIBUTE_ALIGN(32);

    params.mFd = fd;
    params.mHasAddress = TRUE;
    memcpy(&params.mAddress, &rAddress, sizeof(params.mAddress));

    return IOS_Ioctl(mFd, IOCTL_SO_CONNECT, &params, sizeof(params), nullptr, 0);
}

IOSError NetworkSystem::recv(IOSFd fd, void* pBuffer, u32 size, u32 flags, NW::SockAddress* pFrom) {
    if (mFd < 0 || fd < 0) {
        return NW::SO_ENXIO;
    }

    struct {
        IOSFd mFd;
        u32 mFlags;
    } params ATTRIBUTE_ALIGN(32);

    params.mFd = fd;
    params.mFlags = flags;

    IOSIoVector iov[3] ATTRIBUTE_ALIGN(32);
    iov[0].base = reinterpret_cast<u8*>(&params);
    iov[0].length = sizeof(params);
    iov[1].base = static_cast<u8*>(pBuffer);
    iov[1].length = size;
    iov[2].base = reinterpret_cast<u8*>(pFrom);
    iov[2].length = pFrom != nullptr ? sizeof(*pFrom) : 0;

    return IOS_Ioctlv(mFd, IOCTLV_SO_RECV_FROM, 1, pFrom != nullptr ? 2 : 1, iov);
}

IOSError NetworkSystem::send(IOSFd fd, void* pBuffer, u32 size, u32 flags, NW::SockAddress* pTo) {
    if (mFd < 0 || fd < 0) {
        return NW::SO_ENXIO;
    }

    struct {
        IOSFd mFd;
        u32 mFlags;
        BOOL mHasAddress;
        NW::SockAddress mAddress;
    } params ATTRIBUTE_ALIGN(32);

    params.mFd = fd;
    params.mFlags = flags;

    if (pTo) {
        params.mHasAddress = TRUE;
        memcpy(&params.mAddress, pTo, sizeof(params.mAddress));
    } else {
        params.mHasAddress = FALSE;
        memset(&params.mAddress, 0, sizeof(params.mAddress));
    }

    IOSIoVector iov[2] ATTRIBUTE_ALIGN(32);
    iov[0].base = static_cast<u8*>(pBuffer);
    iov[0].length = size;
    iov[1].base = reinterpret_cast<u8*>(&params);
    iov[1].length = sizeof(params);

    return IOS_Ioctlv(mFd, IOCTLV_SO_SEND_TO, 2, 0, iov);
}

void NetworkSystem::initAsync() {
    BOOL interrupts = OSDisableInterrupts();

    resetInternalState(STATE_BUSY);

    IOSError err;

    // NCD initialization
    {
        IOSFd ncdFd = IOS_Open(NCD_IOS_PATH, 0);
        if (ncdFd < 0) {
            err = ncdFd;
            goto Error;
        }

        u8 linkStatus[32] ATTRIBUTE_ALIGN(32);

        IOSIoVector iov[1] ATTRIBUTE_ALIGN(32);
        iov[0].base = linkStatus;
        iov[0].length = sizeof(linkStatus);

        IOSError code = IOS_Ioctlv(ncdFd, IOCTLV_NCD_GET_LINK_STATUS, 0, 1, iov);
        IOS_Close(ncdFd);

        if (code < 0) {
            err = code;
            goto Error;
        }
    }

    // NWC24 initialization
    {
        IOSFd kdFd = IOS_Open(NWC24_IOS_PATH, 0);
        if (kdFd < 0) {
            err = kdFd;
            goto Error;
        }

        u8 startup[32] ATTRIBUTE_ALIGN(32);

        IOSError code = IOS_Ioctl(kdFd, IOCTL_NWC24_STARTUP_SOCKET, nullptr, 0, startup, sizeof(startup));
        IOS_Close(kdFd);

        if (code < 0) {
            err = code;
            goto Error;
        }
    }

    // SO initialization
    {
        IOSFd soFd = IOS_Open(SO_IOS_PATH, 0);
        if (soFd < 0) {
            err = soFd;
            goto Error;
        }

        IOSError code = IOS_Ioctl(soFd, IOCTL_SO_INIT_INTERFACE, nullptr, 0, nullptr, 0);
        if (code < 0) {
            IOS_Close(soFd);
            err = code;
            goto Error;
        }

        mFd = soFd;
    }

    OSRestoreInterrupts(interrupts);

    // Local IP fetch
    {
        u32 ip;
        for (u32 i = 0; i < RETRY_TIMES; i++) {
            u32 ip = IOS_Ioctl(mFd, IOCTL_SO_GET_HOST_ID, nullptr, 0, nullptr, 0);

            if (ip != 0) {
                mIP.mAddress = ip;
                goto Success;
            }

            OSSleepTicks(RETRY_TICKS);
        }

        err = NW::SO_ETIMEDOUT;
    }

Error:
    mFd = -1;
    mError = err;
    mState = STATE_INACTIVE;
    mIP.mAddress = 0;
    OSRestoreInterrupts(interrupts);
    return;

Success:
    mError = NW::SO_SUCCESS;
    mState = STATE_ACTIVE;
    return;
}

void NetworkSystem::resetInternalState(State state) {
    mFd = -1;
    mError = NW::SO_SUCCESS;
    mState = state;
    mIP.mAddress = 0;
}
