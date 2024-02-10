#include "Game/Player/MarioEffect.hpp"

MarioEffect::MarioEffect(MarioActor *actor) : MarioModule(actor), _1C(0xFFFFFFFF), _20(0xFFFFFFFF)
{
    SomeData *i = _54;
    // SomeData *end = _54 + 0x100;
    do {
        PSMTXIdentity(i->_4.toMtxPtr());
        PSMTXIdentity(i->_34.toMtxPtr());
        i->_64 = 0;
        i->_68 = 0;
        i->_0 = 0;
        i++;
    } while (i < _54 + 0x100);

    _C = -1;
    _10 = -1;
    _14 = -1;
    _18 = 0;
    _1C.set(0, 0, 0, 0);
    _20.set(0, 0, 0, 0);
    PSMTXIdentity(_24.toMtxPtr());
}
