#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class ProjmapEffectMtxSetter;

class FileSelectSky : public LiveActor {
public:
    /// @brief Creates a new `FileSelectSky`.
    /// @param pName A pointer to the null-terminated name of the object.
    FileSelectSky(const char* pName);

    /// @brief Destroys the `FileSelectSky`.
    /* 0x08 */ virtual ~FileSelectSky();

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x1C */ virtual void calcAnim();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();

private:
    /* 0x8C */ f32 mAngleX;
    /* 0x90 */ f32 mAngleY;
    /* 0x94 */ TPos3f mBaseMtx;
    /* 0xC4 */ ProjmapEffectMtxSetter* mProjmapEffectMtxSetter;
};
