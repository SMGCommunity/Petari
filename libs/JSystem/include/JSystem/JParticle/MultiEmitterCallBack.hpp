#pragma once

class JPABaseEmitter;

class JPAEmitterCallBack {
    virtual ~JPAEmitterCallBack();
    virtual void execute(JPABaseEmitter *);
    virtual void executeAfter(JPABaseEmitter *);
    virtual void draw(JPABaseEmitter *);
    virtual void drawAfter(JPABaseEmitter *);
    virtual void init(JPABaseEmitter *) = 0;
};

class MultiEmitterCallBackBase : public JPAEmitterCallBack {
    virtual ~MultiEmitterCallBackBase();
    virtual void init(JPABaseEmitter *);
};
