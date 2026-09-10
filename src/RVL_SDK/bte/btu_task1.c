#include "bt_types.h"
#include "bta_sys.h"
#include "bte.h"
#include "btm_int.h"
#include "btu.h"
#include "gki_target.h"
#include "hidh_int.h"
#include "l2c_int.h"
#include "rfc_int.h"
#include "sdp_int.h"
#include <revolution/os.h>

tBTU_CB btu_cb;

typedef void(tUSER_TIMEOUT_FUNC)(TIMER_LIST_ENT* p_tle);

static UINT8 btu_count = 1;
static BOOL execute_btu = TRUE;
static UINT32 _btu_g_count = 0;
static UINT32 _btu_last_timer_tick = 0;

void btu_task_init(void) {
    btu_count = 1;
    execute_btu = TRUE;
    _btu_g_count = 0;
    _btu_last_timer_tick = 0;

    btu_init_core();
    BTE_InitStack();
    bta_sys_init();
}

void btu_task_msg_handler(void) {
    UINT16 event;
    BT_HDR* p_msg;
    UINT8 i;
    UINT16 mask;
    BOOLEAN handled;
    BOOLEAN done = FALSE;
    UINT8 tmp_count = 1;
    OSTime now;

    now = OSGetTime();
    _btu_g_count++;

    GKI_disable();
    if (execute_btu) {
        execute_btu = FALSE;
        tmp_count = btu_count;
        btu_count = 1;
    } else {
        btu_count++;
        GKI_enable();
        return;
    }
    GKI_enable();

    event = TASK_MBOX_0_EVT_MASK | TASK_MBOX_2_EVT_MASK;
    if (_btu_g_count > _btu_last_timer_tick + 500) {
        event |= TIMER_0_EVT_MASK | TIMER_1_EVT_MASK;
        _btu_last_timer_tick = _btu_g_count;
    }

    while (!done) {
        done = TRUE;
        if (event & TASK_MBOX_0_EVT_MASK) {
            while ((p_msg = (BT_HDR*)GKI_read_mbox(BTU_HCI_RCV_MBOX)) != NULL) {
                done = FALSE;
                switch (p_msg->event & BT_EVT_MASK) {
                case BT_EVT_TO_BTU_HCI_ACL:
                    l2c_rcv_acl_data(p_msg);
                    break;
                case BT_EVT_TO_BTU_L2C_SEG_XMIT:
                    l2c_link_segments_xmitted(p_msg);
                    break;
                case BT_EVT_TO_BTU_HCI_SCO:
                    btm_route_sco_data(p_msg);
                    break;
                case BT_EVT_TO_BTU_HCI_EVT:
                    btu_hcif_process_event(p_msg);
                    GKI_freebuf(p_msg);
                    break;
                case BT_EVT_TO_BTU_HCI_CMD:
                    btu_hcif_send_cmd(p_msg);
                    break;
                default:
                    i = 0;
                    mask = (UINT16)(p_msg->event & BT_EVT_MASK);
                    handled = FALSE;
                    for (; !handled && i < BTU_MAX_REG_EVENT; i++) {
                        if (btu_cb.event_reg[i].event_cb == NULL)
                            continue;
                        if (mask == btu_cb.event_reg[i].event_range) {
                            if (btu_cb.event_reg[i].event_cb) {
                                btu_cb.event_reg[i].event_cb(p_msg);
                                handled = TRUE;
                            }
                        }
                    }
                    if (handled == FALSE)
                        GKI_freebuf(p_msg);
                    break;
                }
            }
        }

        if (event & TIMER_0_EVT_MASK) {
            TIMER_LIST_ENT* p_tle;
            GKI_update_timer_list(&btu_cb.timer_queue, 1);
            event &= (~TIMER_0_EVT_MASK);
            while (btu_cb.timer_queue.p_first && !btu_cb.timer_queue.p_first->ticks) {
                done = FALSE;
                p_tle = btu_cb.timer_queue.p_first;
                GKI_remove_from_timer_list(&btu_cb.timer_queue, p_tle);
                switch (p_tle->event) {
                case BTU_TTYPE_BTM_DEV_CTL:
                    btm_dev_timeout(p_tle);
                    break;
                case BTU_TTYPE_BTM_ACL:
                    btm_acl_timeout(p_tle);
                    break;
                case BTU_TTYPE_L2CAP_LINK:
                case BTU_TTYPE_L2CAP_CHNL:
                case BTU_TTYPE_L2CAP_HOLD:
                    l2c_process_timeout(p_tle);
                    break;
                case BTU_TTYPE_SDP:
                    sdp_conn_timeout((tCONN_CB*)p_tle->param);
                    break;
                case BTU_TTYPE_BTM_RMT_NAME:
                    btm_inq_rmt_name_failed();
                    break;
                case BTU_TTYPE_BTM_DISCOVERY:
                    btm_discovery_timeout();
                    break;
                case BTU_TTYPE_RFCOMM_MFC:
                case BTU_TTYPE_RFCOMM_PORT:
                    rfcomm_process_timeout(p_tle);
                    break;
                case BTU_TTYPE_BTU_CMD_CMPL:
                    btu_hcif_cmd_timeout();
                    break;
                case BTU_TTYPE_HID_HOST_REPAGE_TO:
                    hidh_proc_repage_timeout(p_tle);
                    break;
                case BTU_TTYPE_USER_FUNC: {
                    tUSER_TIMEOUT_FUNC* p_uf = (tUSER_TIMEOUT_FUNC*)p_tle->param;
                    (*p_uf)(p_tle);
                } break;
                default:
                    i = 0;
                    handled = FALSE;
                    for (; !handled && i < BTU_MAX_REG_TIMER; i++) {
                        if (btu_cb.timer_reg[i].timer_cb == NULL)
                            continue;
                        if (btu_cb.timer_reg[i].p_tle == p_tle) {
                            btu_cb.timer_reg[i].timer_cb(p_tle);
                            handled = TRUE;
                        }
                    }
                    break;
                }
            }
        }

        if (event & TASK_MBOX_2_EVT_MASK) {
            while ((p_msg = (BT_HDR*)GKI_read_mbox(TASK_MBOX_2)) != NULL) {
                done = FALSE;
                bta_sys_event(p_msg);
            }
        }

        if (event & TIMER_1_EVT_MASK) {
            event &= (~TIMER_1_EVT_MASK);
            bta_sys_timer_update();
        }

        if (event & EVENT_MASK(APPL_EVT_7))
            break;
    }

    execute_btu = TRUE;
}

void btu_start_timer(TIMER_LIST_ENT* p_tle, UINT16 type, UINT32 timeout) {
    GKI_remove_from_timer_list(&btu_cb.timer_queue, p_tle);

    p_tle->event = type;
    p_tle->ticks = (s32)timeout;

    GKI_add_to_timer_list(&btu_cb.timer_queue, p_tle);
}

void btu_stop_timer(TIMER_LIST_ENT* p_tle) {
    GKI_remove_from_timer_list(&btu_cb.timer_queue, p_tle);
}
