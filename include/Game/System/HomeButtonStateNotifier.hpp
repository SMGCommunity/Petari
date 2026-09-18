#pragma once

class MoviePlayerSimple;

class HomeButtonStateNotifier {
public:
    /// @brief Creates a new `HomeButtonStateNotifier`.
    HomeButtonStateNotifier();

    void update(bool);
    void registerMoviePlayerSimple(MoviePlayerSimple* pMoviePlayer);
    void unregisterMoviePlayerSimple(MoviePlayerSimple* pMoviePlayer);
    void notifyHomeButtonDeactive();

public:
    /* 0x00 */ bool _0;
    /* 0x04 */ MoviePlayerSimple* mMoviePlayer;
};
