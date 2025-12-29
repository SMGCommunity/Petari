#include "bta_ft_int.h"

/*******************************************************************************
 * headers
 */

#include "bta_ft_api.h"

/*******************************************************************************
 * variables
 */

extern signed bat_ftc_bi_action;

// .rodata
tBTA_FT_CFG const bta_ft_cfg =
{
	&bat_ftc_bi_action,
	0x00, 0x00, 0x01, 0x03,
	"PBAP PCE",
	0x00, 0x00, 0x0b, 0xb8,
	0x00, 0x00, 0x07, 0xd0,
};

// .sdata
tBTA_FT_CFG *p_bta_ft_cfg = (tBTA_FT_CFG *)&bta_ft_cfg;
