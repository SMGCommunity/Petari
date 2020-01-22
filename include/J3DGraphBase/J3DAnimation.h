#ifndef J3DANIMATION_H
#define J3DANIMATION_H

#include "types.h"

class J3DAnmBase
{
public:
    J3DAnmBase();

    virtual ~J3DAnmBase();

    u8 _4;
    u8 _5;
    u16 _6;
    f32 _8;
};

class J3DFrameCtrl
{
public:
    virtual ~J3DFrameCtrl();

    void init(s16);
    bool checkPass(f32);
    void update();

    u8 _4;
    u8 _5;
    u16 _6;
    u16 _8;
    u16 _A;
    f32 _C;
    f32 _10;
};

#endif // J3DANIMATION_H