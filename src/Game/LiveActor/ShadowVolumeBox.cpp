#include "Game/LiveActor/ShadowVolumeBox.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/LiveActor/ShadowDrawer.hpp"
#include "Game/Util/CameraUtil.hpp"

void ShadowVolumeBox_FORCE_MATCH_SDATA2() {
    f32 f1 = 0.0f;
    f32 f2 = 0.5f;
}

ShadowVolumeBox::ShadowVolumeBox() : ShadowVolumeDrawer("影描画[ボリュームボックス]"), mSize(100.0f, 100.0f, 100.0f) {
    for (u32 idx = 0; idx < ARRAY_SIZE(mPoints); idx++) {
        mPoints[idx].zero();
    }
}

void ShadowVolumeBox::setSize(const TVec3f& rSize) {
    mSize.set(rSize);
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
    MtxPtr mtx;
    ShadowController* controller = getController();
    mtx = controller->_18;
    TVec3f side(mtx[0][0], mtx[1][0], mtx[2][0]);
    TVec3f up(mtx[0][1], mtx[1][1], mtx[2][1]);
    TVec3f front(mtx[0][2], mtx[1][2], mtx[2][2]);

    TVec3f pos, dir;
    calcBaseDropPosition(&pos);
    controller->getDropDir(&dir);

    bool dirRight = dir.dot(side) >= 0.0f;
    bool dirUp = dir.dot(up) >= 0.0f;
    bool dirFront = dir.dot(front) >= 0.0f;

    TVec3f size = mSize;
    if (controller->isFollowHostScale()) {
        TVec3f scale(controller->getHost()->mScale);
        scale /= 2.0f;
        size *= scale;
    } else {
        size /= 2.0f;
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
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 14);
    {
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
    }

    GXEnd();

    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 8);
    {
        TDDraw::sendPoint(mPoints[12]);
        TDDraw::sendPoint(mPoints[0]);
        TDDraw::sendPoint(mPoints[2]);
        TDDraw::sendPoint(mPoints[10]);
        TDDraw::sendPoint(mPoints[6]);
        TDDraw::sendPoint(mPoints[4]);
        TDDraw::sendPoint(mPoints[8]);
        TDDraw::sendPoint(mPoints[0]);
    }

    GXEnd();

    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 8);
    {
        TDDraw::sendPoint(mPoints[13]);
        TDDraw::sendPoint(mPoints[1]);
        TDDraw::sendPoint(mPoints[9]);
        TDDraw::sendPoint(mPoints[5]);
        TDDraw::sendPoint(mPoints[7]);
        TDDraw::sendPoint(mPoints[11]);
        TDDraw::sendPoint(mPoints[3]);
        TDDraw::sendPoint(mPoints[1]);
    }

    GXEnd();
}
