#pragma once

class MarioActor;
class MarioModule;

class MarioAnimator : public MarioModule {
public:
	MarioAnimator(MarioActor *);

    virtual void init();
    virtual bool proc(u32);
    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual bool keep();
    virtual bool postureCtrl(MtxPtr);
    virtual void hitWall(const TVec3f &, HitSensor *);
    virtual void hitPoly(u8, const TVec3f &, HitSensor *);
    virtual bool passRing(const HitSensor *);
    virtual f32 getBlurOffset() const;
    virtual void draw3D() const;

    void setHoming();
    bool isAnimationStop();

	XanimeResourceTable* _8;
	XanimePlayer* _c;
	XanimePlayer* _10;
};
