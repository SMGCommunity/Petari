#ifndef BTE_SDP_DEFS_H
#define BTE_SDP_DEFS_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/sdpdefs.h
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

#define BLUETOOTH_DI_SPECIFICATION	0x0103

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

// main
enum
{
	ATTR_ID_SERVICE_RECORD_HDL			= 0x000,
	ATTR_ID_SERVICE_CLASS_ID_LIST		= 0x001,

	ATTR_ID_SERVICE_ID					= 0x003,
	ATTR_ID_PROTOCOL_DESC_LIST			= 0x004,
	ATTR_ID_BROWSE_GROUP_LIST			= 0x005,
	ATTR_ID_LANGUAGE_BASE_ATTR_ID_LIST	= 0x006,

	ATTR_ID_BT_PROFILE_DESC_LIST		= 0x009,
	ATTR_ID_DOCUMENTATION_URL			= 0x00a,
	ATTR_ID_CLIENT_EXE_URL				= 0x00b,

	ATTR_ID_ADDITION_PROTO_DESC_LISTS	= 0x00d,
};

// language
enum
{
	ATTR_ID_SERVICE_NAME				= 0x100,
	ATTR_ID_SERVICE_DESCRIPTION			= 0x101,
	ATTR_ID_PROVIDER_NAME				= 0x102,
};

// device identification
enum
{
	ATTR_ID_SPECIFICATION_ID	= 0x200,
	ATTR_ID_VENDOR_ID			= 0x201,
	ATTR_ID_PRODUCT_ID			= 0x202,
	ATTR_ID_PRODUCT_VERSION		= 0x203,
	ATTR_ID_PRIMARY_RECORD		= 0x204,
	ATTR_ID_VENDOR_ID_SOURCE	= 0x205,
};

enum
{
	DI_VENDOR_ID_SOURCE_BTSIG	= 1,
};

// hid profile
enum
{
	ATTR_ID_HID_DEVICE_RELNUM			= 0x200,
	ATTR_ID_HID_PARSER_VERSION			= 0x201,
	ATTR_ID_HID_DEVICE_SUBCLASS			= 0x202,
	ATTR_ID_HID_COUNTRY_CODE			= 0x203,
	ATTR_ID_HID_VIRTUAL_CABLE			= 0x204,
	ATTR_ID_HID_RECONNECT_INITIATE		= 0x205,
	ATTR_ID_HID_DESCRIPTOR_LIST			= 0x206,

	ATTR_ID_HID_SDP_DISABLE				= 0x208,
	ATTR_ID_HID_BATTERY_POWER			= 0x209,
	ATTR_ID_HID_REMOTE_WAKE				= 0x20a,

	ATTR_ID_HID_LINK_SUPERVISION_TO		= 0x20c,
	ATTR_ID_HID_NORMALLY_CONNECTABLE	= 0x20d,
};

enum
{
	UUID_PROTOCOL_RFCOMM	= 0x003,
	UUID_PROTOCOL_OBEX		= 0x008,
	UUID_PROTOCOL_L2CAP		= 0x100,
};

enum
{
	UUID_SERVCLASS_PUBLIC_BROWSE_GROUP	= 0x1002,

	UUID_SERVCLASS_SERIAL_PORT			= 0x1101,
	UUID_SERVCLASS_LAN_ACCESS_USING_PPP	= 0x1102,
	UUID_SERVCLASS_DIALUP_NETWORKING	= 0x1103,
	UUID_SERVCLASS_IRMC_SYNC			= 0x1104,
	UUID_SERVCLASS_OBEX_OBJECT_PUSH		= 0x1105,
	UUID_SERVCLASS_OBEX_FILE_TRANSFER	= 0x1106,

	UUID_SERVCLASS_HEADSET				= 0x1108,
	UUID_SERVCLASS_CORDLESS_TELEPHONY	= 0x1109,

	UUID_SERVCLASS_AUDIO_SINK			= 0x110b,

	UUID_SERVCLASS_AV_REMOTE_CONTROL	= 0x110e,

	UUID_SERVCLASS_INTERCOM				= 0x1110,
	UUID_SERVCLASS_FAX					= 0x1111,

	UUID_SERVCLASS_PANU					= 0x1115,
	UUID_SERVCLASS_NAP					= 0x1116,
	UUID_SERVCLASS_GN					= 0x1117,
	UUID_SERVCLASS_DIRECT_PRINTING		= 0x1118,

	UUID_SERVCLASS_IMAGING_RESPONDER	= 0x111b,

	UUID_SERVCLASS_HF_HANDSFREE			= 0x111e,

	UUID_SERVCLASS_HUMAN_INTERFACE		= 0x1124,

	UUID_SERVCLASS_SAP					= 0x112d,

	UUID_SERVCLASS_PBAP_PSE				= 0x112f,

	UUID_SERVCLASS_PNP_INFORMATION		= 0x1200,

	UUID_SERVCLASS_VIDEO_SINK			= 0x1304,
};

enum
{
	UINT_DESC_TYPE			= 1,
	TWO_COMP_INT_DESC_TYPE	= 2,
	UUID_DESC_TYPE			= 3,
	TEXT_STR_DESC_TYPE		= 4,
	BOOLEAN_DESC_TYPE		= 5,
	DATA_ELE_SEQ_DESC_TYPE	= 6,
	DATA_ELE_ALT_DESC_TYPE	= 7,
	URL_DESC_TYPE			= 8,
};

enum
{
	SIZE_ONE_BYTE,
	SIZE_TWO_BYTES,
	SIZE_FOUR_BYTES,
	SIZE_EIGHT_BYTES,
	SIZE_SIXTEEN_BYTES,
	SIZE_IN_NEXT_BYTE,
	SIZE_IN_NEXT_WORD,
	SIZE_IN_NEXT_LONG,
};

#ifdef __cplusplus
	}
#endif

#endif // BTE_SDP_DEFS_H
