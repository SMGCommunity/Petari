#include "hci.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * hci/src/hci_h4.c
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

#include <stddef.h>

#include <decomp.h>

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"
#include "gki_target.h"

#include "btu.h"
#include "gki.h"
#include "l2c_int.h"
#include "misc.h"
#include "uusb.h"

/*******************************************************************************
 * macros
 */

#define HCI_CMD_PREAMBLE_SIZE	3
#define HCI_ACL_PREAMBLE_SIZE	4
#define HCI_SCO_PREAMBLE_SIZE	3
#define HCI_EVT_PREAMBLE_SIZE	2

#define H4_TYPE_COMMAND			1
#define H4_TYPE_ACL_DATA		2
#define H4_TYPE_SCO_DATA		3
#define H4_TYPE_EVENT			4

#undef STREAM_TO_UINT16

// TODO: is there a fix that works for everything?
// why another cast?
#define STREAM_TO_UINT16(p, u16)								\
	do															\
	{															\
		*(u16) = 0												\
		+ /*    */((UINT16)(*((p) + 0)) << 0)					\
		+ (UINT16)((UINT16)(*((p) + 1)) << 8);					\
																\
		(p) += 2;												\
	} while (FALSE)

/*******************************************************************************
 * types
 */

typedef struct
{
	HC_BT_HDR	*at_0x00[3];	// size 0x0c, offset 0x00
	UINT16		at_0x0c[3];		// size 0x06, offset 0x0c
	UINT16		at_0x12;		// size 0x01, offset 0x12
	UINT8		at_0x14[3];		// size 0x03, offset 0x14
	UINT8		at_0x17[3];		// size 0x03, offset 0x17
	UINT8		at_0x1a[3];		// size 0x03, offset 0x1a
	char		pad4_[1];		// size 0x01, offset 0x1d
	UINT8		at_0x1e;		// size 0x01, offset 0x1e
	UINT8		at_0x1f;		// size 0x01, offset 0x1f
	UINT8		at_0x20;		// size 0x01, offset 0x20
	char		pad5_[2];		// size 0x02, offset 0x21
	/* 1 byte padding */
} tHCISU_H2_CB; // size 0x24

/*******************************************************************************
 * local function declarations
 */

// forgotten to be given internal linkage? these aren't used anywhere else
extern void hcisu_h2_usb_cback(UINT8, INT8);
extern UINT16 hcisu_h2_receive_msg(UINT16, tHCISU_H2_CB *p_cb);
extern void hcisu_h2_send_msg_now(tHCISU_H2_CB *cb, HC_BT_HDR *p_msg);
extern void hcisu_h2_send_msg(tHCISU_H2_CB *p_cb, HC_BT_HDR *p_msg);

/*******************************************************************************
 * variables
 */

// .rodata
const tHCI_IF hcisu_h2 =
{
	&hcisu_h2_init,
	&hcisu_h2_open,
	&hcisu_h2_close,
	&hcisu_h2_send,
	&hcisu_h2_handle_event
};

// .bss
static tHCISU_H2_CB hcisu_h2_cb;

// .sdata2
static UINT8 const hcisu_preamble_table[] =
{
	HCI_CMD_PREAMBLE_SIZE,
	HCI_ACL_PREAMBLE_SIZE,
	HCI_SCO_PREAMBLE_SIZE,
	HCI_EVT_PREAMBLE_SIZE,
};

static UINT16 const hcisu_msg_evt_table[] =
{
	MSG_HC_TO_STACK_HCI_ERR,
	MSG_HC_TO_STACK_HCI_ACL,
	MSG_HC_TO_STACK_HCI_SCO,
	MSG_HC_TO_STACK_HCI_EVT,
};

/*******************************************************************************
 * functions
 */

void hcisu_h2_usb_cback(UINT8 param_1, INT8 param_2)
{
	switch (param_1)
	{
	case 0:
		break;

	case 4:
		bta_usb_close_evt(param_2);
		break;
	}
}

UINT16 hcisu_h2_receive_msg(UINT16 param_1, tHCISU_H2_CB *p_cb)
{
	UINT16 bytes_read = 0;
	UINT8 byte;
	UINT16 msg_len;
	UINT16 len;
	UINT8 msg_received;
	UINT16 r27;

	if (p_cb->at_0x1e != 2)
		return 0;

	r27 = param_1 + 1;

	while (TRUE)
	{
		if (UUSB_Read(r27, &byte, 1) == 0)
			break;

		++bytes_read;
		msg_received = FALSE;

		switch (p_cb->at_0x1a[param_1])
		{
		case 0:
			if (r27 == 1)
			{
				p_cb->at_0x00[param_1] = GKI_getpoolbuf(GKI_POOL_ID_2);
				p_cb->at_0x14[param_1] = 4;
			}
			else if (r27 == 2)
			{
				p_cb->at_0x00[param_1] = GKI_getpoolbuf(GKI_POOL_ID_3);
				p_cb->at_0x14[param_1] = 2;
			}
			else if (r27 == 3)
			{
				p_cb->at_0x00[param_1] = GKI_getpoolbuf(GKI_POOL_ID_1);
				p_cb->at_0x14[param_1] = 3;
			}
			else
			{
				break;
			}

			if (p_cb->at_0x00[param_1])
			{
				p_cb->at_0x00[param_1]->len = 0;
				p_cb->at_0x00[param_1]->event =
					hcisu_msg_evt_table[p_cb->at_0x14[param_1] - 1];
				p_cb->at_0x00[param_1]->offset = 0;
				p_cb->at_0x1a[param_1] = 1;
			}
			else
			{
				BT_TRACE(HCI, ERROR,
				         "HCIS: Unable to allocate buffer for incoming HCI "
				         "message.");

				p_cb->at_0x1a[param_1] = 2;
			}

			p_cb->at_0x0c[param_1] =
				hcisu_preamble_table[p_cb->at_0x14[param_1] - 1];

			/* fallthrough */

		case 1:
			*((UINT8 *)(p_cb->at_0x00[param_1] + 1)
			  + p_cb->at_0x00[param_1]->len++) = byte;

			--p_cb->at_0x0c[param_1];

			if (p_cb->at_0x0c[param_1] == 0)
			{
				msg_len = byte;

				if (p_cb->at_0x14[param_1] == 2)
				{
					msg_len = (msg_len << 8) + p_cb->at_0x17[param_1];

					if ((p_cb->at_0x00[param_1] =
					         (HC_BT_HDR *)l2cap_link_chk_pkt_start(
								 (BT_HDR *)p_cb->at_0x00[param_1]))
					    == 0)
					{
						p_cb->at_0x0c[param_1] = msg_len;

						if (msg_len == 0)
							p_cb->at_0x1a[param_1] = 0;
						else
							p_cb->at_0x1a[param_1] = 4;

						break;
					}
				}

				p_cb->at_0x0c[param_1] = msg_len;

				if (sizeof(HC_BT_HDR)
				        + hcisu_preamble_table[p_cb->at_0x14[param_1] - 1]
				        + msg_len
				    > GKI_get_buf_size(p_cb->at_0x00[param_1]))
				{
					GKI_freebuf(p_cb->at_0x00[param_1]);
					p_cb->at_0x00[param_1] = NULL;
					p_cb->at_0x1a[param_1] = 4;

					BT_TRACE(HCI, ERROR,
					         "HCIS: Invalid length for incoming HCI message.");

					break;
				}

				if (msg_len != 0)
				{
					p_cb->at_0x1a[param_1] = 3;
				}
				else
				{
					msg_received = TRUE;
					p_cb->at_0x1a[param_1] = 0;
				}
			}
			else
			{
				p_cb->at_0x17[param_1] = byte;
			}

			break;

		case 2:
			--p_cb->at_0x0c[param_1];

			if (p_cb->at_0x0c[param_1] == 0)
			{
				msg_len = byte;

				if (p_cb->at_0x14[param_1] == 2)
					msg_len = (msg_len << 8) + p_cb->at_0x17[param_1];

				p_cb->at_0x0c[param_1] = msg_len;
				p_cb->at_0x1a[param_1] = 4;
			}
			else
			{
				p_cb->at_0x17[param_1] = byte;
			}

			break;

		case 3:
			*((UINT8 *)(p_cb->at_0x00[param_1] + 1)
			  + p_cb->at_0x00[param_1]->len++) = byte;

			--p_cb->at_0x0c[param_1];

			len = UUSB_Read(param_1 + 1,
			                (UINT8 *)(p_cb->at_0x00[param_1] + 1)
			                    + p_cb->at_0x00[param_1]->len,
			                p_cb->at_0x0c[param_1]);

			p_cb->at_0x00[param_1]->len += len;
			p_cb->at_0x0c[param_1] -= len;
			bytes_read += len;

			if (p_cb->at_0x0c[param_1] == 0)
			{
				if (p_cb->at_0x14[param_1] == 2
				    && l2cap_link_chk_pkt_end() == 0)
				{
					p_cb->at_0x1a[param_1] = 0;
				}
				else
				{
					msg_received = TRUE;
					p_cb->at_0x1a[param_1] = 0;
				}
			}

			break;

		case 4:
			--p_cb->at_0x0c[param_1];

			if (p_cb->at_0x0c[param_1] == 0)
				p_cb->at_0x1a[param_1] = 0;

			break;
		}

		if (msg_received)
		{
			GKI_send_msg(hcisu_h2_cb.at_0x1f, 0, p_cb->at_0x00[param_1]);
			p_cb->at_0x00[param_1] = NULL;
		}
	}

	return bytes_read;
}

void hcisu_h2_send_msg_now(tHCISU_H2_CB *cb, HC_BT_HDR *p_msg)
{
	UINT8 type = 0;
	UINT16 handle;
	UINT16 bytes_to_send;
	UINT8 *p = (UINT8 *)(p_msg + 1) + p_msg->offset;
	UINT16 bytes_sent;
	UINT8 num;

	if (p_msg->event == MSG_STACK_TO_HC_HCI_ACL)
	{
		type = H4_TYPE_ACL_DATA;
		num = 2;
	}
	else if (p_msg->event == MSG_STACK_TO_HC_HCI_SCO)
	{
		type = H4_TYPE_SCO_DATA;
		num = 3;
	}
	else if (p_msg->event == MSG_STACK_TO_HC_HCI_CMD)
	{
		type = H4_TYPE_COMMAND;
		num = 0;
	}

	if (p_msg->event == MSG_STACK_TO_HC_HCI_ACL
	    && p_msg->len > btu_cb.hcit_acl_pkt_size)
	{
		STREAM_TO_UINT16(p, &handle);

		handle = (handle & 0xcfff) | 0x1000;

		while (p_msg->len > btu_cb.hcit_acl_pkt_size)
		{
			bytes_to_send = btu_cb.hcit_acl_pkt_size;
			bytes_sent = UUSB_Write(num, p, bytes_to_send, p_msg);

			p_msg->offset += btu_cb.hcit_acl_data_size;
			p_msg->len -= btu_cb.hcit_acl_data_size;

			p = (UINT8 *)(p_msg + 1) + p_msg->offset;

			UINT16_TO_STREAM(p, handle);

			if (p_msg->len > btu_cb.hcit_acl_pkt_size)
				UINT16_TO_STREAM(p, btu_cb.hcit_acl_data_size);
			else
				UINT16_TO_STREAM(p, p_msg->len - HCI_ACL_PREAMBLE_SIZE);

			if (p_msg->layer_specific)
			{
				if (--p_msg->layer_specific == 0)
				{
					p_msg->event = MSG_HC_TO_STACK_L2C_SEG_XMIT;

					GKI_send_msg(hcisu_h2_cb.at_0x1f, 0, p_msg);

					return;
				}
			}
		}
	}

	bytes_to_send = p_msg->len;
	bytes_sent = UUSB_Write(num, p, bytes_to_send, p_msg);

	GKI_freebuf(p_msg);

	(void)cb;
}

void hcisu_h2_send_msg(tHCISU_H2_CB *p_cb, HC_BT_HDR *p_msg)
{
	hcisu_h2_send_msg_now(p_cb, p_msg);
}

void hcisu_h2_init(UINT8 param_1, UINT8 param_2, UINT16 param_3)
{
	hcisu_h2_cb.at_0x1e = 0;

	hcisu_h2_cb.at_0x1a[0] = 0;
	hcisu_h2_cb.at_0x1a[1] = 0;
	hcisu_h2_cb.at_0x1a[2] = 0;

	hcisu_h2_cb.at_0x1f = param_1;
	hcisu_h2_cb.at_0x20 = param_2;
	hcisu_h2_cb.at_0x12 = param_3;
}

BOOLEAN hcisu_h2_open(tHCI_CFG *_p_cfg)
{
	tHCI_CFG *p_cfg = _p_cfg;
	tUUSB uusb;

	uusb.at_0x0b = 0;
	uusb.at_0x04 = p_cfg->at_0x00;
	uusb.at_0x06 = p_cfg->at_0x02;
	uusb.at_0x08 = hcisu_h2_cb.at_0x1f;

	hcisu_h2_cb.at_0x1e = 2;

	UUSB_Register(&uusb);
	UUSB_Open(&uusb, &hcisu_h2_usb_cback);

	return TRUE;
}

void hcisu_h2_close(void)
{
	hcisu_h2_cb.at_0x1e = 0;

	UUSB_Close();
	UUSB_Unregister();
}

BOOLEAN hcisu_h2_send(HC_BT_HDR *p_msg)
{
	hcisu_h2_send_msg(&hcisu_h2_cb, p_msg);

	return TRUE;
}

unk_t hcisu_h2_handle_event(UINT16 len)
{
	UINT16 bytes_read;

	bytes_read = hcisu_h2_receive_msg(
		len - sizeof(HC_BT_HDR) - hcisu_h2_cb.at_0x12, &hcisu_h2_cb);

	return 0;
}
