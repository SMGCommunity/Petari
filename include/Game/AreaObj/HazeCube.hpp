#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class HazeCube : public AreaObj {
public:
    HazeCube(int, const char*);
    virtual ~HazeCube();

    virtual void init(const JMapInfoIter&);
    virtual const char* getManagerName() const;

    f32 _3C;
};
