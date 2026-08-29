#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class PartsModel;

class AssemblyBlock : public LiveActor {
public:
    /// @brief Creates a new `AssemblyBlock`.
    /// @param A pointer to the null-terminated name of the object.
    AssemblyBlock(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeAssemble();
    void exeAssembleWait();
    void exeReturn();
    void exeTimer();
    bool tryStartAssemble();
    bool tryStartReturn();

    /* 0x08C */ TPos3f _8C;
    /* 0x0BC */ TPos3f _BC;
    /* 0x0EC */ TPos3f _EC;
    /* 0x11C */ s32 mObjArg7;
    /* 0x120 */ f32 mPlayerSearchDistance;
    /* 0x124 */ TVec3f _124;
    /* 0x130 */ f32 mFloatRotateSpeed;
    /* 0x134 */ s32 mTimer;
    /* 0x138 */ PartsModel* mBloomModel;
    /* 0x13C */ bool _13C;
};
