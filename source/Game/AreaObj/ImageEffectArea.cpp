#include "Game/AreaObj/ImageEffectArea.h"

ImageEffectArea::ImageEffectArea(EImageEffectType effectType, int type, const char *pName) :
    AreaObj(type, pName) {
    mEffectType = effectType;
}