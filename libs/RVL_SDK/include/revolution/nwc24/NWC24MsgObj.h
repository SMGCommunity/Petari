#ifndef RVL_SDK_NWC24_MSG_OBJ_H
#define RVL_SDK_NWC24_MSG_OBJ_H
#include <revolution/types.h>
#include <cstdlib>

#include <revolution/nwc24/NWC24Types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct RFLCharData RFLCharData;

NWC24Err NWC24InitMsgObj(NWC24MsgObj* pMsg, NWC24MsgType type);
NWC24Err NWC24SetMsgToId(NWC24MsgObj* pMsg, NWC24UserId userId);
NWC24Err NWC24SetMsgText(NWC24MsgObj* pMsg, const char* pText, u32 len, NWC24Charset charset, NWC24Encoding encoding);
NWC24Err NWC24SetMsgFaceData(NWC24MsgObj* pMsg, const RFLCharData* pData);
NWC24Err NWC24SetMsgAttached(NWC24MsgObj* pMsg, const char* data, u32 size, NWC24MIMEType type);
NWC24Err NWC24SetMsgTag(NWC24MsgObj* pMsg, u16 tag);
NWC24Err NWC24SetMsgLedPattern(NWC24MsgObj* pMsg, u16 pattern);
NWC24Err NWC24SetMsgDesignatedTime(NWC24MsgObj* pMsg, s32 delay);
NWC24Err NWC24GetMsgSize(const NWC24MsgObj* pMsg, u32* size);
NWC24Err NWC24SetMsgMBDelay(NWC24MsgObj* pMsg, u8 delay);
NWC24Err NWC24SetMsgAltName(NWC24MsgObj* pMsg, const u16* pName, u32 len);
NWC24Err NWC24SetMsgMBNoReply(NWC24MsgObj* pMsg, BOOL enable);
NWC24Err NWC24SetMsgMBRegDate(NWC24MsgObj* pMsg, u16 year, u8 month, u8 day);

#ifdef __cplusplus
}
#endif
#endif
