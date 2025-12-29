#ifndef BTE_BTA_AA_INT_H
#define BTE_BTA_AA_INT_H

/*******************************************************************************
 * headers
 */

#include "data_types.h"

#include "bta_aa_api.h"

/*******************************************************************************
 * external globals
 */

#ifdef __cplusplus
	extern "C" {
#endif

extern tBTA_AA_CFG const bta_aa_cfg;
extern tBTA_AA_CFG *p_bta_aa_cfg;

extern UINT16 const bta_aa_rc_id[];
extern UINT16 *p_bta_aa_rc_id;

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_AA_INT_H
