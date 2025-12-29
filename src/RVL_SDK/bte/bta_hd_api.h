#ifndef BTE_BTA_HD_API_H
#define BTE_BTA_HD_API_H

/* References:
 * esp-idf <github.com/espressif/esp-idf>
 * [1] components/bt/host/bluedroid/bta/hd/include/bta_hd_api.h
 */

/*******************************************************************************
 * headers
 */

#include <macros.h>

#include "bt_types.h"
#include "data_types.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

// [1]: names
typedef UINT8 tBTA_HD_STATUS;
enum
{
	BTA_HD_OK,
	BTA_HD_ERROR,
};

typedef UINT8 tBTA_HD_EVT;
enum
{
	BTA_HD_ENABLE_EVT,
	BTA_HD_OPEN_EVT,
	BTA_HD_CLOSE_EVT,
	BTA_HD_3_EVT,
};

typedef struct
{
	tBTA_HD_STATUS	status;	// size 0x01, offset 0x00
} tBTA_HD_ENABLE; // size 0x01

typedef struct
{
	BD_ADDR	bd_addr;	// size 0x06, offset 0x00
} tBTA_HD_OPEN; // size 0x06

typedef struct
{
	BD_ADDR	bd_addr;	// size 0x06, offset 0x00
} tBTA_HD_CLOSE; // size 0x06

typedef union
{
	tBTA_HD_ENABLE	enable;	// size 0x01
	tBTA_HD_OPEN	open;	// size 0x06
	tBTA_HD_CLOSE	close;	// size 0x06
} tBTA_HD; // size 0x06

typedef void tBTA_HD_CBACK(tBTA_HD_EVT event, tBTA_HD *p_data);

/*******************************************************************************
 * functions
 */

void BTA_HdEnable(BD_ADDR bd_addr, UINT8 sec_mask, char const *p_service_name,
                  tBTA_HD_CBACK *p_cback, UINT8 app_id);
void BTA_HdDisable(void);
void BTA_HdOpen(UINT8 sec_mask);
void BTA_HdClose(void);
void BTA_HdSendRegularKey(UINT8 modifier, UINT8 key_code);
void BTA_HdSendSpecialKey(UINT8 key_len, UINT8 *key_seq);
void BTA_HdSendMouseReport(UINT8 is_left, UINT8 is_right, UINT8 is_middle,
                           UINT8 delta_x, UINT8 delta_y, UINT8 delta_wheel);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_HD_API_H
