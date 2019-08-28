#ifndef AUDSOUNDOBJECT_H
#define AUDSOUNDOBJECT_H

#include "Actor/Sound/JAUSoundObject.h"

class AudSoundObject : public JAUSoundObject
{
public:
    u8 _18[0x48-0x18];

    void addToSoundObjHolder();
};

#endif // AUDSOUNDOBJECT_H