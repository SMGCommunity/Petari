#pragma once

#include <revolution.h>
#include "Game/Animation/AnmPlayer.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"

class ActorLightCtrl;
class BckCtrlData;
class BrkCtrl;
class BtkCtrl;
class CollisionParts;
class CollisionPartsFilterBase;
class HitSensor;
class LiveActor;
class LiveActorGroup;
class LodCtrl;
class ModelObj;
class MsgSharedGroup;
class Nerve;
class PartsModel;
class ProjmapEffectMtxSetter;
class ResTIMG;
class ResourceHolder;
class TexMtxCtrl;
class TriangleFilterBase;

namespace MR {
    enum CollisionScaleType { 
        AutoEqualScale = 0, 
        NoScale = 1, 
        UNKNOWN_2 = 2, 
        UNKNOWN_3 = 3 
    };

    bool isExistIndirectTexture(const LiveActor*);

    bool isAnyAnimStopped(const LiveActor*, const char*);

    void validateClipping(LiveActor*);
    void invalidateClipping(LiveActor*);
    void setClippingTypeSphere(LiveActor*, f32);
    void setClippingTypeSphere(LiveActor*, f32, const TVec3f*);
    void setClippingFarMax(LiveActor*);
    void setClippingFar50m(LiveActor*);
    void setClippingFar200m(LiveActor*);
    void startBtk(const LiveActor*, const char*);

    void setBaseScale(LiveActor*, const TVec3f&);

    MsgSharedGroup* joinToGroupArray(LiveActor*, const JMapInfoIter&, const char*, s32);
    LiveActorGroup* getGroupFromArray(const LiveActor*);

    void copyTransRotateScale(const LiveActor*, LiveActor*);

    void initDefaultPos(LiveActor*, const JMapInfoIter&);
    void initDefaultPosNoRepeat(LiveActor*, const JMapInfoIter&);
    bool isValidMovement(const LiveActor*);
    bool isValidCalcAnim(const LiveActor*);
    bool isValidCalcViewAndEntry(const LiveActor*);
    bool isValidDraw(const LiveActor*);

    bool isValidCollisionParts(LiveActor*);
    void setCollisionMtx(LiveActor*);
    void setCollisionMtx(LiveActor *, CollisionParts *);

    void invalidateClipping(LiveActor*);

    bool isClipped(const LiveActor*);
    bool isInvalidClipping(const LiveActor*);

    ResourceHolder* getResourceHolder(const LiveActor*);
    ResourceHolder* getModelResourceHolder(const LiveActor*);

    void setBinderCollisionPartsFilter(LiveActor*, CollisionPartsFilterBase*);
    void setBinderExceptActor(LiveActor*, const LiveActor*);
    void setBindTriangleFilter(LiveActor*, TriangleFilterBase*);
    bool isExistBinder(const LiveActor*);
    void onEntryDrawBuffer(LiveActor*);
    void offEntryDrawBuffer(LiveActor*);
    bool isDead(const LiveActor*);
    bool isHiddenModel(const LiveActor*);
    void showModel(LiveActor*);
    void hideModel(LiveActor*);
    void hideModelAndOnCalcAnim(LiveActor*);
    void showModelIfHidden(LiveActor*);
    void hideModelIfShown(LiveActor*);
    void hideModelAndOnCalcAnimIfShown(LiveActor*);
    void stopAnimFrame(LiveActor*);
    void releaseAnimFrame(LiveActor*);
    bool isNoCalcAnim(const LiveActor*);
    void onCalcAnim(LiveActor*);
    void offCalcAnim(LiveActor*);
    bool isNoCalcView(const LiveActor*);
    bool isNoEntryDrawBuffer(const LiveActor*);
    bool isNoBind(const LiveActor*);
    void onBind(LiveActor*);
    void offBind(LiveActor*);
    bool isCalcGravity(const LiveActor*);
    void onCalcGravity(LiveActor*);
    void offCalcGravity(LiveActor*);
    void joinToGroup(LiveActor*, const char*);

    const char* getModelResName(const LiveActor*);

    void calcGravityOrZero(LiveActor*);

    void calcAnimDirect(LiveActor*);

    void calcGravity(LiveActor*);
    void calcGravity(LiveActor*, const TVec3f&);

    void offCalcShadow(LiveActor*, const char*);

    void zeroVelocity(LiveActor*);

    void initLightCtrl(LiveActor*);
    void initLightCtrlForPlayer(LiveActor*);
    void initLightCtrlNoDrawEnemy(LiveActor*);
    void initLightCtrlNoDrawMapObj(LiveActor*);
    void updateLightCtrl(LiveActor*);
    void updateLightCtrlDirect(LiveActor*);
    void loadActorLight(const LiveActor*);
    void calcLightPos0(TVec3f*, const LiveActor*);
    void calcLightPos1(TVec3f*, const LiveActor*);
    const GXColor* getLightAmbientColor(const LiveActor*);
    ActorLightCtrl* getLightCtrl(const LiveActor*);
    bool isStep(const LiveActor*, s32) NO_INLINE;
    bool isFirstStep(const LiveActor*);
    bool isLessStep(const LiveActor*, s32);
    bool isLessEqualStep(const LiveActor*, s32);
    bool isGreaterStep(const LiveActor*, s32);
    bool isGreaterEqualStep(const LiveActor*, s32);
    bool isIntervalStep(const LiveActor*, s32);
    bool isNewNerve(const LiveActor*);
    f32 calcNerveRate(const LiveActor*, s32);
    f32 calcNerveRate(const LiveActor*, s32, s32);
    f32 calcNerveEaseInRate(const LiveActor*, s32);
    f32 calcNerveEaseOutRate(const LiveActor*, s32);
    f32 calcNerveEaseOutRate(const LiveActor*, s32, s32);
    f32 calcNerveEaseInOutRate(const LiveActor*, s32);
    f32 calcNerveEaseInOutRate(const LiveActor*, s32, s32);
    f32 calcNerveValue(const LiveActor*, s32, f32, f32);
    f32 calcNerveValue(const LiveActor*, s32, s32, f32, f32);
    f32 calcNerveEaseInValue(const LiveActor*, s32, f32, f32);
    f32 calcNerveEaseInValue(const LiveActor*, s32, s32, f32, f32);
    f32 calcNerveEaseOutValue(const LiveActor*, s32, f32, f32);
    f32 calcNerveEaseInOutValue(const LiveActor*, s32, f32, f32);
    f32 calcNerveEaseInOutValue(const LiveActor*, s32, s32, f32, f32);
    void setNerveAtStep(LiveActor*, const Nerve*, s32);
    void setNerveAtBckStopped(LiveActor*, const Nerve*);
    bool trySetNerve(LiveActor*, const Nerve*);
    const TVec3f* getGroundNormal(const LiveActor*);
    const TVec3f* getWallNormal(const LiveActor*);
    const TVec3f* getRoofNormal(const LiveActor*);
    const TVec3f* getBindedNormal(const LiveActor*);

    void initFur(LiveActor *);
    void initFurPlanet(LiveActor *);
    void initFurPlayer(LiveActor *);
    void initCollisionParts(LiveActor*, const char*, HitSensor*, MtxPtr);

    void initCollisionPartsAutoEqualScale(LiveActor*, const char*, HitSensor*, MtxPtr);

    void setBaseTRMtx(LiveActor*, MtxPtr);
    void setBaseTRMtx(LiveActor*, const TQuat4f&);
    void setBaseTRMtx(LiveActor*, const TPos3f&);

    void setClippingFar(LiveActor*, f32);
    void setClippingFar100m(LiveActor*);

    void setClippingTypeSphereContainsModelBoundingBox(LiveActor*, f32);

    void validateCollisionParts(LiveActor*);
    void validateCollisionParts(CollisionParts*);

    void invalidateCollisionParts(LiveActor*);
    void invalidateCollisionParts(CollisionParts*);

    void offUpdateCollisionParts(LiveActor*);

    void onUpdateCollisionPartsOnetimeImmediately(LiveActor*);

    bool isExistCollisionParts(const LiveActor*);

    void resetAllCollisionMtx(LiveActor*);

    void startAllAnim(const LiveActor*, const char*);
    bool tryStartAllAnim(const LiveActor*, const char*);

    bool isAnyAnimOneTimeAndStopped(const LiveActor*, const char*);

    bool isBrkOneTimeAndStopped(const LiveActor*);

    bool tryStartBckAndBtp(const LiveActor *, const char *, const char *);
    void setAllAnimFrame(const LiveActor*, const char*, f32);
    void setAllAnimFrameAndStop(const LiveActor *, const char *, f32);
    void setAllAnimFrameAtEnd(const LiveActor*, const char*);

    void startBck(const LiveActor*, const char*, const char*);
    void startBckNoInterpole(const LiveActor*, const char*);
    void startBckWithInterpole(const LiveActor*, const char*, s32);
    void startBrk(const LiveActor*, const char*);
    void startBva(const LiveActor*, const char*);
    void setBvaFrameAndStop(const LiveActor*, f32);

    J3DFrameCtrl* getBckCtrl(const LiveActor*);
    J3DFrameCtrl* getBrkCtrl(const LiveActor*);
    J3DFrameCtrl* getBtkCtrl(const LiveActor*);
    J3DFrameCtrl* getBpkCtrl(const LiveActor*);
    J3DFrameCtrl* getBtpCtrl(const LiveActor*);
    J3DFrameCtrl* getBvaCtrl(const LiveActor*);

    void startBrkAndSetFrameAndStop(const LiveActor*, const char*, f32);
    bool tryStartBck(const LiveActor*, const char*, const char*);
    void setBckRate(const LiveActor*, f32);

    void setBrkRate(const LiveActor*, f32);

    void setBvaRate(const LiveActor *, f32);

    void setBckFrame(const LiveActor*, f32);
    f32 getBckFrameMax(const LiveActor*);
    f32 getBrkFrameMax(const LiveActor*);
    f32 getBtkFrameMax(const LiveActor*);
    void setBckFrameAndStop(const LiveActor*, f32);

    s16 getBrkFrameMax(const LiveActor*, const char*);

    void setBtkFrame(LiveActor*, f32);
    void setBtkFrameAndStop(const LiveActor*, f32);

    bool isExistBck(const LiveActor*, const char*);
    bool isExistBva(const LiveActor*, const char*);
    bool isExistBtk(const LiveActor*, const char*);
    bool isExistBpk(const LiveActor*, const char*);
    bool isExistBtp(const LiveActor*, const char*);
    bool isExistBrk(const LiveActor*, const char*);

    bool isBckPlaying(const LiveActor*, const char*);

    bool isBckLooped(const LiveActor *);

    bool isBckStopped(const LiveActor*);
    bool isBrkStopped(const LiveActor*);
    bool isBtkStopped(const LiveActor*);
    bool isBtpStopped(const LiveActor*);
    bool isBpkStopped(const LiveActor*);

    bool isBckOneTimeAndStopped(const LiveActor*);

    void setBrkFrame(const LiveActor*, f32);
    void setBrkFrameAndStop(const LiveActor*, f32);
    void setBrkFrameEndAndStop(const LiveActor*);

    void setBckFrameAtRandom(const LiveActor*);

    void setBaseTRMtx(LiveActor*, const TPos3f&);

    void setBpkFrame(const LiveActor*, f32);
    void stopBck(const LiveActor*);

    void startBtp(const LiveActor*, const char*);
    void setBtpFrame(const LiveActor*, f32);
    void setBtpFrameAndStop(const LiveActor*, f32);

    void startBpk(const LiveActor*, const char*);

    ProjmapEffectMtxSetter* initDLMakerProjmapEffectMtxSetter(LiveActor*);

    void newDifferedDLBuffer(LiveActor*);

    ModelObj* createModelObjMapObj(const char*, const char*, MtxPtr);
    ModelObj* createModelObjMapObjStrongLight(const char*, const char*, MtxPtr);
    ModelObj* createModelObjIndirectMapObj(const char*, const char*, MtxPtr);
    ModelObj* createModelObjNoSilhouettedMapObj(const char*, const char*, MtxPtr);
    ModelObj* createModelObjNpc(const char*, const char*, MtxPtr);
    ModelObj* createModelObjEnemy(const char *, const char *, MtxPtr);

    ModelObj* createModelObjBloomModel(const char *, const char *, MtxPtr);

    PartsModel* createPartsModelMapObj(LiveActor*, const char*, const char*, MtxPtr);

    void sendMsgToGroupMember(u32, LiveActor*, HitSensor*, const char*);

    void callAppearAllGroupMember(const LiveActor*);
    void callMakeActorDeadAllGroupMember(const LiveActor*);
    void callRequestMovementOnAllGroupMember(const LiveActor*);
    void callMakeActorAppearedAllGroupMember(const LiveActor*);

    void setGroupClipping(LiveActor*, const JMapInfoIter&, int);

    PartsModel* createPartsModelNoSilhouettedMapObj(LiveActor*, const char*, const char*, MtxPtr);

    void startAction(const LiveActor*, const char*);

    bool tryStartAction(const LiveActor *, const char *);

    LodCtrl* createLodCtrlPlanet(LiveActor*, const JMapInfoIter&, f32, s32);
    LodCtrl* createLodCtrlNPC(LiveActor*, const JMapInfoIter&);
    LodCtrl* createLodCtrlMapObj(LiveActor*, const JMapInfoIter&, f32);

    bool changeShowModelFlagSyncNearClipping(LiveActor*, f32);

    u32 createIndirectPlanetModel(LiveActor*, MtxPtr);
    bool tryCreateMirrorActor(LiveActor*, const char*);

    CollisionParts* createCollisionPartsFromLiveActor(LiveActor*, const char*, HitSensor*, CollisionScaleType);
    CollisionParts* createCollisionPartsFromLiveActor(LiveActor*, const char*, HitSensor*, MtxPtr, CollisionScaleType);

    CollisionParts* tryCreateCollisionMoveLimit(LiveActor*, MtxPtr, HitSensor*);

    bool isBinded(const LiveActor*);
    bool isBinded(const LiveActor*, HitSensor*);

    bool isBindedGround(const LiveActor*);
    bool isBindedWall(const LiveActor*);

    bool isBindedGroundIce(const LiveActor*);
    bool isBindedGroundWater(const LiveActor*);

    bool isActionEnd(const LiveActor*);

    void setMirrorReflectionInfoFromModel(LiveActor*);

    bool isBindedRoof(const LiveActor*);
    bool isOnGround(const LiveActor*);

    bool isPressedRoofAndGround(const LiveActor*);

    void setBinderExceptSensorType(LiveActor*, const TVec3f*, f32);

    void setBinderOffsetVec(LiveActor*, const TVec3f*, bool);
    void setBinderRadius(LiveActor*, f32);

    void setBinderExceptActor(LiveActor*, const LiveActor*);

    bool tryCreateCollisionAllOtherCategory(LiveActor*, MtxPtr, HitSensor*, CollisionParts**, CollisionParts**, CollisionParts**);
    bool tryCreateCollisionAllOtherCategory(LiveActor*, HitSensor*, CollisionParts**, CollisionParts**, CollisionParts**);

    f32 getCollisionBoundingSphereRange(const LiveActor*);

    bool isExistAnim(const LiveActor*, const char*);

    void setMirrorReflectionInfoFromMtxYUp(const TPos3f&);

    const char* createLowModelObjName(const LiveActor*);
    const char* createMiddleModelObjName(const LiveActor*);

    void addToAttributeGroupSearchTurtle(const LiveActor*);

    void stopSceneAtStep(const LiveActor*, s32, s32);

    void tryRumblePadAndCameraDistanceStrong(const LiveActor*, f32, f32, f32);

    void initJointTransform(const LiveActor*);

    void initCollisionPartsAutoEqualScaleOne(LiveActor*, const char*, HitSensor*, MtxPtr);
    void initCollisionPartsFromResourceHolder(LiveActor*, const char*, HitSensor*, ResourceHolder*, MtxPtr);

    ResTIMG* getTexFromArc(const char*, const LiveActor*);
    PartsModel* createBloomModel(LiveActor*, MtxPtr);

    CollisionParts* getCollisionParts(const LiveActor*);

    TexMtxCtrl* initDLMakerTexMtx(LiveActor*, const char*);

    void changeModelDataTexAll(LiveActor*, const char*, const ResTIMG&);

    void reflectBckCtrlData(LiveActor*, const BckCtrlData&);

    bool isExistKcl(LiveActor *, const char *);

    s32 countShowGroupMember(const LiveActor *);
    s32 countHideGroupMember(const LiveActor *);

    f32 getBckFrame(const LiveActor *);

    f32 calcNerveValue(const LiveActor *, s32, s32, f32, f32);

    LiveActor* getPairedGroupMember(const LiveActor *);

    TVec3f* getBindedFixReactionVector(const LiveActor *);

    CollisionParts* tryCreateCollisionSunshade(LiveActor *, HitSensor *);
};  // namespace MR
