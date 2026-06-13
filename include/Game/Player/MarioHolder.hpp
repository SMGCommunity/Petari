#pragma once

#include "Game/NameObj/NameObj.hpp"

class MarioActor;

class MarioHolder : public NameObj {
public:
    MarioHolder();

    virtual ~MarioHolder();

    void setMarioActor(MarioActor* actor);
    MarioActor* getMarioActor() const;

    MarioActor* mActor;
};

namespace MR {
    MarioHolder* getMarioHolder();
};  // namespace MR
