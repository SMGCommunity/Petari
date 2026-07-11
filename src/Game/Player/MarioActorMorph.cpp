#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/ModelHolder.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"

void MarioActor::setPlayerMode(u8 playerMode, bool myBool) {
    if (mPlayerMode == (u16)playerMode) {
        if (playerMode == 1) {
            _3DC = mConst->getTable()->mMetalHoldTime;
            MR::startSubBGM("BGM_MUTEKI_A", false);
        }

        if (playerMode == 9) {
            _3DC = mConst->getTable()->mTornadoHoldTime;
        }

        if (playerMode == 3) {
            _3DC = mConst->getTable()->mIceModeTime;
            MR::startSubBGM("BGM_ICE_A", false);
        }

        if (playerMode == 2) {
            _3DC = mConst->getTable()->mFireModeTime;
            MR::startSubBGM("BGM_FIRE_A", false);
        }

        if (playerMode == 7) {
            _3DC = 3600;
            MR::startStageBGM("BGM_FLYING_A", false);
        }

        if (playerMode != 0) {
            playEffect("アイテム再ゲット");
        }

        return;
    }

    if (mMario->isStatusActive(MarioStatus_Foo)) {
        mMario->closeStatus(nullptr);
    }

    _3DC = 0;
    _A6E = false;

    switch (mPlayerMode) {
    case 5:
        mMario->endRabbitMode();

        MR::startBtp(this, "ColorChange");
        MR::setBtpFrameAndStop(this, 0.0f);
        break;

    case 6:
        MR::deleteEffect(_9A4, "Glow");

        if (playerMode == 0) {
            MR::offCalcAnim(_9A4);
        } else {
            _9A4->kill();
        }

        _483 = false;

        mMario->resetTeresaMode();
        break;

    case 2:
        MR::startBtp(this, "ColorChange");
        MR::setBtpFrameAndStop(this, 0.0f);

        MR::stopSubBGM(30);
        break;

    case 4:
        stopAnimation("ハチ飛行中");

        if (!mMario->isSwimming()) {
            MR::getGameSceneLayoutHolder()->changeLifeMeterModeGround();
        }

        _9E4->kill();
        _9E8->kill();
        break;

    case 3:
        stopEffect("アイス中");

        changeHandMaterial();

        if (mMario->isStatusActive(MarioStatus_Skate)) {
            mMario->closeStatus(nullptr);
        }

        MR::stopSubBGM(30);
        break;

    case 1:
        stopEffect("無敵中");

        _A6E = false;

        MR::stopSubBGM(30);

        changeHandMaterial();
        break;

    case 7:
        MR::startCurrentStageBGM();
        break;
    }

    _3D6 = mPlayerMode;
    mPlayerMode = playerMode;

    switch (playerMode) {
    case 9:
        _3DC = mConst->getTable()->mTornadoHoldTime;

        MR::startBtp(this, "TornadoElement");

        if (isJumping()) {
            changeAnimation("エレメントゲット", nullptr);
        } else {
            changeAnimation("エレメントゲット接地中", nullptr);
        }
        break;

    case 1:
        _3DC = mConst->getTable()->mMetalHoldTime;

        playEffect("無敵中");

        MR::startSubBGM("BGM_MUTEKI_A", false);

        changeHandMaterial();

        MR::stopBrk(_9C8);
        MR::stopBrk(_A50);
        MR::stopBrk(_A54);

        MR::startBrk(_9C8, "InvincibleMario");
        MR::startBrk(_A50, "InvincibleMarioHandL");
        MR::startBrk(_A54, "InvincibleMarioHandR");

        MR::removeAllClingingKarikari();
        break;

    case 0:
        MR::startBtp(this, "ElementEnd");

        if (myBool) {
            switch (_3D6) {
            case 4:
            case 5:
            case 6:
                playSound("呪い解除", -1);
                break;

            default:
                playSound("変身解除", -1);
                break;
            }
        }

        _3DC = 0;
        mMario->_76C = 0;

        if (mMario->isSwimming()) {
            if (_468 == 0) {
                changeAnimation(nullptr, "水泳基本");
            }
        } else if (isJumping()) {
            changeAnimation(nullptr, "落下");
        } else {
            changeAnimation(nullptr, "基本");
        }

        if (_4A4 != nullptr) {
            _4A4->mHost->kill();
            _4A4 = nullptr;
        }
        break;

    case 5:
        mMario->startRabbitMode();
        rushDropThrowMemoSensor();
        break;

    case 6:
        _9A4->appear();

        MR::onCalcAnim(_9A4);

        runTeresaBaseAnimation();

        MR::startBrk(_9A4, "Erase");
        MR::setBrkFrameAndStop(_9A4, 0.0f);

        _483 = true;

        _9A8 = 0.0f;
        _9AC = 0.0f;

        rushDropThrowMemoSensor();

        mMario->startTeresaMode();

        _9A4->calcAnim();

        MR::emitEffect(_9A4, "Glow");
        MR::emitEffect(_9A4, "Appear");
        break;

    case 2:
        MR::startSubBGM("BGM_FIRE_A", false);

        _3DC = mConst->getTable()->mFireModeTime;
        break;

    case 3:
        playEffect("アイス中");

        MR::startSubBGM("BGM_ICE_A", false);

        changeHandMaterial();

        _3DC = mConst->getTable()->mIceModeTime;
        break;

    case 4:
        MR::getGameSceneLayoutHolder()->changeLifeMeterModeBee();

        rushDropThrowMemoSensor();

        _9E4->appear();
        MR::showModel(_9E4);

        mMario->_402 = mConst->getTable()->mAirWalkTime;
        mMario->_42A = 0;

        _9E8->appear();
        _9F0 = false;

        MR::startBck(_9E8, "Wait", nullptr);
        MR::startBva(_9E8, "Wait");

        MR::stopBtk(_9E8);
        break;

    case 7:
        MR::startStageBGM("BGM_FLYING_A", false);
        _3DC = 3600;
        break;
    }

    if (mPlayerMode == 0) {
        if (mTransforming) {
            MR::endDemo(this, "マリオ変身");
            mTransforming = false;
        }

        if (myBool) {
            _3D8 = 30;
            mMario->startPadVib("マリオ[変身解除]");
        } else {
            _3D8 = 2;
        }

        playEffect("変身解除");
    } else {
        if (_3D8 == 0 || _3D6 == 0) {
            mPowerupCollected = true;
        } else {
            _3D8 = 64;

            MR::stopAnimFrame(this);

            playEffect("変身");

            mMario->startPadVib("マリオ[変身]");

            mPowerupCollected = false;
        }

        _945 = 0;
        _946 = 0;

        _6D4 = 0.0f;
        _6D8 = 0.0f;

        _94C = 0;
        _94E = 0;

        updateFairyStar();
    }

    updateHandAtMorph();
}

void MarioActor::resetPlayerModeOnDamage() {
    if (isActionOk("ダメージ解除")) {
        setPlayerMode(0, true);
    }
}

void MarioActor::resetPlayerModeOnNoDamage() {
    if (isActionOk("ノーダメージ解除")) {
        setPlayerMode(0, true);
    }
}

void MarioActor::updatePlayerMode() {
    if (_3DC == 0) {
        return;
    }

    _3DC--;

    if (_3DC != 0) {
        return;
    }

    setPlayerMode(0, true);
}

struct myStruct {
    const char* _0[9];
    u32 _24;
};

static myStruct cMorphStringTable[] = {{"DieBlackHole", 0, 0, 0, "DieBlackHoleBee", 0, 0, 0, "DieBlackHoleLuigi", 0}, {0}};

void MarioActor::touchSensor(HitSensor* pSensor) {
    switch (pSensor->mType) {
    case ACTMES_GROUP_ATTACK:
        if (MR::isExistMapCollision(_2A0, pSensor->mPosition - _2A0)) {
            return;
        }

        mMario->touchWater();
    }

    if (MR::isSensorEnemy(pSensor) && mMario->isStatusActive(MarioStatus_Bury)) {
        mMario->closeStatus(nullptr);
    }
}

void MarioActor::initMorphStringTable() {
    for (myStruct* item = cMorphStringTable;; item++) {
        if (item->_0[0] == nullptr) {
            break;
        }

        item->_24 = MR::getHashCode(item->_0[0]);
    }
}

// const char* MarioActor::changeMorphString(const char* pName) const {}
