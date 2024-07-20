#include "Game/MapObj/AstroDomeOrbit.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"

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
};

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
        
        mRotation.setInline(z, y, x);
    }
}