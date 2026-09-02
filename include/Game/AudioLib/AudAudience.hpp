#pragma once

#include "Game/AudioLib/AudAudible.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JASSoundParams.hpp"
#include "JSystem/JAudio2/JAUAudible.hpp"
#include "JSystem/JAudio2/JAUSoundInfo.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include <JSystem/JAudio2/JAIAudience.hpp>
#include <JSystem/JAudio2/JAUAudience.hpp>
#include <JSystem/JAudio2/JAUDoppler.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>

class JAUDopplerAudibleRelPos;

class AudDopplerAudienceState {
public:
    AudDopplerAudienceState() {
        // TPos3f mtx;
        // mtx.identity();
        // resetMtx(mtx);
    }

    void resetMtx(const Mtx pMtx) NO_INLINE {
        mMtx.set(pMtx);
        TPos3f lookAt;
        lookAt.setPositionFromLookAt(mMtx);
        lookAt.getTrans(mPos);
        mPrevPos.set(mPos);
        mVelocity.zero();
    }

    void setMtx(const MtxPtr pMtx) {
        mPrevPos.set(mPos);
        mMtx.set(pMtx);
        TPos3f lookAt;
        lookAt.setPositionFromLookAt(mMtx);
        lookAt.getTrans(mPos);
        mVelocity.sub(mPos, mPrevPos);
    }

    const TVec3f& getPos() const {
        return mPos;
    }

    void convertAbsToRel(const TVec3f& rPos, JAUDopplerAudibleRelPos* pRel) {
        TVec3f diff;
        diff.sub(rPos, mPos);

        if (diff.x > -0.002f && diff.x < 0.002f) {
            if (diff.x < 0.0f) {
                diff.x = -0.002f;
            } else {
                diff.x = 0.002f;
            }
        }
        if (diff.y > -0.002f && diff.y < 0.002f) {
            if (diff.y < 0.0f) {
                diff.y = -0.002f;
            } else {
                diff.y = 0.002f;
            }
        }
        if (diff.z > -0.002f && diff.z < 0.002f) {
            if (diff.z < 0.0f) {
                diff.z = -0.002f;
            } else {
                diff.z = 0.002f;
            }
        }

        PSMTXMultVecSR(mMtx, diff, pRel->mPos);
        pRel->mDir.set(diff);
        pRel->mDist = pRel->mDir.normalize();
    }

    const TVec3f& getVelocity() {
        return mVelocity;
    }

    /* 0x00 */ TPos3f mMtx;
    /* 0x30 */ TVec3f mVelocity;
    /* 0x3C */ TVec3f mPos;
    /* 0x48 */ TVec3f mPrevPos;
};

class AudAudience_withSetting : public JAIAudience {
public:
    /// @brief Creates a new `AudAudience_withSetting`.
    AudAudience_withSetting();

    void updateSetting();

    u32 calcDeltaPriority_(f32 dist, bool excludeFar, s32 distVolBit) const {
        f32 farDist = distVolBit == 8 ? mSetting.mFarDistance / 2.0f : mSetting.mFarDistance * distVolBit;

        if (mFarCamera) {
            farDist *= 3.0f;
        }

        if (dist > farDist) {
            if (excludeFar) {
                return 0xFFFFFFFF;
            }

            return mSetting.mPriorityDownRange;
        }

        if (dist < mSetting.mNearDistance) {
            return 0;
        }

        f32 priorityRate = distVolBit == 1 ? mPriorityRate : mSetting.mPriorityDownRange / (farDist - mSetting.mNearDistance);
        return priorityRate * (dist - mSetting.mNearDistance);
    }

    f32 calcVolume_(f32 dist, s32 distVolBit) {
        f32 farDist = distVolBit == 8 ? mSetting.mFarDistance / 2.0f : mSetting.mFarDistance * distVolBit;

        if (mFarCamera) {
            farDist *= 3.0f;
        }

        if (dist > farDist) {
            return mSetting.mMinVolume;
        }

        if (dist < mSetting.mNearDistance) {
            return 1.0f;
        }

        f32 volumeRate = distVolBit == 1 ? mVolumeRate : (mSetting.mMinVolume - 1.0f) / (farDist - mSetting.mNearDistance);
        return MR::clamp(volumeRate * (dist - mSetting.mNearDistance) + 1.0f, 0.0f, 1.0f);
    }

    f32 calcDolby_(const TVec3f& rPos, s32 distVolBit) {
        f32 z = rPos.z;
        if (z > mSetting.mBehindDolbyZ) {
            return mSetting.mMaxDolby;
        }

        if (z < mSetting.mFrontDolbyZ) {
            return mSetting.mMinDolby;
        }

        if (z < 0.0f) {
            return mSetting.mCenterDolby + mFrontDolbyRate * z;
        } else {
            return mSetting.mCenterDolby + mBehindDolbyRate * z;
        }
    }

    f32 calcPan_(f32 dist, const TVec3f& rPos) {
        f32 panRate = dist <= MR::epsilon() ? 0.0f : rPos.x / dist;

        return panRate * mSetting.mPanAmplitude + 0.5f;
    }

    f32 calcDopplerPitch_(const TVec3f& rDir, const TVec3f& rVel1, const TVec3f& rVel2, f32 doppler) {
        f32 vel1 = doppler * rDir.dot(rVel1);
        f32 vel2 = doppler * rDir.dot(rVel2);

        return (mSetting.mSonicSpeed + vel1) / (mSetting.mSonicSpeed + vel2);
    }

    f32 calcDopplerPitch_(f32 vel1, f32 vel2) {
        return (mSetting.mSonicSpeed + vel1) / (mSetting.mSonicSpeed + vel2);
    }

    f32 calcPitch_(JAUDopplerAudibleChannel* pChannel, f32 pitch) {
        if (pChannel->mPitch > 0.0f) {
            if (pChannel->mPitch > pitch * mSetting.mPitchDeltaRatioRange) {
                return pChannel->mPitch;
            }
            if (pitch > pChannel->mPitch * mSetting.mPitchDeltaRatioRange) {
                return pChannel->mPitch;
            }
            pChannel->mPitch = pitch;
            return pitch;
        }

        if (pitch > 0.0f) {
            pChannel->mPitch = pitch;
            return pitch;
        }

        return 1.0f;
    }

    f32 calcFxMix_(f32 dist) {
        if (dist > mSetting.mFarDistance) {
            return mSetting.mMaxFxMix;
        }

        if (dist < mSetting.mNearDistance) {
            return mSetting.mMinFxMix;
        }

        return mSetting.mMinFxMix + mFxMixRate * (dist - mSetting.mNearDistance);
    }

    /* 0x04 */ JAUAudience3DSetting mSetting;
    /* 0x3C */ f32 mFrontDolbyRate;
    /* 0x40 */ f32 mBehindDolbyRate;
    /* 0x44 */ f32 mVolumeRate;
    /* 0x48 */ f32 mPriorityRate;
    /* 0x4C */ f32 mFxMixRate;
    /* 0x50 */ bool mFarCamera;
};

class AudAudience_withSetting_doppler : public AudAudience_withSetting {
public:
    /// @brief Creates a new `AudAudience_withSetting_doppler`.
    AudAudience_withSetting_doppler() : AudAudience_withSetting(){};
};

template < int MAX_CHANNELS, class Audible, class AudienceState, class Audience >
class AudAudience_ : public Audience {
public:
    AudAudience_() : Audience(), mMaxChannels(MAX_CHANNELS) {
        for (int i = 0; i < MAX_CHANNELS; i++) {
            TPos3f mtx;
            mtx.identity();
            mStates[i].resetMtx(mtx);
        }
    };

    virtual int getMaxChannels() {
        return mMaxChannels;
    }

    virtual JAIAudible* newAudible(const TVec3f& rPos, JAISoundID soundID, const TVec3f* pPos, u32 mask) {
        mask |= ~((1 << mMaxChannels) - 1);
        if (mask == 0xFFFFFFFF) {
            return nullptr;
        }

        Audible* audible = new Audible(rPos, pPos, mask);
        if (audible == nullptr) {
            return nullptr;
        }

        for (int i = 0; i < mMaxChannels; i++) {
            JAUDopplerAudibleChannel* channel = audible->mChannels[i];  // TODO: inherited?
            if (channel == nullptr) {
                continue;
            }

            mStates[i].convertAbsToRel(audible->getPos(), &channel->mRelPos);
            Audience::calcDeltaPriority_(channel->mRelPos.mDist, false, 1);
        }

        JAUSoundInfo* info = JAUSoundInfo::getInstance();
        if (info != nullptr) {
            audible->mAudibleParam = JAUAudibleParam(info->getAudibleSw(soundID), -1);
        }

        return audible;
    }

    virtual void deleteAudible(JAIAudible* pAudible) {
        delete pAudible;
    }

    virtual u32 calcPriority(JAIAudible* pAudible) {
        Audible* audible = (Audible*)pAudible;
        if ((audible->getAudibleParam().get_BIT11()) == 0) {
            for (int i = 0; i < mMaxChannels; i++) {
                if (audible->mChannels[i] != nullptr) {
                    mStates[i].convertAbsToRel(audible->getPos(), &audible->mChannels[i]->mRelPos);
                }
            }
            return 0;
        }

        u32 priority = -1;
        u32 deltaPriority[MAX_CHANNELS];

        for (int i = 0; i < mMaxChannels; i++) {
            // FIXME!!!!!
            JAUDopplerAudibleChannel* channel = audible->mChannels[i];
            if (channel != nullptr) {
                u32 volDistBit = audible->getAudibleParam().getVolDistBit();
                mStates[i].convertAbsToRel(audible->getPos(), &audible->mChannels[i]->mRelPos);
                bool bit = !audible->getAudibleParam().get_BIT8();
                deltaPriority[i] = Audience::calcDeltaPriority_(channel->mRelPos.getDist(), bit, volDistBit);
                if (deltaPriority[i] < priority) {
                    priority = deltaPriority[i];
                }
            }
        }

        return priority;
    }

    /*
    s32 getVolDistBit(Audible* audible) {
        JAUAudibleParam param = audible->getAudibleParam();
        return param.getVolDistBit();
    }
    */

    virtual void mixChannelOut(const JASSoundParams& rParams, JAIAudible* pAudible, int channelNum) NO_INLINE {
        Audible* audible = (Audible*)pAudible;
        JAUDopplerAudibleChannel* channel = audible->getChannel(channelNum);
        if (channel == nullptr) {
            return;
        }

        JASSoundParams params;

        if (audible->getAudibleParam().calcVolume()) {
            // FIXME: needs a double-load
            params.mVolume = Audience::calcVolume_(channel->mRelPos.getDist(), audible->getVolDistBit());

            f32 vol = params.mVolume;
            if (!audible->mIsResetting) {
                if (vol > audible->mVolume) {
                    if (vol - audible->mVolume >= 0.2f) {
                        vol = audible->mVolume + (vol - audible->mVolume) / 5.0f;
                    }
                } else {
                    if (audible->mVolume - vol >= 0.2f) {
                        vol = vol + (audible->mVolume - vol) / 5.0f;
                    }
                }
            }
            audible->mVolume = vol;
            params.mVolume = vol;
        } else {
            params.mVolume = 1.0f;
        }

        if (audible->getAudibleParam().calcDolby()) {
            params.mDolby = Audience::calcDolby_(channel->mRelPos.getPos(), audible->getVolDistBit());

            f32 dolby = params.mDolby;
            if (!audible->mIsResetting) {
                if (dolby > audible->mDolby) {
                    if (dolby - audible->mDolby >= 0.2f) {
                        dolby = audible->mDolby + (dolby - audible->mDolby) / 5.0f;
                    }
                } else {
                    if (audible->mDolby - dolby >= 0.2f) {
                        dolby = dolby + (audible->mDolby - dolby) / 5.0f;
                    }
                }
            }
            audible->mDolby = dolby;
            params.mDolby = dolby;
        } else {
            params.mDolby = 0.1f;
        }

        if (audible->getAudibleParam().calcPan()) {
            if (mMaxChannels > 2) {
                params.mPan = (channelNum & 1) != 0 ? 1.0f : 0.0f;
            } else {
                params.mPan = Audience::calcPan_(channel->mRelPos.getDist(), channel->mRelPos.getPos());
            }

            f32 pan = params.mPan;
            if (!audible->mIsResetting) {
                if (pan > audible->mPan) {
                    if (pan - audible->mPan >= 0.2f) {
                        pan = audible->mPan + (pan - audible->mPan) / 5.0f;
                    }
                } else {
                    if (audible->mPan - pan >= 0.2f) {
                        pan = pan + (audible->mPan - pan) / 5.0f;
                    }
                }
            }
            audible->mPan = pan;
            params.mPan = pan;
        } else {
            params.mPan = 0.5f;
        }

        params.mPitch = calcPitch(channel, audible, &mStates[channelNum]);

        params.mFxMix = calcFxMix(channel, audible);

        audible->setOuterParams(rParams, params, channelNum);
        audible->mIsResetting = false;
    }

    f32 calcPitch(JAUDopplerAudibleChannel* channel, Audible* audible, AudienceState* pState) {
        JAUAudibleParam param = audible->getAudibleParam();
        if (param.getDoppler() != 0) {
            f32 doppler = param.getDopplerPower();

            f32 vel1 = doppler * channel->getDir().dot(pState->getVelocity());
            f32 vel2 = doppler * channel->getDir().dot(audible->getVelocity());

            f32 pitch = Audience::calcDopplerPitch_(vel1, vel2);

            return Audience::calcPitch_(channel, pitch);
        } else {
            return 1.0f;
        }
    }

    f32 calcFxMix(JAUDopplerAudibleChannel* channel, Audible* audible) {
        if (audible->getAudibleParam().calcFxMix()) {
            return Audience::calcFxMix_(channel->mRelPos.getDist());
        } else {
            return 0.0f;
        }
    }

    /* 0x054 */ int mMaxChannels;
    /* 0x058 */ AudienceState mStates[MAX_CHANNELS];
    /* 0x1A8 */  // Audible* mAudible;
};

template < int MAX_CHANNELS >
class AudAudience : public AudAudience_< MAX_CHANNELS, AudDopplerAudible< MAX_CHANNELS >, AudDopplerAudienceState, AudAudience_withSetting_doppler > {
public:
    AudAudience()
        : AudAudience_< MAX_CHANNELS, AudDopplerAudible< MAX_CHANNELS >, AudDopplerAudienceState, AudAudience_withSetting_doppler >(){

          };
    /* 0x1A8 */  // AudDopplerAudible< MAX_CHANNELS >* mAudible;
};
