#include "Game/MapObj/SoundEmitter.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"



namespace {
    const  JAISoundID sSeTable[33] = {
        JAISoundID(0x1)
    };
    
}

namespace NrvSoundEmitter {
    NEW_NERVE(SoundEmitterNrvStandBy, SoundEmitter, StandBy);
    NEW_NERVE(SoundEmitterNrvPlaySound, SoundEmitter, PlaySound);
}

SoundEmitter::SoundEmitter(const char* pName) : LiveActor(pName) {
    _8C = -1;
    
}
SoundEmitter::~SoundEmitter() {

}

void SoundEmitter::init(const JMapInfoIter & rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &_8C);
    MR::connectToSceneMapObjMovement(this);
    MR::registerDemoSimpleCastAll(this);
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
    }
    initSound(8, false);
    MR::setClippingTypeSphere(this, 20000.0f);
    MR::useStageSwitchReadAppear(this, rIter);
    MR::useStageSwitchReadA(this, rIter);
    if (MR::isValidSwitchAppear(this)) {
        initNerve(&NrvSoundEmitter::SoundEmitterNrvStandBy::sInstance);
    } else {
        initNerve(&NrvSoundEmitter::SoundEmitterNrvPlaySound::sInstance);
    }
    makeActorAppeared();
}

void SoundEmitter::control() {
    if (MR::isValidSwitchA(this)) {
        if (MR::isOnSwitchA(this)) {
            kill();
        }
    }
}

void SoundEmitter::exePlaySound() {
    RailRider* rider = mRailRider;
    s32 v3 = -1;
    if (rider)
    {
        switch(_8C)
        {
            case 0xd:
            case 0x11:
            case 0x12: {
                rider->moveToNearestPos(*MR::getCameraWatchPos());
            } break;
            default: {
                rider->moveToNearestPos(MR::getCamPos());
            } break;
        }
        mPosition.x = mRailRider->mCurPos.x;
        mPosition.y = mRailRider->mCurPos.y;
        mPosition.z = mRailRider->mCurPos.z;
    }
    
    switch (_8C) {
        case 9:
        case 0xA:
        case 0xB: {
            if (MR::calcDistanceToPlayer(this) < 1000.0f) {
                return;
            }
        } break;
        case 0x16:
        case 0x17: {
            if (MR::isDemoActive("ロゼッタ状況説明デモ")) {
                return;
            }
        } break;
        case 0x1A: {
            v3 = 5;
        } break;
        case 0x1E: {
            MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
            kill();
            return;
        } break;
    }
    if (_8C >= 0) {
        MR::startLevelSound(this, sSeTable[_8C], -1, -1, v3);
    }
}

inline void SoundEmitter::exeStandBy() {
    if ( MR::isValidSwitchAppear(this)) {
        if ( MR::isOnSwitchAppear(this) ) {
            setNerve(&NrvSoundEmitter::SoundEmitterNrvPlaySound::sInstance);
        }
  }
}
