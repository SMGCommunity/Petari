#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFollow : public Camera {
public:
    CameraFollow(const char* pName = "フォローカメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToRoundLeft() const;
    virtual bool isEnableToRoundRight() const;
    virtual bool isEnableToReset() const {
        return true;
    }
    virtual CamTranslatorBase* createTranslator();

    void setParam(f32, f32, f32, f32, f32, bool);
    void calcIdealPos();
    void calcRotPos();
    void constrainDistance(TVec3f*, const TVec3f&);
    void constrainAngleX(f32*);
    void constrainAngleY(f32*, const TVec3f&, f32);
    f32 getMagnitudeOfMovingRound() const;
    void checkShiftStateToRound(const TVec3f&, const TVec3f&);
    void arrangeHeight();
    void reflectRotOffset();
    bool isWallForward() const;
    void arrangeLookUp();
    bool checkAscent(TVec3f*) const;
    void arrangeLookDown();
    bool checkDescent(TVec3f*) const;
    void startRound(f32, f32);
    void checkValidityOfRound(const TVec3f&, const TVec3f&);
    bool isUpsideDownByRoll() const;

    /* 0x4C */ f32 mDistMax;
    /* 0x50 */ f32 mDistMin;
    /* 0x54 */ f32 mAngleXMin;
    /* 0x58 */ f32 mAngleXRange;
    /* 0x5C */ TVec3f mPos;
    /* 0x68 */ TVec3f mWatchPos;
    /* 0x74 */ f32 mAngleYRoundSpeed;  // moving round magnitude (rate?)
    /* 0x78 */ f32 mAngleXSpeed;
    /* 0x7C */ bool mCheckWall;
    /* 0x7D */ bool mIsRounding;
    /* 0x80 */ f32 mAngleYTarget;
    /* 0x84 */ f32 mAngleYSpeed;
    /* 0x88 */ bool mIsRoundLeftValid;
    /* 0x89 */ bool mIsRoundRightValid;
};
