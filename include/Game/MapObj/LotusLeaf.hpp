#pragma  once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"


class LotusLeaf : LiveActor {
public:
    LotusLeaf(const char *);

    virtual ~LotusLeaf();
    virtual void init(const JMapInfoIter &);

    void convergeToInitPos();
    void exeWait();
    void exeWaitPlayerOn();
    void exeShake();

    TVec3f _8C;
    f32 _98;
    f32 _9C;
};
