#ifndef AREAOBJ_H
#define AREAOBJ_H

#include "Actor/NameObj/NameObj.h"

class AreaObj : public NameObj
{
public:
    AreaObj(s32, const char *);
    
    u32* _C; // AreaFormCube*
    s32 _10;
    u8 _14;
    u8 _15;
    u8 _16;
    u8 _17;
    s32 _18; // obj_arg0
    s32 _1C; // obj_arg1
    s32 _20; // obj_arg2
    s32 _24; // obj_arg3
    s32 _28; // obj_arg4
    s32 _2C; // obj_arg5
    s32 _30; // obj_arg6
    s32 _34; // obj_arg7
    u32* _38; // StageSwitchCtrl*
};

#endif // AREAOBJ_H