#include "bt_types.h"
#include "bta_sys.h"
#include "bte.h"
#include "btu.h"
#include "gki_target.h"
#include "revolution/bte.h"

char lbl_806B2DE0;
int _btu_g_count;
int _btu_last_timer_tick;
int execute_btu;

void btu_task_init(void) {
    lbl_806B2DE0 = 1;
    execute_btu = 1;
    _btu_g_count = 0;
    _btu_last_timer_tick = 0;

    btu_init_core();
    BTE_InitStack();
    bta_sys_init();
}

// btu_task_msg_handler

void btu_start_timer(TIMER_LIST_ENT* p_tle, UINT16 type, UINT32 timeout) {
    BT_HDR* p_msg;

    GKI_remove_from_timer_list(&btu_cb.timer_queue, p_tle);

    p_tle->event = type;
    p_tle->ticks = timeout; /* Save the number of seconds for the timer */

    GKI_add_to_timer_list(&btu_cb.timer_queue, p_tle);
}

void btu_stop_timer(TIMER_LIST_ENT* p_tle) {
    GKI_remove_from_timer_list(&btu_cb.timer_queue, p_tle);
}
