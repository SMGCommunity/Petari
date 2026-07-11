#include "Game/LiveActor/ShadowVolumeBox.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/ShadowDrawer.hpp"
#include "Game/Util/CameraUtil.hpp"

void FORCE_MATCH_SDATA2() {
    f32 f1 = 0.0f;
    f32 f2 = 0.5f;
}

ShadowVolumeBox::ShadowVolumeBox() : ShadowVolumeDrawer("影描画[ボリュームボックス]"), mSize(100.0f, 100.0f, 100.0f) {
    for (u32 idx = 0; idx < ARRAY_SIZE(mPoints); idx++) {
        mPoints[idx].zeroInline();
    }
}

void ShadowVolumeBox::setSize(const TVec3f& rSize) {
    mSize.set< f32 >(rSize);
}

void ShadowVolumeBox::loadModelDrawMtx() const {
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    makeVertexBuffer();
}

void ShadowVolumeBox::makeVertexBuffer() const {
    // FIXME: regswap and load swap
    // https://decomp.me/scratch/MMjPY
    ShadowController* controller = getController();
    TPos3f* mtx = (TPos3f*)controller->_18;

    // TODO/FIXME: why are these ctors for the get_Dir behavior?
    TVec3f side(mtx->mMtx[0][0], mtx->mMtx[1][0], mtx->mMtx[2][0]);
    TVec3f up(mtx->mMtx[0][1], mtx->mMtx[1][1], mtx->mMtx[2][1]);
    TVec3f front(mtx->mMtx[0][2], mtx->mMtx[1][2], mtx->mMtx[2][2]);

    TVec3f pos, dir;
    calcBaseDropPosition(&pos);
    controller->getDropDir(&dir);

    bool dirRight = dir.dot(side) >= 0.0f;
    bool dirUp = dir.dot(up) >= 0.0f;
    bool dirFront = dir.dot(front) >= 0.0f;

    TVec3f size = mSize;
    if (controller->isFollowHostScale()) {
        // FIXME: some weirder TVec inline happening here
        size *= controller->getHost()->mScale * 0.5f;
    } else {
        size.scale(0.5f);
    }

    side.scale(size.x);
    up.scale(size.y);
    front.scale(size.z);

    dir.scale(calcBaseDropLength());
    mPoints[0].set(pos + side + up + front);
    mPoints[1].set(pos + side + up - front);
    mPoints[2].set(pos + side - up + front);
    mPoints[3].set(pos + side - up - front);
    mPoints[4].set(pos - side + up + front);
    mPoints[5].set(pos - side + up - front);
    mPoints[6].set(pos - side - up + front);
    mPoints[7].set(pos - side - up - front);

    if (dirRight) {
        if (dirUp) {
            if (dirFront) {
                mPoints[12].set(mPoints[6] + dir);
                mPoints[13].set(mPoints[1]);
            } else {
                mPoints[12].set(mPoints[0]);
                mPoints[13].set(mPoints[7] + dir);
            }
            mPoints[8].set(mPoints[4] + dir);
            mPoints[9].set(mPoints[5] + dir);
            mPoints[10].set(mPoints[2]);
            mPoints[11].set(mPoints[3]);
        } else {
            if (dirFront) {
                mPoints[12].set(mPoints[4] + dir);
                mPoints[13].set(mPoints[3]);
            } else {
                mPoints[12].set(mPoints[2]);
                mPoints[13].set(mPoints[5] + dir);
            }
            mPoints[8].set(mPoints[0]);
            mPoints[9].set(mPoints[1]);
            mPoints[10].set(mPoints[6] + dir);
            mPoints[11].set(mPoints[7] + dir);
        }
        mPoints[0] += dir;
        mPoints[1] += dir;
        mPoints[2] += dir;
        mPoints[3] += dir;
    } else {
        if (dirUp) {
            if (dirFront) {
                mPoints[12].set(mPoints[2] + dir);
                mPoints[13].set(mPoints[5]);
            } else {
                mPoints[12].set(mPoints[4]);
                mPoints[13].set(mPoints[3] + dir);
            }
            mPoints[8].set(mPoints[0] + dir);
            mPoints[9].set(mPoints[1] + dir);
            mPoints[10].set(mPoints[6]);
            mPoints[11].set(mPoints[7]);
        } else {
            if (dirFront) {
                mPoints[12].set(mPoints[0] + dir);
                mPoints[13].set(mPoints[7]);
            } else {
                mPoints[12].set(mPoints[6]);
                mPoints[13].set(mPoints[1] + dir);
            }
            mPoints[8].set(mPoints[4]);
            mPoints[9].set(mPoints[5]);
            mPoints[10].set(mPoints[2] + dir);
            mPoints[11].set(mPoints[3] + dir);
        }
        mPoints[4] += dir;
        mPoints[5] += dir;
        mPoints[6] += dir;
        mPoints[7] += dir;
    }
}

void ShadowVolumeBox::drawShape() const {
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 0xE);
    TDDraw::sendPoint(mPoints[0]);
    TDDraw::sendPoint(mPoints[1]);
    TDDraw::sendPoint(mPoints[2]);
    TDDraw::sendPoint(mPoints[3]);
    TDDraw::sendPoint(mPoints[10]);
    TDDraw::sendPoint(mPoints[11]);
    TDDraw::sendPoint(mPoints[6]);
    TDDraw::sendPoint(mPoints[7]);
    TDDraw::sendPoint(mPoints[4]);
    TDDraw::sendPoint(mPoints[5]);
    TDDraw::sendPoint(mPoints[8]);
    TDDraw::sendPoint(mPoints[9]);
    TDDraw::sendPoint(mPoints[0]);
    TDDraw::sendPoint(mPoints[1]);
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 8);
    TDDraw::sendPoint(mPoints[12]);
    TDDraw::sendPoint(mPoints[0]);
    TDDraw::sendPoint(mPoints[2]);
    TDDraw::sendPoint(mPoints[10]);
    TDDraw::sendPoint(mPoints[6]);
    TDDraw::sendPoint(mPoints[4]);
    TDDraw::sendPoint(mPoints[8]);
    TDDraw::sendPoint(mPoints[0]);
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 8);
    TDDraw::sendPoint(mPoints[13]);
    TDDraw::sendPoint(mPoints[1]);
    TDDraw::sendPoint(mPoints[9]);
    TDDraw::sendPoint(mPoints[5]);
    TDDraw::sendPoint(mPoints[7]);
    TDDraw::sendPoint(mPoints[11]);
    TDDraw::sendPoint(mPoints[3]);
    TDDraw::sendPoint(mPoints[1]);
}
