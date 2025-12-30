#include "bta_prm_api.h"

/*******************************************************************************
 * headers
 */

#include <string.h>

#include "data_types.h" // UINT8

#include "bta_prm_int.h"
#include "bta_sys.h"
#include "gki.h"

/*******************************************************************************
 * variables
 */

// .sdata2
static tBTA_SYS_REG const bta_prm_reg =
{
	&bta_prm_hdl_event,
	NULL
};

/*******************************************************************************
 * functions
 */

void BTA_PatchRam(tBTA_PRM_CBACK *p_cback, char const *p_name, UINT8 fs_app_id)
{
	tBTA_PRM_API_ENABLE *p_buf;

	GKI_sched_lock();
	bta_sys_register(BTA_ID_PRM, &bta_prm_reg);
	GKI_sched_unlock();

	if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL)
	{
		memset(p_buf, 0, sizeof *p_buf);

		p_buf->hdr.event = BTA_PRM_API_ENABLE_EVT;
		p_buf->p_cback = p_cback;
		p_buf->fs_app_id = fs_app_id;

		if (p_name)
		{
			p_buf->p_name = GKI_getbuf(strlen(p_name) + 1);
			strcpy(p_buf->p_name, p_name);
		}

		bta_sys_sendmsg(p_buf);
	}
}
