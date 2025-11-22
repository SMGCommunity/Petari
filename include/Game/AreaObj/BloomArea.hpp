#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class BloomArea : public ImageEffectArea {
public:
    BloomArea(int, const char*);
    virtual ~BloomArea();

    virtual void init(const JMapInfoIter&);
    virtual const char* getManagerName() const;

    u8 _40;
    u8 _41;
    u8 _42[2];
    s32 _44;
    s32 _48;
};
