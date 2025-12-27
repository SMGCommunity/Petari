#ifndef BTE_H
#define BTE_H

#include "revolution/bte/data_types.h"
#include "revolution/bte/gki.h"
#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef UINT8 BD_ADDR[6];

typedef struct {
    UINT16 event;
    UINT16 len;
    UINT16 offset;
    UINT16 layer_specific;
} BT_HDR;

typedef BOOLEAN(tBTA_SYS_EVT_HDLR)(BT_HDR* p_msg);
typedef void(tBTA_SYS_DISABLE)(void);

typedef UINT8 tBTA_SYS_ID;
typedef UINT8 tBTA_SYS_CONN_STATUS;
typedef UINT8 tBTA_SYS_PREF_ROLES;

typedef UINT16 tBTA_HH_ATTR_MASK;
typedef UINT8 tBTA_HH_PROTO_MODE;
typedef UINT16 tBTA_HH_EVT;
typedef UINT8 tBTA_HH_STATUS;

typedef UINT8 tBTA_DM_SEC_EVT;
typedef UINT8 tBTA_SIG_STRENGTH_MASK;
typedef INT8 tBTA_DM_RSSI_VALUE;
typedef UINT8 tBTA_DM_LINK_QUALITY_VALUE;

typedef UINT8 BD_NAME[248];
typedef UINT8* BD_NAME_PTR;

typedef UINT8 DEV_CLASS[3];
typedef UINT8* DEV_CLASS_PTR;

typedef UINT8 LINK_KEY[16];

typedef UINT8 tBTA_SERVICE_ID;

typedef void (*tUSER_CBACK)(UINT8 err);

typedef UINT8 tBTM_STATUS;

typedef UINT8 tBTA_SEC;

typedef struct {
    tBTA_SYS_EVT_HDLR* evt_hdlr;
    tBTA_SYS_DISABLE* disable;
} tBTA_SYS_REG;

typedef struct {
    UINT16 mbox_evt;
    UINT8 mbox;
    UINT8 timer;
    UINT8 trace_level;
} tBTA_SYS_CFG;

typedef struct {
    BD_ADDR bda;
    tBTA_HH_STATUS status;
    UINT8 handle;
} tBTA_HH_CONN;

typedef tBTA_HH_CONN tBTA_HH_DEV_INFO;

typedef struct desc_info {
    UINT16 dl_len;
    UINT8* dsc_list;
} tHID_DEV_DSCP_INFO;

typedef struct {
    tBTA_HH_STATUS status;
    UINT8 handle;
} tBTA_HH_CBDATA;

typedef struct {
    tBTA_HH_STATUS status;
    UINT8 handle;
    union {
        tBTA_HH_PROTO_MODE proto_mode;
        BT_HDR rpt_data;
        UINT8 idle_rate;
    } rsp_data;
} tBTA_HH_HSDATA;

typedef struct {
    UINT8 handle;
    UINT16 num_queued;
    UINT16 num_not_acked;
} tBTA_HH_ACL_QUEUE;

typedef struct {
    UINT16 num_avail_buffs;
    UINT16 num_buffs;
    UINT16 num_links;
    tBTA_HH_ACL_QUEUE queue_info[4];
} tBTA_HH_ACL_QUEUE_INFO;

typedef tHID_DEV_DSCP_INFO tBTA_HH_DEV_DSCP_INFO;

typedef union {
    tBTA_HH_DEV_INFO dev_info;
    tBTA_HH_CONN conn;
    tBTA_HH_CBDATA dev_status;
    tBTA_HH_STATUS status;
    tBTA_HH_DEV_DSCP_INFO dscp_info;
    tBTA_HH_HSDATA hs_data;
    tBTA_HH_ACL_QUEUE_INFO acl_queue_info;
} tBTA_HH;

typedef struct {
    BD_ADDR bd_addr;
} tBTA_DM_ENABLE;

typedef struct {
    BD_ADDR bd_addr;
    BD_NAME bd_name;
    DEV_CLASS dev_class;
} tBTA_DM_PIN_REQ;

typedef struct {
    BD_ADDR bd_addr;
    BD_NAME bd_name;
    BOOLEAN key_present;
    LINK_KEY key;
    BOOLEAN success;
} tBTA_DM_AUTH_CMPL;

typedef struct {
    BD_ADDR bd_addr;
    BD_NAME bd_name;
    tBTA_SERVICE_ID service;
} tBTA_DM_AUTHORIZE;

typedef struct {
    BD_ADDR bd_addr;
} tBTA_DM_LINK_UP;

typedef struct {
    BD_ADDR bd_addr;
    UINT8 status;
} tBTA_DM_LINK_DOWN;

typedef struct {
    BD_ADDR bd_addr;
    tBTA_SIG_STRENGTH_MASK mask;
    tBTA_DM_RSSI_VALUE rssi_value;
    tBTA_DM_LINK_QUALITY_VALUE link_quality_value;

} tBTA_DM_SIG_STRENGTH;

typedef struct {
    UINT8 level;
} tBTA_DM_BUSY_LEVEL;

typedef union {
    tBTA_DM_ENABLE enable;
    tBTA_DM_PIN_REQ pin_req;
    tBTA_DM_AUTH_CMPL auth_cmpl;
    tBTA_DM_AUTHORIZE authorize;
    tBTA_DM_LINK_UP link_up;
    tBTA_DM_LINK_DOWN link_down;
    tBTA_DM_SIG_STRENGTH sig_strength;
    tBTA_DM_BUSY_LEVEL busy_level;
} tBTA_DM_SEC;

typedef struct {
    BD_ADDR bd_addr;
    u8 bd_name[64];
    u8 link_key[16];
} SCBtCmpDevInfoSingle;

typedef struct {
    BD_ADDR bd_addr;
    u8 bd_name[64];
} SCBtDeviceInfoSingle;

typedef struct {
    u8 num;
    SCBtCmpDevInfoSingle info[6];
} SCBtCmpDevInfoArray;

typedef struct {
    u8 num;
    SCBtDeviceInfoSingle info[16];
} SCBtDeviceInfoArray;

tBTM_STATUS btm_remove_acl(BD_ADDR);
void BTA_HhClose(UINT8);
void BTA_HhSendData(UINT8, BT_HDR*);
void BTA_DmSendHciReset(void);
void BTA_HhGetAclQueueInfo(void);

typedef void(tBTA_HH_CBACK)(tBTA_HH_EVT, tBTA_HH*);

#ifdef __cplusplus
}
#endif

#endif  // BTE_H
