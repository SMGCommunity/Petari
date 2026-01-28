#ifndef BTE_RFC_DEFS_H
#define BTE_RFC_DEFS_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/rfcdefs.h
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
 * macros
 */

#define PORT_MAX_RFC_PORTS				31

#define RFCOMM_DEFAULT_MTU				127

#define RFCOMM_SABME					0x2f
#define RFCOMM_UA						0x63
#define RFCOMM_DM						0x0f
#define RFCOMM_DISC						0x43
#define RFCOMM_UIH						0xef

#define RFCOMM_CTRL_FRAME_LEN			3
#define RFCOMM_MIN_OFFSET				5
#define RFCOMM_DATA_OVERHEAD			(RFCOMM_MIN_OFFSET + 1)

#define RFCOMM_EA						1
#define RFCOMM_EA_MASK					0x01

#define RFCOMM_CR_MASK					0x02
#define RFCOMM_SHIFT_CR					1

#define RFCOMM_SHIFT_DLCI				2
#define RFCOMM_SHIFT_DLCI2				6

#define RFCOMM_PF						0x10
#define RFCOMM_PF_MASK					0x10
#define RFCOMM_PF_OFFSET				4

#define RFCOMM_SHIFT_LENGTH1			1
#define RFCOMM_SHIFT_LENGTH2			7

#define RFCOMM_PARSE_CTRL_FIELD(ea, cr, dlci, p_data)			\
	do															\
	{															\
		ea = *p_data & RFCOMM_EA;								\
		cr = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;		\
		dlci = *p_data++ >> RFCOMM_SHIFT_DLCI;					\
																\
		if (!ea)												\
			dlci += *p_data++ << RFCOMM_SHIFT_DLCI2;			\
	} while (FALSE)

#define RFCOMM_PARSE_TYPE_FIELD(type, pf, p_data)				\
	do															\
	{															\
		type = *p_data & ~RFCOMM_PF_MASK;						\
		pf = (*p_data++ & RFCOMM_PF_MASK) >> RFCOMM_PF_OFFSET;	\
	} while (FALSE)

#define RFCOMM_PARSE_LEN_FIELD(ea, length, p_data)				\
	do															\
	{															\
		ea = (*p_data & RFCOMM_EA);								\
		length = (*p_data++ >> RFCOMM_SHIFT_LENGTH1);			\
																\
		if (!ea)												\
			length += (*p_data++ << RFCOMM_SHIFT_LENGTH2);		\
	} while (FALSE)

#define RFCOMM_FRAME_IS_CMD(initiator, cr)	\
	(((initiator) && !(cr)) || (!(initiator) && (cr)))

#define RFCOMM_FRAME_IS_RSP(initiator, cr)	\
	(((initiator) && (cr)) || (!(initiator) && !(cr)))

#define RFCOMM_CR(initiator, is_command)	\
	((((initiator) && (is_command)) || (!(initiator) && !(is_command))) << 1)

#define RFCOMM_I_CR(is_command)			((is_command) ? 2 : 0)

#define RFCOMM_MAX_DLCI					61

#define RFCOMM_VALID_DLCI(dlci)	\
	((dlci) == 0 || ((dlci) >= 2 && (dlci) <= RFCOMM_MAX_DLCI))

#define RFCOMM_PN_DLCI_MASK				0x3f

#define RFCOMM_PN_FRAM_TYPE_UIH			0x00
#define RFCOMM_PN_FRAME_TYPE_MASK		0x0f

#define RFCOMM_PN_CONV_LAYER_TYPE_1		0x00
#define RFCOMM_PN_CONV_LAYER_CBFC_I		0xf0
#define RFCOMM_PN_CONV_LAYER_CBFC_R		0xe0
#define RFCOMM_PN_CONV_LAYER_MASK		0xf0

#define RFCOMM_PN_PRIORITY_0			0
#define RFCOMM_PN_PRIORITY_MASK			0x3f

#define RFCOMM_PN_K_MASK				0x07

#define RFCOMM_T1_DSEC					0
#define RFCOMM_N2						0
#define RFCOMM_K_MAX					7

#define RFCOMM_MSC_FC					0x02
#define RFCOMM_MSC_RTC					0x04
#define RFCOMM_MSC_RTR					0x08
#define RFCOMM_MSC_IC					0x40
#define RFCOMM_MSC_DV					0x80

#define RFCOMM_MSC_SHIFT_BREAK			4
#define RFCOMM_MSC_BREAK_MASK			0xf0
#define RFCOMM_MSC_BREAK_PRESENT_MASK	0x02

#define RFCOMM_RPN_BITS_MASK			0x03
#define RFCOMM_RPN_BITS_SHIFT			0

#define RFCOMM_RPN_STOP_BITS_MASK		0x01
#define RFCOMM_RPN_STOP_BITS_SHIFT		2

#define RFCOMM_RPN_PARITY_MASK			0x01
#define RFCOMM_RPN_PARITY_SHIFT			3

#define RFCOMM_RPN_PARITY_TYPE_MASK		0x03
#define RFCOMM_RPN_PARITY_TYPE_SHIFT	4

enum
{
	RFCOMM_FC_OFF					= 0,
	RFCOMM_FC_RTR_ON_INPUT			= 1 << 2,
	RFCOMM_FC_RTR_ON_OUTPUT			= 1 << 3,
	RFCOMM_FC_RTC_ON_INPUT			= 1 << 4,
	RFCOMM_FC_RTC_ON_OUTPUT			= 1 << 5,

	RFCOMM_FC_MASK					= 0x3f
};

enum
{
	RFCOMM_RPN_PM_BIT_RATE			= 1 << 0,
	RFCOMM_RPN_PM_DATA_BITS			= 1 << 1,
	RFCOMM_RPN_PM_STOP_BITS			= 1 << 2,
	RFCOMM_RPN_PM_PARITY			= 1 << 3,
	RFCOMM_RPN_PM_PARITY_TYPE		= 1 << 4,
	RFCOMM_RPN_PM_XON_CHAR			= 1 << 5,
	RFCOMM_RPN_PM_XOFF_CHAR			= 1 << 6,

	RFCOMM_RPN_PM_XONXOFF_ON_INPUT	= 1 << 8,
	RFCOMM_RPN_PM_XONXOFF_ON_OUTPUT	= 1 << 9,
	RFCOMM_RPN_PM_RTR_ON_INPUT		= 1 << 10,
	RFCOMM_RPN_PM_RTR_ON_OUTPUT		= 1 << 11,
	RFCOMM_RPN_PM_RTC_ON_INPUT		= 1 << 12,
	RFCOMM_RPN_PM_RTC_ON_OUTPUT		= 1 << 13,

	RFCOMM_RPN_PM_MASK				= 0x3f7f,
};

#define RFCOMM_RLS_ERROR				0x01

#define RFCOMM_MX_DLCI					0

#define RFCOMM_MX_PN					0x80
#define RFCOMM_MX_PN_LEN				8

#define RFCOMM_MX_TEST					0x20

#define RFCOMM_MX_FCON					0xa0
#define RFCOMM_MX_FCON_LEN				0

#define RFCOMM_MX_FCOFF					0x60
#define RFCOMM_MX_FCOFF_LEN				0

#define RFCOMM_MX_MSC					0xe0
#define RFCOMM_MX_MSC_LEN_NO_BREAK		2
#define RFCOMM_MX_MSC_LEN_WITH_BREAK	3

#define RFCOMM_MX_NSC					0x10
#define RFCOMM_MX_NSC_LEN				1

#define RFCOMM_MX_RPN					0x90
#define RFCOMM_MX_RPN_REQ_LEN			1
#define RFCOMM_MX_RPN_LEN				8

#define RFCOMM_MX_RLS					0x50
#define RFCOMM_MX_RLS_LEN				2

#endif // BTE_RFC_DEFS_H
