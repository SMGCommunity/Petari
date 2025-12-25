#pragma once

#include <revolution/types.h>

class CodeTable;
class JMapInfoIter;
class Triangle;

enum CollisionFloorCode {
    CollisionFloorCode_Normal,
    CollisionFloorCode_Death,
    CollisionFloorCode_Slip,
    CollisionFloorCode_NoSlip,
    CollisionFloorCode_DamageNormal,
    CollisionFloorCode_Ice,
    CollisionFloorCode_JumpLow,
    CollisionFloorCode_JumpMiddle,
    CollisionFloorCode_JumpHigh,
    CollisionFloorCode_Slider,
    CollisionFloorCode_DamageFire,
    CollisionFloorCode_JumpNormal,
    CollisionFloorCode_FireDance,
    CollisionFloorCode_Sand,
    CollisionFloorCode_Glass,
    CollisionFloorCode_DamageElectric,
    CollisionFloorCode_PullBack,
    CollisionFloorCode_Sink,
    CollisionFloorCode_SinkPoison,
    CollisionFloorCode_Slide,
    CollisionFloorCode_WaterBottomH,
    CollisionFloorCode_WaterBottomM,
    CollisionFloorCode_WaterBottomL,
    CollisionFloorCode_Wet,
    CollisionFloorCode_Needle,
    CollisionFloorCode_SinkDeath,
    CollisionFloorCode_Snow,
    CollisionFloorCode_RailMove,
    CollisionFloorCode_AreaMove,
    CollisionFloorCode_Press,
    CollisionFloorCode_NoStampSand,
    CollisionFloorCode_SinkDeathMud,
    CollisionFloorCode_Brake,
    CollisionFloorCode_GlassIce,
    CollisionFloorCode_JumpParasol,

    CollisionFloorCode_End,
};

enum CollisionWallCode {
    CollisionWallCode_Normal,
    CollisionWallCode_NotWallJump,
    CollisionWallCode_NotWallSlip,
    CollisionWallCode_NotGrab,
    CollisionWallCode_GhostThroughCode,
    CollisionWallCode_NotSideStep,
    CollisionWallCode_Rebound,
    CollisionWallCode_Fur,
    CollisionWallCode_NoAction,

    CollisionWallCode_End,
};

enum CollisionSoundCode {
    CollisionSoundCode_Null,
    CollisionSoundCode_Soil,
    CollisionSoundCode_Lawn,
    CollisionSoundCode_Stone,
    CollisionSoundCode_Marble,
    CollisionSoundCode_WoodThick,
    CollisionSoundCode_WoodThin,
    CollisionSoundCode_Metal,
    CollisionSoundCode_Snow,
    CollisionSoundCode_Ice,
    CollisionSoundCode_Shallow,
    CollisionSoundCode_Sand,
    CollisionSoundCode_Beach,
    CollisionSoundCode_Carpet,
    CollisionSoundCode_Mud,
    CollisionSoundCode_Honey,

    CollisionSoundCode_End,
};

enum CollisionCameraCode {
    CollisionCameraCode_NoThrough,
    CollisionCameraCode_Through,

    CollisionCameraCode_End,
};

class CollisionCode {
public:
    /// @brief Creates a new `CollisionCode`.
    CollisionCode();

    const char* getFloorCodeString(const Triangle&);
    const char* getWallCodeString(const Triangle&);
    const char* getSoundCodeString(const Triangle&);
    const char* getSoundCodeString(const JMapInfoIter&);
    u32 getCameraID(const Triangle&);
    u32 getCode(const JMapInfoIter&, CodeTable*, const char*);
    void createFloorTable();
    void createWallTable();
    void createSoundTable();
    void createCameraTable();
    u32 getFloorCode(const JMapInfoIter&);
    u32 getWallCode(const JMapInfoIter&);
    u32 getSoundCode(const JMapInfoIter&);
    u32 getCameraCode(const JMapInfoIter&);
    u32 getCameraID(const JMapInfoIter&);

    /* 0x00 */ CodeTable* mFloorTable;
    /* 0x04 */ CodeTable* mWallTable;
    /* 0x08 */ CodeTable* mSoundTable;
    /* 0x0C */ CodeTable* mCameraTable;
};

class CodeTable {
public:
    CodeTable(u32);

    void add(const char*, u32);
    const char* getString(u32);

    /* 0x00 */ u32 mCodeNum;
    /* 0x04 */ u32 mCodeNumMax;
    /* 0x08 */ u32* mHashTable;
    /* 0x0C */ u32* mCodeTable;
    /* 0x10 */ const char** mNameTable;
};
