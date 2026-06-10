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

        pSystem->mAudience.mSetting._0 = AudParams::micFarDistance;
        pSystem->mAudience.mSetting._4 = AudParams::micNearDistance;
        pSystem->mAudience.mSetting._8 = AudParams::micMinVolume;
        pSystem->mAudience.mSetting._C = AudParams::micFrontDolbyZ;
        pSystem->mAudience.mSetting._10 = AudParams::micBehindDolbyZ;
        pSystem->mAudience.mSetting._14 = AudParams::micMaxDolby;
        pSystem->mAudience.mSetting._18 = AudParams::micMinDolby;
        pSystem->mAudience.mSetting._1C = AudParams::micCenterDolby;
        pSystem->mAudience.mSetting._28 = AudParams::micPanAmplitude;
        pSystem->mAudience.mSetting._2C = AudParams::micSonicSpeed;
        pSystem->mAudience.mSetting._30 = AudParams::micPitchDeltaRatioRange;
        pSystem->mAudience.mSetting._34 = AudParams::micPriorityDownRange;

        AudWrap::getSystem()->mAudience.updateSetting();
    }

    const TVec3f& getMicPos() {
        return AudWrap::getSystem()->getMicPos(0);
    }
};  // namespace AudMicWrap
