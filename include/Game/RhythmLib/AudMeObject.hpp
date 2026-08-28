#pragma once

#include "Game/RhythmLib/AudMeHandles.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JKernel/JKRDisposer.hpp>

class AudMe;

class AudMeHandle {
public:
    /// @brief Creates a new `AudMeHandle`.
    AudMeHandle();

    /// @brief Destroys the `AudMeHandle`.
    ~AudMeHandle();

    void releaseMe();

private:
    /* 0x00 */ AudMe* mMe;
};

class AudMeObject : public AudMeHandles, public JKRDisposer {
public:
    AudMeObject(TVec3f*, u8, JKRHeap*);

    /* 0x08 */ virtual ~AudMeObject();
    /* 0x0C */ virtual void dispose();

    void startMe(u32);
    AudMeHandle* getLowestPrioHandle(u32);
};
