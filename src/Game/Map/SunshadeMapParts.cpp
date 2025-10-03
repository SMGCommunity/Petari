#include "Game/Map/SunshadeMapParts.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/GeneralMapParts.hpp"
#include "Game/Map/SunshadeMapHolder.hpp"

SunshadeMapParts::SunshadeMapParts(const char *pName) : GeneralMapParts(pName) {
    mCollision = nullptr;
}

void SunshadeMapParts::calcAnim() {
    LiveActor::calcAnim();

    if (mCollision != nullptr) {
        MR::setCollisionMtx(this, mCollision);
    }
}

namespace MR {
    SunshadeMapParts* createSunshadeMapParts(const char *pName) {
        return new SunshadeMapParts(pName);
    }
};

SunshadeMapParts::~SunshadeMapParts() {

}

void SunshadeMapParts::init(const JMapInfoIter &rIter) {
    GeneralMapParts::init(rIter);
    MR::createSunshadeMapHolder();
    mCollision = MR::tryCreateCollisionSunshade(this, getSensor("body"));
}
