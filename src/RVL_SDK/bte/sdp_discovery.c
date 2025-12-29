#include "sdp_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/sdp/sdp_discovery.c
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

#include "btu.h"
#include "gki.h"
#include "l2c_api.h"
#include "sdp_api.h"
#include "sdpdefs.h"

/*******************************************************************************
 * macros
 */

#define MAX_NEST_LEVELS 5

#define SDP_ADDITIONAL_LIST_MASK 0x80

/*******************************************************************************
 * local function declarations
 */

static UINT8* sdpu_build_uuid_seq(UINT8* p_out, UINT16 num_uuids, tSDP_UUID* p_uuid_list);
static void sdp_snd_service_search_req(tCONN_CB* p_ccb, UINT8 cont_len, UINT8* p_cont);

static void process_service_search_rsp(tCONN_CB* p_ccb, UINT8* p_reply, UINT16 len);
static void process_service_attr_rsp(tCONN_CB* p_ccb, UINT8* p_reply, UINT16 len);
static void process_service_search_attr_rsp(tCONN_CB* p_ccb, UINT8* p_reply, UINT16 len);
static UINT8* save_attr_seq(tCONN_CB* p_ccb, UINT8* p, UINT8* p_msg_end);
static tSDP_DISC_REC* add_record(tSDP_DISCOVERY_DB* p_db, BD_ADDR p_bda);
static UINT8* add_attr(UINT8* p, tSDP_DISCOVERY_DB* p_db, tSDP_DISC_REC* p_rec, UINT16 attr_id, tSDP_DISC_ATTR* p_parent_attr, UINT8 nest_level);

/*******************************************************************************
 * functions
 */

static UINT8* sdpu_build_uuid_seq(UINT8* p_out, UINT16 num_uuids, tSDP_UUID* p_uuid_list) {
    UINT16 xx;
    UINT8* p_len;

    UINT8_TO_BE_STREAM(p_out, DATA_ELE_SEQ_DESC_TYPE << 3 | SIZE_IN_NEXT_BYTE);

    p_len = p_out;
    p_out += 1;

    /* explicitly post-increment */
    for (xx = 0; xx < num_uuids; xx++, p_uuid_list++) {
        if (p_uuid_list->len == 2) {
            UINT8_TO_BE_STREAM(p_out, UUID_DESC_TYPE << 3 | SIZE_TWO_BYTES);
            UINT16_TO_BE_STREAM(p_out, p_uuid_list->uu.uuid16);
        } else if (p_uuid_list->len == 4) {
            UINT8_TO_BE_STREAM(p_out, UUID_DESC_TYPE << 3 | SIZE_FOUR_BYTES);
            UINT32_TO_BE_STREAM(p_out, p_uuid_list->uu.uuid32);
        } else {
            UINT8_TO_BE_STREAM(p_out, UUID_DESC_TYPE << 3 | SIZE_SIXTEEN_BYTES);
            ARRAY_TO_BE_STREAM(p_out, p_uuid_list->uu.uuid128, p_uuid_list->len);
        }
    }

    xx = p_out - p_len - 1;
    UINT8_TO_BE_STREAM(p_len, xx);

    return p_out;
}

static void sdp_snd_service_search_req(tCONN_CB* p_ccb, UINT8 cont_len, UINT8* p_cont) {
    UINT8* p;
    UINT8* p_start;
    UINT8* p_param_len;
    BT_HDR* p_cmd;
    UINT16 param_len;

    if ((p_cmd = GKI_getpoolbuf(SDP_POOL_ID)) == NULL) {
        sdp_disconnect(p_ccb, SDP_NO_RESOURCES);
        return;
    }

    p_cmd->offset = L2CAP_MIN_OFFSET;
    p = p_start = (UINT8*)(p_cmd + 1) + L2CAP_MIN_OFFSET;

    UINT8_TO_BE_STREAM(p, SDP_PDU_SERVICE_SEARCH_REQ);
    UINT16_TO_BE_STREAM(p, p_ccb->transaction_id);
    ++p_ccb->transaction_id;

    p_param_len = p;
    p += 2;
    p = sdpu_build_uuid_seq(p, p_ccb->p_db->num_uuid_filters, p_ccb->p_db->uuid_filters);

    UINT16_TO_BE_STREAM(p, sdp_cb.max_recs_per_search);
    UINT8_TO_BE_STREAM(p, cont_len);

    if (cont_len && p_cont) {
        memcpy(p, p_cont, cont_len);
        p += cont_len;
    }

    param_len = p - p_param_len - 2;
    UINT16_TO_BE_STREAM(p_param_len, param_len);

    p_ccb->disc_state = SDP_DISC_WAIT_HANDLES;
    p_cmd->len = p - p_start;

    L2CA_DataWrite(p_ccb->connection_id, p_cmd);

    btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_SDP, SDP_INACT_TIMEOUT);
}

void sdp_disc_connected(tCONN_CB* p_ccb) {
    if (p_ccb->is_attr_search) {
        p_ccb->disc_state = SDP_DISC_WAIT_SEARCH_ATTR;

        process_service_search_attr_rsp(p_ccb, NULL, 0);
    } else {
        p_ccb->num_handles = 0;
        sdp_snd_service_search_req(p_ccb, 0, NULL);
    }
}

void sdp_disc_server_rsp(tCONN_CB* p_ccb, BT_HDR* p_msg) {
    UINT8* p;
    UINT8 rsp_pdu;
    BOOLEAN invalid_pdu = TRUE;

    btu_stop_timer(&p_ccb->timer_entry);

    p = (UINT8*)(p_msg + 1) + p_msg->offset;

    BE_STREAM_TO_UINT8(p, &rsp_pdu);

    --p_msg->len;

    switch (rsp_pdu) {
    case SDP_PDU_SERVICE_SEARCH_RSP:
        if (p_ccb->disc_state == SDP_DISC_WAIT_HANDLES) {
            process_service_search_rsp(p_ccb, p, p_msg->len);
            invalid_pdu = FALSE;
        }
        break;

    case SDP_PDU_SERVICE_ATTR_RSP:
        if (p_ccb->disc_state == SDP_DISC_WAIT_ATTR) {
            process_service_attr_rsp(p_ccb, p, p_msg->len);
            invalid_pdu = FALSE;
        }
        break;

    case SDP_PDU_SERVICE_SEARCH_ATTR_RSP:
        if (p_ccb->disc_state == SDP_DISC_WAIT_SEARCH_ATTR) {
            process_service_search_attr_rsp(p_ccb, p, p_msg->len);
            invalid_pdu = FALSE;
        }
        break;
    }

    if (invalid_pdu) {
        SDP_TRACE(WARNING, "SDP - Unexp. PDU: %d in state: %d", rsp_pdu, p_ccb->disc_state);
        sdp_disconnect(p_ccb, SDP_GENERIC_ERROR);
    }
}

static void process_service_search_rsp(tCONN_CB* p_ccb, UINT8* p_reply, UINT16 len) {
    UINT16 xx;
    UINT16 total;
    UINT16 cur_handles;
    UINT16 orig;
    UINT8 cont_len;

    p_reply += 4;
    BE_STREAM_TO_UINT16(p_reply, &total);
    BE_STREAM_TO_UINT16(p_reply, &cur_handles);

    orig = p_ccb->num_handles;
    p_ccb->num_handles += cur_handles;

    if (p_ccb->num_handles == 0) {
        SDP_TRACE(WARNING, "SDP - Rcvd ServiceSearchRsp, no matches");
        sdp_disconnect(p_ccb, SDP_NO_RECS_MATCH);
        return;
    }

    if (total > sdp_cb.max_recs_per_search)
        total = sdp_cb.max_recs_per_search;

    if (p_ccb->num_handles > sdp_cb.max_recs_per_search)
        p_ccb->num_handles = sdp_cb.max_recs_per_search;

    for (xx = orig; xx < p_ccb->num_handles; ++xx)
        BE_STREAM_TO_UINT32(p_reply, &p_ccb->handles[xx]);

    BE_STREAM_TO_UINT8(p_reply, &cont_len);

    if (cont_len != 0) {
        if (cont_len > SDP_MAX_CONTINUATION_LEN) {
            sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
            return;
        }

        sdp_snd_service_search_req(p_ccb, cont_len, p_reply);
    } else {
        p_ccb->disc_state = SDP_DISC_WAIT_ATTR;

        process_service_attr_rsp(p_ccb, NULL, 0);
    }
}

static void process_service_attr_rsp(tCONN_CB* p_ccb, UINT8* p_reply, UINT16 len) {
    UINT8* p_start;
    UINT8* p_param_len;
    UINT16 param_len;
    UINT16 list_byte_count;
    BOOLEAN cont_request_needed = FALSE;

    if (p_reply) {
        p_reply += 4;

        BE_STREAM_TO_UINT16(p_reply, &list_byte_count);

        if (p_ccb->list_len + list_byte_count > SDP_MAX_LIST_BYTE_COUNT) {
            sdp_disconnect(p_ccb, SDP_INVALID_PDU_SIZE);
            return;
        }

        memcpy(&p_ccb->rsp_list[p_ccb->list_len], p_reply, list_byte_count);
        p_ccb->list_len += list_byte_count;
        p_reply += list_byte_count;

        if (*p_reply) {
            if (*p_reply > SDP_MAX_CONTINUATION_LEN) {
                sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
                return;
            }
            cont_request_needed = TRUE;
        } else {
            if (!save_attr_seq(p_ccb, p_ccb->rsp_list, p_ccb->rsp_list + p_ccb->list_len)) {
                sdp_disconnect(p_ccb, SDP_DB_FULL);
                return;
            }
            p_ccb->list_len = 0;
            p_ccb->cur_handle++;
        }
    }

    if (p_ccb->cur_handle < p_ccb->num_handles) {
        BT_HDR* p_msg = GKI_getpoolbuf(SDP_POOL_ID);
        UINT8* p;

        if (!p_msg) {
            sdp_disconnect(p_ccb, SDP_NO_RESOURCES);
            return;
        }

        p_msg->offset = L2CAP_MIN_OFFSET;
        p = p_start = (UINT8*)(p_msg + 1) + L2CAP_MIN_OFFSET;

        UINT8_TO_BE_STREAM(p, SDP_PDU_SERVICE_ATTR_REQ);
        UINT16_TO_BE_STREAM(p, p_ccb->transaction_id);
        p_ccb->transaction_id++;

        p_param_len = p;
        p += 2;

        UINT32_TO_BE_STREAM(p, p_ccb->handles[p_ccb->cur_handle]);
        UINT16_TO_BE_STREAM(p, sdp_cb.max_attr_list_size);

        if (p_ccb->p_db->num_attr_filters)
            p = sdpu_build_attrib_seq(p, p_ccb->p_db->attr_filters, p_ccb->p_db->num_attr_filters);
        else
            p = sdpu_build_attrib_seq(p, NULL, 0);

        if (cont_request_needed) {
            memcpy(p, p_reply, *p_reply + 1);
            p += *p_reply + 1;
        } else {
            UINT8_TO_BE_STREAM(p, 0);
        }

        param_len = p - p_param_len - 2;
        UINT16_TO_BE_STREAM(p_param_len, param_len);

        p_msg->len = p - p_start;

        L2CA_DataWrite(p_ccb->connection_id, p_msg);

        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_SDP, SDP_INACT_TIMEOUT);
    } else {
        sdp_disconnect(p_ccb, SDP_SUCCESS);
        return;
    }
}

static void process_service_search_attr_rsp(tCONN_CB* p_ccb, UINT8* p_reply, UINT16 len) {
    UINT8* p;
    UINT8* p_start;
    UINT8* p_end;
    UINT8* p_param_len;
    UINT8 type;
    UINT32 seq_len;
    UINT16 param_len;
    UINT16 lists_byte_count = 0;
    BOOLEAN cont_request_needed = FALSE;

    if (p_reply) {
        p_reply += 4;

        BE_STREAM_TO_UINT16(p_reply, &lists_byte_count);

        if ((p_ccb->list_len + lists_byte_count) > SDP_MAX_LIST_BYTE_COUNT) {
            sdp_disconnect(p_ccb, SDP_INVALID_PDU_SIZE);
            return;
        }

        memcpy(&p_ccb->rsp_list[p_ccb->list_len], p_reply, lists_byte_count);
        p_ccb->list_len += lists_byte_count;
        p_reply += lists_byte_count;

        if (*p_reply) {
            if (*p_reply > SDP_MAX_CONTINUATION_LEN) {
                sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
                return;
            }

            cont_request_needed = TRUE;
        }
    }

    if (cont_request_needed || !p_reply) {
        BT_HDR* p_msg = GKI_getpoolbuf(SDP_POOL_ID);
        UINT8* p;

        if (!p_msg) {
            sdp_disconnect(p_ccb, SDP_NO_RESOURCES);
            return;
        }

        p_msg->offset = L2CAP_MIN_OFFSET;
        p = p_start = (UINT8*)(p_msg + 1) + L2CAP_MIN_OFFSET;

        UINT8_TO_BE_STREAM(p, SDP_PDU_SERVICE_SEARCH_ATTR_REQ);
        UINT16_TO_BE_STREAM(p, p_ccb->transaction_id);
        ++p_ccb->transaction_id;

        p_param_len = p;
        p += 2;

        p = sdpu_build_uuid_seq(p, p_ccb->p_db->num_uuid_filters, p_ccb->p_db->uuid_filters);

        UINT16_TO_BE_STREAM(p, sdp_cb.max_attr_list_size);

        if (p_ccb->p_db->num_attr_filters) {
            p = sdpu_build_attrib_seq(p, p_ccb->p_db->attr_filters, p_ccb->p_db->num_attr_filters);
        } else {
            p = sdpu_build_attrib_seq(p, NULL, 0);
        }

        if (p_reply) {
            memcpy(p, p_reply, *p_reply + 1);
            p += *p_reply + 1;
        } else {
            UINT8_TO_BE_STREAM(p, 0);
        }

        param_len = p - p_param_len - 2;
        UINT16_TO_BE_STREAM(p_param_len, param_len);

        p_msg->len = p - p_start;

        L2CA_DataWrite(p_ccb->connection_id, p_msg);

        btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_SDP, SDP_INACT_TIMEOUT);

        return;
    }

    p = p_ccb->rsp_list;

    type = *p++;

    if (type >> 3 != DATA_ELE_SEQ_DESC_TYPE) {
        SDP_TRACE(WARNING, "SDP - Wrong type: 0x%02x in attr_rsp", type);
        return;
    }

    p = sdpu_get_len_from_type(p, type, &seq_len);
    p_end = &p_ccb->rsp_list[p_ccb->list_len];

    if (p + seq_len != p_end) {
        sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
        return;
    }

    while (p < p_end) {
        p = save_attr_seq(p_ccb, p, &p_ccb->rsp_list[p_ccb->list_len]);
        if (!p) {
            sdp_disconnect(p_ccb, SDP_DB_FULL);
            return;
        }
    }

    sdp_disconnect(p_ccb, SDP_SUCCESS);
}

static UINT8* save_attr_seq(tCONN_CB* p_ccb, UINT8* p, UINT8* p_msg_end) {
    UINT32 seq_len;
    UINT32 attr_len;
    UINT16 attr_id;
    UINT8 type;
    UINT8* p_seq_end;
    tSDP_DISC_REC* p_rec;

    type = *p++;

    if (type >> 3 != DATA_ELE_SEQ_DESC_TYPE) {
        SDP_TRACE(WARNING, "SDP - Wrong type: 0x%02x in attr_rsp", type);
        return NULL;
    }

    p = sdpu_get_len_from_type(p, type, &seq_len);
    if (p + seq_len > p_msg_end) {
        SDP_TRACE(WARNING, "SDP - Bad len in attr_rsp %d", seq_len);
        return NULL;
    }

    p_rec = (void*)add_record(p_ccb->p_db, p_ccb->device_address);
    if (!p_rec) {
        SDP_TRACE(WARNING, "SDP - DB full");
        return NULL;
    }

    p_seq_end = p + seq_len;

    while (p < p_seq_end) {
        type = *p++;
        p = sdpu_get_len_from_type(p, type, &attr_len);

        if (type >> 3 != UINT_DESC_TYPE || attr_len != 2) {
            SDP_TRACE(WARNING, "SDP - Bad type: 0x%02x or len: %d in attr_rsp", type, attr_len);
            return NULL;
        }

        BE_STREAM_TO_UINT16(p, &attr_id);

        p = (void*)add_attr(p, p_ccb->p_db, p_rec, attr_id, NULL, 0);
        if (!p) {
            SDP_TRACE(WARNING, "SDP - DB full");
            return NULL;
        }
    }

    return p;
}

static tSDP_DISC_REC* add_record(tSDP_DISCOVERY_DB* p_db, BD_ADDR p_bda) {
    tSDP_DISC_REC* p_rec;

    if (p_db->mem_free < sizeof *p_rec)
        return NULL;

    p_rec = (tSDP_DISC_REC*)p_db->p_free_mem;
    p_db->p_free_mem += sizeof *p_rec;
    p_db->mem_free -= sizeof *p_rec;

    p_rec->p_first_attr = NULL;
    p_rec->p_next_rec = NULL;

    memcpy(p_rec->remote_bd_addr, p_bda, BD_ADDR_LEN);

    if (!p_db->p_first_rec) {
        p_db->p_first_rec = p_rec;
    } else {
        tSDP_DISC_REC* p_rec1 = p_db->p_first_rec;

        while (p_rec1->p_next_rec)
            p_rec1 = p_rec1->p_next_rec;

        p_rec1->p_next_rec = p_rec;
    }

    return p_rec;
}

static UINT8* add_attr(UINT8* p, tSDP_DISCOVERY_DB* p_db, tSDP_DISC_REC* p_rec, UINT16 attr_id, tSDP_DISC_ATTR* p_parent_attr, UINT8 nest_level) {
    tSDP_DISC_ATTR* p_attr;
    UINT32 attr_len;
    UINT32 total_len;
    UINT16 attr_type;
    UINT16 id;
    UINT8 type;
    UINT8* p_end;
    UINT8 is_additional_list = nest_level & SDP_ADDITIONAL_LIST_MASK;

    nest_level &= ~SDP_ADDITIONAL_LIST_MASK;

    type = *p++;
    p = sdpu_get_len_from_type(p, type, &attr_len);

    attr_len &= SDP_DISC_ATTR_LEN_MASK;
    attr_type = type >> 3 & 0x0f;

    if (attr_len > 4)
        total_len = attr_len - 4 + sizeof *p_attr;
    else
        total_len = sizeof *p_attr;

    total_len = (total_len + 3) & ~3;

    if (p_db->mem_free < total_len)
        return NULL;

    p_attr = (tSDP_DISC_ATTR*)p_db->p_free_mem;
    p_attr->attr_id = attr_id;
    p_attr->attr_len_type = (UINT16)attr_len | attr_type << 12;
    p_attr->p_next_attr = NULL;

    switch (attr_type) {
    case UINT_DESC_TYPE:
        if (is_additional_list && attr_len == 2) {
            BE_STREAM_TO_UINT16(p, &id);

            if (id != ATTR_ID_PROTOCOL_DESC_LIST) {
                p -= 2;
            } else {
                p_db->p_free_mem += sizeof *p_attr;
                p_db->mem_free -= sizeof *p_attr;
                p_end = p + attr_len;

                total_len = 0;

                if (nest_level >= MAX_NEST_LEVELS) {
                    SDP_TRACE(ERROR, "SDP - attr nesting too deep");
                    return (p_end);
                }

                p = add_attr(p, p_db, p_rec, ATTR_ID_PROTOCOL_DESC_LIST, p_attr, nest_level + 1);

                break;
            }
        }

    case TWO_COMP_INT_DESC_TYPE:
        switch (attr_len) {
        case 1:
            p_attr->attr_value.v.u8 = *p++;
            break;

        case 2:
            BE_STREAM_TO_UINT16(p, &p_attr->attr_value.v.u16);
            break;

        case 4:
            BE_STREAM_TO_UINT32(p, &p_attr->attr_value.v.u32);
            break;

        default:
            BE_STREAM_TO_ARRAY(p, p_attr->attr_value.v.array, (INT32)attr_len);
            break;
        }
        break;

    case UUID_DESC_TYPE:
        switch (attr_len) {
        case 2:
            BE_STREAM_TO_UINT16(p, &p_attr->attr_value.v.u16);
            break;

        case 4:
            BE_STREAM_TO_UINT32(p, &p_attr->attr_value.v.u32);

            if (p_attr->attr_value.v.u32 < 0x10000) {
                attr_len = 2;
                p_attr->attr_len_type = (UINT16)attr_len | attr_type << 12;
                p_attr->attr_value.v.u16 = p_attr->attr_value.v.u32;
            }

            break;

        case 16:
            if (sdpu_is_base_uuid(p)) {
                if (p[0] == 0 && p[1] == 0) {
                    p_attr->attr_len_type = (p_attr->attr_len_type & ~SDP_DISC_ATTR_LEN_MASK) | 2;
                    p += 2;

                    BE_STREAM_TO_UINT16(p, &p_attr->attr_value.v.u16);
                    p += MAX_UUID_SIZE - 4;
                } else {
                    p_attr->attr_len_type = (p_attr->attr_len_type & ~SDP_DISC_ATTR_LEN_MASK) | 4;

                    BE_STREAM_TO_UINT32(p, &p_attr->attr_value.v.u32);
                    p += MAX_UUID_SIZE - 4;
                }
            } else {
                BE_STREAM_TO_ARRAY(p, p_attr->attr_value.v.array, (INT32)attr_len);
            }
            break;

        default:
            SDP_TRACE(WARNING, "SDP - bad len in UUID attr: %d", attr_len);
            return p + attr_len;
        }
        break;

    case DATA_ELE_SEQ_DESC_TYPE:
    case DATA_ELE_ALT_DESC_TYPE:

        p_db->p_free_mem += sizeof(tSDP_DISC_ATTR);
        p_db->mem_free -= sizeof(tSDP_DISC_ATTR);
        p_end = p + attr_len;
        total_len = 0;

        if (nest_level >= MAX_NEST_LEVELS) {
            SDP_TRACE(ERROR, "SDP - attr nesting too deep");
            return p_end;
        }

        if (is_additional_list != 0 || attr_id == ATTR_ID_ADDITION_PROTO_DESC_LISTS) {
            nest_level |= SDP_ADDITIONAL_LIST_MASK;
        }

        while (p < p_end) {
            p = add_attr(p, p_db, p_rec, 0, p_attr, nest_level + 1);
            if (!p)
                return NULL;
        }

        break;

    case TEXT_STR_DESC_TYPE:
    case URL_DESC_TYPE:
        BE_STREAM_TO_ARRAY(p, p_attr->attr_value.v.array, (INT32)attr_len);
        break;

    case BOOLEAN_DESC_TYPE:
        switch (attr_len) {
        case 1:
            p_attr->attr_value.v.u8 = *p++;
            break;

        default:
            SDP_TRACE(WARNING, "SDP - bad len in boolean attr: %d", attr_len);
            return p + attr_len;
        }

        break;

    default:
        break;
    }

    p_db->p_free_mem += total_len;
    p_db->mem_free -= total_len;

    if (!p_parent_attr) {
        if (!p_rec->p_first_attr)
            p_rec->p_first_attr = p_attr;
        else {
            tSDP_DISC_ATTR* p_attr1 = p_rec->p_first_attr;

            while (p_attr1->p_next_attr)
                p_attr1 = p_attr1->p_next_attr;

            p_attr1->p_next_attr = p_attr;
        }
    } else if (!p_parent_attr->attr_value.v.p_sub_attr) {
        p_parent_attr->attr_value.v.p_sub_attr = p_attr;
    } else {
        tSDP_DISC_ATTR* p_attr1 = p_parent_attr->attr_value.v.p_sub_attr;

        while (p_attr1->p_next_attr)
            p_attr1 = p_attr1->p_next_attr;

        p_attr1->p_next_attr = p_attr;
    }

    return p;
}
