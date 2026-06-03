#include "Game/AudioLib/AudUtil.hpp"

namespace AudUtil {
    f32 linerMax(s32 val, s32 max, f32 end) {
        f32 ret;
        if (val <= 0.0f) {
            ret = 0.0f;
        } else if (val >= max) {
            ret = end;
        } else {
            ret = end * val / max;
        }
        return ret;
    }

    f32 linerMinMax(s32 val, s32 min, s32 max, f32 start, f32 end) {
        f32 ret;
        if (val <= min) {
            ret = start;
        } else if (val >= max) {
            ret = end;
        } else {
            ret = start + (end - start) * (val - min) / (max - min);
        }
        return ret;
    }

    bool isProperCurrentStageOrAudioDebugMode() {
        return true;
    }
};  // namespace AudUtil
