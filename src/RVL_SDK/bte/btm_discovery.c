#include "btm_api.h"

/*******************************************************************************
 * headers
 */

#include "revolution/types.h"
#include <cstring>
#include <mem.h>

#include "bt_target.h"
#include "bt_types.h"
#include "data_types.h"

#include "btm_int.h"
#include "btu.h"  // btu_stop_timer
#include "sdp_api.h"
#include "sdpdefs.h"  // UUID_SERVCLASS_PUBLIC_BROWSE_GROUP

/*******************************************************************************
 * local function declarations
 */

static void btm_disc_compl_cb(UINT16 result);

/*******************************************************************************
 * functions
 */

void btm_discovery_db_init(void) {
    btm_cb.p_btm_ddb = (tBTM_DISCOVERY_DB*)btm_cb.btm_disc_vars.btm_db_area;

    memset(&btm_cb.btm_disc_vars, 0, sizeof btm_cb.btm_disc_vars);

    btm_cb.btm_disc_vars.num_uuid_filters = 1;
    btm_cb.btm_disc_vars.uuid_filters[0].len = LEN_UUID_16;
    btm_cb.btm_disc_vars.uuid_filters[0].uu.uuid16 = UUID_SERVCLASS_PUBLIC_BROWSE_GROUP;

    SDP_InitDiscoveryDb(btm_cb.p_btm_ddb, BTM_DISC_DB_SIZE, 1, btm_cb.btm_disc_vars.uuid_filters, 0, NULL);
}

void btm_discovery_db_reset(void) {
    tBTM_CMPL_CB* p_cb;

    btu_stop_timer(&btm_cb.btm_disc_vars.disc_timer_ent);

    if (btm_cb.btm_disc_vars.disc_active) {
        btm_cb.btm_disc_vars.disc_active = BTM_NON_DISCOVERABLE;

        p_cb = btm_cb.btm_disc_vars.p_disc_cmpl_cb;
        btm_cb.btm_disc_vars.p_disc_cmpl_cb = NULL;

        if (p_cb)
            (*p_cb)(NULL);
    }

    btm_discovery_db_init();
}

void btm_discovery_timeout(void) {
    /* ... */
}

static void btm_disc_compl_cb(UINT16 result) {
    tBTM_CMPL_CB* p_cb;

    if (result != SDP_DB_FULL && btm_cb.btm_disc_vars.disc_active != BTM_GENERAL_DISCOVERABLE) {
        btm_cb.btm_disc_vars.p_cur_ii = BTM_InqDbNext(btm_cb.btm_disc_vars.p_cur_ii);

        if (btm_cb.btm_disc_vars.p_cur_ii &&
            SDP_ServiceSearchRequest(btm_cb.btm_disc_vars.p_cur_ii->results.remote_bd_addr, btm_cb.p_btm_ddb, &btm_disc_compl_cb)) {
            return;
        }
    }

    btm_cb.btm_disc_vars.disc_active = BTM_NON_DISCOVERABLE;

    p_cb = btm_cb.btm_disc_vars.p_disc_cmpl_cb;
    btm_cb.btm_disc_vars.p_disc_cmpl_cb = NULL;

    if (p_cb)
        (*p_cb)(&result);
}

void BTM_SetDiscoveryParams(UINT16 num_uuid, tSDP_UUID* p_uuid_list, UINT16 num_attr, UINT16* p_attr_list) {
    UINT16 xx;

    if (num_attr > SDP_MAX_ATTR_FILTERS)
        num_attr = SDP_MAX_ATTR_FILTERS;

    if (num_uuid > SDP_MAX_UUID_FILTERS)
        num_uuid = SDP_MAX_UUID_FILTERS;

    for (xx = 0; xx < num_uuid; ++xx)
        btm_cb.btm_disc_vars.uuid_filters[xx] = *p_uuid_list++;

    btm_cb.btm_disc_vars.num_uuid_filters = num_uuid;

    for (xx = 0; xx < num_attr; ++xx)
        btm_cb.btm_disc_vars.attr_filters[xx] = *p_attr_list++;

    btm_cb.btm_disc_vars.num_attr_filters = num_attr;
}

tBTM_STATUS BTM_StartDiscovery(tBTM_CMPL_CB* p_cmpl_cb, BD_ADDR p_rem_addr) {
    UINT8 disc;

    if (btm_cb.btm_disc_vars.disc_active != BTM_NON_DISCOVERABLE)
        return BTM_BUSY;

    if (!p_rem_addr) {
        btm_cb.btm_disc_vars.p_cur_ii = BTM_InqDbFirst();
        if (!btm_cb.btm_disc_vars.p_cur_ii)
            return BTM_UNKNOWN_ADDR;

        p_rem_addr = btm_cb.btm_disc_vars.p_cur_ii->results.remote_bd_addr;

        disc = BTM_LIMITED_DISCOVERABLE;
    } else {
        disc = BTM_GENERAL_DISCOVERABLE;
    }

    SDP_InitDiscoveryDb(btm_cb.p_btm_ddb, BTM_DISC_DB_SIZE, btm_cb.btm_disc_vars.num_uuid_filters, btm_cb.btm_disc_vars.uuid_filters,
                        btm_cb.btm_disc_vars.num_attr_filters, btm_cb.btm_disc_vars.attr_filters);

    if (!SDP_ServiceSearchRequest(p_rem_addr, btm_cb.p_btm_ddb, &btm_disc_compl_cb)) {
        return BTM_ERR_PROCESSING;
    }

    btm_cb.btm_disc_vars.disc_active = disc;
    btm_cb.btm_disc_vars.p_disc_cmpl_cb = p_cmpl_cb;

    return BTM_CMD_STARTED;
}

tSDP_DISC_REC* BTM_FindAttribute(UINT16 attr_id, tSDP_DISC_REC* p_start_rec) {
    return SDP_FindAttributeInDb(btm_cb.p_btm_ddb, attr_id, p_start_rec);
}

tSDP_DISC_REC* BTM_FindService(UINT16 service_uuid, tSDP_DISC_REC* p_start_rec) {
    return SDP_FindServiceInDb(btm_cb.p_btm_ddb, service_uuid, p_start_rec);
}
