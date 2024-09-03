#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/Util/MathUtil.hpp"

class Pole : public LiveActor {
    public:
        Pole(const char *);

        virtual void init(const JMapInfoIter &);
        virtual void appear();        
        virtual void calcAnim();
        virtual void attackSensor(HitSensor *, HitSensor *);
        virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
        virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);
        virtual MtxPtr getBaseMtx() const;        

        void exeDemoAppear();
        void exeFreeInvalid();
        void exeBindStart();
        void exeBindWait();
        void exeBindTurn();
        void exeBindClimbUp();
        void exeBindFallDown();
        void exeBindHandstandStart();
        void exeBindHandstandWait();
        void exeBindHandstandEnd();
        void exeBindHandstandTurn();
        bool tryJump(bool, f32);
        bool tryTurn();
        bool tryClimbUp();
        bool tryFallDown();
        bool tryHandstandTurn();
        bool isEnableTurn() const;
        void updateBindTrans();
        void calcGravityMtx(TPos3f *);
        void updateTopPos(f32);
        f32 getPoleSubPadStickX() const;
        f32 getPoleSubPadStickY() const;
        inline void exeCalcFoot() {
            MR::calcPerpendicFootToLineInside(&mPosition, *MR::getPlayerCenterPos(), _98, _8C);
        }  
            
        TVec3f _8C;
        TVec3f _98;
        f32 _A4;
        bool _A8;
        bool _A9;
        bool _AA;
        bool _AB;
        bool _AC;
        f32 _B0;
        f32 _B4;
        LiveActor* mActor; // 0xB8
        ActorCameraInfo* mInfo; // 0xBC
        TVec3f _C0;
        TVec3f _CC;
        TVec3f _D8;
        TVec3f _E4;
        TMtx34f _F0;
        MtxPtr _120;
        MtxPtr _124;
        TMtx34f _128;
};

namespace NrvPole {
    NERVE(PoleNrvDemoAppear);
    NERVE(PoleNrvFree);
    NERVE(PoleNrvFreeInvalid);
    NERVE(PoleNrvBindStart);
    NERVE(PoleNrvBindStartFast);
    NERVE(PoleNrvBindWait);        
    NERVE(PoleNrvBindTurnLeft);
    NERVE(PoleNrvBindTurnRight);
    NERVE(PoleNrvBindTurnEnd);
    NERVE(PoleNrvBindClimbUp);
    NERVE(PoleNrvBindFallDown);
    NERVE(PoleNrvBindHandstandStart);
    NERVE(PoleNrvBindHandstandWait);
    NERVE(PoleNrvBindHandstandEnd);
    NERVE(PoleNrvBindHandstandTurn);             
};