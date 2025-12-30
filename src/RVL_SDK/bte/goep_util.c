#include "goep_api.h"

/*******************************************************************************
 * headers
 */

#include <mem.h>

#include <macros.h>  // ARRAY_LENGTH

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"
#include "gki_target.h"  // PPC_TASK
#include "wcassert.h"    // WCAssert_Line

#include "btm_api.h"  // BTM_SetSecurityLevel
#include "gki.h"
#include "goep_int.h"
#include "sdp_api.h"
#include "sdpdefs.h"

/*******************************************************************************
 * macros
 */

#define GOEP_NUM_PROTO_ELEMS 3

/*******************************************************************************
 * variables
 */

// .sbss
tGOEP_CB goep_cb;

/*******************************************************************************
 * functions
 */

void GOEP_Init(void) {
    memset(&goep_cb, 0, sizeof goep_cb);

    goep_cb.trace_level = BT_TRACE_LEVEL_DEBUG;
}

BOOLEAN GOEP_SetSecurityLevel(BOOLEAN bOrig, char* pName, UINT32 service, UINT8 level, UINT8 scn) {
    char sec_name[22];  // TODO: Where does 22 come from

    if (!pName)
        pName = "";

    GOEP_TRACE(EVENT,
               "GOEP:  Set Security Mode;  name '%s'; service %d; "
               "level 0x%02x;  scn %d",
               pName, service, level, scn);

    if (strlen(pName) >= ARRAY_LENGTH(sec_name) - 1) {
        strncpy(sec_name, pName, ARRAY_LENGTH(sec_name) - 1);
        sec_name[ARRAY_LENGTH(sec_name) - 1] = '\0';

        pName = sec_name;
    }

    if (!BTM_SetSecurityLevel(bOrig, pName, service, level, BT_PSM_RFCOMM, BTM_SEC_PROTO_RFCOMM, scn)) {
        GOEP_TRACE(WARNING, "GOEP:  Security Registration failed for %s", pName);

        return FALSE;
    }

    return TRUE;
}

void GOEP_SetTraceLevel(UINT8 level) {
    goep_cb.trace_level = level;
}

void GOEP_FreeBuf(void** p_buf) {
    if (!p_buf)
        return;

    if (!*p_buf)
        return;

    GKI_freebuf(*p_buf);
    *p_buf = NULL;
}

void GOEP_SendMsg(void* p_msg) {
    if (!p_msg)
        return;

    GKI_send_msg(PPC_TASK, TASK_MBOX_0, p_msg);
}

tGOEP_STATUS GOEP_Register(char* p_name, UINT32* phSDP, UINT8 scn, UINT8 num_srv_class, UINT16* p_service_class, UINT16 profile_id, UINT16 version) {
    tGOEP_STATUS status = GOEP_ERROR;
    tSDP_PROTOCOL_ELEM protoList[GOEP_NUM_PROTO_ELEMS];
    UINT16 browse;

    WCAssert_Line(202, phSDP);
    WCAssert_Line(203, scn);

    if (!phSDP || !scn)
        return GOEP_INVALID_PARAM;

    *phSDP = SDP_CreateRecord();

    WCAssert_Line(212, *phSDP != 0);

    GOEP_TRACE(API, "GOEP:  Register with SDP:  scn %d, record:0x%08x", scn, *phSDP);

    if (*phSDP == 0)
        return GOEP_RESOURCES;

    if (SDP_AddServiceClassIdList(*phSDP, num_srv_class, p_service_class)) {
        protoList[0].protocol_uuid = UUID_PROTOCOL_L2CAP;
        protoList[0].num_params = 0;

        protoList[1].protocol_uuid = UUID_PROTOCOL_RFCOMM;
        protoList[1].num_params = 1;
        protoList[1].params[0] = scn;

        protoList[2].protocol_uuid = UUID_PROTOCOL_OBEX;
        protoList[2].num_params = 0;

        if (SDP_AddProtocolList(*phSDP, ARRAY_LENGTH(protoList), protoList)) {
            if (p_name && *p_name != '\0') {
                SDP_AddAttribute(*phSDP, ATTR_ID_SERVICE_NAME, TEXT_STR_DESC_TYPE, strlen(p_name) + 1, (UINT8*)p_name);
            }

            if (version)
                SDP_AddProfileDescriptorList(*phSDP, profile_id, version);

            status = GOEP_SUCCESS;
        }
    }

    browse = UUID_SERVCLASS_PUBLIC_BROWSE_GROUP;

    if (status == GOEP_SUCCESS && !SDP_AddUuidSequence(*phSDP, ATTR_ID_BROWSE_GROUP_LIST, 1, &browse)) {
        status = GOEP_ERROR;
    }

    if (status != GOEP_SUCCESS) {
        SDP_DeleteRecord(*phSDP);

        GOEP_TRACE(DEBUG, "GOEP_Register status: %d", status);
    } else {
        GOEP_TRACE(EVENT, "GOEP:  Register with SDP returns handle 0x%08x", *phSDP);
    }

    return status;
}
