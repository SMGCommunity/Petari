#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class SimpleBloomArea : public ImageEffectArea {
public:
    SimpleBloomArea(int, const char*);
    virtual ~SimpleBloomArea();

    virtual void init(const JMapInfoIter&);

    virtual const char* getManagerName() const;

    u32 _40;
    u8 _44;
    u8 _45;
    u8 _46[2];
};
