#include "revolution/nwc24.h"

static u8 TtableInv[16];
static u8 ExcTable[8];

unsigned long long getUnScrambleId(unsigned long long v);

static int checkCRC(u64 v) {
    int i;

    for (i = 0; i < 43; i++) {
        if (((v >> (53 - (i + 1))) & 1) != 0)
            v ^= 1589ULL << (42 - i);
    }

    return v != 0;
}

NWC24Err NWC24iCheckUserIdCRC(u64 userId) {
    u64 idDecoded = getUnScrambleId(userId);
    if (!checkCRC(idDecoded)) {
        return NWC24_OK;
    }

    return NWC24_ERR_ID_CRC;
}

static u8 getbyte(u64 value, u8 index) {
    return value >> (index * 8);
}

static u64 setbyte(u64 value, u8 index, u8 set) {
    u64 mask = 0xFFULL << (8 * index);
    return (value & ~mask) | ((u64)set << (8 * index));
}

u64 getUnScrambleId(u64 v) {
    u32 v1;
    u32 lo;
    u32 hi;
    u64 temp;
    u8 i;
    u8 byte;

    v1 = ((v >> 32) & 0x1FFFFF) ^ 0x5E5E;
    v1 |= ((v ^ 0x5E) & 1) << 21;

    lo = (v1 << 31) | ((v ^ 0x5E5E5E5E) >> 1);
    hi = v1 >> 1;

    temp = ((u64)hi << 32) | lo;

    for (i = 0; i < 6; ++i)
        temp = setbyte(temp, i, getbyte(temp, ExcTable[i]));

    for (i = 0; i < 6; ++i) {
        byte = getbyte(temp, i);
        byte = TtableInv[byte & 0xF] | (TtableInv[byte >> 4] << 4);
        temp = setbyte(temp, i, byte);
    }

    lo = temp;
    hi = temp >> 32;

    return (((u64)((lo >> 22) | ((hi & 0x7FF) << 10)) << 32) | ((lo << 10) | ((hi >> 11) & 0x3FF))) ^ 0xB3B3B3B3B3B3ULL;
}
