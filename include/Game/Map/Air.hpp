#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Air : public LiveActor {
public:
    Air(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void initModel(const char*);
    virtual void setFarClipping();

    bool isDrawing() const;
    bool tryChange();
    void appearFadeOut();
    void appearFadeIn();
    void exeIn();
    void exeOut();

    /* 0x8C */ u8 _8C;
    /* 0x8D */ bool _8D;
    /* 0x90 */ f32 mDistance;
};

class AirFar100m : public Air {
public:
    AirFar100m(const char*);

    virtual void setFarClipping();
};

class ProjectionMapAir : public Air {
public:
    ProjectionMapAir(const char*);

    virtual void initModel(const char*);
};

class PriorDrawAir : public Air {
public:
    PriorDrawAir(const char*);
};

class PriorDrawAirHolder : public NameObj {
public:
    PriorDrawAirHolder();

    void add(PriorDrawAir*);
    bool isExistValidDrawAir() const;

    /* 0x0C */ PriorDrawAir* mAirs[8];
    /* 0x2C */ s32 mAirCount;
};

namespace MR {
    bool isExistPriorDrawAir();
};  // namespace MR
