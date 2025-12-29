#include "bta_hd_int.h"

/* References:
 * esp-idf <github.com/espressif/esp-idf>
 * [1] components/bt/host/bluedroid/bta/hd/include/bta_hd_main.c
 */

/*******************************************************************************
 * headers
 */

#include <string.h>

#include "bt_trace.h"
#include "bt_types.h" // BT_HDR
#include "data_types.h"

#include "bd.h" // bdcpy
#include "bta_hd_api.h"
#include "gki.h" // GKI_init_q
#include "hidd_api.h"
#include "hiddefs.h"

/*******************************************************************************
 * macros
 */

#define BTA_HD_NUM_COLS			2

#define BTA_HD_IGNORE			BTA_HD_MAX_ACTION

#define BTA_HD_ACTION_INDEX		0
#define BTA_HD_NEXT_STATE_INDEX	1

/*******************************************************************************
 * types
 */

// [1]: names
typedef UINT8 tBTA_HD_STATE;
enum
{
	BTA_HD_INIT_ST,
	BTA_HD_LISTEN_ST,
	BTA_HD_CONN_ST,
};

/* typedef UINT8 tBTA_HD_ACTION_TYPE; */
enum
{
	BTA_HD_INIT_CON_ACT,
	BTA_HD_CLOSE_ACT,
	BTA_HD_DISABLE_ACT,
	BTA_HD_OPEN_ACT,
	BTA_HD_OPEN_CB_ACT,
	BTA_HD_INPUT_ACT,
	BTA_HD_DISCNTD_ACT,
	BTA_HD_DISCNT_ACT,

	BTA_HD_MAX_ACTION
};

// NOTE: state table typedef is not an array to match dwarf

typedef void tBTA_HD_ACTION(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data);
typedef UINT8 tBTA_HD_STATE_TABLE_ENTRY[BTA_HD_NUM_COLS];

#if 0
typedef tBTA_HD_STATE_TABLE_ENTRY tBTA_HD_STATE_TABLE[];
#else
typedef tBTA_HD_STATE_TABLE_ENTRY tBTA_HD_STATE_TABLE;
#endif

/*******************************************************************************
 * local function declarations
 */

static void bta_hd_api_enable(tBTA_HD_MSG *p_data);

/*******************************************************************************
 * variables
 */

// .rodata
tBTA_HD_ACTION * const bta_hd_action[] =
{
	&bta_hd_init_con_act,
	&bta_hd_close_act,
	&bta_hd_disable_act,
	&bta_hd_open_act,
	&bta_hd_opn_cb_act,
	&bta_hd_input_act,
	&bta_hd_discntd_act,
	&bta_hd_discnt_act,
};

tBTA_HD_STATE_TABLE_ENTRY const bta_hd_st_listen[] =
{
	{BTA_HD_INIT_CON_ACT, BTA_HD_LISTEN_ST},
	{BTA_HD_CLOSE_ACT,    BTA_HD_LISTEN_ST},
	{BTA_HD_DISABLE_ACT,  BTA_HD_INIT_ST  },
	{BTA_HD_IGNORE,       BTA_HD_LISTEN_ST},
	{BTA_HD_IGNORE,       BTA_HD_LISTEN_ST},
	{BTA_HD_OPEN_ACT,     BTA_HD_CONN_ST  },
	{BTA_HD_DISCNTD_ACT,  BTA_HD_LISTEN_ST},
};

tBTA_HD_STATE_TABLE_ENTRY const bta_hd_st_open[] =
{
	{BTA_HD_IGNORE,       BTA_HD_CONN_ST  },
	{BTA_HD_CLOSE_ACT,    BTA_HD_CONN_ST  },
	{BTA_HD_DISABLE_ACT,  BTA_HD_INIT_ST  },
	{BTA_HD_INPUT_ACT,    BTA_HD_CONN_ST  },
	{BTA_HD_OPEN_CB_ACT,  BTA_HD_CONN_ST  },
	{BTA_HD_IGNORE,       BTA_HD_CONN_ST  },
	{BTA_HD_DISCNT_ACT,   BTA_HD_LISTEN_ST},
};

// .bss
tBTA_HD_CB bta_hd_cb;

// .sdata2
tBTA_HD_STATE_TABLE_ENTRY const * const bta_hd_st_tbl[] =
{
	bta_hd_st_listen,
	bta_hd_st_open,
};

/*******************************************************************************
 * functions
 */

static void bta_hd_api_enable(tBTA_HD_MSG *p_data)
{
	tHID_DEV_SDP_INFO sdp_info;
	tHID_DEV_REG_INFO reg_info;
	UINT8 sec_mask = p_data->api_enable.sec_mask;
	tBTA_HD_STATUS status = BTA_HD_ERROR;

	memset(&bta_hd_cb, 0, sizeof bta_hd_cb);
	GKI_init_q(&bta_hd_cb.out_q);
	bta_hd_cb.p_cback = p_data->api_enable.p_cback;
	reg_info.app_cback = &bta_hd_hidd_cback;
	bdcpy(reg_info.host_addr, p_data->api_enable.bd_addr);

	if (p_bta_hd_cfg->use_qos)
		reg_info.qos_info = &p_bta_hd_cfg->qos;
	else
		reg_info.qos_info = NULL;

	if (HID_DevRegister(&reg_info) == HID_SUCCESS)
	{
		HID_DevSetSecurityLevel("BT HID Combo Mouse/Keyboard", sec_mask);
		memcpy(&sdp_info, &p_bta_hd_cfg->sdp_info, sizeof sdp_info);

		if (p_data->api_enable.service_name[0] != '\0')
		{
			BCM_STRNCPY_S(sdp_info.svc_name, HID_MAX_SVC_NAME_LEN,
			              p_data->api_enable.service_name,
			              BTA_HD_APP_NAME_LEN + 1);
		}

		bta_hd_cb.sdp_handle = HID_DevSetSDPRecord(&sdp_info);
	}

	if (bta_hd_cb.sdp_handle != 0)
	{
		status = BTA_HD_OK;
		bta_hd_cb.state = BTA_HD_LISTEN_ST;
	}

	(*bta_hd_cb.p_cback)(BTA_HD_ENABLE_EVT, (tBTA_HD *)&status);
}

void bta_hd_sm_execute(tBTA_HD_CB *p_cb, UINT16 event, tBTA_HD_MSG *p_data)
{
	tBTA_HD_STATE_TABLE_ENTRY const *state_table;
	UINT8 action;

	if (p_cb->state == BTA_HD_INIT_ST)
	{
		APPL_TRACE(EVENT, "HD event=0x%x received in IDLE", event);
		return;
	}

	state_table = bta_hd_st_tbl[p_cb->state - 1];
	event &= 0xff;

	APPL_TRACE(EVENT, "HD event=0x%x state=%d, next: %d", event, p_cb->state,
	           state_table[event][BTA_HD_NEXT_STATE_INDEX]);
	p_cb->state = state_table[event][BTA_HD_NEXT_STATE_INDEX];

	action = state_table[event][BTA_HD_ACTION_INDEX];
	if (action != BTA_HD_IGNORE)
		(*bta_hd_action[action])(p_cb, p_data);
}

BOOLEAN bta_hd_hdl_event(BT_HDR *p_msg)
{
	switch (p_msg->event)
	{
	case BTA_HD_API_ENABLE_EVT:
		bta_hd_api_enable((tBTA_HD_MSG *)p_msg);
		break;

	default:
		bta_hd_sm_execute(&bta_hd_cb, p_msg->event, (tBTA_HD_MSG *)p_msg);
		break;
	}

	return TRUE;
}
