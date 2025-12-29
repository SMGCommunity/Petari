#include "bta_ct_int.h"

/*******************************************************************************
 * headers
 */

#include "bta_ct_api.h"

/*******************************************************************************
 * variables
 */

extern signed bta_ct_action;
extern signed bta_ic_action;

// .rodata
tBTA_CT_CFG const bta_ct_cfg =
{
	&bta_ct_action,
	&bta_ic_action,
	0x01, 0x03, 0x00, 0x00
};

// .sdata
tBTA_CT_CFG *p_bta_ct_cfg = (tBTA_CT_CFG *)&bta_ct_cfg;
