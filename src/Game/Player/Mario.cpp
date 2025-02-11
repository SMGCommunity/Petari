#include "Game/Player/Mario.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/MarioAbyssDamage.hpp"
#include "Game/Player/MarioBlown.hpp"
#include "Game/Player/MarioBump.hpp"
#include "Game/Player/MarioBury.hpp"
#include "Game/Player/MarioClimb.hpp"
#include "Game/Player/MarioCrush.hpp"
#include "Game/Player/MarioDamage.hpp"
#include "Game/Player/MarioDarkDamage.hpp"
#include "Game/Player/MarioFaint.hpp"
#include "Game/Player/MarioFireDamage.hpp"
#include "Game/Player/MarioFireDance.hpp"
#include "Game/Player/MarioFireRun.hpp"
#include "Game/Player/MarioFlip.hpp"
#include "Game/Player/MarioFlow.hpp"
#include "Game/Player/MarioFoo.hpp"
#include "Game/Player/MarioFpView.hpp"
#include "Game/Player/MarioFreeze.hpp"
#include "Game/Player/MarioFrontStep.hpp"
#include "Game/Player/MarioHang.hpp"
#include "Game/Player/MarioMagic.hpp"
#include "Game/Player/MarioMapCode.hpp"
#include "Game/Player/MarioMove.hpp"
#include "Game/Player/MarioParalyze.hpp"
#include "Game/Player/MarioRabbit.hpp"
#include "Game/Player/MarioRecovery.hpp"
#include "Game/Player/MarioSideStep.hpp"
#include "Game/Player/MarioSkate.hpp"
#include "Game/Player/MarioSlider.hpp"
#include "Game/Player/MarioStep.hpp"
#include "Game/Player/MarioStick.hpp"
#include "Game/Player/MarioStun.hpp"
#include "Game/Player/MarioSukekiyo.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/MarioTalk.hpp"
#include "Game/Player/MarioTeresa.hpp"
#include "Game/Player/MarioWait.hpp"
#include "Game/Player/MarioWall.hpp"
#include "Game/Player/MarioWarp.hpp"

Mario::Mario(MarioActor *actor) : MarioModule(actor)
{
    initMember();
    _160.zero();
    _13C.zero();
    _148.zero();
    _154.zero();
    _208.set(1.0f, 0.0f, 0.0f);
    _220 = _208;
    _43C = _208;
    _214.set(1.0f, 0.0f, 0.0f);
    _22C.set(1.0f, 0.0f, 0.0f);
    _3E4.set(1.0f, 0.0f, 0.0f);
    _2B4 = 0.0f;
    _310.set(0.0f, 0.0f, 1.0f);
    _290 = _310;
    _1D8.set(0.0f, -1.0f, 0.0f);
    _1E4.zero();
    bool nearZero = MR::isNearZero(_1D8, 0.0010000000475f);
    _1F0 = -_1D8;
    _1FC = _1F0;
    _938 = _1F0;
    _944 = _208;
    _950 = _310;
    _60D = 0;
    _71C = 0;
    _278 = 0.0f;
    _2D0 = 0.0f;
    _2D4.zero();
    _2E0.zero();
    _2EC.zero();
    _42C = 0;
    _2F8.zero();
    _328.zero();
    _334.zero();
    _488 = 0.0f;
    _430 = 0;
    _3BC = 0;
    _3BE = 0;
    _3C0 = 0;
    _3CE = 0;
    _3CC = 0;
    _3CA = 0;
    _3D0 = 0;
    _3D4 = 0;
    _424 = 0;
    _434 = 0;
    _436 = 0;
    _428 = 0;
    _3D2 = 0;
    _404 = 0;
    _406 = 0;
    _408 = 0;
    _40A = 0;
    _426 = 0;
    _418 = 0;
    _41A = 0;
    _774 = 0;
    _3C2 = 0;
    _3C4 = 0;
    _3C6 = 0;
    _3C8 = 0;
    _7D0 = 0x78;
    _524 = 0.0f;
    _528 = 0.0f;
    _52C = 0.0f;
    _530 = 0.0f;
    _534 = 0;
    _538 = 0.0f;
    _53C = 0.0f;
    _544 = 0;
    _540 = 0.0f;
    _3F8 = 0;
    _3FA = 0;
    _3FC = 0;
    _3FE = 0;
    _400 = 0;
    _402 = 0;
    _27C = 0.0f;
    _734 = 0;
    _735 = 0;
    _748 = 0;
    _71E = 0;
    _414 = 0;
    _416 = 0;
    _41C = 0;
    _41E = 0;
    _410 = 0;
    _420 = 0;
    _43A = 0;
    _3F4 = 0.0f;
    _3F0 = 1.0f;
    _58.zero();
    _12C = 0;
    _76C = 0;
    _770 = 0.0f;
    _738 = 0.0f;
    _73C = 0.0f;
    _740 = 1.0f;
    _744 = 0.0f;
    _368.set(0.0f, 1.0f, 0.0f);
    _374 = -_368;
    _380 = _368;
    _38C = -_368;
    _398.set(0.0f, 1.0f, 0.0f);
    _344.set(1.0f, 0.0f, 0.0f);
    _74C = 0.0f;
    _750 = 0;
    _754 = 0;
    _40C = 0;
    _40E = 0;
    _412 = 0;
    _558 = 0;
    _2A8 = -_368;
    _29C = _368;
    _548 = _128 = _124 = 0.0f;
    _54C = _1F0;
    _C4.identity();
    _64.identity();
    _94.identity();
    clearSlope();
    _910.zero();
    _91C.zero();
    _928.zero();
    _934 = 0.8f;
    _3C = 0.001f;
    mMovementStates_LOW_WORD = 0;
    mMovementStates_HIGH_WORD = 0;
    _10_LOW_WORD = 0;
    _10_HIGH_WORD = 0;
    mDrawStates_WORD = 0;
    _1C_WORD = 0;
    _20 = 0;
    _24 = 0;
    _28 = 0;

    _458 = TriangleFilterDelegator<Mario>::allocateDelegator(this, &Mario::isIgnoreTriangle);

    _45C = new Triangle();
    _460 = new Triangle();
    _464 = new Triangle();
    _468 = new Triangle();
    _46C = new Triangle();

    _4C8 = new Triangle();
    _4CC = new Triangle();
    _4D0 = new Triangle();
    _4D4 = new Triangle();
    _4D8 = new Triangle();
    _4DC = new Triangle();

    _470 = new Triangle();
    _474 = new Triangle();
    _478 = new Triangle();

    _7E0 = new Triangle();

    _820 = new Triangle();

    _47C = new Triangle();
    _480 = new Triangle();
    _484 = new Triangle();

    for (unsigned int i = 0; i < 0x20; i++) {
        _57C[i] = new Triangle();
    }

    _578 = 0;

    _4E8.zero();
    _4F4.zero();

    _7D4.zero();
    _8A4.zero();

    _8C8 = new Triangle();

    _8E8 = new Triangle();

    _8CC = new Triangle();
    _8D0 = new Triangle();

    _564 = -1;
    _568 = 0;
    _56C = 0;
    _570 = 0;

    for (unsigned int i = 0; i < 9; i++) {
        _A8C[i].zero();
    }

    _4A4.zero();
    _4B0.zero();

    _25C.zero();
    _268.zero();

    _350.zero();
    _35C.zero();

    _184.zero();

    _1A8.zero();

    _190.zero();
    _19C.zero();

    _4E4 = 0.0f;
    _4E0 = 0.0f;

    _72C = 200.0f;

    _730 = 0;

    _8D8 = 0;
    _8D4 = 0;

    _A38 = 0;
    _A68 = 0;

    _5FC = 0;

    _1CC.zero();

    _38 = 0.0f;
    _1C0.z = 0.0f;
    _1C0.y = 0.0f;
    _1C0.x = 0.0f;

    _3D8.zero();

    _244.zero();
    _238.zero();
    _250.zero();

    clear2DStick();

    _6AC = 0;
    _6AD = 0;
    _6B0.zero();
    _6BC.zero();

    _63C.set(1.0f, 0.0f, 0.0f);
    _648.set(0.0f, 1.0f, 0.0f);
    _654.set(0.0f, 0.0f, 1.0f);
    _6A0 = _654;

    initSound();

    _97C = 0;

    mFlow = new MarioFlow(mActor);
    mWall = new MarioWall(mActor);
    mDamage = new MarioDamage(mActor);
    mFaint = new MarioFaint(mActor);
    mBlown = new MarioBlown(mActor);
    mHang = new MarioHang(mActor);
    mSwim = new MarioSwim(mActor);
    mSlider = new MarioSlider(mActor);
    mFireDamage = new MarioFireDamage(mActor);
    mFireRun = new MarioFireRun(mActor);
    mFireDance = new MarioFireDance(mActor);
    mAbyssDamage = new MarioAbyssDamage(mActor);
    mDarkDamage = new MarioDarkDamage(mActor);
    mStep = new MarioStep(mActor);
    mBump = new MarioBump(mActor);
    mParalyze = new MarioParalyze(mActor);
    mStun = new MarioStun(mActor);
    mCrush = new MarioCrush(mActor);
    mFreeze = new MarioFreeze(mActor);
    mMagic = new MarioMagic(mActor);
    mFpView = new MarioFpView(mActor);
    mRecovery = new MarioRecovery(mActor);
    mFlip = new MarioFlip(mActor);
    mSideStep = new MarioSideStep(mActor);
    mFrontStep = new MarioFrontStep(mActor);
    mStick = new MarioStick(mActor);
    mRabbit = new MarioRabbit(mActor);
    mSukekiyo = new MarioSukekiyo(mActor);
    mBury = new MarioBury(mActor);
    mWait = new MarioWait(mActor);
    mClimb = new MarioClimb(mActor);
    mSkate = new MarioSkate(mActor);
    mFoo = new MarioFoo(mActor);
    mWarp = new MarioWarp(mActor);
    mTeresa = new MarioTeresa(mActor);
    mTalk = new MarioTalk(mActor);
    mMove = new MarioMove(mActor);

    initTask();
    _95C = new FloorCode();
    _960 = 0;
    _962 = 0;
    for (int i = 0; i < 3; i++) {
        _964[i] = _960;
    }
    _96A = 0;
    _574 = 0;
    _898 = 0;
}

/*
void Mario::setFrontVecKeepUp(const TVec3f &v)
{
    TVec3f stack_38(_208);
    if (!MR::isNearZero(v, 0.001f)) {
        setFrontVec(v);
    }
    TVec3f stack_2C;
    PSVECCrossProduct(&_1F0, &_208, &stack_2C);

    if (MR::isNearZero(stack_2C, 0.001f)) {
        TVec3f stack_20;
        MR::vecBlendSphere(stack_38, v, &stack_20, 0.5f);
        PSVECCrossProduct(&_1F0, &stack_20, &stack_2C);
        setFrontVec(stack_20);
        if (MR::isNearZero(stack_2C, 0.001f)) {
            setFrontVecKeepSide(v);
            return;
        }
    }
    MR::normalize(&stack_2C);
    _310 = stack_2C;
    MR::normalize(&_310);
    TVec3f stack_14;
    PSVECCrossProduct(&_310, &_1F0, &stack_14);
    setFrontVec(stack_14);
    _22C = _208;
    _328 = _208 % PSVECMag(_328);
    _344 = _310;
}
*/
