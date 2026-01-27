#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JParticle/JPADrawInfo.hpp"
#include "JSystem/JParticle/JPAList.hpp"
#include "JSystem/JSupport/JSUList.hpp"
#include <revolution/types.h>


class JPAEmitterCallBack;
class JPAParticleCallBack;
class JPABaseEmitter;
class JPAResourceManager;
class JPABaseParticle;
class JKRHeap;
struct JPAEmitterWorkData;

class JPAEmitterManager {
public:
    JPAEmitterManager(u32, u32, JKRHeap*, u8, u8);
    JPABaseEmitter* createSimpleEmitterID(JGeometry::TVec3< f32 > const&, u16, u8, u8, JPAEmitterCallBack*, JPAParticleCallBack*);
    void calc(u8);
    void draw(JPADrawInfo const*, u8);
    void forceDeleteAllEmitter();
    void forceDeleteGroupEmitter(u8);
    void forceDeleteEmitter(JPABaseEmitter*);
    void entryResourceManager(JPAResourceManager*, u8);
    void clearResourceManager(u8);
    void calcYBBCam();
    JPAResourceManager* getResourceManager(u16 idx) const { return pResMgrAry[idx]; }
    JPAResourceManager* getResourceManager(u8 res_mgr_id) const { return pResMgrAry[res_mgr_id]; }
    int getEmitterNumber() const { return emtrNum - mFreeEmtrList.getNumLinks(); }

public:
    /* 0x00 */ JSUList< JPABaseEmitter >* pEmtrUseList;
    /* 0x04 */ JSUList< JPABaseEmitter > mFreeEmtrList;
    /* 0x10 */ JPAList< JPABaseParticle > mPtclPool;
    /* 0x1C */ JPAResourceManager** pResMgrAry;
    /* 0x20 */ JPAEmitterWorkData* pWd;
    /* 0x24 */ u32 emtrNum;
    /* 0x28 */ u32 ptclNum;
    /* 0x2C */ u8 gidMax;
    /* 0x2D */ u8 ridMax;
};
