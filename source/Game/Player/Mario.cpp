#include "Game/Player/Mario.h"
#include "Game/Player/MarioFlow.h"
#include "Game/Player/MarioWall.h"
#include "Game/Player/MarioDamage.h"
#include "Game/Player/MarioFaint.h"
#include "Game/Player/MarioBlown.h"
#include "Game/Player/MarioHang.h"
#include "Game/Player/MarioSwim.h"
#include "Game/Player/MarioSlider.h"
#include "Game/Player/MarioFireDamage.h"
#include "Game/Player/MarioFireRun.h"
#include "Game/Player/MarioFireDance.h"
#include "Game/Player/MarioAbyssDamage.h"
#include "Game/Player/MarioDarkDamage.h"
#include "Game/Player/MarioStep.h"
#include "Game/Player/MarioBump.h"
#include "Game/Player/MarioParalyze.h"
#include "Game/Player/MarioStun.h"
#include "Game/Player/MarioCrush.h"
#include "Game/Player/MarioFreeze.h"
#include "Game/Player/MarioMagic.h"
#include "Game/Player/MarioFpView.h"
#include "Game/Player/MarioRecovery.h"
#include "Game/Player/MarioFlip.h"
#include "Game/Player/MarioSideStep.h"
#include "Game/Player/MarioFrontStep.h"
#include "Game/Player/MarioStick.h"
#include "Game/Player/MarioRabbit.h"
#include "Game/Player/MarioSukekiyo.h"
#include "Game/Player/MarioBury.h"
#include "Game/Player/MarioWait.h"
#include "Game/Player/MarioClimb.h"
#include "Game/Player/MarioSkate.h"
#include "Game/Player/MarioFoo.h"
#include "Game/Player/MarioWarp.h"
#include "Game/Player/MarioTeresa.h"
#include "Game/Player/MarioTalk.h"
#include "Game/Player/MarioMove.h"
#include "Game/Player/MarioMapCode.h"
#include "Game/Map/HitInfo.h"

Mario::Mario(MarioActor *parent) {
	initMember();
	_160.zero();
	_13C.zero();
	_148.zero();
	_154.zero();
	_43C = _220 = _208 = TVec3f(1f, 0f, 0f);
	_214 = TVec3f(1f, 0f, 0f);
	_22C = TVec3f(1f, 0f, 0f);
	_3E4 = TVec3f(1f, 0f, 0f);
	_2B4 = 0f;
	_290 = _310 = TVec3f(0f, 0f, 1f);
	_1D8 = TVec3f(0f, -1f, 0f);
	_1E4.zero();
	bool nearZero = MR::isNearZero(_1D8, 0.0010000000475f);
	_938 = _1F0 = -_1D8;
	_944 = _208;
	_950 = _310;
	_60D = 0;
	_71C = 0;
	_278 = 0f;
	_2D0 = 0f;
	_2D4.zero();
	_2E0.zero();
	_2EC.zero();
	_2F8.zero();
	_328.zero();
	_334.zero();
	_488=0f;
    _430=0;
    _3BC=0;
    _3BE=0;
    _3C0=0;
    _3CE=0;
    _3CC=0;
    _3CA=0;
    _3D0=0;
    _3D4=0;
    _424=0;
    _434=0;
    _436=0;
    _428=0;
    _3D2=0;
    _404=0;
    _406=0;
    _408=0;
    _40A=0;
    _426=0;
    _418=0;
    _41A=0;
    _774=0;
    _3C2=0;
    _3C4=0;
    _3C6=0;
    _3C8=0;
    _7D0=0x78;
    _524=0f;
    _528=0f;
    _52C=0f;
    _530=0f;
    _534=0;
    _538=0f;
    _53C=0f;
    _544=0;
    _540=0f;
    _3F8=0;
    _3FA=0;
    _3FC=0;
    _3FE=0;
    _400=0;
    _402=0;
    _27C=0f;
    _734=0;
    _735=0;
    _748=0;
    _71E=0;
    _414=0;
    _416=0;
    _41C=0;
    _41E=0;
    _410=0;
    _420=0;
    _43A=0;
    _3F4=0f;
    _3F0=1f;
	_58.zero();
	_12C = 0;
	_76C = 0;
	_770 = _738 = _73C = 0f;
	_740 = 1f;
	_744 = 0f;
	_368 = TVec3f(0f, 1f, 0f);
	_374 = -_368;
	_380 = _368;
	_38C = -_368;
	_398 = TVec3f(0f, 1f, 0f);
	_344 = TVec3f(1f, 0f, 0f);
	_74C = 0f;
	_750 = 0;
	_754 = 0;
	_40C = 0;
	_40E = 0;
	_412 = 0;
	_558 = 0;
	_2A8 = -_368;
	_29C = _368;
	_124 = _128 = _548 = 0f;
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
	_8 = _C = _10 = _14 = _18 = _1C = _20 = _24 = _28 = 0;

	_458 = new TriangleFilterDelegator<Mario>(this, 0, -1, &Mario::isIgnoreTriangle);

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

	for(int i = 0; i < 0x20; i++) {
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
	_564 = 0;
	_56C = 0;
	
	for(int i = 0; i < 9; i++) {
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

	_4E4 = 0f;
	_4E0 = 0f;

	_72C = 200f;

	_730 = 0;

	_8D8 = 0;
	_8D4 = 0;

	_A38 = 0;
	_A68 = 0;
	
	_5FC = 0;

	_1CC.zero();

	_38 = 0f;
	_1C0 = TVec3f(0f, 0f, 0f);

	_3D8.zero();

	_244.zero();
	_238.zero();
	_250.zero();

	clear2DStick();

	_6AC = 0;
	_6AD = 0;
	_6B0.zero();
	_6BC.zero();

	_63C = TVec3f(1f, 0f, 0f);
	_648 = TVec3f(0f, 1f, 0f);
	_6A0 = _654 = TVec3f(0f, 0f, 1f);

	initSound();

	_97C = 0;

	_854 = new MarioFlow(mActor);
	_758 = new MarioWall(mActor);
	_7C0 = new MarioDamage(mActor);
    _87C = new MarioFaint(mActor);
    _880 = new MarioBlown(mActor);
    _79C = new MarioHang(mActor);
    _884 = new MarioSwim(mActor);
    _888 = new MarioSlider(mActor);
    _858 = new MarioFireDamage(mActor);
    _860 = new MarioFireRun(mActor);
    _85C = new MarioFireDance(mActor);
    _874 = new MarioAbyssDamage(mActor);
    _878 = new MarioDarkDamage(mActor);
    _88C = new MarioStep(mActor);
    _890 = new MarioBump(mActor);
    _864 = new MarioParalyze(mActor);
    _868 = new MarioStun(mActor);
    _86C = new MarioCrush(mActor);
    _870 = new MarioFreeze(mActor);
    _894 = new MarioMagic(mActor);
    _89C = new MarioFpView(mActor);
    _7A0 = new MarioRecovery(mActor);
    _7A8 = new MarioFlip(mActor);
    _7AC = new MarioSideStep(mActor);
    _7B0 = new MarioFrontStep(mActor);
    _768 = new MarioStick(mActor);
    _778 = new MarioRabbit(mActor);
    _780 = new MarioSukekiyo(mActor);
    _784 = new MarioBury(mActor);
    _788 = new MarioWait(mActor);
    _78C = new MarioClimb(mActor);
    _7B4 = new MarioSkate(mActor);
    _77C = new MarioFoo(mActor);
    _7A4 = new MarioWarp(mActor);
    _7BC = new MarioTeresa(mActor);
    _7B8 = new MarioTalk(mActor);
    _8A0 = new MarioMove(mActor);

	initTask();
	_95C = new FloorCode();
	_960 = 0;
	_962 = 0;
	for(int i = 0; i < 3; i++) {
		_964[i] = _962;
	}
	_96A = 0;
	_574 = 0;
	_898 = 0;
}
