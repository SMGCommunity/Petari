#include "Game/Ride/BigBubble.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace NrvBigBubble {
  NEW_NERVE(BigBubbleNrvAppear, BigBubble, Appear);
  NEW_NERVE(BigBubbleNrvWait, BigBubble, Wait);
  NEW_NERVE_ONEND(BigBubbleNrvCapture, BigBubble, Capture, Capture);
  NEW_NERVE(BigBubbleNrvBreak, BigBubble, Break);
  NEW_NERVE(BigBubbleNrvEscape, BigBubble, Escape);
  NEW_NERVE(BigBubbleNrvGoal, BigBubble, Goal);
  NEW_NERVE(BigBubbleNrvMerged, BigBubble, Merged);
};

/*
BigBubble::BigBubble(const char *pName) : LiveActor(pName), _8C(0), _90(0), _94(0), _98(0), _9C(0), _148(0, 0, 0, 1), _158(0, 0, 0, 1) {
  _174.x = 0;
  _174.y = 0;
  _174.z = 0;
  _180.x = 0;
  _180.y = 0;
  _180.z = 0;
  _1A4.x = 0;
  _1A4.y = 0;
  _1A4.z = 0;
  _1B0.x = 0;
  _1B0.y = 0;
  _1B0.z = 0;
  _1F8 = 0;
  _1FC = 0;
  _228 = 255;
  _229 = false;
  _22A = false;
  _22B = false;
  _22C = false;
  _22D = false;
  _22E = false;
  _22F = false;
  _230 = false;
  _231 = true;
  _232 = true;

  _8C = new OctahedronBezierSurface(3);
  _B8.identity();
  _118.identity();
}
*/

/*
void BigBubble::init(const JMapInfoIter &rIter) {
  MR::initDefaultPos(this, rIter);
  _118.setTrans(mPosition);
  MR::connectToScene(this, 41, 5, -1, -1);
  BigBubble::initSensor();
  initNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance);
  getBaseRadius();
  initBinder(0, 0, 16);
  initEffectKeeper(0, "BigBubble", false);
  MR::setEffectHostMtx(this, "PushWind", _B8);
  initSound(6, false);
  MR::onCalcGravity(this);
  getBaseRadius();
  MR::initShadowVolumeSphere(this, 0.8f);
  getBaseRadius();
  MR::initStarPointerTarget(this, 0, 0);
  if (MR::isValidInfo(rIter))
    makeActorAppeared();
  else
    makeActorDead();
  MR::invalidateClipping(this);
}
*/

void BigBubble::setActionType(s32 v1) {
  switch (v1) {
    case 0: {
      _231 = false;
      _232 = true;
    }
    default: {
      return;
    }
    case 1: {
      _231 = false;
      _232 = false;
    }
  }
}

void BigBubble::setHost(LiveActor *mActor) { 
  this->_94 = mActor;
}

bool BigBubble::requestCancelBind() {
  if (_98) {
    _98 = 0;
  }
  _22B = 0;
  MR::emitEffect(this, "Break");
  MR::startSound(this, "SE_OJ_BIG_BUBBLE_BREAK", -1, -1);
  _230 = true;
  _168 = _174;
  _1F8 = 0;
  kill();
  return true;
}

bool BigBubble::tryAppearEnd() {
  if (MR::isGreaterEqualStep(this, 30)) {
    setNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance);
    return true;
  }
  return false;
}

bool BigBubble::tryAutoBreak() {
  if (!_22C && MR::isGreaterStep(this, 1200) || _22F) {
    if (_98) {
      MR::endBindAndPlayerWeakGravityLimitJump(this, mVelocity);
      _98 = false;
    }
    setNerve(&NrvBigBubble::BigBubbleNrvBreak::sInstance);
    return true;
  }
  return false;
}

bool BigBubble::tryBreakEnd() {
  if (MR::isGreaterStep(this, 20)) {
    kill();
    return true;
  }
  return false;
}

bool BigBubble::tryEscape()
{
  if (MR::testSubPadTriggerZ(0))
  {
    setNerve(&NrvBigBubble::BigBubbleNrvEscape::sInstance);
    return true;
  }
  return false;
}

bool BigBubble::tryEscapeEnd()
{
  if (MR::isGreaterStep(this, 30))
  {
    if (_98)
    {
      MR::calcGravity(this);
      TVec3f vec;
      vec = mGravity;
      vec *= 30;
      MR::endBindAndPlayerJump(this, vec, 30);
      _98 = false;
    }
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_BIG_BUBBLE_BREAK", -1, -1);
    kill();
    return true;
  }
  return false;
}

bool BigBubble::tryMergedCancel() {
  if (MR::isDead(_9C)) {
    kill();
    return true;
  }
  return false;
}

bool BigBubble::tryMergeEnd() {
  if (MR::isGreaterStep(this, 45)) {
    if (_9C->receiveMessage(62, getSensor("body"), _9C->getSensor("body"))) {
      _9C = 0;
      kill();
      return true;
    }
  }
  return false;
}

void BigBubble::exeWait() {
  if (MR::isFirstStep(this))
    _1A4.zero();
  addCoriolisAccel();
  if (_22C) {
    MR::zeroVelocity(this);
  }
  else {
    updateNormalVelocity();
  }
  if (tryAutoBreak()) {
    return;
  }
}

void BigBubble::exeBreak() {
  if (MR::isFirstStep(this)) {
    if (_98) {
      MR::endBindAndPlayerWeakGravityLimitJump(this, mVelocity);
      _98 = false;
    }
    _22B = false;
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_BIG_BUBBLE_BREAK", -1, -1);
    _230 = true;
    _168 = _174;
    _1F8 = 0;
  }
  _1F8 = MR::calcNerveRate(this, 20);
  updateNormalVelocity();
  if (tryBreakEnd())
    return;
}

void BigBubble::exeGoal() {
  if (MR::isFirstStep(this)) {
    MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
  }
  updateNormalVelocity();
  if (tryBreakEnd()) {
    return;
  }
}

bool BigBubble::isPushable() const {
  if (isNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance) || isNerve(&NrvBigBubble::BigBubbleNrvCapture::sInstance)) {
    return true;
  }
  return false;
}

bool BigBubble::isBindMario() const {
  return _98;
}

bool BigBubble::isDraw() const { 
  return (!_22E && !_230 && !MR::isDead(this));
}

bool BigBubble::isMerged() {
  return isNerve(&NrvBigBubble::BigBubbleNrvMerged::sInstance);
}

bool BigBubble::isEnemyAttackBreakable() const {
  if (isNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance) || isNerve(&NrvBigBubble::BigBubbleNrvCapture::sInstance)) {
    return true;
  }
  return false;
}

/*
void BigBubble::exeEscape()
{
  if (MR::isFirstStep(this))
  {
    _22B = false;
    MR::startBckPlayer("SwimDive", "BigBubbleEscape");
    MR::startSoundPlayer("SE_PV_HIP_DROP", -1);
    MR::startSoundPlayer("SE_PM_PRE_HIPDROP", -1);
    _168 = _174;
    MR::calcNerveRate(this, 30);
    MR::calcGravity(this);
    updateNormalVelocity();
    TVec3f vec;
    _158.getYDir(vec);
    
  }
}
*/

/*
void BigBubble::exeAppear() {
  if (MR::isFirstStep(this)) {

    _180 = mPosition;
    MR::invalidateHitSensors(this);
    MR::getRandomVector(&_1B0, 0.03f);
    MR::startSound(this, "SE_OJ_BIG_BUBBLE_APPEAR", -1, -1);
    setScale(MR::calcNerveValue(this, 30, 0.01f, _218));
    getBaseRadius();
    TVec3f vec;
    vec *= 0;
    TVec3f vec2;
    if (tryAppearEnd()) {
      MR::onCalcGravity(this);
      MR::validateHitSensors(this);
      MR::invalidateHitSensor(this, "ride");
    }
  }
}
*/

BigBubble::~BigBubble() {}

MtxPtr BigBubble::getBaseMtx() const { 
  return (MtxPtr)&_118;
}
