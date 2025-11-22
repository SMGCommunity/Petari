#pragma once

#include "Game/NPC/NPCActorItem.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class JMapInfo;
class NPCActorItem;

class NPCParameterEdit {
public:
    NPCParameterEdit(const char*, const char**, bool);

    virtual void read(JMapInfo*, s32);
};

class NPCParameterBool {
public:
    NPCParameterBool(const char*, bool);

    virtual void read(JMapInfo*, s32);
};

class NPCParameterV3f {
public:
    NPCParameterV3f(const char*, TVec3f*, f32, f32);

    virtual void read(JMapInfo*, s32);
};

class NPCParameterJoint {
public:
    NPCParameterJoint(const char*, const char*, const char**);

    virtual void read(JMapInfo*, s32);
};

class NPCParameterReader {
public:
    NPCParameterReader(const char*);

    virtual void read(JMapInfo*, s32);
};

class NPCCapsParameterReader : public NPCParameterReader {
public:
    NPCCapsParameterReader(const char*);

    void init();
};

class NPCItemParameterReader : public NPCParameterReader {
public:
    NPCItemParameterReader(const char*);

    void copy(const NPCActorItem*);
};
