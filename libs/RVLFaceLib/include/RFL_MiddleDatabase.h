#ifndef RFL_MIDDLEDATABASE_H
#define RFL_MIDDLEDATABASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/types.h>

typedef enum {
	RFLMiddleDBType_HiddenRandom,
	RFLMiddleDBType_HiddenNewer,
	RFLMiddleDBType_HiddenOlder,
	RFLMiddleDBType_Random,
	RFLMiddleDBType_UserSet,
	RFLMiddleDBType_WiFi
} RFLMiddleDBType;

u32 RFLGetMiddleDBBufferSize(u16);

#ifdef __cplusplus
}
#endif

#endif // RFL_MIDDLEDATABASE_H