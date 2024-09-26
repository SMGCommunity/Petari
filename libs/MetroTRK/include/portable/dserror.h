#ifndef DSERROR_H
#define DSERROR_H


#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	kNoError = 0,
	kStepError = 1,
	kParameterError = 2,
	kEventQueueFull = 0x100,
	kNoMessageBufferAvailable = 0x300,
	kMessageBufferOverflow = 0x301,
	kMessageBufferReadError = 0x302,
	kDispatchError = 0x500,
	kInvalidMemory = 0x700,
	kInvalidRegister = 0x701,
	kCWDSException = 0x702,
	kUnsupportedError = 0x703,
	kInvalidProcessId = 0x704,
	kInvalidThreadId = 0x705,
	kOsError = 0x706,
	kError800 = 0x800
} DSError;

#ifdef __cplusplus
};
#endif

#endif // DSERROR_H