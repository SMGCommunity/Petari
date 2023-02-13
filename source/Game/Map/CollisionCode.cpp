#include "Game/Map/CollisionCode.h"

#include "Game/Util/HashUtil.h"

CollisionCode::CollisionCode()
    : mFloorTable(0), mWallTable(0), mSoundTable(0), mCameraTable(0) {
    
    createFloorTable();
    createWallTable();
    createSoundTable();
    createCameraTable();
}

CodeTable::CodeTable(u32 maxNum) {
    mMaxNumCodes = maxNum;
    mNumCodes = 0;
    mHashCodes = new u32[maxNum];
    _C = new u32[maxNum];
    mNames = new const char*[maxNum];
}

void CodeTable::add(const char *pName, u32 num) {
    mHashCodes[mNumCodes] = MR::getHashCode(pName);
    _C[mNumCodes] = num;
    mNames[mNumCodes] = pName;
    mNumCodes++;
}

const char* CollisionCode::getFloorCodeString(const Triangle &rTriangle) {
    JMapInfoIter iter = rTriangle.getAttributes();

    u32 code = getCode(iter, mFloorTable, "Floor_code");
    return mFloorTable->getString(code);
}

const char* CodeTable::getString(u32 val) {
    for (s32 i = 0; i < mNumCodes; i++) {
        if (val == _C[i]) {
            return mNames[i];
        }
    }

    return "NoIter";   
}

const char* CollisionCode::getWallCodeString(const Triangle &rTriangle) {
    JMapInfoIter iter = rTriangle.getAttributes();

    u32 code = getCode(iter, mWallTable, "Wall_code");
    return mWallTable->getString(code);
}

const char* CollisionCode::getSoundCodeString(const Triangle &rTriangle) {
    JMapInfoIter iter = rTriangle.getAttributes();
    return getSoundCodeString(iter);
}

const char* CollisionCode::getSoundCodeString(const JMapInfoIter &rIter) {
    if (!rIter.isValid()) {
        return "nullptr";
    }

    if (rIter.mInfo->getValueType("Sound_code") == 0) {
        u32 code = 0;

        bool ret = rIter.getValue<u32>("Sound_code", &code);

        if (ret) {
            return mSoundTable->getString(code);
        }
    }
    else {
        const char* code = 0;
        
        bool ret = rIter.getValue<const char*>("Sound_code", &code);

        if (ret) {
            return code;
        }
    }

    return "nullptr";
}

u32 CollisionCode::getCameraID(const Triangle &rTriangle) {
    JMapInfoIter iter = rTriangle.getAttributes();
    return getCameraID(iter);
}

u32 CollisionCode::getCode(const JMapInfoIter &rIter, CodeTable *pCodeTable, const char *pName) {
    if (!rIter.isValid()) {
        return 0;
    }

    if (rIter.mInfo->getValueType(pName) == 0) {
        u32 code = 0;

        bool ret = rIter.getValue<u32>(pName, &code);

        if (ret) {
            return code;
        }
    }
    else {
        const char* code = 0;

        bool ret = rIter.getValue<const char*>(pName, &code);

        if (ret) {
            u32 ret_code = 0;
            u32 hash = MR::getHashCode(code);

            for (s32 i = 0; i < pCodeTable->mNumCodes; i++) {
                if (hash == pCodeTable->mHashCodes[i]) {
                    ret_code = pCodeTable->_C[i];
                    break;
                }
            }
            
            return ret_code;
        }
    }

    return 0;
}

void CollisionCode::createFloorTable() {
    mFloorTable = new CodeTable(0x23);

    mFloorTable->add("Normal", 0);
    mFloorTable->add("Death", 1);
    mFloorTable->add("Slip", 2);
    mFloorTable->add("NoSlip", 3);
    mFloorTable->add("DamageNormal", 4);
    mFloorTable->add("Ice", 5);
    mFloorTable->add("JumpLow", 6);
    mFloorTable->add("JumpMiddle", 7);
    mFloorTable->add("JumpHigh", 8);
    mFloorTable->add("Slider", 9);
    mFloorTable->add("DamageFire", 0xA);
    mFloorTable->add("JumpNormal", 0xB);
    mFloorTable->add("FireDance", 0xC);
    mFloorTable->add("Sand", 0xD);
    mFloorTable->add("Glass", 0xE);
    mFloorTable->add("DamageElectric", 0xF);
    mFloorTable->add("PullBack", 0x10);
    mFloorTable->add("Sink", 0x11);
    mFloorTable->add("SinkPoison", 0x12);
    mFloorTable->add("Slide", 0x13);
    mFloorTable->add("WaterBottomH", 0x14);
    mFloorTable->add("WaterBottomM", 0x15);
    mFloorTable->add("WaterBottomL", 0x16);
    mFloorTable->add("Wet", 0x17);
    mFloorTable->add("Needle", 0x18);
    mFloorTable->add("SinkDeath", 0x19);
    mFloorTable->add("Snow", 0x1A);
    mFloorTable->add("RailMove", 0x1B);
    mFloorTable->add("AreaMove", 0x1C);
    mFloorTable->add("Press", 0x1D);
    mFloorTable->add("NoStampSand", 0x1E);
    mFloorTable->add("SinkDeathMud", 0x1F);
    mFloorTable->add("Brake", 0x20);
    mFloorTable->add("GlassIce", 0x21);
    mFloorTable->add("JumpParasol", 0x22);
}

void CollisionCode::createWallTable() {
    mWallTable = new CodeTable(0x9);

    mWallTable->add("Normal", 0);
    mWallTable->add("NotWallJump", 1);
    mWallTable->add("NotWallSlip", 2);
    mWallTable->add("NotGrab", 3);
    mWallTable->add("GhostThroughCode", 4);
    mWallTable->add("NotSideStep", 5);
    mWallTable->add("Rebound", 6);
    mWallTable->add("Fur", 7);
    mWallTable->add("NoAction", 8);
}

void CollisionCode::createSoundTable() {
    mSoundTable = new CodeTable(0x10);

    mSoundTable->add("nullptr", 0);
    mSoundTable->add("Soil", 1);
    mSoundTable->add("Lawn", 2);
    mSoundTable->add("Stone", 3);
    mSoundTable->add("Marble", 4);
    mSoundTable->add("WoodThick", 5);
    mSoundTable->add("WoodThin", 6);
    mSoundTable->add("Metal", 7);
    mSoundTable->add("Snow", 8);
    mSoundTable->add("Ice", 9);
    mSoundTable->add("Shallow", 0xA);
    mSoundTable->add("Sand", 0xB);
    mSoundTable->add("Beach", 0xC);
    mSoundTable->add("Carpet", 0xD);
    mSoundTable->add("Mud", 0xE);
    mSoundTable->add("Honey", 0xF);
}

void CollisionCode::createCameraTable() {
    mCameraTable = new CodeTable(2);

    mCameraTable->add("NoThrough", 0);
    mCameraTable->add("Through", 1);
}

u32 CollisionCode::getFloorCode(const JMapInfoIter &rIter) {
    return getCode(rIter, mFloorTable, "Floor_code");
}

u32 CollisionCode::getWallCode(const JMapInfoIter &rIter) {
    return getCode(rIter, mWallTable, "Wall_code");
}

u32 CollisionCode::getSoundCode(const JMapInfoIter &rIter) {
    return getCode(rIter, mSoundTable, "Sound_code");
}

u32 CollisionCode::getCameraCode(const JMapInfoIter &rIter) {
    return getCode(rIter, mCameraTable, "Camera_through");
}

u32 CollisionCode::getCameraID(const JMapInfoIter &rIter) {
    if (!rIter.isValid()) {
        return -1;
    }

    u32 id = -1;
    rIter.getValue<u32>("camera_id", &id);

    return id;
}
