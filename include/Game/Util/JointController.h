#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TPosition3.h"

class J3DJoint;
class J3DModel;

struct JointControllerInfo {

};

class JointController {
public:
    JointController();

    virtual s32 calcJointMatrix(TPos3f *, const JointControllerInfo &);
    virtual s32 calcJointMatrixAfterChild(TPos3f *, const JointControllerInfo &);

    void registerCallBack();
    void calcJointMatrixAndSetSystem(J3DJoint *);
    void calcJointMatrixAfterChildAndSetSystem(J3DJoint *);
    static void staticCallBack(J3DJoint *, int);

    J3DModel* mModel;   // _4
    J3DJoint* mJoint;   // _8
};

template<typename T>
class JointControllerDelegator : public JointController {
public:

    typedef bool (T::*func)(TPos3f *, const JointControllerInfo &);

    inline JointControllerDelegator(func calcFunc, func calcAfterChild) : JointController() {
        mMtxCalcFunc = calcFunc;
        mMtxCalcAfterChildFunc = calcAfterChild;
    }

    virtual ~JointControllerDelegator();

    T* mHost;                       // _C
    func mMtxCalcFunc;             // _10
    func mMtxCalcAfterChildFunc;   // _14
};