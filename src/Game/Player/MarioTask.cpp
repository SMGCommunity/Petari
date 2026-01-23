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
    extern u8 sInstance__Q213NrvMarioActor17MarioActorNrvWait;
    extern u8 sInstance__Q213NrvMarioActor21MarioActorNrvGameOver;
    extern u8 sInstance__Q213NrvMarioActor26MarioActorNrvGameOverAbyss;
    extern u8 sInstance__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2;
    extern u8 sInstance__Q213NrvMarioActor25MarioActorNrvGameOverFire;
    extern u8 sInstance__Q213NrvMarioActor30MarioActorNrvGameOverBlackHole;
    extern u8 sInstance__Q213NrvMarioActor28MarioActorNrvGameOverNonStop;
    extern u8 sInstance__Q213NrvMarioActor25MarioActorNrvGameOverSink;
    extern u8 sInstance__Q213NrvMarioActor21MarioActorNrvTimeWait;
    extern u8 sInstance__Q213NrvMarioActor19MarioActorNrvNoRush;

    void __ct__Q213NrvMarioActor17MarioActorNrvWaitFv(void*);
    void __ct__Q213NrvMarioActor21MarioActorNrvGameOverFv(void*);
    void __ct__Q213NrvMarioActor26MarioActorNrvGameOverAbyssFv(void*);
    void __ct__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2Fv(void*);
    void __ct__Q213NrvMarioActor25MarioActorNrvGameOverFireFv(void*);
    void __ct__Q213NrvMarioActor30MarioActorNrvGameOverBlackHoleFv(void*);
    void __ct__Q213NrvMarioActor28MarioActorNrvGameOverNonStopFv(void*);
    void __ct__Q213NrvMarioActor25MarioActorNrvGameOverSinkFv(void*);
    void __ct__Q213NrvMarioActor21MarioActorNrvTimeWaitFv(void*);
    void __ct__Q213NrvMarioActor19MarioActorNrvNoRushFv(void*);
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
char lbl_805CCCC4[] = "いい汗\0";
static Mario::Task sTaskHipDropBlurHopper = &Mario::taskOnHipDropBlurHopper;
static Mario::Task sTaskHipDropBlur = &Mario::taskOnHipDropBlur;
char lbl_805CCCE4[] = "ホッパー尻落ルイージ";
char lbl_805CCCF9[] = "ホッパー尻落";
char lbl_805CCD06[] = "尻落ルイージ";
char lbl_805CCD13[] = "尻落";
static Mario::Task sTaskHipDropSlide = &Mario::taskOnHipDropSlide;
char lbl_805CCD24[] = "ヒップドロップ滑り\0";
static Mario::Task sTaskJumpDropSlide = &Mario::taskOnHipDropSlide;
char lbl_805CCD44[] = "ジャンプ順滑り\0ジャンプ逆滑り\0\0";
static struct {
    Mario::Task task;
    u8 gap_07_805CCD70_data[0xA0];
} sTaskFreezeEnd = { &Mario::taskOnFreezeEnd };

char lbl_805CCE10[] = "テレサ消える\0声トルネード\0CS_TERESA";
char lbl_805CCE34[] = "落下";
char lbl_805CCE39[] = "テレサ風に乗る\0テレサ壁反射\0声壁反射\0WallHit";
char lbl_805CCE66[] = "テレサ踏ん張り";
char lbl_805CCE75[] = "fallquicklystart";
char lbl_805CCE86[] = "声壁押し";
char lbl_805CCE8F[] = "基本";
char lbl_805CCE94[] = "blink\0sleep\0Sleep\0wait\0spin\0fall\0テレサ現れる";
char lbl_805CCEC2[] = "基本\0";

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
        stopEffect(lbl_805CCCE4);
    } else {
        stopEffect(lbl_805CCCF9);
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
        stopEffect(lbl_805CCD06);
    } else {
        stopEffect(lbl_805CCD13);
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
        stopEffect(lbl_805CCCC4);
        return false;
    }

    if (_71C > 2) {
        playEffect(lbl_805CCCC4);
    } else {
        stopEffect(lbl_805CCCC4);
    }

    return true;
}

void Mario::startHipDropBlur() {
    if (isPlayerModeHopper()) {
        if (lbl_806B6288) {
            playEffect(lbl_805CCCE4);
        } else {
            playEffect(lbl_805CCCF9);
        }
        pushTask(sTaskHipDropBlurHopper, 0x80);
        return;
    }

    if (lbl_806B6288) {
        playEffect(lbl_805CCD06);
    } else {
        playEffect(lbl_805CCD13);
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

    changeAnimation(lbl_805CCD24, (const char*)nullptr);
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
                changeAnimation(lbl_805CCD44, (const char*)nullptr);
            } else {
                changeAnimation(lbl_805CCD44 + 0x0f, (const char*)nullptr);
            }
        }
    }

    mJumpVec.x = sZero;
    mJumpVec.y = sZero;
    mJumpVec.z = sZero;
}

extern "C" void __sinit_MarioTask_cpp() {
    __ct__Q213NrvMarioActor17MarioActorNrvWaitFv(&sInstance__Q213NrvMarioActor17MarioActorNrvWait);
    __ct__Q213NrvMarioActor21MarioActorNrvGameOverFv(&sInstance__Q213NrvMarioActor21MarioActorNrvGameOver);
    __ct__Q213NrvMarioActor26MarioActorNrvGameOverAbyssFv(&sInstance__Q213NrvMarioActor26MarioActorNrvGameOverAbyss);
    __ct__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2Fv(&sInstance__Q213NrvMarioActor27MarioActorNrvGameOverAbyss2);
    __ct__Q213NrvMarioActor25MarioActorNrvGameOverFireFv(&sInstance__Q213NrvMarioActor25MarioActorNrvGameOverFire);
    __ct__Q213NrvMarioActor30MarioActorNrvGameOverBlackHoleFv(&sInstance__Q213NrvMarioActor30MarioActorNrvGameOverBlackHole);
    __ct__Q213NrvMarioActor28MarioActorNrvGameOverNonStopFv(&sInstance__Q213NrvMarioActor28MarioActorNrvGameOverNonStop);
    __ct__Q213NrvMarioActor25MarioActorNrvGameOverSinkFv(&sInstance__Q213NrvMarioActor25MarioActorNrvGameOverSink);
    __ct__Q213NrvMarioActor21MarioActorNrvTimeWaitFv(&sInstance__Q213NrvMarioActor21MarioActorNrvTimeWait);
    __ct__Q213NrvMarioActor19MarioActorNrvNoRushFv(&sInstance__Q213NrvMarioActor19MarioActorNrvNoRush);
}

__declspec(section ".ctors") extern void* const __sinit_MarioTask_cpp_reference = __sinit_MarioTask_cpp;
