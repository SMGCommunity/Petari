#include "Game/LiveActor/ShadowVolumeModel.h"
#include "Game/Util.h"

ShadowVolumeModel::ShadowVolumeModel(const char *pName) : ShadowVolumeDrawer(pName) {
    mModelData = 0;
}

void ShadowVolumeModel::initVolumeModel(const char *pModelName) {
    mModelData = MR::getJ3DModelData(pModelName);
}

void ShadowVolumeModel::drawShape() const {
    MR::drawSimpleModel(mModelData);
}