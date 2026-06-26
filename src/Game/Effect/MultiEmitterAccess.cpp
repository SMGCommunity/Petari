#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Effect/MultiEmitterCallBack.hpp"
#include "Game/Effect/SingleEmitter.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JParticle/JPAEmitter.hpp"

void MultiEmitter::setGlobalRotation(const TVec3s& rRotation, s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->setGlobalRotation(rRotation);
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->setGlobalRotation(rRotation);
    }
}

void MultiEmitter::setGlobalTranslation(const TVec3f& rPosition, s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->mEmitter->mGlobalTrs.set(rPosition);
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->mEmitter->mGlobalTrs.set(rPosition);
    }
}

void MultiEmitter::setGlobalScale(const TVec3f& rScale, s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->setGlobalScale(rScale);
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->setGlobalScale(rScale);
    }
}

void MultiEmitter::setGlobalSRTMatrix(const MtxPtr pMtx, s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->setGlobalSRTMatrix(pMtx);
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->setGlobalSRTMatrix(pMtx);
    }
}

void MultiEmitter::setGlobalPrmColor(u8 r, u8 g, u8 b, s32 idx) {
    bool b1 = MR::isNearZero(_2C);

    if (idx == -1) {
        _1C->mPrmColor.set(r, g, b, -1);

        if (!b1) {
            for (s32 i = 0; i < _0.size(); i++) {
                SingleEmitter* pEmitter = getValidEmitter(i, false);
                if (pEmitter != nullptr) {
                    pEmitter->mEmitter->setGlobalPrmColor(r, g, b);
                }
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->setGlobalPrmColor(r, g, b);
    }
}

void MultiEmitter::setGlobalEnvColor(u8 r, u8 g, u8 b, s32 idx) {
    bool b1 = MR::isNearZero(_2C);

    if (idx == -1) {
        _1C->mEnvColor.set(r, g, b, -1);

        if (!b1) {
            for (s32 i = 0; i < _0.size(); i++) {
                SingleEmitter* pEmitter = getValidEmitter(i, false);
                if (pEmitter != nullptr) {
                    pEmitter->mEmitter->setGlobalEnvColor(r, g, b);
                }
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->setGlobalEnvColor(r, g, b);
    }
}

void MultiEmitter::setLocalScale(const TVec3f& rScale, s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->mEmitter->mLocalScl.set(rScale);
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->mEmitter->mLocalScl.set(rScale);
    }
}

void MultiEmitter::setDirectionalSpeed(f32 speed, s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->mEmitter->setDirectionalSpeed(speed);
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->mEmitter->setDirectionalSpeed(speed);
    }
}

void MultiEmitter::setRate(f32 rate, s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->mEmitter->setRate(rate);
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->mEmitter->setRate(rate);
    }
}

void MultiEmitter::stopCalcEmitter(s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->mEmitter->stopCalcEmitter();
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->mEmitter->stopCalcEmitter();
    }
}

void MultiEmitter::playCalcEmitter(s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->mEmitter->playCalcEmitter();
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->mEmitter->playCalcEmitter();
    }
}

void MultiEmitter::stopDrawParticle(s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->mEmitter->stopDrawParticle();
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->mEmitter->stopDrawParticle();
    }
}

void MultiEmitter::playDrawParticle(s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->mEmitter->playDrawParticle();
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->mEmitter->playDrawParticle();
    }
}

void MultiEmitter::setParticleCallBackPtr(JPAParticleCallBack* pCallBack, s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->mEmitter->setParticleCallBackPtr(pCallBack);
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->mEmitter->setParticleCallBackPtr(pCallBack);
    }
}

void MultiEmitter::pauseOff(s32 idx) {
    if (idx == -1) {
        for (s32 i = 0; i < _0.size(); i++) {
            SingleEmitter* pEmitter = getValidEmitter(i, false);
            if (pEmitter != nullptr) {
                pEmitter->mEmitter->pauseOff();
            }
        }

        return;
    }

    SingleEmitter* pEmitter = getValidEmitter(idx, false);
    if (pEmitter != nullptr) {
        pEmitter->mEmitter->pauseOff();
    }
}

void ParticleEmitter::setGlobalRotation(const TVec3s& rRotation) {
    mEmitter->setGlobalRotation(rRotation);
}

void ParticleEmitter::setGlobalScale(const TVec3f& rScale) {
    mEmitter->setGlobalScale(rScale);
}

void ParticleEmitter::setGlobalSRTMatrix(const MtxPtr pMtx) {
    mEmitter->setGlobalSRTMatrix(pMtx);
}

void ParticleEmitter::setGlobalPrmColor(u8 r, u8 g, u8 b) {
    mEmitter->setGlobalPrmColor(r, g, b);
}

void ParticleEmitter::setGlobalEnvColor(u8 r, u8 g, u8 b) {
    mEmitter->setGlobalEnvColor(r, g, b);
}
