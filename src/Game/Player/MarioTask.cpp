#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Util/MathUtil.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

extern "C" {
extern u8 lbl_806B6288;
}

static const f32 sOne = 1.0f;
static const f32 sZero = 0.0f;
static const f32 sSameDirEpsilon = 0.01f;
static const f32 sNearZeroEpsilon = 0.001f;
static const f32 sHipDropSlideAngle = 0.7853982f;
static const f32 sHipDropSlideRot = 0.10471976f;
static const f32 sHipDropSlideLen = 10.0f;
static const f32 sJumpDropSlideDotMin = 0.1f;
static const f32 sHopperJumpSlideDotMin = 0.707f;

static Mario::Task sTaskHandy = &Mario::taskOnHandy;
static Mario::Task sTaskHipDropBlurHopper = &Mario::taskOnHipDropBlurHopper;
static Mario::Task sTaskHipDropBlur = &Mario::taskOnHipDropBlur;
static Mario::Task sTaskHipDropSlide = &Mario::taskOnHipDropSlide;
static Mario::Task sTaskJumpDropSlide = &Mario::taskOnHipDropSlide;
static struct {
    Mario::Task task;
    u8 gap_07_805CCD70_data[0xA0];
} sTaskFreezeEnd = {&Mario::taskOnFreezeEnd};

void Mario::delTask(MarioModuleTask* pTask) {
    MarioModuleTask* next = pTask->mNext;
    if (!next) {
        return;
    }

    MarioModuleTask* head = _974;
    if (head == pTask) {
        if (next == pTask) {
            _974 = nullptr;
        } else {
            _974 = next;
        }
        pTask->mNext = nullptr;
        pTask->end();
        return;
    }

    while (true) {
        if (head == nullptr) {
            return;
        }

        MarioModuleTask* headNext = head->mNext;
        if (headNext == pTask) {
            if (next == pTask) {
                head->mNext = head;
            } else {
                head->mNext = next;
            }
            pTask->mNext = nullptr;
            pTask->end();
            return;
        }

        if (headNext == head) {
            return;
        }
    }
}

void Mario::execTask() {
    MarioModuleTask* task = _974;

    while (true) {
        if (task == nullptr) {
            return;
        }

        MarioModuleTask* next = task->mNext;
        if (!task->exec()) {
            delTask(task);
        }

        if (next == task) {
            return;
        }

        task = next;
    }
}

void Mario::drawTask() const {
    MarioModuleTask* task = _974;

    while (true) {
        if (task == nullptr) {
            return;
        }

        MarioModuleTask* next = task->mNext;
        task->draw();
        if (next == task) {
            return;
        }

        task = next;
    }
}

void Mario::initTask() {
    for (u32 i = 0; i < ARRAY_SIZE(_984); i++) {
        _984[i] = nullptr;
    }

    _974 = nullptr;
}

bool Mario::isActiveTask(Task task) {
    for (u32 i = 0; i < ARRAY_SIZE(_984); i++) {
        if (_984[i] == task) {
            return true;
        }
    }

    return false;
}

bool Mario::isActiveTaskID(u32 id) {
    for (u32 i = 0; i < ARRAY_SIZE(_984); i++) {
        if (_984[i] != nullptr) {
            if (_A08[i] & id) {
                return true;
            }
        }
    }

    return false;
}

bool Mario::pushTask(Task task, u32 flags) {
    if (isActiveTask(task)) {
        return false;
    }

    u32 index = 0;
    for (; index < ARRAY_SIZE(_984); index++) {
        if (_984[index] == nullptr) {
            break;
        }
    }

    _984[index] = task;
    _A08[index] = flags;
    return true;
}

void Mario::popTask(Task task) {
    for (u32 i = 0; i < ARRAY_SIZE(_984); i++) {
        if (_984[i] == task) {
            _984[i] = nullptr;
            return;
        }
    }
}

void Mario::callExtraTasks(u32 flags) {
    execTask();

    for (u32 i = 0; i < ARRAY_SIZE(_984); i++) {
        if (_984[i] == nullptr) {
            continue;
        }

        if (flags != 0 && !(flags & _A08[i])) {
            continue;
        }

        if (!(this->*_984[i])(_A08[i])) {
            Task task = _984[i];
            popTask(task);
        }
    }
}

void Mario::startHandy() {
    pushTask(sTaskHandy, 0x40);
}

bool Mario::taskOnHipDropBlurHopper(u32) {
    if (!getPlayer()->mMovementStates._B || mMovementStates._1 || isStatusActive(MarioStatus_Swim)) {
        if (gIsLuigi) {
            stopEffect("ホッパー尻落ルイージ");
        } else {
            stopEffect("ホッパー尻落");
        }
        return false;
    }

    return true;
}

bool Mario::taskOnHipDropBlur(u32) {
    if (!getPlayer()->mMovementStates._B || mMovementStates._1 || isStatusActive(MarioStatus_Swim)) {
        if (gIsLuigi) {
            stopEffect("尻落ルイージ");
        } else {
            stopEffect("尻落");
        }
        return false;
    }

    return true;
}

bool Mario::taskOnHipDropSlide(u32 flags) {
    if (flags == 0x100) {
        if (!getPlayer()->mMovementStates._B) {
            return false;
        }
    }

    if (MR::diffAngleAbs(_70C, getAirGravityVec()) > sHipDropSlideAngle) {
        return false;
    }

    if (getPlayer()->mMovementStates._1 || !getPlayer()->mMovementStates.jumping || isStatusActive(MarioStatus_Swim)) {
        return false;
    }

    HitSensor* sensor = _A68;
    bool enabled = sensor->mValidByHost != 0 && sensor->mValidBySystem != 0;

    if (!enabled) {
        return false;
    }

    TVec3f dir(mPosition);
    dir -= _A4C;
    dir.setLength(_A64);

    TVec3f dirCopy(dir);
    MR::normalizeOrZero(&dir);

    TVec3f reverseGravity = -_70C;
    if (dir.dot(reverseGravity) <= sZero) {
        return false;
    }

    Mtx rot;
    PSMTXRotAxisRad(rot, &_A58, sHipDropSlideRot);
    PSMTXMultVecSR(rot, &dir, &dir);
    dir.setLength(_A64);
    dir -= dirCopy;
    dir.setLength(sHipDropSlideLen);

    cutGravityElementFromJumpVec(true);
    cutVecElementFromJumpVec(_70C);
    addVelocity(dir);

    dirCopy.setLength(sOne);
    addVelocity(dirCopy);
    return true;
}

bool Mario::taskOnFreezeEnd(u32) {
    return mActor->finalizeFreezeModel();
}

void Mario::startFreezeEnd() {
    pushTask(sTaskFreezeEnd.task, 0x800);
}

bool Mario::taskOnHandy(u32) {
    if (!mActor->_468) {
        stopEffect("いい汗");
        return false;
    }

    if (mTargetWalkSpeedIndex > 2) {
        playEffect("いい汗");
    } else {
        stopEffect("いい汗");
    }

    return true;
}

void Mario::startHipDropBlur() {
    if (isPlayerModeHopper()) {
        if (lbl_806B6288) {
            playEffect("ホッパー尻落ルイージ");
        } else {
            playEffect("ホッパー尻落");
        }
        pushTask(sTaskHipDropBlurHopper, 0x80);
        return;
    }

    if (lbl_806B6288) {
        playEffect("尻落ルイージ");
    } else {
        playEffect("尻落");
    }
    pushTask(sTaskHipDropBlur, 0x80);
}

void Mario::startHipDropSlide(const HitSensor* pSensor) {
    if (isActiveTaskID(0x100)) {
        return;
    }

    if (isActiveTaskID(0x200)) {
        return;
    }

    pushTask(sTaskHipDropSlide, 0x100);

    TVec3f dir(mPosition - pSensor->mPosition);

    if (MR::isSameDirection(dir, getAirGravityVec(), sSameDirEpsilon) || MR::isNearZero(dir, sNearZeroEpsilon)) {
        dir = -mFrontVec;
    }

    MR::normalizeOrZero(&dir);

    _A58.cross(-getAirGravityVec(), dir);
    MR::normalizeOrZero(&_A58);

    _A68 = const_cast< HitSensor* >(pSensor);
    _A4C = pSensor->mPosition;
    _A64 = pSensor->mRadius;
    _70C = getAirGravityVec();

    changeAnimation("ヒップドロップ滑り", static_cast< const char* >(nullptr));
}

void Mario::startJumpDropSlide(const HitSensor* pSensor) {
    if (isActiveTaskID(0x100)) {
        return;
    }

    if (isActiveTaskID(0x200)) {
        return;
    }

    TVec3f dir(mPosition - pSensor->mPosition);
    MR::normalizeOrZero(&dir);

    f32 dot = dir.dot(-getAirGravityVec());
    if (dot <= sJumpDropSlideDotMin) {
        return;
    }

    pushTask(sTaskJumpDropSlide, 0x200);

    TVec3f slideDir(mPosition - pSensor->mPosition);

    if (MR::isSameDirection(slideDir, getAirGravityVec(), sSameDirEpsilon) || MR::isNearZero(slideDir, sNearZeroEpsilon)) {
        slideDir = -mFrontVec;
    }

    MR::normalizeOrZero(&slideDir);

    _A58.cross(-getAirGravityVec(), slideDir);
    MR::normalizeOrZero(&_A58);

    _A68 = const_cast< HitSensor* >(pSensor);
    _A4C = pSensor->mPosition;
    _A64 = pSensor->mRadius;
    _70C = getAirGravityVec();

    if (isPlayerModeHopper()) {
        if (dot > sHopperJumpSlideDotMin) {
            f32 frontDot = mFrontVec.dot(slideDir);
            if (frontDot > sZero) {
                changeAnimation("ジャンプ順滑り", static_cast< const char* >(nullptr));
            } else {
                changeAnimation("ジャンプ逆滑り", static_cast< const char* >(nullptr));
            }
        }
    }

    mJumpVec.x = sZero;
    mJumpVec.y = sZero;
    mJumpVec.z = sZero;
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
