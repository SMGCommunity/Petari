#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/MoviePlayerSimple.hpp"
#include "Game/Util/ScreenUtil.hpp"

namespace {
    MoviePlayerSimple* getMoviePlayer() {
        return MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayerSimple);
    }
};

namespace MR {
    void startMoviePlayer(const char *pThpFile) {
        getMoviePlayer()->startMovie(pThpFile, false);
        GameSceneFunction::requestPlayMovieDemo();
    }

    void stopMoviePlayer() {
        getMoviePlayer()->stopMovie();
    }

    bool isActiveMoviePlayer() {
        if (!MR::isExistSceneObj(SceneObj_MoviePlayerSimple)) {
            return false;
        }

        return getMoviePlayer()->isMovieActive();
    }

    bool isMoviePlayerPlaying() {
        if (!MR::isActiveMoviePlayer()) {
            return false;
        }

        return getMoviePlayer()->isMoviePlaying();
    }

    s32 getMovieCurrentFrame() {
        if (!MR::isActiveMoviePlayer()) {
            return -1;
        }

        return getMoviePlayer()->getCurrentFrame();
    }

    u32 getMovieTotalFrame() {
        return getMoviePlayer()->getTotalFrame();
    }

    void setMovieVolume(f32 volume, s32 time) {
        getMoviePlayer()->setVolume(volume, time);
    }
};
