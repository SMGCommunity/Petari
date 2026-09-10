#include "revolution/nwc24.h"

static const u8 TtableInv[16] = {13, 5, 9, 7, 0, 15, 10, 2, 12, 3, 14, 1, 8, 6, 11, 4};
static const u8 ExcTable[8] = {1, 5, 0, 4, 2, 3, 6, 7};

unsigned long long getUnScrambleId(u64 v);

static int checkCRC(u64 v) {
    int i;

    for (i = 0; i < 43; i++) {
        if (((v >> (53 - (i + 1))) & 1) != 0)
            v ^= 1589ULL << (42 - i);
    }

    return v != 0;
}

NWC24Err NWC24iCheckUserIdCRC(NWC24UserId userId) {
    u64 idDecoded = getUnScrambleId(userId);
    if (!checkCRC(idDecoded)) {
        return NWC24_OK;
    }

    return NWC24_ERR_ID_CRC;
}

static u8 getbyte(u64 value, u8 index) {
    return (value >> (index * 8)) & 0xFF;
}

static u64 setbyte(u64 value, u8 index, u8 set) {
    return (value & ~(0xFFULL << ((u64)8 * index))) | ((u64)set << ((u64)8 * index));
}

u64 getUnScrambleId(u64 v) {
    u64 uidtemp;
    u8 i;

    v &= 0x1FFFFFFFFFFFFFULL;
    v ^= 0x5E5E5E5E5E5EULL;
    v &= 0x1FFFFFFFFFFFFFULL;
    v |= (((v & 0xFF) << 5) & 0x20) << 48;
    v >>= 1;

    uidtemp = v;
    for (i = 0; i < 6; ++i) {
        v = setbyte(v, i, getbyte(uidtemp, ExcTable[i]));
    }

    for (i = 0; i < 6; ++i) {
        v = setbyte(v, i, (TtableInv[(getbyte(v, i) >> 4) & 0xF] << 4) | TtableInv[getbyte(v, i) & 0xF]);
    }

    v = ((v & 0x7FFFFFFFFFFULL) << 10) | ((v >> 43) & 0x3FF);
    v ^= 0xB3B3B3B3B3B3ULL;
    return v;
}
