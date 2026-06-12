#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MapPartsRailMover;
class PartsModel;

class ElectricBall : public LiveActor {
public:
    ElectricBall(const char*);

    virtual ~ElectricBall();
    virtual void init(const JMapInfoIter&);

    void initBalls(const JMapInfoIter&);
    void startMove();
    void exeWait();

    class Ball {
        Ball();
        void activate();
        void init(LiveActor*);
        void updatePosition(const TPos3f&);

        PartsModel* _0;
        TVec3f mPosition;
        bool _10;
    };

    TMtx34f _8C;
    MR::AssignableArray< Ball > _BC;
    TVec3f _C4;
    MapPartsRailMover* _4C;
    f32 mArg1;  // _D0
    f32 mArg2;  // _D4
};
