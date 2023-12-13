#include "Game/Map/GroundChecker.hpp"

GroundChecker::GroundChecker(const char *pName, f32 a2, f32 a3) : LiveActor(pName) {
    _8C = 1.0f;
    _90 = 0.0f;
    MR::invalidateClipping(this);
    initBinder(a2, a3, 0);
    makeActorAppeared();
}

void GroundChecker::bringFriction(f32 a1, f32 a2) {
    _8C = (_8C * (1.0f - a2)) + (a1 * a2);
}

GroundChecker::~GroundChecker() {
    
}