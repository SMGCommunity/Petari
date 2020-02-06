#ifndef GHOSTPACKET_H
#define GHOSTPACKET_H

#include "types.h"
#include "JGeometry/TVec3.h"

class GhostPacket
{
public:
    GhostPacket(void *, u32);

    void read(u8 *, u32);
    void read(u32 *);
    void read(s16 *);
    void read(char **);
    void read(s8 *);
    void read(JGeometry::TVec3<s8> *);
    void read(JGeometry::TVec3<s16> *);

    u32 _0;
    void* mSource; // _4
    u32 mCurPos; // _8
    u32 _C;
};

#endif // GHOSTPACKET_H