#include "Game/Camera/CameraShaker.hpp"
#include "Game/Camera/CameraShakePatternImpl.hpp"
#include "Game/Camera/CameraShakeTask.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "JSystem/JUtility/JUTVideo.hpp"

namespace {
    static const f32 sInfinityIntensity = 1.0f;
    static const f32 sInfinitySpeed = 15.0f;
}  // namespace

CameraShaker::CameraShaker(const char* pName) : NameObj(pName) {
    createSinglyVerticalTask();
    createSinglyHorizontalTask();
    createInfinityTask();
}

void CameraShaker::movement() {
    updateSinglyVerticalTask();
    updateSinglyHorizontalTask();
    updateInfinityTask();
    setOffset();
}

void CameraShaker::shakeVertical(ESinglyVerticalPower power) {
    if (mVerticalTasks[power]->isEnd()) {
        mVerticalTasks[power]->start(1, 0);
    }
}

void CameraShaker::shakeInfinity(NameObj* pNameObj, f32 intesity, f32 speed) {
    for (u32 i = 0; i < NR_INFINITY_TASKS; i++) {
        if (mInfinityTasks[i]->isEnd()) {
            startInfinity(i, intesity, speed);
            mInfinityNameObjs[i] = pNameObj;
            break;
        }
    }
}

void CameraShaker::stopShakingInfinity(NameObj* pNameObj) {
    for (u32 i = 0; i < NR_INFINITY_TASKS; i++) {
        if (mInfinityNameObjs[i] == pNameObj) {
            mInfinityTasks[i]->endForce();
            mInfinityNameObjs[i] = nullptr;
            break;
        }
    }
}

void CameraShaker::createSinglyVerticalTask() {
    static const f32 cSinglyIntensity[] = {0.08f, 0.2f, 0.5f, 1.0f, 3.0f, 6.0f, 9.0f};

    for (u32 i = 0; i < NR_VERTICAL_TASKS; i++) {
        CameraShakePatternSingly* singly = new CameraShakePatternSingly(cSinglyIntensity[i]);
        CameraShakeTask* task = new CameraShakeTask(singly);

        mVerticalTasks[i] = task;
    }
}

void CameraShaker::createSinglyHorizontalTask() {
    // There is a bug with Nintendo's code.
    // There are 3 horizontal tasks, but the loop runs seven, probably because the
    // count was confused with vertical tasks. The game will read garbage memory from
    // the horizontal float table and will set infinity tasks for values out of range,
    // but it works because the infinity tasks are created afterwards and then set correctly.

    static const f32 cSinglyIntensity[] = {0.3f, 1.0f, 3.0f};

    // Should be i < NR_HORIZONTAL_TASKS
    for (u32 i = 0; i < NR_VERTICAL_TASKS; i++) {
        CameraShakePatternSingly* singly = new CameraShakePatternSingly(cSinglyIntensity[i]);

        singly->setDirection(TVec2f(1.0f, 0.0f));

        CameraShakeTask* task = new CameraShakeTask(singly);

        mHorizontalTasks[i] = task;
    }
}

void CameraShaker::createInfinityTask() {
    for (u32 i = 0; i < NR_INFINITY_TASKS; i++) {
        CameraShakePatternVerticalSin* sin = new CameraShakePatternVerticalSin(::sInfinityIntensity, sInfinitySpeed);
        CameraShakeTask* task = new CameraShakeTask(sin);

        mInfinityTasks[i] = task;
        mInfinityNameObjs[i] = nullptr;
    }
}

void CameraShaker::updateSinglyVerticalTask() {
    for (u32 i = 0; i < NR_VERTICAL_TASKS; i++) {
        mVerticalTasks[i]->movement();
    }
}

void CameraShaker::updateSinglyHorizontalTask() {
    for (u32 i = 0; i < NR_HORIZONTAL_TASKS; i++) {
        mHorizontalTasks[i]->movement();
    }
}

void CameraShaker::updateInfinityTask() {
    for (u32 i = 0; i < NR_INFINITY_TASKS; i++) {
        mInfinityTasks[i]->movement();
    }
}

void CameraShaker::setOffset() {
    TVec2f offset = TVec2f(0.0f, 0.0f);

    addSinglyVerticalTaskOffset(&offset);
    addSinglyHorizontalTaskOffset(&offset);
    addInfinityTaskOffset(&offset);
    adjustOffsetToScreen(&offset);
    MR::setShakeOffset(offset.x, offset.y);
}

void CameraShaker::addSinglyVerticalTaskOffset(TVec2f* pOffset) {
    for (u32 i = 0; i < NR_VERTICAL_TASKS; i++) {
        if (!mVerticalTasks[i]->isEnd()) {
            TVec2f offset;
            mVerticalTasks[i]->getOffset(&offset);

            pOffset->x += offset.x;
            pOffset->y += offset.y;
        }
    }
}

void CameraShaker::addSinglyHorizontalTaskOffset(TVec2f* pOffset) {
    for (u32 i = 0; i < NR_HORIZONTAL_TASKS; i++) {
        if (!mHorizontalTasks[i]->isEnd()) {
            TVec2f offset;
            mHorizontalTasks[i]->getOffset(&offset);

            pOffset->x += offset.x;
            pOffset->y += offset.y;
        }
    }
}

void CameraShaker::addInfinityTaskOffset(TVec2f* pOffset) {
    for (u32 i = 0; i < NR_INFINITY_TASKS; i++) {
        if (!mInfinityTasks[i]->isEnd()) {
            TVec2f offset;
            mInfinityTasks[i]->getOffset(&offset);

            pOffset->x += offset.x;
            pOffset->y += offset.y;
        }
    }
}

void CameraShaker::adjustOffsetToScreen(TVec2f* pOffset) {
    pOffset->y *= 30.0f / (s32)JUTVideo::getManager()->getEfbHeight();
    pOffset->x *= 30.0f / MR::getScreenWidth();
}

void CameraShaker::startInfinity(u32 index, f32 intesity, f32 speed) {
    CameraShakePatternVerticalSin* sin = static_cast< CameraShakePatternVerticalSin* >(mInfinityTasks[index]->mPattern);

    sin->mIntensity = intesity;
    sin->mSpeed = speed;

    mInfinityTasks[index]->startInfinity(0);
}
