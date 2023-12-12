#pragma once

#include "Game/AreaObj/AreaObj.h"

class SwitchArea : public AreaObj {
public:
    SwitchArea(int, const char *);
    virtual ~SwitchArea();

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual const char *getManagerName() const;
            
    bool isUpdate() const;
};