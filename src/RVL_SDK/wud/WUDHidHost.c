#include "revolution.h"
#include "revolution/bte.h"
#include "revolution/os/OSInterrupt.h"
#include "revolution/wud.h"
#include <cstring>

void WUDHidHostCallback(tBTA_HH_EVT event, tBTA_HH *data) {
    tBTA_HH_CONN* conn;
    tBTA_HH_CBDATA* cb;
    tBTA_HH_ACL_QUEUE_INFO* queue_info;
    WUDDeviceInfo* info;
    WUDControlBlock* block = &_wcb;
    s32 i;

    switch (event) {
        case 0:
            WUD_DEBUGPrint("BTA_HH_ENABLE_EVT\n");
            block->profileMask |= 1;
            break;
        case 1:
            WUD_DEBUGPrint("BTA_HH_DISABLE_EVT\n");
            break;
        case 2:
            WUD_DEBUGPrint("BTA_HH_OPEN_EVT\n");
            conn = (tBTA_HH_CONN*)data;
            WUD_DEBUGPrint("handle: %d, addr: %02x:%02x:%02x:%02x:%02x:%02x\n", 
                conn->handle, conn->bda[0], conn->bda[1], conn->bda[2], conn->bda[3], conn->bda[4], conn->bda[5]);

            if (conn->status == 0) {
                if (!memcmp(_work.bd_addr, conn->bda, 6)) {
                    info = &_work;
                }
                else {
                    info = WUDiGetDevInfo(conn->bda);
                }

                switch (info->status) {
                    case 12:
                        block->syncStatus = 18;
                        break;
                    case 2:
                        block->syncStatus = 23;
                        break;
                }

                info->status = 8;
                info->handle = conn->handle;
                block->connNums++;
                info = WUDiGetDevInfo(conn->bda);

                if (info == NULL) {
                    info = &_work;
                }

                _dev_handle_to_bda[conn->handle] = info->bd_addr;
                _dev_handle_queue_size[conn->handle] = 0;
                _dev_handle_notack_num[conn->handle] = 0;

                if (info->sync_type == 3 || info->sync_type == 1) {
                    WUDiMoveTopSmpDevInfoPtr(info);
                }
                else {
                    WUDiMoveTopStdDevInfoPtr(info);
                }

                WUDSetSniffMode(info->bd_addr, 8);

                if (block->hidConnCallback) {
                    block->hidConnCallback(info, 1);
                }
            }
            else {
                WUD_DEBUGPrint("error code: %d\n", conn->status);

                if (block->syncStatus != 0) {
                    if (!memcmp(conn->bda, _work.bd_addr, 6)) {
                        if (_work.status == 2) {
                            if (WUDiGetDevInfo(conn->bda) && conn->status == 12) {
                                WUDiRemoveDevice(conn->bda);
                                block->linkNums--;
                            }

                            block->syncStatus = 255;
                        }
                    }
                }
                else {
                    if (WUDiGetDevInfo(conn->bda) && conn->status == 12) {
                        info = WUDiGetDevInfo(conn->bda);
                        if (info != NULL) {
                            if (info->sync_type == 3 || info->sync_type == 1) {
                                WUDiMoveBottomSmpDevInfoPtr(info);
                            }
                            else {
                                WUDiMoveBottomStdDevInfoPtr(info);
                            }
                        }

                        WUDiRemoveDevice(conn->bda);
                        block->linkNums--;
                    }
                }
            }
            break;
        case 3:
            WUD_DEBUGPrint("BTA_HH_CLOSE_EVT\n");
            cb = (tBTA_HH_CBDATA*)data;
            block->connNums--;
            WUD_DEBUGPrint("device handle : %d   status = %d\n", cb->handle, cb->status);
            info = WUDiGetDevInfo(_dev_handle_to_bda[cb->handle]);
            if (info != NULL) {
                if (info->sync_type == 3 || info->sync_type == 1) {
                    WUDiMoveTopOfDisconnectedSmpDevice(info);
                }
                else {
                    WUDiMoveTopOfDisconnectedStdDevice(info);
                }
            }
            
            _dev_handle_to_bda[cb->handle] = NULL;
            _dev_handle_queue_size[cb->handle] = 0;
            _dev_handle_notack_num[cb->handle] = 0;

            if (block->hidConnCallback) {
                block->hidConnCallback(info, 0);
            }
            break;

        case 5:
            WUD_DEBUGPrint("BTA_HH_SET_RPT_EVT\n");
            break;
        case 4:
            WUD_DEBUGPrint("BTA_HH_GET_RPT_EVT\n");
            break;
        case 7:
            WUD_DEBUGPrint("BTA_HH_SET_PROTO_EVT\n");
            break;
        case 6:
            WUD_DEBUGPrint("BTA_HH_GET_PROTO_EVT\n");
            break;
        case 9:
            WUD_DEBUGPrint("BTA_HH_SET_IDLE_EVT\n");
            break;
        case 8:
            WUD_DEBUGPrint("BTA_HH_GET_IDLE_EVT\n");
            break;
        case 10:
            WUD_DEBUGPrint("BTA_HH_GET_DCSP_EVT\n");
            break;
        case 11:
            WUD_DEBUGPrint("BTA_HH_ADD_DEV_EVT\n");
            conn = (tBTA_HH_CONN*)data;
            WUD_DEBUGPrint("result: %d, handle: %d, addr: %02x:%02x:%02x:%02x:%02x:%02x\n", 
                conn->status, conn->handle, conn->bda[0], conn->bda[1], 
                conn->bda[2], conn->bda[3], conn->bda[4], conn->bda[5]);

            info = WUDiGetDevInfo(conn->bda);
            info->handle = conn->handle;
            _dev_handle_to_bda[conn->handle] = info->bd_addr;
            _dev_handle_queue_size[conn->handle] = 0;
            _dev_handle_notack_num[conn->handle] = 0;
            break;
        case 12:
            WUD_DEBUGPrint("BTA_HH_RMV_DEV_EVT\n");
            conn = (tBTA_HH_CONN*)data;
            WUD_DEBUGPrint("result: %d, handle: %d, addr: %02x:%02x:%02x:%02x:%02x:%02x\n", 
                conn->status, conn->handle, conn->bda[0], conn->bda[1], 
                conn->bda[2], conn->bda[3], conn->bda[4], conn->bda[5]);
            break;
        case 13:
            WUD_DEBUGPrint("BTA_HH_VS_UNPLUG_EVT\n");
            break;
        case 15:
            queue_info = (tBTA_HH_ACL_QUEUE_INFO*)data;
            block->aclAvailBufSize = queue_info->num_avail_buffs;
            block->aclMaxBufSize = queue_info->num_buffs;

            if (block->linkNums < queue_info->num_links) {
                OSReport("WARNING: link num count is modified.\n");
                block->linkNums = queue_info->num_links;
            }

            for (i = 0; i < queue_info->num_links; i++) {
                if (queue_info->queue_info[i].handle >= 0 && queue_info->queue_info[i].handle < 16) {
                    _dev_handle_queue_size[queue_info->queue_info[i].handle] = queue_info->queue_info[i].num_queued;
                    _dev_handle_notack_num[queue_info->queue_info[i].handle] = queue_info->queue_info[i].num_not_acked;
                }
            }
            break;
    }
}

void bta_hh_co_data(u8 handle, u8* rpt, u16 len, tBTA_HH_PROTO_MODE mode, u8 subclass, u8 id) {
    WUDControlBlock* block = &_wcb;

    if (id == 3) {
        if (block->hidRecvCallback) {
            block->hidRecvCallback(handle, rpt, len);
        }
    }
    else {
        WUD_DEBUGPrint("Invalid app_id [%d]\n", id);
    }
}

void bta_hh_co_open(u8, u8, tBTA_HH_ATTR_MASK, u8) {
    WUD_DEBUGPrint("bta_hh_co_open()\n");
}

void bta_hh_co_close(u8, u8) {
    WUD_DEBUGPrint("bta_hh_co_close()\n");
}

BOOLEAN bta_dm_co_get_compress_memory(tBTA_SYS_ID, UINT8 **) {
    return 0;
}
