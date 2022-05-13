#include "Game/AreaObj/QuakeEffectArea.h"
#include "Game/Scene/SceneObjHolder.h"

QuakeEffectArea::QuakeEffectArea(int type, const char *pName) : AreaObj(type, pName) {
    if (!MR::createSceneObj(SceneObj_QuakeEffectGenerator)) {
        MR::createSceneObj(SceneObj_QuakeEffectGenerator);
    }
}

QuakeEffectArea::~QuakeEffectArea() {

}