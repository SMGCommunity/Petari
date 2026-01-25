#pragma once

#include "JSystem/JGeometry/TBox.hpp"
#include "JSystem/JSupport/JSUList.hpp"

class J2DAnmBase;
class J2DAnmColor;
class J2DAnmTransform;
class J2DAnmTevRegKey;
class J2DAnmTextureSRTKey;
class J2DAnmVisibilityFull;
class J2DAnmVtxColor;
class J2DGrafContext;
class J2DScreen;
class J2DAnmTexPattern;
class JKRArchive;
class JSURandomInputStream;
struct ResFONT;
struct ResTIMG;

class J2DPane {
public:
    J2DPane();

    virtual ~J2DPane();
    virtual u16 getTypeID() const;
    virtual void move(f32 x, f32 y);
    virtual void add(f32 x, f32 y);
    virtual void resize(f32 x, f32 y);
    virtual void setCullBack(bool cull);
    virtual void setCullBack(_GXCullMode cmode);
    virtual void setAlpha(u8 alpha);
    virtual bool setConnectParent(bool connected);
    virtual void calcMtx();
    virtual void update();
    virtual void drawSelf(f32 arg1, f32 arg2);
    virtual void drawSelf(f32 arg1, f32 arg2, Mtx* mtx);
    virtual J2DPane* search(u64 tag);
    virtual J2DPane* searchUserInfo(u64 tag);
    virtual void makeMatrix(f32 a, f32 b);
    virtual void makeMatrix(f32 a, f32 b, f32 c, f32 d);
    virtual bool isUsed(const ResTIMG* timg);
    virtual bool isUsed(const ResFONT* font);
    virtual void clearAnmTransform();
    virtual void rewriteAlpha();
    virtual void setAnimation(J2DAnmBase* anm);
    virtual void setAnimation(J2DAnmTransform* anm);
    virtual void setAnimation(J2DAnmColor* anm);
    virtual void setAnimation(J2DAnmTexPattern* anm);
    virtual void setAnimation(J2DAnmTextureSRTKey* anm);
    virtual void setAnimation(J2DAnmTevRegKey* anm);
    virtual void setAnimation(J2DAnmVisibilityFull* anm);
    virtual void setAnimation(J2DAnmVtxColor* anm);
    virtual const J2DAnmTransform* animationTransform(const J2DAnmTransform* transform);
    virtual void setVisibileAnimation(J2DAnmVisibilityFull* visibility);
    virtual void setAnimationVF(J2DAnmVisibilityFull* visibility);
    virtual void setVtxColorAnimation(J2DAnmVtxColor* vtx_color);
    virtual void setAnimationVC(J2DAnmVtxColor* vtx_color);
    virtual const J2DAnmTransform* animationPane(const J2DAnmTransform* transform);

    void initiate();
    void changeUseTrans(J2DPane* other);
    void place(const JGeometry::TBox2< f32 >& dim);
    J2DPane* getFirstChildPane();
    J2DPane* getNextChildPane();
    J2DPane* getParentPane();
    static s16 J2DCast_F32_to_S16(f32 value, u8 arg2);
    void animationTransform();
    void updateTransform(const J2DAnmTransform*);

    f32 getHeight() const { return mBounds.getHeight(); }
    f32 getWidth() const { return mBounds.getWidth(); }
    JSUTree< J2DPane >* getFirstChild() { return mPaneTree.getFirstChild(); }
    JSUTree< J2DPane >* getEndChild() { return mPaneTree.getEndChild(); }
    const JSUTree< J2DPane >* getPaneTree() { return &mPaneTree; }

    bool isVisible() const { return mVisible; }

    static JGeometry::TBox2< f32 > static_mBounds;

    u16 _4;
    int mKind;
    u64 mInfoTag;
    u64 mUserInfoTag;
    TBox2f mBounds;
    TBox2f mGlobalBounds;
    TBox2f mClipRect;
    Mtx mPositionMtx;
    Mtx mGlobalMtx;
    bool mVisible;
    u8 mCullMode;
    u8 mAlpha;
    u8 mColorAlpha;
    bool mIsInfluencedAlpha;
    bool mConnected;
    char mRotAxis;
    u8 mBasePosition;
    f32 mRotateX;
    f32 mRotateY;
    f32 mRotateZ;
    f32 mRotateOffsetX;
    f32 mRotateOffsetY;
    f32 mScaleX;
    f32 mScaleY;
    f32 mTranslateX;
    f32 mTranslateY;
    JSUTree< J2DPane > mPaneTree;
    const J2DAnmTransform* mTransform;
};
