#include "Game/Effect/SimpleEffectObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace NrvSimpleEffectObj {
    NEW_NERVE(HostTypeWait, SimpleEffectObj, Wait);
    NEW_NERVE(HostTypeMove, SimpleEffectObj, Move);
};  // namespace NrvSimpleEffectObj

namespace {
    const s_effectDataTable sEffectDataTable[12] = {
        {"IceLayerBreak", "SE_OJ_ICE_LAYER_BREAK", nullptr, 0, "最強", 7},
        {"LavaVolcanoEruption", nullptr, nullptr, 0, "最強", 0},
        {"LavaHomeVolcanoFallingRock", nullptr, "SE_AT_LV_FALLING_ROCK", -1, nullptr, 0},
        {"WaterLayerBreak", "SE_OJ_WATER_LAYER_BREAK", nullptr, 0, "最強", 7},
        {"DrainPipeBubbleA", nullptr, "SE_OJ_LV_PIPE_BUBBLE_A", -1, nullptr, 0},
        {"DrainPipeBubbleB", nullptr, "SE_OJ_LV_PIPE_BUBBLE_B", -1, nullptr, 0},
        {"StarPieceClusterBreak", "SE_OJ_CRYSTAL_CAGE_L_BREAK", nullptr, 0, "最強", 7},
        {"BattleShipExplosionMetal", "SE_AT_BTLSHIP_EXPLODE_METAL", nullptr, -1, nullptr, 0},
        {"BattleShipExplosionRock", "SE_AT_BTLSHIP_EXPLODE_ROCK", nullptr, -1, nullptr, 0},
        {"Steam", nullptr, "SE_OJ_LV_HD_STEAM", -1, nullptr, 0},
        {"IcicleRockLight", nullptr, "SE_OJ_LV_ICICLE_ROCK_LIGHT", -1, nullptr, 0},
        {"UFOKinokoLandingBlackSmoke", nullptr, "SE_OJ_LV_UFO_KINOKO_SMOKE", -1, nullptr, 0},
    };
    const s_effectDataTable* findDataElement(const char* pString) {
        u32 i = 0;
        u32 v2 = 0;
        for (i = 0; i < 12; i++) {
            const s_effectDataTable* table = &sEffectDataTable[v2];
            if (MR::isEqualString(pString, table->_0)) {
                return table;
            }
            v2 += 1;
        }
        return nullptr;
    }
};  // namespace

SimpleEffectObj::SimpleEffectObj(const char* pName) : LiveActor(pName) {
    _8C = nullptr;
}

void SimpleEffectObj::init(const JMapInfoIter& rIter) {
    TVec3f stack_8;
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneMapObjMovement(this);
    MR::getObjectName(&_8C, rIter);
    initEffectKeeper(0, _8C, false);
    initSound(4, false);
    initNerve(&NrvSimpleEffectObj::HostTypeWait::sInstance);
    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnOffA(this, MR::Functor(this, &SimpleEffectObj::setStateMove), MR::Functor(this, &SimpleEffectObj::setStateWait));
    } else {
        setNerve(&NrvSimpleEffectObj::HostTypeMove::sInstance);
    }
    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoActionFunctor(this, MR::Functor(this, &SimpleEffectObj::setStateMove), nullptr);
    } else {
        MR::registerDemoSimpleCastAll(this);
    }
    PSVECAdd(getClippingCenterOffset(), &stack_8, &stack_8);
    _90.set(stack_8);
    MR::setClippingTypeSphere(this, getClippingRadius(), &_90);
    MR::setGroupClipping(this, rIter, 0x40);
    MR::setClippingFar(this, getFarClipDistance());
    MR::joinToGroupArray(this, rIter, nullptr, 0x20);
    makeActorAppeared();
}

bool SimpleEffectObj::isStateMove() const {
    return isNerve(&NrvSimpleEffectObj::HostTypeMove::sInstance);
}

void SimpleEffectObj::startClipped() {
    LiveActor::startClipped();
    if (isSyncClipping() && isNerve(&NrvSimpleEffectObj::HostTypeMove::sInstance)) {
        MR::deleteEffectAll(this);
    }
}

void SimpleEffectObj::endClipped() {
    LiveActor::endClipped();
    if (isSyncClipping() && isNerve(&NrvSimpleEffectObj::HostTypeMove::sInstance)) {
        MR::emitEffect(this, _8C);
    }
}

void SimpleEffectObj::setStateWait() {
    MR::deleteEffect(this, _8C);
    setNerve(&NrvSimpleEffectObj::HostTypeWait::sInstance);
}

void SimpleEffectObj::setStateMove() {
    setNerve(&NrvSimpleEffectObj::HostTypeMove::sInstance);
}

void SimpleEffectObj::exeMove() {
    const s_effectDataTable* v1;
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, _8C);
        v1 = ::findDataElement(_8C);
        s32 v4;
        if (v1) {
            v4 = v1->_C;
        } else {
            v4 = -1;
        }
        if (v4 != -1 && v4 != -1) {
            if (v4 == 0) {
                MR::shakeCameraStrong();
            } else if (v4 == 1) {
                MR::shakeCameraNormal();
            } else if (v4 == 2) {
                MR::shakeCameraWeak();
            }
        }
        v1 = ::findDataElement(_8C);
        const char* Sound;
        if (v1) {
            Sound = v1->_4;
        } else {
            Sound = nullptr;
        }
        if (Sound) {
            MR::startSound(this, Sound, -1, -1);
        }
    }
    v1 = ::findDataElement(_8C);
    const char* levelSound;
    if (v1) {
        levelSound = v1->_8;
    } else {
        levelSound = nullptr;
    }
    if (levelSound) {
        MR::startLevelSound(this, levelSound, -1, -1, -1);
    }
    v1 = ::findDataElement(_8C);
    s32 v13;
    if (v1) {
        v13 = v1->_14;
    } else {
        v13 = -1;
    }
    if (v13 < 0) {
        return;
    }
    if (MR::isStep(this, v13)) {
        v1 = ::findDataElement(_8C);
        const char* v15;
        if (v1) {
            v15 = v1->_10;
        } else {
            v15 = nullptr;
        }
        if (v15) {
            MR::tryRumblePad(this, v15, 0);
        }
    }
}

inline void SimpleEffectObj::exeWait() {}
