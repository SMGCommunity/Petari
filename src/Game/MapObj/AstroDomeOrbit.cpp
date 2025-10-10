#include "Game/MapObj/AstroDomeOrbit.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

namespace {
    static f32 cRotateOutermost[3] = {
        20.0f, 45.0f, 0.0f
    };

    static f32 cRadius[5] = {
        4000.0f, 6200.0f, 8100.0f, 10300.0f, 12000.0f
    };

    static f32 cRadiusLastDome[4] = {
        4000.0f, 6700.0f, 9100.0f, 11800.0f
    };

    static Color8 cColor(19, 177, 255, 255);
    static Color8 cBloomColor(0, 180, 100, 255);
};

AstroDomeOrbit::AstroDomeOrbit() : LiveActor("天文ドームの軌道"), _8C(5000.0f), _90(0.0f) {}

/* void AstroDomeOrbit::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, -1, -1, -1, 31);
    MR::invalidateClipping(this);
    MR::FunctorBase& func1 = MR::Functor(this, &AstroDomeOrbit::drawBloom, &AstroDomeOrbit::draw);   
    MR::createAdaptorAndConnectToDrawBloomModel("天文ドーム軌道ブルーム描画", func1);
    makeActorDead();
} */

void AstroDomeOrbit::draw() const {
    if (MR::isValidDraw(this)) {
        initDraw(cColor);
        drawCelling(100.0f, true, 50.0f);
        drawCelling(100.0f, false, 50.0f);
        drawSide(100.0f, true, 50.0f);
        drawSide(100.0f, false, 50.0f);
    }
}

void AstroDomeOrbit::drawBloom() const {
    if (MR::isValidDraw(this)) {
        initDraw(cColor);
        drawCelling(131.0f, true, 60.0f);
        drawCelling(131.0f, false, 60.0f);
        drawSide(131.0f, true, 60.0f);
        drawSide(131.0f, false, 60.0f);
    }
}

void AstroDomeOrbit::moveCoord() {
    _90 = calcRepeatedRotateCoord(_90 + -0.05f);
}

void AstroDomeOrbit::setup(s32 radiusIdx) {
    s32 miniNum = MiniatureGalaxyFunction::getMiniatureGalaxyNum();
    f32* domes = cRadiusLastDome;

    if (radiusIdx == 5) {
        domes = cRadius;
    }

    f32 radiusFlt = radiusIdx;
    _8C = domes[radiusIdx];
    _90 = 230.0f * (radiusFlt - 4.503601774854144e15);

    if (radiusIdx >= 4) {
        f32 z = cRotateOutermost[0];
        f32 y = cRotateOutermost[1];
        f32 x = cRotateOutermost[2];
        
        mRotation.set(z, y, x);
    }
}
