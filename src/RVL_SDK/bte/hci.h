#ifndef BTE_HCI_H
#define BTE_HCI_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * hci/include/hci.h
 */

/******************************************************************************
 *
 *  Copyright (C) 2009-2012 Broadcom Corporation
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

#include <decomp.h>

#include "data_types.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	MSG_HC_TO_STACK_HCI_EVT			= 0x1000,
	MSG_HC_TO_STACK_HCI_ACL			= 0x1100,
	MSG_HC_TO_STACK_HCI_SCO			= 0x1200,
	MSG_HC_TO_STACK_HCI_ERR			= 0x1300,
	MSG_HC_TO_STACK_L2C_SEG_XMIT	= 0x1900,
};

enum
{
	MSG_STACK_TO_HC_HCI_CMD			= 0x2000,
	MSG_STACK_TO_HC_HCI_ACL			= 0x2100,
	MSG_STACK_TO_HC_HCI_SCO			= 0x2200,
};

// separate anonymous struct to keep layers separate, i guess
typedef struct
{
	UINT16	event;			// size 0x02, offset 0x00
	UINT16	len;			// size 0x02, offset 0x02
	UINT16	offset;			// size 0x02, offset 0x04
	UINT16	layer_specific;	// size 0x02, offset 0x06
} HC_BT_HDR; // size 0x08


typedef struct
{
	UINT16	at_0x00;	// size 0x02, offset 0x00
	UINT16	at_0x02;	// size 0x02, offset 0x02
} tHCI_CFG; // size 0x04

typedef void tHCI_INIT(UINT8, UINT8, UINT16);
typedef BOOLEAN tHCI_OPEN(tHCI_CFG *p_cfg);
typedef void tHCI_CLOSE(void);
typedef BOOLEAN tHCI_SEND(HC_BT_HDR *p_msg);
typedef unk_t tHCI_HANDLE_EVENT(UINT16 len);

typedef struct
{
	tHCI_INIT			*init;			// size 0x04, offset 0x00
	tHCI_OPEN			*open;			// size 0x04, offset 0x04
	tHCI_CLOSE			*close;			// size 0x04, offset 0x08
	tHCI_SEND			*send;			// size 0x04, offset 0x0c
	tHCI_HANDLE_EVENT	*handle_event;	// size 0x04, offset 0x10
} tHCI_IF; // size 0x14

/*******************************************************************************
 * external globals
 */

extern tHCI_IF const hcisu_h2;

/*******************************************************************************
 * functions
 */

void hcisu_h2_init(UINT8, UINT8, UINT16);
BOOLEAN hcisu_h2_open(tHCI_CFG *p_cfg);
void hcisu_h2_close(void);
BOOLEAN hcisu_h2_send(HC_BT_HDR *p_msg);
unk_t hcisu_h2_handle_event(UINT16 len);

#ifdef __cplusplus
	}
#endif

#endif // BTE_HCI_H
