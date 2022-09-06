#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.h"

class J3DJoint;
class J3DModel;
class LiveActor;

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
class JointControlDelegator : public JointController {
public:

    typedef bool (T::*func)(TPos3f *, const JointControllerInfo &);

    inline JointControlDelegator(T *pHost, func calcFunc, func calcAfterChild) : JointController() {
        mHost = pHost;
        mMtxCalcFunc = calcFunc;
        mMtxCalcAfterChildFunc = calcAfterChild;
    }

    virtual ~JointControlDelegator() {
        
    }

    T* mHost;                       // _C
    func mMtxCalcFunc;             // _10
    func mMtxCalcAfterChildFunc;   // _14
};

namespace MR {
    void setJointControllerParam(JointController *, const LiveActor *, const char *);

    template <class T>
    JointControlDelegator<T>* createJointDelegatorWithNullChildFunc(T *pHost, bool (T::*calcFunc)(TPos3f *, const JointControllerInfo &), const char *pName) {
        JointControlDelegator<T>* delegator = new JointControlDelegator<T>(pHost, calcFunc, 0);
        setJointControllerParam(delegator, pHost, pName);
        return delegator;
    }
};