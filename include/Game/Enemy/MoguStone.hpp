#pragma once

#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ParabolicPath.hpp"

namespace {
    static f32 hAddHeight = 200.0f;   
};

class MoguStone : public ModelObj {
    public:
        MoguStone(const char *, const char *);

        virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
        //incomplete class

        TVec4f _90;
        TVec4f _A0;
        TVec3f _B0;
        f32 _BC;
        bool _C0;
};

class ThrowingIce : public MoguStone {
    public:
        ThrowingIce(const char *);

        virtual void init(const JMapInfoIter &);
        virtual void doBehavior();
        virtual void attackSensor(HitSensor *, HitSensor *);
        virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
        virtual void startBreakSound();
        virtual void startThrowLevelSound();

        void emitIce(const TVec3f &, const TVec3f &, f32, const TVec3f &);

        ParabolicPath* mPath; //_C4
        
        
};

namespace NrvMoguStone {      
    NERVE(MoguStoneNrvTaken);     
    NERVE(MoguStoneNrvThrow);            
    //incomplete
};
