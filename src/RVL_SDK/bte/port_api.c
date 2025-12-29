#include "port_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/port_api.c
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

#include "gki.h"
#include "l2c_api.h"
#include "port_int.h"
#include "rfc_int.h"
#include "rfcdefs.h"
#include "sdpdefs.h"

/*******************************************************************************
 * macros
 */

#define PORT_BREAK_DURATION 1

/*******************************************************************************
 * local function declarations
 */

static int port_write(tPORT* p_port, BT_HDR* p_buf);

/*******************************************************************************
 * functions
 */

int RFCOMM_CreateConnection(UINT16 uuid, UINT8 scn, BOOLEAN is_server, UINT16 mtu, BD_ADDR bd_addr, UINT16* p_handle, tPORT_CALLBACK* p_mgmt_cb) {
    tPORT* p_port;
    int i;
    UINT8 dlci;
    tRFC_MCB* p_mcb = port_find_mcb(bd_addr);
    UINT16 rfcomm_mtu;

    RFCOMM_TRACE(API, "RFCOMM_CreateConnection() called SCN: %d is_server:%d mtu:%d", scn, is_server, mtu);

    RFCOMM_TRACE(API, "RFCOMM_CreateConnection()  BDA: %02x-%02x-%02x-%02x-%02x-%02x", bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4],
                 bd_addr[5]);

    *p_handle = 0;

    if (p_mcb && !p_mcb->is_initiator && !is_server)
        dlci = (scn << 1) + 1;
    else
        dlci = scn << 1;

    if (!is_server && ((p_port = port_find_port(dlci, bd_addr)) != NULL)) {
        if (!p_port->is_server) {
            RFCOMM_TRACE(ERROR,
                         "RFCOMM_CreateConnection - already opened "
                         "state:%d, RFC state:%d, MCB state:%d",
                         p_port->state, p_port->rfc.state, p_port->rfc.p_mcb ? p_port->rfc.p_mcb->state : 0);
            return PORT_ALREADY_OPENED;
        }
    }

    if ((p_port = port_allocate_port(dlci, bd_addr)) == NULL) {
        RFCOMM_TRACE(WARNING, "RFCOMM_CreateConnection - no resources");
        return PORT_NO_RESOURCES;
    }

    p_port->default_signal_state = PORT_DTRDSR_ON | PORT_CTSRTS_ON | PORT_DCD_ON;

    switch (uuid) {
    case UUID_PROTOCOL_OBEX:
        p_port->default_signal_state = PORT_OBEX_DEFAULT_SIGNAL_STATE;
        break;

    case UUID_SERVCLASS_SERIAL_PORT:
        p_port->default_signal_state = PORT_SPP_DEFAULT_SIGNAL_STATE;
        break;

    case UUID_SERVCLASS_LAN_ACCESS_USING_PPP:
        p_port->default_signal_state = PORT_PPP_DEFAULT_SIGNAL_STATE;
        break;

    case UUID_SERVCLASS_DIALUP_NETWORKING:
    case UUID_SERVCLASS_FAX:
        p_port->default_signal_state = PORT_DUN_DEFAULT_SIGNAL_STATE;
        break;
    }

    RFCOMM_TRACE(EVENT, "RFCOMM_CreateConnection dlci:%d signal state:0x%x", dlci, p_port->default_signal_state);

    *p_handle = p_port->inx;

    p_port->state = PORT_STATE_OPENING;
    p_port->uuid = uuid;
    p_port->is_server = is_server;
    p_port->scn = scn;
    p_port->ev_mask = 0;

    if (mtu) {
        rfcomm_mtu = L2CAP_MTU_SIZE - RFCOMM_DATA_OVERHEAD;

        if (mtu)  // ?
            p_port->mtu = mtu < rfcomm_mtu ? mtu : rfcomm_mtu;
        else
            p_port->mtu = rfcomm_mtu;
    }

    p_port->local_ctrl.modem_signal = p_port->default_signal_state;
    p_port->local_ctrl.fc = FALSE;

    p_port->p_mgmt_callback = p_mgmt_cb;

    for (i = 0; i < BD_ADDR_LEN; ++i)
        p_port->bd_addr[i] = bd_addr[i];

    if (p_port->is_server)
        return PORT_SUCCESS;

    return port_open_continue(p_port);
}

int RFCOMM_RemoveConnection(UINT16 handle) {
    tPORT* p_port;

    RFCOMM_TRACE(API, "RFCOMM_RemoveConnection() handle:%d", handle);

    if (handle == 0 || handle > MAX_RFC_PORTS) {
        RFCOMM_TRACE(ERROR, "RFCOMM_RemoveConnection() BAD handle:%d", handle);
        return PORT_BAD_HANDLE;
    }

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED) {
        RFCOMM_TRACE(EVENT, "RFCOMM_RemoveConnection() Not opened:%d", handle);
        return PORT_SUCCESS;
    }

    p_port->state = PORT_STATE_CLOSING;

    port_start_close(p_port);

    return PORT_SUCCESS;
}

int PORT_SetEventCallback(UINT16 port_handle, tPORT_CALLBACK* p_port_cb) {
    tPORT* p_port;

    if (port_handle == 0 || port_handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[port_handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    RFCOMM_TRACE(API, "PORT_SetEventCallback() handle:%d", port_handle);

    p_port->p_callback = p_port_cb;

    return PORT_SUCCESS;
}

int PORT_SetDataCallback(UINT16 port_handle, tPORT_DATA_CALLBACK* p_port_cb) {
    tPORT* p_port;

    RFCOMM_TRACE(API, "PORT_SetDataCallback() handle:%d cb 0x%x", port_handle, p_port_cb);

    if (port_handle == 0 || port_handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[port_handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    p_port->p_data_callback = p_port_cb;

    return PORT_SUCCESS;
}

int PORT_SetEventMask(UINT16 port_handle, UINT32 mask) {
    tPORT* p_port;

    RFCOMM_TRACE(API, "PORT_SetEventMask() handle:%d mask:0x%x", port_handle, mask);

    if (port_handle == 0 || port_handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[port_handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    p_port->ev_mask = mask;

    return PORT_SUCCESS;
}

int PORT_CheckConnection(UINT16 handle, BD_ADDR bd_addr, UINT16* p_lcid) {
    tPORT* p_port;

    RFCOMM_TRACE(API, "PORT_CheckConnection() handle:%d", handle);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    if (!p_port->rfc.p_mcb || !p_port->rfc.p_mcb->peer_ready || p_port->rfc.state != RFC_STATE_OPENED) {
        return PORT_LINE_ERR;
    }

    memcpy(bd_addr, p_port->rfc.p_mcb->bd_addr, BD_ADDR_LEN);
    *p_lcid = p_port->rfc.p_mcb->lcid;

    return PORT_SUCCESS;
}

int PORT_SetState(UINT16 handle, tPORT_STATE* p_settings) {
    tPORT* p_port;
    UINT8 baud_rate;

    RFCOMM_TRACE(API, "PORT_SetState() handle:%d", handle);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    if (p_port->line_status)
        return PORT_LINE_ERR;

    RFCOMM_TRACE(API, "PORT_SetState() handle:%d FC_TYPE:0x%x", handle, p_settings->fc_type);

    baud_rate = p_port->user_port_pars.baud_rate;
    p_port->user_port_pars = *p_settings;

    if (baud_rate != p_settings->baud_rate)
        port_start_par_neg(p_port);

    return PORT_SUCCESS;
}

int PORT_GetState(UINT16 handle, tPORT_STATE* p_settings) {
    tPORT* p_port;

    RFCOMM_TRACE(API, "PORT_GetState() handle:%d", handle);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    if (p_port->line_status)
        return PORT_LINE_ERR;

    *p_settings = p_port->user_port_pars;

    return PORT_SUCCESS;
}

int PORT_Control(UINT16 handle, UINT8 signal) {
    tPORT* p_port;
    UINT8 old_modem_signal;

    RFCOMM_TRACE(API, "PORT_Control() handle:%d signal:0x%x", handle, signal);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    old_modem_signal = p_port->local_ctrl.modem_signal;
    p_port->local_ctrl.break_signal = 0;

    switch (signal) {
    case PORT_SET_CTSRTS:
        p_port->local_ctrl.modem_signal |= PORT_CTSRTS_ON;
        break;

    case PORT_CLR_CTSRTS:
        p_port->local_ctrl.modem_signal &= ~PORT_CTSRTS_ON;
        break;

    case PORT_SET_DTRDSR:
        p_port->local_ctrl.modem_signal |= PORT_DTRDSR_ON;
        break;

    case PORT_CLR_DTRDSR:
        p_port->local_ctrl.modem_signal &= ~PORT_DTRDSR_ON;
        break;

    case PORT_SET_RI:
        p_port->local_ctrl.modem_signal |= PORT_RING_ON;
        break;

    case PORT_CLR_RI:
        p_port->local_ctrl.modem_signal &= ~PORT_RING_ON;
        break;

    case PORT_SET_DCD:
        p_port->local_ctrl.modem_signal |= PORT_DCD_ON;
        break;

    case PORT_CLR_DCD:
        p_port->local_ctrl.modem_signal &= ~PORT_DCD_ON;
        break;
    }

    if (signal == PORT_BREAK)
        p_port->local_ctrl.break_signal = PORT_BREAK_DURATION;
    else if (p_port->local_ctrl.modem_signal == old_modem_signal)
        return PORT_SUCCESS;

    port_start_control(p_port);

    RFCOMM_TRACE(EVENT, "PORT_Control DTR_DSR : %d, RTS_CTS : %d, RI : %d, DCD : %d", p_port->local_ctrl.modem_signal & MODEM_SIGNAL_DTRDSR ? 1 : 0,
                 p_port->local_ctrl.modem_signal & MODEM_SIGNAL_RTSCTS ? 1 : 0, p_port->local_ctrl.modem_signal & MODEM_SIGNAL_RI ? 1 : 0,
                 p_port->local_ctrl.modem_signal & MODEM_SIGNAL_DCD ? 1 : 0);

    return PORT_SUCCESS;
}

int PORT_FlowControl(UINT16 handle, BOOLEAN enable) {
    tPORT* p_port;
    BOOLEAN old_fc;
    UINT32 events;

    RFCOMM_TRACE(API, "PORT_FlowControl() handle:%d enable: %d", handle, enable);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    if (!p_port->rfc.p_mcb)
        return PORT_NOT_OPENED;

    p_port->rx.user_fc = !enable;

    if (p_port->rfc.p_mcb->flow == PORT_FC_CREDIT) {
        if (!p_port->rx.user_fc)
            port_flow_control_peer(p_port, TRUE, 0);
    } else {
        old_fc = p_port->local_ctrl.fc;

        p_port->local_ctrl.fc = p_port->rx.user_fc | p_port->rx.peer_fc;

        if (p_port->local_ctrl.fc != old_fc)
            port_start_control(p_port);
    }

    if (enable && p_port->rx.queue_size != 0) {
        events = PORT_EV_RXCHAR;

        if (p_port->rx_flag_ev_pending) {
            p_port->rx_flag_ev_pending = FALSE;
            events |= PORT_EV_RXFLAG;
        }

        events &= p_port->ev_mask;

        if (p_port->p_callback && events)
            (*p_port->p_callback)(events, p_port->inx);
    }

    return PORT_SUCCESS;
}

int PORT_GetModemStatus(UINT16 handle, UINT8* p_signal) {
    tPORT* p_port;

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    *p_signal = p_port->peer_ctrl.modem_signal;

    RFCOMM_TRACE(API, "PORT_GetModemStatus() handle:%d signal:%x", handle, *p_signal);

    return PORT_SUCCESS;
}

int PORT_ClearError(UINT16 handle, UINT16* p_errors, tPORT_STATUS* p_status) {
    tPORT* p_port;

    RFCOMM_TRACE(API, "PORT_ClearError() handle:%d", handle);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    *p_errors = p_port->line_status;

    p_port->line_status = p_port->line_status & LINE_STATUS_FAILED;

    PORT_GetQueueStatus(handle, p_status);

    return PORT_SUCCESS;
}

int PORT_SendError(UINT16 handle, UINT8 errors) {
    tPORT* p_port;

    RFCOMM_TRACE(API, "PORT_SendError() handle:%d errors:0x%x", handle, errors);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    if (!p_port->rfc.p_mcb)
        return PORT_NOT_OPENED;

    RFCOMM_LineStatusReq(p_port->rfc.p_mcb, p_port->dlci, errors);

    return PORT_SUCCESS;
}

int PORT_GetQueueStatus(UINT16 handle, tPORT_STATUS* p_status) {
    tPORT* p_port;

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    p_status->in_queue_size = p_port->rx.queue_size;
    p_status->out_queue_size = p_port->tx.queue_size;

    p_status->mtu_size = (UINT16)p_port->peer_mtu;

    p_status->flags = 0;

    if (!(p_port->peer_ctrl.modem_signal & PORT_CTSRTS_ON))
        p_status->flags |= PORT_FLAG_CTS_HOLD;

    if (!(p_port->peer_ctrl.modem_signal & PORT_DTRDSR_ON))
        p_status->flags |= PORT_FLAG_DSR_HOLD;

    if (!(p_port->peer_ctrl.modem_signal & PORT_DCD_ON))
        p_status->flags |= PORT_FLAG_RLSD_HOLD;

    return PORT_SUCCESS;
}

int PORT_Purge(UINT16 handle, UINT8 purge_flags) {
    tPORT* p_port;
    BT_HDR* p_buf;
    UINT16 count;
    UINT32 events;

    RFCOMM_TRACE(API, "PORT_Purge() handle:%d flags:0x%x", handle, purge_flags);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    if (purge_flags & PORT_PURGE_RXCLEAR) {
        count = p_port->rx.queue.count;

        while ((p_buf = GKI_dequeue(&p_port->rx.queue)) != NULL)
            GKI_freebuf(p_buf);

        p_port->rx.queue_size = 0;

        if (count)
            port_flow_control_peer(p_port, TRUE, count);
    }

    if (purge_flags & PORT_PURGE_TXCLEAR) {
        while ((p_buf = GKI_dequeue(&p_port->tx.queue)) != NULL)
            GKI_freebuf(p_buf);

        p_port->tx.queue_size = 0;

        events = PORT_EV_TXEMPTY;

        events |= port_flow_control_user(p_port);

        events &= p_port->ev_mask;

        if (p_port->p_callback && events)
            (*p_port->p_callback)(events, p_port->inx);
    }

    return PORT_SUCCESS;
}

int PORT_ReadData(UINT16 handle, char* p_data, UINT16 max_len, UINT16* p_len) {
    tPORT* p_port;
    BT_HDR* p_buf;
    UINT16 count;

    RFCOMM_TRACE(API, "PORT_ReadData() handle:%d max_len:%d", handle, max_len);

    *p_len = 0;

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    if (p_port->line_status)
        return PORT_LINE_ERR;

    p_buf = GKI_getfirst(&p_port->rx.queue);
    if (!p_buf)
        return PORT_SUCCESS;

    count = p_port->rx.queue.count;

    GKI_disable();

    while (max_len && p_buf) {
        if (p_buf->len > max_len) {
            memcpy(p_data, (UINT8*)(p_buf + 1) + p_buf->offset, max_len);
            p_buf->offset += max_len;
            p_buf->len -= max_len;

            *p_len += max_len;

            break;
        } else {
            memcpy(p_data, (UINT8*)(p_buf + 1) + p_buf->offset, p_buf->len);

            *p_len += p_buf->len;
            max_len -= p_buf->len;

            if (max_len) {
                p_data += p_buf->len;
                p_buf = GKI_getnext(p_buf);
            }

            GKI_freebuf(GKI_dequeue(&p_port->rx.queue));
        }
    }

    p_port->rx.queue_size -= *p_len;

    GKI_enable();

    if (*p_len == 1) {
        RFCOMM_TRACE(EVENT, "PORT_ReadData queue:%d returned:%d %x", p_port->rx.queue_size, *p_len, p_data[0]);
    } else {
        RFCOMM_TRACE(EVENT, "PORT_ReadData queue:%d returned:%d", p_port->rx.queue_size, *p_len);
    }

    count -= p_port->rx.queue.count;

    port_flow_control_peer(p_port, TRUE, count);

    return PORT_SUCCESS;
}

int PORT_Read(UINT16 handle, BT_HDR** pp_buf) {
    tPORT* p_port;
    BT_HDR* p_buf;

    RFCOMM_TRACE(API, "PORT_Read() handle:%d", handle);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    if (p_port->line_status)
        return PORT_LINE_ERR;

    p_buf = GKI_dequeue(&p_port->rx.queue);
    if (p_buf) {
        p_port->rx.queue_size -= p_buf->len;

        port_flow_control_peer(p_port, TRUE, 1);
    }

    *pp_buf = p_buf;

    return PORT_SUCCESS;
}

static int port_write(tPORT* p_port, BT_HDR* p_buf) {
    if (p_port->is_server && p_port->rfc.state != RFC_STATE_OPENED) {
        GKI_freebuf(p_buf);
        return PORT_CLOSED;
    }

    if (p_port->tx.peer_fc || !p_port->rfc.p_mcb || !p_port->rfc.p_mcb->peer_ready || p_port->rfc.state != RFC_STATE_OPENED ||
        (p_port->port_ctrl & (PORT_CTRL_REQ_SENT | PORT_CTRL_IND_RECEIVED)) != (PORT_CTRL_REQ_SENT | PORT_CTRL_IND_RECEIVED)) {
        if (p_port->tx.queue_size > PORT_TX_CRITICAL_WM || p_port->tx.queue.count > PORT_TX_BUF_CRITICAL_WM) {
            RFCOMM_TRACE(WARNING, "PORT_Write: Queue size: %d", p_port->tx.queue_size);

            GKI_freebuf(p_buf);

            if (p_port->p_callback && p_port->ev_mask & PORT_EV_ERR)
                (*p_port->p_callback)(PORT_EV_ERR, p_port->inx);

            return PORT_TX_FULL;
        }

        RFCOMM_TRACE(EVENT,
                     "PORT_Write : Data is enqued. flow disabled %d peer_ready %d state "
                     "%d ctrl_state %x",
                     p_port->tx.peer_fc, p_port->rfc.p_mcb && p_port->rfc.p_mcb->peer_ready, p_port->rfc.state, p_port->port_ctrl);

        GKI_enqueue(&p_port->tx.queue, p_buf);
        p_port->tx.queue_size += p_buf->len;

        return PORT_CMD_PENDING;
    } else {
        RFCOMM_TRACE(EVENT, "PORT_Write : Data is being sent");

        RFCOMM_DataReq(p_port->rfc.p_mcb, p_port->dlci, p_buf);

        return PORT_SUCCESS;
    }
}

int PORT_Write(UINT16 handle, BT_HDR* p_buf) {
    tPORT* p_port;
    UINT32 event = 0;
    int rc;

    RFCOMM_TRACE(API, "PORT_Write() handle:%d", handle);

    if (handle == 0 || handle > MAX_RFC_PORTS) {
        GKI_freebuf(p_buf);
        return PORT_BAD_HANDLE;
    }

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED) {
        GKI_freebuf(p_buf);
        return PORT_NOT_OPENED;
    }

    if (p_port->line_status) {
        RFCOMM_TRACE(WARNING, "PORT_Write: Data dropped line_status:0x%x", p_port->line_status);

        GKI_freebuf(p_buf);
        return PORT_LINE_ERR;
    }

    rc = port_write(p_port, p_buf);
    event |= port_flow_control_user(p_port);

    switch (rc) {
    case PORT_TX_FULL:
        event |= PORT_EV_ERR;
        break;

    case PORT_SUCCESS:
        event |= PORT_EV_TXCHAR | PORT_EV_TXEMPTY;
        break;
    }

    event &= p_port->ev_mask;

    if (p_port->p_callback && event)
        (*p_port->p_callback)(event, p_port->inx);

    return PORT_SUCCESS;
}

int PORT_WriteData(UINT16 handle, char* p_data, UINT16 max_len, UINT16* p_len) {
    tPORT* p_port;
    BT_HDR* p_buf;
    UINT32 event = 0;
    int rc = 0;
    UINT16 length;

    RFCOMM_TRACE(API, "PORT_WriteData() max_len:%d", max_len);

    *p_len = 0;

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return PORT_BAD_HANDLE;

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED) {
        RFCOMM_TRACE(WARNING, "PORT_WriteData() no port state:%d", p_port->state);
        return PORT_NOT_OPENED;
    }

    if (!max_len || !p_port->peer_mtu) {
        RFCOMM_TRACE(ERROR, "PORT_WriteData() peer_mtu:%d", p_port->peer_mtu);
        return PORT_UNKNOWN_ERROR;
    }

    length = RFCOMM_DATA_POOL_BUF_SIZE - sizeof(BT_HDR) - L2CAP_MIN_OFFSET - RFCOMM_DATA_OVERHEAD;

    GKI_disable();

    if (((p_buf = (BT_HDR*)p_port->tx.queue.p_last) != NULL) && p_buf->len + max_len <= p_port->peer_mtu && p_buf->len + max_len <= length) {
        memcpy((UINT8*)(p_buf + 1) + p_buf->offset + p_buf->len, p_data, max_len);
        p_port->tx.queue_size += max_len;

        *p_len = max_len;
        p_buf->len += max_len;

        GKI_enable();

        return PORT_SUCCESS;
    }

    GKI_enable();

    while (max_len) {
        if (p_port->tx.queue_size > PORT_TX_HIGH_WM || p_port->tx.queue.count > PORT_TX_BUF_HIGH_WM) {
            break;
        }

        p_buf = GKI_getpoolbuf(RFCOMM_DATA_POOL_ID);
        if (!p_buf)
            break;

        p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET;
        p_buf->layer_specific = handle;

        if (p_port->peer_mtu < length)
            length = p_port->peer_mtu;

        if (max_len < length)
            length = max_len;

        p_buf->len = length;
        p_buf->event = BT_EVT_TO_BTU_SP_DATA;

        memcpy((UINT8*)(p_buf + 1) + p_buf->offset, p_data, length);

        RFCOMM_TRACE(EVENT, "PORT_WriteData %d bytes", length);

        rc = port_write(p_port, p_buf);

        event |= port_flow_control_user(p_port);

        if (rc == PORT_SUCCESS)
            event |= PORT_EV_TXCHAR;

        if (rc != PORT_SUCCESS && rc != PORT_CMD_PENDING)
            break;

        *p_len += length;
        max_len -= length;
        p_data += length;
    }

    if (!max_len && rc != PORT_CMD_PENDING && rc != PORT_TX_QUEUE_DISABLED)
        event |= PORT_EV_TXEMPTY;

    event &= p_port->ev_mask;

    if (p_port->p_callback && event)
        (*p_port->p_callback)(event, p_port->inx);

    return PORT_SUCCESS;
}

int PORT_Test(UINT16 handle, UINT8* p_data, UINT16 len) {
    BT_HDR* p_buf;
    tPORT* p_port;

    RFCOMM_TRACE(API, "PORT_Test() len:%d", len);

    if (handle == 0 || handle > MAX_RFC_PORTS)
        return (PORT_BAD_HANDLE);

    p_port = &rfc_cb.port.port[handle - 1];

    if (!p_port->in_use || p_port->state == PORT_STATE_CLOSED)
        return PORT_NOT_OPENED;

    if (len > (p_port->mtu == 0 ? RFCOMM_DEFAULT_MTU : p_port->mtu))
        return PORT_UNKNOWN_ERROR;

    if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) != NULL) {
        p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET + 2;
        p_buf->len = len;

        memcpy((UINT8*)(p_buf + 1) + p_buf->offset, p_data, p_buf->len);

        rfc_send_test(p_port->rfc.p_mcb, TRUE, p_buf);

        return PORT_SUCCESS;
    } else {
        return PORT_NO_MEM;
    }
}

void RFCOMM_Init(void) {
    memset(&rfc_cb, 0, sizeof rfc_cb);

    rfc_cb.rfc.last_mux = MAX_BD_CONNECTIONS;

    rfc_cb.trace_level = BT_TRACE_LEVEL_DEBUG;

    rfcomm_l2cap_if_init();
}

UINT8 PORT_SetTraceLevel(UINT8 new_level) {
    if (new_level != 0xff)
        rfc_cb.trace_level = new_level;

    return rfc_cb.trace_level;
}
