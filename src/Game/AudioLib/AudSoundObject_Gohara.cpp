#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/AudioLib/AudUtil.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Util/MathUtil.hpp"

void AudSoundObject_Gohara_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

void AudSoundObject::modifySe_Gohara(JAISoundHandle* pHandle, s32 levelA, s32 levelB) {
    f32 volume = 1.0f;
    f32 pitch = 1.0f;
    u32 steps = 0;

    switch (pHandle->getSound()->getID()) {
    case SE_BM_BOSS_BUG_BOMB_GROUND: {
        volume = levelA / 100.0f;
        break;
    }
    case SE_BM_BOSS_BUG_BOMB_GROUND_WATER: {
        if (levelA >= 50) {
            volume = levelA / 100.0f;
        } else {
            volume = 0.0f;
        }
        break;
    }
    case SE_BM_LV_BOSS_BUG_FLY_BASE:
    case SE_BM_LV_BOSS_BUG_FLY_ADD:
    case SE_BM_LV_BOSS_BUG_FLY_ROTATE:
    case SE_BM_LV_BOSS_BUG_FLY_ROLL:
    case SE_BM_LV_BOSS_BUG_FLY_ALARM:
    case SE_BM_LV_BOSS_BUG_FLY_BASE2:
    case SE_BM_LV_BOSS_BUG_FLY_SHAKE:
    case SE_BM_LV_BOSS_BUG_FLY_SHAKE2: {
        if (levelB == -1) {
            levelB = 0x7F - 1;
        }
        volume = levelA / 1000.0f;
        writePort(pHandle, 10, levelB);
        break;
    }
    case SE_SY_CTRL_GUIDE_CHANGE:
    case SE_SY_CTRL_GUIDE_CHANGE2:
    case SE_SM_LV_SURF_RAY_CURVE:
    case SE_SM_LV_SURF_RAY_MOVE0:
    case SE_SM_LV_SURF_RAY_MOVE1:
    case SE_SM_LV_SURF_RAY_MOVE2:
    case SE_SM_LV_SURF_RAY_MOVE3:
    case SE_SM_SURF_RAY_JUMPIN1:
    case SE_SM_SURF_RAY_JUMPOUT1:
    case SE_SM_SURF_RAY_MOVE_WIND:
    case SE_SM_SURF_RAY_JUMPIN0: {
        volume = levelA / 1000.0f;
        break;
    }
    case SE_SY_SURF_TUTORIAL_TIMER:
    case SE_SY_SURF_TUTORIAL_TIMER2:
    case SE_SY_SURF_TUTORIAL_TIMER3:
    case SE_SY_SURF_TUTORIAL_TIMER4:
    case SE_SY_SURF_TUTORIAL_GONEXT:
    case SE_SY_SURF_TUTORIAL_GONEXT2: {
        writePort(pHandle, 11, levelA);
        break;
    }
    default: {
        return;
    }
    }

    pHandle->getSound()->getAuxiliary().moveVolume(volume, steps);
    pHandle->getSound()->getAuxiliary().movePitch(pitch, steps);
}
