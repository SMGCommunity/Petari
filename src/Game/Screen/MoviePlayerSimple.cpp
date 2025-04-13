#include "Game/Screen/MoviePlayerSimple.hpp"
#include "Game/Screen/THPSimplePlayerWrapper.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/HomeButtonStateNotifier.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/JKernel/JKRDisposer.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>

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
    _44 = 0;
    _45 = 0;
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
    SingletonHolder<GameSystem>::get()->mHomeButtonStateModif->unregisterMoviePlayerSimple(this);
    mPlayerWrapper->quit();
    delete mPlayerWrapper;
    JKRHeap::destroy(mHeap);
}

THPSimplePlayerWrapper::~THPSimplePlayerWrapper() {
    
}

void MoviePlayerSimple::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMovie(this);
    mPlayerWrapper = new THPSimplePlayerWrapper("THPシンプルプレイヤーのラッパ");
    mPlayerWrapper->init(0);
    mHeap = JKRExpHeap::create(calcNeedMemoryForMovieWorks(), MR::getCurrentHeap(), true);
    initNerve(&MoviePlayerSimpleOff::sInstance);
    kill();
}

// MoviePlayerSimple::draw

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
    SingletonHolder<GameSystem>::get()->mHomeButtonStateModif->registerMoviePlayerSimple(this);
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

void MoviePlayerSimple::exeSimpleSuspend() {
    
}
