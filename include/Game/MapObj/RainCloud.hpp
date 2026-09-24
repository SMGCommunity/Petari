#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class LodCtrl;
class MapPartsRailMover;
class ModelObj;
class NameObjArchiveListCollector;

class RainCloud : public LiveActor {
public:
    RainCloud(const char* pName);

    virtual ~RainCloud();
    virtual void init(const JMapInfoIter& rIter);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeAppear();
    void exeWait();
    void exeDisappear();
    void exeEnd();
    void exeSoftTouch();
    void exeHardTouch();
    void switchEffect();
    void updateRainCylinder();
    bool isNextStartOK() const;

    static void makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter);

    /* 0x8C */ ModelObj* mCloudCylinder;
    /* 0x90 */ TPos3f mRainCylinderMtx;
    /* 0xC0 */ TPos3f mSplashMtx;
    /* 0xF0 */ MapPartsRailMover* mRailMover;
    /* 0xF4 */ LodCtrl* mLodCtrl;
    /* 0xF8 */ TVec3f mClippingCenter;
    /* 0x104 */ f32 mRainLength;
    /* 0x108 */ f32 mShadowDropLength;
    /* 0x10C */ bool mIsOnPlayer;
    /* 0x10D */ bool mNoRain;
};
