#include "Game/Player/DrawAdaptor.hpp"
#include "Game/Util/ObjUtil.hpp"

DrawAdaptor::DrawAdaptor(const MR::FunctorBase &rFunctor, s32 a2) : NameObj("ドロー2D") {
    mFunctor = rFunctor.clone(nullptr);
    MR::connectToScene(this, -1, -1, -1, a2);
}

DrawAdaptor::~DrawAdaptor() {}

void DrawAdaptor::draw() const {
    mFunctor->operator()();
}
