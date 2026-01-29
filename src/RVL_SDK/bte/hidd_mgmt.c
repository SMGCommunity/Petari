#include "hidd_int.h"

/*******************************************************************************
 * headers
 */

#include "revolution/types.h"
#include <cstring>

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "btu.h"
#include "hidd_api.h"
#include "hiddefs.h"

/*******************************************************************************
 * local function declarations
 */

static tHID_STATUS hidd_no_conn_proc_evt(UINT8 event, void* p_data);
static tHID_STATUS hidd_connecting_proc_evt(UINT8 event, void* p_data);
static tHID_STATUS hidd_connected_proc_evt(UINT8 event, void* p_data);
static tHID_STATUS hidd_disc_ing_proc_evt(UINT8 event, void* p_data);

/*******************************************************************************
 * variables
 */

// .rodata
tHID_DEV_PROC_EVT* const hidd_sm_proc_evt[] = {
    &hidd_no_conn_proc_evt,
    &hidd_connecting_proc_evt,
    &hidd_connected_proc_evt,
    &hidd_disc_ing_proc_evt,
};

// .bss
tHID_DEV_CTB hd_cb;

/*******************************************************************************
 * functions
 */

tHID_STATUS hidd_mgmt_process_evt(UINT8 event, void* p_data) {
    HIDD_TRACE(DEBUG, "hidd_mgmt_process_evt known: %d, s:%d, e:%d", hd_cb.host_known, hd_cb.dev_state, event);

    if (hd_cb.host_known) {
        return (*hidd_sm_proc_evt[hd_cb.dev_state])(event, p_data);
    } else if (event == 0) {
        hd_cb.host_known = TRUE;
        memcpy(hd_cb.host_addr, p_data, BD_ADDR_LEN);

        hd_cb.dev_state = 2;

        (*hd_cb.callback)(0, 0, (tHID_DEV_REG_INFO_CBACK_DATA*)hd_cb.host_addr);

        return 0;
    } else {
        return 13;
    }
}

static tHID_STATUS hidd_no_conn_proc_evt(UINT8 event, void* p_data) {
    tHID_STATUS st = 0;

    switch (event) {
    case 0:
        hd_cb.dev_state = 2;

        (*hd_cb.callback)(0, 0, (tHID_DEV_REG_INFO_CBACK_DATA*)hd_cb.host_addr);
        break;

    case 4:
        hd_cb.conn_tries = 1;
        hd_cb.dev_state = 1;

        st = hidd_conn_initiate();
        if (st == 0)
            break;

        btu_start_timer(&hd_cb.conn.timer_entry, 22, 1);

        return 0;

    default:
        st = 4;
        break;
    }

    return st;
}

void hidd_proc_repage_timeout(void) {
    HIDD_TRACE(DEBUG, "hidd_proc_repage_timeout");

    ++hd_cb.conn_tries;

    if (hidd_conn_initiate()) {
        if (hd_cb.conn_tries > 15) {
            hd_cb.dev_state = 0;
            (*hd_cb.callback)(1, 0, NULL);
        } else {
            btu_start_timer(&hd_cb.conn.timer_entry, 22, 1);
        }
    } else {
        (*hd_cb.callback)(2, hd_cb.conn_tries, NULL);
    }
}

static tHID_STATUS hidd_connecting_proc_evt(UINT8 event, void* p_data) {
    UINT16 reason;

    switch (event) {
    case 0:
        hd_cb.dev_state = 2;
        (*hd_cb.callback)(0, 0, (tHID_DEV_REG_INFO_CBACK_DATA*)hd_cb.host_addr);
        break;

    case 3:
        if (hd_cb.conn_tries > 15) {
            reason = *(UINT16*)p_data;
            hd_cb.dev_state = 0;
            (*hd_cb.callback)(1, reason, NULL);
        } else {
            btu_start_timer(&hd_cb.conn.timer_entry, 22, 1);
        }

        break;

    case 1:
    case 2:
        hd_cb.dev_state = 0;
        (*hd_cb.callback)(1, *(UINT16*)p_data, NULL);
        break;

    case 5:
        hd_cb.dev_state = 0;

        btu_stop_timer(&hd_cb.conn.timer_entry);
        hidd_conn_disconnect();
        break;

    default:
        return 9;
    }

    return 0;
}

void hidd_mgmt_conn_closed(UINT16 reason) {
    if (hd_cb.unplug_on)
        hd_cb.host_known = FALSE;

    hd_cb.dev_state = 0;

    (*hd_cb.callback)(1, reason, NULL);
}

static tHID_STATUS hidd_connected_proc_evt(UINT8 event, void* p_data) {
    switch (event) {
    case 2:
        hd_cb.dev_state = 1;
        hd_cb.conn_tries = 0;

        btu_start_timer(&hd_cb.conn.timer_entry, 22, 1);
        break;

    case 1:
        hidd_mgmt_conn_closed(*(UINT16*)p_data);
        break;

    case 5:
        hd_cb.dev_state = 3;
        hidd_conn_disconnect();
        break;

    case 6:
        return hidd_conn_snd_data(p_data);

    default:
        return 10;
    }

    return 0;
}

static tHID_STATUS hidd_disc_ing_proc_evt(UINT8 event, void* p_data) {
    switch (event) {
    case 1:
    case 2:
    case 3:
        hidd_mgmt_conn_closed(*(UINT16*)p_data);
        break;

    default:
        return 11;
    }

    return 0;
}
