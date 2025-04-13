#pragma once

#include <revolution.h>

class MoviePlayerSimple;

class HomeButtonStateNotifier {
public:
    HomeButtonStateNotifier();

    void update(bool);
    void registerMoviePlayerSimple(MoviePlayerSimple *);
    void unregisterMoviePlayerSimple(MoviePlayerSimple *);
    void notifyHomeButtonDeactive();

    u8 _0;
    MoviePlayerSimple* mMoviePlayer;            // 0x04
};
