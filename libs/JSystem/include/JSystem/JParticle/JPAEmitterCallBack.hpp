#pragma once

class JPABaseEmitter;

class JPAEmitterCallBack {
public:
    virtual ~JPAEmitterCallBack() = 0;
    virtual void execute(JPABaseEmitter*) {}
    virtual void executeAfter(JPABaseEmitter*) {}
    virtual void draw(JPABaseEmitter*) {}
    virtual void drawAfter(JPABaseEmitter*) {}
    virtual void init(JPABaseEmitter*) {}
};
