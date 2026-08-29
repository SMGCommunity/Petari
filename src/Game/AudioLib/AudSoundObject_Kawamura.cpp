#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/AudioLib/AudUtil.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Util/MathUtil.hpp"

void AudSoundObject_Kawamura_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)2.0f;
}

void AudSoundObject::modifySe_Kawamura(JAISoundHandle* pHandle, s32 level) {
    f32 volume = 1.0f;
    f32 pitch = 1.0f;
    u32 steps = 0;

    switch (pHandle->getSound()->getID()) {
    case SE_OJ_COIN_BOUND:
    case SE_OJ_LV_SNOW_DIG: {
        volume = level / 100.0f;
        break;
    }
    case SE_OJ_KEY_SWITCH_BOUND: {
        volume = level / 100.0f;
        volume *= volume;
        break;
    }
    case SE_OJ_COCONUT_BOUND:
    case SE_OJ_COCONUT_BOUND_WATER: {
        volume = level / 100.0f;
        volume *= volume;
        volume *= 2.0f;
        if (volume > 1.0f) {
            volume = 1.0f;
        }

        break;
    }
    case SE_AT_LV_ASTRO_DOME_WIND_1: {
        f32 t = level / 100.0f;
        pitch = (t * t) * 0.5f + 1.0f;
        break;
    }
    case SE_OJ_LV_ROPE_SWING_WIND_1: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        volume = level / 100.0f;
        volume *= volume;
        pitch = volume * 0.5f + 0.9f;
        break;
    }
    case SE_OJ_LV_ROPE_SWING_WIND_2: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }

        if (level < 40) {
            pitch = 1.0f;
            volume = 0.0f;
        } else {
            volume = (level - 40) / 60.0f;
            volume *= volume;
            pitch = volume * 0.4f + 0.9f;
        }
        break;
    }
    case SE_SY_ACTION_COMBO: {
        if (level < 0) {
            level = 0;
        }
        if (level > 12) {
            level = 12;
        }
        writePort(pHandle, 11, level);
        break;
    }
    case SE_SY_FLOWER_GET_COMBO: {
        if (level < 0) {
            level = 0;
        }
        while (level > 12 * 3) {
            level -= 12;
        }

        writePort(pHandle, 11, level);
        break;
    }
    case SE_PM_LV_PULL_BACK_FLY: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        f32 t = level / 50.0f;
        pitch = t * t + 0.75f;
        break;
    }
    case SE_OJ_LV_COCONUT_ROLL: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        f32 t = level / 100.0f;
        pitch = t / 2.0f + 0.5f;
        volume = t * t;
        break;
    }
    case SE_OJ_LV_COCONUT_ROLL_WATER: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        f32 t = level / 100.0f;
        pitch = t / 4.0f + 0.75f;
        volume = t * t;
        break;
    }
    case SE_OJ_LV_SPACE_COCOON_DRAG: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        f32 t = level / 100.0f;
        t *= t;
        pitch = t + 1.0f;
        volume = t * 0.3f + 0.7f;
        break;
    }
    case SE_PM_LV_TERESA_MARIO_FLY: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        f32 t = level / 100.0f;
        volume = t * t;
        if (volume < 0.2f) {
            volume = 0.2f;
        }
        pitch = t / 2.0f + 0.5f;
        break;
    }
    case SE_BM_KOOPA_SPIN_WIND: {
        f32 t = level / 100.0f;
        pitch = t / 2.0f + 1.0f;
        volume = t * t + 0.2f;
        if (volume > 1.0f) {
            volume = 1.0f;
        }
        break;
    }
    case SE_BM_KOOPA_SPIN_HIT_2: {
        f32 t = level / 100.0f;
        pitch = t / 2.0f + 0.9f;
        break;
    }
    case SE_OJ_BIG_BUBBLE_DPD_HIT: {
        f32 t = level / 100.0f;
        pitch = t / 3.0f + 0.9f;
        break;
    }
    case SE_OJ_LV_SPIN_BOX_SLIDE:
    case SE_OJ_SPIN_BOX_SPIN:
    case SE_BM_LV_KOOPA_ROTATE_LOW: {
        f32 t = level / 100.0f;
        pitch = t / 2.0f + 0.75f;
        volume = t * t;
        break;
    }
    case SE_PM_LV_T_MARIO_RIDE_WIND: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        f32 t = level / 100.0f;
        pitch = t / 2.0f + 0.75f;
        volume = t * t;
        break;
    }
    case SE_PM_LV_FOO_FLYING: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        steps = 6;
        level = 100 - level;
        f32 t = level / 100.0f;
        t *= t;
        pitch = t / 10.0f + 0.95f;
        volume = t / 4.0f + 0.75f;
        break;
    }
    case SE_BM_LV_KOOPA_SWAVE_MOVE: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        f32 t = level / 100.0f;
        pitch = (t * t) / 2.0f + 1.0f;
        break;
    }
    case SE_SY_LV_BIG_BUBBLE_WIND: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        f32 t = level / 100.0f;
        t *= t;
        pitch = t / 4.0f + 0.75f;
        volume = t / 2.0f + 0.5f;
        break;
    }
    case SE_BM_LV_SKL_GUARD_ALARM: {
        level = MR::max(level, 0);
        level = MR::min(level, 100000);
        pitch = AudUtil::linerMinMax(level, 300, 2000, 2.0f, 1.85f);
        f32 l = AudUtil::linerMinMax(level, 200, 3000, 4.0f, 24.0f);
        writePort(pHandle, 11, l);
        break;
    }
    case SE_OJ_LV_FLEX_SPHERE_MOVE: {
        if (level < 0) {
            level = 0;
        }
        if (level > 100) {
            level = 100;
        }
        f32 t = level / 100.0f;
        t *= t;
        pitch = t + 0.25f;
        volume = t / 2.0f + 0.3f;
        break;
    }
    default: {
        return;
    }
    }

    pHandle->getSound()->getAuxiliary().moveVolume(volume, steps);
    pHandle->getSound()->getAuxiliary().movePitch(pitch, steps);
}

bool AudSoundObject::modifyLimitedSound_Kawamura(JAISoundID soundID) {
    switch (soundID) {
    case SE_SY_GCAPTURE_APPEAR: {
        if (isPlayingID(SE_SY_SPIN_DRIVER_APPEAR)) {
            stopSound(SE_SY_SPIN_DRIVER_APPEAR, 0);
            return false;
        }
        break;
    }
    case SE_SY_SPIN_DRIVER_APPEAR: {
        if (isPlayingID(SE_SY_GCAPTURE_APPEAR)) {
            return true;
        }
        break;
    }
    case SE_SY_READ_RIDDLE_S: {
        if (isPlayingID(SE_SY_QUESTION_COIN)) {
            return true;
        }
        break;
    }
    case SE_OJ_LV_HOPPER_CONVEYER_MV: {
        if (AudWrap::getSystem()->isAlreadyPlayingSoundNear(soundID, getPos(), 600.0f)) {
            return true;
        }
        break;
    }
    case SE_OJ_LV_M_KOOPA_CATER_S_MV: {
        if (AudWrap::getSystem()->isAlreadyPlayingSoundNear(soundID, getPos(), 500.0f)) {
            return true;
        }
        break;
    }
    case SE_OJ_LV_FIRE_BAR_CENTER: {
        if (AudWrap::getSystem()->isAlreadyPlayingSoundNear(soundID, getPos(), 400.0f)) {
            return true;
        }
        break;
    }
    case SE_OJ_LV_MARINE_STEAM_OUT_S: {
        if (AudWrap::getSystem()->isAlreadyPlayingSoundNear(soundID, getPos(), 400.0f)) {
            return true;
        }
        break;
    }
    case SE_OJ_LV_STAIR_BREAK: {
        if (AudWrap::getSystem()->isAlreadyPlayingSoundNear(soundID, getPos(), 350.0f)) {
            return true;
        }
        break;
    }
    case SE_OJ_LV_LAVA_STEAM_OUT: {
        if (AudWrap::getSystem()->isAlreadyPlayingSoundNear(soundID, getPos(), 350.0f)) {
            return true;
        }
        break;
    }
    case SE_OJ_LV_LAVA_GEYSER_SIGN:
    case SE_OJ_LAVA_GEYSER_SHOOT:
    case SE_OJ_LV_LAVA_GEYSER_KEEP: {
        if (AudWrap::getSystem()->isAlreadyPlayingSoundNear(soundID, getPos(), 400.0f)) {
            return true;
        }
        break;
    }
    default: {
        return false;
    }
    }
    return false;
}
