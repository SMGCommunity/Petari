#pragma once

#include <revolution.h>

class J3DAnmTransform;

class J3DMtxCalc {
public:
    inline J3DMtxCalc() {}

    virtual ~J3DMtxCalc();
    virtual void setAnmTransForm(J3DAnmTransform*);
    virtual J3DAnmTransform* getAnmTransform();
    virtual void setAnmTransform(u8, J3DAnmTransform*);
    virtual J3DAnmTransform* getAnmTransform(u8);
    virtual void setWeight(u8, f32);
    virtual f32 getWeight(u8);
};