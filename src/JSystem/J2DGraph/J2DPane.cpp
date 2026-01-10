#include "JSystem/J2DGraph/J2DPane.hpp"
#include <cmath>

J2DPane::J2DPane() : mBounds(), mGlobalBounds(), mClipRect(), mPaneTree(this) {
    mTransform = NULL;
    mKind = 'PAN1';
    mVisible = true;
    mInfoTag = 0;
    mUserInfoTag = 0;
    mBounds.set(0, 0, 0, 0);
    initiate();
    changeUseTrans(nullptr);
    calcMtx();
}

void J2DPane::initiate() {
    _4 = -1;
    mRotateOffsetY = mRotateOffsetX = mRotateZ = mRotateY = mRotateX = 0;
    mBasePosition = 0;
    mRotAxis = 'z';
    mScaleY = mScaleX = 1;
    mCullMode = GX_CULL_NONE;
    mAlpha = 255;
    mIsInfluencedAlpha = true;
    mColorAlpha = 255;
    mConnected = false;
    calcMtx();
}

void J2DPane::changeUseTrans(J2DPane* p_pane) {
    f32 xOffset = 0;
    f32 yOffset = 0;
    if (mBasePosition % 3 == 1) {
        xOffset = mBounds.getWidth() / 2;
    } else if (mBasePosition % 3 == 2) {
        xOffset = mBounds.getWidth();
    }

    if (mBasePosition / 3 == 1) {
        yOffset = mBounds.getHeight() / 2;
    } else if (mBasePosition / 3 == 2) {
        yOffset = mBounds.getHeight();
    }

    mTranslateX = mBounds._0.x + xOffset;
    mTranslateY = mBounds._0.y + yOffset;

    mRotateOffsetX = xOffset;
    mRotateOffsetY = yOffset;

    f32 addX = -mTranslateX;
    f32 addY = -mTranslateY;
    mBounds.addPos(addX, addY);

    if (p_pane != NULL) {
        u8 otherBasePos = p_pane->mBasePosition;
        f32 width = p_pane->getWidth();
        f32 height = p_pane->getHeight();

        if (otherBasePos % 3 == 1) {
            mTranslateX -= width / 2;
        } else if (otherBasePos % 3 == 2) {
            mTranslateX -= width;
        }

        if (otherBasePos / 3 == 1) {
            mTranslateY -= height / 2;
        } else if (otherBasePos / 3 == 2) {
            mTranslateY -= height;
        }
    }
}

J2DPane::~J2DPane() {
    JSUTreeIterator< J2DPane > iterator;
    for (iterator = mPaneTree.getFirstChild(); iterator != mPaneTree.getEndChild();) {
        J2DPane* child = (iterator++).getObject();
        delete child;
    }
}

void J2DPane::place(JGeometry::TBox2< f32 > const& box) {
    JGeometry::TBox2< f32 > tmpBox;

    if (mBounds._0.x == 0) {
        tmpBox._0.x = 0;
        tmpBox._8.x = box.getWidth();
        mTranslateX = box._0.x;
    } else if (mBounds._8.x == 0) {
        tmpBox._0.x = -box.getWidth();
        tmpBox._8.x = 0;
        mTranslateX = box._8.x;
    } else {
        tmpBox._0.x = -(box.getWidth() / 2);
        tmpBox._8.x = box.getWidth() / 2;
        mTranslateX = (box._0.x + box._8.x) / 2;
    }

    if (mBounds._0.y == 0) {
        tmpBox._0.y = 0;
        tmpBox._8.y = box.getHeight();
        mTranslateY = box._0.y;
    } else if (mBounds._8.y == 0) {
        tmpBox._0.y = -box.getHeight();
        tmpBox._8.y = 0;
        mTranslateY = box._8.y;
    } else {
        tmpBox._0.y = -(box.getHeight() / 2);
        tmpBox._8.y = box.getHeight() / 2;
        mTranslateY = (box._0.y + box._8.y) / 2;
    }

    f32 xOff = tmpBox._0.x - mBounds._0.x;
    f32 yOff = tmpBox._0.y - mBounds._0.y;
    J2DPane* child;
    for (child = getFirstChildPane(); child != NULL; child = child->getNextChildPane()) {
        child->mTranslateX += xOff;
        child->mTranslateY += yOff;
        if (xOff != 0 || yOff != 0) {
            child->calcMtx();
        }
    }
    mBounds = tmpBox;

    J2DPane* parent = getParentPane();
    if (parent != NULL) {
        mTranslateX += parent->mBounds._0.x;
        mTranslateY += parent->mBounds._0.y;
    }
    calcMtx();
}

void J2DPane::move(f32 x, f32 y) {
    f32 width = getWidth();
    f32 height = getHeight();
    place(JGeometry::TBox2< f32 >(x, y, x + width, y + height));
}

void J2DPane::add(f32 x, f32 y) {
    mTranslateX += x;
    mTranslateY += y;
    calcMtx();
}

void J2DPane::resize(f32 x, f32 y) {
    JGeometry::TBox2< f32 > box = mBounds;

    f32 tX = mTranslateX;
    f32 tY = mTranslateY;

    box.addPos(tX, tY);

    const J2DPane* parent = getParentPane();
    if (parent != NULL) {
        f32 xAdd = -parent->mBounds._0.x;
        f32 yAdd = -parent->mBounds._0.y;
        box.addPos(xAdd, yAdd);
    }

    box._8.x = box._8.x + x;
    box._8.y = box._8.y + y;
    place(box);
}

J2DPane* J2DPane::search(u64 tag) {
    if (tag == mInfoTag) {
        return this;
    }

    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        if (J2DPane* result = iter.getObject()->search(tag)) {
            return result;
        }
    }
    return nullptr;
}

J2DPane* J2DPane::searchUserInfo(u64 tag) {
    if (tag == mUserInfoTag) {
        return this;
    }

    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        if (J2DPane* result = iter.getObject()->searchUserInfo(tag)) {
            return result;
        }
    }
    return nullptr;
}

bool J2DPane::isUsed(const ResTIMG* p_timg) {
    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        if (iter.getObject()->isUsed(p_timg))
            return true;
    }
    return false;
}

bool J2DPane::isUsed(const ResFONT* p_font) {
    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        if (iter.getObject()->isUsed(p_font))
            return true;
    }
    return false;
}

void J2DPane::makeMatrix(f32 param_0, f32 param_1, f32 param_2, f32 param_3) {
    f32 tmpX = mRotateOffsetX - param_2;
    f32 tmpY = mRotateOffsetY - param_3;
    Mtx rotX, rotY, rotZ, rotMtx, mtx, tmp;
    PSMTXTrans(mtx, -tmpX, -tmpY, 0);
    PSMTXRotRad(rotX, 'x', DEG_TO_RAD(mRotateX));
    PSMTXRotRad(rotY, 'y', DEG_TO_RAD(mRotateY));
    PSMTXRotRad(rotZ, 'z', DEG_TO_RAD(-mRotateZ));
    PSMTXConcat(rotZ, rotX, tmp);
    PSMTXConcat(rotY, tmp, rotMtx);
    PSMTXScaleApply(mtx, mPositionMtx, mScaleX, mScaleY, 1);
    PSMTXConcat(rotMtx, mPositionMtx, tmp);
    PSMTXTransApply(tmp, mPositionMtx, param_0 + tmpX, param_1 + tmpY, 0);
}

void J2DPane::setCullBack(GXCullMode mode) {
    mCullMode = mode;

    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        iter.getObject()->setCullBack(mode);
    }
}

J2DPane* J2DPane::getFirstChildPane() {
    if (!getFirstChild())
        return NULL;

    return getFirstChild()->getObject();
}

J2DPane* J2DPane::getNextChildPane() {
    if (getPaneTree()->getNextChild() == NULL)
        return NULL;

    return getPaneTree()->getNextChild()->getObject();
}

J2DPane* J2DPane::getParentPane() {
    if (getPaneTree()->getParent() == NULL)
        return NULL;

    return getPaneTree()->getParent()->getObject();
}

s16 J2DPane::J2DCast_F32_to_S16(f32 value, u8 arg2) {
    if (arg2 >= 0xF) {
        return 0;
    } else {
        f32 tmpF;
        tmpF = value;
        if (value < 0) {
            tmpF = -value;
        }
        int tmp = tmpF * (1 << arg2);
        if (tmp >= 0x8000) {
            if (value < 0) {
                return 0x8000;
            } else {
                return 0x7FFF;
            }
        } else if (value < 0) {
            return ~tmp + 1;
        } else {
            return tmp;
        }
    }
}

void J2DPane::setAnimation(J2DAnmTransform* p_anm) {
    mTransform = p_anm;
}

void J2DPane::animationTransform() {
    if (mTransform != NULL)
        animationTransform(mTransform);
}

void J2DPane::clearAnmTransform() {
    J2DAnmTransform* none = NULL;
    setAnimation(none);

    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        iter.getObject()->clearAnmTransform();
    }
}

const J2DAnmTransform* J2DPane::animationTransform(const J2DAnmTransform* p_transform) {
    const J2DAnmTransform* p = p_transform;
    if (mTransform != NULL)
        p = mTransform;

    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        iter.getObject()->animationTransform(p);
    }

    updateTransform(p);
    return p;
}

void J2DPane::setVisibileAnimation(J2DAnmVisibilityFull* p_visibility) {
    setAnimationVF(p_visibility);

    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        iter.getObject()->setVisibileAnimation(p_visibility);
    }
}

void J2DPane::setVtxColorAnimation(J2DAnmVtxColor* p_vtxColor) {
    setAnimationVC(p_vtxColor);

    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        iter.getObject()->setVtxColorAnimation(p_vtxColor);
    }
}

const J2DAnmTransform* J2DPane::animationPane(const J2DAnmTransform* p_transform) {
    const J2DAnmTransform* p = p_transform;
    if (mTransform != NULL)
        p = mTransform;

    JSUTreeIterator< J2DPane > iter;
    for (iter = mPaneTree.getFirstChild(); iter != mPaneTree.getEndChild(); ++iter) {
        iter.getObject()->animationPane(p);
    }

    updateTransform(p);
    return p;
}

/*
void J2DPane::updateTransform(J2DAnmTransform const* p_anmTransform) {
    if (_4 != 0xFFFF && p_anmTransform != NULL) {
        J3DTransformInfo info;
        p_anmTransform->getTransform(_4, &info);
        mScaleX = info.mScale.x;
        mScaleY = info.mScale.z;
        mRotateX = (u16)info.mRotation.x * 360.0f / 65535.0f;
        mRotateY = (u16)info.mRotation.z * 360.0f / 65535.0f;
        mRotateZ = (u16)info.mRotation.y * 360.0f / 65535.0f;
        mTranslateX = info.mTranslate.x;
        mTranslateY = info.mTranslate.z;
        calcMtx();
    }
}
*/

JGeometry::TBox2< f32 > J2DPane::static_mBounds(0, 0, 0, 0);

void J2DPane::calcMtx() {
    if (mPaneTree.getParent() != NULL) {
        makeMatrix(mTranslateX, mTranslateY);
    }
}

void J2DPane::makeMatrix(f32 a, f32 b) {
    makeMatrix(a, b, -mBounds._0.x, -mBounds._0.y);
}

void J2DPane::rewriteAlpha() {}

void J2DPane::drawSelf(f32, f32, f32 (*)[3][4]) {}

void J2DPane::setAlpha(u8 alpha) {
    mAlpha = alpha;
}

void J2DPane::setAnimation(J2DAnmColor* anm) {}

void J2DPane::setAnimation(J2DAnmVtxColor* anm) {}

void J2DPane::setAnimation(J2DAnmTextureSRTKey* anm) {}

void J2DPane::setAnimation(J2DAnmTexPattern* anm) {}

void J2DPane::setAnimation(J2DAnmVisibilityFull* anm) {}

void J2DPane::setAnimation(J2DAnmTevRegKey* anm) {}

void J2DPane::setAnimationVF(J2DAnmVisibilityFull* p_visibility) {
    setAnimation(p_visibility);
}

void J2DPane::setAnimationVC(J2DAnmVtxColor* p_vtxColor) {
    setAnimation(p_vtxColor);
}

void J2DPane::drawSelf(f32, f32) {}

void J2DPane::update() {}

bool J2DPane::setConnectParent(bool connected) {
    mConnected = false;
    return false;
}

void J2DPane::setCullBack(bool cull) {
    GXCullMode mode;

    if (!cull)
        mode = GX_CULL_NONE;
    else
        mode = GX_CULL_BACK;

    setCullBack(mode);
}

u16 J2DPane::getTypeID() const {
    return 16;
}
