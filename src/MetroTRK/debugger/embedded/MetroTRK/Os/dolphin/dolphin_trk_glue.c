#include "TRK_Types.h"

extern void OSReport(const char *, ...);
extern int OSEnableScheduler(void);

struct gDBCommTable_t {
    int (*init)(volatile char **, void *);      // 0x0
    int (*interrupt)(void);                     // 0x4
    int (*shutdown)(void);                      // 0x8
    int (*peek)(void);                          // 0xC
    int (*read)(char *, int);                   // 0x10
    int (*write)(char *, int);                  // 0x14
    int (*open)(void);                          // 0x18
    int (*close)(void);                         // 0x1C
    int (*pre_continue)(void);                  // 0x20
    int (*stop)(void);                          // 0x24
};

typedef struct OSContext {
    u32 gpr[32];
    u32 cr;
    u32 lr;
    u32 ctr;
    u32 xer;
    f64 fpr[32];
    u32 fpscr_pad;
    u32 fpscr;
    u32 srr0;
    u32 srr1;
    u16 mode;
    u16 state;
    u32 gqr[8];
    u32 psf_pad;
    f64 psf[32];
} OSContext;

void TRKLoadContext(OSContext *, int);

static unsigned char TRK_Use_BBA;

struct gDBCommTable_t gDBCommTable = { 0 };

static char board_msg[4] = "%s\n";

void TRK_board_display(const char *msg) {
    OSReport(board_msg, msg);
}

void UnreserveEXI2Port(void) {
    gDBCommTable.pre_continue();
}

void ReserveEXI2Port(void) {
    gDBCommTable.stop();
}

int TRKWriteUARTN(const void *data, unsigned long length) {
    return gDBCommTable.write((char*)data, length) ? -1 : 0;
}

int TRKReadUARTN(void *data, unsigned long length) {
    return gDBCommTable.read(data, length) ? -1 : 0;
}

void TRKPollUART(void) {
    gDBCommTable.peek();
}

void EnableEXI2Interrupts(void) {
    if (!TRK_Use_BBA) {
        if (gDBCommTable.interrupt != 0) {
            gDBCommTable.interrupt();
        }
    }
}

void TRKEXICallBack(void *unk, OSContext *context) {
    OSEnableScheduler();
    TRKLoadContext(context, 0x500);
}