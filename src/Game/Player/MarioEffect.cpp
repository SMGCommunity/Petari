#include "JSystem/JParticle/MultiEmitterCallBack.hpp"
#include "Game/Player/MarioEffect.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Effect/ParticleEmitter.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Map/HitInfo.hpp"
#include "JSystem/JParticle/JPAEmitter.hpp"
#include "JSystem/JParticle/JPAMath.hpp"
#include <cstdio>
#include <cstring>

struct MaterialEffectEntry {
    union FlagWord {
        u32 mWord;
        struct {
            u8 mByte0;
            u8 mByte1;
            u8 mByte2;
            u8 mByte3;
        };
    };

    /* 0x00 */ const char* mName;
    /* 0x04 */ FlagWord mFlag;
    /* 0x08 */ const char* mEffects[7];
    /* 0x24 */ const char* mCurrent;
    /* 0x28 */ MultiEmitter* mEmitter;
};

struct SmokeEffectEntry {
    union FlagWord {
        u32 mWord;
        struct {
            u8 mByte0;
            u8 mByte1;
            u8 mByte2;
            u8 mByte3;
        };
    };

    /* 0x00 */ const char* mName;
    /* 0x04 */ FlagWord mType;
    /* 0x08 */ f32 mScale;
    /* 0x0C */ f32 mRate;
    /* 0x10 */ FlagWord mFlags;
    /* 0x14 */ u16 mInterval;
    /* 0x16 */ u16 mTimer;
    /* 0x18 */ u32 mHash;
    /* 0x1C */ MultiEmitter* mEmitter;
    /* 0x20 */ u8 mMaterial;
    /* 0x21 */ u8 _21[3];
};

MaterialEffectEntry cMaterialEffectTable[] = {
    { "属性尻ドロップ", 0x00000000, { "HipDropSmoke", "HipDropWater", "HipDropFlower", "HipDropSand", "HipDropSnow", "MudCircle", "HoneyCircle" }, nullptr, nullptr },
    { "属性ハチ風", 0x03000000, { "Hovering", "HoveringWater", "Hovering", "Hovering", "Hovering", "Hovering", "Hovering" }, nullptr, nullptr },
    { "属性スピン", 0x00000000, { nullptr, "WaterSpin", "FlowerSpin", "SandSpin", "SnowSpin", "MudCircle", "HoneyCircle" }, nullptr, nullptr },
    { "属性ステージイン", 0x00000000, { "StageStartGroundSmoke", nullptr, nullptr, "StageStartGroundSand", nullptr, nullptr, nullptr }, nullptr, nullptr },
    { nullptr, 0, { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }, nullptr, nullptr }
};

SmokeEffectEntry cSmokeTable[] = {
    { "共通着地普通", 0x08000000, 1.0f, 1.0f, 0x00000400, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通着地大", 0x03000000, 1.5f, 1.0f, 0x00000400, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通壁ジャンプ", 0x03000000, 1.0f, 1.0f, 0x00000300, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通砂煙レベル", 0x07000000, 0.65f, 1.0f, 0x00000000, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通壁手擦り", 0x01000000, 0.35f, 1.0f, 0x02010000, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通壁上昇", 0x01000000, 1.0f, 1.0f, 0x00000200, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通スリップ坂", 0x02000000, 0.35f, 1.0f, 0x00010000, 2, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通スリップ空転", 0x00000000, 1.0f, 1.0f, 0x00000000, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通スリップ坂制動", 0x02000000, 0.5f, 1.0f, 0x00000000, 2, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通壁ヒット着地", 0x03000000, 1.0f, 1.0f, 0x00000400, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通ダメージ着地", 0x03000000, 1.0f, 1.0f, 0x00000400, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通地上スピン", 0x03000000, 0.9f, 1.0f, 0x00000400, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通ブレーキ", 0x02000000, 0.65f, 1.0f, 0x00010000, 15, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通ハイジャンプ", 0x02000000, 0.65f, 1.0f, 0x00010000, 6, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通ブラックホール", 0x01000000, 0.65f, 1.0f, 0x03030000, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通引き戻し着地", 0x03000000, 1.2f, 1.0f, 0x00000400, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通水底接触", 0x01000000, 6.0f, 0.5f, 0x00000400, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通跳躍", 0x08000000, 1.0f, 1.0f, 0x00000C00, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通ひこうき雲", 0x03000000, 0.3f, 0.2f, 0x00010000, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { "共通ひこうきブースト", 0x00000000, 2.0f, 1.0f, 0x00000000, 0, 0, 0, nullptr, 0, { 0, 0, 0 } },
    { nullptr, 0, 0.0f, 0.0f, 0, 0, 0, 0, nullptr, 0, { 0, 0, 0 } }
};

namespace {

#pragma dont_inline on
void addMarioEffectAtJoint(LiveActor* pActor, const char* pEffectName, const char* pJointName, const char* pGroupName) {
    MtxPtr mtx = pJointName ? MR::getJointMtx(pActor, pJointName) : pActor->getBaseMtx();
    pActor->mEffectKeeper->registerEffect(pEffectName, mtx, &pActor->mScale, pGroupName, pJointName);
}
#pragma dont_inline reset

}  // namespace

void MarioEffect::execute(JPABaseEmitter* pEmitter) {
    s32 idx = 0;
    for (s32 i = 0; i < 256; i++) {
        if (_54[i]._68 == pEmitter) {
            break;
        }
        idx++;
    }
    updateFollowMtx(&_54[idx]);
}

JPABaseEmitter* MarioEffect::addRequest(const char* pEffectName, MtxPtr pMtx) {
    s32 requestIndex = 0;
    for (; requestIndex < 256; requestIndex++) {
        if (!_54[requestIndex]._68) {
            break;
        }
    }

    if (requestIndex == 256) {
        requestIndex = 0;
        for (s32 i = 1; i < 256; i++) {
            if (_54[i]._0 > _54[requestIndex]._0) {
                requestIndex = i;
            }
        }
    }

    MovingFollowMtx* target = &_54[requestIndex];

    MultiEmitter* emitter = MR::emitEffectWithEmitterCallBack(mActor, pEffectName, this);
    ParticleEmitter* particle = emitter->getParticleEmitter(0);
    if (!particle) {
        return nullptr;
    }

    JPABaseEmitter* baseEmitter = particle->mEmitter;
    target->_68 = baseEmitter;

    for (s32 i = 0; i < 256; i++) {
        if (requestIndex != i && _54[i]._68 == target->_68) {
            _54[i]._68 = nullptr;
        }
    }

    target->_0 = 0;
    target->_64 = pMtx;
    return target->_68;
}

void MarioEffect::updateFollowMtx(MovingFollowMtx* pFollow) {
    s32 isDeleteEmitter = 0;

    if (pFollow->_68->checkStatus(JPAEmtrStts_EnableDeleteEmitter) &&
        (pFollow->_68->mAlivePtclBase.getNum() + pFollow->_68->mAlivePtclChld.getNum()) == 0) {
        isDeleteEmitter = 1;
    }

    if (isDeleteEmitter) {
        pFollow->_68 = nullptr;
        return;
    }

    if (pFollow->_0 == 0) {
        Mtx temp;
        PSMTXCopy(_24.toMtxPtr(), temp);

        const TVec3f& rTrans = getTrans();
        MR::setMtxTrans(temp, rTrans.x, rTrans.y, rTrans.z);

        if (!pFollow->_64) {
            pFollow->_64 = reinterpret_cast< MtxPtr >(getPlayer()->getMoveBaseMtx());
            if (!pFollow->_64) {
                return;
            }
        }

        Mtx inv;
        PSMTXInverse(pFollow->_64, inv);
        PSMTXConcat(inv, temp, pFollow->_4.toMtxPtr());
        PSMTXCopy(temp, pFollow->_34.toMtxPtr());
    } else {
        PSMTXConcat(pFollow->_64, pFollow->_4.toMtxPtr(), pFollow->_34.toMtxPtr());
    }

    JPASetRMtxTVecfromMtx(pFollow->_34.toMtxPtr(), pFollow->_68->mGlobalRot, &pFollow->_68->mGlobalTrs);

    pFollow->_0++;
    if (pFollow->_0 == 2) {
        pFollow->_68->mStatus &= ~JPAEmtrStts_StopCalc;
        pFollow->_68->mStatus = pFollow->_68->mStatus | JPAEmtrStts_StopEmit;
        pFollow->_68->mMaxFrame = 1;
    }
}

void MarioActor::initMaterialEffect() {
    u32 entryCount = 0;
    MaterialEffectEntry* entry = cMaterialEffectTable;
    while (entry->mName) {
        for (u32 i = 0; i < 7; i++) {
            if (!entry->mEffects[i]) {
                continue;
            }

            if (!MR::isRegisteredEffect(this, entry->mEffects[i])) {
                mEffectKeeper->registerEffectWithoutSRT(entry->mEffects[i], nullptr);
            } else {
                MR::setEffectHostSRT(this, entry->mEffects[i], nullptr, nullptr, nullptr);
            }
        }

        entry->mCurrent = nullptr;
        entryCount++;
        entry->mEmitter = nullptr;
        entry++;
    }

    _BA4 = new HashSortTable(entryCount);
    entry = cMaterialEffectTable;
    while (entry->mName) {
        _BA4->add(entry->mName, reinterpret_cast<u32>(entry), false);
        entry++;
    }

    _BA4->sort();
}

s32 MarioActor::getFloorMaterialIndex(u32 flags) const {
    s32 materialIndex = 0;

    if (mMarioEffect->_10 == 0) {
        return 2;
    }

    if (_3A8 != 0) {
        return 1;
    }

    s32 floor = mMario->_960;
    if ((floor == 0x0D || floor == 0x1E) && !mMario->isCurrentFloorSand()) {
        floor = mMario->_962;
    }

    if (mMario->isStatusActive(1)) {
        if (mMario->mMovementStates._1C) {
            floor = mMario->_964[0];
        } else if (mMario->mMovementStates._5) {
            floor = mMario->_964[1];
        } else if (mMario->mMovementStates._4) {
            floor = mMario->_964[2];
        }
    }

    if (mMario->mDrawStates._C) {
        floor = 0x17;
    }

    if (mMario->mDrawStates._B) {
        floor = 0x17;
    }

    if (flags & 0x2) {
        if (mMario->mDrawStates._1) {
            floor = 0x17;
        }
    }

    if (mMario->_2D0 != 0.0f) {
        floor = 0x1A;
    }

    if (mMario->isPlayerModeBee() && MR::isWallCodeNoAction(mMario->mGroundPolygon)) {
        return -1;
    }

    switch (floor) {
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
        materialIndex = 1;
        break;
    case 0x0E:
    case 0x21:
        return -1;
    case 0x0D:
    case 0x1E:
        materialIndex = 3;
        break;
    case 0x1A:
        materialIndex = 4;
        break;
    case 0x20:
        materialIndex = 5;
        if (strcmp(MR::getSoundCodeString(mMario->_45C), "Honey") == 0) {
            materialIndex = 6;
        }
        break;
    case 0x1B:
    case 0x1C:
        if (strcmp(MR::getSoundCodeString(mMario->_45C), "Sand") == 0) {
            materialIndex = 3;
        }
        break;
    default:
        break;
    }

    if (mMario->mDrawStates._C || mMario->mDrawStates._B) {
        materialIndex = 1;
    }

    if (!mMario->mMovementStates.digitalJump) {
        if (mMario->mSwim->_1B2 != 0) {
            if (mMario->mSwim->_1B8 < 0.0f) {
                materialIndex = 1;
            }
        }
    }

    return materialIndex;
}

MultiEmitter* MarioActor::playMaterialEffect(const char* pName) {
    u32 value = 0;

    _BA4->search(pName, &value);
    MaterialEffectEntry* entry = reinterpret_cast< MaterialEffectEntry* >(value);

    const u8 flag = entry->mFlag.mByte0;
    const s32 materialIndex = getFloorMaterialIndex(flag);
    if (materialIndex == -1) {
        return nullptr;
    }

    const u32 materialOffset = static_cast< u32 >(materialIndex);
    const char* effectName = entry->mEffects[materialOffset];
    if (!effectName) {
        return nullptr;
    }

    if (flag == 1) {
        const char* current = entry->mCurrent;
        if (current && current == effectName) {
            return entry->mEmitter;
        }
    }

    MultiEmitter* emitter = MR::emitEffect(this, effectName);

    const char* current = entry->mCurrent;
    if (current && current != entry->mEffects[materialOffset]) {
        MR::deleteEffect(this, current);
    }

    entry->mCurrent = entry->mEffects[materialOffset];
    entry->mEmitter = emitter;

    if (emitter) {
        MarioEffect* marioEffect = mMarioEffect;
        if (marioEffect->_18) {
            emitter->setGlobalPrmColor(marioEffect->_1C.r, marioEffect->_1C.g, marioEffect->_1C.b, -1);
            emitter->setGlobalEnvColor(marioEffect->_20.r, marioEffect->_20.g, marioEffect->_20.b, -1);
        } else {
            emitter->setGlobalPrmColor(0xFF, 0xFF, 0xFF, -1);
            emitter->setGlobalEnvColor(0xFF, 0xFF, 0xFF, -1);
        }
    }

    return emitter;
}

void MarioActor::stopMaterialEffect(const char* pName) {
    u32 value = 0;

    _BA4->search(pName, &value);
    MaterialEffectEntry* entry = reinterpret_cast< MaterialEffectEntry* >(value);

    if (entry->mCurrent) {
        MR::deleteEffect(this, entry->mCurrent);
        entry->mCurrent = nullptr;
        entry->mEmitter = nullptr;
    }
}

void MarioActor::initCommonEffect() {

    _B9C = 0;
    _B9E = 0;
    _BA0 = new SmokeEffectEntry*[8];

    SmokeEffectEntry* entry = cSmokeTable;
    while (entry->mName) {
        const char* smokeEffects[] = { "SmokeSphere", "SmokeSphereLoop", "SmokeCircle", "SmokeCircleLoop" };
        const char* waterEffects[] = { "WaterSphere", "WaterSphereLoop", "WaterCircle", "WaterCircleLoop" };
        const char* flowerEffects[] = { "FlowerCircle", "FlowerSphereLoop", "FlowerCircle", "FlowerSphereLoop" };
        const char* sandEffects[] = { "SandCircle", "SandSphereLoop", "SandCircle", "SandSphereLoop" };
        const char* snowEffects[] = { "SnowSphere", "SnowSphereLoop", "SnowCircle", "SnowCircleLoop" };
        const char* smokeFollowEffects[] = { "SmokeSphereFollow", "SmokeSphereFollow1Time", "SmokeCircleFollow", "SmokeCircleLoopFollow" };
        const char* mudEffects[] = { "MudSphere", "MudSphereLoop", "MudCircle", "MudCircleLoop" };
        const char* honeyEffects[] = { "HoneySphere", "HoneySphereLoop", "HoneyCircle", "HoneyCircleLoop" };

        s32 variant = 0;
        s32 useFollow = 0;

        switch (entry->mType.mByte0) {
        case 0:
            variant = 0;
            break;
        case 1:
        case 2:
            variant = 1;
            break;
        case 3:
            variant = 2;
            break;
        case 4:
        case 5:
            variant = 3;
            break;
        case 6:
            useFollow = 1;
            variant = 0;
            break;
        case 7:
            useFollow = 1;
            variant = 1;
            break;
        case 8:
            useFollow = 1;
            variant = 2;
            break;
        case 9:
            useFollow = 1;
            variant = 3;
            break;
        default:
            variant = 0;
            break;
        }

        char name[0x90];
        for (u32 materialIndex = 0; materialIndex < 7; materialIndex++) {
            sprintf(name, "%s", entry->mName);

            const u8 flag2 = entry->mFlags.mByte2;
            if ((flag2 & 0x8) && materialIndex != 1) {
                continue;
            }

            const char* const* effectList;
            switch (materialIndex) {
            case 0:
                effectList = useFollow ? smokeFollowEffects : smokeEffects;
                break;
            case 1:
                effectList = waterEffects;
                break;
            case 2:
                effectList = flowerEffects;
                break;
            case 3:
                effectList = sandEffects;
                break;
            case 4:
                effectList = snowEffects;
                break;
            case 5:
                effectList = mudEffects;
                break;
            case 6:
                effectList = honeyEffects;
                break;
            }

            if (flag2 != 0) {
                const char* effectName = effectList[variant];
                mEffectKeeper->registerEffectWithoutSRT(effectName, name + materialIndex);
            } else {
                const char* effectName = effectList[variant];
                const u8 flag0 = entry->mFlags.mByte0;
                switch (flag0) {
                case 0: {
                    mEffectKeeper->registerEffectWithoutSRT(effectName, name + materialIndex);
                    const char* hostName = name + materialIndex;
                    if (hostName == nullptr) {
                        hostName = effectName;
                    }
                    MR::setEffectHostSRT(this, hostName, &mPosition, &mRotation, nullptr);
                    break;
                }
                case 1:
                    addMarioEffectAtJoint(this, effectName, "FootR", name + materialIndex);
                    break;
                case 2:
                    addMarioEffectAtJoint(this, effectName, "HandR", name + materialIndex);
                    break;
                case 3:
                    addMarioEffectAtJoint(this, effectName, "Spine1", name + materialIndex);
                    break;
                default:
                    break;
                }
            }

            if ((entry->mFlags.mByte1 & 0x1) != 0) {
                MR::getEffect(this, name + materialIndex)->forceFollowOn();
            }
            if ((entry->mFlags.mByte1 & 0x2) != 0) {
                MR::getEffect(this, name + materialIndex)->forceScaleOn();
            }
        }

        entry->mTimer = 0;
        switch (entry->mType.mByte0) {
        case 2:
        case 5: {
            _BA0[_B9E] = entry;
            _B9E++;
            break;
        }
        default:
            entry->mInterval = 0;
            break;
        }

        entry->mHash = MR::getHashCode(entry->mName);
        entry->mEmitter = nullptr;
        entry->mMaterial = 0;
        _B9C++;
        entry++;
    }
}

MultiEmitter* MarioActor::playCommonEffect(const char* pName) {
    if (mPlayerMode == 3) {
        return nullptr;
    }

    const u32 hash = MR::getHashCode(pName);
    u16 count = _B9C;
    SmokeEffectEntry* entry = cSmokeTable;

    while (count > 0) {
        if (entry->mHash == hash) {
            s32 materialIndex = getFloorMaterialIndex(0);
            MultiEmitter* emitter = nullptr;

            if (materialIndex == -1) {
                return nullptr;
            }

            if ((entry->mFlags.mByte2 & 0x8) && materialIndex != 1) {
                return nullptr;
            }

            switch (entry->mType.mByte0) {
            case 1:
            case 2:
            case 4:
            case 5:
                emitter = entry->mEmitter;
                break;
            case 7:
            case 9:
                if (materialIndex != 0) {
                    emitter = entry->mEmitter;
                }
                break;
            default:
                break;
            }

            if (emitter && materialIndex != entry->mMaterial) {
                emitter->deleteEmitter();
                emitter = nullptr;
            }

            if (!emitter) {
                s32 needsFollow = 0;
                s32 needsSRT = 0;

                if (materialIndex == 0) {
                    if (entry->mType.mByte0 >= 6) {
                        const Triangle* tri = mMario->getGroundPolygon();
                        MtxPtr prevMtx = tri->getPrevBaseMtx()->toMtxPtr();
                        MtxPtr baseMtx = mMario->getGroundPolygon()->getBaseMtx()->toMtxPtr();

                        if (!MR::isSameMtx(baseMtx, prevMtx)) {
                            needsFollow = 1;
                        }

                        needsSRT = 1;
                    }
                }

                if (needsFollow) {
                    const Triangle* tri = mMario->getGroundPolygon();
                    JPABaseEmitter* baseEmitter = mMarioEffect->addRequest(pName + materialIndex, tri->getBaseMtx()->toMtxPtr());

                    if (baseEmitter) {
                        TVec3f scale(entry->mScale, entry->mScale, entry->mScale);
                        baseEmitter->mGlobalScl.x = scale.x;
                        baseEmitter->mGlobalScl.y = scale.y;
                        baseEmitter->mGlobalScl.z = scale.z;
                        baseEmitter->mGlobalPScl.x = scale.x;
                        baseEmitter->mGlobalPScl.y = scale.y;
                    }
                } else {
                    if (needsSRT) {
                        const u8 hostFlag = entry->mFlags.mByte1;
                        const TVec3f* hostPos = nullptr;
                        const TVec3f* hostRot = nullptr;
                        const TVec3f* hostScale = nullptr;

                        if (hostFlag & 0x1) {
                            hostPos = &mPosition;
                        }

                        if (hostFlag & 0x4) {
                            hostRot = &mRotation;
                        }

                        if (hostFlag & 0x2) {
                            hostScale = &mScale;
                        }

                        MR::setEffectHostSRT(this, pName + materialIndex, hostPos, hostRot, hostScale);
                    }

                    emitter = MR::emitEffect(this, pName + materialIndex);
                }
            }

            if (!(entry->mFlags.mByte1 & 0x2)) {
                if (emitter) {
                    emitter->setGlobalScale(entry->mScale, -1);
                }
            }

            if (emitter) {
                emitter->setRate(entry->mRate, -1);
            }

            if (entry->mFlags.mByte2 & 0x1) {
                TVec3f dir(mMario->getWallNorm());
                TVec3f zero(0.0f, 0.0f, 0.0f);
                TPos3f mtx;
                TVec3f rot;
                MR::makeMtxUpNoSupportPos(&mtx, dir, zero);
                MR::makeRTFromMtxPtr(nullptr, &rot, mtx.toMtxPtr(), true);

                if (emitter) {
                    emitter->setGlobalRotationDegree(rot, -1);
                }

                if (emitter) {
                    emitter->setGlobalTranslation(mPosition, -1);
                }
            } else {
                TVec3f dir(mMario->_368);
                TVec3f zero(0.0f, 0.0f, 0.0f);
                TPos3f mtx;
                TVec3f rot;
                MR::makeMtxUpNoSupportPos(&mtx, dir, zero);
                MR::makeRTFromMtxPtr(nullptr, &rot, mtx.toMtxPtr(), true);
                PSMTXCopy(mtx.toMtxPtr(), mMarioEffect->_24.toMtxPtr());

                if (emitter) {
                    emitter->setGlobalRotationDegree(rot, -1);
                }

                if (emitter) {
                    emitter->setGlobalTranslation(mPosition, -1);
                }
            }

            if (entry->mFlags.mByte2 & 0x2) {
                TVec3f trans(mMario->_4E8);
                if (emitter) {
                    emitter->setGlobalTranslation(trans, -1);
                }
            }

            if (entry->mInterval != 0) {
                entry->mTimer = entry->mInterval;
            }

            switch (entry->mType.mByte0) {
            case 1:
            case 2:
            case 4:
            case 5:
                entry->mEmitter = emitter;
                entry->mMaterial = static_cast< u8 >(materialIndex);
                break;
            case 7:
            case 9:
                if (materialIndex != 0) {
                    entry->mEmitter = emitter;
                }
                entry->mMaterial = static_cast< u8 >(materialIndex);
                break;
            default:
                break;
            }

            if (emitter) {
                MarioEffect* marioEffect = mMarioEffect;
                if (marioEffect->_18) {
                    emitter->setGlobalPrmColor(marioEffect->_1C.r, marioEffect->_1C.g, marioEffect->_1C.b, -1);
                    emitter->setGlobalEnvColor(marioEffect->_20.r, marioEffect->_20.g, marioEffect->_20.b, -1);
                } else {
                    emitter->setGlobalPrmColor(0xFF, 0xFF, 0xFF, -1);
                    emitter->setGlobalEnvColor(0xFF, 0xFF, 0xFF, -1);
                }

                if (materialIndex == 1) {
                    emitter->setDrawOrder(3);
                }
            }

            return emitter;
        }

        entry++;
        count--;
    }

    return nullptr;
}
void MarioActor::stopCommonEffect(const char* pName) {
    const u32 hash = MR::getHashCode(pName);
    SmokeEffectEntry* entry = cSmokeTable;
    u32 i = 0;

    while (i < _B9C) {
        if (entry->mHash == hash) {
            if (entry->mEmitter) {
                entry->mEmitter->deleteEmitter();
                entry->mEmitter = nullptr;
            }
        }

        i++;
        entry++;
    }
}

MultiEmitter* MarioActor::playEffect(const char* pName) {
    s32 isBlocked = 0;
    if (_482 || _483) {
        isBlocked = 1;
    }

    if (isBlocked) {
        return nullptr;
    }

    if (isCommonEffect(pName)) {
        return playCommonEffect(pName);
    }

    if (isMaterialEffect(pName)) {
        return playMaterialEffect(pName);
    }

    return MR::emitEffect(this, pName);
}

void MarioActor::playEffectTrans(const char* pName, const TVec3f& rTrans) {
    MultiEmitter* emitter = playEffect(pName);
    if (emitter) {
        emitter->setGlobalTranslation(rTrans, -1);
    }
}

void MarioActor::playEffectRT(const char* pName, const TVec3f& rDir, const TVec3f& rTrans) {
    if (MR::isNearZero(rDir, 0.001f)) {
        return;
    }

    TVec3f zero(0.0f, 0.0f, 0.0f);
    TPos3f mtx;
    MR::makeMtxUpNoSupportPos(&mtx, rDir, zero);

    TVec3f rot;
    MR::makeRTFromMtxPtr(nullptr, &rot, mtx.toMtxPtr(), true);

    MultiEmitter* emitter = playEffect(pName);
    if (emitter) {
        emitter->forceFollowOff();
        emitter->setGlobalTranslation(rTrans, -1);
        emitter->setGlobalRotationDegree(rot, -1);
    }
}

void MarioActor::playEffectRTZ(const char* pName, const TVec3f& rDir, const TVec3f& rTrans) {
    if (MR::isNearZero(rDir, 0.001f)) {
        return;
    }

    TVec3f zero(0.0f, 0.0f, 0.0f);
    TPos3f mtx;
    MR::makeMtxUpNoSupportPos(&mtx, rDir, zero);

    TVec3f rot;
    MR::makeRTFromMtxPtr(nullptr, &rot, mtx.toMtxPtr(), true);

    MultiEmitter* emitter = playEffect(pName);
    if (emitter) {
        emitter->forceFollowOff();
        emitter->setGlobalTranslation(rTrans, -1);
        emitter->setGlobalRotationDegree(rot, -1);
    }
}

void MarioActor::playEffectRTW(const char* pName, const TVec3f& rDir, const TVec3f& rTrans) {
    if (MR::isNearZero(rDir, 0.001f)) {
        return;
    }

    TVec3f zero(0.0f, 0.0f, 0.0f);
    TPos3f mtx;
    MR::makeMtxUpNoSupportPos(&mtx, rDir, zero);

    TVec3f rot;
    MR::makeRTFromMtxPtr(nullptr, &rot, mtx.toMtxPtr(), true);

    _BB8 = rot;
    _BAC = rTrans;
    playEffect(pName);
}

void MarioActor::playEffectSRT(const char* pName, f32 scale, const TVec3f& rDir, const TVec3f& rTrans) {
    if (MR::isNearZero(rDir, 0.001f)) {
        return;
    }

    TVec3f zero(0.0f, 0.0f, 0.0f);
    TPos3f mtx;
    MR::makeMtxUpNoSupportPos(&mtx, rDir, zero);

    TVec3f rot;
    MR::makeRTFromMtxPtr(nullptr, &rot, mtx.toMtxPtr(), true);

    MultiEmitter* emitter = playEffect(pName);
    if (emitter) {
        emitter->setGlobalScale(scale, -1);
    }
    if (emitter) {
        emitter->setGlobalTranslation(rTrans, -1);
    }
    if (emitter) {
        emitter->setGlobalRotationDegree(rot, -1);
    }
}

void MarioActor::stopEffect(const char* pName) {
    if (isCommonEffect(pName)) {
        stopCommonEffect(pName);
        return;
    }

    if (isMaterialEffect(pName)) {
        stopMaterialEffect(pName);
        return;
    }

    MR::deleteEffect(this, pName);
}

void MarioActor::stopEffectForce(const char* pName) {
    if (isCommonEffect(pName)) {
        stopCommonEffect(pName);
        return;
    }

    if (isMaterialEffect(pName)) {
        stopMaterialEffect(pName);
        return;
    }

    MR::forceDeleteEffect(this, pName);
}

void MarioActor::updateEffect() {
    s32 effectA = 0;
    s32 effectB = 0;
    s32 effectC = 0;

    if (mMario->mMovementStates._1A) {
        effectA = 1;
    }

    if (mMario->mMovementStates._F && mMario->mMovementStates._1D) {
        effectA = 1;
    }

    if (mMario->mMovementStates._1D && mMario->_3F4 > 0.001f) {
        effectA = 1;
    }

    if (mMario->isStatusActive(0x14)) {
        effectA = 1;
    }

    if (mMario->mMovementStates._2A || mMario->mMovementStates._29) {
        if (mMario->_278 > 0.001f) {
            effectA = 1;
        }
    }

    if (effectA != 0) {
        effectB = 1;
        effectA = 0;
    }

    if (_3C0 || mPlayerMode == 1 || mMario->isStatusActive(6) || mMario->isStatusActive(0x12) || _934) {
        effectA = 0;
        effectB = 0;
        effectC = 0;
    }

    if (effectA) {
        playEffect("共通砂煙レベル");
    } else {
        if ((_B98 >> 31) != 0) {
            stopEffect("共通砂煙レベル");
        }
    }

    if (effectB) {
        if (((_B98 >> 30) & 1) == 0) {
            playEffect("雪煙レベル");
        }
    }

    if (!effectB) {
        if (((_B98 >> 30) & 1) != 0) {
            stopEffect("雪煙レベル");
        }
    }

    _B98 = (_B98 & 0x1FFFFFFF) | (static_cast< u32 >(effectA) << 31) | (static_cast< u32 >(effectB) << 30) |
           (static_cast< u32 >(effectC) << 29);

    effectA = 0;
    if (mMario->mMovementStates._3B && !mMario->mMovementStates._6 && mMario->mMovementStates._1D) {
        effectA = 1;
    }

    if (effectA) {
        playEffect("共通スリップ坂");
    } else {
        if (((_B98 >> 28) & 1) != 0) {
            stopEffect("共通スリップ坂");
        }
    }

    _B98 = (_B98 & ~0x10000000) | (static_cast< u32 >(effectA) << 28);

    if ((mHealth <= 1 || mWaterLife <= 1) && !MR::isDemoActive() && !MR::isPowerStarGetDemoActive() &&
        !MR::isGalaxyDarkCometAppearInCurrentStage() && MR::isPermitSE() && isEnableNerveChange()) {
        playSound("ライフ警告", -1);
    }

    s32 nearZero = 0;
    if (MR::isNearZero(_240)) {
        nearZero = 1;
    }

    if (!nearZero) {
        if (_B98 & 0x08000000) {
            playEffect("宇宙遊泳");
        }
    }

    if (nearZero) {
        if (!(_B98 & 0x08000000)) {
            stopEffect("宇宙遊泳");
        }
    }

    _B98 = (_B98 & ~0x08000000) | (static_cast< u32 >(nearZero) << 27);

    if (!isJumping() && _934 == 0) {
        MR::deleteEffect(this, "ジャンプフェアリー");
    }

    mMarioEffect->doCubeEffect();

    effectC = 0;
    if (mMario->mMovementStates._1F) {
        if (mPlayerMode == 4 && mMario->checkLvlA() && mMario->_402 != 0 && ((_37C & 3) == 0)) {
            if (mMario->mVerticalSpeed < 100.0f) {
                effectC = 1;
            } else if (mMario->mSwim->_1B2 != 0 && mMario->mSwim->_1B8 < 100.0f) {
                effectC = 1;
            }
        }
    }

    if (effectC) {
        playEffectRT("属性ハチ風", mMario->_368, mMario->mShadowPos);
    } else {
        if (_B98 & 0x04000000) {
            stopEffect("属性ハチ風");
        }
    }

    _B98 = (_B98 & ~0x04000000) | (static_cast< u32 >(effectC) << 26);

    u32 i = 0;
    while (i < _B9E) {
        SmokeEffectEntry* entry = _BA0[i];
        if (entry->mTimer != 0) {
            entry->mTimer--;
            if (entry->mTimer == 0) {
                stopEffect(entry->mName);
            }
        }

        i++;
    }
}

void MarioActor::initEffect() {
    _B98 = 0;
    initEffectKeeper(0xB8, "Mario", true);
    initCommonEffect();
    initMaterialEffect();

    MR::setEffectName(this, "SlipUp", "スリップアップ");
    MR::setEffectName(this, "WallSpark", "壁スパーク");
    MR::setEffectName(this, "Damage", "ダメージ");
    MR::setEffectName(this, "StepOn", "ふみつぶし");
    MR::setEffectName(this, "SuperSpinDriverEnd", "スーパースピンドライバ終了");
    MR::setEffectName(this, "BringBubble", "引き戻し泡");
    MR::setEffectName(this, "BringBubbleBreak", "引き戻し泡破裂");
    MR::setEffectName(this, "WaterSplashRight", "水はね右");
    MR::setEffectName(this, "WaterSplashLeft", "水はね左");
    MR::setEffectName(this, "WaterSplashSRight", "水はね右弱");
    MR::setEffectName(this, "WaterSplashSLeft", "水はね左弱");
    MR::setEffectName(this, "SpinLightBurst", "スピンライト消去");
    MR::setEffectName(this, "ElectricDamage", "ビリビリ");
    MR::setEffectName(this, "FireDamageSmoke", "炎ダメージ煙");
    MR::setEffectName(this, "BlueFireDamageSmoke", "炎ダメージ青煙");
    MR::setEffectName(this, "SpinLight", "スピンライト");
    MR::setEffectName(this, "IceMarioSpinLight", "アイススピン");
    MR::setEffectName(this, "FireMarioSpinLight", "ファイアスピン");
    MR::setEffectName(this, "BeeMarioSpinLight", "ハチスピン");
    MR::setEffectName(this, "BeeLuigiSpinLight", "ハチルイージスピン");
    MR::setEffectName(this, "Sweat", "いい汗");
    MR::setEffectName(this, "CarryStartShort", "ひろいクイック");
    MR::setEffectName(this, "AirRecover", "酸素回復");
    MR::setEffectName(this, "Invincible", "無敵中");
    MR::setEffectHostSRT(this, "無敵中", &mPosition, nullptr, nullptr);
    MR::setEffectName(this, "Ice", "アイス中");
    MR::setEffectHostSRT(this, "アイス中", &mPosition, nullptr, nullptr);
    MR::setEffectName(this, "FireBallThrow", "ファイアボール投げ");
    MR::setEffectName(this, "Throw", "こうら投げ");
    MR::setEffectName(this, "DieBuryBody", "砂埋まり体");
    MR::setEffectName(this, "DieBuryHand", "砂埋まり手");
    MR::setEffectName(this, "DieDeathMudBody", "泥埋まり体");
    MR::setEffectName(this, "DieDeathMudHand", "泥埋まり手");
    MR::setEffectName(this, "SpinLightConcentrate", "スピン許可");
    MR::setEffectName(this, "FooMarioHandGlowL", "フーマリオグロー左");
    MR::setEffectName(this, "FooMarioHandGlowR", "フーマリオグロー右");
    MR::setEffectName(this, "FooMarioBrakeLampL", "フーマリオブレーキ左");
    MR::setEffectName(this, "FooMarioBrakeLampR", "フーマリオブレーキ右");
    MR::setEffectName(this, "FooMarioEndL", "フーマリオ解除左");
    MR::setEffectName(this, "FooMarioEndR", "フーマリオ解除右");
    MR::setEffectName(this, "WallHit", "壁ヒット");
    MR::setEffectName(this, "BarrierBound", "結界ヒット");
    MR::setEffectName(this, "WaterBound", "水壁ヒット");
    MR::setEffectName(this, "IceWallJump", "氷壁ジャンプ");
    MR::setEffectHostSRT(this, "氷壁ジャンプ", nullptr, nullptr, nullptr);
    MR::setEffectName(this, "CeilingCrash", "天井ヒット");
    MR::setEffectName(this, "Coin", "コイン");
    MR::setEffectName(this, "PunchHit", "パンチヒット");
    MR::setEffectName(this, "WaterRipple", "水波紋");
    MR::setEffectName(this, "SwimWaitRipple", "水面ウエイト波紋");
    MR::setEffectHostSRT(this, "水面ウエイト波紋", &_BAC, &_BB8, nullptr);
    MR::setEffectName(this, "SwimSpinLight", "水泳スピン");
    MR::setEffectName(this, "DiveBubble", "水面Ｚ沈降");
    MR::addEffect(this, "WaterColumnS");
    MR::setEffectName(this, "WaterColumnS", "水面ジャンプ水柱");
    MR::setEffectName(this, "SurfaceWave", "水面泳ぎ");
    MR::setEffectHostSRT(this, "水面泳ぎ", &_BAC, &_BB8, nullptr);
    MR::setEffectName(this, "BodyBubble", "水中身体泡");
    MR::setEffectName(this, "MouthBubble", "水中口泡");
    MR::setEffectName(this, "SeaSmoke", "水底接触");
    MR::setEffectName(this, "Metamorphosis", "変身");
    MR::setEffectName(this, "MetamorphosisEnd", "変身解除");
    MR::setEffectName(this, "BeeFlowerPollen", "ハチ花くっつき");
    MR::setEffectHostSRT(this, "ハチ花くっつき", nullptr, nullptr, nullptr);
    MR::setEffectName(this, "SkateL", "スケート左");
    MR::setEffectName(this, "SkateR", "スケート右");
    MR::setEffectName(this, "IceJumpLand", "スケート着地");

    MR::setEffectHostSRT(this, "天井ヒット", nullptr, nullptr, nullptr);
    MR::setEffectHostSRT(this, "壁ヒット", nullptr, nullptr, nullptr);
    MR::setEffectHostSRT(this, "パンチヒット", nullptr, nullptr, nullptr);
    MR::setEffectHostSRT(this, "FlowerSpin", nullptr, nullptr, nullptr);
    MR::setEffectHostSRT(this, "水波紋", nullptr, nullptr, nullptr);
    MR::setEffectHostSRT(this, "結界ヒット", nullptr, nullptr, nullptr);
    MR::setEffectHostSRT(this, "水壁ヒット", nullptr, nullptr, nullptr);

    MR::setEffectName(this, "Blur", "ワープポッドブラー");
    MR::setEffectName(this, "SpinRing", "スピンリング");
    MR::setEffectName(this, "Vanish", "ブラックホール消滅");

    mEffectKeeper->registerEffect("CrestGetFlyC", _D1C.toMtxPtr(), "雪煙レベル", nullptr);

    const TVec3f* pos = &mPosition;
    const TVec3f* rot = &mRotation;
    const TVec3f* scale = &mScale;

    mEffectKeeper->registerEffect("CommonPhotonDustCircle", pos, rot, scale, "スイングフライ");
    mEffectKeeper->registerEffect("MarioSpaceDust", pos, rot, scale, "宇宙遊泳");
    mEffectKeeper->registerEffect("CrestGetFlyC", pos, rot, scale, "ジャンプフェアリー");
    mEffectKeeper->registerEffect("MiniPandaSpinLoopLeg", pos, rot, scale, "Bダッシュ");
    mEffectKeeper->registerEffect("IndTest", pos, rot, scale, "インダイレクトテスト");
    mEffectKeeper->registerEffect("TestMarioBaobabooPowerDown", _DAC.toMtxPtr(), "ラケット消去", nullptr);
    mEffectKeeper->registerEffect("TornadoChargeEnd", pos, rot, scale, "チャージ完了");
    mEffectKeeper->registerEffect("TornadoCharge", pos, rot, scale, "チャージ中");
    mEffectKeeper->registerEffect("AppleHit", pos, rot, scale, "チャージ切れ");

    MR::setEffectName(this, "LandRubber", "特殊着地");
    MR::setEffectName(this, "DieDarkMatter", "ダークマター死亡");
    MR::setEffectName(this, "HopperMarioHipDropBlur", "ホッパー尻落");
    MR::setEffectName(this, "HopperLuigiHipDropBlur", "ホッパー尻落ルイージ");
    MR::setEffectName(this, "HipDropBlur", "尻落");
    MR::setEffectName(this, "HipDropBlurLuigi", "尻落ルイージ");
    MR::setEffectName(this, "GetAgain", "アイテム再ゲット");

    mEffectKeeper->registerEffect("MarioPunchLBlur", pos, rot, scale, "パンチブラー左");
    MR::getEffect(this, "パンチブラー左")->forceFollowOn();
    mEffectKeeper->registerEffect("MarioPunchRBlur", pos, rot, scale, "パンチブラー右");
    MR::getEffect(this, "パンチブラー右")->forceFollowOn();

    mEffectKeeper->finalizeSort();

    ModelObj* obj = new ModelObj("WaterColumn", "WaterColumn", nullptr, -2, -2, -2, false);
    _BA8 = obj;
    obj->kill();
}

void MarioActor::emitEffectWaterColumn(const TVec3f& rDir, const TVec3f& rTrans) {
    if (MR::isNearZero(rDir, 0.001f)) {
        return;
    }

    _BA8->appear();
    MR::startBck(_BA8, "WaterColumn", nullptr);
    MR::startBrk(_BA8, "WaterColumn");

    TVec3f zero(0.0f, 0.0f, 0.0f);
    TVec3f rot;
    TPos3f mtx;
    MR::makeMtxUpNoSupportPos(&mtx, rDir, zero);
    MR::makeRTFromMtxPtr(nullptr, &rot, mtx.toMtxPtr(), true);

    _BA8->mPosition = rTrans;
    _BA8->mRotation = rot;
    _BA8->calcAnim();

    MR::emitEffect(_BA8, "WaterColumn");
    mMario->pushTask(&Mario::taskOnEffectCheck, 2);
}

bool MarioActor::isCommonEffect(const char* pName) const {
    if (static_cast< u8 >(pName[0]) == 0x8B) {
        if (static_cast< u8 >(pName[1]) == 0xA4) {
            return true;
        }
    }

    return false;
}

bool MarioActor::isMaterialEffect(const char* pName) const {
    if (static_cast< u8 >(pName[0]) == 0x91) {
        if (static_cast< u8 >(pName[1]) == 0xAE) {
            return true;
        }
    }

    return false;
}

MarioEffect::MarioEffect(MarioActor* pActor) : MarioModule(pActor) {
    _1C.mColor = 0xFFFFFFFFu;
    _20.mColor = 0xFFFFFFFFu;

    MovingFollowMtx* follow = _54;
    MovingFollowMtx* end = _54 + 0x100;
    do {
        PSMTXIdentity(follow->_4.toMtxPtr());
        PSMTXIdentity(follow->_34.toMtxPtr());
        follow->_64 = nullptr;
        follow->_68 = nullptr;
        follow->_0 = 0;
        ++follow;
    } while (follow < end);

    _C = -1;
    _10 = -1;
    _14 = -1;
    _18 = 0;
    _1C.set(0, 0, 0, 0);
    _20.set(0, 0, 0, 0);
    PSMTXIdentity(_24.toMtxPtr());
}

void MarioEffect::playSwingEffect() {
    if (_C == 1 && _10 == 0) {
        playEffectTrans("FlowerSpin", getShadowPos());
    }

    Mario* mario = getPlayer();
    playEffectRT("属性スピン", mario->_368, getTrans());
}

void MarioEffect::doCubeEffect() {
    Mario* mario = getPlayer();
    if (mario->mMovementStates._9) {
        return;
    }

    s32 unsetArg = -1;
    _C = unsetArg;
    _10 = unsetArg;

    AreaObj* area = MR::getAreaObj("EffectCylinder", mActor->mPosition);
    if (area) {
        s32 arg0 = MR::getAreaObjArg(area, 0);
        _10 = MR::getAreaObjArg(area, 1);
        _C = arg0;
    } else {
        _14 = unsetArg;
    }

    AreaObj* colorArea = MR::getAreaObj("SmokeEffectColorArea", mActor->mPosition);
    if (colorArea) {
        const s32 prmR = MR::getAreaObjArg(colorArea, 0);
        const s32 prmG = MR::getAreaObjArg(colorArea, 1);
        const s32 prmB = MR::getAreaObjArg(colorArea, 2);
        _1C.set(prmR, prmG, prmB, 0);

        const s32 envR = MR::getAreaObjArg(colorArea, 3);
        const s32 envG = MR::getAreaObjArg(colorArea, 4);
        const s32 envB = MR::getAreaObjArg(colorArea, 5);
        _20.set(envR, envG, envB, 0);
        _18 = 1;
    } else {
        _18 = 0;
    }

    if (getPlayer()->_10.digitalJump) {
        playSwingEffect();
        Mario* swingMario = getPlayer();
        swingMario->_10.digitalJump = false;
    }
}

bool Mario::taskOnEffectCheck(u32 flags) {
    if (flags & 0x2) {
        return mActor->checkEffectWaterColumn();
    }

    return true;
}

bool MarioActor::checkEffectWaterColumn() {
    if (MR::isBckStopped(_BA8)) {
        _BA8->kill();
        return false;
    }

    return true;
}

void MarioActor::stopSpinTicoEffect(bool force) {
    _948 = 0;
    _94C = 0;
    _94E = 0;
    _946 = 0;

    if (force) {
        stopEffectForce("スピンリング");
    } else {
        stopEffect("スピンリング");
    }
}

namespace NrvMarioActor {
INIT_NERVE(MarioActorNrvWait);
INIT_NERVE(MarioActorNrvGameOver);
INIT_NERVE(MarioActorNrvGameOverAbyss);
INIT_NERVE(MarioActorNrvGameOverAbyss2);
INIT_NERVE(MarioActorNrvGameOverFire);
INIT_NERVE(MarioActorNrvGameOverBlackHole);
INIT_NERVE(MarioActorNrvGameOverNonStop);
INIT_NERVE(MarioActorNrvGameOverSink);
INIT_NERVE(MarioActorNrvTimeWait);
INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor

MarioEffect::~MarioEffect() {
}

void MultiEmitter::setGlobalScale(f32 scale, s32 a2) {
    TVec3f scaleVec(scale, scale, scale);
    setGlobalScale(scaleVec, a2);
}
