#include "Game/Player/Mario.hpp"

void Mario::initMember()
{
    _40.zero();
    _4C.zero();
    PSMTXIdentity(_F4.toMtxPtr());
    _130.zero();
    _16C.zero();
    _178.zero();
    _1B4.zero();
    _274 = 0;
    _280 = 0.0f;
    _284.zero();
    _2B8.zero();
    _2C4.zero();
    _304.zero();
    _31C.zero();

    _340 = 0.0f;
    _3A4.zero();
    _3B0.zero();

    _422 = 0;
    _42A = 0;
    _438 = 0;
    _448.zero();

    _454 = 0.0f;
    _48C.zero();
    _498.zero();
    _4BC.zero();
    _50C.zero();
    _518.zero();
    _600.zero();
    _60C = 0;
    _638 = 0;
    _660.zero();
    _688.zero();
    _694.zero();

    _6CC = 0.0f;
    _6C8 = 0.0f;
    _6D0.zero();
    _6DC.zero();
    _6E8.zero();
    _6F4.zero();
    _700.zero();
    _70C.zero();

    _718 = 0.0f;
    _71D = 0;
    _71F = 0;
    _720 = 0;
    _724 = 0;
    _728 = 0;

    _75C.zero();
    _790.zero();
    _7C4.zero();
    _7D4.zero();
    PSMTXIdentity(_7E4.toMtxPtr());
    _814.zero();
    PSMTXIdentity(_824.toMtxPtr());

    _8B0.zero();
    _8BC.zero();
    _8DC.zero();

    _8EC = 0;
    _8F4 = 0.0f;

    for (int i = 0; i < 11; i++) {
        _A08[i] = 0;
    }

    _A34 = 0;
    _A3C = 0;

    _A40.zero();
    _A4C.zero();
    _A58.zero();
    _A64 = 0f;

    for (int i = 0; i < 0x20; i++) {
        _A6C[i] = 0;
    }
}
