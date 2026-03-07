#include "Game/Boss/BossStinkBugActionBase.hpp"

class BossStinkBugActionFlyHigh : public BossStinkBugActionBase {
public:
    BossStinkBugActionFlyHigh(BossStinkBug* pStinkBug);

    virtual void appear();
    virtual void control();
    virtual bool isValidFollowId(s32 id) const;
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool isAngry() const {
        return true;
    };

    void exeFlyDash();
    void exeFly();
    void exeShakeOffSign();
    void exeShakeOff();
    void exeShakeOffWait();
    void exeShakeOffEnd();
    void exeRollSign();
    void exeRoll();
    void exeRollWait();
    void exeRollEnd();
    void exeTurnSign();
    void exeTurn();
    void exeFall();
    void exeToGround();
    void exeLand();
    void exeGroundRun();
    void exeToFly();
    void exeDamage();
    bool isEnableHipDrop() const;
    bool isEnableAttack() const;

    f32 _64;
    s32 _68;
    bool _6C;
    s32 _70;
};
