#pragma once

#include "Game/AreaObj/AreaForm.hpp"
#include "Game/AreaObj/AreaObj.hpp"

class CameraRepulsiveArea : public AreaObj {
public:
    CameraRepulsiveArea(int formType, const char* pName) : AreaObj(formType, pName) {
    }

    virtual ~CameraRepulsiveArea();

    virtual const char* getManagerName() const {
        return "CameraRepulsiveArea";
    }

    virtual TVec3f getRepulsion(const TVec3f&) = 0;
};

class CameraRepulsiveSphere : public CameraRepulsiveArea {
public:
    CameraRepulsiveSphere(const char* pName) : CameraRepulsiveArea(AreaForm::Type_Sphere, pName) {
    }

    virtual ~CameraRepulsiveSphere();

    virtual TVec3f getRepulsion(const TVec3f&);
};

class CameraRepulsiveCylinder : public CameraRepulsiveArea {
public:
    CameraRepulsiveCylinder(const char* pName) : CameraRepulsiveArea(AreaForm::Type_Cylinder, pName) {
    }

    virtual ~CameraRepulsiveCylinder();

    virtual TVec3f getRepulsion(const TVec3f&);
};
