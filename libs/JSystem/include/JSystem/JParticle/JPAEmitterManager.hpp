#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <revolution/types.h>

class JKRHeap;
class JPABaseEmitter;
class JPADrawInfo;
class JPAEmitterCallBack;
class JPAParticleCallBack;
class JPAResourceManager;

class JPAEmitterManager {
public:
    JPAEmitterManager(u32, u32, JKRHeap*, u8, u8);

    JPABaseEmitter* createSimpleEmitterID(const TVec3f&, u16, u8, u8, JPAEmitterCallBack*, JPAParticleCallBack*);
    void calc(u8);
    void calcYBBCam();
    void draw(const JPADrawInfo*, u8);
    void forceDeleteEmitter(JPABaseEmitter*);
    void entryResourceManager(JPAResourceManager*, u8);

    /* 0x00 */ void* _0;
    /* 0x04 */ JSUPtrList _4;
    /* 0x10 */ void* _10; // Part of JPAList<T>
    /* 0x14 */ void* _14; // Part of JPAList<T>
    /* 0x18 */ u32 _18; // Part of JPAList<T>
    /* 0x1C */ void* _1C;
    /* 0x20 */ JPABaseEmitter* _20;
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 _28;
    /* 0x2C */ u8 _2C;
    /* 0x2D */ u8 _2D;
};
