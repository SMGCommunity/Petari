#ifndef BTE_H
#define BTE_H

#ifdef __cplusplus
extern "C" {
#endif

/* bte uses different types so we will respect them here */

typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef signed   long   INT32;
typedef signed   char   INT8;
typedef signed   short  INT16;
typedef unsigned char   BOOLEAN;

typedef unsigned char   UBYTE;

typedef UINT8 BD_ADDR[6];

typedef struct {
    UINT16 event;
    UINT16 len;
    UINT16 offset;
    UINT16 layer_specific;
} BT_HDR;

typedef BOOLEAN (tBTA_SYS_EVT_HDLR)(BT_HDR *p_msg);
typedef void (tBTA_SYS_DISABLE)(void);

typedef UINT8 tBTA_SYS_ID;
typedef UINT8 tBTA_SYS_CONN_STATUS;
typedef UINT8 tBTA_SYS_PREF_ROLES;

typedef UINT16 tBTA_HH_ATTR_MASK;
typedef UINT8 tBTA_HH_PROTO_MODE;
typedef UINT16 tBTA_HH_EVT;
typedef UINT8 tBTA_HH_STATUS;

typedef struct {
    tBTA_SYS_EVT_HDLR *evt_hdlr;
    tBTA_SYS_DISABLE *disable;
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
    UINT8 *dsc_list;
} tHID_DEV_DSCP_INFO;

typedef struct {
    tBTA_HH_STATUS status; 
    UINT8 handle;
} tBTA_HH_CBDATA;

typedef struct {
    tBTA_HH_STATUS  status;
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

typedef union
{
	tBTA_HH_DEV_INFO dev_info;
    tBTA_HH_CONN conn;
    tBTA_HH_CBDATA dev_status;
    tBTA_HH_STATUS status;
    tBTA_HH_DEV_DSCP_INFO dscp_info;
    tBTA_HH_HSDATA hs_data;
    tBTA_HH_ACL_QUEUE_INFO acl_queue_info;
} tBTA_HH;

#ifdef __cplusplus
}
#endif

#endif // BTE_H
