#include "bta_aa_int.h"

/*******************************************************************************
 * headers
 */

#include "data_types.h"

#include "bta_aa_api.h"

/*******************************************************************************
 * variables
 */

// .rodata
tBTA_AA_CFG const bta_aa_cfg =
{
	0x00, 0x00, 0x10, 0x18,
	0x00, 0x30, 0x00, 0x02,
	0x00, 0x01, 0x02, 0xa0,
	0x02, 0xa0, 0x00, 0xc8,
};

UINT16 const bta_aa_rc_id[] =
{
	0x021f, 0x0000,
	0x0000, 0x0003,
	0x1870, 0x0000,
	0x0000, 0x0000,
};

// .sdata
tBTA_AA_CFG *p_bta_aa_cfg = (tBTA_AA_CFG *)&bta_aa_cfg;
UINT16 *p_bta_aa_rc_id = (UINT16 *)bta_aa_rc_id;
