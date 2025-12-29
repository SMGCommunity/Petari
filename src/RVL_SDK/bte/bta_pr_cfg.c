#include "bta_pr_int.h"

/*******************************************************************************
 * headers
 */

#include "bta_pr_api.h"

/*******************************************************************************
 * variables
 */

extern signed p_pr_bi_action;

// .sdata2
tBTA_PR_CFG const bta_pr_cfg =
{
	&p_pr_bi_action,
	0x01, 0x00, 0x00, 0x00
};

// .sdata
tBTA_PR_CFG *p_bta_pr_cfg = (tBTA_PR_CFG *)&bta_pr_cfg;
