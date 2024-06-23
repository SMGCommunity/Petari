#pragma once

class JPABaseEmitter;
class JPABaseParticle;

class JPAEmitterCallBack {
    virtual ~JPAEmitterCallBack();
    virtual void execute(JPABaseEmitter *);
    virtual void executeAfter(JPABaseEmitter *);
    virtual void draw(JPABaseEmitter *);
    virtual void drawAfter(JPABaseEmitter *);
    virtual void init(JPABaseEmitter *) = 0;
};

class JPAParticleCallBack {
public:
    virtual ~JPAParticleCallBack();
    virtual void execute(JPABaseEmitter *, JPABaseParticle *) {

    }

    virtual void draw(JPABaseEmitter *, JPABaseParticle *) {

    }
};