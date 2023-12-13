#include "Game/AreaObj/HazeCube.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

HazeCube::HazeCube(int type, const char *pName) : AreaObj(type, pName) {
    _3C = 1000.0f;
}

HazeCube::~HazeCube() {

}

void HazeCube::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);

    if (mObjArg0 >= 0) {
        _3C = static_cast<f32>(mObjArg0);
    }
    
    MR::createSceneObj(SceneObj_HeatHazeDirector);
}

const char *HazeCube::getManagerName() const {
    return "HazeCube";
}
