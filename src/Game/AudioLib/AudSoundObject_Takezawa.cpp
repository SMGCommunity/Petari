#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/AudioLib/AudUtil.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Util/MathUtil.hpp"

void AudSoundObject_Takezawa_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
}

namespace {
    static const s32 cMagicPntGVolMinPrm = 500;
    static const s32 cMagicPntGVolMaxPrm = 1500;
    static const f32 cMagicPntGVolMin = 0.8f;
    static const f32 cMagicPntGVolMax = 1.0f;
    static const s32 cMagicPntGPitMinPrm = 100;
    static const s32 cMagicPntGPitMaxPrm = 2000;
    static const f32 cMagicPntGPitMin = 0.5f;
    static const f32 cMagicPntGPitMax = 2.0f;
    static const s32 cUnizoRollVolMinPrm = 200;
    static const s32 cUnizoRollVolMaxPrm = 700;
    static const f32 cUnizoRollVolMin = 0.1f;
    static const f32 cUnizoRollVolMax = 1.0f;
    static const s32 cUnizoRollPitMinPrm = 50;
    static const s32 cUnizoRollPitMaxPrm = 1000;
    static const f32 cUnizoRollPitMin = 0.8f;
    static const f32 cUnizoRollPitMax = 1.0f;
    static const s32 cUnizoRollGroundVolMinPrm = 200;
    static const s32 cUnizoRollGroundVolMaxPrm = 700;
    static const f32 cUnizoRollGroundPitMin = 0.9f;
    static const f32 cUnizoRollGroundPitMax = 1.0f;
    static const s32 cTorpedoPitMinPrm = 300;
    static const s32 cTorpedoPitMaxPrm = 2000;
    static const f32 cTorpedoPitMin = 1.27f;
    static const f32 cTorpedoPitMax = 1.2f;
    static const s32 cTorpedoWiatMinPrm = 200;
    static const s32 cTorpedoWiatMaxPrm = 3000;
    static const f32 cTorpedoWiatMin = 4.0f;
    static const f32 cTorpedoWiatMax = 24.0f;
    static const s32 cBombBoundVolMinPrm = 100;
    static const s32 cBombBoundVolMaxPrm = 1000;
};  // namespace

void AudSoundObject::modifySe_Takezawa(JAISoundHandle* pHandle, s32 levelA, s32 levelB) {
    // FIXME: a few minor swaps and compiler optimizations
    // https://decomp.me/scratch/qpcVz

    f32 volume = 1.0f;
    f32 pitch = 1.0f;
    u32 steps = 0;

    switch (pHandle->getSound()->getID()) {
    case SE_SM_LV_IRONSPH_ROLL1:
    case SE_SM_LV_IRONSPH_ROLL_RAIL: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 10000);
        volume = AudUtil::linerMax(levelA, 100, 1.0f);
        break;
    }
    case SE_SM_LV_BALLOONSPH_ROLL: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 1000000);
        volume = AudUtil::linerMax(levelA, 10000, 1.0f);
        break;
    }
    case SE_SM_IRONSPH_HIT_WALL:
    case SE_SM_BALLOONSPH_HIT_WALL: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = AudUtil::linerMax(levelA, 20, 1.0f);
        break;
    }
    case SE_SM_LV_SLED_YOS_RUN1: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 10000);
        volume = AudUtil::linerMax(levelA, 2000, 1.0f);
        break;
    }
    case SE_SM_LV_SLED_MOVE: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 10000);
        volume = AudUtil::linerMinMax(levelA, 500, 3000, 0.4f, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 1600, 3000, 0.85f, 1.5f);
        break;
    }
    case SE_SM_LV_SLED_YOS_WATER_RUN: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 10000);
        volume = AudUtil::linerMinMax(levelA, 500, 3000, 0.4f, 1.0f);
        break;
    }
    case SE_EM_LV_OTAJACK_PULL: {
        // FIXME: regswap
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 2000, 40000, 0.5f, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 2000, 40000, 0.8f, 1.1f);
        break;
    }
    case SE_SM_LV_FOLLOWKIKKI_MOVE: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMax(levelA, 2500, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 0, 2000, 0.8f, 1.1f);
        break;
    }
    case SE_SM_LV_KART_ENGINE_LOW: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        if (levelA <= 1000) {
            volume = AudUtil::linerMinMax(levelA, 0, 1000, 0.3f, 1.0f);
        } else {
            volume = AudUtil::linerMinMax(levelA, 1500, 2500, 1.0f, 0.0f);
        }
        pitch = AudUtil::linerMinMax(levelA, 0, 2000, 1.0f, 2.0f);
        break;
    }
    case SE_SM_LV_KART_ENGINE_HIGH: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 1500, 2500, 0.0f, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 1500, 4000, 1.0f, 2.0f);
        break;
    }
    case SE_SM_KART_LAND: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 1000, 2500, 0.7f, 1.0f);
        break;
    }
    case SE_SM_LV_HELIBIRD_FLY: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 500, 3000, 0.4f, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 500, 3000, 1.0f, 1.5f);
        break;
    }
    case SE_SM_LV_MOCINA_PULL2: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 10, 100, 0.5f, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 10, 70, 1.2f, 1.8f);
        break;
    }
    case SE_SM_LV_JETSWING_ENGINE: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 500, 3000, 0.2f, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 1600, 8000, 0.9f, 1.4f);
        break;
    }
    case SE_SM_LV_JETSWING_MOVE: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 500, 3000, 0.1f, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 1600, 8000, 0.85f, 1.7f);
        break;
    }
    case SE_SM_JETSWING_LAND: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 1000, 2500, 0.7f, 1.0f);
        break;
    }
    case SE_SM_LV_FOOFIGHTER_MOVE1: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 500, 2500, 0.5f, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 500, 2500, 1.0f, 1.7f);
        break;
    }
    case SE_SM_LV_FOOFIGHTER_MOVE2: {
        // FIXME: compiler optimizing 16-bit compare with 100000
        s32 lvlA = levelA >> 16;
        lvlA = MR::min(MR::max(lvlA, 0), 100000);
        s32 lvlB = levelA & 0xFFFF;
        lvlB = MR::min(lvlB, 100000);
        volume = AudUtil::linerMinMax(lvlA, 500, 10000, 0.3f, 1.0f);
        pitch = AudUtil::linerMinMax(MR::max(lvlB - lvlA / 10, 0), 5000, 11000, 1.0f, 0.5f);
        break;
    }
    case SE_OJ_LV_MAGIC_PNT_G_PULL: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, ::cMagicPntGVolMinPrm, ::cMagicPntGVolMaxPrm, ::cMagicPntGVolMax, ::cMagicPntGVolMin);
        if (levelA <= ::cMagicPntGPitMinPrm) {
            pitch = ::cMagicPntGPitMin;
        } else if (levelA >= ::cMagicPntGPitMaxPrm) {
            pitch = ::cMagicPntGPitMax;
        } else {
            f32 d = ::cMagicPntGPitMaxPrm - ::cMagicPntGPitMinPrm;
            f32 t = levelA - ::cMagicPntGPitMinPrm;
            pitch = (t * t) / (d * d) * (::cMagicPntGPitMax - ::cMagicPntGPitMin) + ::cMagicPntGPitMin;
        }
        break;
    }
    case SE_OJ_LV_SPIDER_THREAD_PULL: {
        // FIXME: compiler optimizing 16-bit compare with 100000
        s32 pitchLvl = MR::min(levelA >> 16, 100000);
        s32 volLvl = MR::min(levelA & 0xFFFF, 100000);
        volume = AudUtil::linerMinMax(volLvl, 10, 600, 0.0f, 1.0f);
        pitch = AudUtil::linerMinMax(pitchLvl, 10, 500, 1.0f, 1.5f);
        break;
    }
    case SE_SY_BLUECHIP_GET:
    case SE_SY_YELLOWCHIP_GET:
    case SE_OJ_BLUECHIP_COMPLETE:
    case SE_OJ_YELLOWCHIP_COMPLETE: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 12);
        writePort(pHandle, 11, levelA);
        break;
    }
    case SE_EM_LV_UNIZO_ROLL_GROUND: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        if (levelA >= ::cUnizoRollGroundVolMinPrm) {
            volume = AudUtil::linerMinMax(levelA, ::cUnizoRollGroundVolMinPrm, ::cUnizoRollGroundVolMaxPrm, ::cUnizoRollVolMin, ::cUnizoRollVolMax);
        } else {
            volume = 0.0f;
        }
        pitch = AudUtil::linerMinMax(levelA, ::cUnizoRollPitMinPrm, ::cUnizoRollPitMaxPrm, ::cUnizoRollGroundPitMin, ::cUnizoRollGroundPitMax);
        break;
    }
    case SE_EM_LV_UNIZO_ROLL_SHALLOW:
    case SE_EM_LV_UNIZO_ROLL_WATER: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        if (levelA >= ::cUnizoRollVolMinPrm) {
            volume = AudUtil::linerMinMax(levelA, ::cUnizoRollVolMinPrm, ::cUnizoRollVolMaxPrm, ::cUnizoRollVolMin, ::cUnizoRollVolMax);
        } else {
            volume = 0.0f;
        }
        pitch = AudUtil::linerMinMax(levelA, ::cUnizoRollPitMinPrm, ::cUnizoRollPitMaxPrm, ::cUnizoRollPitMin, ::cUnizoRollPitMax);
        break;
    }
    case SE_EM_LV_UNIZO_LAND_WATER:
    case SE_EM_LV_UNIZO_LAND: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        if (levelA >= 50) {
            volume = AudUtil::linerMinMax(levelA, 50, 200, 0.3f, 1.0f);
        } else {
            volume = 0.0f;
        }
        pitch = AudUtil::linerMinMax(levelA, 5, 100, 0.9f, 1.0f);
        break;
    }
    case SE_EM_LV_UNIZO_COLLISION_WATER:
    case SE_EM_LV_UNIZO_COLLISION: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        if (levelA >= 100) {
            volume = AudUtil::linerMinMax(levelA, 100, 800, 0.5f, 1.0f);
        } else {
            volume = 0.0f;
        }
        pitch = AudUtil::linerMinMax(levelA, 100, 800, 0.9f, 1.0f);
        break;
    }
    case SE_BM_LV_KAMECK_FLOAT: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        volume = AudUtil::linerMinMax(levelA, 100, 2000, 0.3f, 1.0f);
        pitch = AudUtil::linerMinMax(levelA, 100, 2000, 0.6f, 1.0f);
        break;
    }
    case SE_EM_LV_TORPEDO_ALARM: {
        // FIXME: regswap
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100000);
        pitch = AudUtil::linerMinMax(levelA, ::cTorpedoPitMinPrm, ::cTorpedoPitMaxPrm, ::cTorpedoPitMin, ::cTorpedoPitMax);
        f32 wait = AudUtil::linerMinMax(levelA, ::cTorpedoWiatMinPrm, ::cTorpedoWiatMaxPrm, ::cTorpedoWiatMin, ::cTorpedoWiatMax);
        writePort(pHandle, 11, wait);
        break;
    }
    case SE_EM_BOMB_BOUND: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 10000);
        if (levelA >= 100) {
            volume = AudUtil::linerMinMax(levelA, ::cBombBoundVolMinPrm, ::cBombBoundVolMaxPrm, 0.6f, 1.0f);
        } else {
            volume = 0.0f;
        }
        break;
    }
    case SE_RS_LV_NOTE_TIMER_SLOW:
    case SE_RS_LV_NOTE_TIMER_MIDDLE:
    case SE_RS_LV_NOTE_TIMER_FAST: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 127);
        writePort(pHandle, 13, levelA);
        break;
    }
    case SE_SM_LV_TICO_FLOAT:
    case SE_SM_LV_TICO_FLOAT_DEMO: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = levelA / 100.0f;
        break;
    }
    case SE_EM_LV_STRSPIDER_SWING1:
    case SE_EM_LV_STRSPIDER_SWING2: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = levelA / 100.0f;
        break;
    }
    case SE_BM_LV_BBEGO_ROT_NEEDLE: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = levelA / 100.0f;
        break;
    }
    case SE_SM_STRAYTICO_GET: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 127);
        writePort(pHandle, 11, levelA);
        break;
    }
    case SE_SM_LV_RABBIT_NEAR: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 127);
        writePort(pHandle, 13, levelA);
        break;
    }
    case SE_SM_LV_RABBIT_NEAR2: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = levelA / 100.0f;
        break;
    }
    case SE_SV_LV_RABBIT_NEAR:
    case SE_SM_LV_RABBIT_RUS_LEAVES:
    case SE_SM_LV_RABBIT_RUS_HOLE: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = levelA / 100.0f;
        if (levelB > 0) {
            levelB = MR::min(levelB, 100);
            pHandle->getSound()->getAuxiliary().moveFxMix(levelB / 100.0f, 0);
        }
        break;
    }
    case SE_SV_LV_RABBIT_NEAR2: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = levelA / 100.0f;
        if (levelB > 0) {
            levelB = MR::min(levelB, 100);
            pHandle->getSound()->getAuxiliary().moveFxMix(levelB / 100.0f, 0);
        }
        break;
    }
    case SE_SM_LV_TICOFAT_EATING:
    case SE_SM_LV_TICOFAT_GLAD: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 127);
        writePort(pHandle, 11, levelA);
        break;
    }
    case SE_EM_STRSPIDER_COLLISION: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = levelA / 100.0f;
        break;
    }
    case SE_EM_LV_STRSPIDER_SPIN: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        levelB = MR::max(levelB, 0);
        levelB = MR::min(levelB, 100);
        volume = levelA / 100.0f;
        writePort(pHandle, 13, levelB);
        break;
    }
    case SE_SY_TRAMPLE_COMBO: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 6);
        writePort(pHandle, 11, levelA);
        break;
    }
    case SE_EM_LV_WATERBAZ_STORM: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = levelA / 100.0f;
        break;
    }
    case SE_EM_LV_MAGKILLER_FLY: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 200);
        pitch = levelA / 100.0f;
        break;
    }
    case SE_BM_LV_ICEMERAKING_ROLL: {
        levelA = MR::max(levelA, 0);
        levelA = MR::min(levelA, 100);
        volume = levelA / 100.0f;
        break;
    }

    default: {
        return;
    }
    }

    pHandle->getSound()->getAuxiliary().moveVolume(volume, steps);
    pHandle->getSound()->getAuxiliary().movePitch(pitch, steps);
}

bool AudSoundObject::modifyLimitedSound_Takezawa(JAISoundID soundID) {
    if (!isPlayingID(soundID)) {
        switch (soundID) {
        case SE_RS_LV_NOTE_TIMER_SLOW:
        case SE_RS_LV_NOTE_TIMER_MIDDLE:
        case SE_RS_LV_NOTE_TIMER_FAST: {
            if (isPlayingID(SE_RS_LV_NOTE_TIMER_SLOW) || isPlayingID(SE_RS_LV_NOTE_TIMER_MIDDLE) || isPlayingID(SE_RS_LV_NOTE_TIMER_FAST)) {
                return true;
            }
            break;
        }
        default:
            return false;
        }
    }
    return false;
}
