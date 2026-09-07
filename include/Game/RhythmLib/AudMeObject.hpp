#pragma once

#include "Game/RhythmLib/AudMeHandles.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JKernel/JKRDisposer.hpp>

class AudMeObject : public AudMeHandles, public JKRDisposer {
public:
    AudMeObject(TVec3f*, u8, JKRHeap*);

    virtual ~AudMeObject();
    virtual void dispose();

    AudMeHandle* startMe(u32);
    AudMeHandle* getLowestPrioHandle(u32);

    /* 0x24 */ TVec3f* mPos;
    /* 0x28 */ bool mIsAllocated;
};
