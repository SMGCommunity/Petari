#include "Game/MapObj/SandUpDownEffectObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/TriangleFilter.hpp"

namespace {
    struct LineCheckParam {
        /* 0x00 */ const char* mObjName;
        /* 0x04 */ f32 _4;
        /* 0x08 */ f32 _8;
    };

    static const LineCheckParam sLineCheckParamTable[] = {
        {"SandUpDownColumnAEffect", 800.0f, 1600.0f}, {"SandUpDownKGBaseEffect", 5000.0f, 2000.0f}, {"SandUpDownTowerTopEffect", 3500.0f, 3550.0f},
        {"UFOSandObstacleAEffect", 400.0f, 800.0f},   {"UFOSandObstacleBEffect", 200.0f, 400.0f},   {"UFOSandObstacleCEffect", 400.0f, 800.0f},
    };
};  // namespace

namespace {
    const LineCheckParam* findLineCheckParamData(const char* pObjName) {
        for (u32 i = 0; i < ARRAY_SIZE(sLineCheckParamTable); i++) {
            const LineCheckParam* param = &sLineCheckParamTable[i];

            if (MR::isEqualString(pObjName, param->mObjName)) {
                return param;
            }
        }

        return nullptr;
    }

    bool isNotGroundCodeNoStampSand(const Triangle* pTriangle) {
        return !MR::isGroundCodeNoStampSand(pTriangle);
    }
};  // namespace

namespace NrvSandUpDownEffectObj {
    NEW_NERVE(SandUpDownEffectObjNrvWait, SandUpDownEffectObj, Wait);
    NEW_NERVE(SandUpDownEffectObjNrvFallingRock, SandUpDownEffectObj, FallingRock);
    NEW_NERVE(SandUpDownEffectObjNrvFallingRockAndSmoke, SandUpDownEffectObj, FallingRockAndSmoke);
    NEW_NERVE(SandUpDownEffectObjNrvBurySmokePrepare, SandUpDownEffectObj, BurySmokePrepare);
    NEW_NERVE(SandUpDownEffectObjNrvBurySmoke, SandUpDownEffectObj, BurySmoke);
    NEW_NERVE(SandUpDownEffectObjNrvBuryEnd, SandUpDownEffectObj, BuryEnd);
};  // namespace NrvSandUpDownEffectObj

SandUpDownEffectObj::SandUpDownEffectObj(const char* pName) : LiveActor(pName), _8C(), _90(), _94(0.0f, 0.0f, 0.0f), _D0() {
    _A0.identity();
}

void SandUpDownEffectObj::init(const JMapInfoIter& rIter) {
    const char* objName;

    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::getObjectName(&objName, rIter);
    initEffectKeeper(0, objName, false);
    MR::makeMtxTR(_A0.toMtxPtr(), this);
    MR::setEffectHostMtx(this, "BurySmoke", _A0.toMtxPtr());

    TVec3f yDir;
    _A0.getYDir(yDir);
    mGravity = -yDir;

    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);

    if (MR::isValidSwitchA(this)) {
        MR::listenStageSwitchOnA(this, MR::Functor(this, &SandUpDownEffectObj::startMoveDown));
    }

    MR::listenStageSwitchOnB(this, MR::Functor(this, &SandUpDownEffectObj::startMoveUp));

    _8C = ::findLineCheckParamData(objName)->_4;
    _90 = ::findLineCheckParamData(objName)->_8;
    _D0 = MR::createTriangleFilterFunc(&::isNotGroundCodeNoStampSand);
    _94.scaleAdd(_8C - _90 * 0.5f, yDir, mPosition);

    MR::setClippingTypeSphere(this, _90 * 0.5f + 100.0f, &_94);
    initNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvWait::sInstance);
    makeActorAppeared();
}

void SandUpDownEffectObj::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffectAll(this);
    }
}

void SandUpDownEffectObj::exeFallingRock() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "FallingRock");
    }

    bool isOnSwitchAOnly = MR::isOnSwitchA(this) && !MR::isOnSwitchB(this);

    if (isOnSwitchAOnly) {
        MR::deleteEffect(this, "FallingRock");
        setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvWait::sInstance);
    } else if (checkSandHit()) {
        setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvFallingRockAndSmoke::sInstance);
    }
}

void SandUpDownEffectObj::exeFallingRockAndSmoke() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "BurySmoke");
    }

    if (!checkSandHit()) {
        MR::deleteEffect(this, "BurySmoke");
        setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvFallingRock::sInstance);
    } else {
        bool isOnSwitchAOnly = MR::isOnSwitchA(this) && !MR::isOnSwitchB(this);

        if (isOnSwitchAOnly) {
            MR::deleteEffectAll(this);
            setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvWait::sInstance);
        }
    }
}

void SandUpDownEffectObj::exeBurySmokePrepare() {
    if (checkSandHit()) {
        setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvBurySmoke::sInstance);
    }
}

void SandUpDownEffectObj::exeBurySmoke() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "BurySmoke");
    }

    if (!checkSandHit()) {
        MR::deleteEffect(this, "BurySmoke");
        setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvBuryEnd::sInstance);
    } else if (isUfoTop()) {
        setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvWait::sInstance);
    }
}

void SandUpDownEffectObj::exeBuryEnd() {
    if (isUfoTop()) {
        setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvWait::sInstance);
    }
}

void SandUpDownEffectObj::startMoveUp() {
    setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvBurySmokePrepare::sInstance);
}

void SandUpDownEffectObj::startMoveDown() {
    setNerve(&NrvSandUpDownEffectObj::SandUpDownEffectObjNrvFallingRock::sInstance);
}

bool SandUpDownEffectObj::checkSandHit() {
    TVec3f vec1;
    TVec3f vec2;
    TVec3f hitPos;

    vec1.scaleAdd(-_8C, mGravity, mPosition);
    vec2.scale(_90, mGravity);

    bool isHit = MR::getFirstPolyOnLineToMap(&hitPos, nullptr, vec1, vec2, nullptr, _D0);

    if (isHit) {
        _A0.setTrans(hitPos);
    }

    return isHit;
}

bool SandUpDownEffectObj::isUfoTop() const {
    if (MR::isValidSwitchA(this)) {
        return !MR::isOnSwitchA(this) && MR::isOnSwitchB(this);
    }

    return MR::isOnSwitchB(this);
}
