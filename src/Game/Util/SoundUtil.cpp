#include "Game/Util/SoundUtil.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/AudioLib/AudWrap.hpp"

namespace MR {
    JAISoundHandle* startSystemSE(JAISoundID id, s32 param2, s32 param3) {
        return AudWrap::getSystemSeObject()->startSoundParam(id, param2, param3);
    }

    JAISoundHandle* startSystemLevelSE(const char* pName, s32 param2, s32 param3) {
        return AudWrap::getSystemSeObject()->startLevelSoundParam(pName, param2, param3);
    }
};  // namespace MR
