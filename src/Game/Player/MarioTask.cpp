#include "Game/Player/Mario.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/MathUtil.hpp"
#include "revolution/mtx.h"

struct PTMF {
    long this_delta;
    long vtbl_offset;
    long func_data;
};

extern "C" {
    extern const PTMF __ptmf_null;
    long __ptmf_test(const PTMF* ptmf);
    long __ptmf_cmpr(const PTMF* ptmf1, const PTMF* ptmf2);
}

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
} sTaskFreezeEnd = { &Mario::taskOnFreezeEnd };

class MarioModuleTask {
public:
    virtual void init() = 0;
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void calc() = 0;
    virtual void end() = 0;
    virtual bool exec() = 0;
    virtual void draw() const = 0;

    void* _4;
    MarioModuleTask* mNext;
};

void Mario::delTask(MarioModuleTask* pTask) {
    MarioModuleTask* next = pTask->mNext;
    if (!next) {
        return;
    }

    MarioModuleTask* head = _974;
    if (head == pTask) {
        _974 = (next == pTask) ? nullptr : next;
        pTask->mNext = nullptr;
        pTask->end();
        return;
    }

    if (!head) {
        return;
    }

    while (true) {
        MarioModuleTask* headNext = head->mNext;
        if (headNext == pTask) {
            head->mNext = (next == pTask) ? head : next;
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
    if (!task) {
        return;
    }

    while (true) {
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
    if (!task) {
        return;
    }

    while (true) {
        MarioModuleTask* next = task->mNext;
        task->draw();
        if (next == task) {
            return;
        }

        task = next;
    }
}

void Mario::initTask() {
    register u32 offset = 0;
    register long value1;
    register long value0;
    const volatile PTMF* src = &__ptmf_null;
    for (u32 i = 0; i < 11; i++, offset += sizeof(PTMF)) {
        value1 = src->this_delta;
        PTMF* dst = reinterpret_cast<PTMF*>(reinterpret_cast<u8*>(this) + offset + 0x984);
        value0 = src->vtbl_offset;
        dst->this_delta = value1;
        dst->vtbl_offset = value0;
        dst->func_data = src->func_data;
    }

    _974 = nullptr;
}

bool Mario::isActiveTask(Task task) {
    for (int i = 0; i < 11; i++) {
        if (__ptmf_cmpr(reinterpret_cast<const PTMF*>(&_984[i]), reinterpret_cast<const PTMF*>(&task)) == 0) {
            return true;
        }
    }

    return false;
}

bool Mario::isActiveTaskID(u32 id) {
    for (int i = 0; i < 11; i++) {
        if (__ptmf_test(reinterpret_cast<const PTMF*>(&_984[i]))) {
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

    int index = 0;
    for (; index < 11; index++) {
        if (!__ptmf_test(reinterpret_cast<const PTMF*>(&_984[index]))) {
            break;
        }
    }

    _984[index] = task;
    _A08[index] = flags;
    return true;
}

void Mario::popTask(Task task) {
    for (int i = 0; i < 11; i++) {
        if (__ptmf_cmpr(reinterpret_cast<const PTMF*>(&_984[i]), reinterpret_cast<const PTMF*>(&task)) == 0) {
            reinterpret_cast<PTMF&>(_984[i]) = __ptmf_null;
            return;
        }
    }
}

void Mario::callExtraTasks(u32 flags) {
    execTask();

    for (int i = 0; i < 11; i++) {
        if (!__ptmf_test(reinterpret_cast<const PTMF*>(&_984[i]))) {
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
    Mario* player = getPlayer();
    if (player->mMovementStates_LOW_WORD & 0x00080000) {
        if (!(mMovementStates_LOW_WORD & 0x20000000)) {
            if (!isStatusActive(6)) {
                return true;
            }
        }
    }

    if (lbl_806B6288) {
        stopEffect("ホッパー尻落ルイージ");
    } else {
        stopEffect("ホッパー尻落");
    }

    return false;
}

bool Mario::taskOnHipDropBlur(u32) {
    Mario* player = getPlayer();
    if (player->mMovementStates_LOW_WORD & 0x00080000) {
        if (!(mMovementStates_LOW_WORD & 0x20000000)) {
            if (!isStatusActive(6)) {
                return true;
            }
        }
    }

    if (lbl_806B6288) {
        stopEffect("尻落ルイージ");
    } else {
        stopEffect("尻落");
    }

    return false;
}

bool Mario::taskOnHipDropSlide(u32 flags) {
    if (flags == 0x100) {
        Mario* player = getPlayer();
        if (!(player->mMovementStates_LOW_WORD & 0x00080000)) {
            return false;
        }
    }

    if (MR::diffAngleAbs(_70C, getAirGravityVec()) > sHipDropSlideAngle) {
        return false;
    }

    Mario* player = getPlayer();
    if (player->mMovementStates_LOW_WORD & 0x20000000) {
        return false;
    }

    player = getPlayer();
    if (!(player->mMovementStates_LOW_WORD & 0x80000000)) {
        return false;
    }

    if (isStatusActive(6)) {
        return false;
    }

    HitSensor* sensor = _A68;
    bool enabled = false;
    if (sensor->mValidByHost != 0 && sensor->mValidBySystem != 0) {
        enabled = true;
    }

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
    PSMTXRotAxisRad(rot, reinterpret_cast<const Vec*>(&_A58), sHipDropSlideRot);
    PSMTXMultVecSR(rot, reinterpret_cast<const Vec*>(&dir), reinterpret_cast<Vec*>(&dir));
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

    if (_71C > 2) {
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

    TVec3f dir(mPosition);
    dir -= pSensor->mPosition;

    if (MR::isSameDirection(dir, getAirGravityVec(), sSameDirEpsilon)) {
        TVec3f tmp = -mFrontVec;
        dir = tmp;
    } else if (MR::isNearZero(dir, sNearZeroEpsilon)) {
        TVec3f tmp = -mFrontVec;
        dir = tmp;
    }

    MR::normalizeOrZero(&dir);

    TVec3f reverseGravity = -getAirGravityVec();
    PSVECCrossProduct(reinterpret_cast<const Vec*>(&reverseGravity), reinterpret_cast<const Vec*>(&dir), reinterpret_cast<Vec*>(&_A58));
    MR::normalizeOrZero(&_A58);

    _A68 = const_cast<HitSensor*>(pSensor);
    _A4C = pSensor->mPosition;
    _A64 = pSensor->mRadius;
    _70C = getAirGravityVec();

    changeAnimation("ヒップドロップ滑り", (const char*)nullptr);
}

void Mario::startJumpDropSlide(const HitSensor* pSensor) {
    if (isActiveTaskID(0x100)) {
        return;
    }

    if (isActiveTaskID(0x200)) {
        return;
    }

    TVec3f dir(mPosition);
    dir -= pSensor->mPosition;
    MR::normalizeOrZero(&dir);

    TVec3f reverseGravity = -getAirGravityVec();
    f32 dot = dir.dot(reverseGravity);
    if (dot <= sJumpDropSlideDotMin) {
        return;
    }

    pushTask(sTaskJumpDropSlide, 0x200);

    TVec3f slideDir(mPosition);
    slideDir -= pSensor->mPosition;

    if (MR::isSameDirection(slideDir, getAirGravityVec(), sSameDirEpsilon)) {
        TVec3f tmp = -mFrontVec;
        slideDir = tmp;
    } else if (MR::isNearZero(slideDir, sNearZeroEpsilon)) {
        TVec3f tmp = -mFrontVec;
        slideDir = tmp;
    }

    MR::normalizeOrZero(&slideDir);

    TVec3f slideReverseGravity = -getAirGravityVec();
    PSVECCrossProduct(reinterpret_cast<const Vec*>(&slideReverseGravity), reinterpret_cast<const Vec*>(&slideDir), reinterpret_cast<Vec*>(&_A58));
    MR::normalizeOrZero(&_A58);

    _A68 = const_cast<HitSensor*>(pSensor);
    _A4C = pSensor->mPosition;
    _A64 = pSensor->mRadius;
    _70C = getAirGravityVec();

    if (isPlayerModeHopper()) {
        if (dot > sHopperJumpSlideDotMin) {
            f32 frontDot = mFrontVec.dot(slideDir);
            if (frontDot > sZero) {
                changeAnimation("ジャンプ順滑り", (const char*)nullptr);
            } else {
                changeAnimation("ジャンプ逆滑り", (const char*)nullptr);
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
