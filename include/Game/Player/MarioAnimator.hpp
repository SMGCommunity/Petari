#pragma once

#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioModule.hpp"

class MarioActor;
class XanimeResourceTable;
class XanimePlayer;

class MarioAnimator : public MarioModule {
public:
    MarioAnimator(MarioActor *);

    void init();
    void update();

    void setHoming();
    bool isAnimationStop();
    void setSpeed(f32);
    void forceSetBlendWeight(const f32 *);
    void waterToGround();
    void initCallbackTable();
    void change(const char *);
    void changeDefaultUpper(const char *);

    inline void f1(const char *name)
    {
        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);

        _C->setDefaultAnimation(name);
        change(name);
    }

    XanimeResourceTable *_8;
    XanimePlayer *_C;
    XanimePlayer *_10;
    u8 _14;
    u8 _15;
    u8 _16;
    f32 _18;
    f32 _1C;
    f32 _20;
    f32 _24;
    TMtx34f _28;
    f32 _58;
    f32 _5C;
    TVec3f _60;
    u8 _6C;
    f32 _70;
    u32 _74;
    u16 _78;
    TMtx34f _7C;
    TMtx34f _AC;
    TMtx34f _DC;
    u8 _10C;
    u8 _10D;
    u8 _10E;
    f32 _110;
    u32 _114;
    f32 _118;

    u8 _11C[8];
};
