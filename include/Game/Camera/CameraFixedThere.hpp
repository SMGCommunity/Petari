#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFixedThere : public Camera {
public:
    enum CameraType {
        /* 0x0 */ CameraType_GravityUp,
        /* 0x1 */ CameraType_WorldUp,
    };

    CameraFixedThere(const char* pName = "その場定点カメラ");

    /* 0x08 */ virtual ~CameraFixedThere();
    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x3C */ virtual bool isCorrectingErpPositionOff() const {
        return true;
    }
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void setParam(u32 cameraType, bool isFovyFixed) {
        mCameraType = cameraType;
        mIsFovyFixed = isFovyFixed;
    }

    void copyStatusFromPrevCamera();
    bool calcEyeDir(TVec3f*);
    void makeAxisAndRoll();
    void updateUpVec(const TVec3f&);
    void updateNormalUpVec(const TVec3f&);

    /* 0x4C */ u32 mCameraType;
    /* 0x50 */ bool mIsFovyFixed;
    /* 0x54 */ TVec3f mUp;
    /* 0x60 */ TVec3f mAxis;
    /* 0x6C */ f32 mRoll;
};
