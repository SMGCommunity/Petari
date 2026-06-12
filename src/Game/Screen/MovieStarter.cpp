#include "Game/Screen/MovieStarter.hpp"
#include "Game/Screen/MoviePlayingSequence.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

MovieStarter::MovieStarter(const char* pName) : LiveActor(pName), mMovieType(-1) {
}

void MovieStarter::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayoutMovement(this);
    MR::invalidateClipping(this);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnAppear(this, MR::Functor_Inline(this, &MovieStarter::appear));
    }

    MR::useStageSwitchWriteDead(this, rIter);
    MR::createMoviePlayingSequence();
    MR::getJMapInfoArg0WithInit(rIter, &mMovieType);
    makeActorDead();
}

void MovieStarter::appear() {
    LiveActor::appear();

    if (mMovieType == 0) {
        if (MR::isOnGameEventFlagPlayMoviePrologueA()) {
            kill();
            return;
        }

        MR::onGameEventFlagPlayMoviePrologueA();
    }

    if (mMovieType == 1) {
        MR::onGameEventFlagPlayMoviePrologueB();
    }

    MR::startMovie(mMovieType);
}

void MovieStarter::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    LiveActor::kill();
}

void MovieStarter::control() {
    if (MR::isEndMovie(mMovieType)) {
        kill();
    }
}
