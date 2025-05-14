#include "Game/Util/ScreenUtil.hpp"
#include "Game/Screen/MoviePlayerSimple.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/GameSceneFunction.hpp"

namespace MR {
    void startMoviePlayer(const char *pThpFile) {
        MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayerSimple)->startMovie(pThpFile, false);
        GameSceneFunction::requestPlayMovieDemo();
    }

    void stopMoviePlayer() {
        MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayerSimple)->stopMovie();
    }

    bool isActiveMoviePlayer() {
        if (!MR::isExistSceneObj(SceneObj_MoviePlayerSimple)) {
            return false;
        }

        return MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayerSimple)->isMovieActive();
    }

    bool isMoviePlayerPlaying() {
        if (!MR::isActiveMoviePlayer()) {
            return false;
        }

        return MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayerSimple)->isMoviePlaying();
    }

    s32 getMovieCurrentFrame() {
        if (!MR::isActiveMoviePlayer()) {
            return -1;
        }

        return MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayerSimple)->getCurrentFrame();
    }

    u32 getMovieTotalFrame() {
        return MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayerSimple)->getTotalFrame();
    }

    void setMovieVolume(f32 volume, s32 time) {
        MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayerSimple)->setVolume(volume, time);
    }
};
