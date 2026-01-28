#include "bta_hd_api.h"

/*******************************************************************************
 * headers
 */

#include <string.h>

#include "bt_types.h"
#include "data_types.h"

#include "bd.h" // bdcpy
#include "bta_hd_int.h"
#include "bta_sys.h"
#include "gki.h"

/*******************************************************************************
 * variables
 */

// .sdata2
static tBTA_SYS_REG const bta_hd_reg =
{
	&bta_hd_hdl_event,
	&BTA_HdDisable
};

/*******************************************************************************
 * functions
 */

void BTA_HdEnable(BD_ADDR bd_addr, UINT8 sec_mask, char const *p_service_name,
                  tBTA_HD_CBACK *p_cback, UINT8 app_id)
{
	tBTA_HD_API_ENABLE *p_buf;

	GKI_sched_lock();

	bta_sys_register(BTA_ID_HD, &bta_hd_reg);

	GKI_sched_unlock();

	if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL)
	{
		p_buf->hdr.event = BTA_HD_API_ENABLE_EVT;
		p_buf->app_id = app_id;

		if (p_service_name)
		{
			BCM_STRNCPY_S(p_buf->service_name, BTA_HD_APP_NAME_LEN + 1,
			              p_service_name, BTA_HD_APP_NAME_LEN);
			p_buf->service_name[BTA_HD_APP_NAME_LEN] = '\0';
		}
		else
		{
			p_buf->service_name[0] = '\0';
		}

		p_buf->p_cback = p_cback;
		bdcpy(p_buf->bd_addr, bd_addr);
		p_buf->sec_mask = sec_mask;

		bta_sys_sendmsg(p_buf);
	}
}

void BTA_HdDisable(void)
{
	BT_HDR *p_buf;

	if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL)
	{
		p_buf->event = BTA_HD_API_DISABLE_EVT;

		bta_sys_sendmsg(p_buf);
	}
}

void BTA_HdOpen(UINT8 sec_mask)
{
	tBTA_HD_API_OPEN *p_buf;

	if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL)
	{
		p_buf->hdr.event = BTA_HD_API_OPEN_EVT;
		p_buf->sec_mask = sec_mask;

		bta_sys_sendmsg(p_buf);
	}
}

void BTA_HdClose(void)
{
	BT_HDR *p_buf;

	if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL)
	{
		p_buf->event = BTA_HD_API_CLOSE_EVT;

		bta_sys_sendmsg(p_buf);
	}
}

void BTA_HdSendRegularKey(UINT8 modifier, UINT8 key_code)
{
	tBTA_HD_API_INPUT *p_buf;

	if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL)
	{
		// TODO: rid
		p_buf->hdr.event = BTA_HD_API_SEND_EVT;
		p_buf->rid = 1;
		p_buf->keyCode = key_code;
		p_buf->buttons = modifier;

		bta_sys_sendmsg(p_buf);
	}
}

void BTA_HdSendSpecialKey(UINT8 key_len, UINT8 *key_seq)
{
	tBTA_HD_API_SPEC *p_buf;

	if ((p_buf = GKI_getbuf(sizeof *p_buf + key_len)) != NULL)
	{
		p_buf->hdr.event = BTA_HD_API_SEND_EVT;
		p_buf->rid = 0;
		p_buf->len = key_len;
		p_buf->seq = (UINT8 *)(p_buf + 1);
		memcpy(p_buf->seq, key_seq, key_len);

		bta_sys_sendmsg(p_buf);
	}
}

void BTA_HdSendMouseReport(UINT8 is_left, UINT8 is_right, UINT8 is_middle,
                           UINT8 delta_x, UINT8 delta_y, UINT8 delta_wheel)
{
	tBTA_HD_API_INPUT *p_buf;

	if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL)
	{
		p_buf->hdr.event = BTA_HD_API_SEND_EVT;
		p_buf->rid = 2;
		p_buf->keyCode = delta_x;
		p_buf->keyCode2 = delta_y;
		p_buf->buttons =
			(is_left ? 1 : 0) | (is_right ? 2 : 0) | (is_middle ? 4 : 0);
		p_buf->wheel = delta_wheel;

		bta_sys_sendmsg(p_buf);
	}
}
