#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Air : public LiveActor {
public:
    Air(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x78 */ virtual void initModel(const char*);
    /* 0x7C */ virtual void setFarClipping();

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

class AirFar100m : public Air {
public:
    AirFar100m(const char*);

    /* 0x7C */ virtual void setFarClipping();
};

class ProjectionMapAir : public Air {
public:
    ProjectionMapAir(const char*);

    /* 0x78 */ virtual void initModel(const char*);
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

    PriorDrawAir* mAirs[8];  // 0xC
    s32 mAirCount;           // 0x2C
};

namespace MR {
    bool isExistPriorDrawAir();
};  // namespace MR
