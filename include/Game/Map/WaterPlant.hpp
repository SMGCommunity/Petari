#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/PlayerUtil.hpp"

class JUTTexture;

class WaterPlant : public LiveActor {
public:
    class PlantData {
    public:
        /// @brief Creates a new `PlantData`.
        PlantData();

        /* 0x00 */ TVec3f mPosition;
        /* 0x0C */ TVec3f mRotation;
        /* 0x18 */ TVec3f mSpinSwingDir;
        /* 0x24 */ TVec3f mTempSpinSwingDir;
        /* 0x30 */ f32 mSpinSwingStrength;
        /* 0x34 */ s32 mSpinWaveSwingTimer;
        /* 0x38 */ s32 mSpinWaveTimer;
        /* 0x3C */ TVec3f mSpinWaveSwingVec1;
        /* 0x48 */ TVec3f mSpinWaveSwingVec2;
        /* 0x54 */ TVec3f mSpinWaveSwingVec3;
        /* 0x60 */ f32 mHeight1;
        /* 0x64 */ f32 mHeight2;
        /* 0x68 */ f32 mHeight3;
        /* 0x6C */u8 pad_6C;
        /* 0x70 */ f32* mSwingStrength1;
        /* 0x74 */ f32* mSwingStrength2;
        /* 0x78 */ f32* mSwingStrength3;

        bool newSpinWaveTimerIsGreater(s32 newTimer) {
            return !(newTimer < mSpinWaveTimer);
        }
    };

    /// @brief Creates a new `WaterPlant`.
    /// @param pName A pointer to the null-terminated name of the object.
    WaterPlant(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void movement();
    virtual void draw() const;

    void initPlant();
    void initPlantAfterPlacement();
    void drawDirect() const;

    PlantData* getData(int i) {
        return &mPlantDataArray[i];
    }

    /* 0x8C */ s32 mPlantNum;
    /* 0x90 */ PlantData* mPlantDataArray;
    /* 0x94 */ f32 mAreaRadius;
    /* 0x98 */ bool mIsSpin;
    /* 0x9C */ f32 mHalfWidth;
    /* 0xA0 */ f32 mHeight;
    /* 0xA4 */ s32 mType;
};

class WaterPlantDrawInit : public NameObj {
public:
    /// @brief Creates a new `WaterPlantDrawInit`.
    WaterPlantDrawInit();

    virtual void movement();

    virtual void initDraw() const;

    void loadTex(int);
    void updateSwingPos();

    /* 0x0C */ f32 mAngleOffset;
    /* 0x10 */ f32* mSwingPosTable;
    /* 0x14 */ JUTTexture* mPlantATex;
    /* 0x18 */ JUTTexture* mPlantBTex;
    /* 0x1C */ JUTTexture* mPlantCTex;
    /* 0x20 */ JUTTexture* mPlantDTex;
};
