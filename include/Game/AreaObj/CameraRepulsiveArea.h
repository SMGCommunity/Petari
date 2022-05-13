#pragma once

#include "Game/AreaObj/AreaObj.h"

class CameraRepulsiveArea : public AreaObj {
public:
    inline CameraRepulsiveArea(int type, const char *pName) : AreaObj(type, pName) {

    }

    virtual ~CameraRepulsiveArea();

    virtual const char* getManagerName() const;

    virtual TVec3f getRepulsion(const TVec3f &) = 0;
};

class CameraRepulsiveSphere : public CameraRepulsiveArea {
public:
    inline CameraRepulsiveSphere(int type, const char *pName) : CameraRepulsiveArea(type, pName) {

    }

    virtual ~CameraRepulsiveSphere();

    virtual TVec3f getRepulsion(const TVec3f &);
};

class CameraRepulsiveCylinder : public CameraRepulsiveArea {
public:
    inline CameraRepulsiveCylinder(int type, const char *pName) : CameraRepulsiveArea(type, pName) {

    }

    virtual ~CameraRepulsiveCylinder();

    virtual TVec3f getRepulsion(const TVec3f &);
};