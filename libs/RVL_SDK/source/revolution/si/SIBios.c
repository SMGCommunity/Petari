#include <revolution/si.h>
#include <revolution/os.h>
#include <private/flipper.h>
#include <private/io_reg.h>

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

static void GetTypeCallback(s32 chan, u32 error, OSContext* context) {
    return;
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