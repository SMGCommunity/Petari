#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class LiveActor;
class JointControllerInfo;

class TurnJointCtrl {
public:
    enum AXIS { X = 0, Y = 1, Z = 2 };  // ??? maybe?
    struct Ctrl {};

    TurnJointCtrl(LiveActor*);

    void init(f32, f32, f32);
    void addFace(const char*, f32, AXIS, AXIS, AXIS);
    void addWaist(const char*, f32, AXIS, AXIS, AXIS);
    void startCtrl(s32);
    void endCtrl(s32);
    void validate();
    void invalidate();
    void setStarePos(const TVec3f&);
    void update();
    void setCallBackFunction();
    void udateJointMtxCallBackFace(TPos3f*, const JointControllerInfo&);
    void udateJointMtxCallBackWaist(TPos3f*, const JointControllerInfo&);
    void getMtxDir(TVec3f*, const TPos3f*, AXIS);
    void updateJointMtxCallBack(TPos3f*, const Ctrl&);

    /* 0x00 */ u8 pad[0x5C];
    /* 0x5C */ f32 _5C;
    /* 0x60 */ f32 _60;
    /* 0x64 */ u32 _64;
    /* 0x68 */ u32 _68;
};
