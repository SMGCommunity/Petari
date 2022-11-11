#pragma once

#include "Game/LiveActor/Spine.h"

/// @brief Used for executing states of a LiveActor.
class Nerve {
public:
    /// @brief Executes a state based on the host actor.
    /// @param pSpine The spine that contains the host LiveActor to execute the state for.
    virtual void execute(Spine *pSpine) const = 0;
    /// @brief Executes after the last iteration of a state before it executes another state.
    /// @param pSpine The spine that contains the host LiveActor to execute the state for.
    virtual void executeOnEnd(Spine *pSpine) const;
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

#define NERVE_DECL_ONEND(name, parent_class, func, onEndFunc)\
class name : public Nerve\
{\
public:\
    name() NO_INLINE {\
    };\
    virtual void execute(Spine *pSpine) const {\
        parent_class* actor = reinterpret_cast<parent_class*>(pSpine->mExecutor);\
        actor->func();\
    };\
    virtual void executeOnEnd(Spine *pSpine) const {\
        parent_class* actor = reinterpret_cast<parent_class*>(pSpine->mExecutor);\
        actor->onEndFunc();\
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
