#include "Game/AreaObj/ImageEffectArea.h"

ImageEffectArea::ImageEffectArea(EImageEffectType effectType, int type, const char *pName) :
    AreaObj(type, pName) {
    mEffectType = effectType;
}

ImageEffectAreaMgr::ImageEffectAreaMgr(s32 type, const char *pName) : AreaObjMgr(type, pName) {

}

void ImageEffectAreaMgr::initAfterPlacement() {
    sort();
}

// ImageEffectAreaMgr::sort()

ImageEffectAreaMgr::~ImageEffectAreaMgr() {
    
}