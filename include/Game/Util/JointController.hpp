#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class J3DJoint;
class J3DModel;
class LiveActor;

struct UnkStruct {
    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u16 _14;
};

struct JointControllerInfo {
    u32 _0;
    UnkStruct* _4;
};

class JointController {
public:
    JointController();

    virtual bool calcJointMatrix(TPos3f*, const JointControllerInfo&);
    virtual bool calcJointMatrixAfterChild(TPos3f*, const JointControllerInfo&);

    void registerCallBack();
    void calcJointMatrixAndSetSystem(J3DJoint*);
    void calcJointMatrixAfterChildAndSetSystem(J3DJoint*);
    static int staticCallBack(J3DJoint*, int);

    J3DModel* mModel;  // 0x4
    J3DJoint* mJoint;  // 0x8
};

template < typename T >
class JointControlDelegator : public JointController {
public:
    typedef bool (T::*func)(TPos3f*, const JointControllerInfo&);

    inline JointControlDelegator(T* pHost, func calcFunc, func calcAfterChild) : JointController() {
        mHost = pHost;
        mMtxCalcFunc = calcFunc;
        mMtxCalcAfterChildFunc = calcAfterChild;
    }

    inline JointControlDelegator(func calcFunc, T* pHost, func calcAfterChild) : JointController() {
        mHost = pHost;
        mMtxCalcFunc = calcAfterChild;
        mMtxCalcAfterChildFunc = calcFunc;
    }

    virtual ~JointControlDelegator() {}

    virtual bool calcJointMatrix(TPos3f* a1, const JointControllerInfo& a2) {
        if (mMtxCalcFunc != nullptr) {
            return (mHost->*mMtxCalcFunc)(a1, a2);
        } else {
            return false;
        }
    }

    virtual bool calcJointMatrixAfterChild(TPos3f* a1, const JointControllerInfo& a2) {
        if (mMtxCalcAfterChildFunc != nullptr) {
            return (mHost->*mMtxCalcAfterChildFunc)(a1, a2);
        } else {
            return false;
        }
    }

    T* mHost;                     // 0xC
    func mMtxCalcFunc;            // 0x10
    func mMtxCalcAfterChildFunc;  // 0x14
};

namespace MR {
    void setJointControllerParam(JointController*, const LiveActor*, const char*);

    template < class T >
    JointControlDelegator< T >* createJointDelegator(T* pHost, const LiveActor* pActor, bool (T::*calcFunc)(TPos3f*, const JointControllerInfo&),
                                                     bool (T::*calcChild)(TPos3f*, const JointControllerInfo&), const char* pName) {
        JointControlDelegator< T >* delegator = new JointControlDelegator< T >(pHost, calcFunc, calcChild);
        setJointControllerParam(delegator, pActor, pName);
        return delegator;
    }

    template < class T >
    JointControlDelegator< T >* createJointDelegatorWithNullChildFunc(T* pHost, bool (T::*calcFunc)(TPos3f*, const JointControllerInfo&),
                                                                      const char* pName) {
        JointControlDelegator< T >* delegator = new JointControlDelegator< T >(pHost, calcFunc, 0);
        setJointControllerParam(delegator, pHost, pName);
        return delegator;
    }

    template < class T >
    JointControlDelegator< T >* createJointDelegatorWithNullMtxFunc(T* pHost, bool (T::*calcFunc)(TPos3f*, const JointControllerInfo&),
                                                                    const char* pName) {
        JointControlDelegator< T >* delegator = new JointControlDelegator< T >(calcFunc, pHost, 0);
        setJointControllerParam(delegator, pHost, pName);
        return delegator;
    }
};  // namespace MR
