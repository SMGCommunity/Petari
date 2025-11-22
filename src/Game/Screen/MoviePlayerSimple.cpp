#include "Game/Player/MarioActor.hpp"
#include "Game/Screen/MoviePlayerSimple.hpp"
#include "Game/Screen/THPSimplePlayerWrapper.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/HomeButtonStateNotifier.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <JSystem/JKernel/JKRDisposer.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <revolution/gx/GXEnum.h>

namespace {
    static const s16     sCinemaFrameHeightTop = 44;
    static const s16     sCinemaFrameHeightBottom = 412;
    static const GXColor sCinemaFrameColor = {0, 0, 0, 255};
}; // namespace

namespace {
    NEW_NERVE(MoviePlayerSimpleOff, MoviePlayerSimple, SimpleOff);
    NEW_NERVE(MoviePlayerSimpleOpen, MoviePlayerSimple, Open);
    NEW_NERVE(MoviePlayerSimplePreload, MoviePlayerSimple, Preload);
    NEW_NERVE(MoviePlayerSimplePlaying, MoviePlayerSimple, Playing);
    NEW_NERVE(MoviePlayerSimpleSuspend, MoviePlayerSimple, SimpleSuspend);
}; // namespace

MoviePlayerSimple::MoviePlayerSimple()
    : LayoutActor("ムービープレイヤー", true),
      JKRDisposer(),
      mMovie(nullptr),
      mPlayerWrapper(nullptr),
      _44(false),
      _45(false) {
    mMovie = new Movie();
    mMovie->mMovieName = "";
    mMovie->mBuffer = nullptr;
    mMovie->mCurrentFrame = -1;
    mMovie->_18 = false;
    mMovie->_19 = false;
    mMovie->mFrameRateDefault = 1;
    mMovie->_20 = 0;
    mMovie->_24 = 0;
}

MoviePlayerSimple::~MoviePlayerSimple() {
    SingletonHolder< GameSystem >::get()->mHomeButtonStateNotifier->unregisterMoviePlayerSimple(this);
    mPlayerWrapper->quit();
    delete mPlayerWrapper;
    JKRHeap::destroy(mHeap);
}

void MoviePlayerSimple::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMovie(this);

    mPlayerWrapper = new THPSimplePlayerWrapper("THPシンプルプレイヤーのラッパ");
    mPlayerWrapper->init(0);

    mHeap = JKRExpHeap::create(calcNeedMemoryForMovieWorks(), MR::getCurrentHeap(), true);

    initNerve(&MoviePlayerSimpleOff::sInstance);
    kill();
}

void MoviePlayerSimple::draw() const {
    if (!isNerve(&MoviePlayerSimplePlaying::sInstance) && !isNerve(&MoviePlayerSimpleSuspend::sInstance)) {
        GXColor fillColor;
        fillColor.r = 0;
        fillColor.g = 0;
        fillColor.b = 0;
        fillColor.a = 255;

        MR::fillScreen(fillColor);
    } else {
        drawCinemaFrame();

        u32 frameBufferWidth;

        if (MR::isScreen16Per9()) {
            frameBufferWidth = MR::getFrameBufferWidth();
        } else {
            frameBufferWidth = 832;
        }

        u32 ySize = mMovie->mVideoInfo.ySize;
        u32 v4 = (MR::getFrameBufferWidth() - frameBufferWidth) / 2;
        u32 v5 = (MR::getFrameBufferHeight() - ySize) / 2;
        THPGXSetTexObjFilter(GX_LINEAR);
        mMovie->mCurrentFrame = mPlayerWrapper->drawCurrentFrame(JUTVideo::sManager->mRenderModeObj, v4, v5, frameBufferWidth, ySize);
    }
}

void MoviePlayerSimple::startMovie(const char* pMovieName, bool a2) {
    if (!MR::isFileExist(pMovieName, false)) {
        OSPanic(__FILE__, 0xDB, "Moive File NOT found.");
    }

    mMovie->mMovieName = pMovieName;
    mMovie->_18 = a2;
    mMovie->_24 = 0;

    play(a2);
    appear();
    MR::requestMovementOn(this);
    SingletonHolder< GameSystem >::get()->mHomeButtonStateNotifier->registerMoviePlayerSimple(this);
}

void MoviePlayerSimple::stopMovie() {
    stop();
    kill();
    setNerve(&::MoviePlayerSimpleOff::sInstance);
}

bool MoviePlayerSimple::isMovieActive() const {
    if (MR::isDead(this)) {
        return false;
    }

    return isNerve(&MoviePlayerSimpleOpen::sInstance) || isNerve(&MoviePlayerSimplePreload::sInstance) || isNerve(&MoviePlayerSimplePlaying::sInstance) || isNerve(&MoviePlayerSimpleSuspend::sInstance);
}

bool MoviePlayerSimple::isMoviePlaying() const {
    if (isMovieActive()) {
        return isNerve(&MoviePlayerSimplePlaying::sInstance);
    }

    return false;
}

s32 MoviePlayerSimple::getCurrentFrame() const {
    if (isMovieActive()) {
        return mMovie->mCurrentFrame;
    }

    return -1;
}

s32 MoviePlayerSimple::getTotalFrame() const {
    return mPlayerWrapper->getTotalFrame();
}

void MoviePlayerSimple::setFrameRateDefault() {
    f32 frameRate = mPlayerWrapper->getFrameRate();

    if (MR::isNearZero(frameRate - 59.94f, 0.001f)) {
        mMovie->mFrameRateDefault = 1;
    } else if (MR::isNearZero(frameRate - 29.97f, 0.001f)) {
        mMovie->mFrameRateDefault = 2;
    }
}

void MoviePlayerSimple::setVolume(f32 volume, s32 time) {
    mPlayerWrapper->setVolume(volume * 127.0f, time);
}

void MoviePlayerSimple::setUnpauseHomeButtonFlag() {
    mPlayerWrapper->setUnpauseFrameFlag();
}

void MoviePlayerSimple::exeSimpleOff() {
}

void MoviePlayerSimple::exeOpen() {
    if (MR::isFirstStep(this)) {
        mPlayerWrapper->open(mMovie->mMovieName);
    }

    if (mPlayerWrapper->_9) {
        mPlayerWrapper->getVideoInfo(&mMovie->mVideoInfo);
        mMovie->_20 = 0;
        mMovie->_19 = false;
        setFrameRateDefault();
        mMovie->mBuffer = new (mHeap, 32) u8[mPlayerWrapper->calcNeedMemory()];
        mPlayerWrapper->setBuffer(mMovie->mBuffer);
        mPlayerWrapper->preLoad(_44 != false);
        setNerve(&MoviePlayerSimplePreload::sInstance);
    }
}

void MoviePlayerSimple::exePreload() {
    if (mPlayerWrapper->isPreLoading()) {
        return;
    }

    setNerve(&MoviePlayerSimplePlaying::sInstance);
    appear();
}

void MoviePlayerSimple::exePlaying() {
    if (mMovie->mCurrentFrame + 1 == mPlayerWrapper->getTotalFrame()) {
        mMovie->_24 = 0;

        if (!mMovie->_18) {
            stop();
            setNerve(&MoviePlayerSimpleOff::sInstance);
            return;
        }
    }

    if (!mMovie->_19) {
        s32 result = mPlayerWrapper->decode(0);

        switch (result) {
        case 0:
            mMovie->_19 = true;
            break;
        case 2:
            mMovie->_24++;
            break;
        case 3:
            return;
        }
    }

    mMovie->_20++;

    if (mMovie->_19) {
        if (mMovie->mFrameRateDefault <= mMovie->_20) {
            mMovie->_20 = 0;
            mMovie->_19 = false;
        }
    }
}

void MoviePlayerSimple::exeSimpleSuspend() {
}

void MoviePlayerSimple::control() {
    mPlayerWrapper->updateNerve();
}

void MoviePlayerSimple::play(bool a1) {
    _44 = a1;
    setNerve(&MoviePlayerSimpleOpen::sInstance);
    MR::startStarPointerModeDemo(this);
    _45 = GameSystemFunction::setPermissionToCheckWiiRemoteConnectAndScreenDimming(false);
}

void MoviePlayerSimple::stop() {
    mPlayerWrapper->loadStop();
    mPlayerWrapper->close();
    delete[] mMovie->mBuffer;
    mMovie->mBuffer = nullptr;
    MR::endStarPointerMode(this);
    GameSystemFunction::setPermissionToCheckWiiRemoteConnectAndScreenDimming(_45);
}

u32 MoviePlayerSimple::calcNeedMemoryForMovieWorks() {
    u32 needMemory = 0;
    needMemory += 0x140000;

    s32 texCount = mPlayerWrapper->getUseTextureCount();

    needMemory += OSRoundUp32B(texCount * (MR::getFrameBufferWidth() * MR::getFrameBufferHeight()));
    needMemory += OSRoundUp32B(texCount * (MR::getFrameBufferWidth() * MR::getFrameBufferHeight() / 4));
    needMemory += OSRoundUp32B(texCount * (MR::getFrameBufferWidth() * MR::getFrameBufferHeight() / 4));

    return needMemory + 0x1188;
}

void MoviePlayerSimple::drawCinemaFrame() const {
    MR::fillScreenSetup(sCinemaFrameColor);

    s16    width = MR::getFrameBufferWidth();
    TBox2s top(0, 0, width, sCinemaFrameHeightTop);
    MR::fillScreenArea(top._0, top._8);

    s16    height = MR::getFrameBufferHeight();
    TBox2s bottom(0, sCinemaFrameHeightBottom, width, height);
    MR::fillScreenArea(bottom._0, bottom._8);
}
