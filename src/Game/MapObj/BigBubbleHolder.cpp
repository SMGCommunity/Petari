#include "Game/MapObj/BigBubbleHolder.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BigBubbleDrawer.hpp"
#include "Game/MapObj/BigBubbleMoveLimitter.hpp"
#include "Game/Ride/BigBubble.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    BigBubbleHolder* getBigBubbleHolder() {
        return MR::getSceneObj< BigBubbleHolder >(SceneObj_BigBubbleHolder);
    }
};  // namespace

BigBubbleHolder::BigBubbleHolder(const char* pName) : NameObj(pName), mDrawer(nullptr), mNumLimitters(0), mNumBubbles(0), mIsMarioBinded(false) {
    for (s32 idx = 0; idx < 16; idx++) {
        mBubbles[idx] = nullptr;
        mDrawBubbles[idx] = nullptr;
    }

    for (s32 idx = 0; idx < 64; idx++) {
        mLimitters[idx] = nullptr;
    }

    mDrawer = new BigBubbleDrawer();
    MR::registerPreDrawFunction(MR::Functor_Inline(&MR::setUpBigBubbleDraw), MR::DrawType_BigBubble);
    MR::connectToScene(this, MR::MovementType_EnemyDecoration, -1, -1, MR::DrawType_BigBubble);
}

void BigBubbleHolder::movement() {
    mIsMarioBinded = false;
    for (s32 idx = 0; idx < 16; idx++) {
        if (mBubbles[idx]->isBindMario()) {
            mIsMarioBinded = true;
            break;
        }
    }
}

void BigBubbleHolder::draw() const {
    for (s32 idx = 0; idx < mNumBubbles; idx++) {
        mDrawBubbles[idx]->draw();
    }
}

void BigBubbleHolder::addMoveLimitter(BigBubbleMoveLimitter* pLimitter) {
    for (s32 idx = 0; idx < mNumLimitters; idx++) {
        if (mLimitters[idx]->getLimitterID() == pLimitter->getLimitterID()) {
            return;
        }
    }
    mLimitters[mNumLimitters++] = pLimitter;
}

BigBubble* BigBubbleHolder::generateBigBubble(LiveActor* pHost, const TVec3f& rPos, const TVec3f& rUp, f32 volume, bool isObstruct, s32 actionType,
                                              s32 shapeType, s32 limitterID) {
    BigBubbleMoveLimitter* limitter = nullptr;
    if (limitterID >= 0) {
        limitter = getMoveLimitter(limitterID);
    }

    for (s32 idx = 0; idx < 16; idx++) {
        if (MR::isDead(mBubbles[idx])) {
            mBubbles[idx]->setHost(pHost);
            mBubbles[idx]->generate(rPos, rUp, volume, isObstruct, actionType, shapeType, limitter);

            return mBubbles[idx];
        }
    }

    return nullptr;
}

BigBubbleMoveLimitter* BigBubbleHolder::getMoveLimitter(s32 limitterID) const {
    for (s32 idx = 0; idx < mNumLimitters; idx++) {
        if (mLimitters[idx]->getLimitterID() == limitterID) {
            return mLimitters[idx];
        }
    }
    return nullptr;
}

void BigBubbleHolder::sortDrawOrder() {
    mNumBubbles = 0;
    for (s32 idx = 0; idx < 16; idx++) {
        if (mBubbles[idx]->isDraw()) {
            bool skipSort;
            s32 other = mNumBubbles;
            for (; other > 0; other--) {
                if (!mDrawBubbles[other - 1]->isMerged() && mBubbles[idx]->isMerged()) {
                    skipSort = false;
                } else if (mDrawBubbles[other - 1]->isMerged() && !mBubbles[idx]->isMerged()) {
                    skipSort = true;
                } else {
                    skipSort = mDrawBubbles[other - 1]->getDrawZ() < mBubbles[idx]->getDrawZ();
                }

                if (!skipSort) {
                    break;
                }

                mDrawBubbles[other] = mDrawBubbles[other - 1];
            }
            mDrawBubbles[other] = mBubbles[idx];
            mNumBubbles++;
        }
    }
}

void MR::setUpBigBubbleDraw() {
    BigBubbleHolder* holder = ::getBigBubbleHolder();
    holder->sortDrawOrder();
    holder->mDrawer->setUpDraw();
}

void MR::createBigBubbleHolder() {
    MR::createSceneObj(SceneObj_BigBubbleHolder);
}

void MR::limitBigBubblePosition(TVec3f* pPos, f32 f1, s32 limitterID) {
    ::getBigBubbleHolder()->getMoveLimitter(limitterID)->limitPosition(pPos, f1);
}

void MR::addBigBubbleMoveLimitter(BigBubbleMoveLimitter* pLimitter) {
    ::getBigBubbleHolder()->addMoveLimitter(pLimitter);
}

BigBubble* MR::generateBigBubble(LiveActor* pHost, const TVec3f& rPos, const TVec3f& rUp, f32 volume, bool isObstruct, s32 actionType, s32 shapeType,
                                 s32 limitterID) {
    return ::getBigBubbleHolder()->generateBigBubble(pHost, rPos, rUp, volume, isObstruct, actionType, shapeType, limitterID);
}

bool MR::isPlayerBindedBigBubble() {
    if (!MR::isExistSceneObj(SceneObj_BigBubbleHolder)) {
        return false;
    }

    return ::getBigBubbleHolder()->mIsMarioBinded;
}

void BigBubbleHolder::init(const JMapInfoIter& rIter) {
    for (s32 idx = 0; idx < 16; idx++) {
        mBubbles[idx] = new BigBubble("オオアワ[共有]");
        mBubbles[idx]->initWithoutIter();
    }
}
