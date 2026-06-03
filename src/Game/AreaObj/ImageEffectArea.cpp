#include "Game/AreaObj/ImageEffectArea.hpp"

ImageEffectArea::ImageEffectArea(EImageEffectType effectType, int formType, const char* pName) : AreaObj(formType, pName) {
    mEffectType = effectType;
}

ImageEffectAreaMgr::ImageEffectAreaMgr(s32 maxNum, const char* pName) : AreaObjMgr(maxNum, pName) {}

void ImageEffectAreaMgr::initAfterPlacement() {
    sort();
}

// ImageEffectAreaMgr::sort()

ImageEffectAreaMgr::~ImageEffectAreaMgr() {}