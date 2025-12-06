#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class CameraRepulsiveArea : public AreaObj {
public:
    inline CameraRepulsiveArea(int type, const char* pName) : AreaObj(type, pName) {}

    virtual ~CameraRepulsiveArea();

    virtual const char* getManagerName() const;

    virtual TVec3f getRepulsion(const TVec3f&) = 0;
};

class CameraRepulsiveSphere : public CameraRepulsiveArea {
public:
    inline CameraRepulsiveSphere(const char* pName) : CameraRepulsiveArea(2, pName) {}

    virtual ~CameraRepulsiveSphere();

    virtual TVec3f getRepulsion(const TVec3f&);
};

class CameraRepulsiveCylinder : public CameraRepulsiveArea {
public:
    inline CameraRepulsiveCylinder(const char* pName) : CameraRepulsiveArea(3, pName) {}

    virtual ~CameraRepulsiveCylinder();

    virtual TVec3f getRepulsion(const TVec3f&);
};
