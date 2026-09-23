#include "Game/MapObj/OceanWaveFloater.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/MapObj/FloaterFloatingForceTypeSpring.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/WaveFloatingForce.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "math_types.hpp"
#include <revolution/types.h>

namespace {
    struct Param {
        /* 0x00 */ const char* const mObjectName;
        /* 0x04 */ f32 mAmplitude;
        /* 0x08 */ u32 mStepCount;
        /* 0x0C */ f32 mSinkDepth;
        /* 0x10 */ f32 mRippleHeight;
        /* 0x14 */ bool mIsUseProjmap;
    };

    static const Param sDataTable[] = {
        {"OceanPierFloaterA", 30.0f, 300, 140.0f, 120.0f, false},
        {"OceanHexagonFloater", 50.0f, 330, 150.0f, 100.0f, false},
    };

    static const s32 sWaveSeStepsMin = 60;
    static const s32 sWaveSeStepsMax = 120;

    const Param* getParam(const char* pChar) {
        for (u32 i = 0; i < ARRAY_SIZE(::sDataTable); i++) {
            if (MR::isEqualString(pChar, ::sDataTable[i].mObjectName)) {
                return &::sDataTable[i];
            }
        }

        return nullptr;
    }
};  // namespace

OceanWaveFloater::OceanWaveFloater(const char* pName) : MapObjActor(pName), mSpring(), mForce(), mRipplePos(gZeroVec), mSoundDelay(), mCanRipple() {
}

void OceanWaveFloater::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);

    mForce = new WaveFloatingForce(this, ::getParam(mObjectName)->mStepCount, ::getParam(mObjectName)->mAmplitude, true);

    MapObjActorInitInfo info = MapObjActorInitInfo();
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(nullptr);
    info.setupSound(4);
    info.setupNoAppearRiddleSE();

    if (::getParam(mObjectName)->mIsUseProjmap) {
        info.setupProjmapMtx(false);
    }

    initialize(rIter, info);

    mRipplePos.set(mPosition);

    MR::setEffectHostSRT(this, "Ripple", &mRipplePos, nullptr, nullptr);

    mSoundDelay = MR::getRandom(::sWaveSeStepsMin, ::sWaveSeStepsMax);

    mSpring = new FloaterFloatingForceTypeSpring(this, mObjectName);
    mSpring->init(rIter);
    mSpring->start();

    MR::onCalcGravity(this);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorAppeared();
    } else {
        appear();
    }
}

void OceanWaveFloater::appear() {
    MapObjActor::appear();
    MR::emitEffect(this, "Ripple");
}

void OceanWaveFloater::initAfterPlacement() {
    MR::calcMapGroundUpper(&mMapGroundUpper, this);
    mOffset = mPosition.distance(mMapGroundUpper);

    if ((mPosition - mMapGroundUpper).dot(mGravity) < 0.0f) {
        mOffset *= -1.0f;
    }

    if (mMatrixSetter != nullptr) {
        TVec3f offset(0.0f, mOffset, 0.0f);
        mMatrixSetter->updateMtxUseBaseMtxWithLocalOffset(TVec3f(0.0f, mOffset, 0.0f));
    }
}

void OceanWaveFloater::startClipped() {
    MR::deleteEffect(this, "Ripple");
    MapObjActor::startClipped();
}

void OceanWaveFloater::endClipped() {
    MapObjActor::endClipped();
    MR::emitEffect(this, "Ripple");
}

void OceanWaveFloater::control() {
    mSpring->movement();
    mSpring->updateHostVelocity(&mVelocity);

    mForce->update();

    controlEffect();
    mSoundDelay--;
    if (mSoundDelay <= 0) {
        MR::startSound(this, "SE_OJ_PIER_FLOATER_WAVE");
        mSoundDelay = MR::getRandom(::sWaveSeStepsMin, ::sWaveSeStepsMax);
    }
}

void OceanWaveFloater::calcAndSetBaseMtx() {
    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.setRotateDegree(mRotation);
    TVec3f offs = mGravity * mForce->getCurrentValue();
    baseMtx.setTrans(mPosition - offs);
    MR::setBaseTRMtx(this, baseMtx);
}

f32 OceanWaveFloater::getCurrentSinkDepth() const {
    TPos3f baseMtx = getBaseMtx();

    TVec3f trans;
    baseMtx.getTrans(trans);

    TVec3f yDir;
    baseMtx.getYDir(yDir);

    f32 sign = MR::sign(yDir.dot(mRipplePos - trans));
    return sign * mRipplePos.distance(trans);
}

void OceanWaveFloater::controlEffect() {
    f32 paramVal = ::getParam(mObjectName)->mSinkDepth;
    if (paramVal < getCurrentSinkDepth() && !mCanRipple) {
        MR::deleteEffect(this, "Ripple");
        mCanRipple = true;
    } else if (getCurrentSinkDepth() < ::getParam(mObjectName)->mRippleHeight && mCanRipple) {
        MR::emitEffect(this, "Ripple");
        mCanRipple = false;
    }
}
