#include "Game/Screen/StarPointerDirector.hpp"
#include "Game/Screen/LayoutCoreUtil.hpp"
#include "Game/Screen/StarPointerController.hpp"
#include "Game/Screen/StarPointerGuidance.hpp"
#include "Game/Screen/StarPointerLayout.hpp"
#include "Game/System/DrawSyncManager.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JMath/JMath.hpp>
#include <revolution/gx/GXGet.h>

StarPointerDirector::StarPointerDirector()
    : mIsUpdateTransHolder(false), mIsAllowP1StarPieceShot(false), mIsAllowP2StarPieceShot(false), mControllers(nullptr),
      mStarPointerLayouts(nullptr), mTransHolder(nullptr), mPeekZ(nullptr), mGuidance(nullptr), mNozzleAimPos(0.0f, 0.0f, 0.0f) {
    mPeekZ = new StarPointerPeekZ();
    mTransHolder = new StarPointerTransformHolder();
    mControllers = new StarPointerController[StarPointerFunction::getNumStarPointer()];
    for (s32 channel = 0; channel < StarPointerFunction::getNumStarPointer(); channel++) {
        mControllers[channel].initAndSetPort(channel);
        mPeekZ->mInfos[channel] = &mControllers[channel].mInfo;
    }
}

void StarPointerDirector::update() {
    if (MR::isStarPointerModeHomeButton() || MR::isStarPointerModeErrorWindow()) {
        mIsOSPointerMode = true;
        return;
    }

    if (mIsOSPointerMode && mStarPointerLayouts != nullptr) {
        for (s32 channel = 0; channel < StarPointerFunction::getNumStarPointer(); channel++) {
            getStarPointerLayout(channel)->hideBlur();
        }
    }

    mIsOSPointerMode = false;

    if (_18 < 0) {
        _18++;
    }

    mTransHolder->movement();

    for (s32 channel = 0; channel < StarPointerFunction::getNumStarPointer(); channel++) {
        if (MR::isConnectedWPad(channel)) {
            getStarPointerController(channel)->movement(mPeekZ->mProjectionParameters, mPeekZ->mViewportParameters);
        }

        if (mStarPointerLayouts != nullptr) {
            getStarPointerLayout(channel)->movement();
            getStarPointerLayout(channel)->calcAnim();
        }
    }

    if (mGuidance != nullptr) {
        mGuidance->movement();
        mGuidance->calcAnim();
    }
}

void StarPointerDirector::draw() {
    if (mGuidance != nullptr) {
        mGuidance->draw();
    }

    if (mStarPointerLayouts != nullptr) {
        for (s32 channel = 0; channel < StarPointerFunction::getNumStarPointer(); channel++) {
            getStarPointerLayout(channel)->draw();
        }
    }
}

void StarPointerDirector::startHandPointer() {
    if (mStarPointerLayouts != nullptr) {
        mStarPointerLayouts[0].changeLayout(StarPointerKind_HandPointer);
    }
}

void StarPointerDirector::startHandPointerReactionWithCrossCursor() {
    if (mStarPointerLayouts != nullptr) {
        mStarPointerLayouts[0].changeLayout(StarPointerKind_HandPointerReactionWithCrossCursor);
    }
}

void StarPointerDirector::startFingerPointer() {
    if (mStarPointerLayouts != nullptr) {
        mStarPointerLayouts[0].changeLayout(StarPointerKind_FingerPointer);
    }
}

void StarPointerDirector::startStarPointer() {
    if (mStarPointerLayouts != nullptr) {
        mStarPointerLayouts[0].changeLayout(StarPointerKind_StarPointer);
    }
}

void StarPointerDirector::startStarPointerNozzle() {
    if (mStarPointerLayouts != nullptr) {
        mStarPointerLayouts[0].changeLayout(StarPointerKind_StarPointerNozzle);
    }
}

void StarPointerDirector::setGameSceneCameraMtx() {
    if (mIsUpdateTransHolder) {
        mTransHolder->mViewMtx.setInline(MR::getCameraViewMtx());
        mTransHolder->mProjMtx.setInline(MR::getCameraProjectionMtx());
        mTransHolder->mFovy = MR::getFovy();
    }
}

StarPointerController* StarPointerDirector::getStarPointerController(s32 channel) const {
    return &mControllers[channel];
}

StarPointerLayout* StarPointerDirector::getStarPointerLayout(s32 channel) const {
    return &mStarPointerLayouts[channel];
}

void StarPointerDirector::createLayout() {
    mStarPointerLayouts = new StarPointerLayout[StarPointerFunction::getNumStarPointer()];

    for (u32 channel = 0; channel < StarPointerFunction::getNumStarPointer(); channel++) {
        mStarPointerLayouts[channel].initWithPort(channel);
        mStarPointerLayouts[channel].mDirector = this;
    }

    mGuidance = new StarPointerGuidance("スターポインタガイダンス");
    mGuidance->initWithoutIter();
}

namespace {
    static f32 mtx_identity[3][4] = {{1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}};
};  // namespace

StarPointerTransformHolder::StarPointerTransformHolder() : mViewMtx(mtx_identity), mProjMtx(mtx_identity) {
}

void StarPointerTransformHolder::movement() {
    f32 fovyRad = PI_180 * getFovy();
    f32 tan = JMASinRadian(fovyRad * 0.5f) / JMACosRadian(fovyRad * 0.5f);

    mFocalLength = ((MR::getScreenHeight() * 0.5f) / tan);
}

StarPointerPeekZ::StarPointerPeekZ() {
    mInfos = new DpdInfo*[2];
    mToken = DrawSyncManager::sInstance->setCallback(2, 1, this);
}

void StarPointerPeekZ::setDrawSyncToken() {
    GXGetProjectionv(mProjectionParameters);
    GXGetViewportv(mViewportParameters);
    DrawSyncManager::sInstance->pushBreakPoint();
    GXSetDrawSync(mToken);
}

void StarPointerPeekZ::drawSyncCallback(u16 token) {
    for (s32 channel = 0; channel < StarPointerFunction::getNumStarPointer(); channel++) {
        if (MR::isInRange(mInfos[channel]->mPos.x, 0.0f, MR::getScreenWidth() - 1) &&
            MR::isInRange(mInfos[channel]->mPos.y, 0.0f, MR::getScreenHeight() - 1)) {
            TVec2f pos;
            MR::convertScreenPosToFrameBufferPos(&pos, mInfos[channel]->mPos);
            GXPeekZ(pos.x, pos.y, &mInfos[channel]->mZDepth);
            mInfos[channel]->mDrawReady = true;
        }
    }
}

namespace StarPointerFunction {
    bool isOnScreenEdge(const TVec2f& rPos, f32 marginX, f32 marginY) {
        if (rPos.x <= marginX) {
            return true;
        }

        if (rPos.y <= marginY) {
            return true;
        }

        if (MR::getScreenWidth() - marginX <= rPos.x) {
            return true;
        }

        if (MR::getScreenHeight() - marginY <= rPos.y) {
            return true;
        }

        return false;
    }

    bool isOnScreenEdge(s32 channel) {
        TVec2f pos;
        MR::convertPaneLocalPosToScreenPos(&pos, MR::getRootPane(getStarPointerDirector()->getStarPointerLayout(channel)), TVec2f(0.0f, 0.0f));
        return isOnScreenEdge(pos, 0.0f, 0.0f);
    }

    bool forceInsideScreenEdge(TVec2f* pPos) {
        bool forced = false;

        f32 margin = 0.0f;
        f32 width = MR::getScreenWidth() - margin;
        f32 height = MR::getScreenHeight() - margin;

        if (pPos->x < margin) {
            pPos->x = margin;
            forced = true;
        } else if (width < pPos->x) {
            pPos->x = width;
            forced = true;
        }

        if (pPos->y < margin) {
            pPos->y = margin;
            forced = true;
        } else if (height < pPos->y) {
            pPos->y = height;
            forced = true;
        }

        return forced;
    }

    StarPointerDirector* getStarPointerDirector() {
        return SingletonHolder< GameSystem >::get()->mObjHolder->mStarPointerDir;
    }

    s32 getNumStarPointer() {
        return 2;
    }

    s32 getPastPointNum(s32 channel) {
        return getStarPointerDirector()->getStarPointerController(channel)->mPastPointNum;
    }

    const TVec2f& getPastPosition(s32 channel, s32 index) {
        return getStarPointerDirector()->getStarPointerController(channel)->mPastPosition[index];
    }

    s32 getNextPastPointNum(s32 channel) {
        return getStarPointerDirector()->getStarPointerController(channel)->mNextPastPointNum;
    }

    bool canShoot(s32 channel) {
        return getStarPointerDirector()->getStarPointerLayout(channel)->mShootDisabled == false;
    }
};  // namespace StarPointerFunction
