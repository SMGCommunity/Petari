#include "bta_prm_int.h"

/*******************************************************************************
 * headers
 */

#include <string.h>

#include "BCM2045.h"

#include "bt_trace.h"
#include "bt_types.h" // BT_HDR
#include "data_types.h"

#include "bta_fs_co.h"
#include "bta_sys.h" // bta_sys_sendmsg
#include "gki.h"

/*******************************************************************************
 * local function declarations
 */

static void bta_lower_level_prm_cback(unsigned char status);

/*******************************************************************************
 * variables
 */

// .bss
tBTA_PRM_CB bta_prm_cb;

/*******************************************************************************
 * functions
 */

static void bta_lower_level_prm_cback(unsigned char status)
{
	BT_HDR *p_buf;
	UINT16 event;

	APPL_TRACE(EVENT, "bta_lower_level_prm_cback status 0x%x", status);

	if (status == 0)
		event = BTA_PRM_INT_WRITE_EVT;
	else if (status == 1)
		event = BTA_PRM_INT_CLOSE_EVT;
	else if (status == 2)
		event = BTA_PRM_INT_5_EVT;
	else
		return;

	if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL)
	{
		p_buf->event = event;

		bta_sys_sendmsg(p_buf);
	}
}

BOOLEAN bta_prm_hdl_event(BT_HDR *p_msg)
{
	tBTA_PRM_DATA *p_prm_msg = (tBTA_PRM_DATA *)p_msg;
	tBTA_PRM_OPEN_EVT *p_open;
	tBTA_PRM_READ_EVT *p_revt;

	APPL_TRACE(EVENT, "PRM state  0x%x, event 0x%x", bta_prm_cb.state,
	           p_msg->event);

	switch (p_msg->event)
	{
	case BTA_PRM_API_ENABLE_EVT:
		memset(&bta_prm_cb, 0, sizeof bta_prm_cb);
		bta_prm_cb.app_id = p_prm_msg->start_evt.fs_app_id;
		bta_prm_cb.p_cback = p_prm_msg->start_evt.p_cback;

		if (!p_prm_msg->start_evt.p_name)
		{
			bta_prm_cb.internal_patch = TRUE;
			bta_prm_cb.state = BTA_PRM_STATE_3;

			BCM2045_PrmInit(&bta_lower_level_prm_cback, 1);
		}
		else
		{
			bta_prm_cb.internal_patch = FALSE;
			bta_prm_cb.state = BTA_PRM_STATE_1;

			bta_fs_co_open(p_prm_msg->start_evt.p_name, BTA_FS_O_RDONLY, 0,
			               BTA_PRM_INT_OPEN_EVT, bta_prm_cb.app_id);

			GKI_freebuf(p_prm_msg->start_evt.p_name);
		}

		break;

	case BTA_PRM_INT_OPEN_EVT:
		p_open = &p_prm_msg->open_evt;

		if (p_open->status == BTA_PRM_STATUS_0)
		{
			bta_prm_cb.state = BTA_PRM_STATE_3;
			bta_prm_cb.file_length = p_open->file_size;
			bta_prm_cb.fd = p_open->fd;

			BCM2045_PrmInit(&bta_lower_level_prm_cback, 0);
		}
		else
		{
			if (bta_prm_cb.p_cback)
				(*bta_prm_cb.p_cback)(1);
		}

		break;

	case BTA_PRM_INT_CLOSE_EVT:
		if (bta_prm_cb.p_cback)
			(*bta_prm_cb.p_cback)(0);

		memset(&bta_prm_cb, 0, sizeof bta_prm_cb);

		break;

	case BTA_PRM_INT_WRITE_EVT:
		memset(bta_prm_cb.patch_data, 0, sizeof bta_prm_cb.patch_data);

		bta_prm_cb.state = BTA_PRM_STATE_2;

		if (bta_prm_cb.read_length == bta_prm_cb.file_length)
		{
			bta_prm_cb.state = BTA_PRM_STATE_3;

			bta_fs_co_close(bta_prm_cb.fd, bta_prm_cb.app_id);
			bta_prm_cb.fd = 0;

			BCM2045_PrmLaunchRam();
		}
		else
		{
			bta_fs_co_read(bta_prm_cb.fd, bta_prm_cb.patch_data,
			               BTA_PRM_MAX_PATCH_SIZE, BTA_PRM_INT_READ_EVT,
			               bta_prm_cb.app_id);
		}

		break;

	case BTA_PRM_INT_READ_EVT:
		p_revt = &p_prm_msg->read_evt;

		bta_prm_cb.read_length += p_revt->num_read;
		bta_prm_cb.state = BTA_PRM_STATE_3;

		if (p_revt->num_read != 0)
		{
			BCM2045_PrmLoadData(bta_prm_cb.patch_data, p_revt->num_read);
		}
		else if (p_revt->num_read == 0 && p_revt->status == BTA_PRM_STATUS_4)
		{
			bta_fs_co_close(bta_prm_cb.fd, bta_prm_cb.app_id);
			bta_prm_cb.fd = 0;

			BCM2045_PrmLaunchRam();
		}
		else if (p_revt->status == BTA_PRM_STATUS_1)
		{
			bta_fs_co_close(bta_prm_cb.fd, bta_prm_cb.app_id);
			bta_prm_cb.fd = 0;

			if (bta_prm_cb.p_cback)
				(*bta_prm_cb.p_cback)(1);
		}

		break;

	case BTA_PRM_INT_5_EVT: // DISABLE?
		if (bta_prm_cb.fd != 0)
			bta_fs_co_close(bta_prm_cb.fd, bta_prm_cb.app_id);

		if (bta_prm_cb.p_cback)
			(*bta_prm_cb.p_cback)(1);

		memset(&bta_prm_cb, 0, sizeof bta_prm_cb);

		break;
	}

	return TRUE;
}
