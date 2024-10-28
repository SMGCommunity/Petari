#include "Game/Player/MarioAnimator.hpp"
#include "Game/Animation/XanimeResource.hpp"
#include "Game/Player/MarioActor.hpp"

#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Player/MarioAnimatorData.hpp"

MarioAnimator::MarioAnimator(MarioActor *actor) : MarioModule(actor)
{
    init();
}

void MarioAnimator::init()
{
    XanimeSwapTable *luigiAnimations = nullptr;
    if (gIsLuigi) {
        luigiAnimations = luigiAnimeSwapTable;
    }
    mResourceTable = new XanimeResourceTable(MR::getResourceHolder(mActor), marioAnimeTable, marioAnimeAuxTable, marioAnimeOfsTable, &singleAnimeTable[0].parent, &doubleAnimeTable[0].parent, &tripleAnimeTable[0].parent, &quadAnimeTable[0].parent, luigiAnimations);

    _14 = 0;
    _15 = 0;
    _16 = 1;

    _18 = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    _24 = 1.0f;

    PSMTXIdentity(_28.toMtxPtr());
    PSMTXIdentity(_7C.toMtxPtr());
    PSMTXIdentity(_AC.toMtxPtr());

    _58 = 0.0f;
    _5C = 0.0f;
    _60.zero();

    _6C = false;
    _10C = 0;
    _10D = 0;
    mUpperDefaultSet = false;
    mCurrBck = nullptr;
    _118 = 0.0f;
    _70 = 0.0f;
    _74 = 0;
    _78 = 0;
    _110 = 0.0f;

    initCallbackTable();

    mXanimePlayer = new XanimePlayer(MR::getJ3DModel(mActor), mResourceTable);

    changeDefault("基本");
    change("基本");

    mXanimePlayer->getCore()->enableJointTransform(MR::getJ3DModelData(mActor));

    mActor->mModelManager->mXanimePlayer = mXanimePlayer;
    mXanimePlayerUpper = new XanimePlayer(MR::getJ3DModel(mActor), mResourceTable, mXanimePlayer);
    changeDefaultUpper("基本");
    mXanimePlayerUpper->changeAnimation("基本");
    mXanimePlayerUpper->mCore->shareJointTransform(mXanimePlayer->mCore);
    PSMTXCopy(MR::tmpMtxRotYRad(3.14159274101f), _DC.toMtxPtr());
}

bool MarioAnimator::isAnimationStop() const
{
    return mXanimePlayer->mCurrentAnimation == mXanimePlayer->mDefaultAnimation;
}

void MarioAnimator::change(const char *name)
{

    if (mActor->_B90) {
        return;
    }

    if (!isTeresaClear()) {
        mXanimePlayer->changeAnimation(name);
    }

    const char *bck = mXanimePlayer->getCurrentBckName();
    if (bck) {
        const XanimeGroupInfo *info = mXanimePlayer->mCurrentAnimation;
        if (info->_18 == 2) {
            f32 arg1 = info->_14, arg2 = info->_10;
            getPlayer()->startBas(bck, false, arg1, arg2);
        }
        else {
            getPlayer()->startBas(bck, false, 0.0f, 0.0f);
        }
        mActor->setBlink(bck);
    }
    else {
        getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);
        mActor->setBlink(nullptr);
    }
    mActor->changeSpecialModeAnimation(name);
    mCurrBck = bck;
    entryCallback(name);
}

void MarioAnimator::changeUpper(const char *name)
{
    mXanimePlayerUpper->changeAnimation(name);
    _6C = true;
}

void MarioAnimator::changeDefault(const char *name)
{
    getPlayer()->startBas(nullptr, false, 0.0f, 0.0f);

    mXanimePlayer->setDefaultAnimation(name);
}

void MarioAnimator::changeDefaultUpper(const char *name)
{
    if (name) {
        mUpperDefaultSet = true;
        mXanimePlayerUpper->setDefaultAnimation(name);
    }
    else {
        mUpperDefaultSet = false;
    }
}
