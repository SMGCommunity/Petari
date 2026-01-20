#include "Game/MapObj/StarPieceFollowGroup.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"

namespace NrvStarPieceFollowGroup {
    NEW_NERVE(HostTypeNrvFollowToPlayer, StarPieceFollowGroup, FollowToPlayer);
    NEW_NERVE(HostTypeNrvFollowEnd, StarPieceFollowGroup, FollowEnd);
}  // namespace NrvStarPieceFollowGroup

StarPieceFollowGroup::StarPieceFollowGroup(const char* pName)
    : LiveActor(pName), mPieces(nullptr), _90(nullptr), mNumPiecesTotal(6), mNumPiecesUse(0), mRadius(350.0f), mAngle(0.0f), mTimeLimit(1200) {}

void StarPieceFollowGroup::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    if (arg0 != -1) {
        mNumPiecesTotal = arg0;
    }

    MR::getJMapInfoArg1NoInit(rIter, &mTimeLimit);
    MR::getJMapInfoArg2NoInit(rIter, &mRadius);

    initNerve(&NrvStarPieceFollowGroup::HostTypeNrvFollowEnd::sInstance);

    mPieces = new StarPiece*[mNumPiecesTotal];
    _90 = new TVec3f[mNumPiecesTotal];
    for (int i = 0; i < mNumPiecesTotal; i++) {
        mPieces[i] = new StarPiece("フォロースターピース");
        mPieces[i]->initWithoutIter();
        mPieces[i]->makeActorDead();
    }

    MR::needStageSwitchWriteA(this, rIter);
    MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &onSwitchA));
    MR::useStageSwitchReadB(this, rIter);

    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    kill();
}

bool StarPieceFollowGroup::beginFollowPieces() {
    if (MR::isValidSwitchB(this) && MR::isOnSwitchB(this)) {
        return false;
    }

    if (MR::isStageStarPieceFollowGroupLimit()) {
        if (MR::getStarPieceNum() >= 100) {
            return false;
        }

        s32 starPieceNumLeft = MR::getStarPieceNum();
        starPieceNumLeft = 100 - starPieceNumLeft;
        s32 numPiecesToUse;
        if (mNumPiecesTotal >= starPieceNumLeft) {
            numPiecesToUse = starPieceNumLeft;
        } else {
            numPiecesToUse = mNumPiecesTotal;
        }

        mNumPiecesUse = numPiecesToUse;
    } else {
        mNumPiecesUse = mNumPiecesTotal;
    }

    for (int i = 0; i < (u32)mNumPiecesUse; i++) {
        mPieces[i]->setFollowPlayerAndAppear();
    }

    placementPiece(mNumPiecesUse);
    return true;
}

void StarPieceFollowGroup::placementPiece(s32 numPlace) {
    TPos3f mtxTRS;
    MR::makeMtxTRS(mtxTRS.toMtxPtr(), this);

    TVec3f zDir;
    TVec3f xDir;

    mtxTRS.getXDirInline(xDir);
    mtxTRS.getZDirInline(zDir);

    TVec3f position(mPosition);
    f32 currentAngle = 0.0f;
    f32 angleBetweenPieces = TWO_PI / numPlace;
    for (u32 i = 0; i < numPlace; i++) {
        f32 zMag = MR::cos(currentAngle) * mRadius;
        f32 xMag = MR::sin(currentAngle) * mRadius;

        mPieces[i]->mPosition.setPS2(
            zDir.multiplyOperatorInline(zMag).addOperatorInLine(xDir.multiplyOperatorInline(xMag)).addOperatorInLine(position));
        currentAngle += angleBetweenPieces;
    }
}

void StarPieceFollowGroup::updateAxises(const TVec3f& rVec1, const TVec3f& rVec2, const TVec3f& rVec3, f32 angle) {
    TQuat4f quat;
    quat.setRotate(mLocalXAxis, rVec1, angle);
    quat.rotate(mLocalXAxis);

    quat.setRotate(mLocalZAxis, rVec2, angle);
    quat.rotate(mLocalZAxis);

    quat.setRotate(mLocalYAxis, rVec3, angle);
    quat.rotate(mLocalYAxis);
}

void StarPieceFollowGroup::followPieces() {
    TVec3f playerSideVec;
    TVec3f playerFrontVec;
    TVec3f playerUpVec;

    MR::getPlayerSideVec(&playerSideVec);
    MR::getPlayerFrontVec(&playerFrontVec);
    MR::getPlayerUpVec(&playerUpVec);

    updateAxises(playerSideVec, playerFrontVec, playerUpVec, 0.25f);

    TVec3f playerPos(*MR::getPlayerPos());
    TVec3f playerVelocity(*MR::getPlayerVelocity());

    playerPos.add(mLocalYAxis.multInLine(0.0f));
    playerPos.add(playerVelocity.multiplyOperatorInline(10.0f));

    s32 numPiecesNotFollowingAnymore = 0;
    f32 currentAngle = mAngle;
    f32 angleBetweenPieces = TWO_PI / mNumPiecesUse;
    for (int i = 0; i < mNumPiecesUse; i++) {
        StarPiece* currentPiece = mPieces[i];
        currentAngle += angleBetweenPieces;
        if (!currentPiece->isFollowing()) {
            numPiecesNotFollowingAnymore++;
            if (numPiecesNotFollowingAnymore == (mNumPiecesUse - 1)) {
                kill();
            }
            continue;
        }

        f32 cos = MR::cos(currentAngle);
        f32 sin = MR::sin(currentAngle);
        f32 cos2 = 1.0f + 0.25f * MR::cos(3.0f * currentAngle);

        TVec3f vec2(playerPos);
        TVec3f vec(0.0f, 0.0f, 0.0f);

        vec.add(mLocalXAxis.multiplyOperatorInline(cos).multiplyOperatorInline(mRadius));
        vec.add(mLocalZAxis.multiplyOperatorInline(sin).multiplyOperatorInline(mRadius));

        _90[i].set(vec);

        vec2.add(vec.multiplyOperatorInline(cos2));
        currentPiece->mVelocity.set(vec2);
        JMathInlineVEC::PSVECSubtract2(&currentPiece->mVelocity, &currentPiece->mPosition, &currentPiece->mVelocity);
        currentPiece->mVelocity.mult(1.0f);
    }
}

void StarPieceFollowGroup::allKillPieces() {
    for (int i = 0; i < (u32)mNumPiecesUse; i++) {
        StarPiece* currentPiece = mPieces[i];
        if (!MR::isDead(currentPiece)) {
            currentPiece->makeActorDead();
        }
    }
}

void StarPieceFollowGroup::onSwitchA() {
    if (beginFollowPieces()) {
        setNerve(&NrvStarPieceFollowGroup::HostTypeNrvFollowToPlayer::sInstance);
        appear();
    } else {
        kill();
    }
}

void StarPieceFollowGroup::exeFollowToPlayer() {
    if (MR::isFirstStep(this)) {
        MR::getPlayerSideVec(&mLocalXAxis);
        MR::getPlayerFrontVec(&mLocalZAxis);
        MR::getPlayerUpVec(&mLocalYAxis);
    }

    followPieces();
    mAngle += 0.025f;

    if (MR::isGreaterStep(this, mTimeLimit)) {
        setNerve(&NrvStarPieceFollowGroup::HostTypeNrvFollowEnd::sInstance);
    }
}

void StarPieceFollowGroup::exeFollowEnd() {
    bool doSomeExtraTVecStuff = false;
    if (MR::isGreaterStep(this, 20)) {
        doSomeExtraTVecStuff = true;
    }

    s32 numPiecesNotFollowingAnymore = 0;
    for (int i = 0; i < (u32)mNumPiecesUse; i++) {
        StarPiece* currentPiece = mPieces[i];
        if (!currentPiece->isFollowing()) {
            numPiecesNotFollowingAnymore++;
            if (numPiecesNotFollowingAnymore == mNumPiecesUse - 1) {
                kill();
            }
            continue;
        }

        if (doSomeExtraTVecStuff) {
            currentPiece->mVelocity.add(_90[i].multiplyOperatorInline(0.05f));
        }

        TVec3f vec2(*MR::getPlayerVelocity());
        MR::normalizeOrZero(&vec2);
        currentPiece->mVelocity.add(vec2.multiplyOperatorInline(10.0f));
    }
    
    if (MR::isGreaterStep(this, 120)) {
        kill();
        MR::offSwitchA(this);
        allKillPieces();
    }
}
