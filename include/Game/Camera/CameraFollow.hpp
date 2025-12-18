#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFollow : public Camera {
public:
    CameraFollow(const char* pName = "フォローカメラ");
    virtual ~CameraFollow();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToRoundLeft() const;
    virtual bool isEnableToRoundRight() const;
    virtual bool isEnableToReset() const;
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
    void checkAscent(TVec3f*) const;
    void arrangeLookDown();
    void checkDescent(TVec3f*) const;
    void startRound(f32, f32);
    void checkValidityOfRound(const TVec3f&, const TVec3f&);
    bool isUpsideDownByRoll() const;

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
    f32 _70;
    f32 _74;
    f32 _78;
    u8 _7C;
    u8 _7D;
    u8 _7E[2];
    f32 _80;
    f32 _84;
    u8 _88;
    u8 _89;
    u8 _8A[2];
};