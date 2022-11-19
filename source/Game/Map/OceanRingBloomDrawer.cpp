#include "Game/Map/OceanRingBloomDrawer.h"
#include "Game/Map/OceanRingDrawer.h"
#include "Game/Map/OceanRing.h"

OceanRingBloomDrawer::OceanRingBloomDrawer(OceanRing *pRing) : NameObj("オーシャンリング[ブルーム描画]") {
    mRing = pRing;
}

void OceanRingBloomDrawer::init(const JMapInfoIter &rIter) {
    MR::connectToScene(this, -1, -1, -1, 0x21);
}

void OceanRingBloomDrawer::draw() const {
    if (!MR::isValidDraw(mRing) || !MR::isCameraInWater()) {
        return;
    }

    mRing->mRingDrawer->drawBloom();
}

OceanRingBloomDrawer::~OceanRingBloomDrawer() {

}
