#include "Game/AreaObj/QuakeEffectArea.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

QuakeEffectArea::QuakeEffectArea(int formType, const char* pName) : AreaObj(formType, pName) {
    if (!MR::isExistSceneObj(SceneObj_QuakeEffectGenerator)) {
        MR::createSceneObj(SceneObj_QuakeEffectGenerator);
    }
}
