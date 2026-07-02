#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;

class WaterPlant : public LiveActor {
public:
    class PlantData {
    public:
        /// @brief Creates a new `PlantData`.
        PlantData();
    };

    /// @brief Creates a new `WaterPlant`.
    /// @param pName A pointer to the null-terminated name of the object.
    WaterPlant(const char* pName);

    virtual ~WaterPlant();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void movement();
    virtual void draw() const;

    void initPlant();
    void initPlantAfterPlacement();
    void drawDirect() const;

    /* 0x8C */ s32 _8C;
    /* 0x90 */ PlantData* _90;
    /* 0x94 */ f32 _94;
    /* 0x98 */ bool _98;
    /* 0x9C */ f32 _9C;
    /* 0xA0 */ f32 mHeight;
    /* 0xA4 */ s32 _A4;
};

class WaterPlantDrawInit : public NameObj {
public:
    /// @brief Creates a new `WaterPlantDrawInit`.
    WaterPlantDrawInit();

    virtual void movement();

    virtual void initDraw() const;

    void loadTex(int);
    void updateSwingPos();

    /* 0x0C */ f32 _C;
    /* 0x10 */ f32* _10;
    /* 0x14 */ JUTTexture* _14;
    /* 0x18 */ JUTTexture* _18;
    /* 0x1C */ JUTTexture* _1C;
    /* 0x20 */ JUTTexture* _20;
};
