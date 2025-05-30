#include "Game/Screen/MoviePlayerSimple.hpp"
#include "Game/System/HomeButtonStateNotifier.hpp"

HomeButtonStateNotifier::HomeButtonStateNotifier() :
    _0(false),
    mMoviePlayer(NULL)
{}

void HomeButtonStateNotifier::update(bool param1) {
    if ((_0 || !param1) && _0 && !param1) {
        notifyHomeButtonDeactive();
    }

    _0 = param1;
}

void HomeButtonStateNotifier::registerMoviePlayerSimple(MoviePlayerSimple* pMoviePlayer) {
    mMoviePlayer = pMoviePlayer;
}

void HomeButtonStateNotifier::unregisterMoviePlayerSimple(MoviePlayerSimple* pMoviePlayer) {
    mMoviePlayer = NULL;
}

void HomeButtonStateNotifier::notifyHomeButtonDeactive() {
    if (mMoviePlayer == NULL) {
        return;
    }

    if (!mMoviePlayer->isMovieActive()) {
        return;
    }

    mMoviePlayer->setUnpauseHomeButtonFlag();
}
