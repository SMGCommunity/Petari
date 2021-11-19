#pragma once

class Spine;

class Nerve {
public:
    virtual void execute(Spine *) const = 0;
    virtual void executeOnEnd(Spine *) const;
};