#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class Binder;
class MultiEmitter;
class ResourceHolder;
class Triangle;
class LiveActor;

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

    void changeEffectName(const char*, const char*);

    void enableSort();
    void playEmitterOffClipped();
    void stopEmitterOnClipped();

    void clear();

    MultiEmitter* getEmitter(const char*) const;
    MultiEmitter* createEmitter(const char*);
    void deleteEmitter(const char*);
    void forceDeleteEmitter(const char*);
    void deleteEmitterAll();
    void forceDeleteEmitterAll();

    bool isRegisteredEmitter(const char*) const;

    void update();

    void onDraw();
    void offDraw();

    void updateFloorCode(const Triangle*);

    const char* _0;
    ResourceHolder* mResourceHolder;  // 0x4
    const char* mResourceName;        // 0x8
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u8 _30;
};
