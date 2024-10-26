#include "Game/AreaObj/QuakeEffectArea.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

QuakeEffectArea::QuakeEffectArea(int type, const char *pName) : AreaObj(type, pName) {
    if (!MR::createSceneObj(SceneObj_QuakeEffectGenerator)) {
        MR::createSceneObj(SceneObj_QuakeEffectGenerator);
    }
}

QuakeEffectArea::~QuakeEffectArea() {

}