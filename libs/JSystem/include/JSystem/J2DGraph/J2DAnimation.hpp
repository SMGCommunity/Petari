#ifndef _J2DANIMATION_HPP
#define _J2DANIMATION_HPP

#include "JSystem/J3DGraphBase/J3DTransform.hpp"

class J2DScreen;

enum EJ2DAnmKind {
    kJ2DAnm_Transform = 0,
    kJ2DAnm_Color = 1,
    kJ2DAnm_TexPattern = 2,
    kJ2DAnm_TextureSRT = 4,
    kJ2DAnm_TevReg = 5,
    kJ2DAnm_Visibility = 6,
    kJ2DAnm_VtxColor = 7
};

class J2DAnmBase {
public:
    virtual ~J2DAnmBase();
    virtual void searchUpdateMaterialID(J2DScreen*);

    EJ2DAnmKind getKind() const {
        return mKind;
    }

    u8 mAttribute;
    u8 mUnknown;
    s16 mFrameMax;
    f32 mFrame;
    EJ2DAnmKind mKind;
};

class J2DAnmTransform : public J2DAnmBase {
public:
    virtual ~J2DAnmTransform();
    virtual void getTransform(u16, J3DTransformInfo*) const;

    f32* mScaleValues;
    s16* mRotationValues;
    f32* mTranslateValues;
};

#endif
