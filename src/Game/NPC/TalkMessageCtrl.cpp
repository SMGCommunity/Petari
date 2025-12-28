#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/AreaObj/MessageArea.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/NPC/TalkNodeCtrl.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/MessageHolder.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"

TalkMessageCtrl::~TalkMessageCtrl() {}

void TalkMessageCtrl::createMessage(const JMapInfoIter& rIter, const char* pName) {
    mNodeCtrl = new TalkNodeCtrl();
    mNodeCtrl->createFlowNode(this, rIter, pName, &mCameraInfo);

    if (mNodeCtrl->mCurrentNode != nullptr) {
        mAlreadyDoneFlags = MR::setupAlreadyDoneFlag((const char*)mNodeCtrl->_0, rIter, &_3C);
    } else {
        _3C = 1;
    }
}

bool isNodeEventContinue(const MessageSystem::FlowNodeEvent* pArg) {
    return pArg->mEventType <= 4 && (1 << pArg->mEventType & 0x15);
}

void TalkMessageCtrl::createMessageDirect(const JMapInfoIter& rIter, const char* pName) {
    mNodeCtrl = new TalkNodeCtrl();
    mNodeCtrl->createFlowNodeDirect(this, rIter, pName, &mCameraInfo);
    _3C = 1;
}

// this functions indicates that TalkNode's mNextIdx is 4 bytes, but it is 2.
bool TalkMessageCtrl::rootNodeEve() {
    TalkNodeCtrl* nodeCtrl = mNodeCtrl;
    TalkNode* nodeEvent = nodeCtrl->getCurrentNodeEvent();
    if (nodeEvent == nullptr) {
        return true;
    }

    u8 groupID = nodeEvent->mGroupID;
    if (groupID <= 1) {
        if (mEventFunc == nullptr) {
            return true;
        }

        if (mEventFunc->operator()(nodeEvent->mNextIdx) == nullptr) {
            return false;
        }
    } else if (groupID == 4) {
        if (mAnimeFunc == nullptr) {
            return true;
        }

        if (mAnimeFunc->operator()(nodeEvent->mNextIdx) == nullptr) {
            return false;
        }
    } else if (groupID == 3) {
        rootNodePst();
        return true;
    } else if (groupID == 5) {
        MR::onSwitchA(mHostActor);
    } else if (groupID == 6) {
        MR::onSwitchB(mHostActor);
    } else if (groupID == 7) {
        if (mKillFunc->operator()(nodeEvent->mNextIdx) == nullptr) {
            return false;
        }
    }

    bool cond = nodeCtrl->isExistNextNode();
    rootNodePst();

    if (cond && nodeCtrl->getCurrentNodeEvent()) {
        return false;
    }

    return true;
}
void TalkMessageCtrl::rootNodePre(bool arg) {
    TalkNodeCtrl* control = mNodeCtrl;
    control->resetTempFlowNode();
    while (true) {
        TalkNode* branch = control->getCurrentNodeBranch();

        if (branch == nullptr) {
            if (arg) {
                return;
            }
            if (!isCurrentNodeContinue()) {
                return;
            }

            control->forwardFlowNode();
            continue;
        }

        bool cond = false;

        switch (branch->mIndex) {
        case 0:
            return;
        case 1:
            if (mBranchFunc == nullptr) {
                cond = true;
                break;
            }
            cond = mBranchFunc->operator()(branch->mNextIdx);
            break;
        case 2:
            cond = MR::isNearPlayerAnyTime(mHostActor, mTalkDistance);
            break;
        case 3:
            cond = MR::isOnSwitchA(mHostActor);
            break;
        case 4:
            cond = MR::isOnSwitchB(mHostActor);
            break;
        case 5:
            cond = MR::isPlayerElementModeNormal();
            break;
        case 6:
            cond = MR::isPlayerElementModeBee();
            break;
        case 7:
            cond = MR::isPlayerElementModeTeresa();
            break;
        case 8:
            cond = MR::isStageStatePowerStarAppeared();
            break;
        case 9:
            cond = _3C != 0;
            break;
        case 10:
            cond = MR::isPlayerLuigi();
            break;
        case 11:
            cond = MR::isTimeKeepDemoActive();
            break;
        case 12:
            cond = MR::isOnMessageAlreadyRead(branch->mNextIdx);
            break;
        case 13:
            cond = MR::isMsgLedPattern();
            break;
        case 14:
            cond = TalkFunction::getBranchAstroGalaxyResult(branch->mNextIdx);
            break;
        }

        if (cond) {
            control->forwardCurrentBranchNode(true);
            continue;
        }

        control->forwardCurrentBranchNode(false);
    }
}

void TalkMessageCtrl::rootNodePst() {
    TalkNodeCtrl* ctrl = mNodeCtrl;
    if (!ctrl->isExistNextNode()) {
        ctrl->resetFlowNode();
    } else {
        ctrl->forwardFlowNode();
    }

    ctrl->recordTempFlowNode();
}

bool TalkMessageCtrl::isCurrentNodeContinue() const {
    TalkNode* nodeEvent = mNodeCtrl->getCurrentNodeEvent();

    if (nodeEvent == nullptr) {
        return false;
    }

    return isNodeEventContinue((MessageSystem::FlowNodeEvent*)nodeEvent);
}

void TalkMessageCtrl::rootNodeSel(bool a1) {
    TalkNodeCtrl* ctrl = mNodeCtrl;
    ctrl->forwardFlowNode();

    if (a1) {
        ctrl->forwardCurrentBranchNode(true);
    } else {
        ctrl->forwardCurrentBranchNode(false);
    }

    ctrl->recordTempFlowNode();
}

void TalkMessageCtrl::registerBranchFunc(const TalkMessageFuncBase& rFunc) {
    mBranchFunc = rFunc.clone();
}

void TalkMessageCtrl::registerEventFunc(const TalkMessageFuncBase& rFunc) {
    mEventFunc = rFunc.clone();
}

void TalkMessageCtrl::registerAnimeFunc(const TalkMessageFuncBase& rFunc) {
    mAnimeFunc = rFunc.clone();
}

void TalkMessageCtrl::registerKillFunc(const TalkMessageFuncBase& rFunc) {
    mKillFunc = rFunc.clone();
}

void TalkMessageCtrl::readMessage() {
    if (mIsOnReadNodeAuto) {
        mNodeCtrl->readMessage();
    }

    if (!_3C) {
        MR::updateAlreadyDoneFlag(mAlreadyDoneFlags, 1);
    }
}

bool TalkMessageCtrl::isSelectYesNo() const {
    TalkNode* node = mNodeCtrl->getNextNodeBranch();

    if (node == nullptr || node->mIndex != 0) {
        return false;
    }

    switch (node->mNextIdx) {
    case 16:
        return false;
    case 14:
        return false;
    }

    return true;
}

u32 TalkMessageCtrl::getMessageID() const {
    TalkNodeCtrl* ctrl = mNodeCtrl;
    if (ctrl->getCurrentNodeMessage() != nullptr) {
        return ctrl->getCurrentNodeMessage()->mIndex;
    } else {
        return ctrl->mCurrentNodeIdx;
    }
}

bool TalkMessageCtrl::requestTalk() {
    return TalkFunction::requestTalkSystem(this, false);
}

bool TalkMessageCtrl::requestTalkForce() {
    return TalkFunction::requestTalkSystem(this, true);
}

void TalkMessageCtrl::startTalk() {
    TalkFunction::startTalkSystem(this, false, true, true);
}

void TalkMessageCtrl::startTalkForce() {
    TalkFunction::startTalkSystem(this, true, true, true);
}

void TalkMessageCtrl::startTalkForcePuppetable() {
    TalkFunction::startTalkSystem(this, true, true, false);
}

void TalkMessageCtrl::startTalkForceWithoutDemo() {
    TalkFunction::startTalkSystem(this, true, false, true);
}

void TalkMessageCtrl::startTalkForceWithoutDemoPuppetable() {
    TalkFunction::startTalkSystem(this, true, false, false);
}

void TalkMessageCtrl::endTalk() {
    TalkFunction::endTalkSystem(this);
}

// regswap between r30 and r31
bool TalkMessageCtrl::isNearPlayer(const TalkMessageCtrl* pCtrl) {
    if (pCtrl == nullptr) {
        return true;
    }

    TVec3f* playerPos = MR::getPlayerPos();
    TVec3f* hostPos = &mHostActor->mPosition;
    TVec3f* ctrlHostPos = &pCtrl->mHostActor->mPosition;

    return hostPos->squared(*playerPos) < ctrlHostPos->squared(*playerPos);
}

void TalkMessageCtrl::startCamera(s32 a1) {
    TalkMessageInfo* inf = &mNodeCtrl->mMessageInfo;
    if (inf->isCameraNormal()) {
        MR::startNPCTalkCamera(this, mHostActor->getBaseMtx(), 1.0f, a1);
    } else if (inf->isCameraEvent()) {
        if (inf->_4) {
            mCameraInfo->mCameraSetID = inf->_4;
            MR::startMultiActorCameraNoTarget(mHostActor, mCameraInfo, "会話", a1);
        }
    }
}

const char* TalkMessageCtrl::getBranchID() const {
    TalkNode* node = mNodeCtrl->getNextNodeBranch();

    if (node == nullptr || node->mIndex != 0) {
        return nullptr;
    }

    const char* result = nullptr;

    switch (node->mNextIdx) {
    case 0:
        result = "PenguinRace";
        break;
    case 1:
        result = "SwimmingSchool";
        break;
    case 2:
        result = "PenguinRace";
        break;
    case 3:
        result = "BombTimeAttackLv1";
        break;
    case 4:
        result = "PhantomTeresaRacer";
        break;
    case 5:
        result = "BombTimeAttackLv2";
        break;
    case 6:
        result = "TrialSurfingCoach";
        break;
    case 7:
        result = "TrialSurfingHowTo";
        break;
    case 8:
        result = "DeathPromenadeTeresaRacer";
        break;
    case 9:
        result = "RosettaFinalBattle";
        break;
    case 10:
        result = "CometTico";
        break;
    case 11:
        result = "TransformTico";
        break;
    case 12:
        result = "ChallengeSurfingCoach";
        break;
    case 13:
        result = "TicoShopExchange";
        break;
    case 14:
        result = "TicoShopWhich";
        break;
    case 15:
        result = "KinopioPurple";
        break;
    case 16:
        result = "CometTicoTell";
        break;
    case 17:
        result = "TrialTamakoroHowTo";
        break;
    case 18:
        result = "KnockOnTheDoor";
        break;
    case 19:
        result = "LedPattern";
        break;
    }

    return result;
}

bool TalkFunction::isShortTalk(const TalkMessageCtrl* pCtrl) {
    return pCtrl->mNodeCtrl->mMessageInfo.isShortTalk();
}

bool TalkFunction::isComposeTalk(const TalkMessageCtrl* pCtrl) {
    return pCtrl->mNodeCtrl->mMessageInfo.isComposeTalk();
}

bool TalkFunction::isSelectTalk(const TalkMessageCtrl* pCtrl) {
    return pCtrl->mNodeCtrl->mNodeData == 0;
}

bool TalkFunction::isEventNode(const TalkMessageCtrl* pCtrl) {
    return pCtrl->mNodeCtrl->isCurrentNodeEvent();
}

void TalkFunction::onTalkStateNone(TalkMessageCtrl* pCtrl) {
    pCtrl->_18 = 0;
}

void TalkFunction::onTalkStateEntry(TalkMessageCtrl* pCtrl) {
    pCtrl->_18 = 1;
}

void TalkFunction::onTalkStateEnableStart(TalkMessageCtrl* pCtrl) {
    pCtrl->_18 = 2;
}

void TalkFunction::onTalkStateTalking(TalkMessageCtrl* pCtrl) {
    pCtrl->_18 = 3;
}

void TalkFunction::onTalkStateEnableEnd(TalkMessageCtrl* pCtrl) {
    pCtrl->_18 = 4;
}

TalkMessageInfo* TalkFunction::getMessageInfo(const TalkMessageCtrl* pCtrl) {
    return &pCtrl->mNodeCtrl->mMessageInfo;
}

const wchar_t* TalkFunction::getMessage(const TalkMessageCtrl* pCtrl) {
    return (const wchar_t*)pCtrl->mNodeCtrl->mMessageInfo._0;
}

const wchar_t* TalkFunction::getSubMessage(const TalkMessageCtrl* pCtrl) {
    return (const wchar_t*)pCtrl->mNodeCtrl->getSubMessage();
}

bool TalkMessageCtrl::inMessageArea() const {
    s8 messageInfoEnumByte = mNodeCtrl->mMessageInfo._A;
    s32 messageInfoEnum = mNodeCtrl->mMessageInfo._A;
    s32 zoneID = mZoneID;

    if (messageInfoEnumByte < 0) {
        return false;
    }

    TVec3f* playerPos = MR::getPlayerCenterPos();
    MessageArea* areaObj = (MessageArea*)MR::getAreaObj("MessageArea", *playerPos);
    if (areaObj != nullptr && messageInfoEnum == areaObj->mObjArg0 && zoneID == areaObj->mZoneID) {
        return true;
    }
    return false;
}

bool TalkMessageCtrl::isNearPlayer(float distance) const {
    if (mNodeCtrl->mMessageInfo.isNullTalk()) {
        return false;
    }

    if (mTalkDistance < 0.0f) {
        return true;
    }

    // regswap between f31 and f30
    float talkDist = mTalkDistance;
    if (distance <= 0.0f && mNodeCtrl->mMessageInfo.isComposeTalk()) {
        float scale = 2.0f;
        talkDist = distance * scale;
    }

    TVec3f v2(mHostActor->mPosition);  // 0x44

    if (mMtx != nullptr) {
        v2.set< float >(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    }

    TVec3f v3;  // 0x38
    float f2 = MR::vecKillElement(v2 - *MR::getPlayerCenterPos(), mHostActor->mGravity, &v3);

    if (__fabs(f2) < talkDist && v3.squared() < talkDist * talkDist) {
        TalkMessageInfo* info = &mNodeCtrl->mMessageInfo;
        bool cond = info->mTalkType;
        if (mIsStartOnlyFront || info->isBalloonSign()) {
            TMtx34f pos;
            pos.set(mHostActor->getBaseMtx());

            TVec3f v3;  // 0x2C
            f32 setZ = pos[2][2];
            f32 setY = pos[1][2];
            f32 setX = pos[0][2];
            v3.set< float >(setX, setY, setZ);

            TVec3f v4;  // 0x20
            setZ = pos[2][3];
            setY = pos[1][3];
            setX = pos[0][3];
            v4.set< float >(setX, setY, setZ);

            f32 f3 = v4.z;

            f2 = MR::vecKillElement((*MR::getPlayerPos() - v4), v3, &v4);

            if (f2 > 0.0f) {
                return true;
            }
        } else {
            return true;
        }
    }
    return inMessageArea();
}

void TalkMessageCtrl::updateBalloonPos() {
    TMtx34f pos;
    pos.set(mHostActor->getBaseMtx());

    if (mMtx != nullptr) {
        pos.set(mMtx);
    }

    TPos3f* tPos = MR::getCameraInvViewMtx();
    TPos3f* tPos2;
    // loop

    TVec3f nullVec(0.0f, 0.0f, 0.0f);
    MtxPtr newMatrix;
    MR::setMtxTrans(newMatrix, nullVec.x, nullVec.y, nullVec.z);
    MR::addTransMtxLocal(newMatrix, _2C);

    TVec3f v3;
    v3.set(newMatrix[3][0], newMatrix[3][1], newMatrix[3][2]);

    TVec3f v4(newMatrix[2][0], newMatrix[2][1], newMatrix[2][2]);

    MR::calcScreenPosition(&_1C, v4 + v3);
}

TalkMessageCtrl::TalkMessageCtrl(LiveActor* pHost, const TVec3f& arg2, MtxPtr pArg3)
    : NameObj("会話制御"), mHostActor(pHost), mNodeCtrl(nullptr), mZoneID(-1), _1C(0.0f, 0.0f, 0.0f), _2C(arg2), mTalkDistance(240.0f), _3C(0),
      mIsOnReadNodeAuto(true), mAlreadyDoneFlags(0), mIsStartOnlyFront(false), mCameraInfo(nullptr), mBranchFunc(nullptr), mEventFunc(nullptr),
      mAnimeFunc(nullptr), mKillFunc(nullptr), mMtx(pArg3), mIsOnRootNodeAuto(false), _18(false), mTagArg(nullptr, CustomTagArg::Type_Uninitialized) {
    mTagArg.mArgType = CustomTagArg::Type_Uninitialized;
    mTagArg.mIntArg = 0;
    MR::createSceneObj(0x19);
    TalkFunction::registerTalkSystem(this);
    mZoneID = MR::getCurrentPlacementZoneId();
}
