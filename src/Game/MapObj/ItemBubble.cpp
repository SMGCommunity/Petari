#include "Game/MapObj/ItemBubble.hpp"
#include "Game/MapObj/StarPiece.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MathUtil.hpp"

namespace NrvItemBubble {
    NEW_NERVE(ItemBubbleNrvWait, ItemBubble, Wait);
    NEW_NERVE(ItemBubbleNrvBreak, ItemBubble, Break);
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
    MR::addHitSensorMapObj(this, "body", 8, mScale.x * 120.f, TVec3f(0.0f, 0.0f, 0.0f));
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
        TVec3f vec(0.0f, 60.0f, 0.0f);
        TVec3f vec2;
        vec2.zero();

        TRot3f mtx;
        PSMTXRotRad(mtx, (char)90, TWO_PI / static_cast< f32 >(itemCount));
        TVec3f vec3;
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

ItemBubble::~ItemBubble() {}

