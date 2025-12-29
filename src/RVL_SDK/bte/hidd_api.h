#ifndef BTE_HID_DEVICE_API_H
#define BTE_HID_DEVICE_API_H

/*******************************************************************************
 * headers
 */

#include "bt_types.h"
#include "data_types.h"

#include "hiddefs.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT8 tHID_CHANNEL;
enum
{
	HID_CHANNEL_INTR,
	HID_CHANNEL_CTRL,
};

typedef struct rep_data
{
	UINT8	rep_type;	// size 0x01, offset 0x00
	UINT8	rep_id;		// size 0x01, offset 0x01
} tHID_DEV_REP_DATA; // size 0x02

typedef struct snd_data_params
{
	UINT8	ctrl_ch;	// size 0x01, offset 0x00
	UINT8	trans_type;	// size 0x01, offset 0x01
	UINT8	param;		// size 0x01, offset 0x02
	BT_HDR	*buf;		// size 0x04, offset 0x04
} tSND_DATA_PARAMS; // size 0x08

typedef struct pwr_md
{
	UINT16	max;		// size 0x02, offset 0x00
	UINT16	min;		// size 0x02, offset 0x02
	UINT16	attempt;	// size 0x02, offset 0x04
	UINT16	timeout;	// size 0x02, offset 0x06
	UINT8	mode;		// size 0x01, offset 0x08
} tHID_DEV_PWR_MD; // size 0x0a

typedef struct curr_pm_setting
{
	UINT8	mode;		// size 0x01, offset 0x00
	UINT16	interval;	// size 0x02, offset 0x02
} tHID_DEV_CURR_PM_SETTING; // size 0x04

typedef struct
{
	FLOW_SPEC	ctrl_ch;	// size 0x18, offset 0x00
	FLOW_SPEC	int_ch;		// size 0x18, offset 0x18
	FLOW_SPEC	hci;		// size 0x18, offset 0x30
} tQOS_INFO; // size 0x048

typedef union
{
	BD_ADDR				host_bdaddr;	// size 0x06, offset 0x00
	BT_HDR				*buffer;		// size 0x04, offset 0x00
	tHID_DEV_REP_DATA	get_rep;		// size 0x02, offset 0x00
	UINT8				pm_err_code;	// size 0x01, offset 0x00
	UINT16				pm_interval;	// size 0x02, offset 0x00
} tHID_DEV_REG_INFO_CBACK_DATA;

typedef void tHID_DEV_REG_INFO_CBACK(UINT8 event, UINT32 data,
                                     tHID_DEV_REG_INFO_CBACK_DATA *pdata);

typedef struct
{
	BD_ADDR					host_addr;	// size 0x06, offset 0x00
	tQOS_INFO				*qos_info;	// size 0x04, offset 0x08
	tHID_DEV_REG_INFO_CBACK	*app_cback;	// size 0x04, offset 0x0C
} tHID_DEV_REG_INFO;	// size 0x010

/*******************************************************************************
 * functions
 */

UINT32 HID_DevSetSDPRecord(tHID_DEV_SDP_INFO *p_sdp_info);
void HID_DevInit(void);
tHID_STATUS HID_DevRegister(tHID_DEV_REG_INFO *p_reg_info);
tHID_STATUS HID_DevDeregister(void);
tHID_STATUS HID_DevConnect(void);
tHID_STATUS HID_DevDisconnect(void);
tHID_STATUS HID_DevHandShake(UINT8 res_code);
tHID_STATUS HID_DevVirtualUnplug(void);
tHID_STATUS HID_DevSendData(tHID_CHANNEL control_ch, UINT8 rep_type,
                            BT_HDR *data_buf);
tHID_STATUS HID_DevSetSecurityLevel(char *serv_name, UINT8 sec_lvl);
tHID_STATUS HID_DevSetPowerMgmtParams(UINT8 conn_substate,
                                      tHID_DEV_PWR_MD pm_params);

#ifdef __cplusplus
	}
#endif

#endif // BTE_HID_DEVICE_API_H
