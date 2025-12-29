#include "sdp_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/sdp/sdp_api.c
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

#include "revolution/types.h"
#include <cstring>

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "sdp_int.h"
#include "sdpdefs.h"

/*******************************************************************************
 * local function declarations
 */

static BOOLEAN sdp_fill_proto_elem(tSDP_DISC_ATTR* p_attr, UINT16 layer_uuid, tSDP_PROTOCOL_ELEM* p_elem);

/*******************************************************************************
 * functions
 */

BOOLEAN SDP_InitDiscoveryDb(tSDP_DISCOVERY_DB* p_db, UINT32 len, UINT16 num_uuid, tSDP_UUID* p_uuid_list, UINT16 num_attr, UINT16* p_attr_list) {
    UINT16 xx;

    if (!p_db || len < sizeof *p_db || num_attr > SDP_MAX_ATTR_FILTERS || num_uuid > SDP_MAX_UUID_FILTERS) {
        SDP_TRACE(ERROR,
                  "SDP_InitDiscoveryDb Illegal param: p_db 0x%x, len "
                  "%d, num_uuid %d, num_attr %d",
                  p_db, len, num_uuid, num_attr);

        return FALSE;
    }

    memset(p_db, 0, len);

    p_db->mem_size = len - sizeof *p_db;
    p_db->mem_free = p_db->mem_size;
    p_db->p_first_rec = NULL;
    p_db->p_free_mem = (UINT8*)(p_db + 1);

    for (xx = 0; xx < num_uuid; ++xx)
        p_db->uuid_filters[xx] = *p_uuid_list++;

    p_db->num_uuid_filters = num_uuid;

    for (xx = 0; xx < num_attr; ++xx)
        p_db->attr_filters[xx] = *p_attr_list++;

    sdpu_sort_attr_list(num_attr, p_db);

    p_db->num_attr_filters = num_attr;

    return TRUE;
}

BOOLEAN SDP_ServiceSearchRequest(UINT8* p_bd_addr, tSDP_DISCOVERY_DB* p_db, tSDP_DISC_CMPL_CB* p_cb) {
    tCONN_CB* p_ccb;

    p_ccb = sdp_conn_originate(p_bd_addr);
    if (!p_ccb)
        return FALSE;

    p_ccb->disc_state = SDP_DISC_WAIT_CONN;
    p_ccb->p_db = p_db;
    p_ccb->p_cb = p_cb;

    return TRUE;
}

BOOLEAN SDP_ServiceSearchAttributeRequest(UINT8* p_bd_addr, tSDP_DISCOVERY_DB* p_db, tSDP_DISC_CMPL_CB* p_cb) {
    tCONN_CB* p_ccb;

    p_ccb = sdp_conn_originate(p_bd_addr);
    if (!p_ccb)
        return FALSE;

    p_ccb->disc_state = SDP_DISC_WAIT_CONN;
    p_ccb->p_db = p_db;
    p_ccb->p_cb = p_cb;

    p_ccb->is_attr_search = TRUE;

    return TRUE;
}

tSDP_DISC_REC* SDP_FindAttributeInDb(tSDP_DISCOVERY_DB* p_db, UINT16 attr_id, tSDP_DISC_REC* p_start_rec) {
    tSDP_DISC_REC* p_rec;
    tSDP_DISC_ATTR* p_attr;

    if (!p_start_rec)
        p_rec = p_db->p_first_rec;
    else
        p_rec = p_start_rec->p_next_rec;

    for (; p_rec; p_rec = p_rec->p_next_rec) {
        for (p_attr = p_rec->p_first_attr; p_attr; p_attr = p_attr->p_next_attr) {
            if (p_attr->attr_id == attr_id)
                return p_rec;
        }
    }

    return NULL;
}

tSDP_DISC_ATTR* SDP_FindAttributeInRec(tSDP_DISC_REC* p_rec, UINT16 attr_id) {
    tSDP_DISC_ATTR* p_attr;

    for (p_attr = p_rec->p_first_attr; p_attr; p_attr = p_attr->p_next_attr) {
        if (p_attr->attr_id == attr_id)
            return p_attr;
    }

    return NULL;
}

tSDP_DISC_REC* SDP_FindServiceInDb(tSDP_DISCOVERY_DB* p_db, UINT16 service_uuid, tSDP_DISC_REC* p_start_rec) {
    tSDP_DISC_REC* p_rec;
    tSDP_DISC_ATTR* p_attr;
    tSDP_DISC_ATTR* p_sattr;

    if (!p_start_rec)
        p_rec = p_db->p_first_rec;
    else
        p_rec = p_start_rec->p_next_rec;

    for (; p_rec; p_rec = p_rec->p_next_rec) {
        for (p_attr = p_rec->p_first_attr; p_attr; p_attr = p_attr->p_next_attr) {
            if (p_attr->attr_id == ATTR_ID_SERVICE_CLASS_ID_LIST && SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) == DATA_ELE_SEQ_DESC_TYPE) {
                for (p_sattr = p_attr->attr_value.v.p_sub_attr; p_sattr; p_sattr = p_sattr->p_next_attr) {
                    if (SDP_DISC_ATTR_TYPE(p_sattr->attr_len_type) == UUID_DESC_TYPE && SDP_DISC_ATTR_LEN(p_sattr->attr_len_type) == 2 &&
                        p_sattr->attr_value.v.u16 == service_uuid) {
                        return p_rec;
                    }
                }

                break;
            } else if (p_attr->attr_id == ATTR_ID_SERVICE_ID) {
                if (SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) == UUID_DESC_TYPE && SDP_DISC_ATTR_LEN(p_attr->attr_len_type) == 2 &&
                    p_attr->attr_value.v.u16 == service_uuid) {
                    return p_rec;
                }
            }
        }
    }

    return NULL;
}

tSDP_DISC_REC* SDP_FindServiceUUIDInDb(tSDP_DISCOVERY_DB* p_db, tBT_UUID* p_uuid, tSDP_DISC_REC* p_start_rec) {
    tSDP_DISC_REC* p_rec;
    tSDP_DISC_ATTR* p_attr;
    tSDP_DISC_ATTR* p_sattr;

    if (!p_start_rec)
        p_rec = p_db->p_first_rec;
    else
        p_rec = p_start_rec->p_next_rec;

    for (; p_rec; p_rec = p_rec->p_next_rec) {
        for (p_attr = p_rec->p_first_attr; p_attr; p_attr = p_attr->p_next_attr) {
            if (p_attr->attr_id == ATTR_ID_SERVICE_CLASS_ID_LIST && SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) == DATA_ELE_SEQ_DESC_TYPE) {
                for (p_sattr = p_attr->attr_value.v.p_sub_attr; p_sattr; p_sattr = p_sattr->p_next_attr) {
                    if (SDP_DISC_ATTR_TYPE(p_sattr->attr_len_type) == UUID_DESC_TYPE) {
                        if (sdpu_compare_uuid_with_attr(p_uuid, p_sattr))
                            return p_rec;
                    }
                }

                break;
            } else if (p_attr->attr_id == ATTR_ID_SERVICE_ID) {
                if (SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) == UUID_DESC_TYPE) {
                    if (sdpu_compare_uuid_with_attr(p_uuid, p_attr))
                        return p_rec;
                }
            }
        }
    }

    return NULL;
}

static BOOLEAN sdp_fill_proto_elem(tSDP_DISC_ATTR* p_attr, UINT16 layer_uuid, tSDP_PROTOCOL_ELEM* p_elem) {
    tSDP_DISC_ATTR* p_sattr;

    for (p_attr = p_attr->attr_value.v.p_sub_attr; p_attr; p_attr = p_attr->p_next_attr) {
        if (SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) != DATA_ELE_SEQ_DESC_TYPE)
            return FALSE;

        for (p_sattr = p_attr->attr_value.v.p_sub_attr; p_sattr; p_sattr = p_sattr->p_next_attr) {
            if (SDP_DISC_ATTR_TYPE(p_sattr->attr_len_type) == UUID_DESC_TYPE && SDP_DISC_ATTR_LEN(p_sattr->attr_len_type) == 2 &&
                p_sattr->attr_value.v.u16 == layer_uuid) {
                p_elem->protocol_uuid = layer_uuid;
                p_elem->num_params = 0;

                for (p_sattr = p_sattr->p_next_attr; p_sattr; p_sattr = p_sattr->p_next_attr) {
                    if (SDP_DISC_ATTR_TYPE(p_sattr->attr_len_type) != UINT_DESC_TYPE) {
                        break;
                    }

                    if (SDP_DISC_ATTR_LEN(p_sattr->attr_len_type) == 2) {
                        p_elem->params[p_elem->num_params++] = p_sattr->attr_value.v.u16;
                    } else {
                        p_elem->params[p_elem->num_params++] = p_sattr->attr_value.v.u8;
                    }

                    if (p_elem->num_params >= SDP_MAX_PROTOCOL_PARAMS)
                        break;
                }

                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOLEAN SDP_FindProtocolListElemInRec(tSDP_DISC_REC* p_rec, UINT16 layer_uuid, tSDP_PROTOCOL_ELEM* p_elem) {
    tSDP_DISC_ATTR* p_attr;

    for (p_attr = p_rec->p_first_attr; p_attr; p_attr = p_attr->p_next_attr) {
        if (p_attr->attr_id == ATTR_ID_PROTOCOL_DESC_LIST && SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) == DATA_ELE_SEQ_DESC_TYPE) {
            return sdp_fill_proto_elem(p_attr, layer_uuid, p_elem);
        }
    }

    return FALSE;
}

BOOLEAN SDP_FindAddProtoListsElemInRec(tSDP_DISC_REC* p_rec, UINT16 layer_uuid, tSDP_PROTOCOL_ELEM* p_elem) {
    tSDP_DISC_ATTR* p_attr;
    tSDP_DISC_ATTR* p_sattr;
    BOOLEAN ret = FALSE;

    for (p_attr = p_rec->p_first_attr; p_attr; p_attr = p_attr->p_next_attr) {
        if (p_attr->attr_id == ATTR_ID_ADDITION_PROTO_DESC_LISTS && SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) == DATA_ELE_SEQ_DESC_TYPE) {
            for (p_sattr = p_attr->attr_value.v.p_sub_attr; p_sattr; p_sattr = p_sattr->p_next_attr) {
                if (SDP_DISC_ATTR_TYPE(p_sattr->attr_len_type) == DATA_ELE_SEQ_DESC_TYPE) {
                    if ((ret = sdp_fill_proto_elem(p_sattr, layer_uuid, p_elem)) == TRUE) {
                        break;
                    }
                }
            }

            return ret;
        }
    }

    return FALSE;
}

BOOLEAN SDP_FindProfileVersionInRec(tSDP_DISC_REC* p_rec, UINT16 profile_uuid, UINT16* p_version) {
    tSDP_DISC_ATTR* p_attr;
    tSDP_DISC_ATTR* p_sattr;

    for (p_attr = p_rec->p_first_attr; p_attr; p_attr = p_attr->p_next_attr) {
        if (p_attr->attr_id == ATTR_ID_BT_PROFILE_DESC_LIST && SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) == DATA_ELE_SEQ_DESC_TYPE) {
            for (p_attr = p_attr->attr_value.v.p_sub_attr; p_attr; p_attr = p_attr->p_next_attr) {
                if (SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) != DATA_ELE_SEQ_DESC_TYPE) {
                    return FALSE;
                }

                for (p_sattr = p_attr->attr_value.v.p_sub_attr; p_sattr; p_sattr = p_sattr->p_next_attr) {
                    if (SDP_DISC_ATTR_TYPE(p_sattr->attr_len_type) == UUID_DESC_TYPE && SDP_DISC_ATTR_LEN(p_sattr->attr_len_type) == 2 &&
                        p_sattr->attr_value.v.u16 == profile_uuid) {
                        p_sattr = p_sattr->p_next_attr;

                        if (SDP_DISC_ATTR_TYPE(p_sattr->attr_len_type) == UINT_DESC_TYPE && SDP_DISC_ATTR_LEN(p_sattr->attr_len_type) == 2) {
                            *p_version = p_sattr->attr_value.v.u16;

                            return TRUE;
                        } else {
                            return FALSE;
                        }
                    }
                }
            }

            return FALSE;
        }
    }

    return FALSE;
}

UINT16 SDP_DiDiscover(BD_ADDR remote_device, tSDP_DISCOVERY_DB* p_db, UINT32 len, tSDP_DISC_CMPL_CB* p_cb) {
    UINT16 result = SDP_DI_DISC_FAILED;
    UINT16 num_uuids = 1;
    UINT16 di_uuid = UUID_SERVCLASS_PNP_INFORMATION;
    tSDP_UUID init_uuid;

    init_uuid.len = 2;
    init_uuid.uu.uuid16 = di_uuid;

    if (SDP_InitDiscoveryDb(p_db, len, num_uuids, &init_uuid, 0, NULL) && SDP_ServiceSearchRequest(remote_device, p_db, p_cb)) {
        result = SDP_SUCCESS;
    }

    return result;
}

UINT8 SDP_GetNumDiRecords(tSDP_DISCOVERY_DB* p_db) {
    UINT8 num_records = 0;
    tSDP_DISC_REC* p_curr_record = NULL;

    do {
        p_curr_record = SDP_FindServiceInDb(p_db, UUID_SERVCLASS_PNP_INFORMATION, p_curr_record);

        if (p_curr_record)
            num_records++;
    } while (p_curr_record);

    return num_records;
}

UINT16 SDP_GetDiRecord(UINT8 get_record_index, tSDP_DI_GET_RECORD* p_device_info, tSDP_DISCOVERY_DB* p_db) {
    UINT16 result = SDP_NO_DI_RECORD_FOUND;
    UINT8 curr_record_index = 1;
    tSDP_DISC_REC* p_curr_record = NULL;

    do {
        p_curr_record = SDP_FindServiceInDb(p_db, UUID_SERVCLASS_PNP_INFORMATION, p_curr_record);
        if (p_curr_record) {
            if (curr_record_index++ == get_record_index) {
                result = SDP_SUCCESS;
                break;
            }
        }
    } while (p_curr_record);

    if (result == SDP_SUCCESS) {
        tSDP_DISC_ATTR* p_curr_attr = NULL;

        p_curr_attr = SDP_FindAttributeInRec(p_curr_record, ATTR_ID_CLIENT_EXE_URL);
        if (p_curr_attr) {
            BCM_STRNCPY_S(p_device_info->rec.client_executable_url, sizeof p_device_info->rec.client_executable_url,
                          (char*)p_curr_attr->attr_value.v.array, SDP_MAX_ATTR_LEN);
        } else {
            p_device_info->rec.client_executable_url[0] = '\0';
        }

        p_curr_attr = SDP_FindAttributeInRec(p_curr_record, ATTR_ID_SERVICE_DESCRIPTION);
        if (p_curr_attr) {
            BCM_STRNCPY_S(p_device_info->rec.service_description, sizeof p_device_info->rec.service_description,
                          (char*)p_curr_attr->attr_value.v.array, SDP_MAX_ATTR_LEN);
        } else {
            p_device_info->rec.service_description[0] = '\0';
        }

        p_curr_attr = SDP_FindAttributeInRec(p_curr_record, ATTR_ID_DOCUMENTATION_URL);
        if (p_curr_attr) {
            BCM_STRNCPY_S(p_device_info->rec.documentation_url, sizeof p_device_info->rec.documentation_url, (char*)p_curr_attr->attr_value.v.array,
                          SDP_MAX_ATTR_LEN);
        } else {
            p_device_info->rec.documentation_url[0] = '\0';
        }

        p_curr_attr = SDP_FindAttributeInRec(p_curr_record, ATTR_ID_SPECIFICATION_ID);
        if (p_curr_attr)
            p_device_info->spec_id = p_curr_attr->attr_value.v.u16;
        else
            result = SDP_ERR_ATTR_NOT_PRESENT;

        p_curr_attr = SDP_FindAttributeInRec(p_curr_record, ATTR_ID_VENDOR_ID);
        if (p_curr_attr)
            p_device_info->rec.vendor = p_curr_attr->attr_value.v.u16;
        else
            result = SDP_ERR_ATTR_NOT_PRESENT;

        p_curr_attr = SDP_FindAttributeInRec(p_curr_record, ATTR_ID_VENDOR_ID_SOURCE);
        if (p_curr_attr)
            p_device_info->rec.vendor_id_source = p_curr_attr->attr_value.v.u16;
        else
            result = SDP_ERR_ATTR_NOT_PRESENT;

        p_curr_attr = SDP_FindAttributeInRec(p_curr_record, ATTR_ID_PRODUCT_ID);
        if (p_curr_attr)
            p_device_info->rec.product = p_curr_attr->attr_value.v.u16;
        else
            result = SDP_ERR_ATTR_NOT_PRESENT;

        p_curr_attr = SDP_FindAttributeInRec(p_curr_record, ATTR_ID_PRODUCT_VERSION);
        if (p_curr_attr)
            p_device_info->rec.version = p_curr_attr->attr_value.v.u16;
        else
            result = SDP_ERR_ATTR_NOT_PRESENT;

        p_curr_attr = SDP_FindAttributeInRec(p_curr_record, ATTR_ID_PRIMARY_RECORD);
        if (p_curr_attr) {
            p_device_info->rec.primary_record = (BOOLEAN)p_curr_attr->attr_value.v.u8;
        } else {
            result = SDP_ERR_ATTR_NOT_PRESENT;
        }
    }

    return result;
}

UINT16 SDP_SetLocalDiRecord(tSDP_DI_RECORD* p_device_info, UINT32* p_handle) {
    UINT16 result = SDP_SUCCESS;
    UINT32 handle;
    UINT16 di_uuid = UUID_SERVCLASS_PNP_INFORMATION;
    UINT16 di_specid = 9;  // TODO: BLUETOOTH_DI_SPECIFICATION? how is this 9
    UINT8 temp_u16[2];
    UINT8* p_temp;

    *p_handle = 0;
    if (p_device_info == NULL)
        return SDP_ILLEGAL_PARAMETER;

    if (p_device_info->primary_record == TRUE && sdp_cb.server_db.di_primary_handle) {
        handle = sdp_cb.server_db.di_primary_handle;
    } else if ((handle = SDP_CreateRecord()) == 0) {
        return SDP_NO_RESOURCES;
    }

    *p_handle = handle;

    if (!SDP_AddServiceClassIdList(handle, 1, &di_uuid))
        result = SDP_DI_REG_FAILED;

    if (result == SDP_SUCCESS) {
        p_temp = temp_u16;
        UINT16_TO_BE_STREAM(p_temp, di_specid);

        if (!SDP_AddAttribute(handle, ATTR_ID_SPECIFICATION_ID, UINT_DESC_TYPE, sizeof di_specid, temp_u16)) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        if (p_device_info->client_executable_url[0] != '\0') {
            if (strlen(p_device_info->client_executable_url) + 1 > SDP_MAX_ATTR_LEN ||
                !SDP_AddAttribute(handle, ATTR_ID_CLIENT_EXE_URL, URL_DESC_TYPE, strlen(p_device_info->client_executable_url) + 1,
                                  (UINT8*)p_device_info->client_executable_url)) {
                result = SDP_DI_REG_FAILED;
            }
        }
    }

    if (result == SDP_SUCCESS) {
        if (p_device_info->service_description[0] != '\0') {
            if (strlen(p_device_info->service_description) + 1 > SDP_MAX_ATTR_LEN ||
                !SDP_AddAttribute(handle, ATTR_ID_SERVICE_DESCRIPTION, TEXT_STR_DESC_TYPE, strlen(p_device_info->service_description) + 1,
                                  (UINT8*)p_device_info->service_description)) {
                result = SDP_DI_REG_FAILED;
            }
        }
    }

    if (result == SDP_SUCCESS) {
        if (p_device_info->documentation_url[0] != '\0') {
            if (strlen(p_device_info->documentation_url) + 1 > SDP_MAX_ATTR_LEN ||
                !SDP_AddAttribute(handle, ATTR_ID_DOCUMENTATION_URL, URL_DESC_TYPE, strlen(p_device_info->documentation_url) + 1,
                                  (UINT8*)p_device_info->documentation_url)) {
                result = SDP_DI_REG_FAILED;
            }
        }
    }

    if (result == SDP_SUCCESS) {
        p_temp = temp_u16;
        UINT16_TO_BE_STREAM(p_temp, p_device_info->vendor);

        if (!SDP_AddAttribute(handle, ATTR_ID_VENDOR_ID, UINT_DESC_TYPE, sizeof p_device_info->vendor, temp_u16)) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        p_temp = temp_u16;
        UINT16_TO_BE_STREAM(p_temp, p_device_info->product);

        if (!SDP_AddAttribute(handle, ATTR_ID_PRODUCT_ID, UINT_DESC_TYPE, sizeof p_device_info->product, temp_u16)) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        p_temp = temp_u16;
        UINT16_TO_BE_STREAM(p_temp, p_device_info->version);

        if (!SDP_AddAttribute(handle, ATTR_ID_PRODUCT_VERSION, UINT_DESC_TYPE, sizeof p_device_info->version, temp_u16)) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        if (!SDP_AddAttribute(handle, ATTR_ID_PRIMARY_RECORD, BOOLEAN_DESC_TYPE, 1, &p_device_info->primary_record)) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result == SDP_SUCCESS) {
        p_temp = temp_u16;
        UINT16_TO_BE_STREAM(p_temp, p_device_info->vendor_id_source);

        if (!SDP_AddAttribute(handle, ATTR_ID_VENDOR_ID_SOURCE, UINT_DESC_TYPE, sizeof p_device_info->vendor_id_source, temp_u16)) {
            result = SDP_DI_REG_FAILED;
        }
    }

    if (result != SDP_SUCCESS)
        SDP_DeleteRecord(handle);
    else if (p_device_info->primary_record == TRUE)
        sdp_cb.server_db.di_primary_handle = handle;

    return result;
}

UINT16 SDP_GetLocalDiRecord(tSDP_DI_GET_RECORD* p_device_info, UINT32* p_handle) {
    UINT16 result = SDP_NO_DI_RECORD_FOUND;
    tSDP_RECORD* p_rec;
    tSDP_ATTRIBUTE* p_attr;
    UINT8* p_temp;
    INT32 templen;

    if (*p_handle == 0)
        *p_handle = sdp_cb.server_db.di_primary_handle;

    if ((p_rec = sdp_db_find_record(*p_handle)) != NULL) {
        // ERRATUM: uses sizeof(tSDP_DI_RECORD), not sizeof(tSDP_DI_GET_RECORD)
        memset(p_device_info, 0, sizeof(tSDP_DI_RECORD));

        result = SDP_SUCCESS;

        if ((p_attr = sdp_db_find_attr_in_rec(p_rec, ATTR_ID_SPECIFICATION_ID, ATTR_ID_SPECIFICATION_ID)) != NULL) {
            p_temp = p_attr->value_ptr;
            BE_STREAM_TO_UINT16(p_temp, &p_device_info->spec_id);
        }

        if ((p_attr = sdp_db_find_attr_in_rec(p_rec, ATTR_ID_VENDOR_ID, ATTR_ID_VENDOR_ID)) != NULL) {
            p_temp = p_attr->value_ptr;
            BE_STREAM_TO_UINT16(p_temp, &p_device_info->rec.vendor);
        }

        if ((p_attr = sdp_db_find_attr_in_rec(p_rec, ATTR_ID_PRODUCT_ID, ATTR_ID_PRODUCT_ID)) != NULL) {
            p_temp = p_attr->value_ptr;
            BE_STREAM_TO_UINT16(p_temp, &p_device_info->rec.product);
        }

        if ((p_attr = sdp_db_find_attr_in_rec(p_rec, ATTR_ID_PRODUCT_VERSION, ATTR_ID_PRODUCT_VERSION)) != NULL) {
            p_temp = p_attr->value_ptr;
            BE_STREAM_TO_UINT16(p_temp, &p_device_info->rec.version);
        }

        if ((p_attr = sdp_db_find_attr_in_rec(p_rec, ATTR_ID_VENDOR_ID_SOURCE, ATTR_ID_VENDOR_ID_SOURCE)) != NULL) {
            p_temp = p_attr->value_ptr;
            BE_STREAM_TO_UINT16(p_temp, &p_device_info->rec.vendor_id_source);
        }

        if ((p_attr = sdp_db_find_attr_in_rec(p_rec, ATTR_ID_PRIMARY_RECORD, ATTR_ID_PRIMARY_RECORD)) != NULL) {
            p_device_info->rec.primary_record = *p_attr->value_ptr;
        }

        if ((p_attr = sdp_db_find_attr_in_rec(p_rec, ATTR_ID_CLIENT_EXE_URL, ATTR_ID_CLIENT_EXE_URL)) != NULL) {
            templen = p_attr->len < SDP_MAX_ATTR_LEN ? p_attr->len : SDP_MAX_ATTR_LEN;
            p_temp = p_attr->value_ptr;

            BE_STREAM_TO_ARRAY(p_temp, p_device_info->rec.client_executable_url, templen);
        }

        if ((p_attr = sdp_db_find_attr_in_rec(p_rec, ATTR_ID_SERVICE_DESCRIPTION, ATTR_ID_SERVICE_DESCRIPTION)) != NULL) {
            templen = p_attr->len < SDP_MAX_ATTR_LEN ? p_attr->len : SDP_MAX_ATTR_LEN;
            p_temp = p_attr->value_ptr;

            BE_STREAM_TO_ARRAY(p_temp, p_device_info->rec.service_description, templen);
        }

        if ((p_attr = sdp_db_find_attr_in_rec(p_rec, ATTR_ID_DOCUMENTATION_URL, ATTR_ID_DOCUMENTATION_URL)) != NULL) {
            templen = p_attr->len < SDP_MAX_ATTR_LEN ? p_attr->len : SDP_MAX_ATTR_LEN;
            p_temp = p_attr->value_ptr;

            BE_STREAM_TO_ARRAY(p_temp, p_device_info->rec.documentation_url, templen);
        }
    } else {
        *p_handle = 0;
    }

    return result;
}

UINT8 SDP_SetTraceLevel(UINT8 new_level) {
    if (new_level != 0xff)
        sdp_cb.trace_level = new_level;

    return sdp_cb.trace_level;
}
