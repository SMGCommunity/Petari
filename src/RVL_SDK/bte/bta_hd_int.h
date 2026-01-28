#ifndef BTE_BTA_HD_INT_H
#define BTE_BTA_HD_INT_H

/* References:
 * esp-idf <github.com/espressif/esp-idf>
 * [1] components/bt/host/bluedroid/bta/hd/include/bta_hd_int.h
 */

/*******************************************************************************
 * headers
 */

#include "bt_types.h"
#include "data_types.h"

#include "bta_hd_api.h"
#include "bta_sys.h"
#include "gki.h"
#include "hidd_api.h"
#include "hiddefs.h"

/*******************************************************************************
 * macros
 */

#define BTA_HD_APP_NAME_LEN	35	// [1]: name

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

// [1]: names
enum
{
	BTA_HD_API_OPEN_EVT = BTA_SYS_EVT_START(BTA_ID_HD),
	BTA_HD_API_CLOSE_EVT,
	BTA_HD_API_DISABLE_EVT,
	BTA_HD_API_SEND_EVT,
	BTA_HD_4_EVT,
	BTA_HD_5_EVT,
	BTA_HD_6_EVT,
	BTA_HD_API_ENABLE_EVT,

	BTA_HD_INVALID_EVT
};

typedef UINT8 tBTA_HDEV_EVT;
enum
{
	BTA_HDEV_EVT_OPEN,
	BTA_HDEV_EVT_CLOSE,
	BTA_HDEV_EVT_RETRYING,
	BTA_HDEV_EVT_MODE_CHG,
	BTA_HDEV_EVT_PM_FAILED,
	BTA_HDEV_EVT_CONTROL,
	BTA_HDEV_EVT_GET_REPORT,
	BTA_HDEV_EVT_SET_REPORT,
	BTA_HDEV_EVT_GET_PROTO,
	BTA_HDEV_EVT_SET_PROTO,
	BTA_HDEV_EVT_GET_IDLE,
	BTA_HDEV_EVT_SET_IDLE,
	BTA_HDEV_EVT_DATA,
	BTA_HDEV_EVT_DATC,
	BTA_HDEV_EVT_L2C_CONG,
};

typedef UINT8 tBTA_HH_TRANS_CTRL_TYPE;
enum
{
	BTA_HD_CTRL_NOP				= HID_PAR_CONTROL_NOP,
	BTA_HD_CTRL_HARD_RESET		= HID_PAR_CONTROL_HARD_RESET,
	BTA_HD_CTRL_SOFT_RESET		= HID_PAR_CONTROL_SOFT_RESET,
	BTA_HD_CTRL_SUSPEND			= HID_PAR_CONTROL_SUSPEND,
	BTA_HD_CTRL_EXIT_SUSPEND	= HID_PAR_CONTROL_EXIT_SUSPEND,
	BTA_HD_CTRL_VCAB_UNPLUG		= HID_PAR_CONTROL_VIRTUAL_CABLE_UNPLUG,
};

typedef UINT8 tBTA_HD_RPT_TYPE;
enum
{
	BTA_HD_REPT_TYPE_MASK = 0x03,

	BTA_HD_REPT_TYPE_OTHER = 0,
	BTA_HD_REPT_TYPE_INPUT,
	BTA_HD_REPT_TYPE_OUTPUT,
	BTA_HD_REPT_TYPE_FEATURE,
};

typedef struct
{
	BT_HDR			hdr;									// size 0x08, offset 0x00
	char			service_name[BTA_HD_APP_NAME_LEN + 1];	// size 0x24, offset 0x08
	tBTA_HD_CBACK	*p_cback;								// size 0x04, offset 0x2c
	BD_ADDR			bd_addr;								// size 0x06, offset 0x30
	UINT8			sec_mask;								// size 0x01, offset 0x36
	UINT8			app_id;									// size 0x01, offset 0x37
} tBTA_HD_API_ENABLE; // size 0x38

typedef struct
{
	BT_HDR	hdr;		// size 0x08, offset 0x00
	UINT8	sec_mask;	// size 0x01, offset 0x08
} tBTA_HD_API_OPEN; // size 0x0a

typedef struct
{
	BT_HDR	hdr;		// size 0x08, offset 0x00
	UINT8	rid;		// size 0x01, offset 0x08
	UINT8	keyCode;	// size 0x01, offset 0x09
	UINT8	keyCode2;	// size 0x01, offset 0x0a
	UINT8	wheel;		// size 0x01, offset 0x0b
	UINT8	buttons;	// size 0x01, offset 0x0c
} tBTA_HD_API_INPUT; // size 0x0e

typedef struct
{
	BT_HDR	hdr;	// size 0x08, offset 0x00
	UINT8	rid;	// size 0x01, offset 0x08
	UINT8	len;	// size 0x01, offset 0x09
	UINT8	*seq;	// size 0x04, offset 0x0c
} tBTA_HD_API_SPEC; // size 0x10

typedef struct
{
	BT_HDR							hdr;	// size 0x08, offset 0x00
	UINT8							event;	// size 0x01, offset 0x08
	UINT32							data;	// size 0x04, offset 0x0c
	tHID_DEV_REG_INFO_CBACK_DATA	*pdata;	// size 0x04, offset 0x10
} tBTA_HD_CBACK_DATA; // size 0x14

typedef union
{
	tBTA_HD_API_ENABLE	api_enable;	// size 0x38
	tBTA_HD_API_OPEN	api_open;	// size 0x0a
	tBTA_HD_API_INPUT	api_input;	// size 0x0e
	tBTA_HD_API_SPEC	api_spec;	// size 0x10
	tBTA_HD_CBACK_DATA	cback_data;	// size 0x14
} tBTA_HD_MSG; // size 0x38

typedef struct
{
	BD_ADDR			peer_addr;	// size 0x06, offset 0x00
	UINT32			sdp_handle;	// size 0x04, offset 0x08
	tBTA_HD_CBACK	*p_cback;	// size 0x04, offset 0x0c
	BUFFER_Q		out_q;		// size 0x0c, offset 0x10
	UINT8			sec_mask;	// size 0x01, offset 0x1c
	UINT8			state;		// size 0x01, offset 0x1d
	UINT8			proto;		// size 0x01, offset 0x1e
	UINT8			app_id;		// size 0x01, offset 0x1f
} tBTA_HD_CB; // size 0x20

typedef struct
{
	tQOS_INFO			qos;		// size 0x48, offset 0x00
	tHID_DEV_SDP_INFO	sdp_info;	// size 0x74, offset 0x48
	BOOLEAN				use_qos;	// size 0x01, offset 0xbc
} tBTA_HD_CFG; // size 0xc0

/*******************************************************************************
 * external globals
 */

extern tBTA_HD_CB bta_hd_cb;
extern tBTA_HD_CFG *p_bta_hd_cfg;

/*******************************************************************************
 * functions
 */

/* bta_hd_act.h */
void bta_hd_init_con_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data);
void bta_hd_close_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data);
void bta_hd_disable_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data);
void bta_hd_open_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data);
void bta_hd_opn_cb_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data);
void bta_hd_input_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data);
void bta_hd_discntd_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data);
void bta_hd_discnt_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data);
void bta_hd_hidd_cback(UINT8 event, UINT32 data,
                       tHID_DEV_REG_INFO_CBACK_DATA *pdata);

/* bta_hd_int.h */
void bta_hd_sm_execute(tBTA_HD_CB *p_cb, UINT16 event, tBTA_HD_MSG *p_data);
BOOLEAN bta_hd_hdl_event(BT_HDR *p_msg);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_HD_INT_H
