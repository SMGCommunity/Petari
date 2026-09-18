#include "Game/Screen/GalaxyNamePlateDrawer.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <algorithm>

namespace {
    struct PlatePriorityLt {
        bool operator()(const GalaxyNamePlatePrioritied* pEntry1, const GalaxyNamePlatePrioritied* pEntry2) {
            return pEntry1->mZ < pEntry2->mZ;
        }
    };
};  // namespace

GalaxyNamePlateDrawer::GalaxyNamePlateDrawer() : NameObj("ギャラクシー名プレート描画") {
    MR::zeroMemory(&mPlateArray, sizeof(mPlateArray.mArray));
}

void GalaxyNamePlateDrawer::init(const JMapInfoIter&) {
    MR::connectToScene(this, -1, -1, -1, MR::DrawType_GalaxyNamePlate);
}

void GalaxyNamePlateDrawer::draw() const {
    const GalaxyNamePlatePrioritied* sortArray[16];
    MR::copyMemory(sortArray, &mPlateArray[0], mPlateArray.size() * sizeof(GalaxyNamePlatePrioritied*));
    std::sort(sortArray, sortArray + mPlateArray.size(), ::PlatePriorityLt());

    for(int i = 0; i < mPlateArray.size(); i++) {
        sortArray[i]->mHostActor->draw();
    }
}

GalaxyNamePlatePrioritied* GalaxyNamePlateDrawer::registerPlate(const LayoutActor* pPlate) {
    GalaxyNamePlatePrioritied* newEntry = new GalaxyNamePlatePrioritied(pPlate);
    mPlateArray.mArray[mPlateArray.mCount] = newEntry;
    mPlateArray.mCount++;
    return newEntry;
}

namespace MR {
    GalaxyNamePlatePrioritied* registerToGalaxyNamePlateDrawer(const LayoutActor* pPlate) {
        createSceneObj(SceneObj_GalaxyNamePlateDrawer);
        return getSceneObj< GalaxyNamePlateDrawer >(SceneObj_GalaxyNamePlateDrawer)->registerPlate(pPlate);
    }
};  // namespace MR
