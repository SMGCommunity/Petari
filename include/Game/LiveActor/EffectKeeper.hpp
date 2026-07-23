#pragma once

#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class Binder;
class HashSortTable;
class LiveActor;
class MultiEmitter;
class ResourceHolder;
class SyncBckEffectChecker;
class Triangle;
class XanimePlayer;

class EffectKeeper {
public:
    EffectKeeper(const char*, ResourceHolder*, int, const char*);

    void init(LiveActor*);
    void initAfterPlacementForAttributeEffect(MtxPtr);
    void setBinder(const Binder*);
    void addEffect(const char*, LiveActor*);
    void registerEffect(const char*, const TVec3f*, const TVec3f*, const TVec3f*, const char*);
    void registerEffect(const char*, MtxPtr, const char*, const char*);
    void registerEffect(const char*, MtxPtr, const TVec3f*, const char*, const char*);
    void registerEffectWithoutSRT(const char*, const char*);
    void registerSyncBckEffect(XanimePlayer*, const char*, const char*, s32, f32, f32, bool);
    MultiEmitter* createEmitter(const char*);
    void deleteEmitter(const char*);
    void forceDeleteEmitter(const char*);
    void deleteEmitterAll();
    void forceDeleteEmitterAll();
    MultiEmitter* getEmitter(const char*) const;
    MultiEmitter* getEmitter(s32) const;
    void changeEffectName(const char*, const char*);
    void update();
    void updateSyncBckEffect();
    void syncEffectBck(MultiEmitter*);
    void updateAttributeEffect();
    void stopEmitterOnClipped();
    void playEmitterOffClipped();
    void clear();
    void changeBck();
    void onDraw();
    void offDraw();
    void enableSort();
    void finalizeSort();
    bool isRegisteredEmitter(const char*) const;
    void checkExistenceAttributeEffect();
    void registMultiEmitter(MultiEmitter*, const char*, const char*);
    void updateFloorCode();
    void updateFloorCode(const Triangle*);
    bool isTypeAttributeEffect(const char*) const;

    /* 0x00 */ const char* _0;
    /* 0x04 */ ResourceHolder* _4; // mResourceHolder
    /* 0x08 */ const char* _8; // mResourceName
    /* 0x0C */ MR::Vector< MR::AssignableArray< MultiEmitter* > > _C;
    /* 0x18 */ HashSortTable* _18;
    /* 0x1C */ u32 _1C;
    /* 0x20 */ SyncBckEffectChecker* _20;
    /* 0x24 */ const Binder* mBinder;
    /* 0x28 */ s32 _28;
    /* 0x2C */ s32 _2C;
    /* 0x30 */ bool _30;
};
