#ifndef NWC24MSGCOMMIT_H
#define NWC24MSGCOMMIT_H

#include "revolution/nwc24.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NWC24_SEND_BOX,
    NWC24_RECV_BOX,
} NWC24MsgBoxId;

NWC24Err NWC24CommitMsg(NWC24MsgObj* obj);

#ifdef __cplusplus
}
#endif

#endif  // NWC24MSGCOMMIT_H
