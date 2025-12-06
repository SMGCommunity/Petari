#pragma once

#include "Game/MapObj/BenefitItemLifeUp.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"
#include "Game/NPC/TicoComet.hpp"

class TicoShop : public TicoComet {
public:
    TicoShop(const char*);

    virtual ~TicoShop();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void startReactionSound();

    bool branchFunc(u32);
    bool eventFunc(u32);
    void exeDemo();

    BenefitItemOneUp* mOneUp;    // 0x19C
    BenefitItemLifeUp* mLifeUp;  // 0x1A0
    bool mChoseOneUp;            // 0x1A4
    bool mChoseLifeUp;           // 0x1A5
};
