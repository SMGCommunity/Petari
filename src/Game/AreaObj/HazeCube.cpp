#include "Game/AreaObj/HazeCube.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace {
    static const f32 sDefaultDistance = 1000.0f;
};  // namespace

HazeCube::HazeCube(int formType, const char* pName) : AreaObj(formType, pName), mDistance(::sDefaultDistance) {
}

void HazeCube::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    if (mObjArg0 >= 0) {
        mDistance = mObjArg0;
    }

    MR::createSceneObj(SceneObj_HeatHazeDirector);
}

HazeCube::~HazeCube() {
}
