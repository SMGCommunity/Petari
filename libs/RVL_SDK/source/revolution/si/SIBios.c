#include <revolution/si.h>
#include <revolution/os.h>
#include <private/flipper.h>
#include <private/io_reg.h>

#define OFFSET(n, a) (((u32) (n)) & ((a) - 1))
#define ROUND(n, a) (((u32) (n) + (a) - 1) & ~((a) - 1))

typedef struct SIControl {
    s32 chan;
    u32 poll;
    u32 inputBytes;
    void* input;
    SICallback callback;
} SIControl;

static SIControl Si = {
    -1
};

static SIPacket Packet[4];
static OSAlarm Alarm[4];

static u32 Type[4] = {
    8,
    8,
    8,
    8
};

static OSTime TypeTime[4];
static OSTime XferTime[4];
static SITypeAndStatusCallback TypeCallback[4][4];
static __OSInterruptHandler RDSTHandler[4];
u32 __PADFixBits;

static BOOL InputBufferValid[4];
static u32 InputBuffer[4][2];
static vu32 InputBufferVcount[4];

static const char* __SIVersion = "<< RVL_SDK - SI \trelease build: Aug  8 2007 02:07:10 (0x4199_60831) >>";

static void GetTypeCallback(s32, u32, OSContext *);

BOOL SIIsChanBusy(s32 chan) {
    return Packet[chan].chan != -1 || Si.chan == chan;
}

static void SIClearTCInterrupt(void){ 
    u32 reg;
    reg = __SIRegs[0xD];
    reg |= 0x80000000;
    reg &= 0xFFFFFFFE;
    __SIRegs[0xD] = reg;
}

static u32 CompleteTransfer(void) {
    u32 sr;
    u32 i;
    u32 rLen;
    u8* input;

    sr = __SIRegs[0xE];
    SIClearTCInterrupt();

    if (Si.chan != -1) {
        XferTime[Si.chan] = __OSGetSystemTime();
        input = Si.input;
        rLen = Si.inputBytes / 4;

        for (i = 0; i < rLen; i++) {
            *(u32*)input = __SIRegs[0x20 + i];
            input += 4;
        }

        rLen = OFFSET(Si.inputBytes, 4);
        if (rLen) {
            u32 temp = __SIRegs[0x20 + i];

            for (i = 0; i < rLen; i++) {
                *input++ = (u8)((temp >> ((3 - i) * 8)) & 0xFF);
            }
        }

        if (__SIRegs[0xD] & 0x20000000) {
            sr >>= 8 * (3 - Si.chan);
            sr &= 0xF;

            if ((sr & 8) && !(Type[Si.chan] & 0x80)) {
                Type[Si.chan] = 8;
            }

            if (sr == 0) {
                sr = 4;
            }
        }
        else {
            TypeTime[Si.chan] = __OSGetSystemTime();
            sr = 0;
        }

        Si.chan = -1;
    }

    return sr;
}

static void SITransferNext(s32 chan) {
    int i;
    SIPacket* packet;

    for (i = 0; i < 4; ++i) {
        ++chan;
        chan %= 4;
        packet = &Packet[chan];

        if (packet->chan != -1 && packet->fire <= __OSGetSystemTime()) {
            if (__SITransfer(packet->chan, packet->output, packet->outputBytes, packet->input, packet->inputBytes, packet->callback)) {
                OSCancelAlarm(&Alarm[chan]);
                packet->chan = -1;
            }

            break;
        }
    }
}

void SIInterruptHandler(__OSInterrupt interrupt, OSContext* context) {
    u32 reg = __SIRegs[0xD];

    if ((reg & 0xC0000000) == 0xC0000000) {
        s32 chan;
        u32 sr;
        SICallback callback;
        chan = Si.chan;
        sr = CompleteTransfer();
        callback = Si.callback;
        Si.callback = 0;
        
        SITransferNext(chan);

        if (callback) {
            callback(chan, sr, context);
        }

        sr = __SIRegs[0xE];
        sr &= 0xF000000 >> (8 * chan);
        __SIRegs[0xE] = sr;

        if (Type[chan] == 0x80 && !SIIsChanBusy(chan)) {
            static u32 cmdTypeAndStatus = 0;
            SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3, GetTypeCallback, (OSTime)OSMicrosecondsToTicks(65));
        }
    }

    if ((reg & 0x18000000) == 0x18000000) {
        int i;
        u32 vcount;
        u32 x;

        vcount = VIGetCurrentLine() + 1;
        x = (Si.poll & 0x03FF0000) >> 16;

        for (i = 0; i < 4; i++) {
            if (SIGetResponseRaw(i)) {
                InputBufferVcount[i] = vcount;
            }
        }

        for (i = 0; i < 4; i++) {
            if (!(Si.poll & (0x80000000 >> (31 - 7 + i)))) {
                continue;
            }

            if (InputBufferVcount[i] == 0 || InputBufferVcount[i] + (x / 2) < vcount) {
                return;
            }
        }

        for (i = 0; i < 4; i++) {
            InputBufferVcount[i] = 0;
        }

        for (i = 0; i < 4; i++) {
            if (RDSTHandler[i]) {
                RDSTHandler[i](interrupt, context);
            }
        }
    }
}

u32 SIGetStatus(s32 chan) {
    BOOL enabled;
    u32 sr;
    int chanShift;

    enabled = OSDisableInterrupts();
    sr = __SIRegs[0xE];
    chanShift = 8 * (3 - chan);
    sr >>= chanShift;

    if (sr & 8) {
        if (!(Type[chan] & 0x80)) {
            Type[chan] = 8;
        }
    }

    OSRestoreInterrupts(enabled);
    return sr;
}

static BOOL SIGetResponseRaw(s32 chan) {
    u32 sr;

    sr = SIGetStatus(chan);

    if (sr & 0x20) {
        InputBuffer[chan][0] = __SIRegs[3 * chan + 1];
        InputBuffer[chan][1] = __SIRegs[3 * chan + 2];
        InputBufferValid[chan] = TRUE;
        return TRUE;
    }

    return FALSE;
}

void SIInit(void) {
    static BOOL Initialized = FALSE;

    if (Initialized) {
        return;
    }

    OSRegisterVersion(__SIVersion);
    Packet[0].chan = Packet[1].chan = Packet[2].chan = Packet[3].chan = -1;
    Si.poll = 0;
    SISetSamplingRate(0);

    while (__SIRegs[0xD] & 1) { }

    __SIRegs[0xD] = 0x80000000;
    __OSSetInterruptHandler(20, SIInterruptHandler);
    __OSUnmaskInterrupts(0x800);

    SIGetType(0);
    SIGetType(1);
    SIGetType(2);
    SIGetType(3);
    Initialized = TRUE;
}

BOOL __SITransfer(s32 chan, void* output, u32 outputBytes, void* input, u32 inputBytes, SICallback callback) {
    BOOL enabled;
    u32 rLen;
    u32 i;
    u32 sr;
    si_comcsr_u comcsr;

    enabled = OSDisableInterrupts();

    if (Si.chan != -1) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    sr = __SIRegs[0xE];
    sr &= 0xF000000 >> (8 * chan);
    __SIRegs[0xE] = sr;

    Si.chan = chan;
    Si.callback = callback;
    Si.inputBytes = inputBytes;
    Si.input = input;

    rLen = ROUND(outputBytes, 4) / 4;

    for (i = 0; i < rLen; i++) {
        __SIRegs[0x20 + i] = ((u32*)output)[i];
    }

    comcsr.val = __SIRegs[0xD];
    comcsr.f.tcint = 1;
    comcsr.f.tcintmsk = callback ? 1 : 0;
    comcsr.f.outlngth = (outputBytes == 128) ? 0 : outputBytes;
    comcsr.f.inlngth = (inputBytes == 128) ? 0 : inputBytes;
    comcsr.f.channel = chan;
    comcsr.f.tstart = 1;
    __SIRegs[0xD] = comcsr.val;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

u32 SISetXY(u32 x, u32 y) {
    u32 poll;
    BOOL enabled;

    poll = x << 16;
    poll |= y << 8;

    enabled = OSDisableInterrupts();
    Si.poll &= ~(0x3FF0000 | 0xFF00);
    Si.poll |= poll;
    poll = Si.poll;
    __SIRegs[0xC] = poll;
    OSRestoreInterrupts(enabled);
    return poll;
}

static void AlarmHandler(OSAlarm* alarm, OSContext* context) {
    s32 chan;
    SIPacket* packet;

    chan = alarm - Alarm;
    packet = &Packet[chan];

    if (packet->chan != -1) {
        if (__SITransfer(packet->chan, packet->output, packet->outputBytes, packet->input, packet->inputBytes, packet->callback)) {
            packet->chan = -1;
        }
    }
}

BOOL SITransfer(s32 chan, void* output, u32 outputBytes, void* input, u32 inputBytes, SICallback callback, OSTime delay) {
    BOOL enabled;
    SIPacket* packet = &Packet[chan];
    OSTime now;
    OSTime fire;

    enabled = OSDisableInterrupts();

    if (packet->chan != -1 || Si.chan == chan) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    now = __OSGetSystemTime();

    if (delay == 0) {
        fire = now;
    }
    else {
        fire = XferTime[chan] + delay;
    }

    if (now < fire) {
        delay = fire - now;
        OSSetAlarm(&Alarm[chan], delay, AlarmHandler);
    }
    else if (__SITransfer(chan, output, outputBytes, input, inputBytes, callback)) {
        OSRestoreInterrupts(enabled);
        return TRUE;
    }

    packet->chan = chan;
    packet->output = output;
    packet->outputBytes = outputBytes;
    packet->input = input;
    packet->inputBytes = inputBytes;
    packet->callback = callback;
    packet->fire = fire;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

static void CallTypeAndStatusCallback(s32 chan, u32 type) {
    SITypeAndStatusCallback callback;
    int i;

    for (i = 0; i < 4; ++i) {
        callback = TypeCallback[chan][i];

        if (callback) {
            TypeCallback[chan][i] = 0;
            callback(chan, type);
        }
    }
}

static void GetTypeCallback(s32 chan, u32 error, OSContext* context) {
    static u32 cmdFixDevice[4];
    u32 type;
    u32 chanBit;
    BOOL fix;
    u32 id;

    Type[chan] &= 0xFFFFFF7F;
    Type[chan] |= error;
    TypeTime[chan] = __OSGetSystemTime();
    type = Type[chan];
    chanBit = 0x80000000 >> chan;
    fix = __PADFixBits & chanBit;
    __PADFixBits &= ~chanBit;

    if ((error & 0xF) || (type & 0x18000000) != 0x08000000 || !(type & 0x80000000) || (type & 0x4000000)) {
        OSSetWirelessID(chan, 0);
        CallTypeAndStatusCallback(chan, Type[chan]);
        return;
    }

    id = OSGetWirelessID(chan) << 8;

    if (fix && (id & 0x100000)) {
        cmdFixDevice[chan] = 0x4E << 24 | (id & 0xCFFF00) | 0x4E100000;
        Type[chan] = 0x80;
        SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3, GetTypeCallback, 0);
        return;
    }

    if (type & 0x100000) {
        if ((id & 0xCFFF00) != (type & 0xCFFF00)) {
            if (!(id & 0x00100000)) {
                id = type & 0xCFFF00;
                id |= 0x00100000;
                OSSetWirelessID(chan, (u16)((id >> 8) & 0xFFFF));
            }

            cmdFixDevice[chan] = 0x4E << 24 | id;
            Type[chan] = 0x80;
            SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3, GetTypeCallback, 0);
            return;
        }
    }
    else if (type & 0x40000000) {
        id = type & 0xCFFF00;
        id |= 0x00100000;

        OSSetWirelessID(chan, (u16)((id >> 8) & 0xFFFF));
        cmdFixDevice[chan] = 0x4E << 24 | id;
        Type[chan] = 0x80;
        SITransfer(chan, &cmdFixDevice[chan], 3, &Type[chan], 3, GetTypeCallback, 0);
        return;
    }
    else {
        OSSetWirelessID(chan, 0);
    }

    CallTypeAndStatusCallback(chan, Type[chan]);
}

u32 SIGetType(s32 chan) {
    static u32 cmdTypeAndStatus = 0;
    BOOL enabled;
    u32 type;
    OSTime diff;

    enabled = OSDisableInterrupts();
    type = Type[chan];
    diff = __OSGetSystemTime() - TypeTime[chan];

    if (Si.poll & (0x80 >> chan)) {
        if (type != 8) {
            TypeTime[chan] = __OSGetSystemTime();
            OSRestoreInterrupts(enabled);
            return type;
        }
        else {
            type = Type[chan] = 0x80;
        }
    }
    else if (diff <= OSMillisecondsToTicks(50) && type != 8) {
        OSRestoreInterrupts(enabled);
        return type;
    }
    else if (diff <= OSMillisecondsToTicks(75)) {
        Type[chan] = 0x80;
    }
    else {
        type = Type[chan] = 0x80;
    }
    
    TypeTime[chan] = __OSGetSystemTime();
    SITransfer(chan, &cmdTypeAndStatus, 1, &Type[chan], 3, GetTypeCallback, OSMicrosecondsToTicks(65));
    OSRestoreInterrupts(enabled);
    return type;
}