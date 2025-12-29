#ifndef BTE_GOEP_API_H
#define BTE_GOEP_API_H

/*******************************************************************************
 * headers
 */

#include "data_types.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT8 tGOEP_STATUS;
enum
{
	GOEP_SUCCESS,
	GOEP_ERROR,
	GOEP_RESOURCES,
	GOEP_INVALID_PARAM,
};

/*******************************************************************************
 * functions
 */

void GOEP_Init(void);
BOOLEAN GOEP_SetSecurityLevel(BOOLEAN bOrig, char *pName, UINT32 service,
                            UINT8 level, UINT8 scn);
void GOEP_SetTraceLevel(UINT8 level);
void GOEP_FreeBuf(void **p_buf);
void GOEP_SendMsg(void *p_msg);
tGOEP_STATUS GOEP_Register(char *p_name, UINT32 *phSDP, UINT8 scn,
                           UINT8 num_srv_class, UINT16 *p_service_class,
                           UINT16 profile_id, UINT16 version);
char const *GOEP_ErrorName(tGOEP_STATUS error);

#ifdef __cplusplus
	}
#endif

#endif // BTE_GOEP_API_H
