#ifndef BTE_BTA_BI_INT_H
#define BTE_BTA_BI_INT_H

/*******************************************************************************
 * headers
 */

#include "data_types.h"

#include "bta_bi_api.h"

/*******************************************************************************
 * external globals
 */

#ifdef __cplusplus
	extern "C" {
#endif

extern UINT8 const bta_bi_image_formats[];
extern UINT8 const bta_bi_attachment_formats[];
extern tBTA_BI_IMAGING_CAPABILITIES const bta_bi_imaging_capabilities[];
extern UINT8 const bta_bi_print_service_id[];
extern UINT8 const bta_bi_archive_service_id[];

extern tBTA_BI_CFG const bta_bi_cfg;
extern tBTA_BI_CFG *p_bta_bi_cfg;

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_BI_INT_H
