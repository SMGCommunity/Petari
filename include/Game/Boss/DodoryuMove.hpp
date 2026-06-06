#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class Dodoryu;

namespace DodoryuSub {
    class MoveStateHolder;

    class MoveState {
    public:
        MoveState(MoveStateHolder* pHolder);

        virtual void execute() = 0;

        /* 0x04 */ MoveStateHolder* mHolder;
        /* 0x08 */ Dodoryu* mHost;
        /* 0x0C */ s32 mStep;
    };

    class MoveStateRail : public MoveState {
    public:
        MoveStateRail(MoveStateHolder* pHolder);

        virtual void execute();

        /* 0x10 */ f32 _10;
        /* 0x14 */ f32 _14;
    };

    class MoveStateChase : public MoveState {
    public:
        MoveStateChase(MoveStateHolder* pHolder) : MoveState(pHolder), _10(), _14(), _18(), _1C() {
        }

        virtual void execute();

        /* 0x10 */ f32 _10;
        /* 0x14 */ f32 _14;
        /* 0x18 */ f32 _18;
        /* 0x1C */ f32 _1C;
    };

    class MoveStateDetour : public MoveState {
    public:
        MoveStateDetour(MoveStateHolder* pHolder);

        virtual void execute();

        bool isPlayerOppositeFromWall() const;

        /* 0x10 */ f32 _10;
        /* 0x14 */ f32 _14;
        /* 0x18 */ f32 _18;
        /* 0x1C */ f32 _1C;
    };

    class MoveStateEscape : public MoveState {
    public:
        MoveStateEscape(MoveStateHolder* pHolder);

        virtual void execute();

        /* 0x10 */ s32 _10;
        /* 0x14 */ s32 _14;
        /* 0x18 */ s32 _18;
        /* 0x1C */ f32 _1C;
        /* 0x20 */ f32 _20;
        /* 0x24 */ f32 _24;
        /* 0x28 */ f32 _28;
        /* 0x2C */ TVec3f _2C;
    };

    class MoveStateJumpOut : public MoveState {
    public:
        MoveStateJumpOut(MoveStateHolder* pHolder);

        virtual void execute();

        /* 0x10 */ f32 _10;
        /* 0x14 */ f32 _14;
    };

    class MoveStateNull : public MoveState {
    public:
        MoveStateNull(MoveStateHolder* pHolder);

        virtual void execute();
    };

    class MoveStateHolder {
    public:
        MoveStateHolder(Dodoryu* pHost);

        void execute();
        void shiftRail(f32);
        void shiftChase(f32, f32, f32, f32);
        void shiftEscape(f32, s32, s32, f32, f32);
        void shiftJumpOut(f32, f32);
        void shiftNull();
        void brake();
        void setRailSpeed(f32);

        /* 0x00 */ Dodoryu* mHost;
        /* 0x04 */ MoveState* mNextState;
        /* 0x08 */ MoveState* mCurrentState;
        /* 0x0C */ MoveStateRail* mRailState;
        /* 0x10 */ MoveStateChase* mChaseState;
        /* 0x14 */ MoveStateDetour* mDetourState;
        /* 0x18 */ MoveStateEscape* mEscapeState;
        /* 0x1C */ MoveStateJumpOut* mJumpOutState;
        /* 0x20 */ MoveStateNull* mNullState;
    };
};  // namespace DodoryuSub
