#ifndef RVL_FACE_LIBRARY_INTERNAL_MAKE_RANDOM_FACE_H
#define RVL_FACE_LIBRARY_INTERNAL_MAKE_RANDOM_FACE_H
#include <RFLi_Types.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

void RFLi_MakeRandomFace(RFLiCharInfo* info, RFLSex sex, RFLAge age,
                         RFLRace race);
void RFLi_MakeRandomFace_Core(RFLiCharInfo* info, RFLSex sex, RFLAge age,
                              RFLRace race);
u8 RFLi_GetFacelineType(RFLSex sex, RFLAge age, RFLRace race);
u8 RFLi_GetHairType(RFLSex sex, RFLAge age, RFLRace race);
u8 RFLi_GetEyeType(RFLSex sex, RFLAge age, RFLRace race);
u8 RFLi_GetEyebrowType(RFLSex sex, RFLAge age, RFLRace race);
u8 RFLi_GetNoseType(RFLSex sex, RFLAge age, RFLRace race);
u8 RFLi_GetMouthType(RFLSex sex, RFLAge age, RFLRace race);
u8 RFLi_GetFaceTexType(RFLSex sex, RFLAge age, RFLRace race);
u8 RFLi_GetGlassType(RFLAge age);
u8 RFLi_GetFaceColor(RFLSex sex, RFLRace race);
u8 RFLi_GetHairColor(RFLAge age, RFLRace race);
u8 RFLi_GetEyeColor(RFLRace race);
u32 RFLi_GetRandU32(u32 max);

#ifdef __cplusplus
}
#endif
#endif
