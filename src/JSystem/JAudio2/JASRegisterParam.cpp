#include "JSystem/JAudio2/JASRegisterParam.hpp"
#include "JSystem/JSupport/JSupport.hpp"

JASRegisterParam::JASRegisterParam() {
    for (s32 i = 0; i < 14; i++) {
        _0[i] = 0;
    }
}

void JASRegisterParam::init() {
    for (s32 i = 0; i < 14; i++) {
        _0[i] = 0;
    }
}

void JASRegisterParam::write(JASRegisterParam::RegID regID, u32 writeValue) {
    if (regID < 14) {
        if (regID <= 2) {
            _0[regID] = (u8)writeValue;
        } else {
            _0[regID] = writeValue;
        }
        return;
    }

    switch (regID) {
    case 32:
        _0[0] = JSUHiByte(writeValue);
        _0[1] = JSULoByte(writeValue);
        break;
    case 33:
        _0[4] = JSUHiHalf(writeValue);
        _0[5] = JSULoHalf(writeValue);
        break;
    case 34:
        _0[4] = u8(_0[4]);
        _0[4] |= u16((writeValue & 0xff) << 8);
        break;
    case 35:
        _0[4] &= 0xff00;
        _0[4] |= u8(writeValue);
        break;
    case 36:
        _0[5] = u8(_0[5]);
        _0[5] |= u16((writeValue & 0xff) << 8);
        break;
    case 37:
        _0[5] &= 0xff00;
        _0[5] |= u8(writeValue);
        break;
    case 38:
        _0[6] = JSUHiHalf(writeValue);
        _0[7] = JSULoHalf(writeValue);
        break;
    case 39:
        _0[8] = JSUHiHalf(writeValue);
        _0[9] = JSULoHalf(writeValue);
        break;
    case 40:
        _0[10] = JSUHiHalf(writeValue);
        _0[11] = JSULoHalf(writeValue);
        break;
    case 41:
        _0[12] = JSUHiHalf(writeValue);
        _0[13] = JSULoHalf(writeValue);
        break;
    }
}

u32 JASRegisterParam::read(JASRegisterParam::RegID regID) {
    if (regID < 14) {
        return _0[regID];
    }

    u32 result = 0;
    switch (regID) {
    case 32:
        result = ((_0[0] & 0xff) << 8) | (_0[1] & 0xff);
        break;
    case 33:
        result = _0[4];
        result <<= 16;
        result |= _0[5];
        break;
    case 34:
        result = JSUHiByte(_0[4]);
        break;
    case 35:
        result = JSULoByte(_0[4]);
        break;
    case 36:
        result = JSUHiByte(_0[5]);
        break;
    case 37:
        result = JSULoByte(_0[5]);
        break;
    case 38:
        result = _0[6];
        result <<= 16;
        result |= _0[7];
        break;
    case 39:
        result = _0[8];
        result <<= 16;
        result |= _0[9];
        break;
    case 40:
        result = _0[10];
        result <<= 16;
        result |= _0[11];
        break;
    case 41:
        result = _0[10];
        result <<= 16;
        result |= _0[11];
        break;
    }

    return result;
}
