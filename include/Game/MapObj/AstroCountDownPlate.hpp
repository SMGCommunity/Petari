#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/J3DGraphBase/J3DTexture.hpp>

class AstroCountDownPlate : public LiveActor {
public:
    AstroCountDownPlate(const char*);

    virtual ~AstroCountDownPlate();
    virtual void init(const JMapInfoIter&);

    void exeWait();
    void exeRevival();
    void exeCountToZero();
    void initTextureAtNumLeftStar();
    void setNumLeftStar();
    void selectNrvWait();
    void setupStateWait();
    void startDemoStartCountDown();
    void startDemoLastBattle();

    J3DTexMtx _8C;
    J3DTexMtx _120;
    u8 _1B4;
};
