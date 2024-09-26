#ifndef RVL_FACE_LIBRARY_NWC24_H
#define RVL_FACE_LIBRARY_NWC24_H
#include <RFL_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations
typedef struct NWC24MsgObj;

RFLErrcode RFLCommitNWC24Msg(struct NWC24MsgObj* msg, u16 index);

#ifdef __cplusplus
}
#endif
#endif
