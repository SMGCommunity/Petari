#pragma once

#include <JSystem/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/JGeometry/TVec.hpp>

class J3DModelData;
class J3DAnmTransform;

class XtransformInfo {
public:
    XtransformInfo();

    /* 0x00 */ TVec3f _0;
    /* 0x0C */ TVec3f _C;
    /* 0x18 */ Quaternion mRotation;
};

class XjointTransform {
public:
    XjointTransform();

    void setLocalScale(f32 scale) {
        mScale.set(scale);
    }

    /* 0x00 */ J3DJoint* _0;
    /* 0x04 */ u16 _4;
    /* 0x08 */ TVec3f mScale;
    /* 0x14 */ TVec3f _14;
    /* 0x20 */ TVec3f _20;
    /* 0x2C */ TVec3f _2C;
    /* 0x38 */ TVec3f _38;
    /* 0x44 */ J3DTransformInfo _44;
    /* 0x64 */ MtxPtr _64;
    /* 0x68 */ u32 _68;
    /* 0x6C */ MtxPtr _6C;
};

class XjointInfo {
public:
    XjointInfo();

    /* 0x00 */ XtransformInfo _0;
    /* 0x28 */ XtransformInfo _28;
    /* 0x50 */ TVec3f _50;
    /* 0x5C */ f32 _5C;
    /* 0x60 */ f32 _60;
};

class XanimeTrack {
public:
    XanimeTrack() {
    }

    void init();
    void setFrame(f32 frame) {
        _8 = frame;
        _C = 1;
    }

    f32 getWeight() const {
        return mWeight;
    }

    /* 0x0 */ J3DAnmTransform* _0;
    /* 0x4 */ f32 mWeight;
    /* 0x8 */ f32 _8;
    /* 0xC */ u8 _C;
};

class XanimeCore : public J3DMtxCalc {
public:
    XanimeCore(u32, u32, u8);
    XanimeCore(u32, XanimeCore*);

    virtual ~XanimeCore();
    virtual void setWeight(u8, f32);
    virtual void init(const Vec&, const Mtx&);
    virtual void calc();

    void shareJointTransform(const XanimeCore*);
    void doFreeze();
    void setBck(u32, J3DAnmTransform*);
    void calcBlend(TVec3f*, TVec3f*);
    void calcSingle(TVec3f*, TVec3f*);
    void calcBlendSpecial();
    void calcScaleBlendMaya(const TVec3f&, const TVec3f&);
    void calcScaleBlendMayaNoTransform(const TVec3f&, const TVec3f&);
    void calcScaleBlendSI(const TVec3f&, const TVec3f&);
    void calcScaleBlendBasic(const TVec3f&, const TVec3f&);
    void calcScaleBlendSpecial();
    void freezeCopy(J3DModelData*, XanimeCore*, u32, u32);
    void initT(J3DModelData*);
    void reconfigJointTransform(J3DModelData*);
    void updateFrame();
    void fixT(TVec3f*);

    void enableJointTransform(J3DModelData*);

    void initMember(u32);

    XjointTransform* getJointTransform(u32 index) {
        if (mTransformList == nullptr) {
            return nullptr;
        }

        return &mTransformList[index];
    }

    /* 0x4 */ u8 _4;
    /* 0x5 */ u8 mTrackCount;
    /* 0x6 */ u8 _6;
    /* 0x8 */ u32 mJointCount;
    /* 0xC */ u16 _C;
    /* 0x10 */ XjointInfo* mJointList;
    /* 0x14 */ XjointTransform* mTransformList;
    /* 0x18 */ XanimeTrack* mTrackList;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ f32 _20;
    /* 0x24 */ f32 mFrameRatio;
    /* 0x28 */ u8 _28;
    /* 0x29 */ u8 _29;
};
