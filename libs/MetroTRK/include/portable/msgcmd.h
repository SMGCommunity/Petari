#ifndef MSGCMD_H
#define MSGCMD_H

typedef enum {
    kDSPing = 0x00,
	kDSConnect = 0x01,
	kDSDisconnect = 0x02,
	kDSReset = 0x03,
	kDSVersions = 0x04,
	kDSSupportMask = 0x05,
	kDSOverride = 0x07,
	kDSReadMemory = 0x10,
	kDSWriteMemory = 0x11,
	kDSReadRegisters = 0x12,
	kDSWriteRegisters = 0x13,
	kDSSetOption = 0x17,
	kDSContinue = 0x18,
	kDSStep = 0x19,
	kDSStop = 0x1a,
	kDSReplyACK = 0x80,
	kDSReplyNAK = 0xFF,
	kDSNotifyStopped = 0x90,
	kDSNotifyException = 0x91,
	kDSWriteFile = 0xD0,
	kDSReadFile = 0xD1,
	kDSOpenFile = 0xD2,
	kDSCloseFile = 0xD3,
	kDSPositionFile = 0xD4
} MessageCommandID;

typedef enum DSFileHandle {
    kDSStdin  = 0x00,
    kDSStdout = 0x01,
    kDSStderr = 0x02
} DSFileHandle;

typedef enum DSIOResult {
    kDSIONoError    = 0x00,
    kDSIOError      = 0x01,
    kDSIOEOF        = 0x02
} DSIOResult;

typedef enum
{
    kDSStepIntoCount = 0x00,            /* Exec count instructions & stop */
    kDSStepIntoRange = 0x01,            /* Exec until PC is out of specified range */
    kDSStepOverCount = 0x10,            /* Step over 1*count instructions & stop */
    kDSStepOverRange = 0x11             /* Step over until PC is out of specified range */
} DSMessageStepOptions;

typedef enum
{
    kDSReplyNoError                 = 0x00, /* no error */
    kDSReplyError                   = 0x01, /* generic error in CWDS message */
    kDSReplyPacketSizeError         = 0x02, /* unexpected pkt size in send msg */
    kDSReplyCWDSError               = 0x03, /* internal error occurred in CWDS */
    kDSReplyEscapeError             = 0x04, /* escape followed by frame flag */
    kDSReplyBadFCS                  = 0x05, /* bad FCS in packet */
    kDSReplyOverflow                = 0x06, /* packet too long */

    kDSReplyUnsupportedCommandError = 0x10, /* command not supported */
    kDSReplyParameterError          = 0x11, /* command param out of range */
    kDSReplyUnsupportedOptionError  = 0x12, /* an option was not supported */
    kDSReplyInvalidMemoryRange      = 0x13, /* read/write to invalid memory */
    kDSReplyInvalidRegisterRange    = 0x14, /* read/write invalid registers */
    kDSReplyCWDSException           = 0x15, /* exception occurred in CWDS */
    kDSReplyNotStopped              = 0x16, /* targeted system or thread is running */

    kDSReplyOsError                 = 0x20, /* general OS-related error */
    kDSReplyInvalidProcessId        = 0x21, /* request specified invalid process */
    kDSReplyInvalidThreadId         = 0x22  /* request specified invalid thread */
} DSReplyError;

#define DSFetch_s8(_p_)     (*((s8 *)_p_))
#define DSFetch_s16(_p_)    (*((s16 *)_p_))
#define DSFetch_s32(_p_)    (*((s32 *)_p_))
#define DSFetch_s64(_p_)    (*((s64 *)_p_))

#define DSFetch_u8(_p_)     (*((u8 *)_p_))
#define DSFetch_u16(_p_)    (*((u16 *)_p_))
#define DSFetch_u32(_p_)    (*((u32 *)_p_))
#define DSFetch_u64(_p_)    (*((u64 *)_p_))

#endif // MSGCMD_H