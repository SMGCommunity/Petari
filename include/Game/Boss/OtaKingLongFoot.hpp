#include "Game/LiveActor/PartsModel.hpp"

class AnimScaleController;

class OtaKingLongFoot : public PartsModel {
public: 
    OtaKingLongFoot(LiveActor*, s32, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void hide();
    void startAppearDemo();
    void startDownDemo();
    void endDemo();
    void initSensor();

    void exeWait();
    void exeDamage();
    void exeAppearDemo();
    void exeDownDemo();

    s32 _9C;
    AnimScaleController* _A0;
};
