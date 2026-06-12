#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class JAIAudible;
class JAISoundID;
struct JASSoundParams;

class JAIAudience {
public:
    virtual ~JAIAudience();

    // TODO: Should all be pure virtual.
    virtual JAIAudible* newAudible(const TVec3f&, JAISoundID, const TVec3f*, u32);
    virtual int getMaxChannels();
    virtual void deleteAudible(JAIAudible*);
    virtual u32 calcPriority(JAIAudible*);
    virtual void mixChannelOut(const JASSoundParams&, JAIAudible*, int);
};
