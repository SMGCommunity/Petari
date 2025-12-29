#include "bta_dg_int.h"

/*******************************************************************************
 * headers
 */

#include "bta_dg_api.h"

/*******************************************************************************
 * variables
 */

// .sdata2
tBTA_DG_CFG const bta_dg_cfg =
{
	0x01, 0x4a, 0x01, 0x4a,
	0x00, 0x70, 0x01, 0x4a,
};

// .sdata
tBTA_DG_CFG *p_bta_dg_cfg = (tBTA_DG_CFG *)&bta_dg_cfg;
