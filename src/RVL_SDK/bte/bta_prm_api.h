#ifndef BTE_BTA_PRM_API_H
#define BTE_BTA_PRM_API_H

/*******************************************************************************
 * headers
 */

#include "data_types.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef void tBTA_PRM_CBACK(UINT8);

/*******************************************************************************
 * functions
 */

void BTA_PatchRam(tBTA_PRM_CBACK *p_cback, char const *p_name, UINT8 fs_app_id);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_PRM_API_H
