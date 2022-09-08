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

    virtual bool calcJointMatrix(TPos3f *, const JointControllerInfo &);
    virtual bool calcJointMatrixAfterChild(TPos3f *, const JointControllerInfo &);

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

    virtual bool calcJointMatrix(TPos3f *a1, const JointControllerInfo &a2) {
        if (mMtxCalcFunc != NULL) {
            return (mHost->*mMtxCalcFunc)(a1, a2);
        } else {
            return false;
        }
    }

    virtual bool calcJointMatrixAfterChild(TPos3f *a1, const JointControllerInfo &a2) {
        if (mMtxCalcAfterChildFunc != NULL) {
            return (mHost->*mMtxCalcAfterChildFunc)(a1, a2);
        } else {
            return false;
        }
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