#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Air : public LiveActor {
public:
    Air(const char*);

    virtual ~Air();
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

    u8 _8C;
    bool _8D;
    f32 mDistance;  // 0x90
};

class ProjectionMapAir : public Air {
public:
    ProjectionMapAir(const char*);

    virtual ~ProjectionMapAir();
    virtual void initModel(const char*);
};

class AirFar100m : public Air {
public:
    AirFar100m(const char*);

    virtual ~AirFar100m();
    virtual void setFarClipping();
};

class PriorDrawAir : public Air {
public:
    PriorDrawAir(const char*);

    virtual ~PriorDrawAir();
};

class PriorDrawAirHolder : public NameObj {
public:
    PriorDrawAirHolder();

    virtual ~PriorDrawAirHolder();

    void add(PriorDrawAir*);
    bool isExistValidDrawAir() const;

    PriorDrawAir* mAirs[8];  // 0xC
    s32 mAirCount;           // 0x2C
};

namespace MR {
    bool isExistPriorDrawAir();
};
