#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TornadoMario : public LiveActor {
public: 
    TornadoMario(const char *);

    virtual ~TornadoMario() {

    }

    virtual void init(const JMapInfoIter &);
    virtual void calcAnim();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void hideForce();
    void setTrHeight(const TVec3f &, const TVec3f &, const TVec3f &, const TVec3f &);
    void show();
    void hide();

    bool _8C;
    bool _8D;
    bool _8E;

    // padding/fake variables
    u32 _90;
    u32 _94;
    u32 _98;

    // these are real
    TVec3f _9C;
    TVec3f _A8;
};
