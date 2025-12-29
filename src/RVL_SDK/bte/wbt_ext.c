#include "wbt_api.h"

/*******************************************************************************
 * headers
 */

#include <mem.h>

#include <macros.h>  // ARRAY_LENGTH

#include "data_types.h"

#include "hcidefs.h"  // LMP_COMPID_BROADCOM
#include "sdp_api.h"
#include "sdpdefs.h"

/*******************************************************************************
 * functions
 */

BOOLEAN WBT_ExtCreateRecord(void) {
    tSDP_DI_GET_RECORD get_device_info;
    UINT32 sdp_record_handle = 0;
    tSDP_DI_RECORD* p_set_info;
    UINT8 temp_u16[2];
    UINT16 list = UUID_SERVCLASS_PUBLIC_BROWSE_GROUP;

    if (SDP_GetLocalDiRecord(&get_device_info, &sdp_record_handle) != SDP_SUCCESS) {
        p_set_info = &get_device_info.rec;

        memset(p_set_info, 0, sizeof *p_set_info);
        p_set_info->vendor = LMP_COMPID_BROADCOM;
        p_set_info->vendor_id_source = DI_VENDOR_ID_SOURCE_BTSIG;
        p_set_info->primary_record = TRUE;

        if (SDP_SetLocalDiRecord(p_set_info, &sdp_record_handle) != SDP_SUCCESS) {
            sdp_record_handle = 0;
            return FALSE;
        }
    }

    SDP_AddUuidSequence(sdp_record_handle, ATTR_ID_BROWSE_GROUP_LIST, 1, &list);

    temp_u16[0] = 0;
    temp_u16[1] = 1;
    SDP_AddAttribute(sdp_record_handle, ATTR_ID_EXT_BRCM_VERSION, UINT_DESC_TYPE, ARRAY_LENGTH(temp_u16), temp_u16);

    return TRUE;
}

BOOLEAN WBT_ExtAddPinCode(void) {
    UINT32 sdp_record_handle = 0;
    UINT8 temp_u32[4];
    tSDP_DI_GET_RECORD get_device_info;

    if (SDP_GetLocalDiRecord(&get_device_info, &sdp_record_handle) == SDP_SUCCESS) {
        temp_u32[0] = 0;
        temp_u32[1] = 0;
        temp_u32[2] = 0;
        temp_u32[3] = 0;

        SDP_AddAttribute(sdp_record_handle, ATTR_ID_EXT_PIN_CODE, 1, ARRAY_LENGTH(temp_u32), temp_u32);

        return TRUE;
    } else {
        return FALSE;
    }
}

UINT32 wbt_sdp_show_ext(UINT8 scn, char* service_name, UINT8 pin_code_ext, UINT8 active_sync_ext) {
    tSDP_DI_RECORD device_info;
    UINT32 sdp_record_handle;

    device_info.client_executable_url[0] = '\0';
    device_info.documentation_url[0] = '\0';
    strcpy(device_info.service_description, service_name);
    device_info.vendor = LMP_COMPID_BROADCOM;
    device_info.vendor_id_source = DI_VENDOR_ID_SOURCE_BTSIG;
    device_info.product = 0x1234;  // TODO: which product is this
    device_info.version = 1;       // TODO: should this be a define or no
    device_info.primary_record = TRUE;

    if (SDP_SetLocalDiRecord(&device_info, &sdp_record_handle) != SDP_SUCCESS)
        sdp_record_handle = 0;

    return sdp_record_handle;
}
