#include "Game/Util/JointRumbler.hpp"
#include "Game/Animation/XanimeCore.hpp"
#include "Game/Animation/XanimePlayer.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/RumbleCalculator.hpp"
#include "math_types.hpp"
#include "revolution/types.h"

JointRumbler::JointRumbler(LiveActor* pActor, const char* pJointName, f32 a3, f32 a4, u32 a5, bool a6, s32 a7)
    : mRumbleCalculator(), mJointTransform(), _8(a7), _C(), mModelScale(1.0f), mFlags() {
    for (int i = 0; i < ARRAY_SIZE(_16); i++) {
        _16[i] = false;
    }

    if (pActor->mModelManager->mXanimePlayer->getCore()->mTransformList == nullptr) {
        MR::initJointTransform(pActor);
    }

    mJointTransform = MR::getJointTransform(pActor, pJointName);
    mRumbleCalculator = new RumbleCalculatorCosMultLinear(a3, HALF_PI, a4, a5);

    if (a6) {
        mFlags |= FLAG_1;
    }
}

void JointRumbler::update() {
    f32 scale;

    if (_C == 0) {
        mRumbleCalculator->calc();
        scale = mRumbleCalculator->_C.y;
    } else {
        if (_C > 0) {
            _C--;
        }

        scale = 0.0f;
    }

    updateModelScale(scale + 1.0f);
}

void JointRumbler::start() {
    mRumbleCalculator->start(0);
    _C = _8;
}

void JointRumbler::reset() {
    mRumbleCalculator->reset();
    _C = -1;
    updateModelScale(1.0f);
}

bool JointRumbler::isRumbling() const {
    return mRumbleCalculator->_4 < mRumbleCalculator->_8;
}

void JointRumbler::updateModelScale(f32 scale) {
    if (testFlag(FLAG_1)) {
        updateScale(&mJointTransform->_14, scale);
    } else {
        updateScale(&mJointTransform->mScale, scale);
    }

    mModelScale = scale;
}

void JointRumbler::updateScale(TVec3f* pVec, f32 scale) {
    if (!testFlag(FLAG_LOCK_X)) {
        pVec->x = scale;
    }

    if (!testFlag(FLAG_LOCK_Y)) {
        pVec->y = scale;
    }

    if (!testFlag(FLAG_LOCK_Z)) {
        pVec->z = scale;
    }
}
