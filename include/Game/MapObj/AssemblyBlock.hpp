#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AssemblyBlock : public LiveActor {
public:
    AssemblyBlock(const char*);

    virtual ~AssemblyBlock();
    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeAssemble();
    void exeAssembleWait();
    void exeReturn();
    void exeTimer();
    bool tryStartAssemble();
    bool tryStartReturn();

    TPos3f _8C;
    TPos3f _BC;
    TPos3f _EC;
    s32 _11C;
    f32 mActivationRange;  // 0x120
    TVec3f _124;
    f32 _130;
    s32 mActivationTime;      // 0x134
    PartsModel* mBloomModel;  // 0x138
    bool _13C;
};
