#include "Game/LiveActor/LiveActor.hpp"

class CocoNutBall : public LiveActor {
public:
    CocoNutBall(const char* pName);

    virtual ~CocoNutBall();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void appearAndThrow(const TVec3f&, f32);
    void hitBackToPlayer();
    void demoBreak(const TVec3f&);
    HitSensor* isBindedAny() const;
    bool isValidReceivePunch() const;
    bool isNerveTrowToOrFreeze() const; // inline used several places
    bool isSensorBody(HitSensor*) const;
    void calcHitBackVelocitAndGravity();
    bool isHitBackRight() const;
    bool isHitBackFront() const;
    void calcHitBackDstPos(TVec3f*, bool, bool);
    bool tryToKill(bool);
    void setVelocityToPlayer(f32, f32);
    void freeze();
    bool isFreezable();
    void processApproachToPlayer();
    void exeThrow();
    void exeHitBackToHost();
    void exeHitBackToPlayer();
    void exeRebound();
    void exeFreeze();
    void exeFreezeRelease();

    LiveActor* _8C;
    TVec3f _90;
    s32 _9C;
    s32 _A0;
    TVec3f _A4;
    TVec3f _B0;
    bool _BC;
    bool _BD;
    bool _BE;
    f32 _C0;
    f32 _C4;
    TVec3f _C8;
    f32 _D4;
    bool _D8;
};
