#include "Game/AudioLib/AudMicWrap.hpp"
#include "Game/AudioLib/AudAudience.hpp"
#include "Game/AudioLib/AudParams.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"

namespace AudMicWrap {
    void setMicMtx(MtxPtr pMtx, s32 param2) {
        AudWrap::getSystem()->setMicMtx(pMtx, param2);
    }

    void setMicEnv() {
        AudSystem* pSystem = AudWrap::getSystem();

        pSystem->mAudience.mSetting.mFarDistance = AudParams::micFarDistance;
        pSystem->mAudience.mSetting.mNearDistance = AudParams::micNearDistance;
        pSystem->mAudience.mSetting.mMinVolume = AudParams::micMinVolume;
        pSystem->mAudience.mSetting.mFrontDolbyZ = AudParams::micFrontDolbyZ;
        pSystem->mAudience.mSetting.mBehindDolbyZ = AudParams::micBehindDolbyZ;
        pSystem->mAudience.mSetting.mMaxDolby = AudParams::micMaxDolby;
        pSystem->mAudience.mSetting.mMinDolby = AudParams::micMinDolby;
        pSystem->mAudience.mSetting.mCenterDolby = AudParams::micCenterDolby;
        pSystem->mAudience.mSetting.mPanAmplitude = AudParams::micPanAmplitude;
        pSystem->mAudience.mSetting.mSonicSpeed = AudParams::micSonicSpeed;
        pSystem->mAudience.mSetting.mPitchDeltaRatioRange = AudParams::micPitchDeltaRatioRange;
        pSystem->mAudience.mSetting.mPriorityDownRange = AudParams::micPriorityDownRange;

        AudWrap::getSystem()->mAudience.updateSetting();
    }

    const TVec3f& getMicPos() {
        return AudWrap::getSystem()->getMicPos(0);
    }
};  // namespace AudMicWrap
