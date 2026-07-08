#include "Game/Player/DrawAdaptor.hpp"
#include "Game/Util/ObjUtil.hpp"

DrawAdaptor::DrawAdaptor(const MR::FunctorBase& rFunc, int drawType) : NameObj("ドロー2D"), mFunc(rFunc.clone(nullptr)) {
    MR::connectToScene(this, -1, -1, -1, drawType);
}

void DrawAdaptor::draw() const {
    (*mFunc)();
}

DrawAdaptor::~DrawAdaptor() {
}
