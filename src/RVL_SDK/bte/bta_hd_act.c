#include "bta_hd_int.h"

/*******************************************************************************
 * headers
 */

#include <string.h>

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "bd.h"
#include "bta_hd_api.h"
#include "bta_sys.h"
#include "btm_api.h"
#include "gki.h"
#include "hidd_api.h"
#include "hiddefs.h"
#include "sdp_api.h"

/*******************************************************************************
 * types
 */

typedef struct
{
	UINT8	modifier;	// size 0x01, offset 0x00
	UINT8	reserved;	// size 0x01, offset 0x01
	UINT8	keycode_1;	// size 0x01, offset 0x02
	UINT8	keycode_2;	// size 0x01, offset 0x03
	UINT8	keycode_3;	// size 0x01, offset 0x04
	UINT8	keycode_4;	// size 0x01, offset 0x05
	UINT8	keycode_5;	// size 0x01, offset 0x06
	UINT8	keycode_6;	// size 0x01, offset 0x07
} tBTA_HD_KEY_BUF; // size 0x08

/*******************************************************************************
 * local function declarations
 */

static void bta_hd_send_data(tBTA_HD_CB *p_cb, UINT8 rep_type, UINT16 len,
                             UINT8 *p_data);
static UINT8 bta_hd_flush_data(tBTA_HD_CB *p_cb);

/*******************************************************************************
 * variables
 */

// NOTE: before .data objects to match DWARF order
// .sdata2
UINT8 const bta_hd_buf_len[] =
{
	9, // default?
	9, // keyboard?
	5, // mouse?
};

// .data
char const *bta_hd_ctrl_str[] =
{
	"NOP",
	"HARD_RESET",
	"SOFT_RESET",
	"SUSPEND",
	"EXIT_SUSPEND",
	"VCAB_UNPLUG",
};

char const *bta_hd_rept_id_str[] =
{
	"MASK",
	"OTHER",
	"INPUT",
	"OUTPUT",
	"FEATURE",
};

char const *bta_hd_evt_str[] =
{
	"OPEN",
	"CLOSE",
	"RETRYING",
	"MODE_CHG",
	"PM_FAILED",
	"CONTROL",
	"GET_REPORT",
	"SET_REPORT",
	"GET_PROTO",
	"SET_PROTO",
	"GET_IDLE",
	"SET_IDLE",
	"DATA",
	"DATC",
	"L2C_CONG",
};

/*******************************************************************************
 * functions
 */

static void bta_hd_send_data(tBTA_HD_CB *p_cb, UINT8 rep_type, UINT16 len,
                             UINT8 *p_data)
{
	BT_HDR *p_buf;
	UINT8 *p;

	// TODO: What is 10
	if ((p_buf = GKI_getbuf(10 + len)) != NULL)
	{
		p = (UINT8 *)(p_buf + 1) + 10;

		memcpy(p, p_data, len);

		p_buf->len = len;
		p_buf->offset = 10;

		HID_DevSendData(HID_CHANNEL_CTRL, rep_type, p_buf);

		if (p_cb)
		{
			bta_sys_busy(BTA_ID_HD, p_cb->app_id, p_cb->peer_addr);
			bta_sys_idle(BTA_ID_HD, p_cb->app_id, p_cb->peer_addr);
		}
	}
}

static UINT8 bta_hd_flush_data(tBTA_HD_CB *p_cb)
{
	BT_HDR *p_buf;
	BOOLEAN set_busy = FALSE;

	while ((p_buf = GKI_dequeue(&p_cb->out_q)) != NULL)
	{
		if (HID_DevSendData(HID_CHANNEL_INTR, HID_PAR_REP_TYPE_INPUT, p_buf)
		    == sizeof *p_buf)
		{
			GKI_enqueue_head(&p_cb->out_q, p_buf);
			break;
		}

		set_busy = TRUE;
	}

	if (set_busy)
	{
		bta_sys_busy(BTA_ID_HD, p_cb->app_id, p_cb->peer_addr);
		bta_sys_idle(BTA_ID_HD, p_cb->app_id, p_cb->peer_addr);
	}

	return GKI_queue_is_empty(&p_cb->out_q);
}

void bta_hd_init_con_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data)
{
	APPL_TRACE(DEBUG, "bta_hd_init_con_act");

	if (HID_DevConnect())
		bta_hd_sm_execute(p_cb, BTA_HD_6_EVT, NULL);
}

void bta_hd_close_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data)
{
	APPL_TRACE(DEBUG, "bta_hd_close_act");

	HID_DevDisconnect();
}

void bta_hd_disable_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data)
{
	APPL_TRACE(DEBUG, "bta_hd_disable_act");

	BTM_SecClrService(BTM_SEC_SERVICE_HID_SEC_CTRL);
	BTM_SecClrService(BTM_SEC_SERVICE_HID_INTR);

	if (bta_hd_cb.sdp_handle != 0)
	{
		SDP_DeleteRecord(bta_hd_cb.sdp_handle);
		bta_hd_cb.sdp_handle = 0;
	}

	HID_DevDeregister();
}

void bta_hd_open_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data)
{
	BD_ADDR_PTR p_addr = p_data->cback_data.pdata->host_bdaddr;

	p_cb->proto = HID_PAR_PROTOCOL_REPORT;

	bdcpy(p_cb->peer_addr, p_addr);

	bta_sys_conn_open(BTA_ID_HD, p_cb->app_id, p_addr);

	(*p_cb->p_cback)(BTA_HD_OPEN_EVT, (tBTA_HD *)p_addr);
}

void bta_hd_opn_cb_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data)
{
	UINT8 res_code;
	UINT8 *p_cong;
	tHID_DEV_DSCP_INFO *p_info;
	tBTA_HD_CBACK_DATA *p_evt = &p_data->cback_data;
	BOOLEAN set_busy = TRUE;

	switch (p_evt->event)
	{
	case BTA_HDEV_EVT_CONTROL:
		if (p_evt->data == 5)
		{
			(*p_cb->p_cback)(BTA_HD_3_EVT, NULL);
			set_busy = FALSE;
		}
		else if (p_evt->data == 3)
		{
			bta_sys_idle(BTA_ID_HD, p_cb->app_id, p_cb->peer_addr);
			set_busy = FALSE;
		}

		break;

	case BTA_HDEV_EVT_GET_REPORT:
		p_info = &p_bta_hd_cfg->sdp_info.dscp_info;
		bta_hd_send_data(p_cb, HID_PAR_REP_TYPE_FEATURE, p_info->dl_len,
		                 p_info->dsc_list);
		break;

	case BTA_HDEV_EVT_GET_PROTO:
		res_code = p_cb->proto;
		bta_hd_send_data(p_cb, HID_PAR_REP_TYPE_OTHER, sizeof res_code,
		                 &res_code);
		break;

	case BTA_HDEV_EVT_SET_PROTO:
		if (p_evt->data == HID_PAR_PROTOCOL_REPORT
		    || p_evt->data == HID_PAR_PROTOCOL_BOOT_MODE)
		{
			p_cb->proto = p_evt->data;
			res_code = 0; // HIDDev equivalent of BTA_HH_OK?
		}
		else
		{
			res_code = 3; // HIDDev equivalent of BTA_HH_HS_TRANS_NOT_SPT?
		}

		HID_DevHandShake(res_code);
		break;

	case BTA_HDEV_EVT_L2C_CONG:
		p_cong = &p_evt->pdata->pm_err_code;
		if (*p_cong == 0)
		{
			bta_hd_flush_data(p_cb);
			set_busy = FALSE;
		}

		break;
	}

	if (set_busy)
	{
		bta_sys_busy(BTA_ID_HD, p_cb->app_id, p_cb->peer_addr);
		bta_sys_idle(BTA_ID_HD, p_cb->app_id, p_cb->peer_addr);
	}
}

void bta_hd_input_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data)
{
	tBTA_HD_API_SPEC *p_spec = &p_data->api_spec;
	tBTA_HD_API_INPUT *p_input = &p_data->api_input;
	tBTA_HD_KEY_BUF key_buf = {0};
	UINT16 len = bta_hd_buf_len[p_input->rid];
	UINT16 size;
	BT_HDR *p_buf;
	UINT8 *p;
	int temp;
	BOOLEAN release = TRUE;

	if (p_input->rid == 0)
		len = 9;

	size = 17 + len;

	p_buf = GKI_getbuf(size);
	if (!p_buf)
		return;

	p_buf->offset = 10;
	p = (UINT8 *)(p_buf + 1) + 10;
	*p++ = p_input->rid;

	// TODO: Name these cases
	switch (p_input->rid)
	{
	case 0:
		--p;
		memcpy(p, p_spec->seq, p_spec->len);

		p_input->rid = *p;
		temp = (10 - 1) - p_spec->len;

		if (temp > 0)
			memset(p + p_spec->len, 0, temp);

		break;

	case 1:
		key_buf.keycode_1 = p_input->keyCode;
		key_buf.modifier = p_input->buttons;
		memcpy(p, &key_buf, len - 1);
		break;

	case 2:
		*p++ = p_input->buttons;
		*p++ = p_input->keyCode;
		*p++ = p_input->keyCode2;
		*p++ = p_input->wheel;
		release = FALSE;
		break;

	default:
		GKI_freebuf(p_buf);
		return;
	}

	p_buf->len = len;

	GKI_enqueue(&p_cb->out_q, p_buf);

	if (release)
	{
		if ((p_buf = GKI_getbuf(size)) == NULL)
			return;

		p_buf->offset = 10;
		p_buf->len = len;

		p = (UINT8 *)(p_buf + 1) + 10;
		*p++ = p_input->rid;

		memset(p, 0, p_buf->len - 1);

		GKI_enqueue(&p_cb->out_q, p_buf);
	}

	bta_hd_flush_data(p_cb);
}

void bta_hd_discntd_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data)
{
	bta_sys_conn_close(BTA_ID_HD, p_cb->app_id, p_cb->peer_addr);
	(*p_cb->p_cback)(BTA_HD_CLOSE_EVT, (tBTA_HD *)p_cb->peer_addr);
}

void bta_hd_discnt_act(tBTA_HD_CB *p_cb, tBTA_HD_MSG *p_data)
{
	BT_HDR *p_buf;

	while ((p_buf = GKI_dequeue(&p_cb->out_q)) != NULL)
		GKI_freebuf(p_buf);

	bta_sys_conn_close(BTA_ID_HD, p_cb->app_id, p_cb->peer_addr);
	(*p_cb->p_cback)(BTA_HD_CLOSE_EVT, (tBTA_HD *)p_cb->peer_addr);
}

void bta_hd_hidd_cback(UINT8 event, UINT32 data,
                       tHID_DEV_REG_INFO_CBACK_DATA *pdata)
{
	tBTA_HD_CBACK_DATA *p_buf;
	tHID_DEV_REP_DATA *pget_rep;
	UINT16 sm_evt = BTA_HD_INVALID_EVT;
	UINT8 res_code;

	APPL_TRACE(EVENT, "HID event=0x%x(%s), data: %d", event,
	           bta_hd_evt_str[event], data);

	switch (event)
	{
	case BTA_HDEV_EVT_OPEN:
		sm_evt = BTA_HD_5_EVT;
		break;

	case BTA_HDEV_EVT_CLOSE:
		sm_evt = BTA_HD_6_EVT;
		break;

	case BTA_HDEV_EVT_CONTROL:
		APPL_TRACE(DEBUG, "EVT_CONTROL:%d(%s)", data, bta_hd_ctrl_str[data]);

		switch (data)
		{
		case BTA_HD_CTRL_SUSPEND:
		case BTA_HD_CTRL_EXIT_SUSPEND:
		case BTA_HD_CTRL_VCAB_UNPLUG:
			sm_evt = BTA_HD_4_EVT;
			HID_DevHandShake(0);
			break;

		default:
			HID_DevHandShake(3);
			break;
		}

		break;

	case BTA_HDEV_EVT_GET_REPORT:
		pget_rep = &pdata->get_rep;

		APPL_TRACE(DEBUG, "RepType:%d(%s), id:%d", pget_rep->rep_type,
		           bta_hd_rept_id_str[pget_rep->rep_type], pget_rep->rep_id);

		if (pget_rep->rep_type == BTA_HD_REPT_TYPE_FEATURE)
			sm_evt = BTA_HD_4_EVT;
		else
			HID_DevHandShake(3);

		break;

	case BTA_HDEV_EVT_SET_REPORT:
		HID_DevHandShake(3);
		break;

	case BTA_HDEV_EVT_GET_PROTO:
		sm_evt = BTA_HD_4_EVT;
		break;

	case BTA_HDEV_EVT_SET_PROTO:
		sm_evt = BTA_HD_4_EVT;
		break;

	case BTA_HDEV_EVT_GET_IDLE:
		res_code = 0;
		bta_hd_send_data(NULL, BTA_HD_CTRL_NOP, sizeof res_code, &res_code);
		break;

	case BTA_HDEV_EVT_SET_IDLE:
		res_code = 4;
		if (data == 0)
			res_code = 0;

		HID_DevHandShake(res_code);
		break;

	case BTA_HDEV_EVT_L2C_CONG:
		sm_evt = BTA_HD_4_EVT;
		break;
	}

	if (sm_evt == BTA_HD_INVALID_EVT)
		return;

	if ((p_buf = GKI_getbuf(sizeof *p_buf + sizeof *p_buf->pdata)))
	{
		p_buf->hdr.event = sm_evt;
		p_buf->event = event;
		p_buf->data = data;

		if (pdata)
		{
			p_buf->pdata = (tHID_DEV_REG_INFO_CBACK_DATA *)(p_buf + 1);
			memcpy(p_buf->pdata, pdata, sizeof *pdata);
		}
		else
		{
			p_buf->pdata = NULL;
		}

		bta_sys_sendmsg(p_buf);
	}
}
