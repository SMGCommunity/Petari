#pragma once

#include <revolution/types.h>

class Dodoryu;

namespace DodoryuSub {
    class MoveStateHolder;

    class MoveState {
    public:
        MoveState(MoveStateHolder* pHolder);

        virtual void execute() = 0;

        /* 0x04 */ MoveStateHolder* mHolder;
        /* 0x08 */ Dodoryu* mHost;
        /* 0x0C */ u32 _C;
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
        MoveStateChase(MoveStateHolder* pHolder);

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

        /* 0x10 */ u32 _10;
        /* 0x14 */ s32 _14;
        /* 0x18 */ s32 _18;
        /* 0x1C */ f32 _1C;
        /* 0x20 */ f32 _20;
        /* 0x24 */ f32 _24;
        /* 0x28 */ f32 _28;
        /* 0x2C */ f32 _2C;
        /* 0x20 */ f32 _30;
        /* 0x34 */ f32 _34;
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
        /* 0x04 */ MoveState* _4;
        /* 0x08 */ MoveState* _8;
        /* 0x0C */ MoveStateRail* mRailState;
        /* 0x10 */ MoveStateChase* mChaseState;
        /* 0x14 */ MoveStateDetour* mDetourState;
        /* 0x18 */ MoveStateEscape* mEscapeState;
        /* 0x1C */ MoveStateJumpOut* mJumpOutState;
        /* 0x20 */ MoveStateNull* mNullState;
    };
};  // namespace DodoryuSub
