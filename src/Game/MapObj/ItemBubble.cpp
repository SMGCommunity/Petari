#include "Game/MapObj/ItemBubble.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/StarPiece.hpp"
#include "Game/Util.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

namespace {
    const f32 cHitSensorRadius = 120.f;
    const f32 cSwingRange = 120.0f;
    const f32 cShootStarSpeed = 24.0f;
};  // namespace

namespace NrvItemBubble {
    NEW_NERVE(ItemBubbleNrvWait, ItemBubble, Wait);
    NEW_NERVE(ItemBubbleNrvBreak, ItemBubble, Break);
};  // namespace NrvItemBubble

void FORCE_OPERATOR() {
    TVec3f vec;
    vec *= 1.0f;
}

ItemBubble::ItemBubble(const char* pName) : LiveActor(pName), _90(nullptr), _94(nullptr) {
    _8C = 0.0f;
}

void ItemBubble::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    if (MR::useStageSwitchReadAppear(this, rIter))
        MR::syncStageSwitchAppear(this);
    initModelManagerWithAnm("ItemBubble", nullptr, false);
    MR::connectToSceneMapObj(this);
    mScale.mult(1.2f);
    PSMTXScale(_9C, mScale.x, mScale.y, mScale.z);
    MR::setMtxTrans(_9C, mPosition.x, mPosition.y, mPosition.z);
    PSMTXCopy(_9C, _CC);
    _108 = mPosition;
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, mScale.x * ::cHitSensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvItemBubble::ItemBubbleNrvWait::sInstance);

    s32 itemType = 0;
    s32 itemCount = 1;
    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg0WithInit(rIter, &itemType);
        MR::getJMapInfoArg1WithInit(rIter, &itemCount);
    }

    if (itemCount < 1)
        itemCount = 1;

    _90 = new RotPartsModel*[itemCount];

    if (itemType == 1)
        _94 = new StarPiece*[itemCount];

    for (u32 i = 0; i < itemCount; i++) {
        switch (itemType) {
        case 0:
            _90[i] = new RotPartsModel(this, "ダミーコイン", "Coin", nullptr, 15, false);
            _90[i]->initWithoutIter();

            switch (itemCount) {
            case 1:
                _90[i]->initFixedPosition(_CC, TVec3f(0.0f, -67.379997f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
                continue;
            case 2:
                switch (i) {
                case 0:
                    _90[i]->initFixedPosition(_CC, TVec3f(-55.66f, -92.290001f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
                    continue;
                case 1:
                    _90[i]->initFixedPosition(_CC, TVec3f(55.66f, -43.950001f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
                    continue;
                }
                continue;
            case 3:
                switch (i) {
                case 0:
                    _90[i]->initFixedPosition(_CC, TVec3f(0.0f, -19.040001f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
                    continue;
                case 1:
                    _90[i]->initFixedPosition(_CC, TVec3f(-55.66f, -130.37f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
                    continue;
                case 2:
                    _90[i]->initFixedPosition(_CC, TVec3f(67.379997f, -130.37f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
                    continue;
                }
                continue;
            }
            break;
        case 1:
            _94[i] = new StarPiece("アイテムバブルピース");
            _94[i]->initWithoutIter();
            _90[i] = new RotPartsModel(this, "スターピース", "StarPiece", nullptr, 13, true);
            MR::initDLMakerMatColor0(_90[i], "StarPiece1_v", &_94[i]->mColor);
            MR::newDifferedDLBuffer(_90[i]);
            _90[i]->initWithoutIter();

            switch (itemCount) {
            case 1:
                _90[i]->initFixedPosition(_CC, TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
                continue;
            }
            break;
        default:
            _90[i] = nullptr;
        }
    }

    mItemCount = itemCount;
    mItemType = itemType;

    if (mItemType == 0)
        MR::declareCoin(this, mItemCount);

    if ((itemType == 0 && itemCount > 3) || (itemType == 1 && itemCount > 1)) {
        TVec3f vec3;
        TVec3f vec(0.0f, 60.0f, 0.0f);
        TVec3f vec2;
        vec2.zero();

        TRot3f mtx;
        PSMTXRotRad(mtx, (char)90, TWO_PI / static_cast< f32 >(itemCount));
        if (itemType == 0)
            vec3.set< f32 >(0.0f, -60.0f, 0.0f);
        else
            vec3.zero();

        for (u32 i = 0; i < itemCount; i++) {
            if (_90[i] == nullptr)
                continue;

            _90[i]->initFixedPosition(_CC, vec + vec3, vec2);
            PSMTXMultVec(mtx, vec, vec);
        }
    }

    mUseRail = false;
    if (MR::isValidInfo(rIter)) {
        mUseRail = MR::isConnectedWithRail(rIter);
        if (mUseRail) {
            initRailRider(rIter);
            MR::initAndSetRailClipping(&_FC, this, 100.0f, 500.0f);
        }
    }

    mRailSpeed = 1.0f;
    makeActorAppeared();
    MR::setClippingFar100m(this);
}

void ItemBubble::initAfterPlacement() {
    TVec3f grav;
    MR::calcGravityVector(this, &grav, nullptr, 0);
    TPos3f mtx;
    MR::makeMtxRotate(mtx, mRotation);
    TVec3f vec;
    mtx.getZDir(vec);

    if (mUseRail)
        MR::moveCoordAndTransToNearestRailPos(this);
}

void ItemBubble::appear() {
    LiveActor::appear();
    MR::offSwitchDead(this);
}

void ItemBubble::kill() {
    if (MR::isValidSwitchDead(this))
        MR::onSwitchDead(this);

    if (mItemCount != 0) {
        TVec3f grav;
        MR::calcGravityVector(this, &grav, nullptr, 0);
        for (u32 i = 0; i < mItemCount; i++) {
            if (_90[i] == nullptr)
                continue;

            switch (mItemType) {
            case 0:
                MR::appearCoinFix(this, getRotPartsPosition(i), 1);
                break;
            case 1:
                StarPiece* piece = getStarPiece(i);
                piece->launch(getRotPartsPosition(i), (-grav).multInLine(::cShootStarSpeed), false, false);
                MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
                break;
            }
        }
    }

    LiveActor::kill();
}

void ItemBubble::calcAndSetBaseMtx() {
    TVec3f camPos = MR::getCamPos();
    camPos.sub(mPosition);

    if (MR::isNearZero(camPos))
        return;

    MR::normalize(&camPos);
    TVec3f YDir(MR::getCamYdir());
    TVec3f camcross(YDir.cross(camPos));

    if (MR::isNearZero(camcross)) {
        return;
    }

    MR::normalize(&camcross);
    reinterpret_cast< TPos3f* >(getBaseMtx())->setXYZDir(camcross, YDir, camPos);
    reinterpret_cast< TPos3f* >(getBaseMtx())->setTrans(mPosition);
}

void ItemBubble::exeWait() {
    if (MR::isFirstStep(this)) {
        // Useless...
    }

    _8C = MR::sin(2.0f * ((static_cast< f32 >(getNerveStep()) / ::cSwingRange) * PI));

    if (mUseRail) {
        if (MR::isRailReachedGoal(this))
            MR::reverseRailDirection(this);

        f32 speed;
        if (MR::getCurrentRailPointArg0WithInit(this, &speed))
            mRailSpeed = speed;

        MR::moveCoordAndFollowTrans(this, mRailSpeed);
    }

    PSMTXScale(_9C, mScale.x, mScale.y, mScale.z);
    PSMTXConcat(_9C, MR::tmpMtxRotZDeg(mRotation.z), _9C);

    mPosition = _108 + TVec3f(0.0f, 1.0f, 0.0f).multInLine2(_8C).multInLine2(30.0f);

    MR::setMtxTrans(_9C, mPosition.x, mPosition.y, mPosition.z);
    if (mUseRail) {
        PSMTXCopy(_9C, _CC);
    } else {
        MR::blendMtx(_CC, _9C, 0.1f, _CC);
    }
    for (u32 i = 0; i < mItemCount; i++) {
        if (_90[i] == nullptr)
            continue;

        _90[i]->mRotation.x = 0.0f;
        _90[i]->mRotation.z = 0.0f;
        _90[i]->mRotation.y += 8.0f;
    }
}

void ItemBubble::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ITEM_BUBBLE_BREAK");
        MR::emitEffect(this, "BubbleBreak");
        kill();
    }
}

bool ItemBubble::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        setNerve(&NrvItemBubble::ItemBubbleNrvBreak::sInstance);
        return true;
    }
    return false;
}

bool ItemBubble::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvItemBubble::ItemBubbleNrvBreak::sInstance);
        return true;
    }
    return false;
}

ItemBubble::~ItemBubble() {
}
