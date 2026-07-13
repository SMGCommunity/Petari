#pragma once

#include <revolution.h>

class J3DAnmTransform;
class J3DMtxBuffer;
class J3DJoint;

class J3DMtxCalc {
public:
    static void setMtxBuffer(J3DMtxBuffer*);

    virtual ~J3DMtxCalc() {
    }
    virtual void setAnmTransform(J3DAnmTransform*) {
    }
    virtual J3DAnmTransform* getAnmTransform() {
        return NULL;
    }
    virtual void setAnmTransform(u8, J3DAnmTransform*) {
    }
    virtual J3DAnmTransform* getAnmTransform(u8) {
        return NULL;
    }
    virtual void setWeight(u8, f32) {
    }
    virtual f32 getWeight(u8) const {
        return 0.0f;
    }
    virtual void init(const Vec& param_0, const Mtx&) = 0;
    virtual void calc() = 0;

    static J3DMtxBuffer* getMtxBuffer() {
        return mMtxBuffer;
    }
    static J3DJoint* getJoint() {
        return mJoint;
    }
    static void setJoint(J3DJoint* joint) {
        mJoint = joint;
    }

    static J3DMtxBuffer* mMtxBuffer;
    static J3DJoint* mJoint;
};  // Size: 0x4
