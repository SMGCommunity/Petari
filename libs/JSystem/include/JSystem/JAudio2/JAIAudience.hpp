#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class JAIAudible;
class JAISoundID;
struct JASSoundParams;

class JAIAudience {
public:
    virtual ~JAIAudience();

    // TODO: Should all be pure virtual.
    virtual JAIAudible* newAudible(const TVec3f&, JAISoundID, const TVec3f*, u32) = 0;
    virtual int getMaxChannels() = 0;
    virtual void deleteAudible(JAIAudible*) = 0;
    virtual u32 calcPriority(JAIAudible*) = 0;
    virtual void mixChannelOut(const JASSoundParams&, JAIAudible*, int) = 0;
};
