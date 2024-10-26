#include "Game/Boss/BossKameckMoveRail.hpp"

BossKameckMoveRail::BossKameckMoveRail(const char *pName) : LiveActor(pName) {
    _8C = 0;
    _90 = 0;
}

void BossKameckMoveRail::init(const JMapInfoIter &rIter) {
    MR::getJMapInfoArg0WithInit(rIter, &_8C);
    initRailRider(rIter);
    makeActorDead();
    _90 = 0;
    s32 pointNum = MR::getRailPointNum(this);
    for (s32 i = 0; i < pointNum; i++) {
        s32 val = -1;
        MR::getRailPointArg0WithInit(this, i, &val);

        if (val != 0) {
            _90++;
        }
    }
}

void BossKameckMoveRail::calcMovePosition(TVec3f *pPos, s32 idx) const {
    MR::calcRailPointPos(pPos, this, idx);
}

s32 BossKameckMoveRail::calcRandomIndex(s32 idx) const {
    s32 pointNum = MR::getRailPointNum(this);
    s32 last = pointNum - 1;

    if (idx < 0) {
        last = pointNum;
    }

    if (last <= 0) {
        return 0;
    }

    s32 rand = MR::getRandom(0, last);
    if (idx >= 0 && rand >= idx) {
        rand++;
    }

    return rand;
}

s32 BossKameckMoveRail::calcRandomStoppableIndex(s32 idx) const {
    s32 last = _90 - 1;

    if (idx < 0) {
        last = _90;
    }

    s32 rand = (last <= 0) ? 0 : MR::getRandom(0, last);
    s32 pointNum = MR::getRailPointNum(this);
    s32 cpy = pointNum;
    s32 val = 0;

    for (s32 i = 0; i < cpy; i++) {
        if (i != idx) {
            s32 pointArg = -1;
            MR::getRailPointArg0WithInit(this, i, &pointArg);

            if (pointArg != 0) {
                if (val == rand) {
                    return i;
                }

                val++;
            }
        }
    }

    return 0;
}

BossKameckMoveRail::~BossKameckMoveRail() {

}
