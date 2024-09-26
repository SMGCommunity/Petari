#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Effect/SpinPullParticleCallBack.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Enemy/MoguStone.hpp"
#include "Game/Camera/CameraTargetDemoActor.hpp"
#include "Game/LiveActor/Binder.hpp"

class Meramera;

class IceMerameraKing : public LiveActor {
    public:
        IceMerameraKing(const char *);

        virtual ~IceMerameraKing();
        virtual void init(const JMapInfoIter &);
        virtual void initAfterPlacement();
        virtual void kill();
        virtual void control();
        virtual void calcAndSetBaseMtx();
        virtual void attackSensor(HitSensor *, HitSensor *);
        virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
        virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

        void exeSearch();
        void exeThrow();
        void tearDownThrow();
        void exeExtinguish();
        void exeEscape();
        void exeEscapeJump();
        void exeDamage();
        void exePreRecover();
        void exeRecover();
        void exePreAttack();
        void exeAttack();
        void exeAttackAfter();
        void exeAngryDemo();
        void exeDeathDemo();
        void addVelocityToInitPos();
        bool isDeadAllIce();
        bool isEnableThrow();
        ThrowingIce* getDeadWeaponAndAppear();
        bool calcJoint(TPos3f *, const JointControllerInfo &);

        inline void exeDeleteEffect() {
            MR::deleteEffect(this, "Rolling");
        }

        inline void exeDeathDemoWait() {
            if (MR::isFirstStep(this)) {
                MR::stopStageBGM(30);
            }
            mVelocity.zero();
        }

        FixedPosition* mFixedPos; //_8C
        ThrowingIce* _90; 
        Meramera* _94; 
        SpinPullParticleCallBack* mSpinParticle; //_98
        ThrowingIce** mActor; //_9C
        s32 _A0;
        s32 _A4; 
        LiveActor* _A8; 
        LiveActor* _AC;
        TVec3f _B0; 
        TVec3f _BC; 
        TVec3f _C8;
        TVec3f _D4; 
        s32 _E0; 
        s32 _E4; 
        s32 _E8; 
        s32 _EC; 
        s32 _F0;
        MR::Vector<MR::AssignableArray<ThrowingIce*> > mModelArray; //_F4
        TQuat4f _100; 
        TVec3f _110; 
        f32 _11C; 
        bool _120;
        bool _121;
        CameraTargetDemoActor* _F8;
        JointControlDelegator<IceMerameraKing>* mJointController; //_FC
};

class IceMerameraKingShockWave : public ModelObj {
    public:
        IceMerameraKingShockWave();

        virtual ~IceMerameraKingShockWave();
        virtual void appear();
        virtual void control();
        virtual void attackSensor(HitSensor *, HitSensor *);
};

namespace NrvIceMerameraKing {
    NERVE(HostTypeNrvSearch);
    NERVE_EXECEND(HostTypeNrvBeginDemo);
    NERVE_EXECEND(HostTypeNrvThrow);
    NERVE_EXECEND(HostTypeNrvThrow2nd);
    NERVE(HostTypeNrvExtinguish);
    NERVE_EXECEND(HostTypeNrvEscape);        
    NERVE(HostTypeNrvEscapeJump);
    NERVE(HostTypeNrvDamage);
    NERVE(HostTypeNrvPreRecover);
    NERVE(HostTypeNrvRecover);
    NERVE(HostTypeNrvPreAttack);
    NERVE(HostTypeNrvAttack);
    NERVE(HostTypeNrvAttackAfter);
    NERVE(HostTypeNrvAngryDemo1st);
    NERVE(HostTypeNrvAngryDemo2nd);     
    NERVE(HostTypeNrvDeathDemoWait);          
    NERVE(HostTypeNrvDeathDemo);                
};