#pragma once

#include "Game/NameObj/NameObj.hpp"

class MarioActor;

class MarioHolder : public NameObj {
public:
    MarioHolder();
    ~MarioHolder();

    void setMarioActor(MarioActor* actor);
    MarioActor* getMarioActor() const;

    MarioActor* mActor;
};

namespace MR {
    MarioHolder* getMarioHolder();
};
