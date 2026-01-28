#ifndef BTE_L2CAP_DEFS_H
#define BTE_L2CAP_DEFS_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/l2cdefs.h
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

#define L2CAP_PKT_OVERHEAD					4
#define L2CAP_CMD_OVERHEAD					4
#define L2CAP_CMD_REJECT_LEN				2
#define L2CAP_CONN_REQ_LEN					4
#define L2CAP_CONN_RSP_LEN					8
#define L2CAP_CONFIG_REQ_LEN				4
#define L2CAP_CONFIG_RSP_LEN				6
#define L2CAP_DISC_REQ_LEN					4
#define L2CAP_DISC_RSP_LEN					4
#define L2CAP_ECHO_REQ_LEN					0
#define L2CAP_ECHO_RSP_LEN					0
#define L2CAP_INFO_RSP_LEN					4

#define L2CAP_PKT_TYPE_SHIFT				12

#define L2CAP_CFG_OPTION_OVERHEAD			2

#define L2CAP_CFG_MTU_OPTION_LEN			2
#define L2CAP_CFG_FLUSH_OPTION_LEN			2
#define L2CAP_CFG_QOS_OPTION_LEN			22
#define L2CAP_CFG_FCR_OPTION_LEN			9

#define L2CAP_DEFAULT_MTU					672

#define L2CAP_CONNLESS_MTU_INFO_SIZE		2
#define L2CAP_EXTENDED_FEATURES_ARRAY_SIZE	4
#define L2CAP_FIXED_CHNL_ARRAY_SIZE			8

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	L2CAP_CMD_REJECT		= 1,
	L2CAP_CMD_CONN_REQ		= 2,
	L2CAP_CMD_CONN_RSP		= 3,
	L2CAP_CMD_CONFIG_REQ	= 4,
	L2CAP_CMD_CONFIG_RSP	= 5,
	L2CAP_CMD_DISC_REQ		= 6,
	L2CAP_CMD_DISC_RSP		= 7,
	L2CAP_CMD_ECHO_REQ		= 8,
	L2CAP_CMD_ECHO_RSP		= 9,
	L2CAP_CMD_INFO_REQ		= 10,
	L2CAP_CMD_INFO_RSP		= 11,
};

enum
{
	L2CAP_PKT_CONTINUE	= 1,
	L2CAP_PKT_START		= 2,
};

enum
{
	L2CAP_CONN_OK				= 0,
	L2CAP_CONN_PENDING			= 1,
	L2CAP_CONN_NO_PSM			= 2,
	L2CAP_CONN_SECURITY_BLOCK	= 3,
	L2CAP_CONN_NO_RESOURCES		= 4,
	L2CAP_CONN_TIMEOUT          = 0xeeee,
};

enum
{
	L2CAP_CMD_REJ_NOT_UNDERSTOOD,
	L2CAP_CMD_REJ_MTU_EXCEEDED,
	L2CAP_CMD_REJ_INVALID_CID,
};

enum
{
	L2CAP_SIGNALLING_CID		= 1,
	L2CAP_CONNECTIONLESS_CID	= 2,

	L2CAP_BASE_APPL_CID			= 64,
};

enum
{
	L2CAP_FIXED_CHNL_SIG_BIT		= 1 << L2CAP_SIGNALLING_CID,
	L2CAP_FIXED_CHNL_CNCTLESS_BIT	= 1 << L2CAP_CONNECTIONLESS_CID,
};

enum
{
	L2CAP_CFG_OK					= 0,
	L2CAP_CFG_UNACCEPTABLE_PARAMS	= 1,
	L2CAP_CFG_UNKNOWN_OPTIONS		= 3,
	L2CAP_CFG_PENDING				= 4,
};

enum
{
	L2CAP_CFG_TYPE_MTU			= 1,
	L2CAP_CFG_TYPE_FLUSH_TOUT	= 2,
	L2CAP_CFG_TYPE_QOS			= 3,
	L2CAP_CFG_TYPE_FCR			= 4,
};

enum
{
	L2CAP_CFG_FLAGS_MASK_CONT	= 1,
};

enum
{
	L2CAP_DISC_OK		= 0,
	L2CAP_DISC_TIMEOUT	= 0xeeee,
};

enum
{
	L2CAP_INFO_RESP_RESULT_SUCCESS,
	L2CAP_INFO_RESP_RESULT_NOT_SUPPORTED,
};

enum
{
	L2CAP_CONNLESS_MTU_INFO_TYPE,
	L2CAP_EXTENDED_FEATURES_INFO_TYPE,
	L2CAP_FIXED_CHANNELS_INFO_TYPE,
};

enum
{
	L2CAP_EXTFEA_ENH_RETRANS	= 1 << 3,
	L2CAP_EXTFEA_STREAM_MODE	= 1 << 4,
	L2CAP_EXTFEA_NO_CRC			= 1 << 5,
	L2CAP_EXTFEA_FIXED_CHNLS	= 1 << 7,
	L2CAP_EXTFEA_UCD_RECEPTION	= 1 << 9,
};

#ifdef __cplusplus
	}
#endif

#endif // BTE_L2CAP_DEFS_H
