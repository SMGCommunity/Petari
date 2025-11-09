#include "Game/Demo/DemoPadRumbler.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Player/MarioAccess.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/MoviePlayingSequence.hpp"
#include "Game/Screen/MovieSubtitles.hpp"
#include "Game/Screen/MovieSubtitlesDataTable.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    /// @brief A type of screen transition.
    enum WipeType {
        WipeType_None,
        WipeType_ForceWipeFade,
        WipeType_WipeFade,
        WipeType_SystemWipeWhiteFade,
        WipeType_WipeWhiteFade,
        WipeType_ForceWipeWhiteFade,
    };

    static const MoviePlayingInfo sInfoTable[] = {
        {
            "/MovieData/PrologueA.thp",
            nullptr,
            nullptr,
            "STM_PROLOGUE_05",
            75,
            60,
            WipeType_WipeFade,
            -1,
            WipeType_ForceWipeFade,
            -1,
            WipeType_WipeFade,
            1.0f,
        },
        {
            "/MovieData/PrologueB.thp",
            nullptr,
            "HeavensDoorGalaxy",
            nullptr,
            75,
            0,
            WipeType_WipeFade,
            -1,
            WipeType_ForceWipeFade,
            -1,
            WipeType_WipeFade,
            0.92f,
        },
        {
            "/MovieData/FinalBattle.thp",
            "/MovieData/FinalBattleLuigi.thp",
            "KoopaBattleVs3Galaxy",
            nullptr,
            0,
            60,
            WipeType_WipeFade,
            -1,
            WipeType_ForceWipeFade,
            -1,
            WipeType_WipeFade,
            1.0f,
        },
        {
            "/MovieData/EpilogueA.thp",
            "/MovieData/EpilogueALuigi.thp",
            nullptr,
            nullptr,
            0,
            120,
            WipeType_SystemWipeWhiteFade,
            90,
            WipeType_WipeWhiteFade,
            180,
            WipeType_None,
            1.0f,
        },
        {
            "/MovieData/EpilogueB.thp",
            nullptr,
            nullptr,
            nullptr,
            75,
            60,
            WipeType_WipeFade,
            -1,
            WipeType_ForceWipeFade,
            -1,
            WipeType_WipeFade,
            1.0f,
        },
        {
            "/MovieData/EndingA.thp",
            "/MovieData/EndingALuigi.thp",
            nullptr,
            nullptr,
            0,
            0,
            WipeType_WipeWhiteFade,
            180,
            WipeType_ForceWipeFade,
            -1,
            WipeType_None,
            1.0f,
        },
        {
            "/MovieData/EndingB.thp",
            nullptr,
            nullptr,
            nullptr,
            240,
            60,
            WipeType_WipeFade,
            180,
            WipeType_ForceWipeFade,
            -1,
            WipeType_WipeFade,
            1.0f,
        },
    };

    void openWipe(WipeType type, s32 time) {
        switch (type) {
        case WipeType_None:
            break;
        case WipeType_ForceWipeFade:
            MR::forceOpenWipeFade();
            break;
        case WipeType_WipeFade:
            MR::openWipeFade(time);
            break;
        case WipeType_SystemWipeWhiteFade:
            MR::openSystemWipeWhiteFade(time);
            break;
        case WipeType_WipeWhiteFade:
            MR::openWipeWhiteFade(time);
            break;
        case WipeType_ForceWipeWhiteFade:
            MR::forceOpenWipeWhiteFade();
            break;
        }
    }

    void closeWipe(WipeType type, s32 time) {
        switch (type) {
        case WipeType_None:
            break;
        case WipeType_ForceWipeFade:
            MR::forceCloseWipeFade();
            break;
        case WipeType_WipeFade:
            MR::closeWipeFade(time);
            break;
        case WipeType_SystemWipeWhiteFade:
            MR::forceCloseSystemWipeWhiteFade();
            break;
        case WipeType_WipeWhiteFade:
            MR::closeWipeWhiteFade(time);
            break;
        case WipeType_ForceWipeWhiteFade:
            MR::forceCloseWipeWhiteFade();
            break;
        }
    }

    const char* getMovieName(const MoviePlayingInfo *pInfo) NO_INLINE {
        if (MR::isPlayerLuigi() && pInfo->mMovieNameLuigi != nullptr) {
            return pInfo->mMovieNameLuigi;
        }
        else {
            return pInfo->mMovieName;
        }
    }
};

namespace NrvMoviePlayingSequence {
    NEW_NERVE(HostTypeWait, MoviePlayingSequence, Wait);
    NEW_NERVE(HostTypePlayWait, MoviePlayingSequence, PlayWait);
    NEW_NERVE(HostTypePlayStart, MoviePlayingSequence, PlayStart);
    NEW_NERVE(HostTypePlay, MoviePlayingSequence, Play);
    NEW_NERVE(HostTypeCloseWipeOnPlaying, MoviePlayingSequence, CloseWipeOnPlaying);
    NEW_NERVE(HostTypeEndWait, MoviePlayingSequence, EndWait);
};

const char* MoviePlayingSequence::getMovieName(MoviePlayingSequence::MovieType type) {
    if (type == MovieType_PrologueA) {
        return "PrologueA";
    }

    if (type == MovieType_PrologueB) {
        return "PrologueB";
    }

    if (type == MovieType_FinalBattle) {
        return "FinalBattle";
    }

    if (type == MovieType_EpilogueA) {
        return "EpilogueA";
    }

    if (type == MovieType_EndingA) {
        return "EndingA";
    }

    if (type == MovieType_EndingB) {
        return "EndingB";
    }

    return nullptr;
}

// https://decomp.me/scratch/O5orZ
MoviePlayingSequence::MoviePlayingSequence(const char *pName, s32 movieType) :
    LayoutActor(pName, true),
    mSubtitles()
{
    mInfo = &sInfoTable[movieType];
    mPadRumbler = new DemoPadRumbler(getMovieName(MovieType(movieType)));

    MR::createSceneObj(SceneObj_MoviePlayerSimple);
    MR::connectToSceneLayoutMovement(this);
    initNerve(&NrvMoviePlayingSequence::HostTypeWait::sInstance);

    s32 subtitleNum = MovieSubtitlesUtil::getSubtitlesMessageNum(mInfo->mMovieName);

    if (subtitleNum > 0) {
        mSubtitles.init(subtitleNum);

        for (s32 i = 0; i < subtitleNum; i++) {
            if (MovieSubtitlesUtil::isExistSubtitles(mInfo->mMovieName, i)) {
                const MoviePlayingInfo* pInfo = mInfo;
                MovieSubtitles* pSubtitles = new MovieSubtitles(MovieSubtitlesUtil::getSubtitlesMessageId(pInfo->mMovieName, i), MovieSubtitlesUtil::getSubtitlesAppearTime(pInfo->mMovieName, i));

                mSubtitles.push_back(pSubtitles);
            }
        }
    }

    kill();
}

void MoviePlayingSequence::appear() {
    LayoutActor::appear();
    setNerve(&NrvMoviePlayingSequence::HostTypePlayWait::sInstance);
}

void MoviePlayingSequence::kill() {
    LayoutActor::kill();
}

void MoviePlayingSequence::exeWait() {
    
}

void MoviePlayingSequence::exePlayWait() {
    if (MR::isFirstStep(this)) {
        MR::startStarPointerModeDemo(this);
        MR::stopSubBGM(60);
        MR::stopStageBGM(60);
        MR::submitSE();

        bool val;

        if (mInfo->mOpenWipeType == WipeType_SystemWipeWhiteFade) {
            val = false;
        }
        else {
            val = mInfo->mOpenWipeType != WipeType_WipeWhiteFade;
        }

        if (val) {
            MR::forceCloseWipeFade();
        }

        if (MR::isEqualStringCase(mInfo->mMovieName, "/MovieData/PrologueA.thp")) {
            MarioAccess::readyRemoteDemo();
            MR::setPlayerPosOnGroundAndWait("リスタート");
            MR::startBckPlayer("Wait", static_cast<const char*>(nullptr));
        }
    }

    if (MR::isStep(this, mInfo->mPlayWaitTime)) {
        setNerve(&NrvMoviePlayingSequence::HostTypePlayStart::sInstance);
    }
}

void MoviePlayingSequence::exePlayStart() {
    if (MR::isFirstStep(this)) {
        MR::startMoviePlayer(::getMovieName(mInfo));
        MR::tryFrameToScreenCinemaFrame();
    }
    else if (MR::isMoviePlayerPlaying()) {
        MR::setMovieVolume(0.0f, 0);
        setNerve(&NrvMoviePlayingSequence::HostTypePlay::sInstance);
    }
}

void MoviePlayingSequence::exePlay() {
    if (MR::isFirstStep(this)) {
        openWipe(WipeType(mInfo->mOpenWipeType), mInfo->mOpenWipeTime);
    }

    if (MR::isStep(this, 3)) {
        MR::setMovieVolume(mInfo->mVolume, 50);
    }

    tryStartSubtitles();
    mPadRumbler->update(MR::getMovieCurrentFrame());

    if (trySkip()) {
        return;
    }

    if (tryEnd()) {
        return;
    }
}

bool MoviePlayingSequence::tryStartSubtitles() {
    for (MovieSubtitles** pSubtitles = mSubtitles.begin(); pSubtitles != mSubtitles.end(); pSubtitles++) {
        s32 index = pSubtitles - mSubtitles.begin();

        if (MR::getMovieCurrentFrame() != MovieSubtitlesUtil::getSubtitlesStartStep(mInfo->mMovieName, index)) {
            continue;
        }

        MR::requestMovementOn(*pSubtitles);
        (*pSubtitles)->appear();

        return true;
    }

    return false;
}

bool MoviePlayingSequence::tryEnd() {
    if (mInfo->mCloseWipeTime == -1) {
        if (!MR::isActiveMoviePlayer()) {
            closeWipe(WipeType(mInfo->mCloseWipeType), mInfo->mCloseWipeTime);
            setNerve(&NrvMoviePlayingSequence::HostTypeEndWait::sInstance);

            return true;
        }

        return false;
    }
    else {
        s32 closeWipeFrame = MR::getMovieTotalFrame() - mInfo->mCloseWipeTime;

        if (closeWipeFrame <= MR::getMovieCurrentFrame()) {
            setNerve(&NrvMoviePlayingSequence::HostTypeCloseWipeOnPlaying::sInstance);

            return true;            
        }

        return false;
    }
}

bool MoviePlayingSequence::trySkip() {
    if (!MR::isActiveMoviePlayer()) {
        return false;
    }

    if (!MR::isEqualStringCase(mInfo->mMovieName, "/MovieData/FinalBattle.thp")) {
        return false;
    }

    if (!MR::isAlreadyVisitedStage("KoopaBattleVs3Galaxy")) {
        return false;
    }

    if (!MR::testCorePadButtonA(WPAD_CHAN0)) {
        return false;
    }

    MR::stopMoviePlayer();
    MR::forceCloseWipeFade();
    setNerve(&NrvMoviePlayingSequence::HostTypeEndWait::sInstance);

    return true;
}

void MoviePlayingSequence::exeCloseWipeOnPlaying() {
    if (MR::isFirstStep(this)) {
        closeWipe(WipeType(mInfo->mCloseWipeType), -1);
    }

    if (!MR::isActiveMoviePlayer()) {
        setNerve(&NrvMoviePlayingSequence::HostTypeEndWait::sInstance);
    }
}

void MoviePlayingSequence::exeEndWait() {
    bool noWipe;

    if (MR::isStep(this, mInfo->mEndWaitTime)) {
        const MoviePlayingInfo* pInfo = mInfo;

        if (MR::isEqualStringCase(pInfo->mMovieName, "/MovieData/FinalBattle.thp")) {
            MR::requestStartScenarioSelect(pInfo->mStageName);
            noWipe = true;
        }
        else if (pInfo->mStageName != nullptr) {
            MR::requestChangeStageInGameMoving(pInfo->mStageName, 1);
            noWipe = true;
        }
        else {
            noWipe = false;
        }

        if (!noWipe) {
            openWipe(WipeType(mInfo->mEndWaitWipeType), -1);
        }

        if (mInfo->mStageBgmName != nullptr) {
            MR::startStageBGM(mInfo->mStageBgmName, 0);
        }

        if (!MR::isEqualStringCase(mInfo->mMovieName, "/MovieData/PrologueB.thp")) {
            MR::permitSE();
        }

        MR::endStarPointerMode(this);

        if (MR::isEqualStringCase(mInfo->mMovieName, "/MovieData/PrologueA.thp")) {
            MarioAccess::endRemoteDemo(nullptr);
        }

        kill();
    }
}

namespace {
    MoviePlayingSequenceHolder* getMoviePlayingSequenceHolder() {
        return MR::getSceneObj<MoviePlayingSequenceHolder>(SceneObj_MoviePlayingSequenceHolder);
    }

    MoviePlayingSequence* getMoviePlayingSequence(int idx) {
        return getMoviePlayingSequenceHolder()->getSequence(idx);
    }
};

MoviePlayingSequenceHolder::MoviePlayingSequenceHolder(const char *pName) : NameObj(pName) {
    s32 diff;

    mSequence.init(7);

    for (MoviePlayingSequence** pSequence = mSequence.begin(); pSequence != mSequence.end(); pSequence++) {
        diff = pSequence - mSequence.begin();
        const char* pMovieName = MoviePlayingSequence::getMovieName(MoviePlayingSequence::MovieType(diff));

        if (pMovieName == nullptr) {
            *pSequence = nullptr;
        }
        else {
            *pSequence = new MoviePlayingSequence(pMovieName, diff);
        }
    }
}

MoviePlayingSequence* MoviePlayingSequenceHolder::getSequence(int idx) const {
    return mSequence[idx];
}

s32 MoviePlayingSequenceHolder::getSequenceNum() const {
    return mSequence.size();
}

namespace MR {
    void createMoviePlayingSequence() {
        createSceneObj(SceneObj_MoviePlayingSequenceHolder);
    }

    void startMovie(int type) {
        getMoviePlayingSequence(type)->appear();
    }

    bool isEndMovie(int type) {
        return isDead(getMoviePlayingSequence(type));
    }

    void startMovieEpilogueA() {
        startMovie(MoviePlayingSequence::MovieType_EpilogueA);
    }

    void startMovieEndingA() {
        startMovie(MoviePlayingSequence::MovieType_EndingA);
    }

    void startMovieEndingB() {
        startMovie(MoviePlayingSequence::MovieType_EndingB);
    }

    bool isEndMovieEpilogueA() {
        return isEndMovie(MoviePlayingSequence::MovieType_EpilogueA);
    }

    bool isEndMovieEndingA() {
        return isEndMovie(MoviePlayingSequence::MovieType_EndingA);
    }

    bool isEndMovieEndingB() {
        return isEndMovie(MoviePlayingSequence::MovieType_EndingB);
    }

    bool isMoviePlayingOnSequence() {
        if (!isExistSceneObj(SceneObj_MoviePlayingSequenceHolder)) {
            return false;
        }

        for (int i = 0; i < getMoviePlayingSequenceHolder()->getSequenceNum(); i++) {
            MoviePlayingSequence* pSequence = getMoviePlayingSequence(i);

            if (pSequence == nullptr) {
                continue;
            }

            if (isDead(pSequence)) {
                continue;
            }

            return true;
        }

        return false;
    }
};
