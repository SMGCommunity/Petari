#include "uusb.h"

/*******************************************************************************
 * headers
 */

#include <cstring>
#include <mem.h>

#include <decomp.h>
#include <macros.h>

#include "data_types.h"
#include "gki_target.h"

#include "bte.h"
#include "gki.h"
#include "hci.h"
#include "l2c_int.h"

#include <revolution/types.h>

#define IS_BTE
#include <context_rvl.h>

/*******************************************************************************
 * macros
 */

#undef NULL
#define NULL ((void*)(0))

#if UUSB_DBG
#define UUSBDBG(param, ...)                                                                                                                          \
    do {                                                                                                                                             \
        ALOGD(param, ##__VA_ARGS__);                                                                                                                 \
    } while (FALSE)
#else
#define UUSBDBG(param, ...)                                                                                                                          \
    do {                                                                                                                                             \
    } while (FALSE)
#endif

/*******************************************************************************
 * types
 */

typedef struct {
    long fd;  // TOOD: IOSFd
    char pad0_[0x10 - (0x00 + 0x04)];
    unk1_t unsigned bulkEndpoint1;
    unk1_t unsigned bulkEndpoint2;
    unk1_t unsigned intrEndpoint1;
    unk1_t at_0x13;  // intrEndpoint2?
    unk4_t vid;
    unk4_t pid;
    UINT8 cmd_buffer_pool;
    UINT8 acl_buffer_pool;
    char pad2_[0x20 - (0x1d + 0x01)];
    tUUSB_CBACK* cback;
    tUUSB* p_uusb_cfg;
    UINT8 trace_state;
    UINT8 state;
    char pad4_[1];
    UINT8 reading_intr_data;
    BUFFER_Q bulk_buffer_q;
    UINT8 at_0x38;
    BUFFER_Q intr_buffer_q;
    UINT8 at_0x48;
    char pad8_[3];
} tUUSB_CB;

/*******************************************************************************
 * local function declarations
 */

static void uusb_ReleaseCmdBufferPool(void);
static void uusb_ReleaseAclBufferPool(void);
static void uusb_CloseDeviceCB(long result, void* p_data);
static void uusb_ReadIntrDataCB(long result, void* p_data);
static void uusb_ReadBulkDataCB(long result, void* p_data);
static void uusb_issue_bulk_read(void);
static void uusb_issue_intr_read(void);
static void uusb_WriteCtrlDataCB(long result, void* p_data);
static void uusb_WriteBulkDataCB(long result, void* p_data);
static long uusb_get_devId(int vid, int pid);

/*******************************************************************************
 * variables
 */

// .bss
static tUUSB_CB usb;
unsigned char __uusb_ppc_stack1[0x1000] __attribute__((aligned(32)));
unsigned char __uusb_ppc_stack2[0x1000] __attribute__((aligned(32)));

// .sdata
// TODO: IOSFd, IOS_INVALID_FD
UINT32 volatile wait4hci = TRUE;
static long __ntd_ios_file_descriptor = -1l;

// .sbss
static int __ntd_pid;
static int __ntd_vid;
static BOOLEAN __ntd_pid_vid_specified;
static UINT8 uusb_g_usb_devid_found = FALSE;
static UINT8 uusb_g_trace_state_initialized = FALSE;
UINT32 __ntd_ohci = 0;
UINT32 __ntd_ohci_init_flag = FALSE;

/*******************************************************************************
 * functions
 */

void HCI_TRACE(char const* p_msg) {
    /* ... */
}

void HCI_TRACE1(char const* p_msg, UINT32 arg1) {
    /* ... */
}

void __ntd_set_ohci(UINT8 ohci) {
    if (ohci == 0) {
        __ntd_ohci = 0;
        __ntd_ohci_init_flag = TRUE;
    } else if (ohci == 1) {
        __ntd_ohci = 1;
        __ntd_ohci_init_flag = TRUE;
    }
}

void __ntd_set_pid_vid(int pid, int vid) {
    __ntd_vid = vid;
    __ntd_pid = pid;
    __ntd_pid_vid_specified = TRUE;
}

int __ntd_get_usb_file_descriptor(void) {
    return __ntd_ios_file_descriptor;
}

UINT32 __ntd_get_allocated_mem_size(void) {
    UINT32 size = 0;

    // TODO: what are these the sizes of?
    size += 0x07620;
    size += 0x10000;
    size -= 0x02BA8;

    return size;
}

void uusb_set_trace_state(UINT8 state) {
    usb.trace_state = state;

    uusb_g_trace_state_initialized = TRUE;
}

UINT8 uusb_get_trace_state(void) {
    return usb.trace_state;
}

static void uusb_ReleaseCmdBufferPool(void) {
    GKI_delete_pool(usb.cmd_buffer_pool);

    usb.cmd_buffer_pool = GKI_INVALID_POOL;
}

static void uusb_ReleaseAclBufferPool(void) {
    GKI_delete_pool(usb.acl_buffer_pool);

    usb.acl_buffer_pool = GKI_INVALID_POOL;
}

static void uusb_CloseDeviceCB(long result, void* p_data) {
    usb.bulkEndpoint1 = 0;
    usb.bulkEndpoint2 = 0;
    usb.intrEndpoint1 = 0;
    usb.at_0x13 = 0;

    usb.fd = 0;

    uusb_g_usb_devid_found = 0;

    UUSB_Unregister();

    if (usb.cback)
        (*usb.cback)(4, result);
}

static void uusb_ReadIntrDataCB(long result, void* p_data) {
    unk4_t sp14 = 0;
    UINT32 len;
    void* p_buffer = NULL;
    HC_BT_HDR* p_buf = p_data;
    unk4_t sp10 = 0;
    unk4_t sp0c = 0;

    len = 0;

    usb.reading_intr_data = FALSE;

    if (usb.state != 2) {
        GKI_freebuf(p_buf);
        p_buf = NULL;

        uusb_ReleaseCmdBufferPool();

        return;
    }

    if (p_buf == NULL)
        goto end;

    if (result <= 0) {
        GKI_freebuf(p_buf);
        p_buf = NULL;
        goto end;
    }

    p_buffer = GKI_getpoolbuf(GKI_POOL_ID_2);
    if (!p_buffer) {
        GKI_freebuf(p_buf);
        p_buf = NULL;
        goto end;
    }

    p_buf->event = 0x1000;
    p_buf->len = result;

    len = sizeof *p_buf + p_buf->len + p_buf->offset;
    len = (len >> 2 << 2) + 4;  // round up to 4
    memcpy(p_buffer, p_buf, len);

    OSSwitchFiberEx((unsigned long)p_buffer, 0, 0, 0, (u32)&bta_ci_hci_msg_handler, (u32)(__uusb_ppc_stack1 + 4096));

    GKI_freebuf(p_buf);
    p_buf = NULL;

end:
    uusb_issue_intr_read();
}

static void uusb_ReadBulkDataCB(long result, void* p_data) {
    unk4_t sp14 = 0;
    UINT32 len;
    void* p_buffer = NULL;
    HC_BT_HDR* p_buf = p_data;
    unk4_t sp10 = 0;

    len = 0;

    if (usb.state != 2) {
        GKI_freebuf(p_buf);
        p_buf = NULL;

        uusb_ReleaseAclBufferPool();

        return;
    }

    if (result <= 0) {
        GKI_freebuf(p_buf);
        p_buf = NULL;
        goto end;
    }

    p_buf->len = result;

    p_buffer = GKI_getpoolbuf(GKI_POOL_ID_3);
    if (!p_buffer) {
        GKI_freebuf(p_buf);
        p_buf = NULL;
        goto end;
    }

    len = sizeof *p_buf + p_buf->len + p_buf->offset;
    len = (len >> 2 << 2) + 4;
    memcpy(p_buffer, p_buf, len);

    p_buffer = l2cap_link_chk_pkt_start(p_buffer);

    if (p_buffer != NULL && l2cap_link_chk_pkt_end()) {
        OSSwitchFiberEx((unsigned long)p_buffer, 0, 0, 0, (u32)&bta_ci_hci_msg_handler, (u32)(__uusb_ppc_stack2 + 4096));
        p_buffer = 0;
    }

    GKI_freebuf(p_buf);
    p_buf = NULL;

end:
    uusb_issue_bulk_read();
}

static void uusb_issue_bulk_read(void) {
    HC_BT_HDR* p_buf = NULL;
    void* p_buffer;
    unk_t r28 = 0;
    IPCResult ret;

    do
        p_buf = GKI_getpoolbuf(usb.acl_buffer_pool);
    while (!p_buf);

    p_buf->event = 0x1100;
    p_buf->len = 0;

    p_buffer = ROUND_UP_PTR((UINT8*)(p_buf + 1), 32);

    p_buf->offset = (UINT8*)p_buffer - (UINT8*)(p_buf + 1);

    // TODO: what is 0x28?
    ret = IUSB_ReadBlkMsgAsync(usb.fd, usb.bulkEndpoint2, GKI_get_buf_size(p_buf) - 0x28 - p_buf->offset, p_buffer, uusb_ReadBulkDataCB, p_buf);

    if (ret != 0) {
        GKI_freebuf(p_buf);
        p_buf = NULL;
    }
}

static void uusb_issue_intr_read(void) {
    HC_BT_HDR* p_buf = NULL;
    void* p_buffer = NULL;
    unk_t r28 = 0;
    IPCResult ret;

    do
        p_buf = GKI_getpoolbuf(usb.cmd_buffer_pool);
    while (p_buf == 0);

    p_buf->event = 0x1000;
    p_buf->len = 0;

    p_buffer = ROUND_UP_PTR((UINT8*)(p_buf + 1), 32);

    p_buf->offset = (UINT8*)p_buffer - (UINT8*)(p_buf + 1);

    ret = IUSB_ReadIntrMsgAsync(usb.fd, usb.intrEndpoint1, GKI_get_buf_size(p_buf) - 0x28 - p_buf->offset, p_buffer, uusb_ReadIntrDataCB, p_buf);

    if (ret != 0) {
        GKI_freebuf(p_buf);
        p_buf = NULL;
    }

    usb.reading_intr_data = TRUE;
}

static void uusb_WriteCtrlDataCB(long result, void* p_data) {
    HC_BT_HDR* p_buf = NULL;
    void* p_buffer = NULL;
    IPCResult ret = 0;

    if (result < 0)
        UUSBDBG("Result was less than 0 : %d", result);

    if (p_data != NULL) {
        GKI_freebuf(p_data);
        p_data = NULL;

        GKI_disable();

        --usb.at_0x48;

        GKI_enable();
    }

    if (usb.at_0x48 >= 5)
        return;

    if (usb.intr_buffer_q.count == 0)
        return;

    p_buf = GKI_dequeue(&usb.intr_buffer_q);

    p_buffer = (UINT8*)(p_buf + 1) + p_buf->offset;

    ret = IUSB_WriteCtrlMsgAsync(usb.fd, 0x20, 0, 0, 0, p_buf->len, p_buffer, uusb_WriteCtrlDataCB, p_buf);

    if (ret < 0) {
        GKI_enqueue_head(&usb.intr_buffer_q, p_buf);
    } else {
        GKI_disable();

        ++usb.at_0x48;

        GKI_enable();
    }
}

static void uusb_WriteBulkDataCB(long result, void* p_data) {
    HC_BT_HDR* p_buf = NULL;
    void* p_buffer = NULL;
    IPCResult ret = 0;

    if (result < 0)
        UUSBDBG("Result was less than 0 : %d", result);

    if (p_data != NULL) {
        GKI_freebuf(p_data);
        p_data = NULL;

        GKI_disable();

        --usb.at_0x38;

        GKI_enable();
    }

    if (usb.at_0x38 >= 5)
        return;

    if (usb.bulk_buffer_q.count == 0)
        return;

    p_buf = GKI_dequeue(&usb.bulk_buffer_q);

    p_buffer = (UINT8*)(p_buf + 1) + p_buf->offset;
    ret = IUSB_WriteBlkMsgAsync(usb.fd, usb.bulkEndpoint1, p_buf->len, p_buffer, uusb_WriteBulkDataCB, p_buf);

    if (ret < 0) {
        GKI_enqueue_head(&usb.bulk_buffer_q, p_buf);
    } else {
        GKI_disable();

        ++usb.at_0x38;

        GKI_enable();
    }
}

static long uusb_get_devId(int vid, int pid) {
    IPCResult ret;

    if (__ntd_ohci_init_flag == TRUE) {
        if (__ntd_ohci == 0)
            ret = IUSB_OpenDeviceIds("oh0", vid, pid, &usb.fd);
        else if (__ntd_ohci == 1)
            ret = IUSB_OpenDeviceIds("oh1", vid, pid, &usb.fd);
    } else {
        ret = IUSB_OpenDeviceIds("oh1", vid, pid, &usb.fd);
    }

    if (0 <= ret)
        __ntd_ios_file_descriptor = ret;

    return ret;
}

void UUSB_Register(tUUSB* uusb) {
    IPCResult ret = 0;

    memset(&usb, 0, sizeof usb);

    GKI_disable();

    // I must say, this 0/2/4/5 stuff seems quite familiar, wouldn't you say?
    usb.state = 5;

    GKI_enable();

    if (!uusb_g_trace_state_initialized)
        uusb_set_trace_state(0);

    ret = IPCCltInit();
    if (ret != 0)
        return;

    ret = IUSB_OpenLib();
    if (ret != 0)
        return;

    usb.p_uusb_cfg = uusb;

    if (__ntd_pid_vid_specified == TRUE) {
        usb.vid = __ntd_vid;
        usb.pid = __ntd_pid;
    } else {
        // clang-format off
		usb.vid = 0x057E; // 1406 - Nintendo Co., Ltd.
		usb.pid = 0x0305; //  773 - Broadcom BCM2045A Bluetooth Radio [Nintendo Wii/Wii U]
        // clang-format on
    }

    usb.bulkEndpoint1 = 0;
    usb.bulkEndpoint2 = 0;
    usb.intrEndpoint1 = 0;
    usb.at_0x13 = 0;

    if (uusb_get_devId(usb.vid, usb.pid) < 0)
        return;

    usb.bulkEndpoint1 = 2;
    usb.bulkEndpoint2 = 130;
    usb.intrEndpoint1 = 129;
    usb.at_0x13 = 0;

    GKI_init_q(&usb.bulk_buffer_q);
    usb.at_0x38 = 0;

    GKI_init_q(&usb.intr_buffer_q);
    usb.at_0x48 = 0;

    usb.cmd_buffer_pool = GKI_create_pool(660, 45, 1, NULL);
    usb.acl_buffer_pool = GKI_create_pool(1800, 30, 1, NULL);

    if (usb.cmd_buffer_pool == GKI_INVALID_POOL || usb.acl_buffer_pool == GKI_INVALID_POOL) {
        return;
    }

    GKI_disable();

    usb.state = 4;

    GKI_enable();

    wait4hci = TRUE;
}

void UUSB_Open(tUUSB* uusb, tUUSB_CBACK* cback) {
    /* Use efficient coding to avoid pipeline stalls */
    if (usb.state != 4)
        return;

    if (usb.cmd_buffer_pool == GKI_INVALID_POOL || usb.acl_buffer_pool == GKI_INVALID_POOL) {
        return;
    }

    GKI_disable();

    usb.cback = cback;
    usb.state = 2;

    GKI_enable();

    uusb_issue_intr_read();
    uusb_issue_bulk_read();

    wait4hci = FALSE;

    (void)uusb;
}

UINT16 UUSB_Read(UINT8 param_1, void* param_2, UINT16 param_3) {
    return 0;
}

void UUSB_ReadBuf(/* ... */) {
    /* ... */
}

unk_t UUSB_WriteBuf(/* ... */) {
    return 0;
}

UINT16 UUSB_Write(UINT8 param_1, void* p_data, UINT16 len, void* param_4) {
    unk2_t sp18 = 0;
    IPCResult ret = 0;
    void* p_buffer = NULL;
    unk4_t sp_0x1c = 0;
    HC_BT_HDR* p_buf = NULL;

    if (usb.state != 2)
        return 0;

    switch (param_1) {
    case 0:
        p_buf = GKI_getpoolbuf(usb.cmd_buffer_pool);

        if (p_buf == NULL)
            return 0;

        p_buf->len = len;

        p_buffer = ROUND_UP_PTR((UINT8*)(p_buf + 1), 32);

        p_buf->offset = (UINT8*)p_buffer - (UINT8*)(p_buf + 1);

        memcpy(p_buffer, p_data, len);

        if (usb.at_0x48 < 5 && usb.intr_buffer_q.count == 0) {
            ret = IUSB_WriteCtrlMsgAsync(usb.fd, 0x20, 0, 0, 0, len, p_buffer, uusb_WriteCtrlDataCB, p_buf);
        } else {
            GKI_enqueue(&usb.intr_buffer_q, p_buf);

            return 0;
        }

        if (ret != 0) {
            if (ret != -22)
                UUSBDBG("ret was not negative twenty two: %d", ret);

            if (ret == -8)
                UUSBDBG("ret was negative eight: %d (8, eight)", ret);

            GKI_freebuf(p_buf);
            p_buf = NULL;
        } else {
            GKI_disable();

            ++usb.at_0x48;

            GKI_enable();
        }

        break;

    case 2:
        p_buf = GKI_getpoolbuf(usb.acl_buffer_pool);

        if (p_buf == NULL)
            return 0;

        p_buf->len = len;

        p_buffer = ROUND_UP_PTR((UINT8*)(p_buf + 1), 32);

        p_buf->offset = (UINT8*)p_buffer - (UINT8*)(p_buf + 1);

        memcpy(p_buffer, p_data, len);

        if (len > 190)
            UUSBDBG(" woah ! thats pretty Big ! :%d", param_3);

        if (usb.at_0x38 < 5 && usb.bulk_buffer_q.count == 0) {
            ret = IUSB_WriteBlkMsgAsync(usb.fd, usb.bulkEndpoint1, len, p_buffer, uusb_WriteBulkDataCB, p_buf);
        } else {
            GKI_enqueue(&usb.bulk_buffer_q, p_buf);

            return 0;
        }

        if (ret != 0) {
            if (ret != -22)
                UUSBDBG("ret was negative 2wenty 2wo: %d", ret);

            if (ret == -8)
                ;
            UUSBDBG("rat was negative eight: %d", ret);

            GKI_freebuf(p_buf);
            p_buf = NULL;
        } else {
            GKI_disable();

            ++usb.at_0x38;

            GKI_enable();
        }

        break;

    case 3:
        break;
    }

    (void)param_4;

    return ret;
}

void UUSB_Ioctl(tUUSB_IOCTL_OP op, void* p_data) {
    /* ... */
}

void UUSB_Close(void) {
    GKI_disable();

    usb.state = 0;

    GKI_enable();

    if (usb.bulk_buffer_q.count) {
        while (usb.bulk_buffer_q.count)
            GKI_freebuf(GKI_dequeue(&usb.bulk_buffer_q));
    }

    if (usb.intr_buffer_q.count) {
        while (usb.intr_buffer_q.count)
            GKI_freebuf(GKI_dequeue(&usb.intr_buffer_q));
    }

    IUSB_CloseDeviceAsync(usb.fd, uusb_CloseDeviceCB, NULL);
}

void UUSB_Unregister(void) {
    IUSB_CloseLib();

    GKI_disable();

    usb.state = 5;
    usb.trace_state = 0;

    uusb_g_trace_state_initialized = FALSE;

    GKI_enable();
}

unk_t UUSB_Feature(void) {
    return 0;
}
