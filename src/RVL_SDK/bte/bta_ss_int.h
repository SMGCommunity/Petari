#ifndef BTE_BTA_SS_INT_H
#define BTE_BTA_SS_INT_H

/*******************************************************************************
 * headers
 */

#include "bta_ss_api.h"

/*******************************************************************************
 * external globals
 */

#ifdef __cplusplus
	extern "C" {
#endif

extern tBTA_SS_CFG const bta_ss_cfg;
extern tBTA_SS_CFG *p_bta_ss_cfg;

extern char const * const bta_ss_dev_info_prop[][2];
extern char const * const bta_ss_pb_info_prop[][2];
extern char const * const bta_ss_cal_info_prop[][2];
extern char const * const bta_ss_note_info_prop[][2];
extern char const * const bta_ss_msg_info_prop[][2];

extern char const ***p_bta_ss_dev_info_prop;
extern char const ***p_bta_ss_pb_info_prop;
extern char const ***p_bta_ss_cal_info_prop;
extern char const ***p_bta_ss_note_info_prop;
extern char const ***p_bta_ss_msg_info_prop;

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_SS_INT_H
