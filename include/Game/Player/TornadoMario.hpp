#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TornadoMario : public LiveActor {
public:
    TornadoMario(const char* pName);

    virtual ~TornadoMario();
    virtual void init(const JMapInfoIter& rIter) override;
    virtual void calcAnim() override;
    virtual void control() override;
    virtual void calcAndSetBaseMtx() override;

    void hideForce();
    void setTrHeight(const TVec3f& rPos, const TVec3f& rFront, const TVec3f& rUp, const TVec3f& rShadowDir);
    void show();
    void hide();

    /* 0x8C */ bool _8C;
    /* 0x8D */ bool mIsVisible;
    /* 0x8E */ bool _8E;
    /* 0x8F */ bool _8F;
    /* 0x90 */ s32 _90;
    /* 0x94 */ s32 _94;
    /* 0x98 */ s32 _98;
    /* 0x9C */ TVec3f mUp;
    /* 0xA8 */ TVec3f mFront;
};
