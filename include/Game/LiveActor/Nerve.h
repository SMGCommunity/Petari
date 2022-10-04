#pragma once

#include "Game/LiveActor/Spine.h"

class Nerve {
public:
    virtual void execute(Spine *) const = 0;
    virtual void executeOnEnd(Spine *) const;
};

#define NERVE(name)\
class name : public Nerve\
{\
public:\
    name() NO_INLINE {\
    };\
    virtual void execute(Spine *) const;\
    static name sInstance;\
};\

#define NERVE_EXECEND(name)\
class name : public Nerve\
{\
public:\
    name() NO_INLINE {\
    };\
    virtual void execute(Spine *) const;\
    virtual void executeOnEnd(Spine *) const;\
    static name sInstance;\
};\

#define INIT_NERVE(name)\
    name name::sInstance;\

#define INIT_NERVE_NEW(name, parent_class, func)\
    name name::sInstance;\
    void name::execute(Spine *pSpine) const { \
        parent_class* actor = reinterpret_cast<parent_class*>(pSpine->mExecutor);\
        actor->func();\
    }\

#define NERVE_DECL(name, parent_class, func)\
class name : public Nerve\
{\
public:\
    name() NO_INLINE {\
    };\
    virtual void execute(Spine *pSpine) const {\
        parent_class* actor = reinterpret_cast<parent_class*>(pSpine->mExecutor);\
        actor->func();\
    };\
    static name sInstance;\
};\

#define NERVE_DECL_NULL(name)\
class name : public Nerve\
{\
public:\
    name() NO_INLINE {\
    };\
    virtual void execute(Spine *pSpine) const {\
    };\
    static name sInstance;\
};\
