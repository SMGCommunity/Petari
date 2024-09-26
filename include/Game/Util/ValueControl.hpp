#pragma once

#include <revolution.h>

class ValueControl {
public:
    ValueControl(int);

    void setDirToOne();
    void setDirToOneResetFrame();
    void setDirToZero();
    void setDirToZeroResetFrame();
    void setMaxFrame(int);
    void update();
    void setZero();
    void setOne();
    f32 getValue() const;
    void resetFrame();

    s32 _0;
    int _4;
    s32 _8;
};