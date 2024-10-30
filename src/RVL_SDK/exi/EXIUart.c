#include <revolution/exi.h>

static s32 Chan;
static u32 Dev;
static u32 Enabled = 0;
static u32 BarnacleEnabled = 0;

static BOOL ProbeBarnacle(s32 chan, u32 dev, u32* revision) {
    BOOL err;
    u32 cmd;

    if (chan != 2 && dev == 0 && !EXIAttach(chan, NULL)) {
        return FALSE;
    }

    err = !EXILock(chan, dev, NULL);
    if (err == 0) {
        err = !EXISelect(chan, dev, 0);

        if (err == 0) {
            cmd = 0x20011300;
            err = FALSE;
            err |= !EXIImm(chan, &cmd, 4, 1, 0);
            err |= !EXISync(chan);
            err |= !EXIImm(chan, revision, 4, 0, 0);
            err |= !EXISync(chan);
            err |= !EXIDeselect(chan);
        }

        EXIUnlock(chan);
    }

    if (chan != 2 && dev == 0) {
        EXIDetach(chan);
    }

    if (err != 0) {
        return FALSE;
    }

    return (*revision != 0xFFFFFFFF) ? TRUE : FALSE;
}

void __OSEnableBarnacle(s32 chan, u32 dev) {
    u32 id;

    if (EXIGetID(chan, dev, &id)) {
        switch (id) {
            case 0xFFFFFFFF:
            case 4:
            case 8:
            case 0x10:
            case 0x20:
            case 0x01010000:
            case 0x01020000:
            case 0x02020000:
            case 0x03010000:
            case 0x04220000:
            case 0x04020100:
            case 0x04020200:
            case 0x04020300:
            case 0x04040404:
            case 0x04060000:
            case 0x04120000:
            case 0x04130000:
            case 0x80000004:
            case 0x80000008:
            case 0x80000010:
            case 0x80000020:
                break;
            default:
                if (ProbeBarnacle(chan, dev, &id)) {
                    Chan = chan;
                    Dev = dev;
                    Enabled = BarnacleEnabled = 0xa5FF005A;
                }

                break;
        }
    }
}