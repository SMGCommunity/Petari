#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class Polta;
class PoltaArm;
class HitSensor; 

namespace PoltaFunction {
	void onMovement(Polta*);
	void emitEffectShadow(Polta*);
	PoltaArm* getLeftArmActor(Polta*);
	PoltaArm* getRightArmActor(Polta*);
	bool isCoreSensor(const Polta*, const HitSensor*);
	bool isBodySensor(const Polta*, const HitSensor*);
	bool isArmSensor(const Polta*, const HitSensor*);
	bool isLeftArmSensor(const Polta*, const HitSensor*);
	bool isRightArmSensor(const Polta*, const HitSensor*);
	bool isHeadSensor(const Polta*, const HitSensor*);
	void setScreamSensorSize(Polta*, f32);
	bool attackScreamSensor(Polta*, HitSensor*, HitSensor*);
	void startAction(Polta*, const char*, bool);
	void startBckBody(Polta*, const char*);
	void setBodyHP(Polta*, s32);
	void damageLeftArm(Polta*);
	void damageRightArm(Polta*);
	void requestStartControllArm(Polta*);
	void startArm(Polta*);
	void breakLeftArm(Polta*);
	void breakRightArm(Polta*);
	void killLeftArm(Polta*);
	void killRightArm(Polta*);
	bool isBrokenLeftArm(Polta*);
	bool isBrokenRightArm(Polta*);
	bool isEnableAttackRightArm(Polta*);
	bool isEnableAttackLeftArm(Polta*);
	bool isMaxGenerateBombTeresa(Polta*);
	s32 getCountDeadGroundRock(Polta*);
	bool appearGroundRock(Polta*, f32, f32);
	bool appearRockCircle(Polta*, const TVec3f&, f32, s32, s32, s32);
	bool appearWhiteRockCircle(Polta*, const TVec3f&, f32, s32, s32);
	bool appearBlackRockCircle(Polta*, const TVec3f&, f32, s32, s32);
	bool appearYellowRockCircle(Polta*, const TVec3f&, f32, s32, s32);
	bool appearBombTeresaFromRoot(Polta*, f32, f32, s32);
	bool appearBombTeresaNormal(Polta*, const TVec3f&, const TVec3f&);
	void disperseBombTeresa(Polta*);
	void breakGroundRock(Polta*);
	void killBombTeresa(Polta*);
	void killPoltaRock(Polta*);
	void killGroundRock(Polta*);
};
