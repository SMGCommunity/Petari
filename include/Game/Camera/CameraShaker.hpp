#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class CameraShakeTask;

#define NR_VERTICAL_TASKS 7
#define NR_HORIZONTAL_TASKS 3
#define NR_INFINITY_TASKS 4

class CameraShaker : public NameObj {
public:
    enum ESinglyVerticalPower {
        SINGLY_VERTICAL_POWER_VERY_WEAK = 0,
        SINGLY_VERTICAL_POWER_WEAK = 1,
        SINGLY_VERTICAL_POWER_NORMAL_WEAK = 2,
        SINGLY_VERTICAL_POWER_NORMAL = 3,
        SINGLY_VERTICAL_POWER_NORMAL_STRONG = 4,
        SINGLY_VERTICAL_POWER_STRONG = 5,
        SINGLY_VERTICAL_POWER_VERY_STRONG = 6
    };

    CameraShaker(const char*);
    virtual ~CameraShaker();

    virtual void movement();

    void shakeVertical(ESinglyVerticalPower);
    void shakeInfinity(NameObj*, f32, f32);
    void stopShakingInfinity(NameObj*);
    void createSinglyVerticalTask();
    void createSinglyHorizontalTask();
    void createInfinityTask();
    void updateSinglyVerticalTask();
    void updateSinglyHorizontalTask();
    void updateInfinityTask();
    void setOffset();
    void addSinglyVerticalTaskOffset(TVec2f*);
    void addSinglyHorizontalTaskOffset(TVec2f*);
    void addInfinityTaskOffset(TVec2f*);
    void adjustOffsetToScreen(TVec2f*);  // TODO
    void startInfinity(u32, f32, f32);

    CameraShakeTask* mVerticalTasks[NR_VERTICAL_TASKS];      // 0xC
    CameraShakeTask* mHorizontalTasks[NR_HORIZONTAL_TASKS];  // 0x28
    CameraShakeTask* mInfinityTasks[NR_INFINITY_TASKS];      // 0x34
    NameObj* mInfinityNameObjs[NR_INFINITY_TASKS];           // 0x44
};