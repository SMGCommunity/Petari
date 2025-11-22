#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class NameObj;
class LiveActor;

class SpinHitController {
public:
    SpinHitController(const NameObj*, s32, bool);
    SpinHitController(const NameObj*, s32, s32, s32, f32, f32, f32, s32, bool);

    void setParameter(const NameObj*, s32, s32, s32, f32, f32, f32, s32, bool);
    void start(LiveActor*, const TVec3f&, const TVec3f&);
    void startWithoutStopScene(LiveActor*, const TVec3f&, const TVec3f&);
    bool execute(LiveActor*);
    void emitItem(LiveActor*);

    s32 _0;
    s32 _4;
    s32 _8;
    f32 _C;
    f32 _10;
    f32 _14;
    bool _18;
    bool _19;
    u32 _1C;
};
