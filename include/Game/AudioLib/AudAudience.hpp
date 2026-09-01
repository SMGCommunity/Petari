#pragma once

#include "Game/AudioLib/AudAudible.hpp"
#include <JSystem/JAudio2/JAIAudience.hpp>
#include <JSystem/JAudio2/JAUAudience.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>

class JAUDopplerAudibleRelPos;

class AudAudience_withSetting : public JAIAudience {
public:
    /// @brief Creates a new `AudAudience_withSetting`.
    AudAudience_withSetting();

    void updateSetting();

    /* 0x04 */ JAUAudience3DSetting mSetting;
    /* 0x3C */ f32 _3C;
    /* 0x40 */ f32 _40;
    /* 0x44 */ f32 _44;
    /* 0x48 */ f32 _48;
    /* 0x4C */ f32 _4C;
    /* 0x50 */ bool mFarCamera;
};

class AudAudience_withSetting_doppler : public AudAudience_withSetting {
public:
    /// @brief Creates a new `AudAudience_withSetting_doppler`.
    AudAudience_withSetting_doppler() : AudAudience_withSetting(){};
};

class AudDopplerAudienceState {
public:
    AudDopplerAudienceState() {
        TPos3f mtx;
        mtx.identity();
        resetMtx(mtx);
    }

    void resetMtx(const MtxPtr pMtx) {
        mMtx.set(pMtx);
        TPos3f lookAt;
        lookAt.setPositionFromLookAt(mMtx);
        lookAt.getTrans(mPos);
        mPrevPos.set(mPos);
        mMoveVec.zero();
    }

    void setMtx(const MtxPtr pMtx) {
        mPrevPos.set(mPos);
        mMtx.set(pMtx);
        TPos3f lookAt;
        lookAt.setPositionFromLookAt(mMtx);
        lookAt.getTrans(mPos);
        mMoveVec.sub(mPos, mPrevPos);
    }

    const TVec3f& getPos() const {
        return mPos;
    }

    void convertAbsToRel(const TVec3f&, JAUDopplerAudibleRelPos*);  // TODO

    /* 0x00 */ TPos3f mMtx;
    /* 0x30 */ TVec3f mMoveVec;
    /* 0x3C */ TVec3f mPos;
    /* 0x48 */ TVec3f mPrevPos;
};

template < int SIZE, class Audible, class AudienceState, class Audience >
class AudAudience_ : public Audience {
public:
    AudAudience_() : Audience(), mCount(SIZE), mAudible(){};

    virtual JAIAudible* newAudible(const TVec3f&, JAISoundID, const TVec3f*, u32);
    virtual int getMaxChannels();
    virtual void deleteAudible(JAIAudible*);
    virtual u32 calcPriority(JAIAudible*);
    virtual void mixChannelOut(const JASSoundParams&, JAIAudible*, int);

    /* 0x054 */ int mCount;
    /* 0x058 */ AudienceState mStates[SIZE];
    /* 0x1A8 */ Audible* mAudible;
};

template < int SIZE >
class AudAudience : public AudAudience_< SIZE, AudDopplerAudible< SIZE >, AudDopplerAudienceState, AudAudience_withSetting_doppler > {
public:
    AudAudience() : AudAudience_< SIZE, AudDopplerAudible< SIZE >, AudDopplerAudienceState, AudAudience_withSetting_doppler >(){};
};
