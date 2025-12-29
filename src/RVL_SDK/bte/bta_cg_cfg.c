#include "bta_cg_int.h"

/*******************************************************************************
 * headers
 */

#include "bta_cg_api.h"

/*******************************************************************************
 * variables
 */

extern signed bta_cg_action;

// .sdata2
tBTA_CG_CFG const bta_cg_cfg =
{
	&bta_cg_action,
	0x03, 0x00, 0x00, 0x00
};

// .sdata
tBTA_CG_CFG *p_bta_cg_cfg = (tBTA_CG_CFG *)&bta_cg_cfg;
