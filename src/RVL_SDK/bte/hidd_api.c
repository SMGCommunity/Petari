#include "hidd_api.h"

/*******************************************************************************
 * headers
 */

#include "revolution/types.h"
#include <cstring>

#include <macros.h>  // ARRAY_LENGTH

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"
#include "wcassert.h"  // WCAssert_Line

#include "btm_api.h"
#include "gki.h"
#include "hidd_int.h"
#include "hiddefs.h"
#include "sdp_api.h"

/*******************************************************************************
 * variables
 */

// .sdata2
static UINT8 const HidDevLangList[8] = {53, 6, 9, 4, 9, 9, 1, 0};

/*******************************************************************************
 * functions
 */

UINT32 HID_DevSetSDPRecord(tHID_DEV_SDP_INFO* p_sdp_info) {
    UINT32 sdp_handle;
    tSDP_PROTOCOL_ELEM protocol_list[2];
    tSDP_PROTO_LIST_ELEM additional_list;
    UINT16 u16;
    UINT8 u8;
    UINT8 RepDescriptor[208];
    UINT8* pd;
    char buf[2];

    if (!p_sdp_info)
        return 0;

    sdp_handle = SDP_CreateRecord();
    if (!sdp_handle) {
        HIDD_TRACE(ERROR, "Could not create service record");
        return 0;
    }

    u16 = 0x1124;
    SDP_AddServiceClassIdList(sdp_handle, 1, &u16);

    protocol_list[0].protocol_uuid = 0x100;
    protocol_list[0].num_params = 1;
    protocol_list[0].params[0] = 0x11;

    protocol_list[1].num_params = 0;
    protocol_list[1].protocol_uuid = 0x11;

    SDP_AddProtocolList(sdp_handle, ARRAY_LENGTH(protocol_list), protocol_list);
    SDP_AddLanguageBaseAttrIDList(sdp_handle, 'en', 0x6a, 0x100);
    SDP_AddProfileDescriptorList(sdp_handle, 0x11, 0x100);

    additional_list.num_elems = 2;

    additional_list.list_elem[0].protocol_uuid = 0x100;
    additional_list.list_elem[0].num_params = 1;
    additional_list.list_elem[0].params[0] = 0x13;

    additional_list.list_elem[1].protocol_uuid = 0x11;
    additional_list.list_elem[1].num_params = 0;

    SDP_AddAdditionProtoLists(sdp_handle, 1, &additional_list);

    if (p_sdp_info->svc_name[0] != '\0') {
        SDP_AddAttribute(sdp_handle, 0x100, 4, (UINT8)(strlen(p_sdp_info->svc_name) + 1), (UINT8*)p_sdp_info->svc_name);
    }

    if (p_sdp_info->svc_descr[0] != '\0') {
        SDP_AddAttribute(sdp_handle, 0x101, 4, (UINT8)(strlen(p_sdp_info->svc_descr) + 1), (UINT8*)p_sdp_info->svc_descr);
    }

    if (p_sdp_info->prov_name[0] != '\0') {
        SDP_AddAttribute(sdp_handle, 0x102, 4, (UINT8)(strlen(p_sdp_info->prov_name) + 1), (UINT8*)p_sdp_info->prov_name);
    }

    UINT16_TO_BE_FIELD(buf, p_sdp_info->rel_num);
    SDP_AddAttribute(sdp_handle, 0x200, 1, 2, (UINT8*)buf);

    UINT16_TO_BE_FIELD(buf, p_sdp_info->hpars_ver);
    SDP_AddAttribute(sdp_handle, 0x201, 1, 2, (UINT8*)buf);

    u16 = 0x100;
    UINT16_TO_BE_FIELD(buf, u16);
    SDP_AddAttribute(sdp_handle, 0x20b, 1, 2, (UINT8*)buf);

    u8 = p_sdp_info->sub_class;
    SDP_AddAttribute(sdp_handle, 0x202, 1, 1, &u8);

    u8 = p_sdp_info->ctry_code;
    SDP_AddAttribute(sdp_handle, 0x203, 1, 1, &u8);

    u8 = 1;
    SDP_AddAttribute(sdp_handle, 0x204, 5, 1, &u8);

    u8 = 1;
    SDP_AddAttribute(sdp_handle, 0x205, 5, 1, &u8);

    pd = RepDescriptor;
    *pd++ = 0x35;
    *pd++ = p_sdp_info->dscp_info.dl_len + 4;
    *pd++ = 0x08;
    *pd++ = 0x22;
    *pd++ = 0x25;
    *pd++ = p_sdp_info->dscp_info.dl_len;

    memcpy(pd, p_sdp_info->dscp_info.dsc_list, p_sdp_info->dscp_info.dl_len);
    pd += p_sdp_info->dscp_info.dl_len;

    SDP_AddAttribute(sdp_handle, 0x206, 6, pd - RepDescriptor, RepDescriptor);

    SDP_AddAttribute(sdp_handle, 0x207, 6, 8, (UINT8*)HidDevLangList);

    u8 = 0;
    SDP_AddAttribute(sdp_handle, 0x208, 5, 1, &u8);

    u8 = 1;
    SDP_AddAttribute(sdp_handle, 0x209, 5, 1, &u8);

    u8 = 1;
    SDP_AddAttribute(sdp_handle, 0x20a, 5, 1, &u8);

    UINT16_TO_BE_FIELD(buf, 0x8000);
    SDP_AddAttribute(sdp_handle, 0x20c, 1, 2, (UINT8*)buf);

    u8 = 0;
    SDP_AddAttribute(sdp_handle, 0x20d, 5, 1, &u8);

    u8 = 0;
    SDP_AddAttribute(sdp_handle, 0x20e, 5, 1, &u8);

    u16 = 0x1002;
    SDP_AddUuidSequence(sdp_handle, 5, 1, &u16);

    return sdp_handle;
}

void HID_DevInit(void) {
    memset(&hd_cb, 0, sizeof hd_cb);

    hd_cb.l2cap_ctrl_cfg.mtu_present = TRUE;
    hd_cb.l2cap_ctrl_cfg.mtu = 64;

    hd_cb.l2cap_int_cfg.mtu_present = TRUE;
    hd_cb.l2cap_int_cfg.mtu = 64;

    hd_cb.conn.timer_entry.param = (TIMER_PARAM_TYPE)&hidd_proc_repage_timeout;

    hd_cb.trace_level = BT_TRACE_LEVEL_NONE;
}

tHID_STATUS HID_DevRegister(tHID_DEV_REG_INFO* p_reg_info) {
    tHID_STATUS st;
    BD_ADDR bt_bd_any = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    UINT16 conn_able;

    if (!p_reg_info || !p_reg_info->app_cback)
        return HID_ERR_INVALID_PARAM;

    if (hd_cb.reg_flag)
        return HID_ERR_ALREADY_REGISTERED;

    if (memcmp(p_reg_info->host_addr, bt_bd_any, BD_ADDR_LEN) != 0) {
        hd_cb.host_known = TRUE;
        memcpy(hd_cb.host_addr, p_reg_info->host_addr, BD_ADDR_LEN);
        hd_cb.dev_state = 0;
        conn_able = 0;
    } else {
        hd_cb.host_known = FALSE;
        conn_able = 1;
    }

    hd_cb.virtual_cable = 1;

    if (p_reg_info->qos_info) {
        memcpy(&hd_cb.qos_info, p_reg_info->qos_info, sizeof hd_cb.qos_info);
        hd_cb.use_qos_flg = TRUE;
    } else {
        hd_cb.use_qos_flg = FALSE;
    }

    hd_cb.callback = p_reg_info->app_cback;
    st = hidd_conn_reg();

    if (st != HID_SUCCESS)
        return st;

    hd_cb.reg_flag = TRUE;
    hd_cb.unplug_on = FALSE;

    HIDD_TRACE(DEBUG, "HID_DevRegister successful");

    return HID_SUCCESS;
}

tHID_STATUS HID_DevDeregister(void) {
    if (!hd_cb.reg_flag)
        return HID_ERR_NOT_REGISTERED;

    hidd_mgmt_process_evt(5, NULL);
    hidd_conn_dereg();
    hd_cb.reg_flag = FALSE;

    return HID_SUCCESS;
}

tHID_STATUS HID_DevConnect(void) {
    // NOTE: ELOGNOT ordering
    if (hd_cb.reg_flag == FALSE)
        return HID_ERR_NOT_REGISTERED;

    return hidd_mgmt_process_evt(4, NULL);
}

tHID_STATUS HID_DevDisconnect(void) {
    // NOTE: ELOGNOT ordering
    if (hd_cb.reg_flag == FALSE)
        return HID_ERR_NOT_REGISTERED;

    return hidd_mgmt_process_evt(5, NULL);
}

tHID_STATUS HID_DevHandShake(UINT8 res_code) {
    tSND_DATA_PARAMS hsk_data;

    if (!hd_cb.reg_flag)
        return HID_ERR_NOT_REGISTERED;

    hsk_data.trans_type = 0;
    hsk_data.ctrl_ch = 1;
    hsk_data.param = res_code;
    hsk_data.buf = NULL;

    return hidd_mgmt_process_evt(6, &hsk_data);
}

tHID_STATUS HID_DevVirtualUnplug(void) {
    tHID_STATUS st;
    tSND_DATA_PARAMS vup_data;

    if (!hd_cb.reg_flag)
        return HID_ERR_NOT_REGISTERED;

    vup_data.trans_type = 1;
    vup_data.ctrl_ch = 1;
    vup_data.param = 5;
    vup_data.buf = NULL;

    st = hidd_mgmt_process_evt(6, &vup_data);

    if (st == HID_SUCCESS)
        hd_cb.unplug_on = TRUE;

    return st;
}

tHID_STATUS HID_DevSendData(UINT8 control_ch, UINT8 rep_type, BT_HDR* data_buf) {
    tSND_DATA_PARAMS snd_data;

    WCAssert_Line(424, control_ch != 1 && control_ch != 0);

    if (!hd_cb.reg_flag)
        return HID_ERR_NOT_REGISTERED;

    snd_data.trans_type = 10;
    snd_data.ctrl_ch = control_ch;
    snd_data.param = rep_type;
    snd_data.buf = data_buf;

    return hidd_mgmt_process_evt(6, &snd_data);
}

tHID_STATUS HID_DevSetSecurityLevel(char* serv_name, UINT8 sec_lvl) {
    hd_cb.sec_mask = sec_lvl;

    if (sec_lvl == 0) {
        if (!BTM_SetSecurityLevel(FALSE, serv_name, 41, 0, 17, 7, 2)) {
            HIDD_TRACE(ERROR, "Security Registration 1 failed");
            return HID_ERR_NO_RESOURCES;
        }

        if (!BTM_SetSecurityLevel(TRUE, serv_name, 41, 0, 17, 7, 2)) {
            HIDD_TRACE(ERROR, "Security Registration 2 failed");
            return HID_ERR_NO_RESOURCES;
        }
    } else {
        if (!BTM_SetSecurityLevel(FALSE, serv_name, 42, sec_lvl, 17, 7, 1)) {
            HIDD_TRACE(ERROR, "Security Registration 3 failed");
            return HID_ERR_NO_RESOURCES;
        }

        if (!BTM_SetSecurityLevel(TRUE, serv_name, 42, sec_lvl, 17, 7, 1)) {
            HIDD_TRACE(ERROR, "Security Registration 4 failed");
            return HID_ERR_NO_RESOURCES;
        }
    }

    if (!BTM_SetSecurityLevel(TRUE, serv_name, 43, 0, 19, 7, 0)) {
        HIDD_TRACE(ERROR, "Security Registration 5 failed");
        return HID_ERR_NO_RESOURCES;
    }

    if (!BTM_SetSecurityLevel(FALSE, serv_name, 43, 0, 19, 7, 0)) {
        HIDD_TRACE(ERROR, "Security Registration 6 failed");
        return HID_ERR_NO_RESOURCES;
    }

    return HID_SUCCESS;
}

tHID_STATUS HID_DevSetPowerMgmtParams(UINT8 conn_substate, tHID_DEV_PWR_MD pm_params) {
    if (conn_substate > (int)ARRAY_LENGTH(hd_cb.pm_params) - 1)
        return HID_ERR_INVALID_PARAM;

    memcpy(&hd_cb.pm_params[conn_substate], &pm_params, sizeof hd_cb.pm_params[conn_substate]);

    if (conn_substate == hd_cb.conn_substate)
        hidd_pm_set_power_mode(&hd_cb.pm_params[conn_substate]);

    return HID_SUCCESS;
}
