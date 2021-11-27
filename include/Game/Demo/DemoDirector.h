#pragma once

#include "Game/NameObj/NameObj.h"

class DemoDirector : public NameObj {
public:
    DemoDirector(const char *);

    virtual ~DemoDirector();
    virtual void movement();
};