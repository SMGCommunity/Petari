#ifndef BTE_SDP_API_H
#define BTE_SDP_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/sdp_api.h
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

#include "bt_target.h"
#include "bt_types.h"
#include "data_types.h"

/*******************************************************************************
 * macros
 */

#define SDP_PSM							0x0001

#define SDP_DISC_ATTR_LEN_MASK			0x0fff
#define SDP_DISC_ATTR_TYPE(len_type)	((len_type) >> 12)
#define SDP_DISC_ATTR_LEN(len_type)		((len_type) & SDP_DISC_ATTR_LEN_MASK)

#define SDP_MAX_LIST_ELEMS				3

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT16 tSDP_STATUS;
enum
{
	SDP_SUCCESS					=  0,
	SDP_INVALID_SERV_REC_HDL	=  2,
	SDP_INVALID_REQ_SYNTAX		=  3,
	SDP_INVALID_PDU_SIZE		=  4,
	SDP_INVALID_CONT_STATE		=  5,
	SDP_NO_RESOURCES			=  6,
	SDP_DI_REG_FAILED			=  7,
	SDP_DI_DISC_FAILED			=  8,
	SDP_NO_DI_RECORD_FOUND		=  9,
	SDP_ERR_ATTR_NOT_PRESENT	= 10,
	SDP_ILLEGAL_PARAMETER		= 11,

	SDP_NO_RECS_MATCH			= 0xfff0,
	SDP_CONN_FAILED				= 0xfff1,
	SDP_CFG_FAILED				= 0xfff2,
	SDP_GENERIC_ERROR			= 0xfff3,
	SDP_DB_FULL					= 0xfff4,
	SDP_SECURITY_ERR			= 0xfff6,
	SDP_CONN_REJECTED			= 0xfff7,

#define SDP_SUCCESS		(SDP_SUCCESS + 0)
#define SDP_CONN_FAILED	(SDP_CONN_FAILED + 0)
};

typedef void tSDP_DISC_CMPL_CB(UINT16 result);

typedef tBT_UUID tSDP_UUID;

typedef struct
{
	union
	{
		UINT8					u8;				// size 0x01
		UINT16					u16;			// size 0x02
		UINT32					u32;			// size 0x04
		UINT8					array[4];		// size 0x04
		struct t_sdp_disc_attr	*p_sub_attr;	// size 0x04
	} v; // size 0x04, offset 0x00
} tSDP_DISC_ATVAL; // size 0x04

typedef struct t_sdp_disc_attr
{
	struct t_sdp_disc_attr	*p_next_attr;	// size 0x04, offset 0x00
	UINT16					attr_id;		// size 0x02, offset 0x04
	UINT16					attr_len_type;	// size 0x02, offset 0x06
	tSDP_DISC_ATVAL			attr_value;		// size 0x04, offset 0x08
} tSDP_DISC_ATTR; // size 0x0c

typedef struct t_sdp_disc_rec
{
	tSDP_DISC_ATTR			*p_first_attr;	// size 0x04, offset 0x00
	struct t_sdp_disc_rec	*p_next_rec;	// size 0x04, offset 0x04
	UINT32					time_read;		// size 0x04, offset 0x08
	BD_ADDR					remote_bd_addr;	// size 0x06, offset 0x0c
	/* 2 bytes padding */
} tSDP_DISC_REC; // size 0x14

typedef struct
{
	UINT32			mem_size;							// size 0x04, offset 0x00
	UINT32			mem_free;							// size 0x04, offset 0x04
	tSDP_DISC_REC	*p_first_rec;						// size 0x04, offset 0x08
	UINT16			num_uuid_filters;					// size 0x02, offset 0x0c
	/* 2 bytes padding */
	tSDP_UUID		uuid_filters[SDP_MAX_UUID_FILTERS];	// size 0x3c, offset 0x10
	UINT16			num_attr_filters;					// size 0x02, offset 0x4c
	UINT16			attr_filters[SDP_MAX_ATTR_FILTERS];	// size 0x18, offset 0x4e
	/* 2 bytes padding */
	UINT8			*p_free_mem;						// size 0x04, offset 0x68
} tSDP_DISCOVERY_DB; // size 0x6c

typedef struct
{
	UINT16	protocol_uuid;						// size 0x02, offset 0x00
	UINT16	num_params;							// size 0x02, offset 0x02
	UINT16	params[SDP_MAX_PROTOCOL_PARAMS];	// size 0x04, offset 0x04
} tSDP_PROTOCOL_ELEM; // size 0x08

typedef struct
{
	UINT16				num_elems;						// size 0x02, offset 0x00
	tSDP_PROTOCOL_ELEM	list_elem[SDP_MAX_LIST_ELEMS];	// size 0x18, offset 0x02
} tSDP_PROTO_LIST_ELEM; // size 0x1a

typedef struct t_sdp_di_record
{
	UINT16	vendor;										// size 0x02, offset 0x00
	UINT16	vendor_id_source;							// size 0x02, offset 0x02
	UINT16	product;									// size 0x02, offset 0x04
	UINT16	version;									// size 0x02, offset 0x06
	BOOLEAN	primary_record;								// size 0x01, offset 0x08
	char	client_executable_url[SDP_MAX_ATTR_LEN];	// size 0x50, offset 0x09
	char	service_description[SDP_MAX_ATTR_LEN];		// size 0x50, offset 0x59
	char	documentation_url[SDP_MAX_ATTR_LEN];		// size 0x50, offset 0xa9
} tSDP_DI_RECORD; // size 0xfa

typedef struct t_sdp_di_get_record
{
	UINT16			spec_id;	// size 0x02, offset 0x00
	tSDP_DI_RECORD	rec;		// size 0xfa, offset 0x02
} tSDP_DI_GET_RECORD; // size 0xfc

/*******************************************************************************
 * functions
 */

tSDP_STATUS SDP_SetLocalDiRecord(tSDP_DI_RECORD *p_device_info,
                                 UINT32 *p_handle);
tSDP_STATUS SDP_GetLocalDiRecord(tSDP_DI_GET_RECORD *p_device_info,
                                 UINT32 *p_handle);
BOOLEAN SDP_AddUuidSequence(UINT32 handle, UINT16 attr_id, UINT16 num_uuids,
                            UINT16 *p_uuids);
BOOLEAN SDP_AddAttribute(UINT32 handle, UINT16 attr_id, UINT8 attr_type,
                         UINT32 attr_len, UINT8 *p_val);
UINT32 SDP_CreateRecord(void);
BOOLEAN SDP_AddServiceClassIdList(UINT32 handle, UINT16 num_services,
                                  UINT16 *p_service_uuids);
BOOLEAN SDP_AddProtocolList(UINT32 handle, UINT16 num_elem,
                            tSDP_PROTOCOL_ELEM *p_elem_list);
BOOLEAN SDP_AddProfileDescriptorList(UINT32 handle, UINT16 profile_uuid,
                                     UINT16 version);
BOOLEAN SDP_DeleteRecord(UINT32 handle);
BOOLEAN SDP_AddLanguageBaseAttrIDList(UINT32 handle, UINT16 lang,
                                      UINT16 char_enc, UINT16 base_id);
BOOLEAN SDP_AddAdditionProtoLists(UINT32 handle, UINT16 num_elem,
                                  tSDP_PROTO_LIST_ELEM *p_proto_list);
BOOLEAN SDP_DeleteAttribute(UINT32 handle, UINT16 attr_id);

// ---

BOOLEAN SDP_InitDiscoveryDb(tSDP_DISCOVERY_DB *p_db, UINT32 len,
                            UINT16 num_uuid, tSDP_UUID *p_uuid_list,
                            UINT16 num_attr, UINT16 *p_attr_list);
BOOLEAN SDP_ServiceSearchRequest(UINT8 *p_bd_addr, tSDP_DISCOVERY_DB *p_db,
                                 tSDP_DISC_CMPL_CB *p_cb);
BOOLEAN SDP_ServiceSearchAttributeRequest(UINT8 *p_bd_addr,
                                          tSDP_DISCOVERY_DB *p_db,
                                          tSDP_DISC_CMPL_CB *p_cb);
tSDP_DISC_REC *SDP_FindAttributeInDb(tSDP_DISCOVERY_DB *p_db, UINT16 attr_id,
                                     tSDP_DISC_REC *p_start_rec);
tSDP_DISC_ATTR *SDP_FindAttributeInRec(tSDP_DISC_REC *p_rec, UINT16 attr_id);
tSDP_DISC_REC *SDP_FindServiceInDb(tSDP_DISCOVERY_DB *p_db, UINT16 service_uuid,
                                   tSDP_DISC_REC *p_start_rec);
tSDP_DISC_REC *SDP_FindServiceUUIDInDb(tSDP_DISCOVERY_DB *p_db,
                                       tBT_UUID *p_uuid,
                                       tSDP_DISC_REC *p_start_rec);

BOOLEAN SDP_FindProtocolListElemInRec(tSDP_DISC_REC *p_rec, UINT16 layer_uuid,
                                      tSDP_PROTOCOL_ELEM *p_elem);
BOOLEAN SDP_FindAddProtoListsElemInRec(tSDP_DISC_REC *p_rec, UINT16 layer_uuid,
                                       tSDP_PROTOCOL_ELEM *p_elem);
BOOLEAN SDP_FindProfileVersionInRec(tSDP_DISC_REC *p_rec, UINT16 profile_uuid,
                                    UINT16 *p_version);
UINT16 SDP_DiDiscover(BD_ADDR remote_device, tSDP_DISCOVERY_DB *p_db,
                      UINT32 len, tSDP_DISC_CMPL_CB *p_cb);
UINT8 SDP_GetNumDiRecords(tSDP_DISCOVERY_DB *p_db);
UINT16 SDP_GetDiRecord(UINT8 get_record_index,
                       tSDP_DI_GET_RECORD *p_device_info,
                       tSDP_DISCOVERY_DB *p_db);
UINT16 SDP_SetLocalDiRecord(tSDP_DI_RECORD *p_device_info, UINT32 *p_handle);
UINT16 SDP_GetLocalDiRecord(tSDP_DI_GET_RECORD *p_device_info, UINT32 *p_handle);
UINT8 SDP_SetTraceLevel(UINT8 new_level);

UINT32 SDP_CreateRecord(void);
BOOLEAN SDP_DeleteRecord(UINT32 handle);
BOOLEAN SDP_AddAttribute(UINT32 handle, UINT16 attr_id, UINT8 attr_type,
                         UINT32 attr_len, UINT8 *p_val);
BOOLEAN SDP_AddSequence(UINT32 handle, UINT16 attr_id, UINT16 num_elem,
                        UINT8 *type, UINT8 *len, UINT8 **p_val);
BOOLEAN SDP_AddUuidSequence(UINT32 handle, UINT16 attr_id, UINT16 num_uuids,
                            UINT16 *p_uuids);
BOOLEAN SDP_AddProtocolList(UINT32 handle, UINT16 num_elem,
                            tSDP_PROTOCOL_ELEM *p_elem_list);
BOOLEAN SDP_AddAdditionProtoLists(UINT32 handle, UINT16 num_elem,
                                  tSDP_PROTO_LIST_ELEM *p_proto_list);
BOOLEAN SDP_AddProfileDescriptorList(UINT32 handle, UINT16 profile_uuid,
                                     UINT16 version);
BOOLEAN SDP_AddLanguageBaseAttrIDList(UINT32 handle, UINT16 lang,
                                      UINT16 char_enc, UINT16 base_id);
BOOLEAN SDP_AddServiceClassIdList(UINT32 handle, UINT16 num_services,
                                  UINT16 *p_service_uuids);
BOOLEAN SDP_DeleteAttribute(UINT32 handle, UINT16 attr_id);

#ifdef __cplusplus
	}
#endif

#endif // BTE_SDP_API_H
