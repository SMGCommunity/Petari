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
    NEW_NERVE(MoviePlayerSimpleOff, MoviePlayerSimple, SimpleOff);
    NEW_NERVE(MoviePlayerSimpleOpen, MoviePlayerSimple, Open);
    NEW_NERVE(MoviePlayerSimplePreload, MoviePlayerSimple, Preload);
    NEW_NERVE(MoviePlayerSimplePlaying, MoviePlayerSimple, Playing);
    NEW_NERVE(MoviePlayerSimpleSuspend, MoviePlayerSimple, SimpleSuspend);
};

MoviePlayerSimple::MoviePlayerSimple() : LayoutActor("ムービープレイヤー", true), JKRDisposer() {
    mMovie = nullptr;
    mPlayerWrapper = nullptr;
    _44 = false;
    _45 = false;
    mMovie = new Movie();
    mMovie->mMovieName = "";
    mMovie->mBuffer = nullptr;
    mMovie->mCurrentFrame = -1;
    mMovie->_18 = 0;
    mMovie->_19 = 0;
    mMovie->mFrameRateDefault = 1;
    mMovie->_20 = 0;
    mMovie->_24 = 0;
}

MoviePlayerSimple::~MoviePlayerSimple() {
    SingletonHolder<GameSystem>::get()->mHomeButtonStateNotifier->unregisterMoviePlayerSimple(this);
    mPlayerWrapper->quit();
    delete mPlayerWrapper;
    JKRHeap::destroy(mHeap);
}

void MoviePlayerSimple::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMovie(this);
    mPlayerWrapper = new THPSimplePlayerWrapper("THPシンプルプレイヤーのラッパ");
    mPlayerWrapper->init(0);
    mHeap = JKRExpHeap::create(calcNeedMemoryForMovieWorks(), MR::getCurrentHeap(), true);
    initNerve(&MoviePlayerSimpleOff::sInstance);
    kill();
}

void MoviePlayerSimple::draw() const {
    if (!isNerve(&MoviePlayerSimplePlaying::sInstance) && !isNerve(&MoviePlayerSimpleSuspend::sInstance)) {
        GXColor color;
        color.r = 0;
        color.g = 0;
        color.b = 0;
        color.a = 0xFF;
        MR::fillScreen(color);
    }
    else {
        drawCinemaFrame();
        u32 frameBufferWidth;
        if (MR::isScreen16Per9()) {
            frameBufferWidth = MR::getFrameBufferWidth();
        }
        else {
            frameBufferWidth = 832;
        }

        u32 ySize = mMovie->mVideoInfo.ySize;
        u32 v4 = (MR::getFrameBufferWidth() - frameBufferWidth) >> 1u;
        u32 v5 = (MR::getFrameBufferHeight() - ySize) >> 1u;
        THPGXSetTexObjFilter(GX_LINEAR);
        mMovie->mCurrentFrame = mPlayerWrapper->drawCurrentFrame(JUTVideo::sManager->mRenderModeObj, v4, v5, frameBufferWidth, ySize);
    }
}

void MoviePlayerSimple::startMovie(const char *pMovieName, bool a2) {
    if (!MR::isFileExist(pMovieName, false)) {
        OSPanic(__FILE__, 0xDB, "Moive File NOT found.");
    }

    mMovie->mMovieName = pMovieName;
    mMovie->_18 = a2;
    mMovie->_24 = 0;
    play(a2);
    appear();
    MR::requestMovementOn(this);
    SingletonHolder<GameSystem>::get()->mHomeButtonStateNotifier->registerMoviePlayerSimple(this);
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

    return isNerve(&MoviePlayerSimpleOpen::sInstance) 
        || isNerve(&MoviePlayerSimplePreload::sInstance)
        || isNerve(&MoviePlayerSimplePlaying::sInstance)
        || isNerve(&MoviePlayerSimpleSuspend::sInstance);
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
    if (MR::isNearZero(frameRate - 59.939999f, 0.001f)) {
        mMovie->mFrameRateDefault = 1;
    }
    else if (MR::isNearZero(frameRate - 29.969999f, 0.001f)) {
        mMovie->mFrameRateDefault = 2;
    }
}

void MoviePlayerSimple::setVolume(f32 volume, s32 time) {
    mPlayerWrapper->setVolume(127.0f * volume, time);
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
        mMovie->_19 = 0;
        setFrameRateDefault();
        mMovie->mBuffer = new(mHeap, 32) u8[mPlayerWrapper->calcNeedMemory()];
        mPlayerWrapper->setBuffer(mMovie->mBuffer);
        mPlayerWrapper->preLoad(_44 != false);
        setNerve(&MoviePlayerSimplePreload::sInstance);
    }
}

void MoviePlayerSimple::exePreload() {
    if (!mPlayerWrapper->isPreLoading()) {
        setNerve(&MoviePlayerSimplePlaying::sInstance);
        appear();
    }
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
                mMovie->_19 = 1;
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
            mMovie->_19 = 0;
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
    int ret = 0;
    s32 width;
    s32 height;
    s32 temp;
    ret += 0x140000;
    s32 texCount = mPlayerWrapper->getUseTextureCount();
    
    height = MR::getFrameBufferHeight();
    width = MR::getFrameBufferWidth();
    temp = (texCount * width);
    ret += ((temp * height + 31) & ~31);
    height = MR::getFrameBufferHeight();
    width = MR::getFrameBufferWidth();
    ret  += ((texCount * (width * height / 4) + 31) & ~31);
    height = MR::getFrameBufferHeight();
    width = MR::getFrameBufferWidth();
    ret += ((texCount * (width * height / 4) + 31) & ~31);
    return ret + 0x1188;
}

GXColor cinemaColor = { 0, 0, 0, 0xFF };

void MoviePlayerSimple::drawCinemaFrame() const {
    MR::fillScreenSetup(cinemaColor);
    s16 width = MR::getFrameBufferWidth();
    TBox2s box(0, 0, width, 44);
    MR::fillScreenArea(box._0, box._8);
    s16 height = MR::getFrameBufferHeight();
    TBox2s box_1(0, 412, width, height);
    MR::fillScreenArea(box_1._0, box_1._8);
}
