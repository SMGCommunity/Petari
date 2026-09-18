#pragma once

#include "private/iostypes.h"
#include "revolution/types.h"

namespace NW {
    struct MACAddress {
        /* 0x00 */ u8 mOctets[6];
    };

    struct IPAddress {
        union {
            /* 0x00 */ u32 mAddress;
            /* 0x00 */ u8 mOctets[4];
        };
    };

    struct SockAddress {
        /* 0x00 */ u8 mLength;
        /* 0x01 */ u8 mFamily;
        /* 0x02 */ u16 mPort;
        /* 0x04 */ IPAddress mIP;
    };

    // Socket address family
    enum {
        SO_AF_INET = 2,
    };

    // Socket type
    enum {
        SO_SOCK_STREAM = 1,
        SO_SOCK_DGRAM = 2,
    };

    // Socket message flags
    enum {
        SO_MSG_OOB = 0x01,
        SO_MSG_PEEK = 0x02,
        SO_MSG_NONBLOCK = 0x04,
    };

    // Soctet option flags
    enum {
        SO_OPT_REUSEADDR = 0x4,
        SO_OPT_LINGER = 0x80,
        SO_OPT_OOBINLINE = 0x100,
        SO_OPT_SNDBUF = 0x1001,
        SO_OPT_RCVBUF = 0x1002,
        SO_OPT_SNDLOWAT = 0x1003,
        SO_OPT_RCVLOWAT = 0x1004,
        SO_OPT_TYPE = 0x1008,
        SO_OPT_ERROR = 0x1009,
    };

    // Socket error
    enum {
        SO_SUCCESS = 0,
        SO_E2BIG = -1,
        SO_EACCES = -2,
        SO_EADDRINUSE = -3,
        SO_EADDRNOTAVAIL = -4,
        SO_EAFNOSUPPORT = -5,
        SO_EAGAIN = -6,
        SO_EALREADY = -7,
        SO_EBADF = -8,
        SO_EBADMSG = -9,
        SO_EBUSY = -10,
        SO_ECANCELED = -11,
        SO_ECHILD = -12,
        SO_ECONNABORTED = -13,
        SO_ECONNREFUSED = -14,
        SO_ECONNRESET = -15,
        SO_EDEADLK = -16,
        SO_EDESTADDRREQ = -17,
        SO_EDOM = -18,
        SO_EDQUOT = -19,
        SO_EEXIST = -20,
        SO_EFAULT = -21,
        SO_EFBIG = -22,
        SO_EHOSTUNREACH = -23,
        SO_EIDRM = -24,
        SO_EILSEQ = -25,
        SO_EINPROGRESS = -26,
        SO_EINTR = -27,
        SO_EINVAL = -28,
        SO_EIO = -29,
        SO_EISCONN = -30,
        SO_EISDIR = -31,
        SO_ELOOP = -32,
        SO_EMFILE = -33,
        SO_EMLINK = -34,
        SO_EMSGSIZE = -35,
        SO_EMULTIHOP = -36,
        SO_ENAMETOOLONG = -37,
        SO_ENETDOWN = -38,
        SO_ENETRESET = -39,
        SO_ENETUNREACH = -40,
        SO_ENFILE = -41,
        SO_ENOBUFS = -42,
        SO_ENODATA = -43,
        SO_ENODEV = -44,
        SO_ENOENT = -45,
        SO_ENOEXEC = -46,
        SO_ENOLCK = -47,
        SO_ENOLINK = -48,
        SO_ENOMEM = -49,
        SO_ENOMSG = -50,
        SO_ENOPROTOOPT = -51,
        SO_ENOSPC = -52,
        SO_ENOSR = -53,
        SO_ENOSTR = -54,
        SO_ENOSYS = -55,
        SO_ENOTCONN = -56,
        SO_ENOTDIR = -57,
        SO_ENOTEMPTY = -58,
        SO_ENOTSOCK = -59,
        SO_ENOTSUP = -60,
        SO_ENOTTY = -61,
        SO_ENXIO = -62,
        SO_EOPNOTSUPP = -63,
        SO_EOVERFLOW = -64,
        SO_EPERM = -65,
        SO_EPIPE = -66,
        SO_EPROTO = -67,
        SO_EPROTONOSUPPORT = -68,
        SO_EPROTOTYPE = -69,
        SO_ERANGE = -70,
        SO_EROFS = -71,
        SO_ESPIPE = -72,
        SO_ESRCH = -73,
        SO_ESTALE = -74,
        SO_ETIME = -75,
        SO_ETIMEDOUT = -76,
        SO_ETXTBSY = -77,
        SO_EXDEV = -78,
    };
}; // namespace NW

class NetworkSystem {
public:
    enum State {
        STATE_INACTIVE,
        STATE_BUSY,
        STATE_ACTIVE,
    };

    IOSFd mFd;
    IOSError mError;
    State mState;
    NW::IPAddress mIP;

    NetworkSystem();
    ~NetworkSystem();

    void initSystem(bool wait = false);
    void closeSystem();

    IOSFd socket(u32 domain, u32 type, u32 protocol);
    IOSError close(IOSFd fd);
    IOSError bind(IOSFd fd, const NW::SockAddress& rAddress);
    IOSError connect(IOSFd fd, const NW::SockAddress& rAddress);
    IOSError recv(IOSFd fd, void* pBuffer, u32 size, u32 flags, NW::SockAddress* pFrom);
    IOSError send(IOSFd fd, void* pBuffer, u32 size, u32 flags, NW::SockAddress* pTo);

public:
    void initAsync();
    void resetInternalState(State state);
};
