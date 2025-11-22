#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class ImageEffectArea : public AreaObj {
public:
    enum EImageEffectType { IMAGE_EFFECT_TYPE_BLOOM = 0, IMAGE_EFFECT_TYPE_SIMPLE_BLOOM = 1, IMAGE_EFFECT_TYPE_BLUR = 2, IMAGE_EFFECT_TYPE_DOF = 3 };

    ImageEffectArea(EImageEffectType, int, const char*);

    virtual ~ImageEffectArea() {}

    virtual bool isSyncWithPlayer() const { return false; }

    s32 mEffectType;  // 0x3C
};

class ImageEffectAreaMgr : public AreaObjMgr {
public:
    ImageEffectAreaMgr(s32, const char*);

    virtual ~ImageEffectAreaMgr();
    virtual void initAfterPlacement();

    void sort();
};