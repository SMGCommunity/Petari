#include "Game/Player/MarioHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

MarioHolder::MarioHolder() : NameObj("マリオ保持")
{
    mActor = nullptr;
}

MarioHolder::~MarioHolder() {}

void MarioHolder::setMarioActor(MarioActor *actor)
{
    mActor = actor;
}

MarioActor *MarioHolder::getMarioActor() const
{
    return mActor;
}

namespace MR {
    MarioHolder *getMarioHolder()
    {
        return (MarioHolder *)MR::getSceneObjHolder()->getObj(SceneObj_MarioHolder);
    }
}    // namespace MR
