#include "Game/Screen/StarPointerTarget.hpp"
#include "Game/Screen/LayoutCoreUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <revolution/mtx.h>

namespace {
    static const f32 hZCheckAddDepth = 60.0f;
};  // namespace

StarPointerTarget::StarPointerTarget(f32 radius, const TVec3f* pPosition, MtxPtr pMtx, TVec3f offset)
    : mRadius3d(radius), mOffset(offset), mLastPointedChannel(WPAD_CHAN0), mPosition(pPosition), mMtx(pMtx) {
}

bool StarPointerTarget::calcScreenPosition(TVec2f* pScreenPos) const {
    TVec3f worldPos, viewPos;
    calcPosition(&worldPos);
    PSMTXMultVec(MR::getStarPointerViewMtx(), &worldPos, &viewPos);
    return calcScreenPositionFromView(pScreenPos, viewPos);
}

bool StarPointerTarget::isPointing(const TVec2f& rPointerPos, f32 zMargin, f32 radiusMargin) const {
    TVec3f worldPos, viewPos;
    calcPosition(&worldPos);
    PSMTXMultVec(MR::getStarPointerViewMtx(), &worldPos, &viewPos);

    if (-viewPos.z <= 0.0f) {
        return false;
    }

    if (zMargin + mRadius3d + ::hZCheckAddDepth < -viewPos.z) {
        return false;
    }

    f32 radius = calcRadius2d(mRadius3d, radiusMargin, -viewPos.z);
    TVec2f screenPos;
    if (!calcScreenPositionFromView(&screenPos, viewPos)) {
        return false;
    }

    if (rPointerPos.squared(screenPos) < radius * radius) {
        return true;
    }

    return false;
}

void StarPointerTarget::calcPosition(TVec3f* pWorldPos) const {
    if (mMtx != nullptr) {
        if (mPosition != nullptr) {
            pWorldPos->set< f32 >(mPosition->x, mPosition->y, mPosition->z);
        } else {
            pWorldPos->set< f32 >(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        }

        pWorldPos->x += mMtx[0][0] * mOffset.x + mMtx[0][1] * mOffset.y + mMtx[0][2] * mOffset.z;
        pWorldPos->y += mMtx[1][0] * mOffset.x + mMtx[1][1] * mOffset.y + mMtx[1][2] * mOffset.z;
        pWorldPos->z += mMtx[2][0] * mOffset.x + mMtx[2][1] * mOffset.y + mMtx[2][2] * mOffset.z;
    } else {
        pWorldPos->set< f32 >(mPosition->x, mPosition->y, mPosition->z);
        pWorldPos->add(mOffset);
    }
}

f32 StarPointerTarget::calcRadius2d(f32 radius3d, f32 margin, f32 z) const {
    f32 fovyRad = MR::getFovy() * PI_180;
    f32 radius2d = (radius3d * ((MR::getScreenHeight() * 0.5f) / MR::tan(fovyRad * 0.5f)) / z);
    return radius2d + margin;
}

bool StarPointerTarget::calcScreenPositionFromView(TVec2f* pScreenPos, const TVec3f& rViewPos) const {
    // FIXME: float regswaps, one in y parameter of TVec4 in mtx.mult
    // https://decomp.me/scratch/RdGzK

    TProj3f mtx;
    mtx.setInline(MR::getStarPointerProjMtx());
    TVec3f pos;
    mtx.mult(rViewPos, pos);
    pos.y = -pos.y;

    if (1.0f < __fabsf(pos.x) || 1.0f < __fabsf(pos.y)) {
        return false;
    }

    if (0.0f < pos.z) {
        return false;
    }

    f32 width = MR::getScreenWidth();
    f32 height = MR::getScreenHeight();
    pScreenPos->set< f32 >(width * 0.5f + pos.x * width * 0.5f, height * 0.5f + pos.y * height * 0.5f);
    return true;
}

StarPointerLayoutTarget::StarPointerLayoutTarget(LayoutActor* pLayoutActor, const char* pLayoutName, const char* pPaneName)
    : mLayout(pLayoutActor), mLayoutName(pLayoutName), mPaneName(pPaneName), mType(TargetType_Circle), mPosition(0.0f, 0.0f), mRadius(0.0f),
      mWidth(0.0f), mHeight(0.0f) {
}

bool StarPointerLayoutTarget::isPointing(const TVec2f& rPos) const {
    switch (mType) {
    case TargetType_Circle:
        return isPointingCircle(rPos);
    case TargetType_Rectangle:
        return isPointingRectangle(rPos);
    default:
        return false;
    }
}

bool StarPointerLayoutTarget::isPointingCircle(const TVec2f& rPos) const {
    // FIXME: regalloc
    // https://decomp.me/scratch/DbtaX

    TVec2f pos;
    calcBasePos(&pos);
    f32 r = mRadius * mRadius;
    f32 dist = pos.squared(rPos);
    return dist < r;
}

bool StarPointerLayoutTarget::isPointingRectangle(const TVec2f& rPos) const {
    // FIXME: float regswaps
    // https://decomp.me/scratch/PLVjp

    TVec2f pos;
    calcBasePos(&pos);

    TBox2f bounds;
    bounds.setInline(pos.x, pos.y, pos.x + mWidth, pos.y + mHeight);

    return bounds.intersectsPoint(rPos);
}

void StarPointerLayoutTarget::calcBasePos(TVec2f* pPos) const {
    if (mPaneName == nullptr) {
        pPos->y = 0.0f;
        pPos->x = 0.0f;
    } else {
        MR::convertPaneLocalPosToScreenPos(pPos, MR::getPane(mLayout, mPaneName), TVec2f(0.0f, 0.0f));
    }

    pPos->x += mPosition.x;
    pPos->y += mPosition.y;
}

StarPointerLayoutTargetKeeper::StarPointerLayoutTargetKeeper(int maxNumTargets) : mMaxNumTargets(maxNumTargets), mNumTargets(0), mTargets(nullptr) {
    mTargets = new StarPointerLayoutTarget*[mMaxNumTargets];
    memset(mTargets, 0, mMaxNumTargets * sizeof(StarPointerLayoutTarget*));
}

void StarPointerLayoutTargetKeeper::addTargetCircle(LayoutActor* pLayoutActor, const char* pLayoutName, f32 radius, const TVec2f& rPosition,
                                                    const char* pPaneName) {
    StarPointerLayoutTarget* target = new StarPointerLayoutTarget(pLayoutActor, pLayoutName, pPaneName != nullptr ? pPaneName : pLayoutName);
    target->mType = StarPointerLayoutTarget::TargetType_Circle;
    target->mRadius = radius;
    target->mPosition.set(rPosition);
    registerTarget(target);
}

StarPointerLayoutTarget* StarPointerLayoutTargetKeeper::getTarget(const char* pLayoutName) const {
    for (s32 idx = 0; idx < mNumTargets; idx++) {
        StarPointerLayoutTarget* target = mTargets[idx];
        if (MR::isEqualString(pLayoutName, target->mLayoutName)) {
            return target;
        }
    }
    return nullptr;
}

void StarPointerLayoutTargetKeeper::registerTarget(StarPointerLayoutTarget* pTarget) {
    mTargets[mNumTargets] = pTarget;
    mNumTargets++;
}
