#pragma once

#include "Game/Util.h"
#include "JSystem/JGeometry/TBox.h"
#include "JSystem/JGeometry/TPosition3.h"
#include "JSystem/JGeometry/TRotation.h"
#include "JSystem/JGeometry/TVec.h"
#include "revolution.h"

class AreaForm {
public:
    virtual void init(const JMapInfoIter &);
    virtual bool isInVolume(const TVec3f &) const;

    TPos3f* _4;
};

class AreaFormCube : public AreaForm {
public:
    AreaFormCube(int);

    virtual void init(const JMapInfoIter &);
    virtual bool isInVolume(const TVec3f &) const;

    void calcWorldPos(TVec3f *) const;
    void calcWorldBox(TDirBox3f *) const;
    void calcLocalPos(TVec3f *, const TVec3f &) const;
    f32 getBaseSize();
    void updateBoxParam();
    void calcWorldMtx(TPos3f *) const;

    int _8;
    TVec3f mTranslation; // _C
    TVec3f mRotation;    // _18
    TVec3f mScale;       // _24
    TVec3f _30;
    TVec3f _3C;
    Mtx _48;
};

class AreaFormSphere : public AreaForm {
public:
    AreaFormSphere();

    virtual void init(const JMapInfoIter &);
    virtual bool isInVolume(const TVec3f &) const;

    void calcUpVec(TVec3f *) const;
    void calcPos(TVec3f *) const;

    TVec3f mTranslation;    // _8
    f32 _14;
    TVec3f mUp;             // _18
};

class AreaFormBowl : public AreaForm {
public:
    AreaFormBowl();

    virtual void init(const JMapInfoIter &);
    virtual bool isInVolume(const TVec3f &) const;

    void calcUpVec(const TVec3f &);

    TVec3f mTranslation;    // _8
    TVec3f mUp;             // _14
    f32 _20;
};

class AreaFormCylinder : public AreaForm {
public:
    AreaFormCylinder();

    virtual void init(const JMapInfoIter &);
    virtual bool isInVolume(const TVec3f &) const;

    void calcPos(TVec3f *) const;
    void calcCenterPos(TVec3f *) const;
    void calcUpVec(TVec3f *) const;
    void calcDir(const TVec3f &);

    TVec3f mTranslation;    // _8
    TVec3f mRotation;       // _14
    f32 _20;
    f32 _24;
};