#include "Game/Screen/MovieSubtitles.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/TalkTextFormer.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvMovieSubtitles {
    NEW_NERVE(HostTypeWait, MovieSubtitles, Wait);
    NEW_NERVE(HostTypeTalk, MovieSubtitles, Talk);
    NEW_NERVE(HostTypeTalkWait, MovieSubtitles, TalkWait);
};

MovieSubtitles::MovieSubtitles(const char *pName, s32 a2) :
    LayoutActor("ムービーの字幕", true),
    mFormerText(nullptr),
    _24(a2)
{
    MR::connectToScene(this, 19, 15, -1, 64);
    initLayoutManager("CinemaSuper", 1);

    mFormerText = new TalkTextFormer(this, "Text00");
    mFormerText->formMessage(MR::getLayoutMessageDirect(pName), 2);

    initNerve(&NrvMovieSubtitles::HostTypeWait::sInstance);
    kill();
}

void MovieSubtitles::appear() {
    LayoutActor::appear();
    setNerve(&NrvMovieSubtitles::HostTypeTalk::sInstance);
}

void MovieSubtitles::exeWait() {
    
}

void MovieSubtitles::exeTalk() {
    mFormerText->updateTalking();

    if (mFormerText->isTextAppearedAll()) {
        setNerve(&NrvMovieSubtitles::HostTypeTalkWait::sInstance);
    }
}

void MovieSubtitles::exeTalkWait() {
    mFormerText->updateTalking();

    if (MR::isStep(this, _24)) {
        if (mFormerText->hasNextPage()) {
            mFormerText->nextPage();
            setNerve(&NrvMovieSubtitles::HostTypeTalk::sInstance);
        }
        else {
            kill();
        }
    }
}
