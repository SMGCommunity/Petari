#pragma once

#include "Game/AreaObj/AreaForm.hpp"
#include "Game/AreaObj/AreaObj.hpp"

class CameraRepulsiveArea : public AreaObj {
public:
    CameraRepulsiveArea(int formType, const char* pName) : AreaObj(formType, pName) {
    }

    /* 0x08 */ virtual ~CameraRepulsiveArea();

    /* 0x28 */ virtual const char* getManagerName() const {
        return "CameraRepulsiveArea";
    }

    virtual TVec3f getRepulsion(const TVec3f&) = 0;
};

class CameraRepulsiveSphere : public CameraRepulsiveArea {
public:
    CameraRepulsiveSphere(const char* pName) : CameraRepulsiveArea(AreaForm::Type_Sphere, pName) {
    }

    /* 0x08 */ virtual ~CameraRepulsiveSphere();

    virtual TVec3f getRepulsion(const TVec3f&);
};

class CameraRepulsiveCylinder : public CameraRepulsiveArea {
public:
    CameraRepulsiveCylinder(const char* pName) : CameraRepulsiveArea(AreaForm::Type_Cylinder, pName) {
    }

    /* 0x08 */ virtual ~CameraRepulsiveCylinder();

    virtual TVec3f getRepulsion(const TVec3f&);
};
