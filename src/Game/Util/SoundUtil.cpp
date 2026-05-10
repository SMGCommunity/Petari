#include "Game/Util/SoundUtil.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/GameAudio/AudTalkSoundData.hpp"

namespace MR {
    void startSystemSE(JAISoundID id, s32 a2, s32 a3) {
        AudWrap::getSystemSeObject()->startSoundParam(id, a2, a3);
    }

    void startSystemLevelSE(const char* pSound, s32 a2, s32 a3) {
        AudWrap::getSystemSeObject()->startLevelSoundParam(pSound, a2, a3);
    }
};  // namespace MR
