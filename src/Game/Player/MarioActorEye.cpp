#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DAnimation.hpp"

extern "C" {
    extern u8 lbl_805BA740[];
    extern char lbl_805BA74A[];
}

void MarioActor::initBlink() {
    _B72 = 0;
    _B74 = 0;
    _B68 = 0;
    mEyeRes = nullptr;
}

void MarioActor::setBlink(const char* pName) {
    if (pName == nullptr) {
        mEyeRes = nullptr;
    }
    else if (MR::getResourceHolder(this)->mBtpResTable->isExistRes(pName)) {
        J3DAnmTexPattern* pPattern = static_cast< J3DAnmTexPattern* >(MR::getResourceHolder(this)->mBtpResTable->getRes(pName));
        mEyeRes = pPattern;
        static_cast< J3DAnmTexPattern* >(mEyeRes)->mFrame = 0.0f;
    }
    else {
        mEyeRes = nullptr;
    }
}

void MarioActor::updateBlink() {
    if (mCurrModel == 1) {
        return;
    }

    J3DAnmTexPattern* pPattern = static_cast< J3DAnmTexPattern* >(mEyeRes);
    if (pPattern) {
        f32 frame = 0.0f;

        if (pPattern->mAttribute == 2) {
            frame = pPattern->mFrame + 1.0f;
            if (frame >= static_cast< f32 >(pPattern->mFrameMax) - 1.0f) {
                frame = 0.0f;
            }
        }
        else {
            frame = mMarioAnim->getFrame();
        }

        pPattern->mFrame = frame;

        u16 eyeLidMaterial = static_cast< u16 >(MR::getMaterialNo(MR::getJ3DModelData(this), lbl_805BA74A));
        for (u16 i = 0; i < pPattern->mUpdateMaterialNum; i++) {
            u16 texNo = 0;
            pPattern->getTexNo(i, &texNo);
            if (pPattern->mUpdateMaterialID[i] == eyeLidMaterial) {
                _B68 = 1;
                _B6A = texNo - _B70;
                return;
            }
        }
    }

    if (isNerve(&NrvMarioActor::MarioActorNrvGameOver::sInstance) && !mEyeRes) {
        if (mMario->isAnimationTerminate(nullptr)) {
            _B6A = 2;
        }
        else {
            _B6A = 1;
        }
        return;
    }

    if (_B68) {
        _B68 = 0;
        _B72 = 0;
        _B74 = MR::getRandom(60L, 360L);
        _B6A = 0;
    }

    if (_B72) {
        u8 blinkStep = _B72 - 1;
        _B72 = blinkStep;
        _B6A = lbl_805BA740[9 - blinkStep];
        _B74 = MR::getRandom(60L, 360L);
        return;
    }

    if (_B74 != 0) {
        _B74--;
    }

    if (_B74 == 0) {
        _B72 = 10;
    }
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
