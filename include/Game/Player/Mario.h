#pragma once

#include "Game/Player/MarioModule.h"
#include "Game/Util/TriangleFilter.h"

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

    virtual void init();
    virtual bool proc(u32);
    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual bool keep();
    virtual bool postureCtrl(MtxPtr);
    virtual void hitWall(const TVec3f &, HitSensor *);
    virtual void hitPoly(u8, const TVec3f &, HitSensor *);
    virtual bool passRing(const HitSensor *);
    virtual f32 getBlurOffset() const;
    virtual void draw3D() const;

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
	void forceExitSwim();
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

	inline bool getPunchFlag1() const { // poor name
		return _8_8;
	}

	inline bool getPunchFlag2() const { // poor name
		return _8_1a;
	}

	inline bool getPunchFlag3() const { // poor name
		return _8_19;
	}

	inline bool get_8_1()  {
		return _8_1;
	}
	inline bool get_8_a() const {
		return _8_a;
	}

		struct Nonsense {
		    unsigned _8_0 : 1;
		    unsigned _8_1 : 1;
		    unsigned _8_2 : 1;
		    unsigned _8_3 : 1;
		    unsigned _8_4 : 1;
		    unsigned _8_5 : 1;
		    unsigned _8_6 : 1;
		    unsigned _8_7 : 1;
		    unsigned _8_8 : 1;
		    unsigned _8_9 : 1;
		    unsigned _8_a : 1;
		    unsigned _8_b : 1;
		    unsigned _8_c : 1;
		    unsigned _8_d : 1;
		    unsigned _8_e : 1;
		    unsigned _8_f : 1;
		    unsigned _8_10 : 1;
		    unsigned _8_11 : 1;
		    unsigned _8_12 : 1;
		    unsigned _8_13 : 1;
		    unsigned _8_14 : 1;
		    unsigned _8_15 : 1;
		    unsigned _8_16 : 1;
		    unsigned _8_17 : 1;
		    unsigned _8_18 : 1;
		    unsigned _8_19 : 1;
		    unsigned _8_1a : 1;
		    unsigned _8_1b : 1;
		    unsigned _8_1c : 1;
		    unsigned _8_1d : 1;
		    unsigned _8_1e : 1;
		    unsigned _8_1f : 1;
		    inline bool get8() const {return _8_8;}
		    inline bool get19() const {return _8_19;}
		    inline bool get1a() const {return _8_1a;}
		    inline bool geta() const {return _8_a;}
		    inline bool get1() const {return _8_1;}
		};

	inline const Nonsense& getStates() const {return bitfieldStruct;}

    //u32 _8;
    union {
		u32 _8;
		struct {
		    unsigned _8_0 : 1;
		    unsigned _8_1 : 1;
		    unsigned _8_2 : 1;
		    unsigned _8_3 : 1;
		    unsigned _8_4 : 1;
		    unsigned _8_5 : 1;
		    unsigned _8_6 : 1;
		    unsigned _8_7 : 1;
		    unsigned _8_8 : 1;
		    unsigned _8_9 : 1;
		    unsigned _8_a : 1;
		    unsigned _8_b : 1;
		    unsigned _8_c : 1;
		    unsigned _8_d : 1;
		    unsigned _8_e : 1;
		    unsigned _8_f : 1;
		    unsigned _8_10 : 1;
		    unsigned _8_11 : 1;
		    unsigned _8_12 : 1;
		    unsigned _8_13 : 1;
		    unsigned _8_14 : 1;
		    unsigned _8_15 : 1;
		    unsigned _8_16 : 1;
		    unsigned _8_17 : 1;
		    unsigned _8_18 : 1;
		    unsigned _8_19 : 1;
		    unsigned _8_1a : 1;
		    unsigned _8_1b : 1;
		    unsigned _8_1c : 1;
		    unsigned _8_1d : 1;
		    unsigned _8_1e : 1;
		    unsigned _8_1f : 1;
		};
		Nonsense bitfieldStruct;
	};
    unsigned _8_20 : 1;
    unsigned _8_21 : 1;
    unsigned _8_22 : 1;
    unsigned _8_23 : 1;
    unsigned _8_24 : 1;
    unsigned _8_25 : 1;
    unsigned _8_26 : 1;
    unsigned _8_27 : 1;
    unsigned _8_28 : 1;
    unsigned _8_29 : 1;
    unsigned _8_2a : 1;
    unsigned _8_2b : 1;
    unsigned _8_2c : 1;
    unsigned _8_2d : 1;
    unsigned _8_2e : 1;
    unsigned _8_2f : 1;
    unsigned _8_30 : 1;
    unsigned _8_31 : 1;
    unsigned _8_32 : 1;
    unsigned _8_33 : 1;
    unsigned _8_34 : 1;
    unsigned _8_35 : 1;
    unsigned _8_36 : 1;
    unsigned _8_37 : 1;
    unsigned _8_38 : 1;
    unsigned _8_39 : 1;
    unsigned _8_3a : 1;
    unsigned _8_3b : 1;
    unsigned _8_3c : 1;
    unsigned _8_3d : 1;
    unsigned _8_3e : 2;
    //u32 _C;
    u32 _10;
    u32 _14;
    union {
		u32 _18;
		struct {
			unsigned _18_0 : 1;
			unsigned _18_1 : 1;
			unsigned _18_2 : 1;
			unsigned _18_3 : 1;
			unsigned _18_4 : 1;
			unsigned _18_5 : 1;
			unsigned _18_6 : 1;
			unsigned _18_7 : 1;
			unsigned _18_8 : 1;
			unsigned _18_9 : 1;
			unsigned _18_a : 1;
			unsigned _18_b : 1;
			unsigned _18_c : 1;
			unsigned _18_d : 1;
			unsigned _18_e : 1;
			unsigned _18_f : 1;
			unsigned _18_10 : 1;
			unsigned _18_11 : 1;
			unsigned _18_12 : 1;
			unsigned _18_13 : 1;
			unsigned _18_14 : 1;
			unsigned _18_15 : 1;
			unsigned _18_16 : 1;
			unsigned _18_17 : 1;
			unsigned _18_18 : 1;
			unsigned _18_19 : 1;
			unsigned _18_1a : 1;
			unsigned _18_1b : 1;
			unsigned _18_1c : 1;
			unsigned _18_1d : 1;
			unsigned _18_1e : 1;
			unsigned _18_1f : 1;
		};
	};
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;


	unsigned _30_0 : 1;
	unsigned _30_1 : 1;
	unsigned _30_2 : 1;
	unsigned _30_3 : 1;
	unsigned _30_4 : 1;
	unsigned _30_5 : 1;
	unsigned _30_6 : 1;
	unsigned _30_7 : 1;
	unsigned _30_8 : 1;
	unsigned _30_9 : 1;
	unsigned _30_a : 1;
	unsigned _30_b : 1;
	unsigned _30_c : 1;
	unsigned _30_d : 1;
	unsigned _30_e : 1;
	unsigned _30_f : 1;
	unsigned _30_10 : 1;
	unsigned _30_11 : 1;
	unsigned _30_12 : 1;
	unsigned _30_13 : 1;
	unsigned _30_14 : 1;
	unsigned _30_15 : 1;
	unsigned _30_16 : 1;
	unsigned _30_17 : 1;
	unsigned _30_18 : 1;
	unsigned _30_19 : 1;
	unsigned _30_1a : 1;
	unsigned _30_1b : 1;
	unsigned _30_1c : 1;
	unsigned _30_1d : 1;
	unsigned _30_1e : 1;
	unsigned _30_1f : 1;
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

    // FAKE
	u32 _610[0xA];
    // NOT FAKE

    u8 _638;
    TVec3f _63C;
    TVec3f _648;
	TVec3f _654;
    TVec3f _660;

    // fake
	TVec3f _66C;
	TVec3f _678;
	f32 _684;
    //not fake

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
