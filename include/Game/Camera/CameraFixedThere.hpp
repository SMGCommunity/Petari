#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFixedThere : public Camera {
public:
    CameraFixedThere(const char* pName = "その場定点カメラ");
    virtual ~CameraFixedThere();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isCorrectingErpPositionOff() const;
    virtual CamTranslatorBase* createTranslator();

    void copyStatusFromPrevCamera();
    void calcEyeDir(TVec3f*);
    void makeAxisAndRoll();
    void updateUpVec(const TVec3f&);
    void updateNormalUpVec(const TVec3f&);

    u32 _4C;
    bool _50;
    u8 _51[3];
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
};