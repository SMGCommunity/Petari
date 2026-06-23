#pragma once

#include "JSystem/JAudio2/JASSoundParams.hpp"

struct JAISoundParamsProperty {
    void init() {
        mVolume = 1.0f;
        mFxMix = 0.0f;
        mPitch = 1.0f;
    }

    /* 0x00 */ f32 mVolume;
    /* 0x04 */ f32 mFxMix;
    /* 0x08 */ f32 mPitch;
};

struct JAISoundParamsTransition {
    struct TTransition {
        void zero() {
            mStep = 0.0f;
            mRemainingSteps = 0;
            mTargetValue = 0.0f;
        }

        void set(f32 newValue, f32 currentValue, u32 maxSteps) {
            mRemainingSteps = maxSteps;
            mStep = (newValue - currentValue) / mRemainingSteps;
            mTargetValue = newValue;
        }

        f32 apply(f32 value) {
            if (mRemainingSteps > 1) {
                mRemainingSteps--;
                value += mStep;
            } else {
                if (mRemainingSteps == 1) {
                    mRemainingSteps = 0;
                    value = mTargetValue;
                }
            }
            return value;
        }

        /* 0x0 */ f32 mStep;
        /* 0x4 */ f32 mTargetValue;
        /* 0x8 */ u32 mRemainingSteps;
    };

    void init() {
        mVolume.zero();
        mPitch.zero();
        mFxMix.zero();
        mPan.zero();
        mDolby.zero();
    }

    void apply(JASSoundParams* params) {
        params->mVolume = mVolume.apply(params->mVolume);
        params->mPitch = mPitch.apply(params->mPitch);
        params->mFxMix = mFxMix.apply(params->mFxMix);
        params->mDolby = mDolby.apply(params->mDolby);
        params->mPan = mPan.apply(params->mPan);
    }

    /* 0x00 */ TTransition mVolume;
    /* 0x0C */ TTransition mPitch;
    /* 0x18 */ TTransition mFxMix;
    /* 0x24 */ TTransition mPan;
    /* 0x30 */ TTransition mDolby;
};

struct JAISoundParamsMove {
    JAISoundParamsMove() : mParams() {
    }

    void init() {
        mParams.init();
        mTransition.init();
    }

    void calc() {
        mTransition.apply(&mParams);
    }

    void moveVolume(f32 newValue, u32 maxSteps);
    void movePitch(f32 newValue, u32 maxSteps);
    void moveFxMix(f32 newValue, u32 maxSteps);
    void movePan(f32 newValue, u32 maxSteps);
    void moveDolby(f32 newValue, u32 maxSteps);

    /* 0x00 */ JASSoundParams mParams;
    /* 0x14 */ JAISoundParamsTransition mTransition;
};

struct JAISoundParams {
    JAISoundParams() : mMove() {
    }
    void mixOutAll(const JASSoundParams& inParams, JASSoundParams* outParams, f32);

    void init() {
        mMove.init();
        mProperty.init();
    }

    JASSoundParams& getMoveParams() {
        return mMove.mParams;
    }

    /* 0x0 */ JAISoundParamsProperty mProperty;
    /* 0xC */ JAISoundParamsMove mMove;
};
