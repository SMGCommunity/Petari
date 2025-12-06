#pragma once

#include "Game/Util.hpp"
#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class AreaForm {
public:
    virtual void init(const JMapInfoIter&);
    virtual bool isInVolume(const TVec3f&) const;

    TPos3f* _4;
};

class AreaFormCube : public AreaForm {
public:
    AreaFormCube(int);

    virtual void init(const JMapInfoIter&);
    virtual bool isInVolume(const TVec3f&) const;

    void calcWorldPos(TVec3f*) const;
    void calcWorldBox(TDirBox3f*) const;
    void calcLocalPos(TVec3f*, const TVec3f&) const;
    f32 getBaseSize();
    void updateBoxParam();
    void calcWorldMtx(TPos3f*) const;

    int _8;
    TVec3f mTranslation;  // 0xC
    TVec3f mRotation;     // 0x18
    TVec3f mScale;        // 0x24
    TBox3f mBounding;     // 0x30
    Mtx _48;
};

class AreaFormSphere : public AreaForm {
public:
    AreaFormSphere();

    virtual void init(const JMapInfoIter&);
    virtual bool isInVolume(const TVec3f&) const;

    void calcUpVec(TVec3f*) const;
    void calcPos(TVec3f*) const;

    TVec3f mTranslation;  // 0x8
    f32 _14;
    TVec3f mUp;  // 0x18
};

class AreaFormBowl : public AreaForm {
public:
    AreaFormBowl();

    virtual void init(const JMapInfoIter&);
    virtual bool isInVolume(const TVec3f&) const;

    void calcUpVec(const TVec3f&);

    TVec3f mTranslation;  // 0x8
    TVec3f mUp;           // 0x14
    f32 _20;
};

class AreaFormCylinder : public AreaForm {
public:
    AreaFormCylinder();

    virtual void init(const JMapInfoIter&);
    virtual bool isInVolume(const TVec3f&) const;

    void calcPos(TVec3f*) const;
    void calcCenterPos(TVec3f*) const;
    void calcUpVec(TVec3f*) const;
    void calcDir(const TVec3f&);

    TVec3f mTranslation;  // 0x8
    TVec3f mRotation;     // 0x14
    f32 _20;
    f32 _24;
};