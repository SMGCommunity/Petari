#include "sdp_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/sdp/sdp_server.c
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

#define SDP_MAX_SERVICE_RSPHDR_LEN 12
#define SDP_MAX_ATTR_RSPHDR_LEN 10
#define SDP_MAX_SERVATTR_RSPHDR_LEN 10

#ifndef SDP_TEXT_BAD_HEADER
#define SDP_TEXT_BAD_HEADER NULL
#endif

#ifndef SDP_TEXT_BAD_PDU
#define SDP_TEXT_BAD_PDU NULL
#endif

#ifndef SDP_TEXT_BAD_UUID_LIST
#define SDP_TEXT_BAD_UUID_LIST NULL
#endif

#ifndef SDP_TEXT_BAD_CONT_LEN
#define SDP_TEXT_BAD_CONT_LEN NULL
#endif

#ifndef SDP_TEXT_BAD_CONT_INX
#define SDP_TEXT_BAD_CONT_INX NULL
#endif

#ifndef SDP_TEXT_BAD_HANDLE
#define SDP_TEXT_BAD_HANDLE NULL
#endif

#ifndef SDP_TEXT_BAD_ATTR_LIST
#define SDP_TEXT_BAD_ATTR_LIST NULL
#endif

/*******************************************************************************
 * local function declarations
 */

static void process_service_search(tCONN_CB* p_ccb, UINT16 trans_num, UINT16 param_len, UINT8* p_req, UINT8* p_req_end);
static void process_service_attr_req(tCONN_CB* p_ccb, UINT16 trans_num, UINT16 param_len, UINT8* p_req, UINT8* p_req_end);
static void process_service_search_attr_req(tCONN_CB* p_ccb, UINT16 trans_num, UINT16 param_len, UINT8* p_req, UINT8* p_req_end);

/*******************************************************************************
 * functions
 */

void sdp_server_handle_client_req(tCONN_CB* p_ccb, BT_HDR* p_msg) {
    UINT8* p_req = (UINT8*)(p_msg + 1) + p_msg->offset;
    UINT8* p_req_end = p_req + p_msg->len;
    UINT8 pdu_id;
    UINT16 trans_num;
    UINT16 param_len;

    btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_SDP, SDP_INACT_TIMEOUT);

    pdu_id = *p_req++;

    BE_STREAM_TO_UINT16(p_req, &trans_num);
    BE_STREAM_TO_UINT16(p_req, &param_len);

    if (p_req + param_len > p_req_end) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_PDU_SIZE, SDP_TEXT_BAD_HEADER);
        return;
    }

    switch (pdu_id) {
    case SDP_PDU_SERVICE_SEARCH_REQ:
        process_service_search(p_ccb, trans_num, param_len, p_req, p_req_end);
        break;

    case SDP_PDU_SERVICE_ATTR_REQ:
        process_service_attr_req(p_ccb, trans_num, param_len, p_req, p_req_end);
        break;

    case SDP_PDU_SERVICE_SEARCH_ATTR_REQ:
        process_service_search_attr_req(p_ccb, trans_num, param_len, p_req, p_req_end);
        break;

    default:
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, SDP_TEXT_BAD_PDU);
        SDP_TRACE(WARNING, "SDP - server got unknown PDU: 0x%x", pdu_id);
        break;
    }
}

static void process_service_search(tCONN_CB* p_ccb, UINT16 trans_num, UINT16 param_len, UINT8* p_req, UINT8* p_req_end) {
    UINT16 max_replies;
    UINT16 cur_handles;
    UINT16 rem_handles;
    UINT16 cont_offset;
    tSDP_UUID_SEQ uid_seq;
    UINT8* p_rsp;
    UINT8* p_rsp_start;
    UINT8* p_rsp_param_len;
    UINT16 rsp_param_len;
    UINT16 num_rsp_handles;
    UINT16 xx;
    UINT32 rsp_handles[SDP_MAX_RECORDS];
    tSDP_RECORD* p_rec = NULL;
    BT_HDR* p_buf;
    BOOLEAN is_cont = FALSE;

    p_req = sdpu_extract_uid_seq(p_req, param_len, &uid_seq);

    if ((!p_req) || (!uid_seq.num_uids)) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, SDP_TEXT_BAD_UUID_LIST);
        return;
    }

    BE_STREAM_TO_UINT16(p_req, &max_replies);

    if (max_replies > SDP_MAX_RECORDS)
        max_replies = SDP_MAX_RECORDS;

    num_rsp_handles = 0;
    while (num_rsp_handles < max_replies) {
        p_rec = sdp_db_service_search(p_rec, &uid_seq);

        if (p_rec)
            rsp_handles[num_rsp_handles++] = p_rec->record_handle;
        else
            break;
    }

    if (*p_req) {
        if (*p_req++ != SDP_CONTINUATION_LEN) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, SDP_TEXT_BAD_CONT_LEN);
            return;
        }

        BE_STREAM_TO_UINT16(p_req, &cont_offset);

        if (cont_offset != p_ccb->cont_offset) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, SDP_TEXT_BAD_CONT_INX);
            return;
        }

        rem_handles = num_rsp_handles - cont_offset;
    } else {
        rem_handles = num_rsp_handles;
        cont_offset = 0;
    }

    cur_handles = (p_ccb->rem_mtu_size - SDP_MAX_SERVICE_RSPHDR_LEN) / 4;

    if (rem_handles <= cur_handles) {
        cur_handles = rem_handles;
    } else {
        p_ccb->cont_offset += cur_handles;
        is_cont = TRUE;
    }

    if ((p_buf = GKI_getpoolbuf(SDP_POOL_ID)) == NULL) {
        SDP_TRACE(ERROR, "SDP - no buf for search rsp");
        return;
    }

    p_buf->offset = L2CAP_MIN_OFFSET;
    p_rsp = p_rsp_start = (UINT8*)(p_buf + 1) + L2CAP_MIN_OFFSET;

    UINT8_TO_BE_STREAM(p_rsp, SDP_PDU_SERVICE_SEARCH_RSP);
    UINT16_TO_BE_STREAM(p_rsp, trans_num);

    p_rsp_param_len = p_rsp;
    p_rsp += 2;

    UINT16_TO_BE_STREAM(p_rsp, num_rsp_handles);
    UINT16_TO_BE_STREAM(p_rsp, cur_handles);

    for (xx = cont_offset; xx < cont_offset + cur_handles; ++xx)
        UINT32_TO_BE_STREAM(p_rsp, rsp_handles[xx]);

    if (is_cont) {
        UINT8_TO_BE_STREAM(p_rsp, SDP_CONTINUATION_LEN);
        UINT16_TO_BE_STREAM(p_rsp, p_ccb->cont_offset);
    } else {
        UINT8_TO_BE_STREAM(p_rsp, 0);
    }

    rsp_param_len = p_rsp - p_rsp_param_len - 2;
    UINT16_TO_BE_STREAM(p_rsp_param_len, rsp_param_len);

    p_buf->len = p_rsp - p_rsp_start;

    L2CA_DataWrite(p_ccb->connection_id, p_buf);
}

static void process_service_attr_req(tCONN_CB* p_ccb, UINT16 trans_num, UINT16 param_len, UINT8* p_req, UINT8* p_req_end) {
    UINT16 max_list_len;
    UINT16 rem_len;
    UINT16 len_to_send;
    UINT16 cont_offset;
    tSDP_ATTR_SEQ attr_seq;
    UINT8* p_rsp;
    UINT8* p_rsp_start;
    UINT8* p_rsp_param_len;
    UINT16 rsp_param_len;
    UINT16 xx;
    UINT32 rec_handle;
    tSDP_RECORD* p_rec;
    tSDP_ATTRIBUTE* p_attr;
    BT_HDR* p_buf;
    BOOLEAN is_cont = FALSE;

    BE_STREAM_TO_UINT32(p_req, &rec_handle);

    if (p_req > p_req_end) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_SERV_REC_HDL, SDP_TEXT_BAD_HANDLE);
        return;
    }

    BE_STREAM_TO_UINT16(p_req, &max_list_len);

    if (max_list_len > p_ccb->rem_mtu_size - SDP_MAX_ATTR_RSPHDR_LEN)
        max_list_len = p_ccb->rem_mtu_size - SDP_MAX_ATTR_RSPHDR_LEN;

    p_req = sdpu_extract_attr_seq(p_req, param_len, &attr_seq);

    if (!p_req || !attr_seq.num_attr || p_req > p_req_end) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, SDP_TEXT_BAD_ATTR_LIST);
        return;
    }

    p_rec = sdp_db_find_record(rec_handle);
    if (!p_rec) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_SERV_REC_HDL, SDP_TEXT_BAD_HANDLE);
        return;
    }

    if (*p_req) {
        if (*p_req++ != SDP_CONTINUATION_LEN) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, SDP_TEXT_BAD_CONT_LEN);
            return;
        }

        BE_STREAM_TO_UINT16(p_req, &cont_offset);

        if (cont_offset != p_ccb->cont_offset) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, SDP_TEXT_BAD_CONT_INX);
            return;
        }

        is_cont = TRUE;
    } else {
        p_ccb->cont_offset = 0;
        p_rsp = p_ccb->rsp_list + 3;

        for (xx = 0; xx < attr_seq.num_attr; ++xx) {
            p_attr = sdp_db_find_attr_in_rec(p_rec, attr_seq.attr_entry[xx].start, attr_seq.attr_entry[xx].end);

            if (!p_attr)
                continue;

            rem_len = 1000 - (UINT16)(p_rsp - p_ccb->rsp_list);

            if (p_attr->len > rem_len - 6)
                break;

            p_rsp = sdpu_build_attrib_entry(p_rsp, p_attr);

            if (attr_seq.attr_entry[xx].start != attr_seq.attr_entry[xx].end) {
                attr_seq.attr_entry[xx].start = p_attr->id + 1;
                --xx;
            }
        }

        p_ccb->list_len = p_rsp - p_ccb->rsp_list;

        if (p_ccb->list_len > 255) {
            p_ccb->rsp_list[0] = DATA_ELE_SEQ_DESC_TYPE << 3 | SIZE_IN_NEXT_WORD;
            p_ccb->rsp_list[1] = (p_ccb->list_len - 3) >> 8;
            p_ccb->rsp_list[2] = p_ccb->list_len - 3;
        } else {
            p_ccb->cont_offset = 1;

            p_ccb->rsp_list[1] = DATA_ELE_SEQ_DESC_TYPE << 3 | SIZE_IN_NEXT_BYTE;
            p_ccb->rsp_list[2] = p_ccb->list_len - 3;

            --p_ccb->list_len;
        }
    }

    if ((p_buf = GKI_getpoolbuf(SDP_POOL_ID)) == NULL) {
        SDP_TRACE(ERROR, "SDP - no buf for search rsp");
        return;
    }

    p_buf->offset = L2CAP_MIN_OFFSET;
    p_rsp = p_rsp_start = (UINT8*)(p_buf + 1) + L2CAP_MIN_OFFSET;

    UINT8_TO_BE_STREAM(p_rsp, SDP_PDU_SERVICE_ATTR_RSP);
    UINT16_TO_BE_STREAM(p_rsp, trans_num);

    p_rsp_param_len = p_rsp;
    p_rsp += 2;

    if (p_ccb->list_len <= max_list_len)
        len_to_send = p_ccb->list_len;
    else
        len_to_send = max_list_len;

    UINT16_TO_BE_STREAM(p_rsp, len_to_send);

    memcpy(p_rsp, &p_ccb->rsp_list[p_ccb->cont_offset], len_to_send);
    p_rsp += len_to_send;

    p_ccb->cont_offset += len_to_send;
    p_ccb->list_len -= len_to_send;

    if (p_ccb->list_len) {
        is_cont = TRUE;

        UINT8_TO_BE_STREAM(p_rsp, SDP_CONTINUATION_LEN);
        UINT16_TO_BE_STREAM(p_rsp, p_ccb->cont_offset);
    } else {
        UINT8_TO_BE_STREAM(p_rsp, 0);
    }

    rsp_param_len = p_rsp - p_rsp_param_len - 2;
    UINT16_TO_BE_STREAM(p_rsp_param_len, rsp_param_len);

    p_buf->len = p_rsp - p_rsp_start;

    L2CA_DataWrite(p_ccb->connection_id, p_buf);
}

static void process_service_search_attr_req(tCONN_CB* p_ccb, UINT16 trans_num, UINT16 param_len, UINT8* p_req, UINT8* p_req_end) {
    UINT16 max_list_len;
    UINT16 rem_len;
    UINT16 len_to_send;
    UINT16 cont_offset;
    tSDP_UUID_SEQ uid_seq;
    UINT8* p_rsp;
    UINT8* p_rsp_start;
    UINT8* p_rsp_param_len;
    UINT16 rsp_param_len;
    UINT16 xx;
    tSDP_RECORD* p_rec;
    tSDP_ATTR_SEQ attr_seq;
    tSDP_ATTR_SEQ attr_seq_sav;
    tSDP_ATTRIBUTE* p_attr;
    BT_HDR* p_buf;
    BOOLEAN maxxed_out = FALSE;
    BOOLEAN is_cont = FALSE;
    UINT8* p_seq_start;
    UINT16 seq_len;

    p_req = sdpu_extract_uid_seq(p_req, param_len, &uid_seq);

    if (!p_req || !uid_seq.num_uids) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, SDP_TEXT_BAD_UUID_LIST);
        return;
    }

    BE_STREAM_TO_UINT16(p_req, &max_list_len);

    if (max_list_len > p_ccb->rem_mtu_size - SDP_MAX_SERVATTR_RSPHDR_LEN)
        max_list_len = p_ccb->rem_mtu_size - SDP_MAX_SERVATTR_RSPHDR_LEN;

    p_req = sdpu_extract_attr_seq(p_req, param_len, &attr_seq);
    if (!p_req || !attr_seq.num_attr) {
        sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_REQ_SYNTAX, SDP_TEXT_BAD_ATTR_LIST);
        return;
    }

    __memcpy(&attr_seq_sav, &attr_seq, sizeof attr_seq_sav);

    if (*p_req) {
        if (*p_req++ != SDP_CONTINUATION_LEN) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, SDP_TEXT_BAD_CONT_LEN);
            return;
        }

        BE_STREAM_TO_UINT16(p_req, &cont_offset);

        if (cont_offset != p_ccb->cont_offset) {
            sdpu_build_n_send_error(p_ccb, trans_num, SDP_INVALID_CONT_STATE, SDP_TEXT_BAD_CONT_INX);
            return;
        }

        is_cont = TRUE;
    } else {
        p_ccb->cont_offset = 0;
        p_rsp = p_ccb->rsp_list + 3;

        for (p_rec = sdp_db_service_search(NULL, &uid_seq); p_rec; p_rec = sdp_db_service_search(p_rec, &uid_seq)) {
            __memcpy(&attr_seq, &attr_seq_sav, sizeof attr_seq);

            p_seq_start = p_rsp;
            p_rsp += 3;

            for (xx = 0; xx < attr_seq.num_attr; ++xx) {
                p_attr = sdp_db_find_attr_in_rec(p_rec, attr_seq.attr_entry[xx].start, attr_seq.attr_entry[xx].end);

                if (!p_attr)
                    continue;

                rem_len = 1000 - (UINT16)(p_rsp - p_ccb->rsp_list);

                if (p_attr->len > rem_len - 6) {
                    maxxed_out = TRUE;
                    break;
                }

                p_rsp = sdpu_build_attrib_entry(p_rsp, p_attr);

                if (attr_seq.attr_entry[xx].start != attr_seq.attr_entry[xx].end) {
                    attr_seq.attr_entry[xx].start = p_attr->id + 1;
                    --xx;
                }
            }

            if (maxxed_out)
                break;

            seq_len = p_rsp - p_seq_start - 3;

            if (seq_len) {
                UINT8_TO_BE_STREAM(p_seq_start, 54);  // TODO
                UINT16_TO_BE_STREAM(p_seq_start, seq_len);
            } else {
                p_rsp = p_seq_start;
            }
        }

        p_ccb->list_len = p_rsp - p_ccb->rsp_list;

        if (p_ccb->list_len > 255) {
            p_ccb->rsp_list[0] = DATA_ELE_SEQ_DESC_TYPE << 3 | SIZE_IN_NEXT_WORD;
            p_ccb->rsp_list[1] = (p_ccb->list_len - 3) >> 8;
            p_ccb->rsp_list[2] = p_ccb->list_len - 3;
        } else {
            p_ccb->cont_offset = 1;

            p_ccb->rsp_list[1] = DATA_ELE_SEQ_DESC_TYPE << 3 | SIZE_IN_NEXT_BYTE;
            p_ccb->rsp_list[2] = p_ccb->list_len - 3;

            --p_ccb->list_len;
        }
    }

    if ((p_buf = GKI_getpoolbuf(SDP_POOL_ID)) == NULL) {
        SDP_TRACE(ERROR, "SDP - no buf for search rsp");
        return;
    }

    p_buf->offset = L2CAP_MIN_OFFSET;
    p_rsp = p_rsp_start = (UINT8*)(p_buf + 1) + L2CAP_MIN_OFFSET;

    UINT8_TO_BE_STREAM(p_rsp, SDP_PDU_SERVICE_SEARCH_ATTR_RSP);
    UINT16_TO_BE_STREAM(p_rsp, trans_num);

    p_rsp_param_len = p_rsp;
    p_rsp += 2;

    if (p_ccb->list_len <= max_list_len)
        len_to_send = p_ccb->list_len;
    else
        len_to_send = max_list_len;

    UINT16_TO_BE_STREAM(p_rsp, len_to_send);

    memcpy(p_rsp, &p_ccb->rsp_list[p_ccb->cont_offset], len_to_send);
    p_rsp += len_to_send;

    p_ccb->cont_offset += len_to_send;
    p_ccb->list_len -= len_to_send;

    if (p_ccb->list_len) {
        is_cont = TRUE;

        UINT8_TO_BE_STREAM(p_rsp, SDP_CONTINUATION_LEN);
        UINT16_TO_BE_STREAM(p_rsp, p_ccb->cont_offset);
    } else {
        UINT8_TO_BE_STREAM(p_rsp, 0);
    }

    rsp_param_len = p_rsp - p_rsp_param_len - 2;
    UINT16_TO_BE_STREAM(p_rsp_param_len, rsp_param_len);

    p_buf->len = p_rsp - p_rsp_start;

    L2CA_DataWrite(p_ccb->connection_id, p_buf);

    (void)&len_to_send;
    (void)&p_buf;
}
