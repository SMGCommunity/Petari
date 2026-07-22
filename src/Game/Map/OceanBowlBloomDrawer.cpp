#include "Game/Map/OceanBowlBloomDrawer.hpp"
#include "Game/Map/OceanBowl.hpp"
#include "Game/Map/OceanBowlPoint.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <revolution/gx/GXVert.h>

OceanBowlBloomDrawer::OceanBowlBloomDrawer(OceanBowl* pHost) : NameObj("オーシャンボウル[ブルーム描画]"), mHost(pHost) {
}

void OceanBowlBloomDrawer::init(const JMapInfoIter&) {
    MR::connectToScene(this, -1, -1, -1, MR::DrawType_OceanBowlBloomDrawer);
}

void OceanBowlBloomDrawer::draw() const {
    if (mHost->mIsClipped || !MR::isValidDraw(mHost) || !MR::isCameraInWater()) {
        return;
    }

    mHost->loadMaterialBloom();

    OceanBowlPoint* pPoint2;
    u16 zero = 0;
    u16 one = 1;
    for (s32 x = 0; x < 24; x++) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 50);
        for (s32 y = 0; y < 25; y++) {
            pPoint2 = mHost->getPoint(x, y);
            OceanBowlPoint* pPoint = mHost->getPoint(x + 1, y);
            GXPosition3f32(pPoint->mVertexPosition.x, pPoint->mVertexPosition.y, pPoint->mVertexPosition.z);
            GXColor4u8(0xFF, 0xFF, 0xFF, mHost->getPoint(x + 1, y)->mAlpha);
            GXTexCoord2s16(zero, zero);

            GXPosition3f32(pPoint2->mVertexPosition.x, pPoint2->mVertexPosition.y, pPoint2->mVertexPosition.z);
            GXColor4u8(0xFF, 0xFF, 0xFF, mHost->getPoint(x, y)->mAlpha);
            GXTexCoord2s16(one, one);

            zero += 2;
            one += 2;
        }
        GXEnd();
    }
}
