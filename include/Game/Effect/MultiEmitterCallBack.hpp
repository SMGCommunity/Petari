#pragma once

#include "Game/Util/Color.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JParticle/JPAEmitterCallBack.hpp>

enum {
    FORCE_FOLLOW_ON = 0x1,
    FORCE_FOLLOW_OFF = 0x2,
    FORCE_SCALE_ON = 0x10,
    RESET_FOLLOW_CURRENT = 0x40,
    BASE_SCALE = 0x100
};

class MultiEmitter;

class MultiEmitterCallBackBase : public JPAEmitterCallBack {
public:
    MultiEmitterCallBackBase() {};

    virtual ~MultiEmitterCallBackBase() {};
    virtual void init(JPABaseEmitter*) {};
};

class MultiEmitterCallBack : public MultiEmitterCallBackBase {
public:
    struct FlagSRT {
        bool mScale;
        bool mRotation;
        bool mTranslation;

        inline bool isTransform() const {
            return mScale || mRotation || mTranslation ? true : false;
        }
    };

    MultiEmitterCallBack(const MultiEmitter*, const TVec3f&);
    MultiEmitterCallBack(const MultiEmitter*, const TVec3f*, const TVec3f*, const TVec3f*, const TVec3f&);
    MultiEmitterCallBack(const MultiEmitter*, MtxPtr, const TVec3f&);
    MultiEmitterCallBack(const MultiEmitter*, MtxPtr, const TVec3f*, const TVec3f&);

    void execute(JPABaseEmitter*);
    void setHostSRT(const TVec3f*, const TVec3f*, const TVec3f*);
    void setHostMtx(MtxPtr);
    void setBaseScale(f32);
    void forceFollowOn();
    void forceFollowOff();
    void forceScaleOn();
    void resetFollowCurrent();
    void init(JPABaseEmitter*);
    void setSRTFromHostMtx(JPABaseEmitter*, MtxPtr, const FlagSRT&, bool);
    void setSRTFromHostSRT(JPABaseEmitter*, const FlagSRT&, bool);
    void setScaleFromHostScale(JPABaseEmitter*, const TVec3f&, bool, bool);
    bool effectLight(JPABaseEmitter*) NO_INLINE;
    void followSRT(JPABaseEmitter*, bool);
    void setColor(JPABaseEmitter*);
    void isFollowSRT(FlagSRT*, bool) const;

    inline bool isFlagOn(u16 flag) const {
        u16 mask = mFlags & flag;
        return mask == flag;
    }

    inline bool isFlagOff(u16 flag) const {
        u16 mask = mFlags & flag;
        return mask != flag;
    }

    inline void turnFlagOn(u16 flag) {
        mFlags |= flag;
    }

    inline void turnFlagOff(u16 flag) {
        mFlags &= ~flag;
    }

    /* 0x04 */ const MultiEmitter* mEmitter;
    /* 0x08 */ const TVec3f* mScale;
    /* 0x0C */ const TVec3f* mRotation;
    /* 0x10 */ const TVec3f* mTranslation;
    /* 0x14 */ MtxPtr mMtx;
    /* 0x18 */ TVec3f _18;
    /* 0x24 */ f32 mBaseScale;
    /* 0x28 */ Color8 mPrmColor;
    /* 0x2C */ Color8 mEnvColor;
    /* 0x30 */ u16 mFlags;
};
