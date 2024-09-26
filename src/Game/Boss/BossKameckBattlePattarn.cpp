#include "Game/Boss/BossKameckBattlePattarn.hpp"

BossKameckBattlePattarn::BossKameckBattlePattarn(s32 *a1, bool a2) {
    _10 = a2;
    _0 = nullptr;
    _4 = 0;
    _8 = 0;
    _C = 0;
    initPattarn(a1);
}

void BossKameckBattlePattarn::initPattarn(s32 *a1) {
    _0 = a1;
    u32 v2 = 0;

    for (s32 i = 0; a1[i] != -1; i++) {
        v2++;
    }

    _4 = v2;
    _8 = v2;
    _C = 0;
}

s32 BossKameckBattlePattarn::goNextPattarn() {
    u32 v1 = _C;
    s32 next = _0[v1];
    _C++;

    if (_4 <= _C) {
        _C = 0;
    }

    return next;
}