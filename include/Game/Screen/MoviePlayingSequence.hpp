#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/Array.hpp"

class DemoPadRumbler;
class MovieSubtitles;

/// @brief The general parameters for a movie.
struct MoviePlayingInfo {
    /// @brief A pointer to the null-terminated absolute path of the associated movie file.
    /* 0x00 */ const char* mMovieName;

    /// @brief A pointer to the null-terminated absolute path of the associated movie file for Luigi.
    /* 0x04 */ const char* mMovieNameLuigi;

    /// @brief A pointer to the null-terminated name of the galaxy to enter after playback.
    /* 0x08 */ const char* mStageName;

    /// @brief A pointer to the null-terminated name of the background music to start after playback.
    /* 0x0C */ const char* mStageBgmName;

    /// @brief The number of frames to wait on a blank screen before playback.
    /* 0x10 */ s32 mPlayWaitTime;

    /// @brief The number of frames to wait on a blank screen after playback.
    /* 0x14 */ s32 mEndWaitTime;

    /// @brief The type of screen transition for the start of playback.
    /* 0x18 */ s32 mOpenWipeType;

    /// @brief The number of frames for the screen to transition at the start of playback.
    /* 0x1C */ s32 mOpenWipeTime;

    /// @brief The type of screen transition for the end of playback.
    /* 0x20 */ s32 mCloseWipeType;

    /// @brief The number of frames for the screen to transition at the end of playback.
    /* 0x24 */ s32 mCloseWipeTime;

    /// @brief The type of screen transition for after playback.
    /* 0x28 */ s32 mEndWaitWipeType;

    /// @brief The loudness of the audio within the unit interval.
    /* 0x2C */ f32 mVolume;
};

class MoviePlayingSequence : public LayoutActor {
public:
    enum MovieType {
        MovieType_PrologueA,
        MovieType_PrologueB,
        MovieType_FinalBattle,
        MovieType_EpilogueA,
        MovieType_EpilogueB,
        MovieType_EndingA,
        MovieType_EndingB,
    };

    MoviePlayingSequence(const char*, s32);

    virtual void appear();
    virtual void kill();

    static const char* getMovieName(MovieType);

    void exeWait();
    void exePlayWait();
    void exePlayStart();
    void exePlay();
    bool tryStartSubtitles();
    bool tryEnd();
    bool trySkip();
    void exeCloseWipeOnPlaying();
    void exeEndWait();

public:
    /* 0x20 */ const MoviePlayingInfo* mInfo;
    /* 0x24 */ MR::Vector< MR::AssignableArray< MovieSubtitles* > > mSubtitles;
    /* 0x30 */ DemoPadRumbler* mPadRumbler;
};

class MoviePlayingSequenceHolder : public NameObj {
public:
    /// @brief Creates a new `MoviePlayingSequenceHolder`.
    /// @param pName A pointer to the null-terminated name of the object.
    MoviePlayingSequenceHolder(const char* pName);

    MoviePlayingSequence* getSequence(int) const;
    s32 getSequenceNum() const;

public:
    /* 0x0C */ MR::AssignableArray< MoviePlayingSequence* > mSequence;
};

namespace MR {
    void createMoviePlayingSequence();
    void startMovie(int);
    bool isEndMovie(int);
    void startMovieEpilogueA();
    void startMovieEndingA();
    void startMovieEndingB();
    bool isEndMovieEpilogueA();
    bool isEndMovieEndingA();
    bool isEndMovieEndingB();
    bool isMoviePlayingOnSequence();
};  // namespace MR
