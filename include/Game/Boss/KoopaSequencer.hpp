#pragma once

#include "Game/System/NerveExecutor.hpp"

class Koopa;
class JMapInfoIter;
class KoopaDemoPowerUp;
class KoopaDemoJumpToPlanet;


class KoopaSequencer : public NerveExecutor {
public:
    KoopaSequencer();

    virtual ~KoopaSequencer();
    virtual void init(Koopa*, const JMapInfoIter&);
    virtual void updateNerve();
    virtual void calcAndSetBaseMtx();

private:
    /* 0x8 */ Koopa* mKoopa;
    /* 0xC */ KoopaDemoPowerUp* mPowerUp;
    /* 0x10 */ u32 _10;
    /* 0x14 */ KoopaDemoJumpToPlanet* _14;
    /* 0x18 */ u32 _18;
    /* 0x1C */ u32 mVs;
    /* 0x20 */ u32 _20;
};
