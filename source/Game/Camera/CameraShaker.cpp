#include "Game/Camera/CameraShaker.h"
#include "Game/Camera/CameraShakePatternImpl.h"
#include "Game/Camera/CameraShakeTask.h"
#include "Game/Util/CameraUtil.h"

f32 sVerticalTaskTable[7] = {
    0.08f,
    0.2f,
    0.5f,
    1.0f,
    3.0f,
    6.0f,
    9.0f
};

f32 sHorizontalTable[3] = {
    0.3f,
    1.0f,
    3.0f
};

CameraShaker::CameraShaker(const char *pName) : NameObj(pName) {
    createSinglyVerticalTask();
    createSinglyHorizontalTask();
    createInfinityTask();
}

CameraShaker::~CameraShaker() {

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

void CameraShaker::shakeInfinity(NameObj *pNameObj, float a2, float a3) {
    for (u32 i = 0; i < NR_INFINITY_TASKS; i++) {
        if (mInfinityTasks[i]->isEnd()) {
            startInfinity(i, a2, a3);
            mInfinityNameObjs[i] = pNameObj;
            break;
        }
    }
}

void CameraShaker::stopShakingInfinity(NameObj *pNameObj) {
    for (u32 i = 0; i < NR_INFINITY_TASKS; i++) {
        if (mInfinityNameObjs[i] == pNameObj) {
            mInfinityTasks[i]->endForce();
            mInfinityNameObjs[i] = nullptr;
            break;
        }
    }
}

void CameraShaker::createSinglyVerticalTask() {
    for (u32 i = 0; i < NR_VERTICAL_TASKS; i++) {
        CameraShakePatternSingly *singly = new CameraShakePatternSingly(sVerticalTaskTable[i]);
        CameraShakeTask *task = new CameraShakeTask(singly);

        mVerticalTasks[i] = task;
    }
}

void CameraShaker::createSinglyHorizontalTask() {
    // There is a bug with Nintendo's code.
    // There are 3 horizontal tasks, but the loop runs seven, probably because the
    // count was confused with vertical tasks. The game will read garbage memory from
    // the horizontal float table and will set infinity tasks for values out of range,
    // but it works because the infinity tasks are created afterwards and then set correctly.

    // Should be i < NR_HORIZONTAL_TASKS
    for (u32 i = 0; i < NR_VERTICAL_TASKS; i++) {
        CameraShakePatternSingly *singly = new CameraShakePatternSingly(sHorizontalTable[i]);

        TVec2f dir;
        dir.x = 1.0f;
        dir.y = 0.0f;

        singly->setDirection(dir);

        CameraShakeTask *task = new CameraShakeTask(singly);

        mHorizontalTasks[i] = task;
    }
}

void CameraShaker::createInfinityTask() {
    for (u32 i = 0; i < NR_INFINITY_TASKS; i++) {
        CameraShakePatternVerticalSin *sin = new CameraShakePatternVerticalSin(1.0f, 15.0f);
        CameraShakeTask *task = new CameraShakeTask(sin);

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

void CameraShaker::addSinglyVerticalTaskOffset(TVec2f *pOffset) {
    for (u32 i = 0; i < NR_VERTICAL_TASKS; i++) {
        if (!mVerticalTasks[i]->isEnd()) {
            TVec2f offset;
            mVerticalTasks[i]->getOffset(&offset);

            pOffset->x += offset.x;
            pOffset->y += offset.y;
        }
    }
}

void CameraShaker::addSinglyHorizontalTaskOffset(TVec2f *pOffset) {
    for (u32 i = 0; i < NR_HORIZONTAL_TASKS; i++) {
        if (!mHorizontalTasks[i]->isEnd()) {
            TVec2f offset;
            mHorizontalTasks[i]->getOffset(&offset);

            pOffset->x += offset.x;
            pOffset->y += offset.y;
        }
    }
}

void CameraShaker::addInfinityTaskOffset(TVec2f *pOffset) {
    for (u32 i = 0; i < NR_INFINITY_TASKS; i++) {
        if (!mInfinityTasks[i]->isEnd()) {
            TVec2f offset;
            mInfinityTasks[i]->getOffset(&offset);

            pOffset->x += offset.x;
            pOffset->y += offset.y;
        }
    }
}

void CameraShaker::startInfinity(unsigned long index, float a2, float a3) {
    CameraShakePatternVerticalSin *sin = reinterpret_cast<CameraShakePatternVerticalSin *>(mInfinityTasks[index]->mPattern);

    sin->_8 = a2;
    sin->_C = a3;

    mInfinityTasks[index]->startInfinity(0);
}