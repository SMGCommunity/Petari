#pragma once

#include "Game/Effect/SimpleEffectObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class NameObjArchiveListCollector;
class ActorCameraInfo;
class ModelObj;
class CameraTargetMtx;

class MorphItemObjNeo : public LiveActor {
public:
    MorphItemObjNeo(const char*, s32);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual bool isDemo() const;

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void glowEffect();

    void exeWait();
    void exeAppear();
    void exeSwitchAppear();
    void exeWait2();
    void exeFly();
    void exeDemo();

    /* 0x8c */ u16 mAppearMode;
    /* 0x8E */ u16 mAppearFrames;
    /* 0x90 */ bool mIsUseSwitch;
    /* 0x91 */ bool mIsFloating;  // Obj_arg2
    /* 0x92 */ bool _92;
    /* 0x93 */ u8 mCameraMode;     // Obj_arg5
    /* 0x94 */ s8 mRotateMode;     // Obj_arg6
    /* 0x95 */ bool _95;           // Obj_arg0
    /* 0x96 */ u8 mContainerType;  // Obj_arg3
    /* 0x97 */ u8 _97;
    /* 0x98 */ s8 mIsBind;      // Obj_arg1
    /* 0x99 */ u8 mShadowType;  // Obj_arg4
    /* 0x9C */ TVec3f mZRotation;
    /* 0xA8 */ s32 mMorphType;
    /* 0xAC */ f32 mAngle;
    /* 0xB0 */ TVec3f mSpawnPosition;
    /* 0xBC */ ActorCameraInfo* mActorCameraInfo;
    /* 0xC0 */ ModelObj* mContainerModel;
    /* 0xC4 */ ModelObj* mContainerBreakModel;
    /* 0xC8 */ TPos3f mBaseMtx;
    /* 0xF8 */ CameraTargetMtx* mCameraTargetMtx;
};

class MorphItemNeoHopper : public MorphItemObjNeo {
public:
    MorphItemNeoHopper(const char* pName) : MorphItemObjNeo(pName, 1) {
    }
};

class MorphItemNeoIce : public MorphItemObjNeo {
public:
    MorphItemNeoIce(const char* pName) : MorphItemObjNeo(pName, 4) {
    }
};

class MorphItemNeoFire : public MorphItemObjNeo {
public:
    MorphItemNeoFire(const char* pName) : MorphItemObjNeo(pName, 5) {
    }
};

class MorphItemNeoFoo : public MorphItemObjNeo {
public:
    MorphItemNeoFoo(const char* pName) : MorphItemObjNeo(pName, 6) {
    }
};

class MorphItemNeoBee : public MorphItemObjNeo {
public:
    MorphItemNeoBee(const char* pName) : MorphItemObjNeo(pName, 2) {
    }
};

class MorphItemNeoTeresa : public MorphItemObjNeo {
public:
    MorphItemNeoTeresa(const char* pName) : MorphItemObjNeo(pName, 3) {
    }
};

// Why is this here?

class EffectObjR100F50SyncClipping : public SimpleEffectObj {
public:
    EffectObjR100F50SyncClipping(const char* pName) : SimpleEffectObj(pName) {
    }

    virtual f32 getClippingRadius() const {
        return 100.0f;
    }
    virtual f32 getFarClipDistance() const {
        return 50.0f;
    }
    virtual bool isSyncClipping() const {
        return true;
    }
};

class EffectObjR500F50 : public SimpleEffectObj {
public:
    EffectObjR500F50(const char* pName) : SimpleEffectObj(pName) {
    }

    virtual f32 getClippingRadius() const {
        return 500.0f;
    }
    virtual f32 getFarClipDistance() const {
        return 50.0f;
    }
};

class EffectObjR1000F50 : public SimpleEffectObj {
public:
    EffectObjR1000F50(const char* pName) : SimpleEffectObj(pName) {
    }

    virtual f32 getClippingRadius() const {
        return 1000.0f;
    }
    virtual f32 getFarClipDistance() const {
        return 50.0f;
    }
};

class EffectObj20x20x10SyncClipping : public SimpleEffectObj {
public:
    EffectObj20x20x10SyncClipping(const char* pName) : SimpleEffectObj(pName) {
    }

    virtual f32 getClippingRadius() const {
        return 1000.0f;
    }
    virtual f32 getFarClipDistance() const {
        return 50.0f;
    }
    virtual TVec3f* getClippingCenterOffset() const {
        return &(TVec3f(0.0f, 200.0f, 0.0f));
    }
    virtual bool isSyncClipping() const {
        return true;
    }
};

class EffectObj50x50x10SyncClipping : public SimpleEffectObj {
public:
    EffectObj50x50x10SyncClipping(const char* pName) : SimpleEffectObj(pName) {
    }

    virtual f32 getClippingRadius() const {
        return 2500.0f;
    };
    virtual f32 getFarClipDistance() const {
        return 50.0f;
    };
    virtual TVec3f* getClippingCenterOffset() const {
        return &(TVec3f(0.0f, 200.0f, 0.0f));
    };
    virtual bool isSyncClipping() const {
        return true;
    };
};

class EffectObj10x10x10SyncClipping : public SimpleEffectObj {
public:
    EffectObj10x10x10SyncClipping(const char* pName) : SimpleEffectObj(pName) {
    }

    virtual f32 getClippingRadius() const {
        return 1000.0f;
    }
    virtual f32 getFarClipDistance() const {
        return 50.0f;
    }
    virtual TVec3f* getClippingCenterOffset() const {
        return &TVec3f(0.0f, 580.0f, 0.0f);
    }
    virtual bool isSyncClipping() const {
        return true;
    }
};
