#pragma once

#include "Game/Player/MarioModule.hpp"
#include "Game/Util/TriangleFilter.hpp"

class MarioActor;
class MarioFlow;
class MarioWall;
class MarioDamage;
class MarioFaint;
class MarioBlown;
class MarioHang;
class MarioSwim;
class MarioSlider;
class MarioFireDamage;
class MarioFireRun;
class MarioFireDance;
class MarioAbyssDamage;
class MarioDarkDamage;
class MarioStep;
class MarioBump;
class MarioParalyze;
class MarioStun;
class MarioCrush;
class MarioFreeze;
class MarioMagic;
class MarioFpView;
class MarioRecovery;
class MarioFlip;
class MarioSideStep;
class MarioFrontStep;
class MarioStick;
class MarioRabbit;
class MarioSukekiyo;
class MarioBury;
class MarioWait;
class MarioClimb;
class MarioSkate;
class MarioFoo;
class MarioWarp;
class MarioTeresa;
class MarioTalk;
class MarioMove;
class Triangle;
class FloorCode;
struct SoundList;
class MarioState;

class Mario : public MarioModule {
public:
    typedef bool (Mario::*Task)(const void *, void *, unsigned long);
    Mario(MarioActor *);

    virtual bool postureCtrl(MtxPtr);

    void update();
    void setHeadAndFrontVecFromRotate(const TVec3f &);
    void initAfterConst();
    void initMember();
    void clearSlope();
    void clear2DStick();
    void initSound();
    void recordRelativePosition();
    unsigned long initSoundTable(SoundList* list, unsigned long);
    void initTask();
    bool isIgnoreTriangle(const Triangle *);
    bool isStatusActive(unsigned long) const;
    bool isSwimming() const;
    bool isOnimasuBinderPressSkip() const;
    void closeStatus(MarioState *);
    void stopWalk();
    void push(const TVec3f &);
    void stopJump();
    void updateGroundInfo();
    void checkEnforceMove();
    bool isDamaging() const;
    bool tryWallPunch();
    void decDamageAfterTimer();
    void incAirWalkTimer();
    void updateCubeCode();
    void startSwim();
    bool checkStartSwim();
    bool forceExitSwim();
    bool forceStartSwimAndShoot(const TVec3f &);
    bool isForceStopRush() const;
    unsigned long getCurrentStatus() const;
    bool trySpinJump(unsigned char);
    void startTeresaDisappear();
    void startBas(const char *, bool, f32, f32);
    bool checkVerticalPress(bool);
    void powerAreaMove();
    void powerRailMove();
    void checkGround();
    void updateFloorCode();
    void inputStick();
    void tryJump();
    void tryForcePowerJump(const TVec3f &, bool);
    const TVec3f& getShadowNorm() const;
    const TVec3f& getWallNorm() const;
    void setSideVec(const TVec3f &);
    void setFrontVecKeepSide(const TVec3f &);
    void setFrontVecKeepUp(const TVec3f &, f32);
    void setFrontVecKeepUp(const TVec3f &, u32);
    void setFrontVecKeepUp(const TVec3f &);
    void forceSetHeadVecKeepSide(const TVec3f &);
    void lockGroundCheck(void *, bool);
    void checkBaseTransBall();
    void changeStatus(MarioState *);

    struct MovementStates {
        unsigned _0 : 1;
        unsigned _1 : 1;
        unsigned _2 : 1;
        unsigned _3 : 1;
        unsigned _4 : 1;
        unsigned _5 : 1;
        unsigned _6 : 1;
        unsigned _7 : 1;
        unsigned _8 : 1;
        unsigned _9 : 1;
        unsigned _A : 1;
        unsigned _B : 1;
        unsigned _C : 1;
        unsigned _D : 1;
        unsigned _E : 1;
        unsigned _F : 1;
        unsigned _10 : 1;
        unsigned _11 : 1;
        unsigned _12 : 1;
        unsigned _13 : 1;
        unsigned _14 : 1;
        unsigned _15 : 1;
        unsigned _16 : 1;
        unsigned _17 : 1;
        unsigned _18 : 1;
        unsigned _19 : 1;
        unsigned _1A : 1;
        unsigned _1B : 1;
        unsigned _1C : 1;
        unsigned _1D : 1;
        unsigned _1E : 1;
        unsigned _1F : 1;
        unsigned _20 : 1;
        unsigned _21 : 1;
        unsigned _22 : 1;
        unsigned _23 : 1;
        unsigned _24 : 1;
        unsigned _25 : 1;
        unsigned _26 : 1;
        unsigned _27 : 1;
        unsigned _28 : 1;
        unsigned _29 : 1;
        unsigned _2A : 1;
        unsigned _2B : 1;
        unsigned _2C : 1;
        unsigned _2D : 1;
        unsigned _2E : 1;
        unsigned _2F : 1;
        unsigned _30 : 1;
        unsigned _31 : 1;
        unsigned _32 : 1;
        unsigned _33 : 1;
        unsigned _34 : 1;
        unsigned _35 : 1;
        unsigned _36 : 1;
        unsigned _37 : 1;
        unsigned _38 : 1;
        unsigned _39 : 1;
        unsigned _3A : 1;
        unsigned _3B : 1;
        unsigned _3C : 1;
        unsigned _3D : 1;
        unsigned _3E : 2;
    };
    struct DrawStates {
        unsigned _0 : 1;
        unsigned _1 : 1;
        unsigned _2 : 1;
        unsigned _3 : 1;
        unsigned _4 : 1;
        unsigned _5 : 1;
        unsigned _6 : 1;
        unsigned _7 : 1;
        unsigned _8 : 1;
        unsigned _9 : 1;
        unsigned _A : 1;
        unsigned _B : 1;
        unsigned _C : 1;
        unsigned _D : 1;
        unsigned _E : 1;
        unsigned _F : 1;
        unsigned _10 : 1;
        unsigned _11 : 1;
        unsigned _12 : 1;
        unsigned _13 : 1;
        unsigned _14 : 1;
        unsigned _15 : 1;
        unsigned _16 : 1;
        unsigned _17 : 1;
        unsigned _18 : 1;
        unsigned _19 : 1;
        unsigned _1A : 1;
        unsigned _1B : 1;
        unsigned _1C : 1;
        unsigned _1D : 1;
        unsigned _1E : 1;
        unsigned _1F : 1;
    };

    inline const MovementStates& getMovementStates() const {return mMovementStates;}
    inline const DrawStates& getDrawStates() const {return mDrawStates;}
    inline const DrawStates& getPrevDrawStates() const {return mPrevDrawStates;}

    union {
        MovementStates mMovementStates; // _8
        struct {
            u32 mMovementStates_LOW_WORD; // _8
            u32 mMovementStates_HIGH_WORD; // _C
        };
    };

    union {
        MovementStates _10;
        struct {
            u32 _10_LOW_WORD; // _10
            u32 _10_HIGH_WORD; // _14
        };
    };
    
    union {
        DrawStates mDrawStates; // _18
        u32 mDrawStates_WORD;
    };
    union {
        DrawStates _1C;
        u32 _1C_WORD;
    };
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;


    DrawStates mPrevDrawStates; // _30
    //FAKE
    u32 _34;
    //NOT FAKE

    f32 _38;
    f32 _3C;
    TVec3f _40;
    TVec3f _4C;
    TVec3f _58;
    TMtx34f _64;
    TMtx34f _94;
    TMtx34f _C4;
    TMtx34f _F4;
    f32 _124;
    f32 _128;
    u32 _12C;
    TVec3f _130;
    TVec3f _13C;
    TVec3f _148;
    TVec3f _154;
    TVec3f _160;
    TVec3f _16C;
    TVec3f _178;
    TVec3f _184;
    TVec3f _190;
    TVec3f _19C;
    TVec3f _1A8;
    TVec3f _1B4;
    TVec3f _1C0;
    TVec3f _1CC;
    TVec3f _1D8;
    TVec3f _1E4;
    TVec3f _1F0;
    TVec3f _1FC;
    TVec3f _208;
    TVec3f _214;
    TVec3f _220;
    TVec3f _22C;
    TVec3f _238;
    TVec3f _244;
    TVec3f _250;
    TVec3f _25C;
    TVec3f _268;
    u8 _274;
    f32 _278;
    f32 _27C;
    f32 _280;
    TVec3f _284;
    TVec3f _290;
    TVec3f _29C;
    TVec3f _2A8;
    f32 _2B4;
    TVec3f _2B8;
    TVec3f _2C4;
    f32 _2D0;
    TVec3f _2D4;
    TVec3f _2E0;
    TVec3f _2EC;
    TVec3f _2F8;
    TVec3f _304;
    TVec3f _310;
    TVec3f _31C;
    TVec3f _328;
    TVec3f _334;
    f32 _340;
    TVec3f _344;
    TVec3f _350;
    TVec3f _35C;
    TVec3f _368;
    TVec3f _374;
    TVec3f _380;
    TVec3f _38C;
    TVec3f _398;
    TVec3f _3A4;
    TVec3f _3B0;
    u16 _3BC;
    u16 _3BE;
    u16 _3C0;
    u16 _3C2;
    u16 _3C4;
    u16 _3C6;
    u16 _3C8;
    u16 _3CA;
    u16 _3CC;
    u16 _3CE;
    u16 _3D0;
    u16 _3D2;
    u16 _3D4;
    TVec3f _3D8;
    TVec3f _3E4;
    f32 _3F0;
    f32 _3F4;
    u16 _3F8;
    u16 _3FA;
    u16 _3FC;
    u16 _3FE;
    u16 _400;
    u16 _402;
    u16 _404;
    u16 _406;
    u16 _408;
    u16 _40A;
    u16 _40C;
    u16 _40E;
    u16 _410;
    u16 _412;
    u16 _414;
    u16 _416;
    u16 _418;
    u16 _41A;
    u16 _41C;
    u16 _41E;
    u16 _420;
    u16 _422;
    u16 _424;
    u16 _426;
    u16 _428;
    u16 _42A;
    u16 _42C;
    u32 _430;
    u16 _434;
    u16 _436;
    u16 _438;
    u16 _43A;
    TVec3f _43C;
    TVec3f _448;
    f32 _454;
    TriangleFilterDelegator<Mario>* _458;
    Triangle* _45C;
    Triangle* _460;
    Triangle* _464;
    Triangle* _468;
    Triangle* _46C;
    Triangle* _470;
    Triangle* _474;
    Triangle* _478;
    Triangle* _47C;
    Triangle* _480;
    Triangle* _484;
    f32 _488;
    TVec3f _48C;
    TVec3f _498;
    TVec3f _4A4;
    TVec3f _4B0;
    TVec3f _4BC;
    Triangle* _4C8;
    Triangle* _4CC;
    Triangle* _4D0;
    Triangle* _4D4;
    Triangle* _4D8;
    Triangle* _4DC;
    f32 _4E0;
    f32 _4E4;
    TVec3f _4E8;
    TVec3f _4F4;

    // FAKE
    TVec3f _500;
    // NOT FAKE

    TVec3f _50C;
    TVec3f _518;
    f32 _524;
    f32 _528;
    f32 _52C;
    f32 _530;
    u16 _534;
    f32 _538;
    f32 _53C;
    f32 _540;
    u16 _544;
    f32 _548;
    TVec3f _54C;
    u32 _558;

    // FAKE
    u32 _55C;
    u32 _560;
    //NOT FAKE

    s32 _564;
    u32 _568;
    u32 _56C;
    u8 _570;
    u32 _574;
    u32 _578;
    Triangle* _57C[0x20];
    u32 _5FC;
    TVec3f _600;
    u8 _60C;
    u8 _60D;
    u8 _60E;
    u8 _60F;
	u8 _610;
	u8 _611;
	f32 _614;
	f32 _618;
	f32 _61C;
	f32 _620;
	u32 _624;
	TVec3f _628;
	f32 _634;

    u8 _638;
    TVec3f _63C;
    TVec3f _648;
    TVec3f _654;
    TVec3f _660;

    u32 _66C;
    TVec3f _670;
    TVec3f _67C;

    TVec3f _688;
    TVec3f _694;
    TVec3f _6A0;
    u8 _6AC;
    u8 _6AD;
    TVec3f _6B0;
    TVec3f _6BC;
    f32 _6C8;
    f32 _6CC;
    TVec3f _6D0;
    TVec3f _6DC;
    TVec3f _6E8;
    TVec3f _6F4;
    TVec3f _700;
    TVec3f _70C;
    f32 _718;
    u8 _71C;
    u8 _71D;
    u8 _71E;
    u8 _71F;
    u32 _720;
    u32 _724;
    u32 _728;
    f32 _72C;
    u32 _730;
    u8 _734;
    u8 _735;
    f32 _738;
    f32 _73C;
    f32 _740;
    f32 _744;
    u8 _748;
    f32 _74C;
    u32 _750;
    u32 _754;
    MarioWall* mWall;
    TVec3f _75C;
    MarioStick* mStick;
    u16 _76C;
    f32 _770;
    u16 _774;
    MarioRabbit* mRabbit;
    MarioFoo* mFoo;
    MarioSukekiyo* mSukekiyo;
    MarioBury* mBury;
    MarioWait* mWait;
    MarioClimb* mClimb;
    TVec3f _790;
    MarioHang* mHang;
    MarioRecovery* mRecovery;
    MarioWarp* mWarp;
    MarioFlip* mFlip;
    MarioSideStep* mSideStep;
    MarioFrontStep* mFrontStep;
    MarioSkate* mSkate;
    MarioTalk* mTalk;
    MarioTeresa* mTeresa;
    MarioDamage* mDamage;
    TVec3f _7C4;
    u16 _7D0;
    TVec3f _7D4;
    Triangle* _7E0;
    TMtx34f _7E4;
    TVec3f _814;
    Triangle* _820;
    TMtx34f _824;
    MarioFlow* mFlow;
    MarioFireDamage* mFireDamage;
    MarioFireDance* mFireDance;
    MarioFireRun* mFireRun;
    MarioParalyze* mParalyze;
    MarioStun* mStun;
    MarioCrush* mCrush;
    MarioFreeze* mFreeze;
    MarioAbyssDamage* mAbyssDamage;
    MarioDarkDamage* mDarkDamage;
    MarioFaint* mFaint;
    MarioBlown* mBlown;
    MarioSwim* mSwim; // _884
    MarioSlider* mSlider; // _888
    MarioStep* mStep; // _88c
    MarioBump* mBump; // _890
    MarioMagic* mMagic; // _894
    u8 _898;
    MarioFpView* mFpView;
    MarioMove* mMove;
    TVec3f _8A4;
    TVec3f _8B0;
    TVec3f _8BC;
    Triangle* _8C8;
    Triangle* _8CC;
    Triangle* _8D0;
    u32 _8D4;
    u32 _8D8;
    TVec3f _8DC;
    Triangle* _8E8;
    u8 _8EC;

    //FAKE
    u32 _8F0;
    //NOT FAKE

    f32 _8F4;

    //FAKE
    TVec3f _8F8;
    TVec3f _904;
    //NOT FAKE

    TVec3f _910;
    TVec3f _91C;
    TVec3f _928;
    f32 _934;
    TVec3f _938;
    TVec3f _944;
    TVec3f _950;
    FloorCode* _95C;
    u16 _960;
    u16 _962;
    u16 _964[3];
    u16 _96A;

    HashSortTable* _96C; //Sounds
    const char* _970; // Sounds or somthing
    u32 _974;

    //Fake
    u32 _978;
    //Not fake
    
    u32 _97C;

    //FAKE
    u32 _980;
    //NOT FAKE
    
    Task _984[0xb];
    u32 _A08[11];
    u16 _A34;
    u32 _A38;
    u16 _A3C;
    TVec3f _A40;
    TVec3f _A4C;
    TVec3f _A58;
    f32 _A64;
    u32 _A68;
    u8 _A6C[0x20];
    TVec3f _A8C[9];
};
