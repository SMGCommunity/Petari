#include "Game/Screen/MoviePlayingSequence.hpp"
#include "Game/Demo/DemoPadRumbler.hpp"
#include "Game/Screen/MovieSubtitles.hpp"
#include "Game/Screen/MovieSubtitlesDataTable.hpp"
#include "Game/Player/MarioAccess.hpp"
#include "Game/Util.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"

namespace {
    enum WipeType {
        Unk = 0,
        WipeType_ForceOpenWipeFade = 1,
        WipeType_OpenWipeFade = 2,
        WipeType_SystemWipeWhiteFade = 3,
        WipeType_OpenWipeWhiteFade = 4,
        WipeType_WipeWhiteFade = 5
    };

    const static MoviePlayingInfo sInfoTable[7] = {
        { "/MovieData/PrologueA.thp", nullptr, nullptr, "STM_PROLOGUE_05", 0x4B, 0x3C, WipeType_OpenWipeFade, -1, WipeType_ForceOpenWipeFade, -1, WipeType_OpenWipeFade, 1.0f },
        { "/MovieData/PrologueB.thp", nullptr, "HeavensDoorGalaxy", nullptr, 0x4B, 0, WipeType_OpenWipeFade, -1, WipeType_ForceOpenWipeFade, -1, WipeType_OpenWipeFade, 0.92000002f },
        { "/MovieData/FinalBattle.thp", "/MovieData/FinalBattleLuigi.thp", "KoopaBattleVs3Galaxy", nullptr, 0, 0x3C, WipeType_OpenWipeFade, -1, WipeType_ForceOpenWipeFade, -1, WipeType_OpenWipeFade, 1.0f },
        { "/MovieData/EpilogueA.thp", "/MovieData/EpilogueALuigi.thp", nullptr, nullptr, 0, 0x78, WipeType_SystemWipeWhiteFade, 0x5A, WipeType_OpenWipeWhiteFade, 0xB4, Unk, 1.0f },
        { "/MovieData/EpilogueB.thp", nullptr, nullptr, nullptr, 0x4B, 0x3C, WipeType_OpenWipeFade, -1, WipeType_ForceOpenWipeFade, -1, WipeType_OpenWipeFade, 1.0f },
        { "/MovieData/EndingA.thp", "/MovieData/EndingALuigi.thp", nullptr, nullptr, 0, 0, WipeType_OpenWipeWhiteFade, 0xB4, WipeType_ForceOpenWipeFade, -1, Unk, 1.0f },
        { "/MovieData/EndingB.thp", nullptr, nullptr, nullptr, 0xF0, 0x3C, WipeType_OpenWipeFade, 0xB4, WipeType_ForceOpenWipeFade, -1, WipeType_OpenWipeFade, 1.0f }
    };

    void openWipe(WipeType type, s32 time) NO_INLINE {
        switch (type) {
            case WipeType_ForceOpenWipeFade:
                MR::forceOpenWipeFade();
                break;
            case WipeType_OpenWipeFade:
                MR::openWipeFade(time);
                break;
            case WipeType_SystemWipeWhiteFade:
                MR::openSystemWipeWhiteFade(time);
                break;
            case WipeType_OpenWipeWhiteFade:
                MR::openWipeWhiteFade(time);
                break;
            case WipeType_WipeWhiteFade:
                MR::forceOpenWipeWhiteFade();
                break;
        }
    }

    void closeWipe(WipeType type, s32 time) NO_INLINE {
        switch (type) {
            case WipeType_ForceOpenWipeFade:
            MR::forceCloseWipeFade();
            break;
            case WipeType_OpenWipeFade:
                MR::closeWipeFade(time);
                break;
            case WipeType_SystemWipeWhiteFade:
                MR::forceCloseSystemWipeWhiteFade();
                break;
            case WipeType_OpenWipeWhiteFade:
                MR::closeWipeWhiteFade(time);
                break;
            case WipeType_WipeWhiteFade:
                MR::forceCloseWipeWhiteFade();
                break;
        }
    }

    // https://decomp.me/scratch/JwRGa
    const char* getMovieName(const MoviePlayingInfo *pInfo) NO_INLINE {
        if (MR::isPlayerLuigi()) {
            return pInfo->mMovieNameLuigi;
        }
        else {
            const char* name = pInfo->mMovieNameLuigi;
            if (pInfo->mMovieNameLuigi != nullptr) {
                name = pInfo->mMovieName;
            }

            return name;
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
    else if (type == MovieType_PrologueB) {
        return "PrologueB";
    }
    else if (type == MovieType_FinalBattle) {
        return "FinalBattle";
    }
    else if (type == MovieType_EpilogueA) {
        return "EpilogueA";
    }
    else if (type == MovieType_EndingA) {
        return "EndingA";
    }
    else if (type == MovieType_EndingB) {
        return "EndingB";
    }

    return nullptr;
}

// https://decomp.me/scratch/O5orZ
MoviePlayingSequence::MoviePlayingSequence(const char *pName, s32 movieType) : LayoutActor(pName, true) {
    mInfo = &sInfoTable[movieType];
    mSubtitles.mCount = 0;
    mPadRumbler = new DemoPadRumbler(MoviePlayingSequence::getMovieName((MovieType)movieType));
    MR::createSceneObj(SceneObj_MoviePlayerSimple);
    MR::connectToSceneLayoutMovement(this);
    initNerve(&NrvMoviePlayingSequence::HostTypeWait::sInstance);

    s32 subtitleNum = MovieSubtitlesUtil::getSubtitlesMessageNum(mInfo->mMovieName);

    if (subtitleNum > 0) {
        mSubtitles.mArray.mArr = new MovieSubtitles*[subtitleNum];
        mSubtitles.mArray.mMaxSize = subtitleNum;

        for (s32 i = 0; i < subtitleNum; i++) {
            if (MovieSubtitlesUtil::isExistSubtitles(mInfo->mMovieName, i)) {
                const MoviePlayingInfo* info = mInfo;
                MovieSubtitles* sub = new MovieSubtitles(MovieSubtitlesUtil::getSubtitlesMessageId(info->mMovieName, i), MovieSubtitlesUtil::getSubtitlesAppearTime(info->mMovieName, i));
                mSubtitles.assign(sub, mSubtitles.mCount++);
            }
        }
    }

    kill();
}

MoviePlayingSequence::~MoviePlayingSequence() {

}

void MoviePlayingSequence::appear() {
    LayoutActor::appear();
    setNerve(&NrvMoviePlayingSequence::HostTypePlayWait::sInstance);
}

void MoviePlayingSequence::kill() {
    LayoutActor::kill();
}

void MoviePlayingSequence::exePlayWait() {
    if (MR::isFirstStep(this)) {
        MR::startStarPointerModeDemo(this);
        MR::stopSubBGM(60);
        MR::stopStageBGM(60);
        MR::submitSE();

        bool val;

        if (mInfo->_18 == WipeType_SystemWipeWhiteFade) {
            val = false;
        }
        else {
            val = mInfo->_18 != WipeType_OpenWipeWhiteFade;
        }

        if (val) {
            MR::forceCloseWipeFade();
        }

        if (MR::isEqualStringCase(mInfo->mMovieName, "/MovieData/PrologueA.thp")) {
            MarioAccess::readyRemoteDemo();
            MR::setPlayerPosOnGroundAndWait("リスタート");
            MR::startBckPlayer("Wait", (const char *)0);
        }
    }

    if (MR::isStep(this, mInfo->_10)) {
        setNerve(&NrvMoviePlayingSequence::HostTypePlayStart::sInstance);
    }
}

void MoviePlayingSequence::exePlayStart() {
    if (MR::isFirstStep(this)) {
        MR::startMoviePlayer(::getMovieName(mInfo));
        MR::tryFrameToScreenCinemaFrame();
    }
    else if (MR::isMoviePlayerPlaying()) {
        MR::setMovieVolume(0, 0.0f);
        setNerve(&NrvMoviePlayingSequence::HostTypePlay::sInstance);
    }
}

void MoviePlayingSequence::exePlay() {
    if (MR::isFirstStep(this)) {
        ::openWipe((WipeType)mInfo->_18, mInfo->_1C);
    }

    if (MR::isStep(this, 3)) {
        MR::setMovieVolume(mInfo->mVolume, 50);
    }

    tryStartSubtitles();
    u32 cur = MR::getMovieCurrentFrame();
    mPadRumbler->update(cur);
    if (!trySkip()) {
        if (!tryEnd()) {
            return;
        }
    }
}

bool MoviePlayingSequence::tryStartSubtitles() {
    MovieSubtitles** entry = mSubtitles.mArray.mArr;

    while (entry != mSubtitles.mArray.mArr + mSubtitles.mCount) {
        s32 index = entry - mSubtitles.mArray.mArr;
        if ((s32)MR::getMovieCurrentFrame() == (s32)MovieSubtitlesUtil::getSubtitlesStartStep(mInfo->mMovieName, index)) {
            MR::requestMovementOn(*entry);
            (*entry)->appear();
            return true;
        }
        ++entry;
    }

    return false;
}

bool MoviePlayingSequence::tryEnd() {
    if (mInfo->_24 == -1) {
        if (!MR::isActiveMoviePlayer()) {
            ::closeWipe((WipeType)mInfo->_20, mInfo->_24);
            setNerve(&NrvMoviePlayingSequence::HostTypeEndWait::sInstance);
            return true;
            
        }
        else {
            return false;
        }
    }
    else {
        s32 totalFrame = MR::getMovieTotalFrame();
        s32 diff = totalFrame - mInfo->_24;
        if (diff <= (s32)MR::getMovieCurrentFrame()) {
            setNerve(&NrvMoviePlayingSequence::HostTypeCloseWipeOnPlaying::sInstance);
            return true;
            
        }
        else {
            return false;
        }
    
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

    if (!MR::testCorePadButtonA(0)) {
        return false;
    }

    MR::stopMoviePlayer();
    MR::forceCloseWipeFade();
    setNerve(&NrvMoviePlayingSequence::HostTypeEndWait::sInstance);
    return true;
}

void MoviePlayingSequence::exeCloseWipeOnPlaying() {
    if (MR::isFirstStep(this)) {
        ::closeWipe((WipeType)mInfo->_20, -1);
    }

    if (!MR::isActiveMoviePlayer()) {
        setNerve(&NrvMoviePlayingSequence::HostTypeEndWait::sInstance);
    }
}

void MoviePlayingSequence::exeEndWait() {
    bool noWipe;

    if (MR::isStep(this, mInfo->_14)) {
        const MoviePlayingInfo* info = mInfo;
        if (MR::isEqualStringCase(info->mMovieName, "/MovieData/FinalBattle.thp")) {
            MR::requestStartScenarioSelect(info->mGalaxyName);
            noWipe = true;
        }
        else {
            if (info->mGalaxyName != nullptr) {
                MR::requestChangeStageInGameMoving(info->mGalaxyName, 1);
                noWipe = true;
            }
            else {
                noWipe = false;
            }
        }

        if (!noWipe) {
            ::openWipe((WipeType)mInfo->_28, -1);
        }

        if (mInfo->mMusic) {
            MR::startStageBGM(mInfo->mMusic, 0);
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

void MoviePlayingSequence::exeWait() {
    
}

namespace {
    MoviePlayingSequence* getMoviePlayingSequence(int idx) NO_INLINE {
        MoviePlayingSequenceHolder* holder = MR::getSceneObj<MoviePlayingSequenceHolder*>(SceneObj_MoviePlayingSequenceHolder);
        return holder->mSequences.mArr[idx];
    }
};

MoviePlayingSequenceHolder::MoviePlayingSequenceHolder(const char *pName) : NameObj(pName) {
    mSequences.mArr = new MoviePlayingSequence*[7];
    mSequences.mMaxSize = 7;
    s32 diff;

    for (MoviePlayingSequence** seqs = mSequences.begin(); seqs != mSequences.end(); ++seqs) {
        diff = seqs - mSequences.mArr;
        const char* movieName = MoviePlayingSequence::getMovieName((MoviePlayingSequence::MovieType)diff);
        if (movieName == nullptr) {
            *seqs = nullptr;
        }
        else {
            *seqs = new MoviePlayingSequence(movieName, diff);
        }
    }
}

namespace MR {
    void createMoviePlayingSequence() {
        MR::createSceneObj(SceneObj_MoviePlayingSequenceHolder);
    }

    void startMovie(int idx) {
        return ::getMoviePlayingSequence(idx)->appear();
    }

    bool isEndMovie(int idx) {
        return MR::isDead(::getMoviePlayingSequence(idx));
    }

    void startMovieEpilogueA() {
        MoviePlayingSequenceHolder* holder = MR::getSceneObj<MoviePlayingSequenceHolder*>(SceneObj_MoviePlayingSequenceHolder);
        holder->mSequences.mArr[3]->appear();
    }

    void startMovieEndingA() {
        MoviePlayingSequenceHolder* holder = MR::getSceneObj<MoviePlayingSequenceHolder*>(SceneObj_MoviePlayingSequenceHolder);
        holder->mSequences.mArr[5]->appear();
    }

    void startMovieEndingB() {
        MoviePlayingSequenceHolder* holder = MR::getSceneObj<MoviePlayingSequenceHolder*>(SceneObj_MoviePlayingSequenceHolder);
        holder->mSequences.mArr[6]->appear();
    }

    bool isEndMovieEpilogueA() {
        return MR::isDead(MR::getSceneObj<MoviePlayingSequenceHolder*>(SceneObj_MoviePlayingSequenceHolder)->mSequences.mArr[3]);
    }

    bool isEndMovieEndingA() {
        return MR::isDead(MR::getSceneObj<MoviePlayingSequenceHolder*>(SceneObj_MoviePlayingSequenceHolder)->mSequences.mArr[5]);
    }

    bool isEndMovieEndingB() {
        return MR::isDead(MR::getSceneObj<MoviePlayingSequenceHolder*>(SceneObj_MoviePlayingSequenceHolder)->mSequences.mArr[6]);
    }

    bool isMoviePlayingOnSequence() {
        if (!MR::isExistSceneObj(SceneObj_MoviePlayingSequenceHolder)) {
            return false;
        }

        int i = 0;

        while (i < MR::getSceneObj<MoviePlayingSequenceHolder*>(SceneObj_MoviePlayingSequenceHolder)->mSequences.mMaxSize) {
            MoviePlayingSequence* seq = ::getMoviePlayingSequence(i);
            if (seq != nullptr && !MR::isDead(seq)) {
                return true;
            }

            i++;
        }

        return false;
    }
};

MoviePlayingSequenceHolder::~MoviePlayingSequenceHolder() {
    
}
