#pragma once

#include "Game/LiveActor/Spine.hpp"

/// @brief Used for executing states of a LiveActor.
class Nerve {
public:
    /// @brief Executes a state based on the host actor.
    /// @param pSpine The spine that contains the host LiveActor to execute the state for.
    virtual void execute(Spine* pSpine) const = 0;
    /// @brief Executes after the last iteration of a state before it executes another state.
    /// @param pSpine The spine that contains the host LiveActor to execute the state for.
    virtual void executeOnEnd(Spine* pSpine) const;
};

/* Defines a basic nerve class */
#define NERVE(name)                                                                                                                                  \
    class name : public Nerve {                                                                                                                      \
    public:                                                                                                                                          \
        name() NO_INLINE {};                                                                                                                         \
        virtual void execute(Spine*) const;                                                                                                          \
        static name sInstance;                                                                                                                       \
    };

/* Defines a basic nerve that overrides the executeOnEnd function */
#define NERVE_EXECEND(name)                                                                                                                          \
    class name : public Nerve {                                                                                                                      \
    public:                                                                                                                                          \
        name() NO_INLINE {};                                                                                                                         \
        virtual void execute(Spine*) const;                                                                                                          \
        virtual void executeOnEnd(Spine*) const;                                                                                                     \
        static name sInstance;                                                                                                                       \
    };

/* Initializes the static instance of a nerve */
#define INIT_NERVE(name) name name::sInstance;

/* Initalizes the static instance of a nerve and also defines the body of said nerve's execute function */
#define INIT_NERVE_NEW(name, parent_class, func)                                                                                                     \
    name name::sInstance;                                                                                                                            \
    void name::execute(Spine* pSpine) const {                                                                                                        \
        parent_class* actor = reinterpret_cast< parent_class* >(pSpine->mExecutor);                                                                  \
        actor->func();                                                                                                                               \
    }

/* Declares a nerve and also defines the body of the nerve's execution function, which calls a specified member function */
#define NERVE_DECL(name, parent_class, func)                                                                                                         \
    class name : public Nerve {                                                                                                                      \
    public:                                                                                                                                          \
        name() NO_INLINE {};                                                                                                                         \
        virtual void execute(Spine* pSpine) const {                                                                                                  \
            parent_class* actor = reinterpret_cast< parent_class* >(pSpine->mExecutor);                                                              \
            actor->func();                                                                                                                           \
        };                                                                                                                                           \
        static name sInstance;                                                                                                                       \
    };

/* Declares a nerve and also defines the body of the nerve's execution function, which calls a specified member function */
#define NERVE_DECL_EXE(name, parent_class, executor_name)                                                                                            \
    class name : public Nerve {                                                                                                                      \
    public:                                                                                                                                          \
        name() NO_INLINE {};                                                                                                                         \
        virtual void execute(Spine* pSpine) const {                                                                                                  \
            parent_class* actor = reinterpret_cast< parent_class* >(pSpine->mExecutor);                                                              \
            actor->exe##executor_name();                                                                                                             \
        };                                                                                                                                           \
        static name sInstance;                                                                                                                       \
    };

/* Declares a nerve and also defines the body of the nerve's execution and executeOnEndfunctions, which calls a specified member function */
#define NERVE_DECL_ONEND(name, parent_class, func, onEndFunc)                                                                                        \
    class name : public Nerve {                                                                                                                      \
    public:                                                                                                                                          \
        name() NO_INLINE {};                                                                                                                         \
        virtual void execute(Spine* pSpine) const {                                                                                                  \
            parent_class* actor = reinterpret_cast< parent_class* >(pSpine->mExecutor);                                                              \
            actor->func();                                                                                                                           \
        };                                                                                                                                           \
        virtual void executeOnEnd(Spine* pSpine) const {                                                                                             \
            parent_class* actor = reinterpret_cast< parent_class* >(pSpine->mExecutor);                                                              \
            actor->onEndFunc();                                                                                                                      \
        };                                                                                                                                           \
        static name sInstance;                                                                                                                       \
    };

/* Declares a nerve and also defines the nerve's execute function with an empty body */
#define NERVE_DECL_NULL(name)                                                                                                                        \
    class name : public Nerve {                                                                                                                      \
    public:                                                                                                                                          \
        name() NO_INLINE {}                                                                                                                          \
        virtual void execute(Spine* pSpine) const {}                                                                                                 \
        inline static name* get() {                                                                                                                  \
            return &sInstance;                                                                                                                       \
        }                                                                                                                                            \
        static name sInstance;                                                                                                                       \
    };

#define NEW_NERVE(name, parent_class, executor_name)                                                                                                 \
    class name : public Nerve {                                                                                                                      \
    public:                                                                                                                                          \
        virtual void execute(Spine* pSpine) const {                                                                                                  \
            parent_class* actor = reinterpret_cast< parent_class* >(pSpine->mExecutor);                                                              \
            actor->exe##executor_name();                                                                                                             \
        };                                                                                                                                           \
        static name sInstance;                                                                                                                       \
    };                                                                                                                                               \
    name name::sInstance;

#define NEW_NERVE_ONEND(name, parent_class, executor_name, executorOnEnd_name)                                                                       \
    class name : public Nerve {                                                                                                                      \
    public:                                                                                                                                          \
        virtual void execute(Spine* pSpine) const {                                                                                                  \
            parent_class* actor = reinterpret_cast< parent_class* >(pSpine->mExecutor);                                                              \
            actor->exe##executor_name();                                                                                                             \
        };                                                                                                                                           \
        virtual void executeOnEnd(Spine* pSpine) const {                                                                                             \
            parent_class* actor = reinterpret_cast< parent_class* >(pSpine->mExecutor);                                                              \
            actor->end##executorOnEnd_name();                                                                                                        \
        };                                                                                                                                           \
        static name sInstance;                                                                                                                       \
    };                                                                                                                                               \
    name name::sInstance;\
