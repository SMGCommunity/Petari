#ifndef NERVE_H
#define NERVE_H

class Spine;

class Nerve
{
public:
    virtual void execute(Spine *) const = 0;
    virtual void executeOnEnd(Spine *) const;
};

#endif // NERVE_H