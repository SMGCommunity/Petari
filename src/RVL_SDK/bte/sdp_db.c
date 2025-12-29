#include "sdp_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/sdp/sdp_db.c
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
#include "wbt_api.h"

/*******************************************************************************
 * local function declarations
 */

static BOOLEAN find_uuid_in_seq(UINT8* p, UINT32 seq_len, UINT8* p_uuid, UINT16 uuid_len, int nest_level);

static int sdp_compose_proto_list(UINT8* p, UINT16 num_elem, tSDP_PROTOCOL_ELEM* p_elem_list);

/*******************************************************************************
 * functions
 */

tSDP_RECORD* sdp_db_service_search(tSDP_RECORD* p_rec, tSDP_UUID_SEQ* p_seq) {
    UINT16 xx;
    UINT16 yy;
    tSDP_ATTRIBUTE* p_attr;
    tSDP_RECORD* p_end = sdp_cb.server_db.record + sdp_cb.server_db.num_records;

    if (!p_rec)
        p_rec = sdp_cb.server_db.record;
    else
        ++p_rec;

    for (; p_rec < p_end; ++p_rec) {
        for (yy = 0; yy < p_seq->num_uids; ++yy) {
            p_attr = p_rec->attribute;

            /* explicitly post-increment */
            for (xx = 0; xx < p_rec->num_attributes; xx++, p_attr++) {
                if (p_attr->type == UUID_DESC_TYPE) {
                    if (sdpu_compare_uuid_arrays(p_attr->value_ptr, p_attr->len, &p_seq->uuid_entry[yy].value[0], p_seq->uuid_entry[yy].len))
                        break;
                } else if (p_attr->type == DATA_ELE_SEQ_DESC_TYPE) {
                    if (find_uuid_in_seq(p_attr->value_ptr, p_attr->len, p_seq->uuid_entry[yy].value, p_seq->uuid_entry[yy].len, 0))
                        break;
                }
            }

            if (xx == p_rec->num_attributes)
                break;
        }

        if (yy == p_seq->num_uids)
            return p_rec;
    }

    return NULL;
}

static BOOLEAN find_uuid_in_seq(UINT8* p, UINT32 seq_len, UINT8* p_uuid, UINT16 uuid_len, int nest_level) {
    UINT8* p_end = p + seq_len;
    UINT8 type;
    UINT32 len;

    if (nest_level > 3)
        return FALSE;

    while (p < p_end) {
        type = *p++;
        p = sdpu_get_len_from_type(p, type, &len);
        type = type >> 3;

        if (type == UUID_DESC_TYPE) {
            if (sdpu_compare_uuid_arrays(p, len, p_uuid, uuid_len))
                return TRUE;
        } else if (type == DATA_ELE_SEQ_DESC_TYPE) {
            if (find_uuid_in_seq(p, len, p_uuid, uuid_len, nest_level + 1))
                return TRUE;
        }

        p = p + len;
    }

    return FALSE;
}

tSDP_RECORD* sdp_db_find_record(UINT32 handle) {
    tSDP_RECORD* p_rec;
    tSDP_RECORD* p_end = sdp_cb.server_db.record + sdp_cb.server_db.num_records;

    for (p_rec = sdp_cb.server_db.record; p_rec < p_end; ++p_rec) {
        if (p_rec->record_handle == handle)
            return p_rec;
    }

    return NULL;
}

tSDP_ATTRIBUTE* sdp_db_find_attr_in_rec(tSDP_RECORD* p_rec, UINT16 start_attr, UINT16 end_attr) {
    tSDP_ATTRIBUTE* p_at;
    UINT16 xx;

    /* explicitly post-increment */
    for (xx = 0, p_at = p_rec->attribute; xx < p_rec->num_attributes; xx++, p_at++) {
        if (p_at->id >= start_attr && p_at->id <= end_attr)
            return p_at;
    }

    return NULL;
}

static int sdp_compose_proto_list(UINT8* p, UINT16 num_elem, tSDP_PROTOCOL_ELEM* p_elem_list) {
    UINT16 xx;
    UINT16 yy;
    UINT16 len;
    BOOLEAN is_rfcomm_scn;
    UINT8* p_head = p;
    UINT8* p_len;

    /* explicitly post-increment */
    for (xx = 0; xx < num_elem; xx++, p_elem_list++) {
        len = 3 + p_elem_list->num_params * 3;
        UINT8_TO_BE_STREAM(p, DATA_ELE_SEQ_DESC_TYPE << 3 | SIZE_IN_NEXT_BYTE);

        p_len = p;
        *p++ = len;

        UINT8_TO_BE_STREAM(p, UUID_DESC_TYPE << 3 | SIZE_TWO_BYTES);
        UINT16_TO_BE_STREAM(p, p_elem_list->protocol_uuid);

        if (p_elem_list->protocol_uuid == UUID_PROTOCOL_RFCOMM)
            is_rfcomm_scn = TRUE;
        else
            is_rfcomm_scn = FALSE;

        for (yy = 0; yy < p_elem_list->num_params; ++yy) {
            if (is_rfcomm_scn) {
                UINT8_TO_BE_STREAM(p, (UINT_DESC_TYPE << 3) | SIZE_ONE_BYTE);
                UINT8_TO_BE_STREAM(p, p_elem_list->params[yy]);

                *p_len -= 1;
            } else {
                UINT8_TO_BE_STREAM(p, (UINT_DESC_TYPE << 3) | SIZE_TWO_BYTES);
                UINT16_TO_BE_STREAM(p, p_elem_list->params[yy]);
            }
        }
    }

    return p - p_head;
}

UINT32 SDP_CreateRecord(void) {
    UINT32 handle;
    UINT8 buf[4];
    tSDP_DB* p_db = &sdp_cb.server_db;

    if (p_db->num_records < SDP_MAX_RECORDS) {
        memset(&p_db->record[p_db->num_records], 0, sizeof p_db->record[p_db->num_records]);

        if (p_db->num_records)
            handle = p_db->record[p_db->num_records - 1].record_handle + 1;
        else
            handle = 0x10000;

        p_db->record[p_db->num_records].record_handle = handle;

        ++p_db->num_records;

        UINT32_TO_BE_FIELD(buf, handle);
        SDP_AddAttribute(handle, ATTR_ID_SERVICE_RECORD_HDL, UINT_DESC_TYPE, 4, buf);

        return p_db->record[p_db->num_records - 1].record_handle;
    }

    return 0;
}

BOOLEAN SDP_DeleteRecord(UINT32 handle) {
    UINT16 xx;
    UINT16 yy;
    UINT16 zz;
    tSDP_RECORD* p_rec = sdp_cb.server_db.record;

    if (handle == 0 || sdp_cb.server_db.num_records == 0) {
        sdp_cb.server_db.num_records = 0;
        sdp_cb.server_db.di_primary_handle = 0;
        sdp_cb.server_db.brcm_di_registered = 0;

        return TRUE;
    }

    /* explicitly post-increment */
    for (xx = 0; xx < sdp_cb.server_db.num_records; xx++, p_rec++) {
        if (p_rec->record_handle == handle) {
            /* explicitly post-increment */
            for (yy = xx; yy < sdp_cb.server_db.num_records; yy++, p_rec++) {
                *p_rec = *(p_rec + 1);

                for (zz = 0; zz < p_rec->num_attributes; ++zz)
                    p_rec->attribute[zz].value_ptr -= sizeof *p_rec;
            }

            --sdp_cb.server_db.num_records;

            if (sdp_cb.server_db.di_primary_handle == handle) {
                sdp_cb.server_db.di_primary_handle = 0;
                sdp_cb.server_db.brcm_di_registered = 0;
            }

            return TRUE;
        }
    }

    return FALSE;
}

BOOLEAN SDP_AddAttribute(UINT32 handle, UINT16 attr_id, UINT8 attr_type, UINT32 attr_len, UINT8* p_val) {
    UINT16 xx;
    UINT16 yy;
    UINT16 zz;
    tSDP_RECORD* p_rec = sdp_cb.server_db.record;

    /* explicitly post-increment */
    for (zz = 0; zz < sdp_cb.server_db.num_records; zz++, p_rec++) {
        if (p_rec->record_handle == handle) {
            tSDP_ATTRIBUTE* p_attr = p_rec->attribute;

            /* explicitly post-increment */
            for (xx = 0; xx < p_rec->num_attributes; xx++, p_attr++) {
                if (p_attr->id == attr_id) {
                    SDP_DeleteAttribute(handle, attr_id);
                    break;
                }

                if (p_attr->id > attr_id)
                    break;
            }

            if (p_rec->num_attributes == SDP_MAX_REC_ATTR)
                return FALSE;

            if (xx == p_rec->num_attributes) {
                p_attr = &p_rec->attribute[p_rec->num_attributes];
            } else {
                for (yy = p_rec->num_attributes; yy > xx; --yy)
                    p_rec->attribute[yy] = p_rec->attribute[yy - 1];
            }

            ++p_rec->num_attributes;

            p_attr->id = attr_id;
            p_attr->len = attr_len;
            p_attr->type = attr_type;

            if (p_rec->free_pad_ptr + attr_len >= SDP_MAX_PAD_LEN) {
                SDP_TRACE(WARNING, "SDP_AddAttribute: attr_len:%d too long. truncate to (%d)", attr_len, SDP_MAX_PAD_LEN - p_rec->free_pad_ptr);

                attr_len = SDP_MAX_PAD_LEN - p_rec->free_pad_ptr;
                p_val[SDP_MAX_PAD_LEN - p_rec->free_pad_ptr] = '\0';
                p_val[SDP_MAX_PAD_LEN - p_rec->free_pad_ptr + 1] = '\0';
            }

            if (attr_len > 0 && p_val) {
                memcpy(&p_rec->attr_pad[p_rec->free_pad_ptr], p_val, attr_len);

                p_attr->value_ptr = &p_rec->attr_pad[p_rec->free_pad_ptr];
                p_rec->free_pad_ptr += attr_len;
            }

            if (handle == sdp_cb.server_db.di_primary_handle && attr_id == ATTR_ID_EXT_BRCM_VERSION) {
                sdp_cb.server_db.brcm_di_registered = TRUE;
            }

            return TRUE;
        }
    }

    return FALSE;
}

BOOLEAN SDP_AddSequence(UINT32 handle, UINT16 attr_id, UINT16 num_elem, UINT8* type, UINT8* len, UINT8** p_val) {
    UINT16 xx;
    UINT8 buff[SDP_MAX_ATTR_LEN * 2];
    UINT8* p;
    UINT8* p_head;

    p = buff;

    for (xx = 0; xx < num_elem; ++xx) {
        p_head = p;

        switch (len[xx]) {
        case 1:
            UINT8_TO_BE_STREAM(p, type[xx] << 3 | SIZE_ONE_BYTE);
            break;

        case 2:
            UINT8_TO_BE_STREAM(p, type[xx] << 3 | SIZE_TWO_BYTES);
            break;

        case 4:
            UINT8_TO_BE_STREAM(p, type[xx] << 3 | SIZE_FOUR_BYTES);
            break;

        case 8:
            UINT8_TO_BE_STREAM(p, type[xx] << 3 | SIZE_EIGHT_BYTES);
            break;

        case 16:
            UINT8_TO_BE_STREAM(p, type[xx] << 3 | SIZE_SIXTEEN_BYTES);
            break;

        default:
            UINT8_TO_BE_STREAM(p, type[xx] << 3 | SIZE_IN_NEXT_BYTE);
            UINT8_TO_BE_STREAM(p, len[xx]);
            break;
        }

        ARRAY_TO_BE_STREAM(p, p_val[xx], len[xx]);

        if (p - buff > SDP_MAX_ATTR_LEN) {
            p = p_head;

            if (p_head == buff) {
                SDP_TRACE(ERROR, "SDP_AddSequence - too long(attribute is not added)!!");

                return FALSE;
            } else {
                SDP_TRACE(ERROR, "SDP_AddSequence - too long, add %d elements of %d", xx, num_elem);
            }

            break;
        }
    }

    return SDP_AddAttribute(handle, attr_id, DATA_ELE_SEQ_DESC_TYPE, p - buff, buff);
}

BOOLEAN SDP_AddUuidSequence(UINT32 handle, UINT16 attr_id, UINT16 num_uuids, UINT16* p_uuids) {
    UINT16 xx;
    UINT8 buff[SDP_MAX_ATTR_LEN * 2];
    UINT8* p = buff;
    INT32 max_len = SDP_MAX_ATTR_LEN - 3;

    /* explicitly post-increment */
    for (xx = 0; xx < num_uuids; xx++, p_uuids++) {
        UINT8_TO_BE_STREAM(p, UUID_DESC_TYPE << 3 | SIZE_TWO_BYTES);
        UINT16_TO_BE_STREAM(p, *p_uuids);

        if (p - buff > max_len) {
            SDP_TRACE(WARNING, "SDP_AddUuidSequence - too long, add %d uuids of %d", xx, num_uuids);

            break;
        }
    }

    return SDP_AddAttribute(handle, attr_id, DATA_ELE_SEQ_DESC_TYPE, p - buff, buff);
}

BOOLEAN SDP_AddProtocolList(UINT32 handle, UINT16 num_elem, tSDP_PROTOCOL_ELEM* p_elem_list) {
    UINT8 buff[SDP_MAX_ATTR_LEN * 2];
    int offset;

    offset = sdp_compose_proto_list(buff, num_elem, p_elem_list);

    return SDP_AddAttribute(handle, ATTR_ID_PROTOCOL_DESC_LIST, DATA_ELE_SEQ_DESC_TYPE, offset, buff);
}

BOOLEAN SDP_AddAdditionProtoLists(UINT32 handle, UINT16 num_elem, tSDP_PROTO_LIST_ELEM* p_proto_list) {
    UINT16 xx;
    UINT8 buff[SDP_MAX_ATTR_LEN * 2];
    UINT8* p = buff;
    UINT8* p_len;
    int offset;

    /* explicitly post-increment */
    for (xx = 0; xx < num_elem; xx++, p_proto_list++) {
        UINT8_TO_BE_STREAM(p, DATA_ELE_SEQ_DESC_TYPE << 3 | SIZE_IN_NEXT_BYTE);
        p_len = p++;

        offset = sdp_compose_proto_list(p, p_proto_list->num_elems, p_proto_list->list_elem);
        p += offset;

        *p_len = p - p_len - 1;
    }

    return SDP_AddAttribute(handle, ATTR_ID_ADDITION_PROTO_DESC_LISTS, DATA_ELE_SEQ_DESC_TYPE, p - buff, buff);
}

BOOLEAN SDP_AddProfileDescriptorList(UINT32 handle, UINT16 profile_uuid, UINT16 version) {
    UINT8 buff[SDP_MAX_ATTR_LEN];
    UINT8* p = buff + 2;

    UINT8_TO_BE_STREAM(p, UUID_DESC_TYPE << 3 | SIZE_TWO_BYTES);
    UINT16_TO_BE_STREAM(p, profile_uuid);

    UINT8_TO_BE_STREAM(p, UINT_DESC_TYPE << 3 | SIZE_TWO_BYTES);
    UINT16_TO_BE_STREAM(p, version);

    buff[0] = DATA_ELE_SEQ_DESC_TYPE << 3 | SIZE_IN_NEXT_BYTE;
    buff[1] = p - (buff + 2);

    return SDP_AddAttribute(handle, ATTR_ID_BT_PROFILE_DESC_LIST, DATA_ELE_SEQ_DESC_TYPE, p - buff, buff);
}

BOOLEAN SDP_AddLanguageBaseAttrIDList(UINT32 handle, UINT16 lang, UINT16 char_enc, UINT16 base_id) {
    UINT8 buff[SDP_MAX_ATTR_LEN];
    UINT8* p = buff;

    UINT8_TO_BE_STREAM(p, UINT_DESC_TYPE << 3 | SIZE_TWO_BYTES);
    UINT16_TO_BE_STREAM(p, lang);

    UINT8_TO_BE_STREAM(p, UINT_DESC_TYPE << 3 | SIZE_TWO_BYTES);
    UINT16_TO_BE_STREAM(p, char_enc);

    UINT8_TO_BE_STREAM(p, UINT_DESC_TYPE << 3 | SIZE_TWO_BYTES);
    UINT16_TO_BE_STREAM(p, base_id);

    return SDP_AddAttribute(handle, ATTR_ID_LANGUAGE_BASE_ATTR_ID_LIST, DATA_ELE_SEQ_DESC_TYPE, p - buff, buff);
}

BOOLEAN SDP_AddServiceClassIdList(UINT32 handle, UINT16 num_services, UINT16* p_service_uuids) {
    UINT16 xx;
    UINT8 buff[SDP_MAX_ATTR_LEN * 2];
    UINT8* p = buff;

    /* explicitly post-increment */
    for (xx = 0; xx < num_services; xx++, p_service_uuids++) {
        UINT8_TO_BE_STREAM(p, UUID_DESC_TYPE << 3 | SIZE_TWO_BYTES);
        UINT16_TO_BE_STREAM(p, *p_service_uuids);
    }

    return SDP_AddAttribute(handle, ATTR_ID_SERVICE_CLASS_ID_LIST, DATA_ELE_SEQ_DESC_TYPE, p - buff, buff);
}

BOOLEAN SDP_DeleteAttribute(UINT32 handle, UINT16 attr_id) {
    UINT16 xx;
    UINT16 yy;
    tSDP_RECORD* p_rec = sdp_cb.server_db.record;
    UINT8* pad_ptr;
    UINT32 len;

    /* explicitly post-increment */
    for (xx = 0; xx < sdp_cb.server_db.num_records; xx++, p_rec++) {
        if (p_rec->record_handle == handle) {
            tSDP_ATTRIBUTE* p_attr = p_rec->attribute;

            SDP_TRACE(API, "Deleting attr_id 0x%04x for handle 0x%x", attr_id, handle);

            /* explicitly post-increment */
            for (xx = 0; xx < p_rec->num_attributes; xx++, p_attr++) {
                if (p_attr->id == attr_id) {
                    pad_ptr = p_attr->value_ptr;
                    len = p_attr->len;

                    if (len) {
                        for (yy = 0; yy < p_rec->num_attributes; ++yy) {
                            if (p_rec->attribute[yy].value_ptr > pad_ptr)
                                p_rec->attribute[yy].value_ptr -= len;
                        }
                    }

                    p_rec->num_attributes--;

                    /* explicitly post-increment */
                    for (yy = xx; yy < p_rec->num_attributes; yy++, p_attr++)
                        *p_attr = *(p_attr + 1);

                    if (len) {
                        xx = p_rec->free_pad_ptr - (pad_ptr + len - p_rec->attr_pad);

                        /* explicitly post-increment */
                        for (yy = 0; yy < xx; yy++, pad_ptr++)
                            *pad_ptr = *(pad_ptr + len);

                        p_rec->free_pad_ptr -= len;
                    }

                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}
