#pragma once

#include "Game/AudioLib/AudAudience.hpp"
#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JASSoundParams.hpp"
#include <JSystem/JAudio2/JAIAudible.hpp>
#include <JSystem/JAudio2/JASHeapCtrl.hpp>
#include <JSystem/JAudio2/JAUAudible.hpp>

class JAUDopplerAudibleChannel;
class JAUDopplerAudibleAbsPos;
class AudAudience_withSetting;

template < int SIZE, class T, class U >
class AudGenericAudible_ : public JAIAudible {
public:
    AudGenericAudible_(const TVec3f& rPos, const TVec3f* pPos, u32 mask) : JAIAudible(), mAbsPos() {
        mAudibleParam = -1;
        mAbsPos.init(&mPos, rPos, pPos);

        for (int i = 0; i < SIZE; i++) {
            if ((mask & 1 << i) == 0) {  // TODO: init call?
                _3C[i].mParams.init();
                _3C[i].mPitch = 1.0f;
                mChannels[i] = &_3C[i];
            } else {
                mChannels[i] = nullptr;
            }
        }

        mVolume = 1.0f;
        mPan = 0.5f;
        mDolby = 0.0f;
        mIsResetting = true;
    }

    virtual JASSoundParams* getOuterParams(int index) {
        T* channel = getChannel(index);
        if (channel != nullptr) {
            return &channel->mParams;
        }
        return nullptr;
    }

    void setOuterParams(const JASSoundParams& rOutParams, const JASSoundParams& rInParams, int index) {
        getOuterParams(index)->combine(rOutParams, rInParams);
    }

    virtual void calc() {
        mAbsPos.calc(mPos);
    }

    const TVec3f& getPos() const {
        return mPos;
    }

    T* getChannel(int index) {
        return mChannels[index];
    }

    JAUAudibleParam& getAudibleParam() {
        return mAudibleParam;
    }

    u32 getVolDistBit() {
        return getAudibleParam().getVolDistBit();
    }

    const TVec3f& getVelocity() const {
        return mAbsPos.getVelocity();
    }

    /* 0x010 */ JAUAudibleParam mAudibleParam;
    /* 0x014 */ U mAbsPos;
    /* 0x02C */ T* mChannels[SIZE];
    /* 0x03C */ T _3C[SIZE];
    /* 0x10C */ f32 mVolume;
    /* 0x110 */ f32 mPan;
    /* 0x114 */ f32 mDolby;
    /* 0x118 */ bool mIsResetting;
};

template < int SIZE >
class AudDopplerAudible : public AudGenericAudible_< SIZE, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos >,
                          public JASPoolAllocObject< AudDopplerAudible< SIZE > > {
public:
    typedef AudGenericAudible_< SIZE, JAUDopplerAudibleChannel, JAUDopplerAudibleAbsPos > Base;
    AudDopplerAudible(const TVec3f& rPos, const TVec3f* pPos, u32 mask)
        : Base(rPos, pPos, mask){

          };

    // size 11c
};
