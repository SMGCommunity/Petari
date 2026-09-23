#include "Game/AreaObj/SoundEmitterCube.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/AudioLib/AudMicWrap.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const char* cSoundNameTable[] = {"SE_AT_LV_GRAVE", "SE_AT_LV_KOOPA_WIND", "SE_AT_LV_WIND_DESERT", "SE_AT_LV_WIND_ICE_MT_TOP"};
}  // namespace

SoundEmitterCube::~SoundEmitterCube() {
}

SoundEmitterCube::SoundEmitterCube(int formType, const char* pName) : AreaObj(formType, pName), mSoundObj(), _40(0.0f, 0.0f, 0.0f), mSoundName() {
}

void SoundEmitterCube::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    mSoundName = ::cSoundNameTable[mObjArg0];
    mSoundObj = new AudAnmSoundObject(&_40, 4, MR::getCurrentHeap());

    MR::connectToSceneAreaObj(this);
    MR::registerDemoSimpleCastAll(this);
}

void SoundEmitterCube::movement() {
    switch (mObjArg0) {
    case 0:
    case 3:
        _40.set(MR::getCameraWatchPos());
        break;
    default:
        _40.set(AudMicWrap::getMicPos());
        break;
    }

    TDirBox3f worldBox;
    MR::calcCubeWorldBox(&worldBox, this);
    TBox3f bounds;
    bounds.i.set(worldBox._24);
    bounds.f.set(worldBox._24);
    TVec3f cornerZ;
    TVec3f cornerY;
    TVec3f cornerX;
    cornerX.scaleAdd(worldBox._30.x, worldBox._0, worldBox._24);
    cornerY.scaleAdd(worldBox._30.y, worldBox._C, worldBox._24);
    cornerZ.scaleAdd(worldBox._30.z, worldBox._18, worldBox._24);
    bounds.extend(cornerX);
    bounds.extend(cornerY);
    bounds.extend(cornerZ);

    cornerX.scaleAdd(worldBox._30.y, worldBox._C, cornerX);
    cornerY.scaleAdd(worldBox._30.z, worldBox._18, cornerY);
    cornerZ.scaleAdd(worldBox._30.x, worldBox._0, cornerZ);
    bounds.extend(cornerX);
    bounds.extend(cornerY);
    bounds.extend(cornerZ);

    cornerX.scaleAdd(worldBox._30.z, worldBox._18, cornerX);
    bounds.extend(cornerX);

    if (_40.x < bounds.i.x) {
        _40.x = bounds.i.x;
    }

    if (_40.x > bounds.f.x) {
        _40.x = bounds.f.x;
    }

    if (_40.y < bounds.i.y) {
        _40.y = bounds.i.y;
    }

    if (_40.y > bounds.f.y) {
        _40.y = bounds.f.y;
    }

    if (_40.z < bounds.i.z) {
        _40.z = bounds.i.z;
    }

    if (_40.z > bounds.f.z) {
        _40.z = bounds.f.z;
    }

    mSoundObj->process();
    MR::startSoundObjectLevel(mSoundObj, mSoundName);
}
