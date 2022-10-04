#pragma once

#include "Game/LiveActor/LiveActor.h"

class AssemblyBlock : public LiveActor {
public:
    AssemblyBlock(const char *);

    virtual ~AssemblyBlock();
    virtual void init(const JMapInfoIter &);
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
    f32 mActivationRange;       // _120
    TVec3f _124;
    f32 _130;
    s32 mActivationTime;        // _134
    PartsModel* mBloomModel;    // _138
    bool _13C;
};

namespace NrvAssemblyBlock {
    NERVE_DECL(AssemblyBlockNrvWait, AssemblyBlock, AssemblyBlock::exeWait);
    NERVE_DECL(AssemblyBlockNrvAssemble, AssemblyBlock, AssemblyBlock::exeAssemble);
    NERVE_DECL(AssemblyBlockNrvAssembleWait, AssemblyBlock, AssemblyBlock::exeAssembleWait);
    NERVE_DECL(AssemblyBlockNrvReturn, AssemblyBlock, AssemblyBlock::exeReturn);
    NERVE_DECL(AssemblyBlockNrvTimer, AssemblyBlock, AssemblyBlock::exeTimer);
};