#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <revolution/types.h>

class JPAEmitterManager;
class JPAResource;

enum JPAEmitterFlag { JPA_EMITTER_PAUSE_FLAG = 1 << 1, JPA_EMITTER_INIT_FLAG = 1 << 6 };

class JPABaseEmitter {
public:
    /// @brief Creates a new `JPABaseEmitter`.
    JPABaseEmitter();

    /// @brief Destorys the `JPABaseEmitter`.
    ~JPABaseEmitter();

    /* 0x000 */ TVec3f _0;
    /* 0x00C */ TVec3f _C;
    /* 0x018 */ TVec3f _18;
    /* 0x024 */ s32 _24;
    /* 0x028 */ f32 _28;
    /* 0x02C */ f32 _2C;
    /* 0x030 */ f32 _30;
    /* 0x034 */ f32 _34;
    /* 0x038 */ f32 _38;
    /* 0x03C */ f32 _3C;
    /* 0x040 */ f32 _40;
    /* 0x044 */ f32 _44;
    /* 0x048 */ f32 _48;
    /* 0x04C */ u16 _4C;
    /* 0x04E */ u16 _4E;
    /* 0x050 */ u16 _50;
    /* 0x052 */ u16 _52;
    /* 0x054 */ u16 _54;
    /* 0x056 */ u8 _56;
    /* 0x058 */ JSUPtrLink _58;
    /* 0x068 */ Mtx _68;
    /* 0x098 */ TVec3f _98;
    /* 0x0A4 */ TVec3f _A4;
    /* 0x0B0 */ TVec2f mGlobalScale;
    /* 0x0B8 */ GXColor mGlobalPrmColor;
    /* 0x0BC */ GXColor mGlobalEnvColor;
    /* 0x0C0 */ void* _C0;
    /* 0x0C4 */ u32 _C4;
    /* 0x0C8 */ void* _C8;  // Part of JPAList<JPABaseParticle>
    /* 0x0CC */ void* _CC;  // Part of JPAList<JPABaseParticle>
    /* 0x0D0 */ u32 _D0;    // Part of JPAList<JPABaseParticle>
    /* 0x0D4 */ void* _D4;  // Part of JPAList<JPABaseParticle>
    /* 0x0D8 */ void* _D8;  // Part of JPAList<JPABaseParticle>
    /* 0x0DC */ u32 _DC;    // Part of JPAList<JPABaseParticle>
    /* 0x0E0 */ void* _E0;  // JPAList<JPABaseParticle>*
    /* 0x0E4 */ JPAEmitterManager* mManager;
    /* 0x0E8 */ JPAResource* mResource;
    /* 0x0EC */ void* _EC;
    /* 0x0F0 */ void* _F0;
    /* 0x0F4 */ u32 mFlag;
    /* 0x0F8 */ f32 _F8;
    /* 0x0FC */ f32 _FC;
    /* 0x100 */ u32 _100;
    /* 0x104 */ s16 _104;
    /* 0x108 */ GXColor _108;
    /* 0x10C */ GXColor _10C;
    /* 0x110 */ u8 _110;
    /* 0x111 */ u8 _111;
};
