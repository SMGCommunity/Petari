#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class J3DJoint;
class J3DModel;
class LiveActor;

struct UnkStruct {
    /* 0x00 */ u32 _0;
    /* 0x04 */ u32 _4;
    /* 0x08 */ u32 _8;
    /* 0x0C */ u32 _C;
    /* 0x10 */ u32 _10;
    /* 0x14 */ u16 _14;
};

struct JointControllerInfo {
    /* 0x00 */ u32 _0;
    /* 0x04 */ UnkStruct* _4;
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

    /* 0x04 */ J3DModel* mModel;
    /* 0x08 */ J3DJoint* mJoint;
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

    virtual ~JointControlDelegator() {
    }

    virtual bool calcJointMatrix(TPos3f* pMtx, const JointControllerInfo& rInfo) {
        if (mMtxCalcFunc != nullptr) {
            return (mHost->*mMtxCalcFunc)(pMtx, rInfo);
        } else {
            return false;
        }
    }

    virtual bool calcJointMatrixAfterChild(TPos3f* pMtx, const JointControllerInfo& rInfo) {
        if (mMtxCalcAfterChildFunc != nullptr) {
            return (mHost->*mMtxCalcAfterChildFunc)(pMtx, rInfo);
        } else {
            return false;
        }
    }

    /* 0x0C */ T* mHost;
    /* 0x10 */ func mMtxCalcFunc;
    /* 0x1C */ func mMtxCalcAfterChildFunc;
};

namespace MR {
    void setJointControllerParam(JointController*, const LiveActor*, const char*);
    void setJointControllerParam(JointController*, const LiveActor*, u16);

    template < class T >
    JointController* createJointController(T* pHost, const LiveActor* pActor, u16 jointIndex,
                                           bool (T::*calcFunc)(TPos3f*, const JointControllerInfo&),
                                           bool (T::*calcChild)(TPos3f*, const JointControllerInfo&)) {
        JointControlDelegator< T >* controller = new JointControlDelegator< T >(pHost, calcFunc, calcChild);
        setJointControllerParam(controller, pActor, jointIndex);
        return controller;
    }

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

    template < class T >
    JointController* createJointController(T* pHost, const char* pName, bool (T::*calcFunc)(TPos3f*, const JointControllerInfo&)) {
        JointControlDelegator< T >* delegator = new JointControlDelegator< T >(calcFunc, pHost, 0);
        setJointControllerParam(delegator, pHost, pName);
        return delegator;
    }
};  // namespace MR
