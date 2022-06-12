#pragma once

class Spine;

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
    name name::sInstance;