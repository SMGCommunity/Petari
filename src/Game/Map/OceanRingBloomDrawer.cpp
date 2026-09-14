#include "Game/Map/OceanRingBloomDrawer.hpp"
#include "Game/Map/OceanRing.hpp"
#include "Game/Map/OceanRingDrawer.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

OceanRingBloomDrawer::OceanRingBloomDrawer(OceanRing* pRing) : NameObj("オーシャンリング[ブルーム描画]") {
    mRing = pRing;
}

void OceanRingBloomDrawer::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_None, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_OceanBowlBloomDrawer);
}

void OceanRingBloomDrawer::draw() const {
    if (!MR::isValidDraw(mRing) || !MR::isCameraInWater()) {
        return;
    }

    mRing->mRingDrawer->drawBloom();
}

OceanRingBloomDrawer::~OceanRingBloomDrawer() {
}
