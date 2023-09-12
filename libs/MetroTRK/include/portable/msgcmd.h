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

#endif // MSGCMD_H