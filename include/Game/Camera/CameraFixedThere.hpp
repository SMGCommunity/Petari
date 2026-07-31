#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFixedThere : public Camera {
public:
    CameraFixedThere(const char* pName = "その場定点カメラ");

    virtual ~CameraFixedThere();
    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isCorrectingErpPositionOff() const {
        return true;
    }
    virtual CamTranslatorBase* createTranslator();

    void copyStatusFromPrevCamera();
    bool calcEyeDir(TVec3f*);
    void makeAxisAndRoll();
    void updateUpVec(const TVec3f&);
    void updateNormalUpVec(const TVec3f&);

    /* 0x4C */ u32 _4C;
    /* 0x50 */ bool _50;
    /* 0x54 */ TVec3f mUp;
    /* 0x60 */ TVec3f mAxis;
    /* 0x6C */ f32 mRoll;
};
