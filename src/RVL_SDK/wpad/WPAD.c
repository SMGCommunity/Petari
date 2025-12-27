#include "revolution/wpad.h"
#include "private/flipper.h"
#include "revolution/dvd.h"
#include "revolution/sc.h"
#include "revolution/vi.h"
#include "revolution/wud.h"
#include <cstdio>
#include <mem.h>

extern volatile BOOL __OSIsReturnToIdle;

WPADControlBlock _wpd[WPAD_MAX_CONTROLLERS];
WPADControlBlock* _wpdcb[WPAD_MAX_CONTROLLERS];

u8 _sleepTime;
u8 _dpdSensitivity;
u8 _sensorBarPos;
BOOL _rumble;
u8 _speakerVolume;
u8 _scFlush;
u8 _gametype;
u16 _gameTitle[17];
const char* _gamecode;

s8 _dev_handle_index[16];
u8 _chan_active_state[WPAD_MAX_CONTROLLERS];
OSAlarm _managerAlarm;

DVDDiskID _diskId;

static u8 _scSetting;
static vu8 _shutdown;
static s8 _afhChannel;
static u8 _regShutdown = 0;

static u16 _senseCnt = 0;
static u8 _checkCnt = 0;
static u16 _afhCnt = 0;

static u8 _extCnt[WPAD_MAX_CONTROLLERS] = {0, 0, 0, 0};
static u8 _rumbleCnt[WPAD_MAX_CONTROLLERS] = {0, 0, 0, 0};

static s8 _infRes[4];

static BOOL _initialized;
static BOOL _startup = FALSE;
static int _recFlag = -1;
static int _recCnt = 0;

#define WPAD_DEFAULT_ACC_DIFF_COUNT_THRESHOLD (u16)(6)
#define WPAD_DEFAULT_ACC_HYST_COUNT_THRESHOLD (u16)(30)

#define WPAD_DEFAULT_DPD_DIFF_COUNT_THRESHOLD (u16)(4)
#define WPAD_DEFAULT_DPD_HYST_COUNT_THRESHOLD (u16)(30)

static u16 __WPAD_acc_diff_count_threshold = WPAD_DEFAULT_ACC_DIFF_COUNT_THRESHOLD;
static u16 __WPAD_dpd_diff_count_threshold = WPAD_DEFAULT_DPD_DIFF_COUNT_THRESHOLD;

static u16 __WPAD_acc_hyst_count_threshold = WPAD_DEFAULT_ACC_HYST_COUNT_THRESHOLD;
static u16 __WPAD_dpd_hyst_count_threshold = WPAD_DEFAULT_DPD_HYST_COUNT_THRESHOLD;

extern void DEBUGPrint(const char*, ...);

static void __ClearControlBlock(s32 chan);

void* noAlloc(u32 size) {
    DEBUGPrint("No Alloc: Nothing to do!!!\n");
    return NULL;
}

u8 noFree(void* ptr) {
    DEBUGPrint("No Free: Nothing to do!!!\n");
    return 0;
}

BOOL OnShutdown(BOOL final, u32 event) {
#pragma unused(event)

    s32 status;
    BOOL ret;

    ret = FALSE;
    status = WUDGetStatus();

    if (final == FALSE) {
        if (status == 3) {
            if (WUDIsBusy()) {
                WPADCancelSyncDevice();
                ret = FALSE;
            } else {
                switch (event) {
                case 0:
                    DEBUGPrint("Deregister allocators because of fatal error.\n");
                    WPADRegisterAllocator(noAlloc, noFree);

                case 2:
                case 3:
                    __WPADShutdown();
                    ret = FALSE;
                    break;
                case 5:
                    if (__OSIsReturnToIdle) {
                        __WPADShutdown();
                    } else {
                        __WPADReconnect(TRUE);
                    }
                    ret = FALSE;
                    break;
                case 1:
                case 4:
                    __WPADReconnect(TRUE);
                    ret = FALSE;
                    break;
                case 6:
                    __WPADReconnect(TRUE);
                    ret = FALSE;
                    break;
                default:
                    break;
                }
            }
        } else if (status == 5) {
            WPADStopSimpleSync();
            ret = FALSE;
        } else if ((status == 4) || (1 == status) || (2 == status)) {
            ret = FALSE;
        } else if (status == 0) {
            ret = TRUE;
        }
    } else {
        ret = TRUE;
    }

    return ret;
}

static void __SendData(s32 chan, WPADCommand cmd) {
    BOOL enable;
    BOOL motor;
    BT_HDR* p_buf = NULL;
    u8* ptr;
    s8 handle;
    s32 status;
    WPADControlBlock* p_wpd;
    u8 rep_id = (u8)cmd.command;
    u8* p_data = cmd.data;
    u16 len = cmd.len;

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    status = p_wpd->status;
    handle = p_wpd->dev_handle;
    if (handle < 0) {
        OSRestoreInterrupts(enable);
        return;
    }
    p_wpd->status = WPAD_ERR_BUSY;
    motor = p_wpd->motor & _rumble;
    OSRestoreInterrupts(enable);

    if (rep_id == WPAD_HIDREP_VIBRATOR) {
        enable = OSDisableInterrupts();
        p_wpd->status = status;
        OSRestoreInterrupts(enable);
    } else if (rep_id == WPAD_HIDREP_STRM) {
        enable = OSDisableInterrupts();
        p_wpd->status = status;
        p_wpd->audioFrames--;
        OSRestoreInterrupts(enable);
    } else {
        enable = OSDisableInterrupts();
        switch (rep_id) {
        case WPAD_HIDREP_WRDATA:
            break;

        case WPAD_HIDREP_RDDATA:
            p_wpd->readError = 0;
            p_wpd->readBaseAddr = cmd.readAddr;
            p_wpd->readLength = cmd.readLen;
            p_wpd->readBufPtr = cmd.readBuf;
            break;

        case WPAD_HIDREP_GETSTAT:
            p_wpd->status = status;
            p_wpd->infoBuf = cmd.info;
            p_wpd->getStatFlag = 1;
            break;

        case WPAD_HIDREP_WAIT: {
            OSTick tick;
            memcpy(&tick, cmd.data, sizeof(OSTick));
            p_wpd->cmdTimer = tick + __OSGetSystemTime();
            p_wpd->cmdTimeoutAction = 1;
        }
            return;
            break;

        default:
            p_data[0] |= 2;
            break;
        }

        p_wpd->resultCallback = cmd.callback;
        p_wpd->cmdId = rep_id;
        p_wpd->cmdTimer = OSSecondsToTicks(2) + __OSGetSystemTime();
        p_wpd->cmdTimeoutAction = 0;
        OSRestoreInterrupts(enable);
    }

    DEBUGPrint("handle = %d, repid = %02x\n", handle, rep_id);

    p_buf = GKI_getbuf((u8)(10 + len + sizeof(BT_HDR)));
    p_buf->len = (u8)(len + 1);
    p_buf->offset = 10;
    ptr = (u8*)(p_buf + 1) + p_buf->offset;

    ptr[0] = rep_id;

    memcpy(ptr + 1, p_data, len);

    if (motor) {
        ptr[1] |= 1;
    } else {
        ptr[1] &= ~1;
    }

    BTA_HhSendData((u8)handle, p_buf);
}

s32 WPADiGetStatus(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s32 status;

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    OSRestoreInterrupts(enable);

    return status;
}

s32 WPADiSendData(s32 chan, WPADCommand cmd) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    s32 status;
    BOOL enable;
    BOOL getStat;
    int t;

    status = WPADiGetStatus(chan);

    if (status == WPAD_ERR_NONE) {
        enable = OSDisableInterrupts();
        getStat = p_wpd->getStatFlag;
        OSRestoreInterrupts(enable);

        if (getStat) {
            status = WPAD_ERR_BUSY;
        } else {
            __SendData(chan, cmd);
        }
    } else if (status != WPAD_ERR_BUSY) {
        if (status == WPAD_ERR_TRANSFER) {
            enable = OSDisableInterrupts();
            p_wpd->status = WPAD_ERR_NONE;
            OSRestoreInterrupts(enable);
        }
    }

    if (status == WPAD_ERR_BUSY) {
        t = (int)OSTicksToSeconds(__OSGetSystemTime() - p_wpd->cmdTimer);
        if (t > 1 && _sleepTime > 0) {
            if (p_wpd->cmdTimeoutAction == 0) {
                p_wpd->cmdTimer = __OSGetSystemTime();
                WPADiDisconnect(chan, FALSE);
            } else {
                enable = OSDisableInterrupts();
                p_wpd->status = WPAD_ERR_TRANSFER;
                OSRestoreInterrupts(enable);
            }
        }
    }
    return status;
}

void WPADiRadioSensitivity(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u32 packet = 2;
    u16 rs;

    rs = (u16)(p_wpd->radioSense * (10 - 1));
    rs += (u16)((p_wpd->packetCnt * 100) / packet);
    rs /= 10;
    rs = (u16)((rs > 100) ? 100 : rs);

    p_wpd->radioSense = (u8)rs;
    p_wpd->packetCnt = 0;

    if (p_wpd->audioStop) {
        if (rs > 85) {
            p_wpd->audioStop = 0;
            p_wpd->audioStopCnt = 0;
        } else if (rs > 80) {
            p_wpd->audioStopCnt++;
            if (p_wpd->audioStopCnt >= 20) {
                p_wpd->audioStop = 0;
                p_wpd->audioStopCnt = 0;
            }
        }
    } else {
        if (rs < 75) {
            p_wpd->audioStop = 1;
            p_wpd->audioStopCnt = 0;
        } else if (rs < 80) {
            p_wpd->audioStopCnt++;
            if (p_wpd->audioStopCnt >= 1) {
                p_wpd->audioStop = 1;
                p_wpd->audioStopCnt = 0;
            }
        }
    }
}

BOOL WPADiIsCoreFormat(u32 fmt) {
    if (fmt == WPAD_FMT_CORE || fmt == WPAD_FMT_CORE_ACC || fmt == WPAD_FMT_CORE_ACC_DPD) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL WPADiIsFsFormat(u32 fmt) {
    if (fmt == WPAD_FMT_FREESTYLE || fmt == WPAD_FMT_FREESTYLE_ACC || fmt == WPAD_FMT_FREESTYLE_ACC_DPD) {
        return TRUE;
    } else {
        return FALSE;
    }
}
BOOL WPADiIsClFormat(u32 fmt) {
    if (fmt == WPAD_FMT_CLASSIC || fmt == WPAD_FMT_CLASSIC_ACC || fmt == WPAD_FMT_CLASSIC_ACC_DPD) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static u8 IsButtonChanged(u16 curr, u16 prev) {
    return (u8)((curr != prev) ? 1 : 0);
}

static u8 IsAnalogChanged(s32 curr, s32 prev, s32 threshold) {
    s32 diff = ((curr - prev) < 0) ? prev - curr : curr - prev;
    return (u8)((diff > threshold) ? 1 : 0);
}

BOOL IsControllerDataChanged(WPADControlBlock* p_wpd, void* p_curr, void* p_prev) {
    u32 fmt;
    u8 mode, result = 0;
    int i;

    mode = p_wpd->devMode;
    fmt = p_wpd->dataFormat;

    if (WPADiIsCoreFormat(fmt)) {
        WPADStatus* curr = (WPADStatus*)p_curr;
        WPADStatus* prev = (WPADStatus*)p_prev;
        result |= IsButtonChanged(curr->button, prev->button);
        if (curr->err == WPAD_ERR_NONE && prev->err == WPAD_ERR_NONE) {
            if (IsAnalogChanged((s32)(curr->accX), (s32)(prev->accX), 12) | IsAnalogChanged((s32)(curr->accY), (s32)(prev->accY), 12) |
                IsAnalogChanged((s32)(curr->accZ), (s32)(prev->accZ), 12)) {
                p_wpd->diffCountAcc++;
                if (p_wpd->diffCountAcc > __WPAD_acc_diff_count_threshold) {
                    p_wpd->diffCountAcc = 0;
                    p_wpd->hystCountAcc = 0;
                    result |= 1;
                }

            } else {
                p_wpd->hystCountAcc = (u16)((p_wpd->hystCountAcc + 1) % __WPAD_acc_hyst_count_threshold);
                if (p_wpd->hystCountAcc == __WPAD_acc_hyst_count_threshold - 1) {
                    if (p_wpd->diffCountAcc > 0) {
                        p_wpd->diffCountAcc--;
                    }
                }
            }

            for (i = 0; i < WPAD_DPD_MAX_OBJECTS; i++) {
                if (IsAnalogChanged((s32)(curr->obj[i].x), (s32)(prev->obj[i].x), 2) |
                    IsAnalogChanged((s32)(curr->obj[i].y), (s32)(prev->obj[i].y), 2)) {
                    p_wpd->diffCountDpd++;
                    if (p_wpd->diffCountDpd > __WPAD_dpd_diff_count_threshold) {
                        p_wpd->diffCountDpd = 0;
                        result |= 1;
                    }
                } else {
                    p_wpd->hystCountDpd = (u16)((p_wpd->hystCountDpd + 1) % __WPAD_dpd_hyst_count_threshold);
                    if (p_wpd->hystCountDpd == __WPAD_dpd_hyst_count_threshold - 1) {
                        if (p_wpd->diffCountDpd > 0) {
                            p_wpd->diffCountDpd--;
                        }
                    }
                }
            }
        }

    } else if (WPADiIsFsFormat(fmt)) {
        WPADFSStatus* curr = (WPADFSStatus*)p_curr;
        WPADFSStatus* prev = (WPADFSStatus*)p_prev;

        result |= IsButtonChanged(curr->button, prev->button);
        if (curr->err == WPAD_ERR_NONE && prev->err == WPAD_ERR_NONE) {
            if (IsAnalogChanged((s32)(curr->accX), (s32)(prev->accX), 12) | IsAnalogChanged((s32)(curr->accY), (s32)(prev->accY), 12) |
                IsAnalogChanged((s32)(curr->accZ), (s32)(prev->accZ), 12)) {
                p_wpd->diffCountAcc++;
                if (p_wpd->diffCountAcc > __WPAD_acc_diff_count_threshold) {
                    p_wpd->diffCountAcc = 0;
                    p_wpd->hystCountAcc = 0;
                    result |= 1;
                }

            } else {
                p_wpd->hystCountAcc = (u16)((p_wpd->hystCountAcc + 1) % __WPAD_acc_hyst_count_threshold);
                if (p_wpd->hystCountAcc == __WPAD_acc_hyst_count_threshold - 1) {
                    if (p_wpd->diffCountAcc > 0) {
                        p_wpd->diffCountAcc--;
                    }
                }
            }

            for (i = 0; i < WPAD_DPD_MAX_OBJECTS; i++) {
                if (IsAnalogChanged((s32)(curr->obj[i].x), (s32)(prev->obj[i].x), 2) |
                    IsAnalogChanged((s32)(curr->obj[i].y), (s32)(prev->obj[i].y), 2)) {
                    p_wpd->diffCountDpd++;
                    if (p_wpd->diffCountDpd > __WPAD_dpd_diff_count_threshold) {
                        p_wpd->diffCountDpd = 0;
                        result |= 1;
                    }
                } else {
                    p_wpd->hystCountDpd = (u16)((p_wpd->hystCountDpd + 1) % __WPAD_dpd_hyst_count_threshold);
                    if (p_wpd->hystCountDpd == __WPAD_dpd_hyst_count_threshold - 1) {
                        if (p_wpd->diffCountDpd > 0) {
                            p_wpd->diffCountDpd--;
                        }
                    }
                }
            }

            if (IsAnalogChanged((s32)(curr->fsAccX), (s32)(prev->fsAccX), 12) | IsAnalogChanged((s32)(curr->fsAccY), (s32)(prev->fsAccY), 12) |
                IsAnalogChanged((s32)(curr->fsAccZ), (s32)(prev->fsAccZ), 12)) {
                p_wpd->diffCountfsAcc++;
                if (p_wpd->diffCountfsAcc > __WPAD_acc_diff_count_threshold) {
                    p_wpd->diffCountfsAcc = 0;
                    p_wpd->hystCountfsAcc = 0;
                    result |= 1;
                }

            } else {
                p_wpd->hystCountfsAcc = (u16)((p_wpd->hystCountfsAcc + 1) % __WPAD_acc_hyst_count_threshold);
                if (p_wpd->hystCountfsAcc == __WPAD_acc_hyst_count_threshold - 1) {
                    if (p_wpd->diffCountfsAcc > 0) {
                        p_wpd->diffCountfsAcc--;
                    }
                }
            }

            result |= IsAnalogChanged((s32)(curr->fsStickX), (s32)(prev->fsStickX), 1);
            result |= IsAnalogChanged((s32)(curr->fsStickY), (s32)(prev->fsStickY), 1);
        }
    } else if (WPADiIsClFormat(fmt)) {
        WPADCLStatus* curr = (WPADCLStatus*)p_curr;
        WPADCLStatus* prev = (WPADCLStatus*)p_prev;
        s32 lstk;
        s32 rstk;
        s32 trig;

        switch (mode) {
        case 1:
            lstk = 16;
            rstk = 32;
            trig = 8;
            break;
        case 3:
            lstk = 4;
            rstk = 4;
            trig = 1;
            break;
        default:
            lstk = 1;
            rstk = 1;
            trig = 1;
            break;
        }

        result |= IsButtonChanged(curr->button, prev->button);
        if (curr->err == WPAD_ERR_NONE && prev->err == WPAD_ERR_NONE) {
            if (IsAnalogChanged((s32)(curr->accX), (s32)(prev->accX), 12) | IsAnalogChanged((s32)(curr->accY), (s32)(prev->accY), 12) |
                IsAnalogChanged((s32)(curr->accZ), (s32)(prev->accZ), 12)) {
                p_wpd->diffCountAcc++;
                if (p_wpd->diffCountAcc > __WPAD_acc_diff_count_threshold) {
                    p_wpd->diffCountAcc = 0;
                    p_wpd->hystCountAcc = 0;
                    result |= 1;
                }

            } else {
                p_wpd->hystCountAcc = (u16)((p_wpd->hystCountAcc + 1) % __WPAD_acc_hyst_count_threshold);
                if (p_wpd->hystCountAcc == __WPAD_acc_hyst_count_threshold - 1) {
                    if (p_wpd->diffCountAcc > 0) {
                        p_wpd->diffCountAcc--;
                    }
                }
            }

            for (i = 0; i < WPAD_DPD_MAX_OBJECTS; i++) {
                if (IsAnalogChanged((s32)(curr->obj[i].x), (s32)(prev->obj[i].x), 2) |
                    IsAnalogChanged((s32)(curr->obj[i].y), (s32)(prev->obj[i].y), 2)) {
                    p_wpd->diffCountDpd++;
                    if (p_wpd->diffCountDpd > __WPAD_dpd_diff_count_threshold) {
                        p_wpd->diffCountDpd = 0;
                        result |= 1;
                    }
                } else {
                    p_wpd->hystCountDpd = (u16)((p_wpd->hystCountDpd + 1) % __WPAD_dpd_hyst_count_threshold);
                    if (p_wpd->hystCountDpd == __WPAD_dpd_hyst_count_threshold - 1) {
                        if (p_wpd->diffCountDpd > 0) {
                            p_wpd->diffCountDpd--;
                        }
                    }
                }
            }
            result |= IsButtonChanged(curr->clButton, prev->clButton);
            result |= IsAnalogChanged((s32)(curr->clLStickX / lstk), (s32)(prev->clLStickX / lstk), 1);
            result |= IsAnalogChanged((s32)(curr->clLStickY / lstk), (s32)(prev->clLStickY / lstk), 1);
            result |= IsAnalogChanged((s32)(curr->clRStickX / rstk), (s32)(prev->clRStickX / rstk), 1);
            result |= IsAnalogChanged((s32)(curr->clRStickY / rstk), (s32)(prev->clRStickY / rstk), 1);
            result |= IsAnalogChanged((s32)(curr->clTriggerL / trig), (s32)(prev->clTriggerL / trig), 1);
            result |= IsAnalogChanged((s32)(curr->clTriggerR / trig), (s32)(prev->clTriggerR / trig), 1);
        }
    } else {
        WPADStatusEx* curr = (WPADStatusEx*)p_curr;
        WPADStatusEx* prev = (WPADStatusEx*)p_prev;

        result |= IsButtonChanged(curr->button, prev->button);
        if (curr->err == WPAD_ERR_NONE && prev->err == WPAD_ERR_NONE) {
            if (IsAnalogChanged((s32)(curr->accX), (s32)(prev->accX), 12) | IsAnalogChanged((s32)(curr->accY), (s32)(prev->accY), 12) |
                IsAnalogChanged((s32)(curr->accZ), (s32)(prev->accZ), 12)) {
                p_wpd->diffCountAcc++;
                if (p_wpd->diffCountAcc > __WPAD_acc_diff_count_threshold) {
                    p_wpd->diffCountAcc = 0;
                    p_wpd->hystCountAcc = 0;
                    result |= 1;
                }

            } else {
                p_wpd->hystCountAcc = (u16)((p_wpd->hystCountAcc + 1) % __WPAD_acc_hyst_count_threshold);
                if (p_wpd->hystCountAcc == __WPAD_acc_hyst_count_threshold - 1) {
                    if (p_wpd->diffCountAcc > 0) {
                        p_wpd->diffCountAcc--;
                    }
                }
            }

            for (i = 0; i < WPAD_DPD_MAX_OBJECTS; i++) {
                if (IsAnalogChanged((s32)(curr->obj[i].x), (s32)(prev->obj[i].x), 2) |
                    IsAnalogChanged((s32)(curr->obj[i].y), (s32)(prev->obj[i].y), 2)) {
                    p_wpd->diffCountDpd++;
                    if (p_wpd->diffCountDpd > __WPAD_dpd_diff_count_threshold) {
                        p_wpd->diffCountDpd = 0;
                        result |= 1;
                    }
                } else {
                    p_wpd->hystCountDpd = (u16)((p_wpd->hystCountDpd + 1) % __WPAD_dpd_hyst_count_threshold);
                    if (p_wpd->hystCountDpd == __WPAD_dpd_hyst_count_threshold - 1) {
                        if (p_wpd->diffCountDpd > 0) {
                            p_wpd->diffCountDpd--;
                        }
                    }
                }
            }
        }
    }

    return result;
}

void WPADRecalibrate(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable = OSDisableInterrupts();
    p_wpd->isSetStickOrigin = 0;
    OSRestoreInterrupts(enable);
}

void CheckButtonCombination(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    WPADCLStatus* cp;
    BOOL enable;
    u8 index;
    s8 cnt = -1;

    enable = OSDisableInterrupts();
    index = (u8)((p_wpd->rxBufIndex) ? 0 : 1);
    cp = (WPADCLStatus*)(p_wpd->rxBuf + index * WPAD_RX_DATASIZE);

    if (WPADiIsFsFormat(p_wpd->dataFormat)) {
        if (cp->button == 0x1C10) {
            cnt = 1;
        }
    }
    if (WPADiIsClFormat(p_wpd->dataFormat)) {
        if (cp->button == 0x1C10) {
            cnt = 1;
        }
        if (cp->err != WPAD_ERR_NONE) {
            cnt = 0;
        } else if (cp->clButton == 0x1450) {
            cnt = 1;
        }
    }
    if (cnt < 0) {
        p_wpd->recalibrateCount = 0;
    } else {
        p_wpd->recalibrateCount += cnt;
    }
    OSRestoreInterrupts(enable);

    if (p_wpd->recalibrateCount > 600) {
        WPADRecalibrate(chan);
    }
}

static void __SetScreenSaverFlag(BOOL flag) {
    if (flag) {
        __VIResetRFIdle();
    }
}

void WPADiCheckContInputs(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL diff = FALSE;
    BOOL enable;
    BOOL screen = FALSE;
    void* p_data;
    u8 index;

    if (p_wpd->status == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    enable = OSDisableInterrupts();
    index = (u8)((p_wpd->rxBufIndex) ? 0 : 1);
    p_data = (void*)(p_wpd->rxBuf + index * WPAD_RX_DATASIZE);
    diff = IsControllerDataChanged(p_wpd, p_data, p_wpd->prBuf);

    if (diff) {
        memcpy(p_wpd->prBuf, p_data, WPAD_RX_DATASIZE);
    }
    if (((WPADStatus*)p_wpd->prBuf)->err != WPAD_ERR_NONE) {
        memset(p_wpd->prBuf, 0, WPAD_RX_DATASIZE);
    }

    OSRestoreInterrupts(enable);
    CheckButtonCombination(chan);

    if (diff) {
        screen = TRUE;
        p_wpd->lastUpdateTime = __OSGetSystemTime();
    } else {
        if (_sleepTime > 0) {
            int sec = (int)OSTicksToSeconds(__OSGetSystemTime() - p_wpd->lastUpdateTime);

            if (sec > 60 * _sleepTime) {
                WPADiDisconnect(chan, TRUE);
            }
        }
    }

    __SetScreenSaverFlag(screen);
}

s8 __GetCmdNumber(WPADCmdQueue* queue) {
    BOOL enable;
    s8 num;

    enable = OSDisableInterrupts();

    num = (s8)(queue->tail - queue->head);
    if (num < 0) {
        num += queue->cmdlen;
    }

    OSRestoreInterrupts(enable);
    return num;
}

BOOL __CanPushCmdQueue(WPADCmdQueue* queue, s8 cmd_num) {
    s8 curr = __GetCmdNumber(queue);

    if (curr + cmd_num <= queue->cmdlen - 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void WPADiClearQueue(WPADCmdQueue* queue) {
    BOOL enable;

    enable = OSDisableInterrupts();

    queue->head = 0;
    queue->tail = 0;
    memset(queue->cmd, 0, queue->cmdlen * sizeof(WPADCommand));

    OSRestoreInterrupts(enable);
}

BOOL WPADiPushCommand(WPADCmdQueue* queue, WPADCommand cmd) {
    BOOL enable;

    enable = OSDisableInterrupts();

    if (__GetCmdNumber(queue) == queue->cmdlen - 1) {
        OSRestoreInterrupts(enable);
        return FALSE;
    }

    memset(queue->cmd + queue->tail, 0, sizeof(WPADCommand));
    memcpy(queue->cmd + queue->tail, &cmd, sizeof(WPADCommand));
    queue->tail = (s8)((queue->tail == queue->cmdlen - 1) ? 0 : queue->tail + 1);
    OSRestoreInterrupts(enable);
    return TRUE;
}

BOOL WPADiSendDPDCSB(WPADCmdQueue* queue, BOOL enable, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_DPDCSB;
    cmd.len = 1;
    cmd.data[0] = (u8)((enable) ? (u8)4 : (u8)0);
    cmd.callback = callback;

    result = WPADiPushCommand(queue, cmd);
    return result;
}

BOOL WPADiSendReadData(WPADCmdQueue* queue, void* p_buf, u16 len, u32 addr, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;
    ASSERT(p_buf != NULL);

    cmd.command = WPAD_HIDREP_RDDATA;
    cmd.len = 6;
    cmd.callback = callback;

    memcpy(cmd.data, &addr, sizeof(addr));
    memcpy(cmd.data + 4, &len, sizeof(len));

    cmd.readBuf = p_buf;
    cmd.readLen = len;
    cmd.readAddr = addr;

    result = WPADiPushCommand(queue, cmd);
    return result;
}

static void WPADiRumbleMotor(s32 chan, BOOL sendCmd) {
    WPADCommand cmd;

    if (_wpdcb[chan]->status == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    if (sendCmd == TRUE || __GetCmdNumber(&_wpdcb[chan]->cmdq) > 0) {
        _wpdcb[chan]->rumble = FALSE;
    } else if (_rumbleCnt[chan] == 5) {
        _wpdcb[chan]->rumble = FALSE;

        cmd.command = WPAD_HIDREP_VIBRATOR;
        cmd.len = 1;
        cmd.data[0] = 0;
        cmd.callback = NULL;

        __SendData(chan, cmd);
    }
    _rumbleCnt[chan] = (u8)((_wpdcb[chan]->rumble) ? _rumbleCnt[chan] + 1 : 0);
}

static u8 __GetDpdSensitivity() {
    u8 level;

    level = (u8)SCGetBtDpdSensibility();
    if (level < 1) {
        level = 1;
    }
    if (level > 5) {
        level = 5;
    }

    return level;
}

static u8 __GetSensorBarPosition() {
    u8 pos;

    if (1 == SCGetWpadSensorBarPosition()) {
        pos = WPAD_SENSOR_BAR_POS_TOP;
    } else {
        pos = WPAD_SENSOR_BAR_POS_BOTTOM;
    }
    return pos;
}

static BOOL __GetMotorMode() {
    BOOL rumble;

    if (1 == SCGetWpadMotorMode()) {
        rumble = TRUE;
    } else {
        rumble = FALSE;
    }
    return rumble;
}

static u8 __ClampSpeakerVolume(u8 vol) {
    u8 v = vol;

    if (vol <= 0) {
        v = 0;
    }
    if (vol >= 127) {
        v = 127;
    }

    return v;
}

u16 _WPADGetStackBufferStatus(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s32 status;
    s8 handle;

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    handle = p_wpd->dev_handle;
    OSRestoreInterrupts(enable);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        return 0;
    } else {
        return _WUDGetQueuedSize(handle);
    }
}

u16 _WPADGetModuleBufferStatus(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s32 status;
    s8 handle;

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    handle = p_wpd->dev_handle;
    OSRestoreInterrupts(enable);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        return 0;
    } else {
        return _WUDGetNotAckedSize(handle);
    }
}

static u8 __GetSpeakerVolume() {
    u8 vol;
    vol = SCGetWpadSpeakerVolume();
    vol = __ClampSpeakerVolume(vol);
    return vol;
}

static void WPADiGetScSettings() {
    if (_scSetting) {
        if (SCCheckStatus() == 0) {
            _dpdSensitivity = __GetDpdSensitivity();
            _sensorBarPos = __GetSensorBarPosition();
            _rumble = __GetMotorMode();
            _speakerVolume = __GetSpeakerVolume();
            _scSetting = 0;
        }
    }
}

static void WPADiAfh() {
    BOOL enable;
    u8* channel = (u8*)OSPhysicalToCached(0x31A2);

    DCInvalidateRange(channel, 1);
    DEBUGPrint("Check the update of WiFi using channel\n");

    if (_afhChannel != *channel) {
        enable = OSDisableInterrupts();
        _afhChannel = (s8)*channel;
        OSRestoreInterrupts(enable);
        DEBUGPrint("WiFi uses channel = %d\n", *channel);
        WUDSetDisableChannel(_afhChannel);
    }
}

static void WPADiContMapTableUpdate() {
    if (_scFlush) {
        if (SCCheckStatus() == 0) {
            if (TRUE == SCSetBtDeviceInfoArray(&_scArray)) {
                SCFlushAsync(NULL);
                _scFlush = 0;
            }
        }
    }
}

static void __reconnect(BOOL exec) {
    DEBUGPrint("Reconnect Start!!\n");
    BTA_DmSendHciReset();
    WPADiShutdown(exec);
}

static BOOL WPADiProcessExtCommand(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    WPADCommand cmd;
    s32 err;

    if (p_wpd->used) {
        if (WPADiGetCommand(&p_wpd->extq, &cmd)) {
            if (cmd.command == WPAD_HIDREP_REPTYPE || p_wpd->infos.attach) {
                err = WPADiSendData(chan, cmd);
                if (err == WPAD_ERR_NONE) {
                    WPADiPopCommand(&p_wpd->extq);
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

BOOL WPADiSendGetContStat(WPADCmdQueue* queue, WPADInfo* info, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_GETSTAT;
    cmd.len = 1;
    cmd.data[0] = 0;
    cmd.callback = callback;
    cmd.info = info;

    result = WPADiPushCommand(queue, cmd);
    return result;
}

BOOL WPADiSendWriteDataCmd(WPADCmdQueue* queue, u8 cmd, u32 addr, WPADCallback callback) {
    return WPADiSendWriteData(queue, &cmd, 1, addr, callback);
}

BOOL WPADiSendWriteData(WPADCmdQueue* queue, void* p_buf, u16 len, u32 addr, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;
    u8 length = (u8)(len & WPAD_WRITE_LEN_MASK);
    cmd.command = WPAD_HIDREP_WRDATA;
    cmd.len = 21;
    cmd.callback = callback;
    memcpy(cmd.data, &addr, sizeof(addr));
    memcpy(cmd.data + 4, &length, sizeof(length));
    memcpy(cmd.data + 5, p_buf, len);

    result = WPADiPushCommand(queue, cmd);
    return result;
}

BOOL WPADiSendSetPort(WPADCmdQueue* queue, u8 pattern, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_PORT;
    cmd.len = 1;
    cmd.data[0] = (u8)(pattern << 4);
    cmd.callback = callback;

    result = WPADiPushCommand(queue, cmd);
    return result;
}

static void setupCallback(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];

    if (result == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    if (result == WPAD_ERR_NONE) {
        p_wpd->setup = TRUE;

        if (p_wpd->connectCallback) {
            p_wpd->connectCallback(chan, result);
        }
    } else {
        WPADiDisconnect(chan, FALSE);
    }
}

static void abortConnCallback(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];

    if (result != WPAD_ERR_NONE) {
        WPADiClearQueue(&p_wpd->cmdq);

        if (result != WPAD_ERR_NO_CONTROLLER) {
            WPADiDisconnect(chan, FALSE);
        }
    }
}

static void firmwareCheckCallback(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u16 size;
    u32 addr;
    u8 port;
    BOOL enable;

    if (result == WPAD_ERR_NO_CONTROLLER) {
        return;
    }

    enable = OSDisableInterrupts();
    p_wpd->oldFw = (result == WPAD_ERR_NONE) ? TRUE : FALSE;
    p_wpd->status = WPAD_ERR_NONE;
    OSRestoreInterrupts(enable);
    size = (u16)((result == WPAD_ERR_NONE) ? 20 : 42);
    addr = (u32)((result == WPAD_ERR_NONE) ? 0x176c : 0);
    port = (u8)(0x01 << chan);

    DEBUGPrint(" ==>this error means that the firmware is for NDEV %s\n", (p_wpd->oldFw) ? "2.0" : "2.1 or later");
    WPADiSendSetReportType(&p_wpd->cmdq, WPAD_FMT_CORE, p_wpd->pwrSave, abortConnCallback);
    WPADiSendDPDCSB(&p_wpd->cmdq, FALSE, abortConnCallback);
    WPADiSendSetPort(&p_wpd->cmdq, port, abortConnCallback);
    WPADiSendReadData(&p_wpd->cmdq, p_wpd->readBuf, sizeof(WPADMEMGameInfo), 0x2A, abortConnCallback);
    WPADiSendReadData(&p_wpd->cmdq, p_wpd->readBuf, sizeof(WPADMEMGameInfo), 0x2A + sizeof(WPADMEMGameInfo), abortConnCallback);
    WPADiSendReadData(&p_wpd->cmdq, p_wpd->readBuf, size, addr, setupCallback);
    WPADiSendGetContStat(&p_wpd->cmdq, NULL, NULL);
}

s32 WPADiRetrieveChannel(u8 dev_handle) {
    u8* devAddr;
    s32 i;

    devAddr = _WUDGetDevAddr(dev_handle);

    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (!memcmp(_scArray.info[i + 10].bd_addr, devAddr, 6)) {
            if (_chan_active_state[i] == 0) {
                _chan_active_state[i] = 1;
                return i;
            }
        }
    }
    for (i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        if (_chan_active_state[i] == 0) {
            _chan_active_state[i] = 1;
            memcpy(_scArray.info[i + 10].bd_addr, devAddr, 6);
            _scFlush = 1;
            return i;
        }
    }

    return -1;
}

static void WPADiRecvCallback(u8 dev_handle, u8* p_rpt, u16) {
    u8 chan;
    s32 err;

    chan = (u8)_dev_handle_index[dev_handle];

    if ((chan >= 0) && (chan <= WPAD_MAX_CONTROLLERS)) {
        err = WPADiHIDParser(chan, p_rpt);
        if (err) {
            DEBUGPrint("HID Parser reports: %d\n", err);
        }
    } else {
        DEBUGPrint("WPADiRecvCallback(): Unknown channel %d\n", chan);
    }
}

void WPADGetAccGravityUnit(s32 chan, u32 type, WPADAcc* acc) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;

    enable = OSDisableInterrupts();
    if (acc) {
        switch (type) {
        case WPAD_DEV_CORE:
            acc->x = (s16)(p_wpd->conf.acc_1g.x - p_wpd->conf.acc_0g.x);
            acc->y = (s16)(p_wpd->conf.acc_1g.y - p_wpd->conf.acc_0g.y);
            acc->z = (s16)(p_wpd->conf.acc_1g.z - p_wpd->conf.acc_0g.z);
            break;

        case WPAD_DEV_FREESTYLE:
            acc->x = (s16)(p_wpd->extConf.fs.acc_1g.x - p_wpd->extConf.fs.acc_0g.x);
            acc->y = (s16)(p_wpd->extConf.fs.acc_1g.y - p_wpd->extConf.fs.acc_0g.y);
            acc->z = (s16)(p_wpd->extConf.fs.acc_1g.z - p_wpd->extConf.fs.acc_0g.z);
            break;
        }
    }
    OSRestoreInterrupts(enable);
}

BOOL WPADiGetCommand(WPADCmdQueue* queue, WPADCommand* cmd) {
    BOOL enable = OSDisableInterrupts();

    if (__GetCmdNumber(queue) == 0) {
        OSRestoreInterrupts(enable);
        return FALSE;
    }

    memcpy(cmd, queue->cmd + queue->head, sizeof(WPADCommand));

    OSRestoreInterrupts(enable);
    return TRUE;
}

BOOL WPADiPopCommand(WPADCmdQueue* queue) {
    BOOL enable = OSDisableInterrupts();

    if (__GetCmdNumber(queue) == 0) {
        OSRestoreInterrupts(enable);
        return FALSE;
    }

    memset(queue->cmd + queue->head, 0, sizeof(WPADCommand));
    queue->head = (s8)((queue->head == queue->cmdlen - 1) ? 0 : queue->head + 1);

    OSRestoreInterrupts(enable);
    return TRUE;
}

static BOOL WPADiProcessCommand(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    WPADCommand cmd;
    s32 err;

    if (p_wpd->used) {
        if (WPADiGetCommand(&p_wpd->cmdq, &cmd)) {
            err = WPADiSendData(chan, cmd);
            if (err == WPAD_ERR_NONE) {
                WPADiPopCommand(&p_wpd->cmdq);
                return TRUE;
            }
        }
    }
    return FALSE;
}

static void WPADiConnCallback(WUDDeviceInfo* info, u8 open) {
    s32 chan = -1;
    WPADControlBlock* p_wpd;
    BOOL isCmdExist;
    WPADCommand cmd;
    u8 dev_handle = info->handle;

    if (open) {
        DEBUGPrint("connection is opened\n");
        chan = WPADiRetrieveChannel(dev_handle);
        p_wpd = _wpdcb[chan];
        _dev_handle_index[dev_handle] = (s8)(chan & 0xff);
        __ClearControlBlock(chan);

        if (!memcmp(info->bd_name, "Nintendo RVL-CNT", 16)) {
            p_wpd->devType = WPAD_DEV_CORE;
        } else {
            p_wpd->devType = WPAD_DEV_FUTURE;
        }
        p_wpd->dev_handle = (s8)dev_handle;
        p_wpd->dataFormat = WPAD_FMT_CORE;
        p_wpd->used = TRUE;
        p_wpd->status = WPAD_ERR_NONE;
        p_wpd->radioSense = 100;
        p_wpd->disconnect = 0;
        p_wpd->extEnc = 0;
        WPADiSendReadData(&p_wpd->cmdq, p_wpd->readBuf, 1, 0x1770, firmwareCheckCallback);
        __SetScreenSaverFlag(TRUE);
    } else {
        DEBUGPrint("connection is closed\n");
        chan = _dev_handle_index[dev_handle];
        _dev_handle_index[dev_handle] = -1;

        if (chan != -1) {
            p_wpd = _wpdcb[chan];
            p_wpd->status = WPAD_ERR_NO_CONTROLLER;

            if (p_wpd->resultCallback) {
                p_wpd->resultCallback(chan, WPAD_ERR_NO_CONTROLLER);
            } else if (_wmb[chan].callback) {
                _wmb[chan].callback(chan, WPAD_ERR_NO_CONTROLLER);
            }
            do {
                isCmdExist = WPADiGetCommand(&p_wpd->cmdq, &cmd);
                if (isCmdExist) {
                    if (cmd.callback) {
                        cmd.callback(chan, WPAD_ERR_NO_CONTROLLER);
                    }
                    WPADiPopCommand(&p_wpd->cmdq);
                }
            } while (isCmdExist);
            DEBUGPrint("clean up command queue\n");

            if (p_wpd->samplingBufs_ptr) {
                WPADSetAutoSamplingBuf(chan, p_wpd->samplingBufs_ptr, p_wpd->bufLength);
            }

            __ClearControlBlock(chan);
            _chan_active_state[chan] = 0;

            if (p_wpd->connectCallback) {
                p_wpd->connectCallback(chan, WPAD_ERR_NO_CONTROLLER);
            }
        } else {
            DEBUGPrint("WARNING: disconnection for device handle not assigned to channel.\n");
        }
    }
}

void WPADiManageHandler(OSAlarm*, OSContext*) {
    s32 chan;
    s32 status;
    BOOL sendCmd;

    status = WPADGetStatus();

    if (status != WPAD_STATE_SETUP) {
        if (status == WPAD_STATE_ENABLED) {
            if (!_initialized) {
                _initialized = TRUE;
                WUDSetHidConnCallback(WPADiConnCallback);
                WUDSetHidRecvCallback(WPADiRecvCallback);

                _recCnt = 50;
            }
        }
        return;
    } else {
        if (_recFlag >= 0) {
            _recCnt--;
            if (_recCnt <= 0) {
                __reconnect((BOOL)_recFlag);
            }
            return;
        }
    }

    for (chan = 0; chan < WPAD_MAX_CONTROLLERS; chan++) {
        sendCmd = FALSE;
        if (_extCnt[chan] == 5) {
            sendCmd |= WPADiProcessExtCommand(chan);
            if (sendCmd) {
                _extCnt[chan] = 0;
            }
        }
        if (!sendCmd) {
            sendCmd |= WPADiProcessCommand(chan);
        }

        WPADiRumbleMotor(chan, sendCmd);

        if (_checkCnt == 5) {
            WPADiCheckContInputs(chan);
        }

        if (_senseCnt == 10) {
            WPADiRadioSensitivity(chan);
        }

        _extCnt[chan] = (u8)((_extCnt[chan] == 5) ? _extCnt[chan] : _extCnt[chan] + 1);
    }

    if (_afhCnt == 60000) {
        WPADiAfh();
    }

    _senseCnt = (u16)((_senseCnt == 10) ? 0 : _senseCnt + 1);
    _checkCnt = (u8)((_checkCnt == 5) ? 0 : _checkCnt + 1);
    _afhCnt = (u16)((_afhCnt == 60000) ? 0 : _afhCnt + 1);

    WPADiContMapTableUpdate();
    WPADiGetScSettings();
    BTA_HhGetAclQueueInfo();
}

u8 __WPADiManageHandlerStack[4096] ATTRIBUTE_ALIGN(32);

void WPADiManageHandler0(OSAlarm* alarm, OSContext* context) {
    OSSwitchFiberEx((u32)alarm, (u32)context, 0, 0, (u32)WPADiManageHandler, (u32)(__WPADiManageHandlerStack + sizeof(__WPADiManageHandlerStack)));
}

static void __ClearControlBlock(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];

    p_wpd->rxBufIndex = 0;

    p_wpd->infoBuf = NULL;
    p_wpd->motor = FALSE;

    p_wpd->resultCallback = NULL;
    p_wpd->extensionCallback = NULL;
    p_wpd->sampleCallback = NULL;
    p_wpd->samplingBufs_ptr = NULL;
    p_wpd->bufLatestIndex = 0;
    p_wpd->bufLength = 0;
    p_wpd->dataFormat = WPAD_FMT_CORE;
    p_wpd->status = WPAD_ERR_NO_CONTROLLER;
    p_wpd->devType = WPAD_DEV_NOT_FOUND;
    p_wpd->devMode = 0;
    p_wpd->isSetStickOrigin = 0;
    p_wpd->recalibrateCount = 0;
    p_wpd->getStatFlag = 0;
    p_wpd->dpdDummyObjSize = 12;
    p_wpd->dpdFormat = 0;
    p_wpd->dpdCommand = 0;

    p_wpd->diffCountDpd = 0;
    p_wpd->hystCountDpd = 0;
    p_wpd->diffCountAcc = 0;
    p_wpd->hystCountAcc = 0;
    p_wpd->diffCountfsAcc = 0;
    p_wpd->hystCountfsAcc = 0;

    p_wpd->lastUpdateTime = __OSGetSystemTime();
    p_wpd->cmdTimer = __OSGetSystemTime();
    p_wpd->cmdTimeoutAction = 0;
    p_wpd->reqVolume = 0;
    p_wpd->reqVolCb = NULL;

    p_wpd->readBufPtr = NULL;
    p_wpd->readBaseAddr = 0;
    p_wpd->readLength = 0;
    p_wpd->readError = 0;

    p_wpd->dev_handle = -1;
    p_wpd->used = FALSE;
    p_wpd->setup = FALSE;
    p_wpd->oldFw = FALSE;
    p_wpd->audioStop = 1;
    p_wpd->audioStopCnt = 0;
    p_wpd->audioFrames = 0;

    p_wpd->keyIdx = 0;
    p_wpd->radioSense = 0;
    p_wpd->packetCnt = 0;
    p_wpd->disconnect = 1;

    p_wpd->infoLocked = 0;
    p_wpd->infoCallback = NULL;

    p_wpd->pwrSave = 0;

    memset(&p_wpd->infos, 0, sizeof(WPADInfo));
    memset(p_wpd->readBuf, 0, sizeof(p_wpd->readBuf));
    memset(p_wpd->rxBuf, 0, WPAD_RX_DATASIZE * 2);
    memset(p_wpd->prBuf, 0, WPAD_RX_DATASIZE);
    memset(&p_wpd->conf, 0, sizeof(WPADConfig));
    memset(&p_wpd->extConf, 0, sizeof(WPADExtConfig));
    memset(&p_wpd->key, 0, sizeof(p_wpd->key));
    memset(&p_wpd->ft, 0, sizeof(p_wpd->ft));
    memset(&p_wpd->sb, 0, sizeof(p_wpd->sb));
    memset(&p_wpd->gameInfo, 0, sizeof(WPADMEMGameInfo));

    p_wpd->gameInfoErr[0] = WPAD_ERR_NO_CONTROLLER;
    p_wpd->gameInfoErr[1] = WPAD_ERR_NO_CONTROLLER;
    p_wpd->cmdq.cmd = p_wpd->cmds;
    p_wpd->cmdq.cmdlen = (u32)(sizeof(p_wpd->cmds) / sizeof(WPADCommand));
    p_wpd->extq.cmd = p_wpd->exts;
    p_wpd->extq.cmdlen = (u32)(sizeof(p_wpd->exts) / sizeof(WPADCommand));
    WPADiClearQueue(&p_wpd->cmdq);
    WPADiClearQueue(&p_wpd->extq);

    _wmb[chan].used = FALSE;
    _wmb[chan].p_buf = NULL;
    _wmb[chan].len = 0;
    _wmb[chan].addr = 0;
    _wmb[chan].callback = NULL;

    _extCnt[chan] = 0;
    _rumbleCnt[chan] = 0;
}

BOOL __SetSensorBarPower(BOOL flag) {
    u32 reg;
    u32 regBak;
    BOOL enable;
    BOOL result;

    enable = OSDisableInterrupts();

    regBak = ACRReadReg(0xC0);

    if (flag) {
        reg = regBak | 0x100;
    } else {
        reg = regBak & ~0x100;
    }

    ACRWriteReg(0xC, reg);
    result = (regBak & 0x100) ? TRUE : FALSE;
    OSRestoreInterrupts(enable);
    return result;
}

static const char* __WPADVersion = "<< RVL_SDK - WPAD \trelease build: Dec 11 2007 01:35:07 (0x4199_60831) >>";

void WPADiInitSub() {
    s32 chan;
    int i;

    __SetSensorBarPower(TRUE);

    for (i = 0; i < 16; i++) {
        _dev_handle_index[i] = -1;
    }

    DEBUGPrint("WPADInit()\n");

    for (chan = 0; chan < WPAD_MAX_CONTROLLERS; ++chan) {
        _wpdcb[chan] = &_wpd[chan];
        _chan_active_state[chan] = 0;
        _wpdcb[chan]->connectCallback = NULL;
        __ClearControlBlock(chan);
        OSInitThreadQueue(&_wpd[chan].threadq);

        _extCnt[chan] = 0;
        _rumbleCnt[chan] = 0;
    }

    _sleepTime = 5;
    _gamecode = OSGetAppGamename();
    _gametype = OSGetAppType();
    _dpdSensitivity = __GetDpdSensitivity();
    _sensorBarPos = __GetSensorBarPosition();
    _rumble = __GetMotorMode();
    _speakerVolume = __GetSpeakerVolume();
    _senseCnt = 0;
    _checkCnt = 0;
    _afhCnt = 0;
    _shutdown = 0;
    _scFlush = 0;
    _scSetting = 1;
    _afhChannel = -1;

    OSRegisterVersion(__WPADVersion);
    OSCreateAlarm(&_managerAlarm);
    OSSetPeriodicAlarm(&_managerAlarm, OSGetTime(), OSMillisecondsToTicks(1), WPADiManageHandler0);
}

static OSShutdownFunctionInfo ShutdownFunctionInfo = {OnShutdown, 127};

void WPADInit(void) {
    BOOL result;

    _startup = TRUE;

    if (_regShutdown == 0) {
        OSRegisterShutdownFunction(&ShutdownFunctionInfo);
        _regShutdown = 1;
    }

    result = WUDInit();

    if (result) {
        _initialized = FALSE;
        _recFlag = -1;
        _recCnt = 50;
        WPADiInitSub();
    }
}

BOOL WPADStartFastSimpleSync() {
    return WUDStartFastSyncSimple();
}

BOOL WPADStopSimpleSync() {
    return WUDStopSyncSimple();
}

WPADSyncDeviceCallback WPADSetSimpleSyncCallback(WPADSyncDeviceCallback callback) {
    return WUDSetSyncSimpleCallback(callback);
}

void WPADRegisterAllocator(WPADAlloc alloc, WPADFree free) {
    WUDRegisterAllocator(alloc, free);
}

u32 WPADGetWorkMemorySize(void) {
    return WUDGetAllocatedMemSize();
}

s32 WPADGetStatus() {
    return WUDGetStatus();
}

u8 WPADGetSensorBarPosition() {
    BOOL enable;
    u8 pos;

    enable = OSDisableInterrupts();
    pos = _sensorBarPos;
    OSRestoreInterrupts(enable);
    return pos;
}

static void CloseCallback(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];

    if (result != WPAD_ERR_NO_CONTROLLER) {
        BTA_HhClose((u8)p_wpd->dev_handle);
    }
}

void WPADDisconnect(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    memset(&_scArray.info[chan + 10], 0, sizeof(SCBtDeviceInfoSingle));
    _scFlush = 1;
    WPADiDisconnect(chan, TRUE);
}

void WPADSetAutoSleepTime(u8 minute) {
    BOOL enable = OSDisableInterrupts();
    _sleepTime = minute;
    OSRestoreInterrupts(enable);
}

s32 WPADProbe(s32 chan, u32* type) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    s32 status;
    BOOL enable = OSDisableInterrupts();
    if (type != NULL) {
        *type = p_wpd->devType;
    }
    status = p_wpd->status;
    if (status != WPAD_ERR_NO_CONTROLLER) {
        if (p_wpd->devType == WPAD_DEV_NOT_FOUND) {
            status = WPAD_ERR_NO_CONTROLLER;
        } else if (!p_wpd->setup) {
            status = WPAD_ERR_BUSY;
        }
    }
    OSRestoreInterrupts(enable);
    return status;
}

WPADSamplingCallback WPADSetSamplingCallback(s32 chan, WPADSamplingCallback callback) {
    WPADControlBlock* p_wpd;
    WPADSamplingCallback prev;
    BOOL enable;

    DEBUGPrint("WPADSetSamplingCallback()\n");

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    prev = p_wpd->sampleCallback;
    p_wpd->sampleCallback = callback;
    OSRestoreInterrupts(enable);
    return prev;
}

WPADConnectCallback WPADSetConnectCallback(s32 chan, WPADConnectCallback callback) {
    WPADControlBlock* p_wpd;
    WPADConnectCallback prev;
    BOOL enable;

    DEBUGPrint("WPADSetConnectCallback()\n");

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    prev = p_wpd->connectCallback;
    p_wpd->connectCallback = callback;
    OSRestoreInterrupts(enable);
    return prev;
}

WPADExtensionCallback WPADSetExtensionCallback(s32 chan, WPADExtensionCallback callback) {
    WPADControlBlock* p_wpd;
    WPADExtensionCallback prev;
    BOOL enable;

    DEBUGPrint("WPADSetExtensionCallback()\n");

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    prev = p_wpd->extensionCallback;
    p_wpd->extensionCallback = callback;
    OSRestoreInterrupts(enable);

    return prev;
}

u32 WPADGetDataFormat(s32 chan) {
    WPADControlBlock* p_wpd;
    u32 fmt;
    BOOL enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    fmt = p_wpd->dataFormat;
    OSRestoreInterrupts(enable);
    return fmt;
}

s32 WPADSetDataFormat(s32 chan, u32 fmt) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    s32 ret;
    BOOL busy;
    BOOL enable;
    BOOL setup;
    u32 prevFmt;

    enable = OSDisableInterrupts();
    setup = p_wpd->setup;
    ret = p_wpd->status;
    prevFmt = p_wpd->dataFormat;
    OSRestoreInterrupts(enable);

    if (ret != WPAD_ERR_NO_CONTROLLER) {
        if (!setup) {
            ret = WPAD_ERR_BUSY;
        } else if (prevFmt != fmt) {
            busy = WPADiSendSetReportType(&p_wpd->cmdq, fmt, p_wpd->pwrSave, NULL);
            if (!busy) {
                ret = WPAD_ERR_BUSY;
            } else {
                enable = OSDisableInterrupts();
                p_wpd->dataFormat = fmt;
                OSRestoreInterrupts(enable);
                ret = WPAD_ERR_NONE;
            }
        } else {
            ret = WPAD_ERR_NONE;
        }
    }
    return ret;
}

void __infoCallback(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];

    if (p_wpd->infoCallback) {
        p_wpd->infoCallback(chan, result);
    }
    p_wpd->infoCallback = NULL;
    p_wpd->infoLocked = 0;
}

void WPADiDisconnect(s32 chan, BOOL polite) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s32 status;

    status = WPADiGetStatus(chan);

    if (status != WPAD_ERR_NO_CONTROLLER) {
        if (polite) {
            enable = OSDisableInterrupts();
            if (p_wpd->disconnect) {
                OSRestoreInterrupts(enable);
                return;
            }
            p_wpd->disconnect = 1;
            OSRestoreInterrupts(enable);
            WPADControlLed(chan, 0, CloseCallback);
        } else {
            u8 addr[6];
            WPADGetAddress(chan, addr);
            btm_remove_acl(addr);
        }
    }
}

s32 WPADGetInfoAsync(s32 chan, WPADInfo* info, WPADCallback callback) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL busy;
    BOOL enable;
    BOOL setup;
    s32 result;
    u8 lock;

    enable = OSDisableInterrupts();
    setup = p_wpd->setup;
    result = p_wpd->status;
    lock = p_wpd->infoLocked;
    OSRestoreInterrupts(enable);

    if (result != WPAD_ERR_NO_CONTROLLER) {
        if (!setup || lock) {
            result = WPAD_ERR_BUSY;
        } else {
            enable = OSDisableInterrupts();
            p_wpd->infoLocked = 1;
            p_wpd->infoCallback = callback;
            OSRestoreInterrupts(enable);
            busy = WPADiSendGetContStat(&p_wpd->cmdq, info, __infoCallback);

            if (busy) {
                result = WPAD_ERR_NONE;
            } else {
                result = WPAD_ERR_BUSY;
                enable = OSDisableInterrupts();
                p_wpd->infoLocked = 0;
                p_wpd->infoCallback = NULL;
                OSRestoreInterrupts(enable);
            }
        }
    }

    if (result != WPAD_ERR_NONE) {
        if (callback) {
            callback(chan, result);
        }
    }

    return result;
}

void WPADControlMotor(s32 chan, u32 command) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    BOOL needed = FALSE;
    s32 status;

    enable = OSDisableInterrupts();
    status = p_wpd->status;

    if (status == WPAD_ERR_NO_CONTROLLER) {
        OSRestoreInterrupts(enable);
        return;
    }

    if (_rumble == FALSE) {
        if (command != WPAD_MOTOR_STOP || p_wpd->motor != TRUE) {
            OSRestoreInterrupts(enable);
            return;
        }
    }

    if ((command == WPAD_MOTOR_STOP && p_wpd->motor == FALSE) || (command == WPAD_MOTOR_RUMBLE && p_wpd->motor == TRUE)) {
        OSRestoreInterrupts(enable);
        return;
    }

    p_wpd->motor = (command == WPAD_MOTOR_STOP) ? FALSE : TRUE;
    p_wpd->rumble = TRUE;
    OSRestoreInterrupts(enable);
}

void WPADEnableMotor(BOOL enable) {
    BOOL intr = OSDisableInterrupts();
    _rumble = enable;
    OSRestoreInterrupts(intr);
}

BOOL WPADIsMotorEnabled() {
    BOOL result;
    BOOL enable = OSDisableInterrupts();
    result = _rumble;
    OSRestoreInterrupts(enable);
    return result;
}

s32 WPADControlLed(s32 chan, u8 pattern, WPADCallback callback) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL busy;
    BOOL enable;
    BOOL setup;
    s32 result = WPAD_ERR_NONE;

    enable = OSDisableInterrupts();
    result = p_wpd->status;
    setup = p_wpd->setup;
    OSRestoreInterrupts(enable);

    if (result != WPAD_ERR_NO_CONTROLLER) {
        if (!setup) {
            result = WPAD_ERR_BUSY;
        } else {
            busy = WPADiSendSetPort(&p_wpd->cmdq, pattern, callback);
            result = (busy) ? WPAD_ERR_NONE : WPAD_ERR_BUSY;
        }
    }

    if (result != WPAD_ERR_NONE) {
        if (callback) {
            callback(chan, result);
        }
    }
    return result;
}

BOOL WPADSaveConfig(WPADFlushCallback callback) {
    BOOL result = TRUE;
    BOOL enable;
    u8 volume;
    u8 mode;

    if (SCCheckStatus() != 0) {
        return FALSE;
    }

    enable = OSDisableInterrupts();
    volume = _speakerVolume;
    mode = (u8)((_rumble) ? 1 : 0);
    OSRestoreInterrupts(enable);

    result &= SCSetWpadSpeakerVolume(volume);
    result &= SCSetWpadMotorMode(mode);

    if (result) {
        SCFlushAsync(callback);
    } else if (callback) {
        callback(2);
    }
    return result;
}

void WPADRead(s32 chan, void* status) {
    WPADControlBlock* p_wpd;
    void* p_data;
    BOOL enable;
    u8 index;

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];

    index = (u8)((p_wpd->rxBufIndex) ? 0 : 1);
    p_data = (void*)(p_wpd->rxBuf + index * WPAD_RX_DATASIZE);

    switch (((WPADStatus*)p_data)->err) {
    case WPAD_ERR_NONE:
        if (WPADiIsCoreFormat(p_wpd->dataFormat)) {
            memcpy(status, p_data, sizeof(WPADStatus));
        } else if (WPADiIsFsFormat(p_wpd->dataFormat)) {
            memcpy(status, p_data, sizeof(WPADFSStatus));
        } else if (WPADiIsClFormat(p_wpd->dataFormat)) {
            memcpy(status, p_data, sizeof(WPADCLStatus));
        } else {
            memcpy(status, p_data, sizeof(WPADStatusEx));
        }
        break;

    case WPAD_ERR_BUSY:
    case WPAD_ERR_CORRUPTED:
        memcpy(status, p_data, sizeof(WPADStatus));
        break;
    default:
        if (WPADiIsCoreFormat(p_wpd->dataFormat)) {
            memset(status, 0, sizeof(WPADStatus));
        } else if (WPADiIsFsFormat(p_wpd->dataFormat)) {
            memset(status, 0, sizeof(WPADFSStatus));
        } else if (WPADiIsClFormat(p_wpd->dataFormat)) {
            memset(status, 0, sizeof(WPADCLStatus));
        } else {
            memset(status, 0, sizeof(WPADStatusEx));
        }

        ((WPADStatus*)status)->err = ((WPADStatus*)p_data)->err;
        break;
    }

    OSRestoreInterrupts(enable);
}

void WPADSetAutoSamplingBuf(s32 chan, void* buf, u32 length) {
    WPADControlBlock* p_wpd;
    BOOL enable;
    s8 err;
    int size;
    int i;

    DEBUGPrint("WPADSetAutoSamplingBuf()\n");

    enable = OSDisableInterrupts();
    p_wpd = _wpdcb[chan];
    err = (s8)((p_wpd->status == WPAD_ERR_NO_CONTROLLER) ? WPAD_ERR_NO_CONTROLLER : WPAD_ERR_INVALID);

    if (WPADiIsCoreFormat(p_wpd->dataFormat)) {
        size = sizeof(WPADStatus);
    } else if (WPADiIsFsFormat(p_wpd->dataFormat)) {
        size = sizeof(WPADFSStatus);
    } else if (WPADiIsClFormat(p_wpd->dataFormat)) {
        size = sizeof(WPADCLStatus);
    } else {
        size = sizeof(WPADStatusEx);
    }

    if (buf) {
        memset(buf, 0, size * length);
        for (i = 0; i < length; ++i) {
            ((WPADStatus*)((u8*)buf + i * size))->err = err;
        }
        p_wpd->bufLatestIndex = 0xFFFFFFFF;
        p_wpd->bufLength = length;
    }
    p_wpd->samplingBufs_ptr = buf;
    OSRestoreInterrupts(enable);
}

void WPADiExcludeButton(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    void* p_data;
    u8 index;
    WPADStatus* p_wp;
    WPADCLStatus* p_cp;

    enable = OSDisableInterrupts();
    index = (u8)((p_wpd->rxBufIndex) ? 0 : 1);
    p_data = (void*)(p_wpd->rxBuf + index * WPAD_RX_DATASIZE);

    p_wp = (WPADStatus*)p_data;
    if ((p_wp->button & WPAD_BTN_HCURSOR_MASK) == WPAD_BTN_HCURSOR_MASK) {
        p_wp->button &= ~WPAD_BUTTON_RIGHT;
    }
    if ((p_wp->button & WPAD_BTN_VCURSOR_MASK) == WPAD_BTN_VCURSOR_MASK) {
        p_wp->button &= ~WPAD_BUTTON_DOWN;
    }

    if (WPADiIsClFormat(p_wpd->dataFormat)) {
        p_cp = (WPADCLStatus*)p_data;
        if ((p_cp->clButton & WPAD_CLB_HCURSOR_MASK) == WPAD_CLB_HCURSOR_MASK) {
            p_cp->clButton &= ~WPAD_CL_BUTTON_RIGHT;
        }
        if ((p_cp->clButton & WPAD_CLB_VCURSOR_MASK) == WPAD_CLB_VCURSOR_MASK) {
            p_cp->clButton &= ~WPAD_CL_BUTTON_DOWN;
        }
    }

    OSRestoreInterrupts(enable);
}

void WPADiCopyOut(s32 chan) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    void* p_data;
    u8 index;
    u32 size;

    enable = OSDisableInterrupts();
    index = (u8)((p_wpd->rxBufIndex) ? 0 : 1);
    p_data = (void*)(p_wpd->rxBuf + index * WPAD_RX_DATASIZE);

    if (p_wpd->samplingBufs_ptr) {
        p_wpd->bufLatestIndex++;
        if (p_wpd->bufLatestIndex >= p_wpd->bufLength) {
            p_wpd->bufLatestIndex = 0;
        }

        if (WPADiIsCoreFormat(p_wpd->dataFormat)) {
            WPADStatus* p_stat;
            p_stat = (WPADStatus*)p_wpd->samplingBufs_ptr + p_wpd->bufLatestIndex;
            size = sizeof(WPADStatus);
            memcpy(p_stat, p_data, size);
        } else if (WPADiIsFsFormat(p_wpd->dataFormat)) {
            WPADFSStatus* p_stat;
            p_stat = (WPADFSStatus*)p_wpd->samplingBufs_ptr + p_wpd->bufLatestIndex;
            size = (((WPADStatus*)p_data)->err == WPAD_ERR_NONE) ? sizeof(WPADFSStatus) : sizeof(WPADStatus);
            memcpy(p_stat, p_data, size);
        } else if (WPADiIsClFormat(p_wpd->dataFormat)) {
            WPADCLStatus* p_stat;
            p_stat = (WPADCLStatus*)p_wpd->samplingBufs_ptr + p_wpd->bufLatestIndex;
            size = (((WPADStatus*)p_data)->err == WPAD_ERR_NONE) ? sizeof(WPADCLStatus) : sizeof(WPADStatus);
            memcpy(p_stat, p_data, size);
        } else {
            WPADStatusEx* p_stat;
            p_stat = (WPADStatusEx*)p_wpd->samplingBufs_ptr + p_wpd->bufLatestIndex;
            size = (((WPADStatus*)p_data)->err == WPAD_ERR_NONE) ? sizeof(WPADStatusEx) : sizeof(WPADStatus);
            memcpy(p_stat, p_data, size);
        }
    }
    if (p_wpd->sampleCallback) {
        p_wpd->sampleCallback(chan);
    }

    p_wpd->packetCnt++;
    OSRestoreInterrupts(enable);
}

BOOL WPADIsSpeakerEnabled(s32 chan) {
    BOOL state;
    BOOL enable;
    WPADControlBlock* p_wpd = _wpdcb[chan];
    enable = OSDisableInterrupts();
    state = p_wpd->infos.speaker;
    OSRestoreInterrupts(enable);
    return state;
}

BOOL WPADiSendMuteSpeaker(WPADCmdQueue* queue, BOOL enable, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;
    cmd.command = WPAD_HIDREP_MUTE;
    cmd.len = 1;
    cmd.data[0] = (u8)((enable) ? (u8)4 : (u8)0);
    cmd.callback = callback;
    result = WPADiPushCommand(queue, cmd);
    return result;
}

BOOL WPADiSendEnableSpeaker(WPADCmdQueue* queue, BOOL enable, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;
    cmd.command = WPAD_HIDREP_ENSPK;
    cmd.len = 1;
    cmd.data[0] = (u8)((enable) ? (u8)4 : (u8)0);
    cmd.callback = callback;
    result = WPADiPushCommand(queue, cmd);
    return result;
}

s32 WPADControlSpeaker(s32 chan, u32 command, WPADCallback callback) {
    BOOL enable;
    BOOL spk;
    BOOL setup;
    s32 result;
    u8 initCmd[] = {0x00, 0x00, 0xd0, 0x07, 0x40, 0x0c, 0x0e};
    WPADControlBlock* p_wpd = _wpdcb[chan];

    enable = OSDisableInterrupts();
    spk = p_wpd->infos.speaker;
    result = p_wpd->status;
    setup = p_wpd->setup;
    OSRestoreInterrupts(enable);

    if (result != WPAD_ERR_NO_CONTROLLER) {
        if (!setup) {
            result = WPAD_ERR_BUSY;
        } else if (command == 0) {
            if (!spk) {
                result = WPAD_ERR_NONE;
            } else {
                enable = OSDisableInterrupts();
                if (__CanPushCmdQueue(&p_wpd->cmdq, 5)) {
                    WPADiSendMuteSpeaker(&p_wpd->cmdq, TRUE, NULL);
                    WPADiSendWriteDataCmd(&p_wpd->cmdq, SPK_DERESET, SPK_RST_REG, NULL);
                    WPADiSendWriteDataCmd(&p_wpd->cmdq, SPK_CLK_OFF, SPK_CLK_REG, NULL);
                    WPADiSendEnableSpeaker(&p_wpd->cmdq, FALSE, NULL);
                    WPADiSendGetContStat(&p_wpd->cmdq, NULL, callback);
                    OSRestoreInterrupts(enable);
                    return WPAD_ERR_NONE;
                } else {
                    result = WPAD_ERR_BUSY;
                }
                OSRestoreInterrupts(enable);
            }
        } else {
            switch (command) {
            case 1:
            case 5:
                enable = OSDisableInterrupts();
                if (__CanPushCmdQueue(&p_wpd->cmdq, 7)) {
                    WPADiSendEnableSpeaker(&p_wpd->cmdq, TRUE, NULL);
                    WPADiSendMuteSpeaker(&p_wpd->cmdq, TRUE, NULL);
                    WPADiSendWriteDataCmd(&p_wpd->cmdq, SPK_CLK_ON, SPK_CLK_REG, NULL);
                    WPADiSendWriteDataCmd(&p_wpd->cmdq, SPK_RESET, SPK_RST_REG, NULL);
                    initCmd[4] = _speakerVolume;
                    WPADiSendWriteData(&p_wpd->cmdq, initCmd, sizeof(initCmd), SPK_RST_REG, NULL);
                    WPADiSendMuteSpeaker(&p_wpd->cmdq, FALSE, NULL);
                    WPADiSendGetContStat(&p_wpd->cmdq, NULL, callback);
                    OSRestoreInterrupts(enable);
                    return WPAD_ERR_NONE;
                } else {
                    result = WPAD_ERR_BUSY;
                }
                OSRestoreInterrupts(enable);
                break;

            case 2:
                if (!WPADiSendMuteSpeaker(&p_wpd->cmdq, TRUE, callback)) {
                    result = WPAD_ERR_BUSY;
                } else {
                    return WPAD_ERR_NONE;
                }
                break;

            case 3:
                if (!WPADiSendMuteSpeaker(&p_wpd->cmdq, FALSE, callback)) {
                    result = WPAD_ERR_BUSY;
                } else {
                    return WPAD_ERR_NONE;
                }
                break;

            case 4:
                if (!WPADiSendWriteDataCmd(&p_wpd->cmdq, SPK_CTRL_PLAY, SPK_CTRL_REG, callback)) {
                    result = WPAD_ERR_BUSY;
                } else {
                    return WPAD_ERR_NONE;
                }
                break;
            }
        }
    }

    if (callback) {
        callback(chan, result);
    }

    return result;
}

u8 WPADGetSpeakerVolume() {
    BOOL enable;
    u8 vol;

    enable = OSDisableInterrupts();
    vol = _speakerVolume;
    OSRestoreInterrupts(enable);

    return vol;
}

void WPADSetSpeakerVolume(u8 volume) {
    BOOL enable;

    enable = OSDisableInterrupts();
    _speakerVolume = __ClampSpeakerVolume(volume);
    OSRestoreInterrupts(enable);
}

BOOL IsBusyStream(s32 chan) {
    BOOL enable;
    WPADControlBlock* p_wpd = _wpdcb[chan];
    u8 stop;
    u32 devType;
    u8 bufNum;
    u16 qSize;
    u16 notAck;
    u8 frames;
    s8 cmdNum;
    u8 linkNum;

    enable = OSDisableInterrupts();
    stop = p_wpd->audioStop;
    devType = p_wpd->devType;
    bufNum = WUDGetBufferStatus();
    cmdNum = __GetCmdNumber(&p_wpd->cmdq);
    qSize = _WPADGetStackBufferStatus(chan);
    notAck = _WPADGetModuleBufferStatus(chan);
    frames = p_wpd->audioFrames;
    linkNum = _WUDGetLinkNumber();
    OSRestoreInterrupts(enable);

    if ((stop) || (notAck > 3) || (bufNum == 10) || (bufNum >= linkNum * 2 + 2) || (devType == WPAD_DEV_UNKNOWN) ||
        (cmdNum >= WPAD_COMMAND_CMD_MAX_LEN - 3) || (frames >= 1)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

BOOL WPADCanSendStreamData(s32 chan) {
    BOOL enable;
    BOOL setup;
    s32 status;
    WPADControlBlock* p_wpd = _wpdcb[chan];

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    setup = p_wpd->setup;
    OSRestoreInterrupts(enable);

    if ((status == WPAD_ERR_NO_CONTROLLER) || (setup == FALSE) || (IsBusyStream(chan))) {
        return FALSE;
    } else {
        return TRUE;
    }
}

BOOL WPADiSendStreamData(WPADCmdQueue* queue, void* p_buf, u16 len) {
    WPADCommand cmd;
    u8 length = (u8)(len << 3);

    cmd.command = WPAD_HIDREP_STRM;
    cmd.len = 21;
    cmd.data[0] = length;
    cmd.callback = NULL;
    memcpy(cmd.data + 1, p_buf, len);
    return WPADiPushCommand(queue, cmd);
}

s32 WPADSendStreamData(s32 chan, void* p_buf, u16 len) {
    BOOL busy;
    BOOL enable;
    BOOL setup;
    s32 status;
    WPADControlBlock* p_wpd = _wpdcb[chan];

    enable = OSDisableInterrupts();
    status = p_wpd->status;
    setup = p_wpd->setup;
    OSRestoreInterrupts(enable);

    if (status == WPAD_ERR_NO_CONTROLLER) {
        return WPAD_ERR_NO_CONTROLLER;
    }

    if (!setup) {
        return WPAD_ERR_BUSY;
    }

    if (IsBusyStream(chan)) {
        return WPAD_ERR_BUSY;
    }

    busy = WPADiSendStreamData(&p_wpd->cmdq, p_buf, len);
    if (!busy) {
        return WPAD_ERR_BUSY;
    }
    enable = OSDisableInterrupts();
    p_wpd->audioFrames++;
    OSRestoreInterrupts(enable);
    return WPAD_ERR_NONE;
}

u8 WPADGetDpdSensitivity() {
    return _dpdSensitivity;
}

BOOL WPADIsDpdEnabled(s32 chan) {
    BOOL state;
    BOOL enable;
    WPADControlBlock* p_wpd = _wpdcb[chan];

    enable = OSDisableInterrupts();
    state = p_wpd->infos.dpd;
    OSRestoreInterrupts(enable);
    return state;
}

static void __dpdCb(s32 chan, s32 result) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    p_wpd->dpdFormat = p_wpd->dpdCommand;
    p_wpd->infos.dpd = (p_wpd->dpdCommand == 0) ? FALSE : TRUE;
}

BOOL WPADiSendEnableDPD(WPADCmdQueue* queue, BOOL enable, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_ENDPD;
    cmd.len = 1;
    cmd.data[0] = (u8)((enable) ? (u8)4 : (u8)0);
    cmd.callback = callback;

    return WPADiPushCommand(queue, cmd);
}

s32 WPADControlDpd(s32 chan, u32 command, WPADCallback callback) {
    BOOL enable;
    BOOL dpd;
    BOOL setup;
    u8 format;
    u8 lastCmd;
    s32 result;
    WPADControlBlock* p_wpd = _wpdcb[chan];

    static const u8 cfg1[5][9] = {
        0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0x64, 0x00, 0xfe, 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0x96, 0x00, 0xb4, 0x02, 0x00, 0x00, 0x71, 0x01,
        0x00, 0xaa, 0x00, 0x64, 0x02, 0x00, 0x00, 0x71, 0x01, 0x00, 0xc8, 0x00, 0x36, 0x07, 0x00, 0x00, 0x71, 0x01, 0x00, 0x72, 0x00, 0x20,
    };
    static const u8 cfg2[5][2] = {
        0xfd, 0x05, 0xb3, 0x04, 0x63, 0x03, 0x35, 0x03, 0x1f, 0x03,
    };

    enable = OSDisableInterrupts();
    dpd = p_wpd->infos.dpd;
    format = p_wpd->dpdFormat;
    lastCmd = p_wpd->dpdCommand;
    result = p_wpd->status;
    setup = p_wpd->setup;
    OSRestoreInterrupts(enable);

    if (result != WPAD_ERR_NO_CONTROLLER) {
        if (!setup) {
            result = WPAD_ERR_BUSY;
        } else if (command == 0) {
            if (!dpd) {
                result = WPAD_ERR_NONE;
            } else {
                enable = OSDisableInterrupts();

                if (__CanPushCmdQueue(&p_wpd->cmdq, 3)) {
                    p_wpd->dpdCommand = (u8)command;

                    WPADiSendEnableDPD(&p_wpd->cmdq, FALSE, NULL);
                    WPADiSendDPDCSB(&p_wpd->cmdq, FALSE, __dpdCb);
                    WPADiSendGetContStat(&p_wpd->cmdq, NULL, callback);

                    OSRestoreInterrupts(enable);
                    return WPAD_ERR_NONE;
                } else {
                    result = WPAD_ERR_BUSY;
                }

                OSRestoreInterrupts(enable);
            }
        } else if (command != lastCmd) {
            enable = OSDisableInterrupts();

            if (__CanPushCmdQueue(&p_wpd->cmdq, 8)) {
                p_wpd->dpdCommand = (u8)command;

                WPADiSendEnableDPD(&p_wpd->cmdq, TRUE, NULL);
                WPADiSendDPDCSB(&p_wpd->cmdq, TRUE, NULL);
                WPADiSendWriteDataCmd(&p_wpd->cmdq, 1, 0x04b00030, NULL);
                WPADiSendWriteData(&p_wpd->cmdq, (void*)cfg1[_dpdSensitivity - 1], 9, 0x04b00000, NULL);
                WPADiSendWriteData(&p_wpd->cmdq, (void*)cfg2[_dpdSensitivity - 1], 2, 0x04b0001a, NULL);
                WPADiSendWriteDataCmd(&p_wpd->cmdq, (u8)command, 0x04b00033, NULL);
                WPADiSendWriteDataCmd(&p_wpd->cmdq, 8, 0x04b00030, __dpdCb);
                WPADiSendGetContStat(&p_wpd->cmdq, NULL, callback);

                OSRestoreInterrupts(enable);
                return WPAD_ERR_NONE;
            } else {
                result = WPAD_ERR_BUSY;
            }
            OSRestoreInterrupts(enable);
        }
    }

    if (callback) {
        callback(chan, result);
    }

    return result;
}

BOOL WPADiSendSetReportType(WPADCmdQueue* queue, u32 fmt, BOOL powerSave, WPADCallback callback) {
    WPADCommand cmd;
    BOOL result;

    cmd.command = WPAD_HIDREP_REPTYPE;
    cmd.len = 2;
    cmd.data[0] = (u8)((powerSave) ? 0 : 4);
    cmd.callback = callback;

    switch (fmt) {
    case WPAD_FMT_CORE:
        cmd.data[1] = 0x30;
        break;
    case WPAD_FMT_CORE_ACC:
        cmd.data[1] = 0x31;
        break;
    case WPAD_FMT_CORE_ACC_DPD:
        cmd.data[1] = 0x33;
        break;
    case WPAD_FMT_FREESTYLE:
        cmd.data[1] = 0x32;
        break;
    case WPAD_FMT_FREESTYLE_ACC:
        cmd.data[1] = 0x35;
        break;
    case WPAD_FMT_FREESTYLE_ACC_DPD:
        cmd.data[1] = 0x37;
        break;
    case WPAD_FMT_CLASSIC:
        cmd.data[1] = 0x32;
        break;
    case WPAD_FMT_CLASSIC_ACC:
        cmd.data[1] = 0x35;
        break;
    case WPAD_FMT_CLASSIC_ACC_DPD:
        cmd.data[1] = 0x37;
        break;
    case WPAD_FMT_CORE_ACC_DPD_FULL:
        cmd.data[1] = 0x3e;
        break;
    }

    return WPADiPushCommand(queue, cmd);
}

void WPADGetAddress(s32 chan, u8* addr) {
    WPADControlBlock* p_wpd = _wpdcb[chan];
    BOOL enable;
    s8 handle;
    u8* devAddr;

    enable = OSDisableInterrupts();
    handle = p_wpd->dev_handle;
    OSRestoreInterrupts(enable);

    devAddr = _WUDGetDevAddr((u8)handle);

    if (devAddr) {
        memcpy(addr, devAddr, 6);
    } else {
        memset(addr, 0, 6);
    }
}

void WPADShutdown() {
    return;
}

void __WPADShutdown() {
    BOOL enable;
    int i;

    enable = OSDisableInterrupts();

    if (_shutdown) {
        OSRestoreInterrupts(enable);
        return;
    }

    _shutdown = 1;
    WUDSetVisibility(0, 0);

    for (i = 10; i < 14; i++) {
        memset(&_scArray.info[i], 0, sizeof(SCBtDeviceInfoSingle));
    }

    WPADiShutdown(FALSE);
    OSRestoreInterrupts(enable);
}

void __WPADReconnect(BOOL exec) {
    BOOL enable = OSDisableInterrupts();

    if (!_startup) {
        OSRestoreInterrupts(enable);
        return;
    }

    if (_shutdown) {
        OSRestoreInterrupts(enable);
        return;
    }

    _shutdown = 1;
    _recFlag = (exec) ? 1 : 0;
    DEBUGPrint("Wait for %d ms until start reconnect!\n", _recCnt);
    OSRestoreInterrupts(enable);
}

void WPADiShutdown(BOOL exec) {
    OSCancelAlarm(&_managerAlarm);
    WUDSetHidRecvCallback(NULL);
    WUDShutdown(exec);
}

BOOL WPADCancelSyncDevice() {
    return WUDCancelSyncDevice();
}
