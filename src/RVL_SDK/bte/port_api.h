#ifndef BTE_RFCOMM_PORT_API_H
#define BTE_RFCOMM_PORT_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/port_api.h
 */

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/* Includes changes by muff1n1634 */

/*******************************************************************************
 * headers
 */

#include "bt_types.h"
#include "data_types.h"

#include "profiles_api.h"

/*******************************************************************************
 * macros
 */

#define PORT_XON_DC1					0x11

#define PORT_XOFF_DC3					0x13

#define PORT_OBEX_DEFAULT_SIGNAL_STATE	(PORT_DTRDSR_ON | PORT_CTSRTS_ON | PORT_DCD_ON)
#define PORT_SPP_DEFAULT_SIGNAL_STATE	(PORT_DTRDSR_ON | PORT_CTSRTS_ON | PORT_DCD_ON)
#define PORT_PPP_DEFAULT_SIGNAL_STATE	(PORT_DTRDSR_ON | PORT_CTSRTS_ON | PORT_DCD_ON)
#define PORT_DUN_DEFAULT_SIGNAL_STATE	(PORT_DTRDSR_ON | PORT_CTSRTS_ON)

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef int tPORT_ERROR;
enum
{
	PORT_SUCCESS				= PORT_ERR_GROUP +  0,
	PORT_UNKNOWN_ERROR			= PORT_ERR_GROUP +  1,
	PORT_ALREADY_OPENED			= PORT_ERR_GROUP +  2,
	PORT_CMD_PENDING			= PORT_ERR_GROUP +  3,

	PORT_NO_MEM					= PORT_ERR_GROUP +  5,
	PORT_NO_RESOURCES			= PORT_ERR_GROUP +  6,

	PORT_BAD_HANDLE				= PORT_ERR_GROUP +  9,
	PORT_NOT_OPENED				= PORT_ERR_GROUP + 10,
	PORT_LINE_ERR				= PORT_ERR_GROUP + 11,
	PORT_START_FAILED			= PORT_ERR_GROUP + 12,
	PORT_PAR_NEG_FAILED			= PORT_ERR_GROUP + 13,
	PORT_PORT_NEG_FAILED		= PORT_ERR_GROUP + 14,
	PORT_SEC_FAILED				= PORT_ERR_GROUP + 15,
	PORT_PEER_CONNECTION_FAILED	= PORT_ERR_GROUP + 16,

	PORT_PEER_TIMEOUT			= PORT_ERR_GROUP + 18,
	PORT_CLOSED					= PORT_ERR_GROUP + 19,
	PORT_TX_FULL				= PORT_ERR_GROUP + 20,

	PORT_TX_QUEUE_DISABLED		= PORT_ERR_GROUP + 23,
	PORT_PAGE_TIMEOUT			= PORT_ERR_GROUP + 24,
	PORT_INVALID_SCN			= PORT_ERR_GROUP + 25,
};

typedef UINT8 tPORT_BAUD_RATE;
enum
{
	PORT_BAUD_RATE_2400,
	PORT_BAUD_RATE_4800,
	PORT_BAUD_RATE_7200,
	PORT_BAUD_RATE_9600,
	PORT_BAUD_RATE_19200,
	PORT_BAUD_RATE_38400,
	PORT_BAUD_RATE_57600,
	PORT_BAUD_RATE_115200,
	PORT_BAUD_RATE_230400,
};

typedef UINT8 tPORT_BYTE_SIZE;
enum
{
	PORT_5_BITS,
	PORT_6_BITS,
	PORT_7_BITS,
	PORT_8_BITS,
};

typedef UINT8 tPORT_STOP_BITS;
enum
{
	PORT_ONESTOPBIT,
	PORT_ONE5STOPBITS,
};

// TODO: fold into boolean
enum
{
	PORT_PARITY_NO,
	PORT_PARITY_YES,
};

typedef UINT8 tPORT_PARITY_TYPE;
enum
{
	PORT_ODD_PARITY,
	PORT_EVEN_PARITY,
	PORT_MARK_PARITY,
	PORT_SPACE_PARITY,
};

typedef UINT8 tPORT_FC_TYPE;
enum
{
	PORT_FC_OFF					= 0,

	PORT_FC_XONXOFF_ON_INPUT	= 1 << 0,
	PORT_FC_XONXOFF_ON_OUTPUT	= 1 << 1,
	PORT_FC_CTS_ON_INPUT		= 1 << 2,
	PORT_FC_CTS_ON_OUTPUT		= 1 << 3,
	PORT_FC_DSR_ON_INPUT		= 1 << 4,
	PORT_FC_DSR_ON_OUTPUT		= 1 << 5,
};

enum
{
	PORT_EV_RXCHAR		= 1 <<  0,
	PORT_EV_RXFLAG		= 1 <<  1,
	PORT_EV_TXEMPTY		= 1 <<  2,
	PORT_EV_CTS			= 1 <<  3,
	PORT_EV_DSR			= 1 <<  4,
	PORT_EV_RLSD		= 1 <<  5,
	PORT_EV_BREAK		= 1 <<  6,
	PORT_EV_ERR			= 1 <<  7,
	PORT_EV_RING		= 1 <<  8,
	PORT_EV_CONNECTED	= 1 <<  9,
	PORT_EV_CTSS		= 1 << 10,
	PORT_EV_DSRS		= 1 << 11,
	PORT_EV_RLSDS		= 1 << 12,
	PORT_EV_OVERRUN		= 1 << 13,
	PORT_EV_TXCHAR		= 1 << 14,
	PORT_EV_CONNECT_ERR	= 1 << 15,
	PORT_EV_FC			= 1 << 16,
	PORT_EV_FCS			= 1 << 17,
};

enum
{
	PORT_SET_DTRDSR = 1,
	PORT_CLR_DTRDSR,
	PORT_SET_CTSRTS,
	PORT_CLR_CTSRTS,
	PORT_SET_RI,
	PORT_CLR_RI,
	PORT_SET_DCD,
	PORT_CLR_DCD,
	PORT_BREAK,
};

enum
{
	PORT_DTRDSR_ON	= 1 << 0,
	PORT_CTSRTS_ON	= 1 << 1,
	PORT_RING_ON	= 1 << 2,
	PORT_DCD_ON		= 1 << 3,
};

enum
{
	PORT_ERR_BREAK		= 1 << 0,
	PORT_ERR_OVERRUN	= 1 << 1, // CLEANUP: header cleanup
};

enum
{
	PORT_PURGE_TXCLEAR	= 1 << 0,
	PORT_PURGE_RXCLEAR	= 1 << 1,
};

typedef void tPORT_CALLBACK(UINT32 code, UINT16 port_handle);
typedef int tPORT_DATA_CALLBACK(UINT16 port_handle, void *p_data, UINT16 len);

typedef struct
{
	tPORT_BAUD_RATE		baud_rate;		// size 0x01, offset 0x00
	tPORT_BYTE_SIZE		byte_size;		// size 0x01, offset 0x01
	tPORT_STOP_BITS		stop_bits;		// size 0x01, offset 0x02
	UINT8				parity;			// size 0x01, offset 0x03
	tPORT_PARITY_TYPE	parity_type;	// size 0x01, offset 0x04
	UINT8				fc_type;		// size 0x01, offset 0x05
	UINT8				rx_char1;		// size 0x01, offset 0x06
	UINT8				xon_char;		// size 0x01, offset 0x07
	UINT8				xoff_char;		// size 0x01, offset 0x08
} tPORT_STATE; // size 0x09

typedef UINT16 tPORT_STATUS_FLAGS;
enum
{
	PORT_FLAG_CTS_HOLD	= 1 << 0,
	PORT_FLAG_DSR_HOLD	= 1 << 1,
	PORT_FLAG_RLSD_HOLD	= 1 << 2,
};

typedef struct
{
	tPORT_STATUS_FLAGS	flags;			// size 0x02, offset 0x00
	UINT16				in_queue_size;	// size 0x02, offset 0x02
	UINT16				out_queue_size;	// size 0x02, offset 0x04
	UINT16				mtu_size;		// size 0x02, offset 0x06
} tPORT_STATUS; // size 0x08

/*******************************************************************************
 * functions
 */

void RFCOMM_Init(void);
int RFCOMM_CreateConnection(UINT16 uuid, UINT8 scn, BOOLEAN is_server,
                            UINT16 mtu, BD_ADDR bd_addr, UINT16 *p_handle,
                            tPORT_CALLBACK *p_mgmt_cb);
int RFCOMM_RemoveConnection(UINT16 handle);

int PORT_SetEventCallback(UINT16 port_handle, tPORT_CALLBACK *p_port_cb);
int PORT_SetDataCallback(UINT16 port_handle, tPORT_DATA_CALLBACK *p_port_cb);
int PORT_SetEventMask(UINT16 port_handle, UINT32 mask);
int PORT_CheckConnection(UINT16 handle, BD_ADDR bd_addr, UINT16 *p_lcid);
int PORT_SetState(UINT16 handle, tPORT_STATE *p_settings);
int PORT_GetState(UINT16 handle, tPORT_STATE *p_settings);
int PORT_Control(UINT16 handle, UINT8 signal);
int PORT_FlowControl(UINT16 handle, BOOLEAN enable);
int PORT_GetModemStatus(UINT16 handle, UINT8 *p_signal);
int PORT_ClearError(UINT16 handle, UINT16 *p_errors, tPORT_STATUS *p_status);
int PORT_SendError(UINT16 handle, UINT8 errors);
int PORT_GetQueueStatus(UINT16 handle, tPORT_STATUS *p_status);
int PORT_Purge(UINT16 handle, UINT8 purge_flags);
int PORT_ReadData(UINT16 handle, char *p_data, UINT16 max_len, UINT16 *p_len);
int PORT_Read(UINT16 handle, BT_HDR **pp_buf);
int PORT_Write(UINT16 handle, BT_HDR *p_buf);
int PORT_WriteData(UINT16 handle, char *p_data, UINT16 max_len, UINT16 *p_len);
int PORT_Test(UINT16 handle, UINT8 *p_data, UINT16 len);
UINT8 PORT_SetTraceLevel(UINT8 new_level);

#ifdef __cplusplus
	}
#endif

#endif // BTE_RFCOMM_PORT_API_H
