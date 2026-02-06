// #include "Game/Enemy/KariKariDirector.hpp"
// #include "Game/Player/Mario.hpp"
// #include "Game/LiveActor/HitSensor.hpp"
// #include "Game/LiveActor/LiveActor.hpp"
// #include "Game/Map/HitInfo.hpp"
// #include "Game/Player/MarioActor.hpp"
// #include "Game/Player/MarioAnimator.hpp"
// #include "Game/Player/MarioConst.hpp"
// #include "Game/Player/MarioRabbit.hpp"
// #include "Game/Player/MarioSwim.hpp"
// #include "Game/Player/MarioWall.hpp"
// #include "Game/Util/ActorSensorUtil.hpp"
// #include "Game/Util/MathUtil.hpp"
// #include "Game/Util/MapUtil.hpp"
// #include "Game/Util/SoundUtil.hpp"
// #include "JSystem/JGeometry/TVec.hpp"
// #include "revolution/types.h"

// static TVec3f operator/(const TVec3f& rVec, f32 div) NO_INLINE {
//     TVec3f ret(rVec);
//     ret.scale(1.0f / div);
//     return ret;
// }

// namespace {
// f32 cDropFrontSpeed = 2.0f;
// }

// bool Mario::isRising() const {
//     if (getPlayerMode() == 4 || getPlayerMode() == 6) {
//         if (_16C.dot(*getGravityVec()) < 0.0f) {
//             return true;
//         }

//         return false;
//     }

//     return mJumpVec.dot(*getGravityVec()) < 0.0f;
// }

// void Mario::checkWallRiseAndSlipFront() {
//     mMovementStates._6 = false;
//     if (mMovementStates._8) {
//         mMovementStates._9 = false;
//         _278 = 0.0f;
//         return;
//     }

//     mMovementStates._9 = true;
// }

// void Mario::tryJump() {
//     bool isMudFloorJump = false;
//     bool useStickJumpAnim = false;

//     TVec3f stickTotal(_190);
//     stickTotal += _19C;

//     if (mMovementStates.jumping) {
//         return;
//     }

//     if (_735 != 0) {
//         if (checkCurrentFloorCodeSevere(0x19)) {
//             if (_735 > 0x20) {
//                 _735 -= 0x20;
//             }

//             if (_735 < 0x40) {
//                 _735 = 0x40;
//             }

//             mMovementStates._38 = false;
//             return;
//         }

//         if (checkCurrentFloorCodeSevere(0x1F)) {
//             return;
//         }

//         if (_735 > 100) {
//             if (_735 <= 0xC8) {
//                 _735 = 0x20;
//                 changeAnimation("埋まりジャンプA", static_cast<const char*>(nullptr));
//             } else {
//                 _735 = 0x80;
//                 changeAnimation("埋まりジャンプB", static_cast<const char*>(nullptr));
//             }

//             if (!checkCurrentFloorCodeSevere(0x12)) {
//                 playSound("毒沼脱出", -1);
//             } else {
//                 playSound("砂脱出", -1);
//             }

//             playSound("声砂脱出", -1);
//             return;
//         }
//     }

//     playEffect("共通跳躍");
//     stopEffectForce("スピンリング");

//     const bool isSquat = checkSquat(true);
//     const bool preMoveState5 = mMovementStates._5;
//     mMovementStates._29 = false;
//     mMovementStates._38 = false;
//     mMovementStates._21 = true;
//     _10_HIGH_WORD |= 0x10000000;
//     mMovementStates._B = false;
//     _42A = 0;

//     if (mActor->getConst().getTable()->mJumpConnectTime < _3CE || !preMoveState5) {
//         _430 = 0;
//     } else {
//         _430++;
//     }

//     if (_430 > 2) {
//         _430 = 0;
//     }

//     if (_430 == 2 && _278 < mActor->getConst().getTable()->mJumpConnectSpeed) {
//         _430 = 0;
//     }

//     if (getPlayerMode() == 4 || getPlayerMode() == 6) {
//         _430 = 0;
//     }

//     if ((mDrawStates_WORD >> 20) & 1) {
//         _430 = 2;
//         MR::start2PJumpAssistJustSound();
//     } else if ((mDrawStates_WORD >> 21) & 1) {
//         MR::start2PJumpAssistSound();
//     }

//     if (isPlayerModeInvincible() && _278 > 1.0f) {
//         _430 = 1;
//         _42A = 1;
//     }

//     if (!MR::isNearZero(stickTotal, 0.001f) && _735 != 0) {
//         _430 = 0;
//         mMovementStates._4 = false;
//         _3D2 = 0;
//     }

//     forceStopTornado();

//     u32 floorCode = getFloorCode();
//     if (floorCode != 0x20 && getPlayerMode() != 4 &&
//         (((mMovementStates._4) != 0) || (_3D2 != 0 && checkStickFrontBack() == 2))) {
//         if (mActor->getConst().getTable()->mTurnJumpInhibitTime <= (mActor->getConst().getTable()->mTurnSlipTime - _3D2)) {
//             tryTurnJump();
//             return;
//         }

//         _3D2 = 0;
//         mMovementStates._4 = false;
//     }

//     checkWallRiseAndSlipFront();

//     const f32 floorAngle = calcPolygonAngleD(mGroundPolygon);
//     useStickJumpAnim = false;
//     if (!MR::isNearZero(_8F8, 0.001f) && (mMovementStates._23)) {
//         useStickJumpAnim = true;
//     }

//     if (floorAngle < 15.0f) {
//         mMovementStates._23 = false;
//         _8F8.zero();
//     }

//     if (getPlayerMode() != 6 && (((_10_LOW_WORD >> 5) & 1) == 0)) {
//         if (((mDrawStates_WORD >> 19) & 1) && isSlipPolygon(_45C)) {
//             const f32 dotFront = mFrontVec.dot(_368);
//             const f32 dotPad = mWorldPadDir.dot(_368);
//             const f32 diff = MR::diffAngleAbsHorizontal(_16C, _368, getAirGravityVec());

//             if (dotFront < 0.0f || dotPad <= 0.0f || diff > 1.5707964f || (mMovementStates._A)) {
//                 if (dotFront >= 0.0f && dotPad <= 0.0f) {
//                     _278 *= 0.5f;
//                     if (getPlayerMode() != 4) {
//                         _402 = 0;
//                         _430 = 3;
//                         mMovementStates._2B = true;
//                     }

//                     useStickJumpAnim = false;
//                 } else {
//                     _430 = 0;
//                     mMovementStates._2B = true;
//                 }
//             } else {
//                 mMovementStates._23 = false;
//             }
//         }

//         if (checkCurrentFloorCodeSevere(0x11)) {
//             isMudFloorJump = true;
//             _430 = 3;
//             _278 *= 0.5f;
//         }
//     }

//     floorCode = getFloorCode();
//     if (floorCode != 0x20 && getPlayerMode() != 4 && _430 != 3) {
//         if (isSquat && _278 > 0.3f && ((mMovementStates._23) == 0)) {
//             trySquatJump();
//             return;
//         }

//         if ((mMovementStates._A) && ((mMovementStates._23) == 0)) {
//             tryBackJump();
//             return;
//         }
//     }

//     if (((mDrawStates_WORD >> 26) & 1) == 0) {
//         TVec3f jumpFront(mFrontVec);
//         jumpFront.scale(_278);
//         TVec3f jumpFrontScale(jumpFront);
//         jumpFrontScale.scale(mActor->getConst().getTable()->mJumpFrontSpeed);
//         mJumpVec = jumpFrontScale;
//     } else {
//         TVec3f jumpFront(mWorldPadDir);
//         jumpFront.scale(_278);
//         TVec3f jumpFrontScale(jumpFront);
//         jumpFrontScale.scale(mActor->getConst().getTable()->mJumpFrontSpeed);
//         mJumpVec = jumpFrontScale;
//     }

//     _10_LOW_WORD &= 0xFFFFFEFF;
//     if ((mDrawStates_WORD >> 20) & 1) {
//         recordJumpEnforceMove();

//         f32 enforce = 1.0f;
//         if (getStickP() <= 0.0f) {
//             _184 = mJumpVec;
//             mJumpVec.zero();
//         } else {
//             const f32 angle = MR::diffAngleAbsHorizontal(mFrontVec, _184, getAirGravityVec());
//             enforce = 1.0f - (angle / 1.5707964f);
//             if (enforce < 0.0f) {
//                 enforce = 0.0f;
//             }

//             const f32 speed = PSVECMag(&mJumpVec);
//             const f32 speedBonus = MR::clamp(1.0f - ((speed - 4.0f) / 10.0f), 0.0f, 1.0f);
//             enforce = MR::clamp(enforce + speedBonus, 0.0f, 1.0f);
//         }

//         doEnforceJump(enforce);
//     }

//     if (!MR::isNearZero(stickTotal, 0.001f)) {
//         TVec3f opposite(-stickTotal);
//         addVelocity(opposite);

//         TVec3f sideMove;
//         MR::vecKillElement(stickTotal, mActor->_240, &sideMove);
//         cutGravityElementFromJumpVec(true);

//         if (sideMove.dot(mJumpVec) <= 0.0f) {
//             TVec3f sideMoveScale(sideMove);
//             sideMoveScale.scale(0.5f);
//             mJumpVec += sideMoveScale;
//         } else {
//             TVec3f sideMoveScale(sideMove);
//             sideMoveScale.scale(1.0f);
//             mJumpVec += sideMoveScale;
//         }
//     }

//     if (useStickJumpAnim) {
//         mJumpVec += _16C;
//         cutGravityElementFromJumpVec(true);
//     }

//     _340 = 1.0f - _278;
//     if (_340 < 0.0f) {
//         _340 = 0.0f;
//     }

//     f32 jumpRatio = 1.0f;
//     if (getFloorCode() == 0x20) {
//         jumpRatio = mActor->getConst().getTable()->mMudFloorJumpWeakRatio;
//     }
//     if ((mDrawStates_WORD >> 20) & 1) {
//         jumpRatio = 0.9f;
//     }

//     TVec3f addVecNeg(-mActor->_240);
//     TVec3f addVec(addVecNeg);
//     addVec.scale(mActor->getConst().getTable()->mJumpHeight[_430]);
//     TVec3f addScale(addVec);
//     addScale.scale(jumpRatio);
//     mJumpVec += addScale;

//     mMovementStates._1 = false;
//     mMovementStates._E = false;
//     if (getPlayerMode() != 4 && getFloorCode() == 0x20) {
//         _430 = 3;
//         isMudFloorJump = true;
//         mMovementStates._2B = true;
//     }

//     if ((_10_LOW_WORD >> 5) & 1) {
//         _430 = 0;
//     }

//     procJump(true);
//     mMovementStates.jumping = true;

//     if (_42A == 1 && _430 == 1) {
//         _430 = 2;
//     }

//     switch (_430) {
//     case 0:
//         if ((mMovementStates._17) == 0) {
//             changeAnimation("壁上昇", "落下");
//         } else {
//             changeAnimation("ジャンプ", "落下");
//         }

//         if (reinterpret_cast<const u8*>(mActor)[0x9F1] == 0) {
//             playSound("ハチ壁ジャンプ", -1);
//         } else {
//             playSound("小ジャンプ", -1);
//         }

//         if (getPlayerMode() != 6) {
//             playSound("声小ジャンプ", -1);
//             playSound("ジャンプ踏切", -1);
//         }

//         if (useStickJumpAnim) {
//             changeAnimation("尻滑りジャンプ", static_cast<const char*>(nullptr));
//             _40A = 0x1E;
//         }

//         if (getPlayerMode() == 4) {
//             changeAnimation("ハチジャンプ", static_cast<const char*>(nullptr));
//             mActor->syncJumpBeeStickMode();
//         }

//         if ((_10_LOW_WORD >> 5) & 1) {
//             changeAnimation("ショートジャンプ", "落下");
//         }
//         break;
//     case 1:
//         changeAnimationNonStop("ジャンプB");
//         changeAnimation(static_cast<const char*>(nullptr), "落下");
//         playSound("中ジャンプ", -1);
//         playSound("声中ジャンプ", -1);
//         playSound("ジャンプ踏切", -1);
//         break;
//     case 2:
//         playSound("大ジャンプ", -1);
//         playSound("声大ジャンプ", -1);
//         playSound("ジャンプ踏切", -1);
//         playEffect("共通ハイジャンプ");

//         if ((mDrawStates_WORD >> 20) & 1) {
//             changeAnimation("スカイラブジャンプ", "落下");
//         } else if (getPlayerMode() == 1 && _278 > 1.0f) {
//             changeAnimation("ダッシュジャンプ", "落下");
//         } else {
//             changeAnimation("ジャンプC", "落下");
//         }
//         break;
//     case 3:
//         if (isMudFloorJump) {
//             changeAnimation("埋まり脱出ジャンプ", "落下");
//         } else {
//             changeAnimation("腹ばいジャンプ", "落下");
//             playSound("声小ジャンプ", -1);
//             playSound("ジャンプ踏切", -1);
//         }
//         break;
//     default:
//         break;
//     }

//     if (_42A == 1 && _430 == 2) {
//         _430 = 1;
//     }

//     const f32 waterDist = mSwim->checkUnderWaterFull(mFrontVec);
//     if (waterDist > 500.0f && _278 == 0.0f) {
//         mMovementStates._E = true;
//         changeAnimationNonStop("飛び込みジャンプ");
//         playSound("声高飛び込み", -1);
//         playSound("ジャンプ踏切", -1);

//         if (_278 < 0.1f) {
//             TVec3f waterAdd(mFrontVec);
//             waterAdd.scale(5.0f);
//             mJumpVec += waterAdd;
//         }
//     }

//     mMovementStates._5 = true;
//     return;
// }

// void Mario::tryTurnJump() {
//     _430 = 0x4;

//     if (_3D2 != 0) {
//         TVec3f horizontal;
//         MR::vecKillElement(_3E4, getAirGravityVec(), &horizontal);
//         MR::normalizeOrZero(&horizontal);

//         const f32 turnJumpFrontSpeed = mActor->getConst().getTable()->mTurnJumpFrontSpeed;
//         TVec3f jumpFront(-horizontal);
//         TVec3f jumpFrontScale(jumpFront);
//         jumpFrontScale.scale(turnJumpFrontSpeed);
//         mJumpVec = jumpFrontScale;

//         TVec3f turnFront(-_3E4);
//         setFrontVecKeepUp(turnFront);
//         _220 = _3E4;
//     } else {
//         TVec3f horizontal;
//         MR::vecKillElement(_220, getAirGravityVec(), &horizontal);
//         MR::normalizeOrZero(&horizontal);

//         const f32 turnJumpFrontSpeed = mActor->getConst().getTable()->mTurnJumpFrontSpeed;
//         TVec3f jumpFront(-horizontal);
//         TVec3f jumpFrontScale(jumpFront);
//         jumpFrontScale.scale(turnJumpFrontSpeed);
//         mJumpVec = jumpFrontScale;
//     }

//     const f32 jumpHeight = mActor->getConst().getTable()->mJumpHeight[_430];
//     TVec3f addVec(-mActor->_240);
//     TVec3f addScale(addVec);
//     addScale.scale(jumpHeight);
//     mJumpVec += addScale;

//     _10_LOW_WORD &= 0xFFFFFEFF;
//     if ((mDrawStates_WORD >> 20) & 1) {
//         recordJumpEnforceMove();
//         doEnforceJump(1.0f);
//     }

//     mMovementStates._E = true;
//     procJump(true);
//     changeAnimation("ジャンプB", "埋まりジャンプA");
//     playSound("中ジャンプ", -1);
//     playSound("声中ジャンプ", -1);
//     playSound("毒沼脱出", -1);
//     playEffect("埋まりジャンプB");

//     _3D0 = 0;
//     mMovementStates.jumping = true;
//     mMovementStates._1 = false;
//     mMovementStates._4 = false;
//     mMovementStates._5 = false;
//     _278 = 0.0f;
//     _10_LOW_WORD &= 0xFFDFFFFF;
//     return;
// }

// void Mario::trySquatJump() {
//     _430 = 0x5;
//     mMovementStates._21 = false;

//     const f32 frontSpeed = mActor->getConst().getTable()->mSquatJumpFrontSpeed;
//     TVec3f jumpFront(getAirFrontVec());
//     jumpFront.scale(frontSpeed);
//     mJumpVec = jumpFront;

//     TVec3f stickVec(_190);
//     stickVec += _19C;
//     if (!MR::isNearZero(stickVec, 0.001f)) {
//         TVec3f turnFront(-stickVec);
//         typedef void (*SetFrontFn)(Mario*, const TVec3f*);
//         u32* pVtable = *reinterpret_cast<u32**>(this);
//         SetFrontFn fn = reinterpret_cast<SetFrontFn>(pVtable[3]);
//         fn(this, &turnFront);

//         MR::vecKillElement(stickVec, mActor->_240, &stickVec);
//         cutGravityElementFromJumpVec(true);

//         TVec3f stickScale(stickVec);
//         stickScale.scale(1.0f);
//         mJumpVec += stickScale;
//     }

//     const f32 jumpHeight = mActor->getConst().getTable()->mJumpHeight[_430];
//     TVec3f addVec(-mActor->_240);
//     TVec3f addScale(addVec);
//     addScale.scale(jumpHeight);
//     mJumpVec += addScale;

//     _10_LOW_WORD &= 0xFFFFFEFF;
//     if ((mDrawStates_WORD >> 20) & 1) {
//         recordJumpEnforceMove();

//         TVec3f moveKeep;
//         f32 gravityMove = MR::vecKillElement(_184, getAirGravityVec(), &moveKeep);

//         f32 enforceScale = 1.0f - (MR::diffAngleAbs(mFrontVec, moveKeep) / 1.5707964f);
//         if (enforceScale < 0.0f) {
//             enforceScale = 0.0f;
//         }

//         const f32 jumpMag = PSVECMag(&mJumpVec);
//         const f32 jumpScale = MR::clamp(1.0f - ((jumpMag - 4.0f) / 10.0f), 0.0f, 1.0f);
//         enforceScale = MR::clamp(enforceScale + jumpScale, 0.0f, 1.0f);

//         if (mJumpVec.dot(moveKeep) < 0.0f) {
//             mDrawStates_WORD |= 0x10;
//             initActiveJumpVec();
//         }

//         mVelocity -= moveKeep;

//         TVec3f moveAdd(moveKeep);
//         moveAdd.scale(enforceScale);
//         mJumpVec += moveAdd;

//         if (gravityMove < 0.0f) {
//             TVec3f gravityVec(getAirGravityVec());
//             gravityVec.scale(gravityMove);
//             mJumpVec += gravityVec;
//         }

//         invalidateRelativePosition();
//     }

//     mMovementStates._E = true;
//     mMovementStates._1 = false;
//     procJump(true);
//     changeAnimationNonStop("共通ハイジャンプ");
//     changeAnimation(static_cast<const char*>(nullptr), "落下");
//     playSound("スカイラブジャンプ", -1);
//     playSound("ダッシュジャンプ", -1);
//     playSound("毒沼脱出", -1);

//     _3D0 = 0;
//     mMovementStates._4 = false;
//     mMovementStates._5 = false;
//     mMovementStates.jumping = true;
//     return;
// }

// void Mario::tryBackJump() {
//     mMovementStates._21 = true;
//     mMovementStates._A = false;
//     _430 = 0x6;

//     const f32 backSpeed = mActor->getConst().getTable()->mSquatJumpBackSpeed;
//     mJumpVec = (-getAirFrontVec()).scaleInline(backSpeed);

//     const f32 jumpHeight = mActor->getConst().getTable()->mJumpHeight[_430];
//     mJumpVec += (-mActor->_240).scaleInline(jumpHeight);

//     _10_LOW_WORD &= 0xFFFFFEFF;
//     if ((mDrawStates_WORD >> 20) & 1) {
//         recordJumpEnforceMove();
//         doEnforceJump(1.0f);
//     }

//     mMovementStates._E = true;
//     procJump(true);
//     changeAnimation("大ジャンプ", "埋まりジャンプA");
//     playSound("中ジャンプ", -1);
//     playSound("声中ジャンプ", -1);
//     playSound("毒沼脱出", -1);
//     playEffect("埋まりジャンプB");

//     TVec3f backFront(-mFrontVec);
//     if (mSwim->checkUnderWaterFull(backFront) > 500.0f && mStickPos.z == 0.0f) {
//         changeAnimationNonStop("声大ジャンプ");
//         playSound("落下", -1);
//         playSound("毒沼脱出", -1);
//     }

//     _3D0 = 0;
//     _278 = 0.0f;
//     mMovementStates.jumping = true;
//     mMovementStates._1 = false;
//     mMovementStates._4 = false;
//     mMovementStates._5 = false;
//     return;
// }

// void Mario::tryTornadoJump() {
//     _430 = 0x8;
//     _42A = 0;

//     const f32 frontSpeed = _278;
//     const f32 tornadoSpeed = mActor->getConst().getTable()->mJumpTornadoSpeed;

//     TVec3f jumpFront(mFrontVec);
//     jumpFront.scale(frontSpeed);

//     TVec3f jumpFrontScale(jumpFront);
//     jumpFrontScale.scale(tornadoSpeed);
//     mJumpVec = jumpFrontScale;

//     const f32 jumpHeight = mActor->getConst().getTable()->mJumpHeight[_430];
//     TVec3f addVec(-mActor->_240);
//     TVec3f addScale(addVec);
//     addScale.scale(jumpHeight);
//     mJumpVec += addScale;

//     mMovementStates._E = false;
//     mMovementStates._F = true;
//     procJump(true);
//     changeAnimation("スピンリング", "埋まりジャンプA");
//     playSound("壁上昇", -1);
//     playSound("落下", -1);

//     mMovementStates.jumping = true;
//     _3D0 = 0;
//     mMovementStates._4 = false;
//     mMovementStates._5 = false;
//     mMovementStates._1D = false;
//     _278 = 0.0f;

//     _402 = mActor->getConst().getTable()->mAirWalkTimeTornado;
//     _424 = mActor->getConst().getTable()->mTornadoZeroGravityTimer;

//     const f32 boosterPower = mActor->getConst().getTable()->mTornadoBoostPower;
//     TVec3f boosterVec(-(*getGravityVec()));
//     TVec3f boosterScale(boosterVec);
//     boosterScale.scale(boosterPower);
//     setRocketBooster(boosterScale, mActor->getConst().getTable()->mTornadoBoostAttn, mActor->getConst().getTable()->mTornadoBoostTimer);
// }

// void Mario::startTornadoCentering(HitSensor* pSensor) {
//     pushTask(reinterpret_cast<Task>(&Mario::taskOnTornadoCentering), 0x400);
//     _A38 = reinterpret_cast<u32>(pSensor);
//     _A34 = 0x1E;
// }

// bool Mario::taskOnTornadoCentering(u32 a1) {
//     (void)a1;

//     if (_A34 != 0) {
//         TVec3f sensorOffset(reinterpret_cast<HitSensor*>(_A38)->mHost->mPosition);
//         sensorOffset -= mPosition;
//         MR::vecKillElement(sensorOffset, *getGravityVec(), &sensorOffset);

//         f32 mag = PSVECMag(&sensorOffset);
//         if (mag < 10.0f) {
//             mag = 10.0f;
//         }

//         f32 ratio = 10.0f / mag;
//         if (ratio < 0.1f) {
//             ratio = 0.1f;
//         }

//         TVec3f trans(sensorOffset);
//         trans.scale(ratio);
//         addTrans(trans, nullptr);

//         u32 timer = _A34;
//         timer = timer - 1;
//         _A34 = timer;

//         if (static_cast<u16>(timer) == 0) {
//             return false;
//         }

//         return true;
//     }

//     return false;
// }

// void Mario::trySpinJump(u8 a1) {
//     if (mMovementStates._C || _430 == 0x8 || isStatusActive(0x17)) {
//         return;
//     }

//     if (getPlayerMode() == 4 || getPlayerMode() == 6) {
//         return;
//     }

//     if (getPlayerMode() == 7 && mVerticalSpeed > 100.0f) {
//         tryStartFoo();
//         return;
//     }

//     if (isAnimationRun("ハチジャンプ")) {
//         return;
//     }

//     if (isAnimationRun("スピンリング")) {
//         return;
//     }

//     _430 = 0x8;
//     _42A = a1 + 1;

//     const f32 frontSpeed = _278;
//     const f32 tornadoSpeed = mActor->getConst().getTable()->mJumpTornadoSpeed;
//     TVec3f jumpFront(mFrontVec);
//     jumpFront.scale(frontSpeed);
//     TVec3f jumpFrontScale(jumpFront);
//     jumpFrontScale.scale(tornadoSpeed);
//     mJumpVec = jumpFrontScale;

//     const f32 spinJumpHeight = mActor->getConst().getTable()->mSpinJumpHeight;
//     TVec3f addVec(-mActor->_240);
//     TVec3f addScale(addVec);
//     addScale.scale(spinJumpHeight);
//     mJumpVec += addScale;

//     mMovementStates._E = false;
//     _402 = mActor->getConst().getTable()->mAirWalkTimeSpin;
//     procJump(true);

//     if (!mMovementStates._C) {
//         stopAnimationUpper(nullptr, nullptr);
//         changeAnimation("ハチジャンプ", "埋まりジャンプA");
//         playSound("壁上昇", -1);
//         playSound("落下", -1);
//         startPadVib(2);
//         _3D0 = 0;
//         mMovementStates.jumping = true;
//         mMovementStates._4 = false;
//         mMovementStates._5 = false;
//         mMovementStates._1D = false;
//         _278 = 0.0f;
//         _10_LOW_WORD |= 0x40000040;

//         if (isStatusActive(0x17)) {
//             mRabbit->hop();
//         }
//     }

//     return;
// }

// void Mario::tryForceJumpDelay(const TVec3f& rVec) {
//     mMovementStates._2D = true;
//     _304 = rVec;
// }

// void Mario::tryFreeJumpDelay(const TVec3f& rVec) {
//     mMovementStates._2E = true;
//     _304 = rVec;
// }

// void Mario::tryForceJump(const TVec3f& rVec, bool a2) {
//     if (rVec.dot(*getGravityVec()) > 0.0f) {
//         mMovementStates._20 = true;
//     }

//     mMovementStates.jumping = true;
//     mMovementStates._1 = false;
//     mMovementStates._5 = false;
//     mMovementStates._E = a2;
//     mMovementStates._B = false;
//     mJumpVec = rVec;

//     if (getPlayerMode() == 4 || getPlayerMode() == 6) {
//         _16C = mJumpVec;
//     }

//     resetTornado();

//     _3BC = 0;
//     _3CA = 0;
//     _3CC = 0;
//     mMovementStates._13 = true;
//     mMovementStates._11 = false;
//     _10_LOW_WORD &= 0xFF7FFFFF;
//     _3BE = 0;
//     _344 = mSideVec;
//     _4B0 = mPosition;
//     _42C = 0;
//     _76C = 0;
//     procJump(false);
//     changeAnimation(static_cast<const char*>(nullptr), "埋まりジャンプA");
//     mRabbit->forceJump();
//     return;
// }

// void Mario::tryForceFreeJump(const TVec3f& rVec) {
//     _430 = 0xC;
//     mMovementStates._2A = true;
//     mMovementStates._6 = false;
//     mMovementStates._9 = false;
//     tryForceJump(rVec, true);
//     _3BC = 0;
//     mMovementStates._11 = false;
//     _402 = mActor->getConst().getTable()->mAirWalkTime;

//     if (getPlayerMode() == 4) {
//         playSound("ジャンプ踏切", -1);
//     }

//     _76C = 0;
//     return;
// }

// void Mario::tryForcePowerJump(const TVec3f& rVec, bool a2) {
//     _430 = 0xD;

//     const bool moveHighBit15 = mMovementStates._30;
//     mMovementStates._2A = true;
//     mMovementStates._2F = true;
//     mMovementStates._6 = false;
//     mMovementStates._9 = false;
//     tryForceJump(rVec, true);
//     mMovementStates._2F = moveHighBit15;
//     _3BC = 0;
//     mMovementStates._11 = false;

//     if (!a2) {
//         _402 = mActor->getConst().getTable()->mAirWalkTime;

//         if (getPlayerMode() == 4) {
//             playSound("ハチ体力完全回復", -1);
//         }
//     }

//     _76C = 0;
//     return;
// }

// void Mario::tryFreeJump(const TVec3f& rVec, bool a2) {
//     _430 = 0;

//     if (rVec.dot(*getGravityVec()) > 0.0f) {
//         mMovementStates._20 = true;
//     }

//     mMovementStates.jumping = true;
//     mMovementStates._1 = false;
//     mMovementStates._5 = false;
//     mMovementStates._E = a2;
//     mMovementStates._B = false;
//     mJumpVec = rVec;
//     resetTornado();

//     _3BC = 0;
//     mMovementStates._13 = true;
//     mMovementStates._11 = false;
//     _3CA = 0;
//     _3CC = 0;
//     _3BE = 0;
//     _76C = 0;
//     _344 = mSideVec;
//     procJump(false);
//     changeAnimation(static_cast<const char*>(nullptr), "落下");
//     startPadVib(2);
//     mRabbit->forceJump();
//     return;
// }

// void Mario::tryWallJump(const TVec3f& rVec, bool a2) {
//     _430 = 0x7;

//     mMovementStates.jumping = true;
//     mMovementStates._5 = false;
//     mMovementStates._E = true;
//     mMovementStates._B = false;
//     mMovementStates._6 = false;
//     mMovementStates._28 = false;
//     mMovementStates._29 = false;
//     _24 &= 0xFF7FFFFF;

//     if (a2) {
//         mMovementStates._9 = true;
//     } else {
//         mMovementStates._9 = false;
//     }

//     mJumpVec = rVec;
//     _4B0 = mPosition;
//     _3BC = 0;
//     mMovementStates._13 = true;
//     mMovementStates._11 = false;
//     _3CA = 0;
//     _3CC = 0;
//     _3BE = 0;
//     mMovementStates._1D = true;

//     if (getPlayerMode() == 5) {
//         changeAnimationNonStop("砂脱出");
//         playSound("声砂脱出", -1);
//     } else {
//         changeAnimation("共通跳躍", "埋まりジャンプA");
//         playSound("埋まりジャンプB", -1);
//     }

//     playSound("毒沼脱出", -1);

//     if (a2) {
//         setFrontVecKeepUp(rVec);
//     }

//     stopWalk();
//     mRabbit->forceJump();
//     return;
// }

// void Mario::tryStickJump(const TVec3f& rVec) {
//     stopJump();
//     initJumpParam();

//     mMovementStates.jumping = true;
//     mMovementStates._5 = false;
//     mMovementStates._E = true;
//     mMovementStates._B = false;
//     _430 = 0;
//     mMovementStates._6 = false;
//     mMovementStates._9 = true;
//     _24 &= 0xFF7FFFFF;
//     mMovementStates._28 = false;
//     mMovementStates._29 = false;
//     mJumpVec = rVec;
//     _3CA = 0;
//     mMovementStates._13 = true;
//     mMovementStates._11 = false;
//     mMovementStates._1D = true;
//     _3CC = 0;
//     _3BE = 0;
//     _76C = mActor->getConst().getTable()->mBeeGravityReviveTime;
//     _770 = 0.0f;
//     _3BC = 0xA;
//     playSound("ハチ壁ジャンプ", -1);
//     playSound("声小ジャンプ", -1);
//     setFrontVecKeepUp(rVec);
//     changeAnimationNonStop("ハチ壁ジャンプ");
//     return;
// }

// void Mario::trySlipUpJump() {
//     if (mMovementStates._1) {
//         mMovementStates._6 = false;
//         mMovementStates._28 = false;
//         return;
//     }

//     changeAnimation("ジャンプ", static_cast<const char*>(nullptr));
//     playSound("ジャンプ", -1);
//     playEffect("ジャンプ");
//     startPadVib("ハチ壁ジャンプ");

//     const f32 slipUpHeight = mActor->getConst().getTable()->mSlipUpHeight;
//     TVec3f jumpVec(-getAirGravityVec());
//     TVec3f jumpScale(jumpVec);
//     jumpScale.scale(slipUpHeight);
//     mJumpVec = jumpScale;

//     f32 ratio = 1.0f;
//     TVec3f gravOfs(*getGravityVec());
//     gravOfs.scale(100.0f);
//     TVec3f predictPos(mPosition);
//     predictPos -= gravOfs;

//     TVec3f frontVec;
//     MR::vecKillElement(mJumpVec, mFrontVec, &frontVec);
//     MR::vecKillElement(frontVec, *getGravityVec(), &frontVec);
//     const f32 slipUpFront = mActor->getConst().getTable()->mSlipUpFront;
//     TVec3f addFront(mFrontVec);
//     addFront.scale(slipUpFront);
//     frontVec += addFront;
//     MR::normalizeOrZero(&frontVec);

//     const f32 testFront = 50.0f;
//     u32 hitCount = 0;
//     TVec3f hitPos;
//     Triangle* pTmpPolygon = getTmpPolygon();
//     while (hitCount < 10) {
//         TVec3f checkPos(frontVec);
//         checkPos.scale(testFront);
//         checkPos += predictPos;

//         TVec3f checkGrav(*getGravityVec());
//         checkGrav.scale(150.0f);
//         if (!MR::getFirstPolyOnLineBFast(checkPos, checkGrav, &hitPos, pTmpPolygon)) {
//             break;
//         }

//         hitCount++;
//     }

//     ratio *= static_cast<f32>(hitCount) / 10.0f;
//     cutVecElementFromJumpVec(mFrontVec);

//     if (mMovementStates._9) {
//         const f32 slipUpFront2 = mActor->getConst().getTable()->mSlipUpFront;
//         TVec3f frontJump(mFrontVec);
//         frontJump.scale(slipUpFront2);
//         TVec3f frontJumpScale(frontJump);
//         frontJumpScale.scale(ratio);
//         mJumpVec += frontJumpScale;
//         mMovementStates._29 = true;
//     } else {
//         const f32 slipUpFrontWeak = mActor->getConst().getTable()->mSlipUpFrontWeak;
//         TVec3f frontJump(mFrontVec);
//         frontJump.scale(slipUpFrontWeak);
//         TVec3f frontJumpScale(frontJump);
//         frontJumpScale.scale(ratio);
//         mJumpVec += frontJumpScale;
//     }

//     mMovementStates._6 = false;
//     mMovementStates._E = true;
//     initJumpParam();
//     _430 = 0xA;
//     return;
// }

// void Mario::tryHangSlipUp() {
//     mMovementStates._5 = false;
//     getPlayer()->tryJump();
//     changeAnimation("つかまりスリップアップ", static_cast<const char*>(nullptr));
//     playSound("スリップアップ", -1);
//     playEffect("スリップアップ");
//     startPadVib("マリオ[スリップアップ]");

//     const f32 slipUpHeight = mActor->getConst().getTable()->mSlipUpHeightHang;
//     TVec3f jumpVec(-getAirGravityVec());
//     TVec3f jumpScale(jumpVec);
//     jumpScale.scale(slipUpHeight);
//     mJumpVec = jumpScale;
//     cutVecElementFromJumpVec(mFrontVec);

//     const f32 slipUpFront = mActor->getConst().getTable()->mSlipUpFrontHang;
//     TVec3f frontJump(mFrontVec);
//     frontJump.scale(slipUpFront);
//     mJumpVec += frontJump;

//     _430 = 0xA;
//     mMovementStates._6 = false;
//     mMovementStates._E = true;
//     mActor->setBlendMtxTimer(0x8);
//     _71C = 0x7;
//     _278 = 0.6f;
//     return;
// }

// void Mario::tryDrop() {
//     if ((mMovementStates._8) && (((mDrawStates_WORD >> 20) & 1) == 0)) {
//         TVec3f wallVec(_4E8);
//         wallVec -= mGroundPos;

//         TVec3f horizontal;
//         MR::vecKillElement(wallVec, *getGravityVec(), &horizontal);

//         if (horizontal.dot(*mFrontWallTriangle->getNormal(0)) < 0.0f && PSVECMag(&horizontal) < 80.0f) {
//             getPlayer()->mMovementStates._1 = true;
//             const f32 pushMag = 80.0f - PSVECMag(&horizontal);

//             TVec3f pushVec(*mFrontWallTriangle->getNormal(0));
//             pushVec.scale(pushMag);
//             TVec3f pushVecScale(pushVec);
//             pushVecScale.scale(0.1f);
//             push(pushVecScale);
//             return;
//         }
//     }

//     if ((mMovementStates._1A) && (((mDrawStates_WORD >> 20) & 1) == 0)) {
//         TVec3f wallVec(_500);
//         wallVec -= mGroundPos;

//         TVec3f horizontal;
//         MR::vecKillElement(wallVec, *getGravityVec(), &horizontal);

//         if (horizontal.dot(*mSideWallTriangle->getNormal(0)) < 0.0f && PSVECMag(&horizontal) < 80.0f) {
//             const f32 pushMag = 80.0f - PSVECMag(&horizontal);

//             TVec3f pushVec(*mSideWallTriangle->getNormal(0));
//             pushVec.scale(pushMag);
//             TVec3f pushVecScale(pushVec);
//             pushVecScale.scale(0.1f);
//             push(pushVecScale);
//             getPlayer()->mMovementStates._1 = true;
//             return;
//         }
//     }

//     const f32 frontSpd = _278;
//     mMovementStates._21 = true;
//     mMovementStates._B = false;

//     TVec3f dropVec(mFrontVec);
//     dropVec.scale(frontSpd);
//     TVec3f dropVecScale(dropVec);
//     dropVecScale.scale(cDropFrontSpeed);
//     mJumpVec = dropVecScale;

//     if (MR::isNearZero(_8F8, 0.001f) && (mMovementStates._23)) {
//         TVec3f side;
//         PSVECCrossProduct(&getAirGravityVec(), &_368, &side);
//         MR::normalizeOrZero(&side);
//         PSVECCrossProduct(&_368, &side, &_8F8);
//         MR::normalizeOrZero(&_8F8);
//     }

//     if (!MR::isNearZero(_8F8, 0.001f)) {
//         mJumpVec = _8F8;
//         cutGravityElementFromJumpVec(false);

//         if (PSVECMag(&_16C) > 20.0f) {
//             TVec3f gravUp(-mActor->getAirGravityVec());
//             TVec3f gravAdd(gravUp);
//             gravAdd.scale(10.0f);
//             mJumpVec += gravAdd;
//         } else {
//             f32 gravScale = PSVECMag(&mActor->_288);
//             if (gravScale > 8.0f) {
//                 gravScale = 8.0f;
//             }

//             TVec3f gravUp(-mActor->getAirGravityVec());
//             TVec3f gravAdd(gravUp);
//             gravAdd.scale(gravScale);
//             mJumpVec += gravAdd;
//             _8F8.zero();
//         }
//     }

//     TVec3f carryVec(_184);
//     MR::vecKillElement(carryVec, *getGravityVec(), &carryVec);
//     invalidateRelativePosition();

//     TVec3f groundDelta(mGroundPos);
//     groundDelta -= mPosition;

//     TVec3f groundDeltaNoGravity;
//     MR::vecKillElement(groundDelta, *getGravityVec(), &groundDeltaNoGravity);

//     if (carryVec.dot(groundDeltaNoGravity) > 0.0f) {
//         TVec3f carryScale(carryVec);
//         carryScale.scale(0.1f);
//         carryVec = carryScale;
//     }

//     mJumpVec += carryVec;

//     _340 = 1.0f - _278;
//     if (_340 < 0.0f) {
//         _340 = 0.0f;
//     }

//     _430 = 0;
//     procJump(true);

//     _10_HIGH_WORD &= 0xEFFFFFFF;
//     mMovementStates._5 = false;
//     mMovementStates._B = false;
//     mMovementStates.jumping = true;
//     mMovementStates._E = true;

//     if (isAnimationRun("ターンジャンプ", 2) || isAnimationRun("後ジャンプ", 3)) {
//         _428 = 0xF;
//     }

//     changeAnimation(nullptr, "埋まりジャンプA");

//     if (getPlayerMode() == 4) {
//         _408 = mActor->getConst().getTable()->mBeeGravityPowerTimeD;
//         _3BC = mActor->getConst().getTable()->mBeeAirWalkInhibitTimeD - 5;
//     }

//     if (isAnimationRun("声後ジャンプ")) {
//         mMovementStates._23 = true;
//     }

//     if (mMovementStates._8) {
//         stopWalk();
//     }

//     return;
// }

// bool Mario::isDigitalJump() const NO_INLINE {
//     return mMovementStates._E;
// }

// void Mario::initActiveJumpVec() {
//     _2EC = getAirGravityVec();
//     MR::vecKillElement(mJumpVec, _2EC, &_2E0);

//     if (MR::normalizeOrZero(&_2E0)) {
//         _2E0 = mFrontVec;
//     }
// }

// void Mario::initJumpParam() {
//     const u32 flags1 = _1C_WORD;
//     mMovementStates._13 = true;

//     if ((flags1 & 0x00200000) || (flags1 & 0x00100000)) {
//         _10_LOW_WORD |= 0x00000001;
//     }

//     const u32 drawFlags = mDrawStates_WORD;
//     mMovementStates._38 = false;
//     mMovementStates._2F = false;
//     _10_LOW_WORD &= 0xFF7FFFFF;
//     _10_LOW_WORD &= 0xFFFDFFFF;

//     if ((drawFlags & 0x00000010) == 0) {
//         initActiveJumpVec();
//     }

//     _42C = 0;
//     _3BC = 0;
//     _3CA = 0;
//     _3CC = 0;
//     _774 = 0;
//     mMovementStates._11 = false;
//     _3BE = 0;
//     _344 = mSideVec;

//     _10_LOW_WORD &= 0xFFF7FFFF;

//     _408 = 0;
//     _76C = 0;
//     mMovementStates._C = false;
//     mMovementStates._10 = false;
//     mMovementStates._4 = false;
//     _3D2 = 0;
//     _3D0 = 0;
//     _3CE = 0;
//     mMovementStates._20 = false;
//     mMovementStates._2A = false;

//     if (_430 != 5) {
//         cancelSquatMode();
//     }

//     _4B0 = mPosition;
//     _426 = 0;

//     if (_735 != 0) {
//         const s32 remain = 0x100 - _735;
//         const f32 factor = 0.2f + (0.8f * (static_cast<f32>(remain) * 0.00390625f));
//         mJumpVec.scale(factor);
//         _735 = 0;
//     }
// }

// bool Mario::isEnableFutureJump() const {
//     if (_430 == 0x3) {
//         return false;
//     }

//     if (!isRising()) {
//         const f32 dot = mJumpVec.dot(*getGravityVec());
//         const s16 limit = mActor->getConst().getTable()->mFutureJumpReqLimitTime;
//         if (mVerticalSpeed < dot * static_cast<f32>(limit)) {
//             return true;
//         }
//     }

//     return false;
// }

// void Mario::procJump(bool a1) {
//     if (!a1 && ((_10_LOW_WORD & 1) == 0) && _430 == 0 && _3BC < 6 && mActor->isRequestJump2P()) {
//         _1C_WORD |= 0x00100000;
//         mMovementStates.jumping = false;
//         _3BC = 6;
//         _10_LOW_WORD |= 1;
//         tryJump();
//         _3BC = 6;
//     }

//     if (mMovementStates._B) {
//         procHipDrop();
//         return;
//     }

//     if (_424 != 0) {
//         _424--;
//         return;
//     }

//     procRocketBooster();

//     if ((_10_LOW_WORD >> 12) & 1) {
//         fixFrontVecByGravity();
//     }

//     if ((_10_LOW_WORD & 1) != 0) {
//         if (!mActor->isKeepJump2P()) {
//             mMovementStates._13 = false;
//         }
//     } else if (!mActor->isKeepJump()) {
//         mMovementStates._13 = false;
//     }

//     if (checkTrgA()) {
//         _558 = mActor->_37C;
//         if (isEnableFutureJump()) {
//             mMovementStates._38 = true;
//         }
//     }

//     if (mActor->isRequestJump2P()) {
//         _558 = mActor->_37C;
//         if (isEnableFutureJump()) {
//             mMovementStates._38 = true;
//         }
//     }

//     bool useConnectGravity = false;
//     if (mMovementStates._13 && !isDigitalJump() && _430 == 9 && isRising()) {
//         useConnectGravity = true;
//     }

//     if (a1 || mMovementStates._13 || isDigitalJump() || !isRising() || _430 == 2) {
//         useConnectGravity = true;
//     }

//     if (_430 == 8 && _42A == 0) {
//         u32 timer = _426;
//         if (timer == 0) {
//             useConnectGravity = true;
//         } else {
//             u32 diff = timer ^ static_cast<u16>(mActor->getConst().getTable()->mTornadoBoostTimer - 0x10);
//             useConnectGravity = static_cast<u32>((static_cast<s32>(diff) >> 1) - static_cast<s32>((diff & timer) >> 31)) != 0;
//         }
//     }

//     f32 gravityScale = 1.0f;
//     if (useConnectGravity && _3BC < 0x2E) {
//         gravityScale = mActor->getConst().getTable()->mGravityRatioA;
//     }

//     if (a1) {
//         initJumpParam();
//     } else if (mActor->isRequestHipDrop() && jumpToHipDrop()) {
//         return;
//     }

//     if (!isRising()) {
//         if (isAnimationRun("つかまりスリップアップ")) {
//             stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
//         }

//         if (!mActor->_3E5) {
//             if (isAnimationRun("坂すべり上向きうつぶせ")) {
//                 stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
//             }

//             if (isAnimationRun("坂すべり下向きあおむけ")) {
//                 stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
//             }

//             if (isAnimationRun("ショート着地")) {
//                 changeAnimation("水泳ジェット", static_cast<const char*>(nullptr));
//             }
//         }

//         _3BE++;
//     } else {
//         f32 ceilDist = calcDistToCeil(false);
//         if (ceilDist < 160.0f) {
//             f32 jumpGravity = cutGravityElementFromJumpVec(true);

//             TVec3f offset(getAirGravityVec());
//             offset.scale(ceilDist);

//             TVec3f effectPos(_4C);
//             effectPos -= offset;
//             playEffectTrans("スピンライト消去", effectPos);

//             f32 reduce = 0.6f;
//             if (_430 == 4 || _430 == 5 || _430 == 6) {
//                 reduce = 0.9f;
//             }

//             TVec3f addVec(getAirGravityVec());
//             addVec.scale(jumpGravity);
//             TVec3f addVecScaled(addVec);
//             addVecScaled.scale(reduce);
//             mJumpVec += addVecScaled;
//         }
//     }

//     const bool wasAirWalk = (_3CC != 0);
//     if (wasAirWalk) {
//         _3CC--;
//     }

//     if (_3BC > 10 && checkTrgA() && (mMovementStates._17)) {
//         mMovementStates._17 = false;
//         if (mWall->startJump()) {
//             return;
//         }
//     }

//     mMovementStates._12 = false;
//     if (_430 == 8) {
//         if (!checkTrgZ()) {
//             mMovementStates._11 = false;
//         } else {
//             resetTornado();
//             cancelTornadoJump();
//             stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
//             changeAnimationInterpoleFrame(1);
//             mDrawStates_WORD |= 0x00800000;
//             playSound("NoAction", -1);

//             if (_42A != 0) {
//                 playEffect("壁はじき");
//             }
//         }

//         if (_42A != 0) {
//             if (_402 != 0) {
//                 _402--;
//             }

//             if (_402 == 0) {
//                 cancelTornadoJump();
//                 playEffect("壁はじき");

//                 if (getPlayerMode() != 4) {
//                     _402 = mActor->getConst().getTable()->mAirWalkTime;
//                 }
//             }
//         }
//     }

//     if (beeMarioOnAir()) {
//         _76C = mActor->getConst().getTable()->mBeeGravityReviveTime;
//     }

//     if (_406 != 0) {
//         _406--;
//     }

//     f32 jumpGravity = 0.0f;
//     if (_430 == 0xC) {
//         jumpGravity = 1.8f;
//     } else if (_430 == 0xD) {
//         jumpGravity = mActor->getConst().getTable()->mGravityJumping[9];
//     } else {
//         jumpGravity = mActor->getConst().getTable()->mGravityJumping[_430];
//     }

//     if (_430 == 0xB && (isRising() || _3BE < mActor->getConst().getTable()->mTrampleBegomaOpenTime)) {
//         jumpGravity = mActor->getConst().getTable()->mGravityJumping[0];
//     }

//     if (_430 == 8 && _42A == 1) {
//         jumpGravity = mActor->getConst().getTable()->mSpinJumpGravity;
//     }

//     if (_430 == 8 && !isRising()) {
//         jumpGravity *= 0.25f;
//     }

//     f32 clingScale = 1.0f;
//     const s32 clingNum = MR::getKarikariClingNum();
//     if (clingNum != 0) {
//         clingScale = (2.0f + static_cast<f32>(clingNum)) * 0.5f;
//         if (getPlayerMode() == 4) {
//             clingScale = 1.0f;
//         }

//         jumpGravity *= clingScale;
//     }

//     if (wasAirWalk) {
//         jumpGravity = mActor->getConst().getTable()->mGravityAirWalk;
//     }

//     if (_76C != 0) {
//         const u16 maxTimer = mActor->getConst().getTable()->mBeeGravityReviveTime;
//         const f32 t = static_cast<f32>(_76C) / static_cast<f32>(maxTimer);
//         _76C--;
//         jumpGravity = ((1.0f - t) + (t * mActor->getConst().getTable()->mGravityAirWalk)) * clingScale;
//     }

//     if (_430 == 0xD && (isDigitalJump() || !isRising())) {
//         gravityScale = 1.0f;
//     }

//     gravityScale *= mActor->getGravityRatio();
//     if (mActor->_334 != 0) {
//         gravityScale *= 0.3f;
//         if (!mActor->isInZeroGravitySpot() && mVerticalSpeed < 300.0f) {
//             gravityScale = 0.0f;
//             if (mVerticalSpeed < 200.0f) {
//                 cutGravityElementFromJumpVec(true);
//                 gravityScale = -0.1f;
//             }
//         }
//     }

//     if (getPlayerMode() == 6) {
//         gravityScale = 0.0f;
//     }

//     if (mMovementStates._17) {
//         f32 wallRatio = static_cast<f32>(_3BC) / static_cast<f32>(mActor->getConst().getTable()->mSlipUpSpdCtrlTimer);
//         if (wallRatio > 1.0f) {
//             wallRatio = 1.0f;
//         }

//         const f32 wallScale = mActor->getConst().getTable()->mSlipUpSpdRatio +
//                               ((1.0f - wallRatio) * (1.0f - mActor->getConst().getTable()->mSlipUpSpdRatio));
//         addVelocity(mJumpVec, wallScale);

//         TVec3f addVec(mActor->_240);
//         addVec.scale(jumpGravity);
//         TVec3f addVecScale(addVec);
//         addVecScale.scale(gravityScale);
//         TVec3f addVecWall(addVecScale);
//         addVecWall.scale(wallScale);
//         mJumpVec += addVecWall;
//         moveWallSlide(1.0f);
//     } else if ((mMovementStates._1) == 0) {
//         addVelocity(mJumpVec);

//         TVec3f addVec(mActor->_240);
//         addVec.scale(jumpGravity);
//         TVec3f addVecScale(addVec);
//         addVecScale.scale(gravityScale);
//         mJumpVec += addVecScale;
//     }

//     checkWallRising();
//     checkWallJumpHit();

//     if ((mMovementStates._1) == 0) {
//         fixFrontVecFromUpSide();
//     }

//     decideSlipUp();

//     if (_430 != 0xD || !mMovementStates._20) {
//         TVec3f side;
//         const f32 dropSpeed = MR::vecKillElement(mJumpVec, *getGravityVec(), &side);

//         f32 maxDrop = mActor->getConst().getTable()->mMaxDropSpeed[mActor->getGravityLevel() & 0xFF];
//         if (_430 == 0xB && !isRising() && _3BE >= mActor->getConst().getTable()->mTrampleBegomaOpenTime) {
//             maxDrop = mActor->getConst().getTable()->mMaxBackJumpSpeed;
//         }

//         if (getPlayerMode() == 4 && !isRising()) {
//             maxDrop = mActor->getConst().getTable()->mMaxDropSpeed[4];
//         }

//         if (maxDrop <= dropSpeed) {
//             TVec3f gravVec(*getGravityVec());
//             gravVec.scale(maxDrop);
//             TVec3f gravAdd(gravVec);
//             gravAdd += side;
//             mJumpVec = gravAdd;
//         }
//     }

//     if ((mMovementStates._1) != 0 && !a1) {
//         doLanding();

//         if (calcDistToCeil(false) < 80.0f) {
//             bool pressedA = false;
//             const HitSensor* sensorA = reinterpret_cast<const HitSensor*>(_730);
//             if (sensorA != nullptr && MR::isSensorPressObj(sensorA)) {
//                 pressedA = true;
//             }

//             bool pressedB = false;
//             const HitSensor* sensorB = reinterpret_cast<const HitSensor*>(reinterpret_cast<const u32*>(mGroundPolygon)[2]);
//             if (sensorB != nullptr && MR::isSensorPressObj(sensorB)) {
//                 pressedB = true;
//             }

//             if (sensorA != sensorB && (pressedA || pressedB)) {
//                 mActor->setPress(0, 0);
//                 mActor->_3B4 = *mGroundPolygon->getNormal(0);
//             }
//         }
//     } else {
//         doAirWalk();
//     }

//     _3BC++;
//     return;
// }

// void Mario::checkWallRising() {
// }

// void Mario::checkWallJumpHit() {
//     if (_430 == 0x5) {
//         if ((mMovementStates._8) && !checkWallJumpCode()) {
//             const f32 wallDot = cutVecElementFromJumpVec(getWallNorm());
//             if (checkWallCode("ジャンプC", true)) {
//                 return;
//             }

//             TVec3f wallBack(getWallNorm());
//             wallBack.scale(wallDot);
//             mJumpVec -= wallBack;

//             TVec3f blownVec(mJumpVec);
//             blownVec.scale(0.2f);
//             blown(blownVec);

//             TVec3f horizontal;
//             MR::vecKillElement(mJumpVec, *getGravityVec(), &horizontal);
//             MR::normalizeOrZero(&horizontal);
//             if (!MR::isNearZero(horizontal, 0.001f)) {
//                 TVec3f turnFront(-horizontal);
//                 setFrontVecKeepUp(turnFront);
//             }

//             _402 = 0;
//             mMovementStates._2B = true;
//             _428 = 0x3C;
//         }
//         return;
//     }

//     if (!isRising() && _3BC > 10 && (mMovementStates._8) && calcPolygonAngleD(mFrontWallTriangle) < 80.0f) {
//         if ((mMovementStates._2E) == 0) {
//             const f32 gravityElement = cutGravityElementFromJumpVec(true);

//             TVec3f gravityVec(*getAirGravityVec());
//             gravityVec.scale(gravityElement);

//             TVec3f wallVec(*mFrontWallTriangle->getNormal(0));
//             wallVec.scale(5.0f);
//             TVec3f jumpVec(wallVec);
//             jumpVec += gravityVec;
//             mJumpVec = jumpVec;
//             mMovementStates._2E = true;
//             changeAnimation("埋まり脱出ジャンプ", static_cast<const char*>(nullptr));
//         } else {
//             const f32 jumpMag = PSVECMag(&mJumpVec);
//             cutVecElementFromJumpVec(*mFrontWallTriangle->getNormal(0));
//             mJumpVec.setLength(jumpMag);
//         }
//     }
// }

// void Mario::decideSlipUp() {
//     if (isAnimationRun("埋まり脱出ジャンプ")) {
//         return;
//     }

//     if (!mMovementStates._9) {
//         return;
//     }

//     if (mMovementStates._F) {
//         mMovementStates._6 = false;
//         return;
//     }

//     bool moveLow6 = mMovementStates._6;
//     bool moveLow7 = mMovementStates._7;

//     if (checkWallCode("腹ばいジャンプ", true)) {
//         return;
//     }

//     if (checkWallCode("ジャンプC", true)) {
//         return;
//     }

//     bool slipFront = false;
//     if (isStickOn()) {
//         const TVec3f& wallNorm = getWallNorm();
//         if (getWorldPadDir().dot(wallNorm) < -0.8f) {
//             slipFront = true;
//         }
//     }

//     if (_430 == 0x7) {
//         slipFront = true;
//     }

//     if ((mMovementStates._2A) == 0 && (mMovementStates._8) && isRising() && slipFront) {
//         mMovementStates._6 = true;
//     }

//     if (isRising() && getPlayerMode() != 4 && getPlayerMode() != 6 && getPlayerMode() != 5) {
//         if (_4E0 < mActor->getConst().getTable()->mSlipUpContinueHeight) {
//             moveLow6 = mMovementStates._6;
//             moveLow7 = mMovementStates._7;
//             if (mMovementStates._8 && mMovementStates._9 && slipFront && mMovementStates._29) {
//                 mMovementStates._6 = true;
//                 cutGravityElementFromJumpVec(true);

//                 TVec3f upBoost(-getAirGravityVec());
//                 upBoost.scale(30.0f);
//                 mJumpVec += upBoost;
//             }
//         }
//     }

//     if (moveLow6) {
//         if (!moveLow7 && isRising() && !mMovementStates._8 &&
//             (mMovementStates._15)) {
//             trySlipUpJump();
//         }
//     }
// }

// void Mario::moveWallSlide(f32 a1) {
//     TVec3f crossVec;
//     const TVec3f& wallNorm = getWallNorm();
//     PSVECCrossProduct(getGravityVec(), &wallNorm, &crossVec);

//     if (!MR::normalizeOrZero(&crossVec) && isStickOn()) {
//         f32 dot = crossVec.dot(getWorldPadDir());
//         const f32 absDot = __fabsf(dot);
//         if (absDot > 0.2f) {
//             if (dot < 0.0f) {
//                 dot = (0.2f + dot) / 0.8f;
//             } else {
//                 dot = (dot - 0.2f) / 0.8f;
//             }

//             addVelocity(crossVec, (10.0f * dot) * a1);
//         }
//     }
// }

// bool Mario::jumpToHipDrop() {
//     if (mMovementStates._B) {
//         return false;
//     }

//     if (_430 == 5) {
//         return false;
//     }

//     if (_430 == 9 || _430 == 0xB || _430 == 0xC || _430 == 0xD) {
//         if (getPlayerMode() != 4 && isRising()) {
//             return false;
//         }
//     }

//     if (mVerticalSpeed < mActor->getConst().getTable()->mHipDropLimitHeight) {
//         return false;
//     }

//     if (MR::isNearZero(mActor->_240, 0.001f)) {
//         return false;
//     }

//     if (getPlayerMode() == 6) {
//         return false;
//     }

//     const bool isNormalDrop = !mActor->_3E5;

//     resetTornado();
//     cancelTornadoJump();

//     mDrawStates_WORD |= 0x00800000;
//     mMovementStates._21 = false;
//     mMovementStates._B = true;
//     mJumpVec.zero();
//     _422 = 0;

//     if (_430 == 4) {
//         TVec3f turnFront(-_220);
//         setFrontVecKeepUp(turnFront);
//         _430 = 0;
//     }

//     if (isAnimationRun("ホッパー壁ジャンプ")) {
//         stopAnimationUpper(nullptr, nullptr);
//     }

//     if (isPlayerModeHopper()) {
//         _720 = getAnimationStringPointer("幅とび");
//         _724 = getAnimationStringPointer("幅ジャンプ");
//         _728 = getAnimationStringPointer("水泳スピン移動");
//         startHipDropBlur();
//     } else if (getPlayerMode() == 4) {
//         _720 = getAnimationStringPointer("声幅ジャンプ");
//         _724 = getAnimationStringPointer("しゃがみジャンプ");

//         if (reinterpret_cast<const u8*>(mActor)[0x9F1] == 0) {
//             _728 = getAnimationStringPointer("声スピン");
//         } else {
//             _728 = getAnimationStringPointer("後方飛び込みジャンプ");
//         }
//     } else if (!isNormalDrop) {
//         _720 = getAnimationStringPointer("水泳スピンジャンプ");
//         _724 = getAnimationStringPointer("空中ひねり");
//         _728 = getAnimationStringPointer("スピンジャンプ");
//         stopEffect("ハチ体力完全回復");
//         _10_HIGH_WORD |= 0x01000000;
//     } else {
//         _720 = getAnimationStringPointer("トルネードジャンプ");
//         _724 = getAnimationStringPointer("\377\377\377\377");
//         _728 = getAnimationStringPointer("水泳スピン移動");
//         startHipDropBlur();
//     }

//     changeAnimation(_720, _724);
//     playSound("ホッパージャンプ", -1);

//     if (isNormalDrop) {
//         playSound("壁ジャンプ", -1);
//     } else {
//         playSound("スリップアップ", -1);
//         playSound("マリオ[スリップアップ]", -1);
//     }

//     _424 = mActor->getConst().getTable()->mHipDropZeroGrTime;
//     mRabbit->forceJump();
//     mMovementStates._3E = 0;
//     return true;
// }

// void Mario::procHipDrop() {
//     if (isAnimationRun(_720)) {
//         if (!isAnimationTerminate(nullptr)) {
//             return;
//         }

//         if (_424 != 0) {
//             _424--;
//             return;
//         }

//         stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
//         return;
//     }

//     if (_424 != 0) {
//         _424--;
//         return;
//     }

//     if (mActor->_38C != 0) {
//         return;
//     }

//     if (MR::isNearZero(mActor->_240, 0.001f)) {
//         mMovementStates.digitalJump = true;
//     }

//     if (mActor->_334 != 0) {
//         _2F8.x *= 0.5f;
//         _2F8.y *= 0.5f;
//         _2F8.z *= 0.5f;
//         mJumpVec.x *= 0.5f;
//         mJumpVec.y *= 0.5f;
//         mJumpVec.z *= 0.5f;
//     }

//     if ((mMovementStates._1) != 0 || isAnimationRun(_728)) {
//         fixFrontVecByGravity();

//         if (isAnimationRun(_728)) {
//             bool shouldEnd = false;

//             if (mActor->_334 == 0) {
//                 if ((mMovementStates._1) == 0 && _3BC > 3 && mVerticalSpeed > 10.0f) {
//                     changeAnimation(_724, static_cast<const char*>(nullptr));
//                     mJumpVec = _2F8;
//                     goto PROC_HIP_DROP_MOVE;
//                 }
//             } else {
//                 shouldEnd = true;
//             }

//             u16 endLimit = 0xF;
//             if (isAnimationRun("ハチヒップドロップ")) {
//                 endLimit = 0x2D;
//             }

//             if (endLimit < _3CE) {
//                 if (isStickOn()) {
//                     shouldEnd = true;
//                 }

//                 if ((mDrawStates_WORD >> 19) & 1) {
//                     shouldEnd = true;
//                 }
//             }

//             if (_3CE > 5 && checkTrgA()) {
//                 shouldEnd = true;
//             }

//             if (isAnimationTerminate(nullptr) || shouldEnd) {
//                 mMovementStates.jumping = false;
//                 mMovementStates._13 = false;
//                 stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//                 stopEffect("ホッパーヒップドロップ開始");

//                 if (checkTrgA()) {
//                     tryJump();
//                     return;
//                 }
//             }

//             _278 = 0.0f;
//         } else {
//             _3CE = 0;
//             _278 = 0.0f;
//             _71E = 0;

//             if (isCurrentFloorSand()) {
//                 changeStatus(reinterpret_cast<MarioState*>(mBury));
//                 return;
//             }

//             if (isCurrentFloorSink()) {
//                 mMovementStates.jumping = false;
//                 mMovementStates._13 = false;
//                 stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//                 _735 = 200;
//                 return;
//             }

//             playSound("NotWallSlip", -1);
//             playSound("カリカリ限界", -1);
//             playEffectRT("ホッパーヒップドロップ開始", _368, _4C);
//             startPadVib("ホッパーヒップドロップ");
//             startCamVib(0);
//             mDrawStates_WORD |= 0x800;
//             MR::removeAllClingingKarikari();

//             if (_960 == 0x1B || _960 == 0x1C || _960 == 9) {
//                 mMovementStates.jumping = false;
//                 mMovementStates._13 = false;
//                 stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//                 return;
//             }

//             changeAnimationWithAttr(_728, 1);
//             _2F8 = mJumpVec;
//             clearSlope();

//             if (reinterpret_cast<const u8*>(mActor)[0xB90] != 0) {
//                 mMovementStates.jumping = false;
//                 mMovementStates._13 = false;
//             }
//         }

//         mJumpVec.zero();
//     } else {
//         _3BC++;

//         if (MR::isNearZero(mActor->getLastMove(), 0.001f)) {
//             _422++;
//             if (_422 == 0xF) {
//                 stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//                 mMovementStates.jumping = false;
//                 mMovementStates._13 = false;
//                 mJumpVec.zero();
//                 mMovementStates._1 = true;
//                 return;
//             }
//         }
//     }
// PROC_HIP_DROP_MOVE:
//     TVec3f moveVec(mJumpVec);
//     if (PSVECMag(&moveVec) >= mVerticalSpeed) {
//         moveVec.setLength(mVerticalSpeed);
//     }
//     addVelocity(moveVec);

//     if ((mMovementStates._1) == 0) {
//         const f32 speedMag = PSVECMag(&mJumpVec);
//         const f32 speedRate = MR::clamp(0.1f + (1.0f - (speedMag / mActor->getConst().getTable()->mLimitSpeedHipDrop)), 0.0f, 1.0f);
//         const f32 gravityHipDrop = mActor->getConst().getTable()->mGravityHipDrop;

//         TVec3f gravVec(*getGravityVec());
//         gravVec.scale(gravityHipDrop);
//         TVec3f gravAdd(gravVec);
//         gravAdd.scale(speedRate);
//         mJumpVec += gravAdd;

//         const f32 jumpGravity = cutGravityElementFromJumpVec(true);
//         if (PSVECMag(&mJumpVec) > 10.0f) {
//             mJumpVec.x *= 0.5f;
//             mJumpVec.y *= 0.5f;
//             mJumpVec.z *= 0.5f;
//         }

//         TVec3f jumpGravityVec(getAirGravityVec());
//         jumpGravityVec.scale(jumpGravity);
//         mJumpVec += jumpGravityVec;

//         if (PSVECMag(&mJumpVec) > mActor->getConst().getTable()->mLimitSpeedHipDrop) {
//             mJumpVec.setLength(mActor->getConst().getTable()->mLimitSpeedHipDrop);
//         }

//         if (((_10_HIGH_WORD >> 23) & 1) && isAnimationRun("ハチヒップドロップ開始")) {
//             playSound("ヒップドロップ着地", -1);

//             const f32 homingMaxAngle = 45.0f;
//             Triangle strikeTriangles[0x20];

//             f32 speed = mVerticalSpeed;
//             if (speed < 0.0f) {
//                 speed = 0.0f;
//             }

//             const u32 strikeNum = static_cast<u32>(Collision::checkStrikeBallToMapWithThickness(mShadowPos, speed, speed, nullptr, nullptr));

//             HitSensor* homingSensor = nullptr;
//             for (u32 i = 0; i < strikeNum; i++) {
//                 const HitInfo* strikeInfo = Collision::getStrikeInfoMap(i);
//                 HitSensor* sensor = strikeInfo->mParentTriangle.mSensor;

//                 if (mActor->selectHomingInSuperHipDrop(sensor->mHost->mName)) {
//                     TVec3f sensorPos(sensor->mPosition);
//                     TVec3f diff(sensorPos);
//                     diff -= mActor->_2A0;

//                     if (MR::diffAngleAbs(diff, *getGravityVec()) < homingMaxAngle) {
//                         homingSensor = sensor;
//                     }
//                 }
//             }

//             if (homingSensor == nullptr) {
//                 homingSensor = reinterpret_cast<HitSensor*>(mActor->_4A8);
//             }

//             if (homingSensor != nullptr) {
//                 TVec3f targetJump(homingSensor->mPosition);
//                 targetJump -= mActor->_2A0;
//                 targetJump.setLength(PSVECMag(&mJumpVec));

//                 TVec3f frontVec;
//                 mActor->getFrontVec(&frontVec);
//                 if (frontVec.dot(mJumpVec) < 0.0f) {
//                     const f32 hitGravity = cutGravityElementFromJumpVec(true);
//                     TVec3f gravVec(getAirGravityVec());
//                     gravVec.scale(hitGravity);
//                     TVec3f jumpNeg(-mJumpVec);
//                     TVec3f jumpNegAdd(jumpNeg);
//                     jumpNegAdd += gravVec;
//                     mJumpVec = jumpNegAdd;
//                 } else if (mJumpVec.dot(targetJump) > 0.0f) {
//                     mJumpVec = targetJump;
//                 }
//             }
//         }

//         if ((mMovementStates._9) || (mMovementStates._1B) ||
//             (mMovementStates._1A)) {
//             _10_HIGH_WORD &= ~0x80;
//         }

//         fixFrontVecFromUpSide();
//     }
// }

// void Mario::doAirWalk() {
//     if (isAnimationRun("水泳ジェット")) {
//         return;
//     }

//     if (_430 == 3) {
//         return;
//     }

//     if (mMovementStates._2F) {
//         return;
//     }

//     if (isAnimationRun("壁はじき")) {
//         return;
//     }

//     if (mMovementStates._1D) {
//         if (_3BC > mActor->getConst().getTable()->mWaitNeutralTimer) {
//             mMovementStates._1D = false;
//         }
//         return;
//     }

//     if (_428 != 0) {
//         _428--;
//         if (_428 == 0) {
//             stopAnimation("坂すべり上向きうつぶせ", 2);
//             stopAnimation("坂すべり下向きあおむけ", 3);
//         }
//         return;
//     }

//     TVec3f moveDir;
//     calcMoveDir(mStickPos.x, mStickPos.y, &moveDir, true);

//     if (mActor->_334 != 0) {
//         moveDir.zero();
//     }

//     if ((mDrawStates_WORD & 0x40000) != 0) {
//         if (MR::diffAngleAbs(getAirGravityVec(), _2EC) > 0.7853982f) {
//             TVec3f sideVec;
//             PSVECCrossProduct(&getAirGravityVec(), &mFrontVec, &sideVec);
//             MR::normalizeOrZero(&sideVec);

//             const f32 angle = MR::diffAngleSignedHorizontal(_2EC, getAirGravityVec(), sideVec);
//             Mtx rotMtx;
//             PSMTXRotAxisRad(rotMtx, &sideVec, angle);

//             TVec3f frontRot;
//             PSMTXMultVecSR(rotMtx, &_2E0, &frontRot);
//             if (frontRot.dot(mFrontVec) < 0.0f) {
//                 moveDir = -mFrontVec;
//             } else {
//                 moveDir = mFrontVec;
//             }
//         }
//     }

//     f32 frontDot = MR::vecKillElement(moveDir, *getGravityVec(), &moveDir);
//     TVec3f sideMove;
//     MR::vecKillElement(moveDir, mFrontVec, &sideMove);

//     bool inhibitFrontAdjust = false;
//     if (_430 == 0xB || _430 == 8 || getPlayerMode() == 4 || getPlayerMode() == 6) {
//         inhibitFrontAdjust = true;
//     }

//     if (!inhibitFrontAdjust) {
//         if (frontDot <= 0.0f) {
//             f32 frontReduction = mActor->getConst().getTable()->mJumpFrontReduction;
//             s16 reduceBegin = mActor->getConst().getTable()->mJumpFrontReductionBeginTime;
//             if (_430 == 5) {
//                 frontReduction = mActor->getConst().getTable()->mSquatJumpFrontReduction;
//                 reduceBegin = mActor->getConst().getTable()->mSquatJumpFrontReductionBTime;
//             }

//             if (((_10_LOW_WORD >> 8) & 1) && reinterpret_cast<u32*>(_45C)[2] == _8D8) {
//                 MtxPtr prevMtx = reinterpret_cast<MtxPtr>(_8E8->getPrevBaseMtx());
//                 MtxPtr baseMtx = reinterpret_cast<MtxPtr>(_8E8->getBaseMtx());
//                 if (MR::isSameMtx(baseMtx, prevMtx)) {
//                     MR::vecKillElement(_8DC, getAirGravityVec(), &_8DC);
//                     mJumpVec -= _8DC;
//                     _8DC.zero();
//                 }
//             }

//             if ((mMovementStates._11) == 0 && _3BC > reduceBegin) {
//                 const f32 jumpGravity = cutGravityElementFromJumpVec(true);
//                 const f32 jumpMag = PSVECMag(&mJumpVec);
//                 mJumpVec.setLength(frontReduction * jumpMag);

//                 TVec3f gravVec(getAirGravityVec());
//                 gravVec.scale(jumpGravity);
//                 mJumpVec += gravVec;
//             }
//         } else {
//             if (!isRising() && mVerticalSpeed < mActor->getConst().getTable()->mLandTurnHeight && (mMovementStates._11) == 0) {
//                 mMovementStates._10 = true;
//             } else {
//                 _3CA++;
//             }

//             f32 speedKiller;
//             if (isAnimationRun("ターンジャンプ")) {
//                 _3CA = 0;
//                 speedKiller = 0.1f;
//             } else if (_3CA < mActor->getConst().getTable()->mBackJumpLimitFrame) {
//                 const f32 remain = static_cast<f32>(mActor->getConst().getTable()->mBackJumpLimitFrame - _3CA);
//                 speedKiller = mActor->getConst().getTable()->mBackJumpRatio *
//                               (remain / static_cast<f32>(mActor->getConst().getTable()->mBackJumpLimitFrame));
//             } else {
//                 speedKiller = 0.0f;
//             }

//             if (mMovementStates._11) {
//                 TVec3f backMove(mFrontVec);
//                 backMove.scale(mActor->getConst().getTable()->mAirWalkBackBonus);
//                 TVec3f backMoveScale(backMove);
//                 backMoveScale.scale(frontDot);

//                 TVec3f result(sideMove);
//                 result += backMoveScale;
//                 moveDir = result;
//             } else {
//                 TVec3f backMove(mFrontVec);
//                 backMove.scale(frontDot);
//                 TVec3f backMoveScale(backMove);
//                 backMoveScale.scale(speedKiller);

//                 TVec3f result(sideMove);
//                 result += backMoveScale;
//                 moveDir = result;
//             }

//             const f32 backDot = -moveDir.dot(mFrontVec);
//             if (backDot > mActor->getConst().getTable()->mMaxBackJumpSpeed) {
//                 moveDir.setLength(mActor->getConst().getTable()->mMaxBackJumpSpeed);
//             } else if (backDot < 0.0f && _3CA < mActor->getConst().getTable()->mBackJumpLimitFrame) {
//                 _3CC = 0xA;
//             }
//         }
//     }

//     TVec3f moveNorm(moveDir);
//     MR::normalizeOrZero(&moveNorm);

//     TVec3f jumpNorm(mJumpVec);
//     MR::normalizeOrZero(&jumpNorm);

//     if (!inhibitFrontAdjust) {
//         const f32 dot = moveNorm.dot(jumpNorm);
//         if (dot < 0.0f) {
//             const f32 jumpGravity = cutGravityElementFromJumpVec(true);

//             TVec3f killer(mJumpVec);
//             killer.scale(mActor->getConst().getTable()->mAirWalkSpeedKiller);
//             TVec3f killerScale(killer);
//             killerScale.scale(dot);
//             mJumpVec += killerScale;

//             TVec3f gravityVec(getAirGravityVec());
//             gravityVec.scale(jumpGravity);
//             mJumpVec += gravityVec;
//         }
//     }

//     if (_430 == 8) {
//         TVec3f spinAdd(moveDir);
//         spinAdd.scale(mActor->getConst().getTable()->mWalkSpeed);
//         TVec3f spinAddScale(spinAdd);
//         spinAddScale.scale(5.0f);
//         TVec3f spinAddDiv(spinAddScale / static_cast<f32>(mActor->getConst().getTable()->mAirWalkTimerFact2));
//         mJumpVec += spinAddDiv;
//     } else if (_430 == 0xB && !isRising()) {
//         TVec3f trampleAdd(moveDir);
//         trampleAdd.scale(mActor->getConst().getTable()->mWalkSpeed);
//         TVec3f trampleAddScale(trampleAdd);
//         trampleAddScale.scale(5.0f);
//         TVec3f trampleAddDiv(trampleAddScale / static_cast<f32>(mActor->getConst().getTable()->mAirWalkTimerFact2));
//         mJumpVec += trampleAddDiv;
//         playSound("天井ヒット", -1);
//     } else if (getPlayerMode() == 4) {
//         TVec3f beeAdd(moveDir);
//         beeAdd.scale(mActor->getConst().getTable()->mBeeAirWalkAcc);
//         mJumpVec += beeAdd;
//         playSound("天井ヒット", -1);
//     } else if (getPlayerMode() == 6) {
//         const f32 moveFrontDot = moveDir.dot(mFrontVec);
//         if (mJumpVec.dot(moveDir) < 0.0f && moveFrontDot < 0.0f) {
//             TVec3f normJump(mJumpVec);
//             if (!MR::normalizeOrZero(&normJump)) {
//                 if (normJump.dot(mFrontVec) < 0.0f) {
//                     const f32 ratio = (moveFrontDot + 1.0f) * 0.5f;
//                     const f32 scale = 0.998f - (0.01f * (1.0f - ratio));
//                     mJumpVec.scale(scale);
//                 }
//             }
//         }
//     } else {
//         TVec3f addVec(moveDir);
//         addVec.scale(mActor->getConst().getTable()->mWalkSpeed);
//         TVec3f addVecScale(addVec);
//         addVecScale.scale(mActor->getConst().getTable()->mAirWalkTimerFact1);
//         TVec3f addVecDiv(addVecScale / static_cast<f32>(_3BC + mActor->getConst().getTable()->mAirWalkTimerFact2));
//         mJumpVec += addVecDiv;
//     }

//     if (_430 != 0xB && getPlayerMode() != 6 && !isRising()) {
//         addVelocity(moveDir, mActor->getConst().getTable()->mWalkSpeed * mActor->getConst().getTable()->mAirWalkTimerFact3);
//     }

//     const f32 jumpGravity = cutGravityElementFromJumpVec(true);
//     if (_430 == 8) {
//         if (PSVECMag(&mJumpVec) > 6.0f) {
//             mJumpVec.setLength(6.0f);
//         }
//     } else if (_430 == 0xB) {
//         if (PSVECMag(&mJumpVec) > 7.0f) {
//             mJumpVec.setLength(7.0f);
//         }
//     } else if (getPlayerMode() == 4) {
//         if (_774 == 0) {
//             if (PSVECMag(&mJumpVec) > mActor->getConst().getTable()->mBeeAirWalkLimit) {
//                 mJumpVec.setLength(mActor->getConst().getTable()->mBeeAirWalkLimit);
//             }
//         } else if (_774 > 0x1E) {
//             if (PSVECMag(&mJumpVec) > mActor->getConst().getTable()->mMaxJumpSpeed) {
//                 mJumpVec.setLength(mActor->getConst().getTable()->mMaxJumpSpeed);
//             }
//         } else {
//             const f32 t = static_cast<f32>(_774) / 30.0f;
//             const f32 limit = (t * mActor->getConst().getTable()->mMaxJumpSpeed) +
//                               ((1.0f - t) * mActor->getConst().getTable()->mBeeAirWalkLimit);
//             if (PSVECMag(&mJumpVec) > limit) {
//                 mJumpVec.setLength(limit);
//             }
//         }
//     } else if (getPlayerMode() != 6) {
//         if (PSVECMag(&mJumpVec) > mActor->getConst().getTable()->mMaxJumpSpeed) {
//             mJumpVec.setLength(mActor->getConst().getTable()->mMaxJumpSpeed);
//         }
//     }

//     TVec3f gravityVec(getAirGravityVec());
//     gravityVec.scale(jumpGravity);
//     mJumpVec += gravityVec;
// }

// void Mario::stopJump() {
//     fixFrontVecByGravity();
//     mJumpVec.zero();

//     mMovementStates.jumping = false;
//     mMovementStates._B = false;
//     mMovementStates._6 = false;

//     mMovementStates._28 = false;
//     mMovementStates._2B = false;

//     _10_LOW_WORD &= 0xFFFFFEFF;
//     _10_LOW_WORD &= 0xFFFFFFBF;
//     _10_LOW_WORD &= 0xFFFFFFFE;

//     if (getPlayerMode() != 4) {
//         _402 = mActor->getConst().getTable()->mAirWalkTime;
//     }

//     _3BC = 0;
//     _424 = 0;
//     _426 = 0;
//     _428 = 0;

//     if (isDefaultAnimationRun("埋まりジャンプA")) {
//         changeAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//     }

//     if (isDefaultAnimationRun(_724)) {
//         changeAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//     }

//     reinterpret_cast<u8*>(mWall)[0x1C] = 0;
// }

// void Mario::cancelTornadoJump() {
//     _430 = 0;

//     if ((mMovementStates._1) == 0) {
//         stopAnimation("ハチジャンプ", "埋まりジャンプA");
//     } else {
//         stopAnimation("ハチジャンプ", static_cast<const char*>(nullptr));
//     }

//     _4B0 = mPosition;
//     _426 = 0;
//     mMovementStates._2B = true;
// }

// void Mario::setRocketBooster(const TVec3f& rVec, f32 a2, u16 a3) {
//     _426 = a3;
//     _454 = a2;
//     _448 = rVec;
// }

// void Mario::procRocketBooster() {
//     if (_426 == 0) {
//         return;
//     }

//     _426--;
//     mJumpVec += _448;
//     _448.scale(_454);
// }

// bool Mario::isSoftLandingFloor() const {
//     switch (_960) {
//     case 0x6:
//     case 0x7:
//     case 0x8:
//     case 0xB:
//     case 0x22:
//         return true;
//     default:
//         return false;
//     }
// }

// void Mario::checkAndTryForceJump() {
//     bool doForceJump = false;
//     TVec3f forceJumpVec;

//     if (!mMovementStates._1 || mMovementStates.jumping || isAnimationRun(_728)) {
//         switch (_960) {
//         case 0x6:
//         case 0x22: {
//             const f32 jumpPower = mActor->getConst().getTable()->mCodeJumpPower[0];
//             TVec3f jumpVec(*mGroundPolygon->getNormal(0));
//             jumpVec.scale(jumpPower);
//             forceJumpVec = jumpVec;
//             doForceJump = true;
//             break;
//         }
//         case 0x7: {
//             const f32 jumpPower = mActor->getConst().getTable()->mCodeJumpPower[1];
//             TVec3f jumpVec(*mGroundPolygon->getNormal(0));
//             jumpVec.scale(jumpPower);
//             forceJumpVec = jumpVec;
//             doForceJump = true;
//             break;
//         }
//         case 0x8: {
//             const f32 jumpPower = mActor->getConst().getTable()->mCodeJumpPower[2];
//             TVec3f jumpVec(*mGroundPolygon->getNormal(0));
//             jumpVec.scale(jumpPower);
//             forceJumpVec = jumpVec;
//             doForceJump = true;
//             break;
//         }
//         case 0xB: {
//             MR::vecKillElement(_16C, *getGravityVec(), &forceJumpVec);
//             forceJumpVec.setLength(10.0f);

//             const f32 jumpPower = mActor->getConst().getTable()->mCodeJumpPower[3];
//             TVec3f gravityNeg(-(*getGravityVec()));
//             TVec3f addVec(gravityNeg);
//             addVec.scale(jumpPower);
//             forceJumpVec += addVec;
//             doForceJump = true;
//             break;
//         }
//         default:
//             break;
//         }
//     }

//     if (mMovementStates._2D) {
//         doForceJump = false;

//         if (mActor->mHealth != 0) {
//             if (getCurrentStatus() == 2) {
//                 closeStatus(nullptr);
//             }

//             tryForceFreeJump(_304);
//         }

//         mMovementStates._2D = false;
//     }

//     if (mMovementStates._2E) {
//         doForceJump = false;

//         if (mActor->mHealth != 0) {
//             if (getCurrentStatus() == 2) {
//                 closeStatus(nullptr);
//             }

//             tryFreeJump(_304, true);
//         }

//         mMovementStates._2E = false;
//     }

//     if (doForceJump) {
//         mActor->sendMsgToSensor(reinterpret_cast<HitSensor* >(reinterpret_cast< u32*>(mGroundPolygon)[2]), 2);

//         if (getCurrentStatus() == 2) {
//             if (mActor->mHealth == 0) {
//                 mActor->forceGameOverNonStop();
//             }

//             closeStatus(nullptr);
//         }

//         mMovementStates._21 = true;
//         mMovementStates._2E = true;
//         _304 = forceJumpVec;
//         changeAnimation("飛び込みジャンプ", static_cast<const char*>(nullptr));

//         if (mActor->isEnableNerveChange()) {
//             playSound("声高飛び込み", -1);
//             playSound("落下", -1);
//             playSound("毒沼脱出", -1);
//         }
//     }
// }

// void Mario::doLanding() {
//     bool doHardLanding = false;
//     bool blendWait = false;
//     bool keepFrontSlip = false;

//     if (getPlayerMode() == 6) {
//         return;
//     }

//     if (mMovementStates._37) {
//         afterLanding2D();
//     }

//     mMovementStates.jumping = false;
//     _10_LOW_WORD &= 0xFFFFFFFE;
//     _10_HIGH_WORD &= 0xBFFFFFFF;
//     _3CE = 0;
//     _1C_WORD |= 0x2000;

//     TVec3f landingDelta(mPosition);
//     landingDelta -= _4B0;
//     if (_3BC < 4 && __fabsf(landingDelta.dot(*getGravityVec())) < 1.0f && PSVECMag(&mJumpVec) < 10.0f) {
//         stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//         return;
//     }

//     if (reinterpret_cast<const u8*>(mActor)[0xEEB] && (mMovementStates._2B) &&
//         ((_10_LOW_WORD >> 6) & 1) && _430 != 3) {
//         _10_LOW_WORD &= 0xFFFFFFBF;
//         reinterpret_cast<u16*>(reinterpret_cast<u8*>(mActor) + 0x946)[0] = 0x10;
//     }

//     resetTornado();

//     if (reinterpret_cast<const u8*>(mActor)[0x9F1] == 0) {
//         playSound("スケート着地", -1);
//     } else {
//         playSound("スケート静止着地", -1);
//     }

//     if ((isAnimationRun("フーファイター着地") || isAnimationRun("重い着地") || isAnimationRun("声ステージイン着地")) &&
//         (getPlayerMode() == 3 || getPlayerMode() == 0)) {
//         playEffect("しゃがみ基本");
//         if (getStickP() == 0.0f) {
//             mJumpVec.zero();
//             stopWalk();
//             changeAnimation("ステージインB", "ショートジャンプ");
//         } else {
//             changeStatus(reinterpret_cast<MarioState*>(mSkate));
//         }
//         return;
//     }

//     if (_430 == 5 && isSkatableFloor()) {
//         if (checkSquat(false)) {
//             mMovementStates._A = true;
//             _278 = 0.0f;
//             stopAnimation(static_cast<const char*>(nullptr), "スーパースピンドライバ終了");
//         } else {
//             cancelSquatMode();
//             stopAnimationUpper(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
//             changeAnimation("ステージインB", "ショートジャンプ");
//         }
//         return;
//     }

//     if (isAnimationRun("共通着地大")) {
//         stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//         return;
//     }

//     if (((reinterpret_cast<const u32*>(getPlayer())[4] >> 29) & 1) != 0) {
//         u32 playerFlags = reinterpret_cast<u32*>(getPlayer())[4];
//         playerFlags &= 0xDFFFFFFF;
//         reinterpret_cast<u32*>(getPlayer())[4] = playerFlags;
//         _430 = 0xE;
//     } else {
//         const u32 landingType = mMovementStates._3E;
//         if (landingType == 2) {
//             reinterpret_cast<u32*>(mActor)[3] &= 0xFFFFFFFC;
//             stopWalk();
//             playSound("中ダメージ着地", -1);
//             playSound("倒れ", -1);
//             changeAnimation("ハード着地", "ショートジャンプ");
//             startCamVib(4);
//             return;
//         }

//         if (landingType == 1) {
//             reinterpret_cast<u32*>(getPlayer())[3] &= 0xFFFFFFFC;
//             playEffect("共通着地普通");
//             playEffect("水上ダメージ中");
//             if (!isStickFull()) {
//                 blendWait = true;
//             }
//             doHardLanding = true;
//             if (_1FC.dot(_368) < 0.0f) {
//                 if (_960 == 0xD || _960 == 0x1E || _960 == 5 || _960 == 0x17) {
//                     changeStatus(reinterpret_cast<MarioState*>(mSukekiyo));
//                 } else {
//                     stopWalk();
//                     startCamVib(4);
//                     changeAnimation("飛び込み失敗回転着地", "ショートジャンプ");
//                     _3CE += 0x10;
//                     forceSetHeadVecKeepSide(_368);
//                 }
//                 return;
//             }
//         } else {
//             if (isCurrentFloorSink()) {
//                 stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//                 _735 = 0x40;
//                 return;
//             }

//             if ((_960 == 0x1B || _960 == 0x1C) && !strcmp(MR::getSoundCodeString(mGroundPolygon), "Sand")) {
//                 stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//                 _735 = 0x20;
//                 return;
//             }

//             if (_3BC > 10) {
//                 playEffect("飛び込み失敗着地");
//             }
//         }
//     }

//     switch (_430) {
//     case 0:
//     case 0xA:
//     case 0xB:
//     case 0xE:
//         startPadVib(static_cast<u32>(0));
//         break;
//     case 2:
//     case 4:
//     case 6:
//         startPadVib(3);
//         break;
//     default:
//         startPadVib(2);
//         break;
//     }

//     getAnimator()->addRumblePower(5.0f, 0x3C);
//     mMovementStates._21 = false;

//     if (getPlayerMode() == 4) {
//         _71E = 0;
//     } else {
//         _402 = mActor->getConst().getTable()->mAirWalkTime;
//     }

//     if (!isSoftLandingFloor()) {
//         TVec3f hardDelta(_4B0);
//         hardDelta -= mPosition;
//         TVec3f invGrav(-(*getGravityVec()));
//         if (hardDelta.dot(invGrav) > mActor->getConst().getTable()->mHardLandingHeight) {
//             doHardLanding = true;
//         }
//     }

//     if (doHardLanding && isCurrentFloorSand()) {
//         changeStatus(reinterpret_cast<MarioState*>(mBury));
//         return;
//     }

//     if (isBlendWaitGround()) {
//         blendWait = true;
//     }
//     fixFrontVecByGravity();

//     if (!doHardLanding && (((mDrawStates_WORD >> 19) & 1) == 0) && _430 != 0xE &&
//         !isAnimationRun("ターンジャンプ") && !isAnimationRun("後ジャンプ") && (((_10_LOW_WORD >> 8) & 1) == 0) &&
//         !isAnimationRun("最強")) {
//         if (_430 == 5 && checkSquat(false)) {
//             mMovementStates._A = true;
//             _278 = 0.0f;
//             stopAnimation(static_cast<const char*>(nullptr), "スーパースピンドライバ終了");
//             goto POST_LANDING;
//         }

//         const bool stickOrKeep = isStickFull() || _60D != 0;
//         const bool squatStop = ((mMovementStates._B) == 0) && checkSquat(false);
//         if ((stickOrKeep && _3CA == 0) || squatStop) {
//             stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//             if (((_10_LOW_WORD >> 21) & 1) == 0) {
//                 recordTurnSlipAngle();
//             }

//             if (_430 == 5) {
//                 cancelSquatMode();
//             }
//             goto POST_LANDING;
//         }

//         switch (_430) {
//         case 2:
//         case 4:
//         case 5:
//         case 6:
//         case 8:
//         case 0xB:
//             keepFrontSlip = true;
//             break;
//         default:
//             break;
//         }

//         if (((_10_LOW_WORD >> 23) & 1) && _45C != nullptr && _8D8 == reinterpret_cast<const u32*>(_45C)[2]) {
//             TVec3f jumpDelta(mJumpVec);
//             jumpDelta -= _8DC;
//             if (jumpDelta.dot(mJumpVec) < 0.0f) {
//                 mJumpVec.zero();
//             } else {
//                 mJumpVec = jumpDelta;
//             }
//         }

//         const f32 frontDot = mFrontVec.dot(mJumpVec);
//         if (frontDot > 0.5f && !keepFrontSlip) {
//             _278 = MR::clamp(frontDot / mActor->getConst().getTable()->mJumpFrontSpeed, 0.0f, 1.0f);
//             stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//             _3FA = 0;
//             _71E = 0;

//             if (((_10_LOW_WORD >> 21) & 1) == 0) {
//                 recordTurnSlipAngle();
//             }
//             goto POST_LANDING;
//         }
//     }

//     if (_430 == 4) {
//         TVec3f front(-_220);
//         setFrontVecKeepUp(front);
//     }

//     if (isAnimationRun("ターンジャンプ") || isAnimationRun("後ジャンプ") || ((_10_LOW_WORD >> 8) & 1)) {
//         if (isAnimationRun("後ジャンプ")) {
//             TVec3f front(-mFrontVec);
//             setFrontVecKeepUp(front);
//         }

//         if ((_10_LOW_WORD >> 8) & 1) {
//             doHardLanding = true;
//         }

//         stopWalk();
//         if (doHardLanding || isAnimationRun("後ジャンプ")) {
//             if (doHardLanding) {
//                 changeAnimation("ターンジャンプ", "ショートジャンプ");
//                 startSlidingTask(8, 5.0f, 0x19);
//                 startCamVib(0);
//             } else {
//                 changeAnimation("声後ジャンプ", "ショートジャンプ");
//             }
//         } else {
//             changeAnimation("声後ジャンプ", "ショートジャンプ");
//         }

//         goto POST_LANDING;
//     }

//     clearSlope();
//     switch (_430) {
//     case 0:
//     case 7:
//         changeAnimation("着地B", "ショートジャンプ");
//         break;
//     case 1:
//         changeAnimation("着地C", "ショートジャンプ");
//         break;
//     case 2:
//         changeAnimation("着地ターン", "ショートジャンプ");
//         break;
//     case 4:
//         changeAnimation("着地幅とび", "ショートジャンプ");
//         break;
//     case 5:
//         if (!checkSquat(false)) {
//             cancelSquatMode();
//         }

//         if (reinterpret_cast<const u32*>(mActor)[0x11A] == 0) {
//             stopAnimationUpper(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
//         }

//         changeAnimation("しゃがみジャンプ着地", "ショートジャンプ");
//         break;
//     case 6:
//         changeAnimation("フーファイター着地", "ショートジャンプ");
//         break;
//     default:
//         if (isAnimationRun("最強")) {
//             changeAnimation("重い着地", "ショートジャンプ");
//         } else {
//             changeAnimation("着地B", "ショートジャンプ");
//         }
//         break;
//     case 3:
//     case 0xA:
//         stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//         break;
//     case 0xE:
//         playSound("中ダメージ着地", -1);
//         playSound("スケート着地", -1);
//         playSound("倒れ", -1);
//         playEffect("声小ダメージ");
//         changeAnimation("重い着地", "ショートジャンプ");
//         doHardLanding = false;
//         blendWait = false;
//         break;
//     }

//     stopWalk();
//     if (blendWait) {
//         changeAnimation("ショート着地", "ショートジャンプ");
//     }

//     if (doHardLanding) {
//         changeAnimation("飛び込み失敗回転着地", "ショートジャンプ");
//         playSound("中ダメージ着地", -1);
//         playSound("倒れ", -1);
//         startCamVib(4);
//         _3CE += 0x10;
//     }

//     if (reinterpret_cast<const u8*>(mActor)[0x9F1] != 0) {
//         changeAnimation("声ステージイン着地", "ショートジャンプ");
//     }

//     if ((mDrawStates_WORD >> 19) & 1) {
//         if (!isAnimationRun("ターンジャンプ", 2) && !isAnimationRun("後ジャンプ", 3)) {
//             stopAnimation(static_cast<const char*>(nullptr), "ショートジャンプ");
//         }

//         changeAnimation("ステージインB", "ショートジャンプ");
//         _71E = 0;
//     }

// POST_LANDING:
//     if (((mDrawStates_WORD >> 19) & 1) == 0) {
//         _8F8.zero();
//     } else {
//         TVec3f jumpNoGrav;
//         MR::vecKillElement(mJumpVec, *getGravityVec(), &jumpNoGrav);

//         if (jumpNoGrav.dot(*mGroundPolygon->getNormal(0)) > 0.0f) {
//             mMovementStates._23 = true;
//             _8F0 = 10.0f;
//         } else {
//             const f32 ratio = (90.0f - calcPolygonAngleD(mGroundPolygon)) / 90.0f;
//             _278 = MR::clamp(_278 * ratio, 0.0f, 1.0f);

//             const f32 minVel = 10.0f - (10.0f * ratio);
//             if (_8F0 < minVel) {
//                 _8F0 = minVel;
//             }
//         }

//         const f32 magSlip = PSVECMag(&_8F8);
//         const f32 magJump = PSVECMag(&jumpNoGrav);
//         if (magJump < magSlip) {
//             const f32 magJump2 = PSVECMag(&jumpNoGrav);
//             const f32 magSlip2 = PSVECMag(&_8F8);
//             _8F8.setLength(0.5f * (magSlip2 + magJump2));
//         } else {
//             _8F8.zero();
//         }
//     }

//     return;
// }

// void Mario::startSlidingTask(u32 a1, f32 a2, u16 a3) {
//     TVec3f slideVec(mFrontVec);
//     slideVec.scale(a2);
//     _A40 = slideVec;
//     _A3C = a3;
//     pushTask(reinterpret_cast<Task>(&Mario::taskOnSlide), a1);
// }

// bool Mario::taskOnSlide(u32 a1) {
//     (void)a1;
//     addVelocity(_A40);
//     _A3C--;
//     return _A3C != 0;
// }

// bool Mario::taskOnWallRising(u32 a1) {
//     (void)a1;

//     if ((mMovementStates._17) == 0) {
//         stopEffect("小ジャンプ");
//         return false;
//     }

//     playSound("声小ジャンプ", -1);
//     return true;
// }

// void Mario::incAirWalkTimer() {
//     const u16 timer = _402;
//     const u16 limit = mActor->getConst().getTable()->mAirWalkTime;

//     if (timer >= limit) {
//         return;
//     }

//     _402 = timer + 1;

//     if (getPlayerMode() != 4) {
//         return;
//     }

//     const u16 nextTimer = _402;
//     const u16 nextLimit = mActor->getConst().getTable()->mAirWalkTime;

//     if (nextTimer >= nextLimit) {
//         playSound("ジャンプ踏切", -1);
//     } else {
//         playSound("尻滑りジャンプ", -1);
//     }
// }

// namespace NrvMarioActor {
//     INIT_NERVE(MarioActorNrvWait);
//     INIT_NERVE(MarioActorNrvGameOver);
//     INIT_NERVE(MarioActorNrvGameOverAbyss);
//     INIT_NERVE(MarioActorNrvGameOverAbyss2);
//     INIT_NERVE(MarioActorNrvGameOverFire);
//     INIT_NERVE(MarioActorNrvGameOverBlackHole);
//     INIT_NERVE(MarioActorNrvGameOverNonStop);
//     INIT_NERVE(MarioActorNrvGameOverSink);
//     INIT_NERVE(MarioActorNrvTimeWait);
//     INIT_NERVE(MarioActorNrvNoRush);
// };  // namespace NrvMarioActor
