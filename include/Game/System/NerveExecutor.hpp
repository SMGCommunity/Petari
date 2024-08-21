#pragma once

#include "Game/LiveActor/Spine.hpp"

class NerveExecutor {
public:
    NerveExecutor(const char *);

    virtual ~NerveExecutor();

    void initNerve(const Nerve *);
    void updateNerve();
    void setNerve(const Nerve *);
    bool isNerve(const Nerve* ) const;
    s32 getNerveStep() const;

    Spine* mSpine; // 0x4
};
