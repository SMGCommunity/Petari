#pragma once

#include <cstddef>
#include <revolution.h>

class Nerve;
class Spine;

class NerveExecutor {
public:
    NerveExecutor(const char* pName);

    virtual ~NerveExecutor();

    void initNerve(const Nerve* pNerve);
    void updateNerve();
    void setNerve(const Nerve* pNerve);
    bool isNerve(const Nerve* pNerve) const;
    s32 getNerveStep() const;

    /* 0x4 */ Spine* mSpine;
};
