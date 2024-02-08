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

	s32 _0; // This is probably wrong
    Spine* mSpine; // _4
};
