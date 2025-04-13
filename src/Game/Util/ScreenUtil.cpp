#include "Game/Util/ScreenUtil.hpp"
#include "Game/Screen/MoviePlayerSimple.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/GameSceneFunction.hpp"

namespace MR {
    void startMoviePlayer(const char *pThpFile) {
        MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayer)->startMovie(pThpFile, false);
        GameSceneFunction::requestPlayMovieDemo();
    }

    void stopMoviePlayer() {
        MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayer)->stopMovie();
    }

    bool isActiveMoviePlayer() {
        if (!MR::isExistSceneObj(SceneObj_MoviePlayer)) {
            return false;
        }

        return MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayer)->isMovieActive();
    }

    bool isMoviePlayerPlaying() {
        if (!MR::isActiveMoviePlayer()) {
            return false;
        }

        return MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayer)->isMoviePlaying();
    }

    u32 getMovieCurrentFrame() {
        if (!MR::isActiveMoviePlayer()) {
            return -1;
        }

        return MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayer)->getCurrentFrame();
    }

    u32 getMovieTotalFrame() {
        return MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayer)->getTotalFrame();
    }

    void setMovieVolume(f32 volume, s32 time) {
        MR::getSceneObj<MoviePlayerSimple*>(SceneObj_MoviePlayer)->setVolume(volume, time);
    }
};
