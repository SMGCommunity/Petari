#pragma once

#include "Inline.hpp"
#include "JSystem/JParticle/JPATexture.hpp"
#include <revolution.h>

class JKRHeap;
class JPAResource;
struct ResTIMG;

class JPAResourceManager {
public:
    JPAResourceManager(void const*, JKRHeap*);
    JPAResource* getResource(u16) const;
    bool checkUserIndexDuplication(u16) const;
    const ResTIMG* swapTexture(ResTIMG const*, char const*);
    void registRes(JPAResource*);
    void registTex(JPATexture*);
    u32 getResUserWork(u16) const;

    void load(u16 idx, GXTexMapID texMapID) NO_INLINE {
        CALL_INLINE_FUNC(load, idx, texMapID);
    }

    inline void INLINE_FUNC_DECL(load, u16 idx, GXTexMapID texMapID) {
        mpTexArr[idx]->load(texMapID);
    }

public:
    /* 0x00 */ JKRHeap* mpHeap;
    /* 0x04 */ JPAResource** mpResArr;
    /* 0x08 */ JPATexture** mpTexArr;
    /* 0x0C */ u16 mResMax;
    /* 0x0E */ u16 mResNum;
    /* 0x10 */ u16 mTexMax;
    /* 0x12 */ u16 mTexNum;
};
