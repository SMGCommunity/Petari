#pragma once

#include "Game/Util/Array.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class MultiEmitterCallBackBase;
class EffectSystem;
class ParticleEmitter;
class XanimePlayer;
class SingleEmitter;
class JPAParticleCallBack;
class MultiEmitterCallBack;
class MultiEmitterParticleCallBack;

class MultiEmitter {
public:
    MultiEmitter(const char*, const TVec3f*, const TVec3f*, const TVec3f*, const TVec3f&);
    MultiEmitter(const char*, MtxPtr, const TVec3f&);
    MultiEmitter(const char*, MtxPtr, const TVec3f*, const TVec3f&);
    MultiEmitter(const char*);

    void createEmitter();
    void createEmitterWithCallBack(MultiEmitterCallBackBase*);

    void deleteEmitter();
    void forceDeleteEmitter();
    void forceDelete(EffectSystem*);
    void deleteForeverEmitter();
    void playCalcAndDeleteForeverEmitter();

    bool isValid() const;
    bool isExistOneTimeEmitter() const;

    void setHostSRT(const TVec3f*, const TVec3f*, const TVec3f*);
    void setHostMtx(MtxPtr);
    void setOffset(const TVec3f&);
    void setBaseScale(f32);
    void setName(const char*);
    bool isEqualName(u16) const;

    ParticleEmitter* getParticleEmitter(int) const;
    void create(EffectSystem*);
    void scanParticleEmitter(EffectSystem*);

    void forceFollowOn();
    void forceFollowOff();
    void forceScaleOn();

    void initSyncBck(XanimePlayer*, const char*, s32, f32);
    void onDeleteSyncBck(bool, f32);
    void addSyncBck(const XanimePlayer*, const char*);
    void setContinueBckEnd(bool);
    void onCreateSyncClipping();
    void onForceDeleteSyncClipping();
    void stopEmitterOnClipped();
    void playEmitterOffClipped();

    void setDrawOrder(s32);

    void addChildEmitter(MultiEmitter*);

    void setGlobalRotationDegree(const TVec3f&, s32);

    void allocateEmitter(const char*);
    SingleEmitter* getValidEmitter(s32, bool);
    void createOneTimeEmitter();
    void createForeverEmitter();

    // In MultiEmitterAccess
    void setGlobalRotation(const TVec3s&, s32);
    void setGlobalTranslation(const TVec3f&, s32);
    void setGlobalScale(const TVec3f&, s32);
    void setGlobalSRTMatrix(const MtxPtr, s32);
    void setGlobalPrmColor(u8, u8, u8, s32);
    void setGlobalEnvColor(u8, u8, u8, s32);
    void setLocalScale(const TVec3f&, s32);
    void setDirectionalSpeed(f32, s32);
    void setRate(f32, s32);
    void stopCalcEmitter(s32);
    void playCalcEmitter(s32);
    void stopDrawParticle(s32);
    void playDrawParticle(s32);
    void setParticleCallBackPtr(JPAParticleCallBack*, s32);
    void pauseOff(s32);

    // In MarioEffect
    void setGlobalScale(f32, s32);

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

    /* 0x00 */ MR::AssignableArray< SingleEmitter > mEmitters;
    /* 0x08 */ MR::Vector< MR::FixedArray< MultiEmitter*, 4 > > mChildren;
    /* 0x1C */ MultiEmitterCallBack* mCallBack;
    /* 0x20 */ MultiEmitterParticleCallBack* mParticleCallBack;
    /* 0x24 */ void* _24;
    /* 0x28 */ void* _28;
    /* 0x2C */ f32 _2C;
    /* 0x30 */ f32 _30;
    /* 0x34 */ u16 mHash;
    /* 0x36 */ u8 mFlags;
};
