#pragma once

#include "Game/Effect/SimpleEffectObj.hpp"
#include "Game/NameObj/NameObj.hpp"

class NameObjArchiveListCollector;
class JMapInfoIter;

class MorphItemObjNeo : public NameObj {
public:
    MorphItemObjNeo(const char*, long);
    virtual ~MorphItemObjNeo();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    void exeWait();
    void exeAppear();
    void exeSwitchAppear();
    void exeWait2();
    void exeFly();
    void exeDemo();

private:
    u8 mPad[0xFC - sizeof(NameObj)];
};

class MorphItemNeoBee : public MorphItemObjNeo {
public:
    MorphItemNeoBee(const char* pName) : MorphItemObjNeo(pName, 2) {}
    virtual ~MorphItemNeoBee();
};

class MorphItemNeoFire : public MorphItemObjNeo {
public:
    MorphItemNeoFire(const char* pName) : MorphItemObjNeo(pName, 5) {}
    virtual ~MorphItemNeoFire();
};

class MorphItemNeoFoo : public MorphItemObjNeo {
public:
    MorphItemNeoFoo(const char* pName) : MorphItemObjNeo(pName, 6) {}
    virtual ~MorphItemNeoFoo();
};

class MorphItemNeoHopper : public MorphItemObjNeo {
public:
    MorphItemNeoHopper(const char* pName) : MorphItemObjNeo(pName, 1) {}
    virtual ~MorphItemNeoHopper();
};

class MorphItemNeoIce : public MorphItemObjNeo {
public:
    MorphItemNeoIce(const char* pName) : MorphItemObjNeo(pName, 4) {}
    virtual ~MorphItemNeoIce();
};

class MorphItemNeoTeresa : public MorphItemObjNeo {
public:
    MorphItemNeoTeresa(const char* pName) : MorphItemObjNeo(pName, 3) {}
    virtual ~MorphItemNeoTeresa();
};

class EffectObj10x10x10SyncClipping : public SimpleEffectObj {
public:
    EffectObj10x10x10SyncClipping(const char* pName) : SimpleEffectObj(pName) {}
    virtual ~EffectObj10x10x10SyncClipping();
    virtual f32 getClippingRadius() const;
    virtual f32 getFarClipDistance() const;
    virtual TVec3f* getClippingCenterOffset() const;
    virtual bool isSyncClipping() const;
};

class EffectObj20x20x10SyncClipping : public SimpleEffectObj {
public:
    EffectObj20x20x10SyncClipping(const char* pName) : SimpleEffectObj(pName) {}
    virtual ~EffectObj20x20x10SyncClipping();
    virtual f32 getClippingRadius() const;
    virtual f32 getFarClipDistance() const;
    virtual TVec3f* getClippingCenterOffset() const;
    virtual bool isSyncClipping() const;
};

class EffectObj50x50x10SyncClipping : public SimpleEffectObj {
public:
    EffectObj50x50x10SyncClipping(const char* pName) : SimpleEffectObj(pName) {}
    virtual ~EffectObj50x50x10SyncClipping();
    virtual f32 getClippingRadius() const;
    virtual f32 getFarClipDistance() const;
    virtual TVec3f* getClippingCenterOffset() const;
    virtual bool isSyncClipping() const;
};

class EffectObjR1000F50 : public SimpleEffectObj {
public:
    EffectObjR1000F50(const char* pName) : SimpleEffectObj(pName) {}
    virtual ~EffectObjR1000F50();
    virtual f32 getClippingRadius() const;
    virtual f32 getFarClipDistance() const;
};

class EffectObjR100F50SyncClipping : public SimpleEffectObj {
public:
    EffectObjR100F50SyncClipping(const char* pName) : SimpleEffectObj(pName) {}
    virtual ~EffectObjR100F50SyncClipping();
    virtual f32 getClippingRadius() const;
    virtual f32 getFarClipDistance() const;
    virtual bool isSyncClipping() const;
};

class EffectObjR500F50 : public SimpleEffectObj {
public:
    EffectObjR500F50(const char* pName) : SimpleEffectObj(pName) {}
    virtual ~EffectObjR500F50();
    virtual f32 getClippingRadius() const;
    virtual f32 getFarClipDistance() const;
};
