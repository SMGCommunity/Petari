#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/Util/Array.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class JMapInfo;
class NPCActorItem;

class NPCParameterBase {
public:
    NPCParameterBase(const char* pName) { mName = pName; };

    const char* mName;

    virtual void read(JMapInfo*, s32) {};
};

class NPCParameterEdit : public NPCParameterBase {
public:
    NPCParameterEdit(const char*, const char**, bool);

    virtual void read(JMapInfo*, s32);

    const char** _8;
    bool _C;
};

class NPCParameterBool : public NPCParameterBase {
public:
    NPCParameterBool(const char*, bool*);

    virtual void read(JMapInfo*, s32);

    bool* _8;
};

class NPCParameterV3f : public NPCParameterBase {
public:
    NPCParameterV3f(const char*, TVec3f*, f32, f32);

    virtual void read(JMapInfo*, s32);

    TVec3f* _8;
    f32 _C;
    f32 _10;
};

class NPCParameterJoint : public NPCParameterBase {
public:
    NPCParameterJoint(const char*, const char*, const char**);

    virtual void read(JMapInfo*, s32);

    const char** _8;
    const char* _C;
    s32 _10;
};

class NPCParameterReader : public NPCParameterBase {
public:
    NPCParameterReader(const char*);

    virtual void read(JMapInfo*, s32);

    MR::Vector< MR::FixedArray< NPCParameterBase*, 48 > > mVector;
};

class NPCCapsParameterReader : public NPCParameterReader {
public:
    NPCCapsParameterReader(const char*);

    void init();

    NPCActorCaps mActorCaps;
};

class NPCItemParameterReader : public NPCParameterReader {
public:
    NPCItemParameterReader(const char*);

    void copy(const NPCActorItem*);
    NPCActorItem mItem;
};

template < typename T >
class NPCParameterRange : public NPCParameterBase {
public:
    NPCParameterRange< T >(const char*, T*, T, T);

    virtual void read(JMapInfo*, s32);

    T* _8;
    T _C;
    T _10;
};
